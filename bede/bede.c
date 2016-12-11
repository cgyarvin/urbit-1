/* bede/bede.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include <gmp.h>
#include <stdint.h>
#include <uv.h>
#include <sigsegv.h>
#include <curses.h>
#include <termios.h>
#include <term.h>
#include <dirent.h>
#include <openssl/ssl.h>

#define U3_GLOBAL
#define C3_GLOBAL
#include "all.h"
#include "vere/vere.h"

    /* u3_work_task: task for worker process.
    */
    typedef struct _u3_work_task {
      c3_d    num_d;                                //  input sequence number
      //
      //  {$sync ~}         ::  flush checkpoint to disk
      //  {$work event}     ::  
      //  {$fail event}: 
      //  {$boot sequence}
      //  
      u3_noun job;
    } u3_work_task;

    /* u3_work_gift: gift from worker process.
    */
      typedef struct _u3_work_gift {
        c3_d    num_d;                              //  input sequence number
        u3_noun job;
      } u3_work_gift;

    /* u3_work_state: noun state of worker process.
    **
    **  the worker state is stored in the start of the image.
    */
      typedef struct _u3_work_state {
        c3_d    nex_d;                              //  next sequence expected
        u3_noun new;                                //  pre-boot list
        u3_noun roc;                                //  
      } u3_work;
      static u3_work* u3W;

/* u3_work_read_task(): pull the next input task.
*/
u3_work_task*
u3_work_read_task(u3_work* wok_u)
{
  // read on stdin
}

/* u3_work_write_gift(): send a gift back to the parent.
*/
void
u3_work_write_gift(u3_work* wok_u, 
                   c3_d     num_d,
                   u3_noun  job)
{
  // write on stdout
}

/* u3_work_step(u3_work_task* taz_u): perform one task; produce gift.
*/
u3_noun
u3_work_step(u3_work*  wok_u,
             u3_noun   job)

{
  //  check that the sender knows the right event number
  //
  c3_assert(nex_d == taz_u->num_d);
  {
  }
}

/* u3_work_loop()
*/
void
u3_work_loop(u3_work* wok_u)
{
  //  the canonical boot sequence runs the first 5 events,
  //  then produces a lifecycle gate whose +7 is the 
  //  standard arvo kernel.
  //
  while ( 1 ) {
    u3_noun taz_u = u3_work_read_task(wok_u);

    c3_assert(taz_u->num_d == wok_u->nex_d);
    wok_u->nex_d += 1;

    if ( taz_u->num_d < 5 ) {
      wok_u->new = u3nc(taz_u->job, wok_u->new);

      else if ( taz_u->num_d == 4 ) {
        u3_noun sys = u3kb_flop(wok_u->new);

        wok_u->roc = u3v_fire(sys);
        wok_u->new = 0;
      }
      u3_work_write_gift(wok_u, taz_u->num_d, 0);
    }
    else {
      u3_noun bog = u3_work_step(wok_u, taz_u->job);

      u3_work_write_gift(wok_u, taz_u->num_d, bog);
    }
  }
}

/* u3_work_main()
*/
void
u3_work_main(void)
{
  u3_work_task* taz;
 
  u3_work_boot();
  taz = 

  =u3_work_next_task();

  //  map in memory
  //  
  u3_work_loop();
}

while ( 1

/* _bede_send(): send noun to fd; transfers
*/

