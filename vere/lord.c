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
  **        released                (output actions allowed)
  **
  **    if the worker replaces the event with a %warm, we go from the
  **    dispatched state back to itself.  currently, we dispatch one
  **    event at a time to the worker, but this is not a hard constraint.
  **
  **    in parallel, we try to save the event.  it goes through phases:
  **      
  **        precommit requested
  **        precommit complete
  **        commit requested
  **        commit complete
  **   
  **    the sanity constraints that connect these two paths:
  **
  **        - an event can't request a commit until it's received.
  **        - an event can't be released until it, and all events
  **          preceding it, are precommitted.
  **
  **    be committed until received, or executed until all events before
  **    it are precommitted.
  **
  **    events are executed in order by the working process, but they
  **    can be either precommitted or committed out-of-order.
  */
    typedef struct _u3_writ {
      u3_lord* god_u;                       //  belonging to
      u3_noun  wit;                         //  source atom
      u3_atom  mat;                         //  jammed atom (whole ++writ)
    } u3_writ;

  /* u3_lord: working process controller.
  */
    typedef struct _u3_proc {
      uv_process_t         cub_u;           //  process handle
      uv_process_options_t ops_u;           //  process configuration
      uv_stdio_container_t cod_u[3];        //  process options
      u3_mojo inn_u;                        //  client's stdin
      u3_moat out_u;                        //  client's stdout
      c3_d    sen_d;                        //  last event dispatched
      c3_d    rec_d;                        //  last event received
      c3_w    mug_w;                        //  mug after last event, or 0
    } u3_proc;

  /* u3_disk: manage events on disk.
  **
  **    any event once discovered should be in one of these sets.
  **    when we complete a precommit, we check that the event is
  **    still in "sent to precommit" -- otherwise, it has been
  **    replaced and needs to be re-precommitted.
  */
    typedef struct _u3_disk {               
      u3_noun moc;                          //  (set evt_d) sent to commit
      u3_noun com;                          //  (set evt_d) committed
      c3_d    com_d;                        //  committed through
      u3_noun rep;                          //  (set evt_d) sent to precommit
      u3_noun pre;                          //  (set 
    } u3_disk;

  /* u3_lord: control child process.
  */
    typedef 
  /* u3_pier: ship controller.
  */
    typedef struct _u3_pier {
      u3_proc*         por_u;               //  live process
      c3_c*            dir_c;               //  pier directory
      c3_d             gen_d;               //  last event discovered
      c3_d             rec_d;               //  last event processed
      c3_d             com_d;               //  last event committed
      c3_d             key_d[4];            //  save and passkey
      u3_disk          log;                 //  
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


/* _lord_disk_precommitted(): 
*/
void
_disk_precomf

/* u3_lord_react(): react to new queue state.
*/
void
u3_lord_react(u3_lord* sir_u)
{
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
