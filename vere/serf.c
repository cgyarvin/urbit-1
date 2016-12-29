/* vere/serf.c
**
**  the main loop of a worker process.
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
#include <vere/vere.h>

    typedef struct _u3_serf {
      c3_d    evt_d;                        //  current event number
      c3_l    mug_l;                        //  hash of state
      c3_d    key_d[4];                     //  disk key
      u3_moat inn_u;                        //  message input
      u3_mojo out_u;                        //  message output
    } u3_serf;
    static u3_serf u3V;

    /*  serf-lord protocol:
    **
    **  ++  plea                            ::  from serf to lord
    **    $%  $:  $play                     ::  send events
    **            p/@                       ::  first number expected
    **            q/@                       ::  mug of state (or 0 to boot)
    **        ==                            ::
    **        $:  $done                     ::  event executed unchanged
    **            p/@                       ::  number of this event
    **            q/@                       ::  mug of state (or 0)
    **            r/(list ovum)             ::  actions
    **        ==                            ::
    **        $:  $work                     ::  replace and retry
    **            p/@                       ::  event number
    **            q/@                       ::  mug of state (or 0)
    **            r/@                       ::  urbit time
    **            s/ovum                    ::  event
    **    ==  ==                            ::
    **
    **  ++  writ                            ::  from lord to serf
    **    $%  $:  $exit                     ::  snapshot, then exit
    **            p/@                       ::  exit code
    **        ==                            ::
    **        $:  $save                     ::  save snapshot to disk
    **            p/@                       ::  number of old snaps to save
    **        ==                            ::
    **        $:  $work                     ::  execute event
    **            p/@                       ::  event number
    **            q/@                       ::  mug of state (or 0)
    **            r/@                       ::  urbit time
    **            s/ovum                    ::  event
    **    ==  ==                            ::
    */

/* _serf_fail(): failure stub.
*/
static void
_serf_fail(void* vod_p, const c3_c* wut_c)
{
  fprintf(stderr, "fail: %s\r\n", wut_c);
  exit(1);
}

/* _serf_send(): send result back to lord.
*/
static void
_serf_send(u3_noun job)
{
  u3_newt_write(&u3V.out_u, u3ke_jam(job), 0);
}

/* _serf_send_replace(): send replacement job back to lord.
*/
static void
_serf_send_replace(u3_noun ovo)
{
  _serf_send(u3nq(c3__work,
                  u3i_chubs(1, &u3V.evt_d),
                  u3V.mug_l,
                  u3nc(u3k(u3A->now), ovo)));
}

/* _serf_send_complete(): report completion.
*/
static void
_serf_send_complete(u3_noun vir)
{
  _serf_send(u3nq(c3__done,
                  u3i_chubs(1, &u3V.evt_d),
                  u3r_mug(u3A->roc),
                  vir));
}

/* _serf_lame(): event failed, replace with error event.
*/
static void
_serf_lame(u3_noun ovo, u3_noun why, u3_noun tan)
{
  /* XX: the next crud will contain the original event.
  */
  u3z(ovo);
  _serf_send_replace(u3nt(c3__crud, why, tan));
}

/* _serf_sure(): event succeeded, report completion.
*/
static void
_serf_sure(u3_noun ovo, u3_noun vir, u3_noun cor)
{
  u3z(ovo);

  u3z(u3A->roc);
  u3A->roc = cor;

  _serf_send_complete(vir);
}

