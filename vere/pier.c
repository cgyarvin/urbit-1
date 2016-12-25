/* vere/lord.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <setjmp.h>
#include <gmp.h>
#include <sigsegv.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <uv.h>
#include <errno.h>
#include <curses.h>
#include <termios.h>
#include <term.h>

#include "all.h"
#include "vere/vere.h"

  /*    event handling proceeds on two parallel paths.  on the first
  **    path, the event is processed in the child worker process (serf).
  **    state transitions are as follows:
  **
  **        generated               (event numbered and queued)
  **        dispatched              (sent to worker)
  **        computed                (completed by worker)
  **        released                (output actions allowed)
  **
  **    we dispatch one event at a time to the worker.  we don't do
  **    anything in parallel.
  **
  **    in parallel, we try to save the event.  it goes through phases:
  **      
  **        generated
  **        precommit requested
  **        precommit complete
  **        commit requested
  **        commit complete
  **   
  **    the sanity constraints that connect these two paths:
  **
  **        - an event can't request a commit until it's computed.
  **        - an event can't be released until it, and all events
  **          preceding it, are precommitted.
  **
  **    event numbers are uint64 (c3_d) which start with 1.  we order
  **    events as we receive them.
  **
  **    events are executed in order by the working process, and
  **    (at present) precommitted and committed in strict order. 
  **
  **    physically, precommits are saved to individual files, then
  **    appended to a single commit log once successfully computed.
  **
  **    the result of computing an event can be completion (in which
  **    case we go directly to commit) or replacement (in which we
  **    replace the input event with a different event).  in case of
  **    replacement, we wait for the precommit to complete, delete
  **    the precommit 
  **
  **    after crash recovery, events which were precommitted but
  **    not yet committed have at-least-once semantics in their
  **    output effects.  (not the actual changes to the arvo state,
  **    which are of course exactly-once.)  ideally all your outputs
  **    are network packets or idempotent http requests!
  */

  /* u3_writ: inbound event.
  */
    typedef struct _u3_writ {
      u3_pier*         pir_u;               //  backpointer to pier
      u3_noun          job;                 //  event source if not reloaded
      c3_d             evt_d;               //  event number
      u3_noun          wen;                 //  event time
      c3_l             msc_l;               //  ms to timeout
      c3_l             mug_l;               //  hash before executing
      u3_file*         fil_u;               //  precommit file
      u3_atom          mat;                 //  jammed $work, or 0
      u3_noun          act;                 //  action list
      struct _u3_writ* nex_u;               //  next in queue, or 0
    } u3_writ;

  /* u3_lord: working process controller.
  */
    typedef struct _u3_lord {
      uv_process_t         cub_u;           //  process handle
      uv_process_options_t ops_u;           //  process configuration
      uv_stdio_container_t cod_u[3];        //  process options
      time_t               wen_t;           //  process creation time
      u3_mojo              inn_u;           //  client's stdin
      u3_moat              out_u;           //  client's stdout
      c3_d                 sen_d;           //  last event dispatched
      c3_d                 dun_d;           //  last event completed
      c3_d                 rel_d;           //  last event released
      c3_d                 bak_d;           //  last event backed-up
      c3_l                 mug_l;           //  mug after last completion
      struct _u3_pier*     pir_u;           //  pier backpointer
    } u3_lord;

  /* u3_disk: manage events on disk.
  **
  **    any event once discovered should be in one of these sets.
  **    at present, all sets are ordered and can be defined by a
  **    simple counter.  any events <= the counter is in the set.
  */
    typedef struct _u3_disk {               
      u3_dire*         dir_u;               //  logfile directory
      u3_foil*         fol_u;               //  logfile
      c3_d             end_d;               //  byte end of file
      c3_d             rep_d;               //  precommit requested
      c3_d             pre_d;               //  precommitted
      c3_d             moc_d;               //  commit requested
      c3_d             com_d;               //  committed
      struct _u3_pier* pir_u;               //  pier backpointer
    } u3_disk;

  /* u3_pier: ship controller.
  */
    typedef struct _u3_pier {
      c3_c*            dir_c;               //  pier directory
      c3_d             gen_d;               //  last event discovered
      c3_d             key_d[4];            //  save and passkey
      u3_disk          log_u;               //  event log
      u3_lord          god_u;               //  computer
      u3_writ*         ent_u;               //  entry of queue
      u3_writ*         ext_u;               //  exit of queue
      struct _u3_lord* nex_u;               //  next in parent list
    } u3_lord;

  /* u3_king: all executing piers.
  */
    typedef struct _u3_king {
      c3_w     len_w;                       //  number of lords used
      c3_w     all_u;                       //  number of lords allocated
      u3_lord* tab_u;                       //  lord table
    } u3_king;

    static u3_king u3K;

