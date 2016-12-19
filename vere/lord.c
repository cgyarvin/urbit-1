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

  /* u3_writ: inbound event.
  **
  **    event handling proceeds on two parallel paths.  on the first
  **    path, the event is processed in the child worker process (serf).
  **    state transitions are as follows:
  **
  **        generated               (event numbered and queued)
  **        dispatched              (sent to worker)
  **        received                (completed by worker)
  **        committed               (actions applied, sent to log)
  **        confirmed               (final log has saved)
  **
  **    if the worker replaces the event with a %swap, 
  */
    typedef struct _u3_writ {
      u3_lord* god_u;                       //  parent
      c3_d     evt_d;                       //  event number
      u3_noun  job;                         //  event data
      c3_o     uns_o;                       //  yes/0 iff not saved
      c3_o     und_o;                       //  yes/0 iff not dispatched
      c3_o     unr_o;                       //  yes/0 iff not received
      c3_o     unc_o;                       //  yes/0 iff not committed
      c3_o     unf_o;                       //  
    } u3_writ;

  /* u3_proc: child process communication.
  */
    typedef struct _u3_proc {
      uv_process... XX
      u3_mojo inn_u;                        //  client's stdin
      u3_moat out_u;                        //  client's stdout
    } u3_proc;

  /* u3_disk: manage events on disk.
  */
    typedef struct _u3_tome {
      c3_d com_d;                           //  events committed
      c3_d 

  /* u3_lord: worker process controller
  */
    typedef struct _u3_lord {
      u3_proc*         por_u;               //  live process
      c3_c*            dir_c;               //  pier directory
      c3_d             gen_d;               //  last event discovered
      c3_d             sen_d;               //  last event dispatched
      c3_d             rec_d;               //  last event processed
      c3_d             com_d;               //  last event committed
      c3_d             key_d[4];            //  disk key
      u3_tome          
      struct _u3_lord* nex_u;               //  next in parent list
    } u3_lord;

  /* u3_king: all lords.
  */
    typedef struct _u3_king {               //  
      c3_w     len_w;                       //  number of lords
      c3_w     all_u;
      u3_lord* tab_u;                       //  lord table
    } u3_king;

    static u3_king u3K;

/* u3_lord_react(): react to new queue state.
*/
void
u3_lord_react(u3_lord* sir_u)
{
  for ( 
}

/* u3_lord_discover(): insert task into process controller.
*/
void
u3_lord_present(u3_lord* sir_u,
                u3_noun  job)
{
}

/* u3_lord_restore(): reload pier images.
*/
void
u3_lord_restore(c3_c* dir_c)
{
}

/* u3_lord_interrupt(): interrupt running process.
*/
void
u3_lord_interrupt(u3_lord* sir_u)
{}

/* u3_lord_import(): import pier from outside path, and restore.
*/
void
u3_lord_import(c3_c* dir_c,
               c3_c* out_c)
{
}

/* u3_lord_boot(): 

/* _lord_poke(): receive result from client.
*/
void
_lord_poke(void* vod_p, u3_noun job)
{
  c3_lord* sir_u = vod_p;

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