/* _serf_poke_work(): apply event.
*/
static void
_serf_poke_work(c3_d    evt_d,              //  event number
                c3_l    mug_l,              //  mug of state
                u3_noun now,                //  event date
                u3_noun ovo)                //  event
{
  u3_noun gon;
  c3_assert(evt_d == u3V.evt_d + 1ULL);
  u3V.evt_d = evt_d;

  c3_assert(u3r_mug(u3A->roc) == mug_l);

  u3z(u3A->now);
  u3A->now = now;

#ifdef GHETTO
  struct timeval b4, f2, d0;
  gettimeofday(&b4, 0);
#endif

  gon = u3m_soft(0, u3v_poke, u3k(ovo));

#ifdef GHETTO
  c3_w ms_w;
  c3_w clr_w;

  gettimeofday(&f2, 0);
  timersub(&f2, &b4, &d0);
  ms_w = (d0.tv_sec * 1000) + (d0.tv_usec / 1000);
  clr_w = ms_w > 1000 ? 1 : ms_w < 100 ? 2 : 3; //  red, green, yellow
  if(c3__belt != u3h(u3t(ovo)) || clr_w != 2){
    uL(fprintf(uH, "\x1b[3%dm%%%s %4d.%02dms\x1b[0m\n",
                       clr_w, txt_c, ms_w, (int) (d0.tv_usec % 1000) / 10));
  }
  free(txt_c);
#endif

  if ( u3_blip != u3h(gon) ) {
    u3_noun why = u3k(u3h(gon));
    u3_noun tan = u3k(u3t(gon));

    u3z(gon);
    _serf_lame(ovo, why, tan);
  }
  else {
    u3_noun vir = u3k(u3h(u3t(gon)));
    u3_noun cor = u3k(u3t(u3t(gon)));

    _serf_sure(ovo, vir, cor);
  }
}

/* _serf_poke_exit(): exit on command.
*/
static void
_serf_poke_exit(c3_w cod_w)                 //  exit code
{
  exit(cod_w);
}

/* _serf_poke(): 
*/
void
_serf_poke(void* vod_p, u3_noun job)
{
  if ( c3y != u3du(job) ) {
    goto error;
  }
  else {
    u3_noun p_job, q_job, r_job, s_job;

    switch ( u3h(job) ) {
      case c3__work: {
        if ( (c3n == u3r_quil(job, 0, &p_job, &q_job, &r_job, &s_job)) || 
             (c3n == u3ud(p_job)) ||
             (u3r_met(6, p_job) != 1) ||
             (c3n == u3ud(q_job)) ||
             (u3r_met(5, q_job) > 1) ||
             (c3n == u3ud(r_job)) ||
             (u3r_met(7, r_job) != 1) ||
             (c3n == u3du(s_job)) )
        {
          goto error;
        }
        _serf_poke_work(u3r_chub(0, p_job),
                        u3r_word(0, q_job),
                        u3k(r_job),
                        u3k(s_job));
        break;
      }
      case c3__exit: {
        if ( (c3n == u3r_cell(job, 0, &p_job)) || 
             (c3n == u3ud(p_job)) ||
             (u3r_met(3, p_job) > 1) )
        {
          goto error;
        }
        _serf_poke_exit(u3k(p_job));
        break;
      }
      case c3__save: {
        if ( (c3n == u3r_cell(job, 0, &p_job)) || 
             (c3n == u3ud(p_job)) ) {
          goto error;
        }
        u3e_save();
        break;
      }
      default: goto error;
    }
    u3z(job);
    return;
  }

  error: {
    _serf_fail(0, "bad job");
  }
}

/* u3_serf_loop:
*/
void
u3_serf_loop(void)
{
  // XX
  exit(1);
}

/* u3_serf_main: main() when run as urbit-client
*/
void
u3_serf_main(c3_i argc, c3_c* argv[])
{
  c3_c* dir_c = argv[1];                    //  path to pier
  c3_c* key_c = argv[2];                    //  passkey, %llx:%llx:%llx:%llx

  c3_assert(3 == argc);

  /* load passkey
  */
  {
    sscanf(key_c, "%llx:%llx:%llx:%llx", &u3V.key_d[0],
                                         &u3V.key_d[1],
                                         &u3V.key_d[2], 
                                         &u3V.key_d[3]);
  }

  /* boot image
  */
  {
    u3V.evt_d = u3m_boot_new(dir_c);
  }

  /* configure pipe to lord process
  */
  {
    c3_i err_i;

    err_i = uv_pipe_init(u3L, &u3V.inn_u.pyp_u, 0);
    c3_assert(!err_i);
    uv_pipe_open(&u3V.inn_u.pyp_u, 0);

    err_i = uv_pipe_init(u3L, &u3V.out_u.pyp_u, 0);
    c3_assert(!err_i);
    uv_pipe_open(&u3V.out_u.pyp_u, 1);
  }

  /* start reading
  */
  u3_newt_read_start(&u3V.inn_u, _serf_poke, _serf_fail);

  /* run main loop
  */
  u3_serf_loop();
}
