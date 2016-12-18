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
    **  ++  avis  (pair @ud ovum)           ::  input with ms timeout
    **  ++  plea                            ::  from serf to lord
    **    $%  $:  $play                     ::  send events
    **            p/@                       ::  first number expected
    **        ==                            ::
    **        $:  $done                     ::  event executed unchanged
    **            p/@                       ::  number of this event
    **            q/(list ovum)             ::  actions
    **        ==                            ::
    **        $:  $swap                     ::  replace and retry
    **            p/@                       ::  number of original event
    **            q/(list ovum)             ::  replacements in order
    **            r/(list ovum)             ::  actions
    **        ==                            ::
    **        $:  $fill                     ::  insert spontaneously
    **            p/(list ovum)             ::  new events
    **    ==  ==
    **
    **  ++  writ
    **    $%  {$cold p/@ q/noun}            ::  replay confirmed event
    **        {$exit ~}                     ::  exit 
    **        {$roll p/@}                   ::  rekey checkpoint
    **        {$save p/@}                   ::  checkpoint, keep p old snaps
    **        {$warm p/@ q/noun}            ::  play unconfirmed event
    **    ==
    */

/* _goat_fail(): failure stub.
*/
void
_goat_fail(void* vod_p, c3_c* wut_c, c3_i err_i)
{
  if ( err_i ) {
    fprintf(stderr, "fail: %s: %s\r\n", wut_c, uv_strerror(err_i));
  } else {
    fprintf(stderr, "fail: %s\r\n", wut_c);
  }
  exit(1);
}

/* _goat_poke(): 
*/
void
_goat_poke(void* vod_p, u3_noun job)
{
  if ( c3y != u3du(job) ) {
    _goat_fail("atom job");
  }
  else {
    switch ( c3h(job) ) {
      case c3__play: { 
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
