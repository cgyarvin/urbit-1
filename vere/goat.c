/* vere/goat.c
**
**  the main loop of a worker process.
*/

#include "all.h"
#include <vere/vere.h>

    typedef struct _u3_serf {
      c3_d    evt_d;                        //  event number
      c3_d    key_d[4];                     //  disk key
      u3_moat inn_u;                        //  message input
      u3_mojo out_u;                        //  message output
      c3_o    sav_o;                        //  pending save
    } u3_serf;
    static u3_serf u3S;

    /*  serf-lord protocol:
    **
    **  ++  plea                            ::  from serf to lord
    **    $%  $:  $play                     ::  send events
    **            p/@                       ::  first number expected
    **        ==                            ::
    **        $:  $done                     ::  event executed unchanged
    **            p/@                       ::  number of this event
    **            q/@                       ::  mug of state (after boot)
    **            r/(list ovum)             ::  actions
    **        ==                            ::
    **        $:  $drop                     ::  ignore event
    **            p/@                       ::  number of this event
    **        ==                            ::
    **        $:  $swap                     ::  replace and retry
    **            p/@                       ::  number of original event
    **            q/ovum                    ::  replacement in order
    **        ==                            ::
    **        $:  $fill                     ::  insert spontaneously
    **            p/ovum                    ::  new event
    **    ==  ==                            ::
    **
    **  ++  writ
    **    $%  $:  $cold                     ::  confirmed event (no output)
    **            p/@                       ::  event number
    **            q/@                       ::  mug of state (after boot)
    **            r/ovum                    ::  input
    **        ==                            ::
    **        $:  $exit                     ::  snapshot, then exit
    **            p/@                       ::  exit code
    **        ==                            ::
    **        $:  $roll                     ::  rekey snapshot on disk
    **            p/@                       ::  new 256-bit key
    **        ==                            ::
    **        $:  $save                     ::  save snapshot to disk
    **            p/@                       ::  number of old snaps to save
    **        ==                            ::
    **        $:  $warm                     ::
    **            p/@                       ::  event number
    **            q/@                       ::  mug of state
    **            r/@ud                     ::  timeout in ms, or 0
    **            s/ovum                    ::  actual event
    **    ==  ==
    */

/* _goat_fail(): failure stub.
*/
static void
_goat_fail(void* vod_p, c3_c* wut_c, c3_i err_i)
{
  if ( err_i ) {
    fprintf(stderr, "fail: %s: %s\r\n", wut_c, uv_strerror(err_i));
  } else {
    fprintf(stderr, "fail: %s\r\n", wut_c);
  }
  exit(1);
}

/* _goat_poke_cold(): apply confirmed event (no timeout or output).
*/
static void
_goat_poke_cold(c3_d    evt_d,              //  event number
                u3_noun ovo)                //  event
{
}

/* _goat_poke_warm(): apply unconfirmed event.
*/
static void
_goat_poke_warm(c3_d    evt_d,              //  event number
                c3_w    tim_w,              //  timeout in ms
                u3_noun ovo)                //  event
{
}

/* _goat_poke_exit(): exit on command.
*/
static void
_goat_poke_exit(c3_w cod_w)                 //  exit code
{
  exit(cod_w);
}

/* _goat_poke_roll(): roll key.
*/
static void
_goat_poke_roll(c3_d key_d[4])
{
  c3_assert(!"roll: not supported");
}

/* _goat_poke_save(): save snapshot.
*/
/* _goat_poke(): 
*/
void
_goat_poke(void* vod_p, u3_noun job)
{
  if ( c3y != u3du(job) ) {
    goto error;
  }
  else {
    u3_noun p_sut, q_sut, r_sut;

    switch ( c3h(job) ) {
      case c3__warm: {
        if ( (c3n == u3r_qual(job, 0, &p_sut, &q_sut, &r_sut)) || 
             (c3n == u3ud(p_sut)) ||
             (u3r_met(6, p_sut) != 1)
             (c3n == u3ud(q_sut)) ||
             (u3r_met(5, q_sut) > 1)
             (c3n == u3du(r_sut)) )
        {
          goto error;
        }
        _goat_poke_warm(u3r_chub(0, p_sut),
                        u3r_word(0, q_sut),
                        u3k(r_sut));
        break;
      }
      case c3__exit: {
        if ( (c3n == u3r_cell(job, 0, &p_sut)) || 
             (c3n == u3ud(p_sut))
             (u3r_met(3, p_sut) > 1) )
        {
          goto error;
        }
        _goat_poke_exit(u3k(p_sut));
        break;
      }
      case c3__roll: {
        if ( (c3n == u3r_cell(job, 0, &p_sut)) || 
             (c3n == u3ud(p_sut)) ) {
          goto error;
        }
        _goat_poke_roll(u3k(p_sut));
        break;
      }
      case c3__save: {
        if ( (c3n == u3r_cell(job, 0, &p_sut)) || 
             (c3n == u3ud(p_sut)) ) {
          goto error;
        }
        u3e_save();
        break;
      }
      case c3__cold: {
        if ( (c3n == u3r_qual(job, 0, &p_sut, &q_sut, &r_sut)) || 
             (c3n == u3ud(p_sut)) ||
             (u3r_met(6, p_sut) != 1)
             (c3n == u3ud(q_sut)) ||
             (u3r_met(5, q_sut) > 1)
             (c3n == u3du(r_sut)) )
        {
          goto error;
        }
        _goat_poke_warm(u3r_chub(0, p_sut),
                        u3r_word(0, q_sut),
                        u3k(r_sut));
        break;
      }
      case c3__warm: {
        if ( (c3n == u3r_qual(job, 0, &p_sut, &q_sut, &r_sut)) || 
             (c3n == u3ud(p_sut)) ||
             (u3r_met(6, p_sut) != 1)
             (c3n == u3ud(q_sut)) ||
             (u3r_met(5, q_sut) > 1)
             (c3n == u3du(r_sut)) )
        {
          goto error;
        }
        _goat_poke_warm(u3r_chub(0, p_sut),
                        u3r_word(0, q_sut),
                        u3k(r_sut));
        break;
      }
      default: goto error;
    }
    u3z(job);
    return;
  }

  error: {
    _goat_fail("bad job");
  }
}

/* u3_goat_main: main() when run as urbit-client
*/
void
u3_goat_main(c3_i argc, c3_c* argv[])
{
  c3_c* dir_c = argv[1];                    //  path to pier
  c3_c* key_c = argv[2];                    //  passkey, %llx:%llx:%llx:%llx

  c3_assert(3 == argc);

  /* load passkey
  */
  {
    sscanf(key_c, "%llx:%llx:%llx:%llx", &u3S.key_d[0],
                                         &u3S.key_d[1],
                                         &u3S.key_d[2], 
                                         &u3S.key_d[3]);
  }

  /* boot image
  */
  {
    u3S.evt_d = u3m_boot_new(dir_c);
    u3S.sav_o = c3n;
  }

  /* configure pipe to lord process
  */
  {
    c3_i err_i;

    err_i = uv_pipe_init(&u3S.inn_u.pyp_u, 0);
    c3_assert(!err_i);
    uv_pipe_open(&u3S.inn_u.pyp_u, 0);

    err_i = uv_pipe_init(&u3S.out_u.pyp_u, 0);
    c3_assert(!err_i);
    uv_pipe_open(&u3S.out_u.pyp_u, 1);
  }

  /* start reading
  */
  u3_newt_read_start(&u3S.inn_u);

  /* run main loop
  */
  u3_goat_loop();
}