/* _pier_disk_bail(): bail from disk i/o.
*/
static void
_pier_work_bail(void* vod_p, c3_c* err_c)
{
  // u3_writ* wit_u = vod_p;

  fprintf(stderr, "storage error: %s\r\n", err_c);
}

/* _pier_disk_precommit_complete(): save request completed.
*/
static void
_pier_disk_precommit_complete(u3_writ* wit_u,
                              u3_foil* fol_u)
{
  u3_disk* log_u = wit_u->pir_u->log_u;

  wit_u->fol_u = fol_u;

  if ( wit_u->evt_d != log_u->rep_d ) {
    /* 
    ** if this precommit is marked as not requested, it's been
    ** replaced in the event stream.
    */
    c3_assert(wit_u->evt_d == (1ULL + log_u->rep_d));

    /* delete the file; the reactor will re-request.
    */
    u3_foil_delete(fol_u);
    wit_u->fol_u = 0; 
  }
  else {
    /* advance the precommit complete pointer.
    */
    c3_assert(wit_u->evt_d == (1ULL + log_u->pre_d));
    log_u->pre_d = wit_u->evt_d;
  }
}

/* _pier_disk_precommit_request(): start save request.
*/
static void
_pier_disk_precommit_request(u3_disk* log_u,
                             u3_writ* wit_u)
{
  c3_c* nam_c;

  /* writ must be fully computed
  */
  {
    c3_assert(0 != wit_u->mat);
  }

  /* build filename
  */
  {
    c3_c  buf_c[256];

    sprintf(buf_c, "%llx-%x.urbit-log", wit_u->evt_d,
                                        u3r_mug(wit_u->mat));

    nam_c = malloc(1 + strlen(buf_c));
    strcpy(nam_c, buf_c);
  }

  /* create and write file.
  */
  {
    c3_d  len_d = u3r_met(6, wit_u->mat);
    c3_d* buf_d = c3_malloc(8 * len_d);
    
    u3_foil_invent(_pier_disk_precommit_complete,
                   wit_u,
                   log_u->dir_u,
                   nam_c,
                   buf_d,
                   len_d);
  }

  /* mark as precommitted.
  */
  pir_u->log_u.rep_d += 1;
}

/* _pier_disk_precommit_replace(): replace precommit.
*/
static void
_pier_disk_replace(u3_disk* log_u, 
                   u3_writ* wit_u)
{
  /* if the replaced event is already precommitted, 
  ** undo the precommit and delete the file.
  */
  if ( wit_u->evt_d <= log_u->pre_d ) {
    c3_assert(0 != wit_u->fol_u);
    c3_assert(wit_u->evt_d == log_u->rep_d);
    c3_assert(wit_u->evt_d == log_u->pre_d);

    log_u->rep_d -= 1ULL;
    log_u->pre_d -= 1ULL;

    u3_foil_delete(0, wit_u, wit_u->fol_u);
  } 
  else {
    /* otherwise, decrement the precommit request counter.
    ** the returning request will notice this and rerequest.
    */
    c3_assert(wit_u->evt_d == log_u->rep_d);
    log_u->rep_d -= 1ULL;
  }
}

