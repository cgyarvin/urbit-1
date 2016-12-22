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
  **    if the worker replaces the event with a %warm, we go from the
  **    dispatched state back to itself.  currently, we dispatch one
  **    event at a time to the worker, but this is not a hard constraint.
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
  **    after crash recovery, events which were precommitted but
  **    not yet committed have at-least-once semantics in their
  **    output effects.  (not the actual changes to the arvo state,
  **    which are of course exactly-once.)  ideally all your actions
  **    are network packets or idempotent http requests.
  */

  /* u3_writ: inbound event.
  */
    typedef struct _u3_writ {
      u3_noun          job;                 //  source atom
      c3_d             evt_d;               //  event number
      c3_l             msc_l;               //  ms to timeout
      c3_w             mug_w;               //  hash before executing
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
      u3_mojo              inn_u;           //  client's stdin
      u3_moat              out_u;           //  client's stdout
      c3_d                 sen_d;           //  last event dispatched
      c3_d                 dun_d;           //  last event completed
      c3_d                 rel_d;           //  last event released
      c3_w                 mug_w;           //  mug after last completion
    } u3_lord;

  /* u3_disk: manage events on disk.
  **
  **    any event once discovered should be in one of these sets.
  **    when we complete a precommit, we check that the event is
  **    still in "sent to precommit" -- otherwise, it has been
  **    replaced and needs to be re-precommitted.
  */
    typedef struct _u3_disk {               
      uv_file fil_u;                        //  logfile
      c3_d    end_d;                        //  byte end of file
      c3_d    rep_d;                        //  precommit requested
      c3_d    pre_d;                        //  all precommitted
      c3_d    moc_d;                        //  commit requested
      c3_d    com_d;                        //  committed
    } u3_disk;

  /* u3_pier: ship controller.
  */
    typedef struct _u3_pier {
      u3_lord*         por_u;               //  live process
      c3_c*            dir_c;               //  pier directory
      c3_d             gen_d;               //  last event discovered
      c3_d             key_d[4];            //  save and passkey
      u3_disk          log_u;               //  event log
      u3_lord          god_u;               //  compute farm
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

/* _pier_work_cb(): callback from work.
*/

/* _pier_work_compute(): send for processing.
*/
static void
_pier_work_compute(u3_pier* pier_u)
{
  u3_noun job;


}

/* _pier_react(): react to i/o, inbound or outbound.
*/
static void
_pier_react(u3_pier* pir_u)
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

      pir_u->god_u.sen_d += 1;
    }

    /* if writ is (a) being computed and (b) next in line to
    ** precommit, request precommit
    */
    if ( (pre_u->evt_d <= pir_u->god_u.sen_d) &&
         (pre_u->evt_d == (1 + pir_u->log_u.rep_d)) ) 
    {
      fprintf(stderr, "precommit request: %llx\r\n", pre_u->evt_d);
      _pier_disk_precommit(&pir_u->log_u, wit_u);

      pir_u->log_u.rep_d += 1;
    }

    /* if writ is (a) computed and (b) precommitted, release actions
    */
    if ( (pre_u->evt_d <= pir_u->god_u.dun_d) &&
         (pre_u->evt_d <= pir_u->log_u.pre_d) )
    {
      c3_assert(pre_u->evt_d == (1 + pre_u->god_u.rel_d));

      fprintf(stderr, "release: %llx\r\n", pre_u->evt_d);
      _pier_work_release(&pir_u->god_u, wit_u);

      pir_u->god_u.rel_d += 1;
    }

    /* if writ is (a) released and (b) next in line to commit, 
    ** request commit
    */
    if ( (pre_u->evt_d <= pir_u->god_u.rel_d) &&
         (pre_u->evt_d == (1 + pir_u->log_u.moc_d)) ) 
    {
      fprintf(stderr, "commit request: %llx\r\n", pre_u->evt_d);
      _pier_disk_commit(&pir_u->log_u, wit_u);

      pir_u->log_u.moc_d += 1;
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
        }
      }

      /* free contents
      */
      u3z(wit_u->job);
      u3z(wit_u->mat);
      u3z(wit_u->act);
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