/* _pier_disk_commit_complete(): commit complete.
*/
static void
_pier_disk_commit_complete(u3_writ* wit_u,
                           u3_foil* del_u,
                           c3_d     del_d,
                           u3_foil* unt_u,
                           c3_d     ent_d)
{
  u3_disk* log_u = &wit_u->pir_u->log_u;

  /* advance commit counter
  */
  {
    c3_assert(wit_u->evt_d == god_u->moc_d); 
    c3_assert(wit_u->evt_d == (1ULL + god_u->com_d); 
    god_u->com_d += 1ULL;
  }
}

/* _pier_disk_commit_request(): start commit.
*/
static void
_pier_disk_commit_request(u3_disk* log_u,
                          u3_writ* wit_u)
{
  u3_foil_commit(_pier_disk_commit_complete,  
                 wit_u,
                 wit_u->fol_u,
                 wit_u->fol_u->end_d,
                 log_u->fol_u,
                 log_u->fol_u->end_d);

  /* advance commit-request counter
  */
  {
    c3_assert(wit_u->evt_d == (1ULL + god_u->moc_d));
    god_u->moc_d += 1ULL;
  }
}

/* _pier_work_bail(): bail from work.
*/
static void
_pier_work_bail(void* vod_p, c3_c* err_c)
{
  // u3_writ* wit_u = vod_p;

  fprintf(stderr, "compute error: %s\r\n", err_c);
}

/* _pier_work_release(): apply side effects.
*/
static void
_pier_work_release(u3_writ* wit_u,
                   u3_foil  fol_u)
{
  u3_lord* god_u = &wit_u->pir_u->god_u;

  /* XX release actions */
  c3_assert(0);

  /* advance release counter
  */
  {
    c3_assert(wit_u->evt_d == (1ULL + god_u->rel_d));
    god_u->rel_d += 1ULL;
  }
}

/* _pier_work_build(): build atomic action.
*/
static void
_pier_work_build(u3_writ* wit_u)
{
  /* set time - XX, leap control.
  */
  if ( 0 == wit_u->now ) {
    gettimeofday(&tim_tv, 0);
    wit_u->now = u3_time_in_tv(&tim_tv);
  }

  /* marshal into atom
  */
  {
    c3_assert(0 == wit_u->mat);

    wit_u->mat = u3ke_jam(u3nq(c3__work, 
                               u3i_chubs(1, &wit_u->evt_d),
                               wit_u->mug_l,
                               u3nc(u3k(wit_u->now), u3k(wit_u->job))));
  }
}

/* _pier_work_send(): send to worker.
*/
static void
_pier_work_send(u3_lord* god_u, 
                u3_writ* wit_u)
{
  c3_assert(0 != wit_u->mat);

  u3_newt_write(&god_u->inn_u, wit_u->mat, wit_u);
}

/* _pier_work_complete(): worker reported completion.
*/
static void
_pier_work_complete(u3_lord* god_u, 
                    u3_writ* wit_u,
                    u3_noun  act)
{
  god_u->dun_d += 1;
  c3_assert(god_u->dun_d == wit_u->evt_d);

  c3_assert(wit_u->act == 0);
  wit_u->act = act;
}

/* _pier_work_replace(): worker reported replacement.
*/
static void
_pier_work_replace(u3_lord* god_u,
                   u3_writ* wit_u,
                   u3_noun  job,
                   u3_noun  mat)
{
  c3_assert(god_u->sen_d == wit_u->evt_d);

  /* move backward in work processing
  */
  {
    u3z(wit_u->job); 
    wit_u->job = job;

    u3z(wit_u->mat);
    wit_u->mat = mat;

    god_u->sen_d -= 1;
  }

  /* move backward in precommit processing
  */
  _pier_disk_precommit_replace(&god_u->pir_u->log_u, wit_u);
}

/* _pier_work_reaction(): process response from worker.
*/
static void
_pier_work_reaction(u3_lord* god_u, 
                    u3_writ* wit_u,
                    u3_atom  mat)
{
  u3_noun p_bap, q_bap, r_bap, s_bap;
  u3_noun bap = u3ke_cue(mat);

  c3_assert(c3y == u3du(bap));

  switch ( u3h(bap) ) {
    default: c3_assert(0);

    case c3__work: {
      if ( (c3y != u3r_quil(bap, 0, &p_bap, &q_bap, &r_bap, &s_bap)) ||
           (c3n != u3ud(p_bap)) || 
           (1 != u3r_met(6, p_bap)) ||
           (c3n != u3ud(q_bap)) ||
           (1 < u3r_met(5, q_bap)) ||
           (c3n != u3ud(r_bap)) ||
           (1 != u3r_met(7, q_bap)) )
      {
        c3_assert(0);
      }
      c3_assert(wit_u->evt_d == u3r_chub(0, p_bap));

      _pier_work_replace(god_u, wit_u, u3k(q_bap), u3k(mat));
    }
    break;

    case c3__done: {
      c3_d evt_d;
      c3_l mug_l;

      if ( (c3y != u3r_qual(bap, 0, &p_bap, &q_bap, &r_bap)) ||
           (c3n != u3ud(p_bap)) || 
           (1 != u3r_met(6, p_bap)) ||
           (c3n != u3ud(q_bap)) ||
           (1 < u3r_met(5, q_bap)) ) ||
      {
        c3_assert(0);
      }
      c3_assert(wit_u->evt_d == u3r_chub(0, p_bap));
      god_u->mug_l = q_bap;

      _pier_work_complete(god_u, wit_u, u3k(r_bap));
    }
    break;
  }
  u3z(bap);
  u3z(mat);
}

/* _pier_work_compute(): dispatch for processing.
*/
static void
_pier_work_compute(u3_lord* god_u, u3_writ* wit_u)
{
  u3_noun mat;

  c3_assert(wit_u->evt_d == (1 + god_u->sen_d));

  wit_u->mug_l = god_u->mug_l;

  _pier_work_build(wit_u);
  _pier_work_send(god_u, wit_u);

  god_u->sen_d += 1;
}

/* _pier_apply(): react to i/o, inbound or outbound.
*/
static void
_pier_apply(u3_pier* pir_u)
{
  u3_writ* wit_u;

  /* iterate from queue exit, advancing any writs that can advance
  */
  wit_u = pir_u->ext_u;
  while ( wit_u ) {
    /* if writ is next in line to compute, request compute
    */
    if ( pre_u->evt_d == (1 + pir_u->god_u.sen_d) ) {
      fprintf(stderr, "compute request: %llx\r\n", pre_u->evt_d);
      _pier_work_compute(&pir_u->god_u, wit_u);
    }

    /* if writ is (a) being computed and (b) next in line to
    ** precommit, request precommit
    */
    if ( (pre_u->evt_d <= pir_u->god_u.sen_d) &&
         (pre_u->evt_d == (1 + pir_u->log_u.rep_d)) ) 
    {
      fprintf(stderr, "precommit request: %llx\r\n", pre_u->evt_d);
      _pier_disk_precommit(&pir_u->log_u, wit_u);
    }

    /* if writ is (a) computed and (b) precommitted, release actions
    */
    if ( (pre_u->evt_d <= pir_u->god_u.dun_d) &&
         (pre_u->evt_d <= pir_u->log_u.pre_d) )
    {
      fprintf(stderr, "release: %llx\r\n", pre_u->evt_d);
      _pier_work_release(&pir_u->god_u, wit_u);
    }

    /* if writ is (a) released and (b) next in line to commit, 
    ** request commit.
    */
    if ( (pre_u->evt_d <= pir_u->god_u.rel_d) &&
         (pre_u->evt_d == (1 + pir_u->log_u.moc_d)) ) 
    {
      fprintf(stderr, "commit request: %llx\r\n", pre_u->evt_d);
      _pier_disk_commit(&pir_u->log_u, wit_u);
    }

    /* if writ is committed, delete from queue
    */
    if ( (pre_u->evt_d <= pir_u->log_u.com_d) ) {
      /* remove from queue; must be at end
      */
      {
        c3_assert(wit_u == pir_u->ext_u);
        pir_u->ext_u = pir_u->ext_u->nex_u;

        if ( wit_u = pir_u->ent_u ) {
          c3_assert(pir_u->ext_u == 0);
          pir_u->ent_u = 0;
        }
      }

      /* delete precommit file
      */
      u3_foil_delete(wit_u->fol_u);
      wit_u->fol_u = 0;

      /* free contents
      */
      u3z(wit_u->job);
      u3z(wit_u->mat);
      u3z(wit_u->act);

      /* free file and dequeue
      */
      {
        u3_writ nex_u = wit_u->nex_u;

        c3_free(wit_u);
        wit_u = nex_u;
      }
    }
    else {    
      /* otherwise, continue backward
      */
      wit_u = wit_u->nex_u;
    }
  }
}

/* u3_pier_discover(): insert task into process controller.
*/
void
u3_pier_discover(u3_pier* pir_u,
                 c3_l     msc_l,
                 u3_noun  job)
{
  u3_writ* wit_u = malloc(sizeof(u3_writ));
  c3_l     mug_l;

  wit_u->pir_u = pir_u;
  wit_u->evt_d = pir_u->gen_d;
  pir_u->gen_d++;

  c3_assert(msc_l <= 0x7fffffff);
  wit_u->msc_l = msc_l;

  wit_u->mat = 0;
  wit_u->nex_u = 0;

  if ( !pir_u->ent_u ) {
    c3_assert(!pir_u->ext_u);

    pir_u->ent_u = pir_u->ext_u = wit_u;
    wit_u->pre_u = wit_u->nex_u = 0;
  }
  else {
    wit_u->nex_u = pir_u->ent_u;
    wit_u->pre_u = 0;

    c3_assert(0 == pir_u->ent_u->pre_u);
    pir_u->ent_u->pre_u = wit_u;

    pir_u->ent_u = wit_u;
  }
}

/* u3_pier_restore(): reload pier images.
*/
void
u3_pier_restore(c3_c* dir_c)
{
}

/* u3_pier_interrupt(): interrupt running process.
*/
void
u3_pier_interrupt(u3_pier* pir_u)
{}

/* u3_pier_import(): import pier from outside path, and restore.
*/
void
u3_pier_import(c3_c* dir_c,
               c3_c* out_c)
{
}

/* _lord_poke(): receive result from client.
*/
void
_lord_poke(void* vod_p, u3_noun job)
{
  c3_lord* pir_u = vod_p;

  if ( c3y != u3du(job) ) {
    goto error;
  }
  else {
    u3_noun p_sut, q_sut, r_sut;

    switch ( c3h(job) ) {
      case c3__play: {
        if ( (c3n == u3r_cell(job, 0, &p_sut)) || 
             (c3n == u3ud(p_sut)) ||
             (u3r_met(6, p_sut != 1)) )
        {
          goto error;
        }
        _lord_poke_play(
        break;
      }
      case c3__done: {
        if ( (c3n == u3r_cell(job, 0, &p_sut, )) || 
             (c3n == u3ud(p_sut)) ) {
          goto error;
        break;
      }
      case c3__swap: {
        break;
      }
      case c3__fill: {
        break;
      }
    }
  }

  error: {
    _lord_fail("bad job");
  }
}

  /* _lord_spawn(): create child process.
  */
  static void
  _lord_spawn(uv_loop_t* lup_u,
              c3_c*      dir_c,
              c3_c*      key_c)
  {
    c3_c* arg_c[3];
    c3_i  err_i;

    arg_c[0] = "bin/urbit-client";      //  executable
    arg_c[1] = ".";                     //  path to checkpoint directory
    arg_c[2] = dir_c;                   //  ship name, no ~
    arg_c[3] = key_c;                   //  disk key, as %llx:%llx:%llx:%llx
    arg_c[4] = 0;

    uv_pipe_init(lup_u, &u3W.inn_u, 0);
    uv_pipe_init(lup_u, &u3W.out_u, 0);

    u3W.cod_u[0].flags = UV_CREATE_PIPE | UV_READABLE_PIPE;
    u3W.cod_u[0].data.stream = (uv_stream_t *)&u3W.inn_u;

    u3W.cod_u[1].flags = UV_CREATE_PIPE | UV_WRITABLE_PIPE;
    u3W.cod_u[1].data.stream = (uv_stream_t *)&u3W.out_u;

    u3W.cod_u[2].flags = UV_INHERIT_FD;
    u3W.cod_u[2].fd = 2;

    u3W.ops_u.stdio = u3W.cod_u;
    u3W.ops_u.stdio_count = 3;
   
    u3W.ops_u.exit_cb = _lord_handle_exit;
    u3W.ops_u.file = arg_c[0];
    u3W.ops_u.args = arg_c;

    fprintf(stderr, "spawn\r\n");
    if ( (err_i = uv_spawn(lup_u, &u3W.cub_u, &u3W.ops_u)) ) {
      fprintf(stderr, "spawn: %s\r\n", uv_strerror(err_i));
      u3_lo_exit();
      exit(1);
    }
    uv_read_start((uv_stream_t*) &u3W.out_u, _lord_alloc, _lord_handle_read);
  }


  /* _lord_handle_exit(): handle subprocess exit.
  */
  static void 
  _lord_handle_exit(uv_process_t* req_u,
                    c3_ds         sas_i,
                    c3_i          sig_i)
  {
    fprintf(stderr, "_lord_handle_exit: status %lld, signal %d\r\n", 
                    sas_i, sig_i);
    uv_close((uv_handle_t*) req_u, 0);
  }
