/* vere/proc.c
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

  /* _proc_handle_exit(): handle subprocess exit.
  */
  static void 
  _proc_handle_exit(uv_process_t* req_u,
                    c3_ds         sas_i,
                    c3_i          sig_i)
  {
    fprintf(stderr, "_proc_handle_exit: status %lld, signal %d\r\n", 
                    sas_i, sig_i);
    uv_close((uv_handle_t*) req_u, 0);
  }

  /* _proc_handle_read(): handle input from subprocess.
  */
  void
  _proc_handle_read(uv_stream_t* str_u,
                    ssize_t      len_i, 
                    const uv_buf_t*    buf_u)
  {
    if ( len_i == UV_EOF ) {
      fprintf(stderr, "_proc_handle_read: eof\r\n");
    }
    else {
      c3_c* buf_c = c3_malloc(len_i + 1);

      fprintf(stderr, "_proc_read: %d\n", (int)len_i);
      strncpy(buf_c, buf_u->base, len_i);
      buf_c[len_i] = 0;
      fprintf(stderr, "_proc_read: %d chars, \"%s\"\r\n", (int)len_i, buf_c);
      free(buf_c);
    }
  }
 
  /* _proc_spawn(): create child process.
  */
  static void
  _proc_spawn(uv_loop_t* lup_u)
  {
    c3_c* arg_c[3];
    c3_i  err_i;

    arg_c[0] = "./dummy";
    arg_c[1] = arg_c[2] = 0;

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
   
    u3W.ops_u.exit_cb = _proc_handle_exit;
    u3W.ops_u.file = arg_c[0];
    u3W.ops_u.args = arg_c;

    fprintf(stderr, "spawn\r\n");
    if ( (err_i = uv_spawn(lup_u, &u3W.cub_u, &u3W.ops_u)) ) {
      fprintf(stderr, "spawn: %s\r\n", uv_strerror(err_i));
      u3_lo_exit();
      exit(1);
    }
    uv_read_start((uv_stream_t*) &u3W.out_u, _proc_alloc, _proc_handle_read);
  }













/* u3_lo_loop(): begin main event loop.
*/
void
u3_lo_loop()
{
  uv_loop_t* lup_u = uv_default_loop();

  u3_Host.lup_u = lup_u;

  signal(SIGPIPE, SIG_IGN);     //  pipe, schmipe
  // signal(SIGIO, SIG_IGN);    //  linux is wont to produce for some reason

  _lo_init();

  u3_raft_init();

  if ( _(u3_Host.ops_u.tex) ) {
    u3t_boff();
    u3t_damp();
    u3_lo_exit();

    fprintf(stderr, "dry run: exit\r\n");
    exit(0);
  }
  else {
    _proc_spawn(u3L);

    if ( c3n == u3_Host.ops_u.bat ) {
      uv_run(u3L, UV_RUN_DEFAULT);
    }
  }
}

/* u3_lo_lead(): actions on promotion to leader.
*/
void
u3_lo_lead(void)
{
  //  Further server configuration.
  //
  {
    u3_http_ef_bake();
  }

  _lo_talk();
  {
    u3_unix_ef_look(c3n);
    u3v_plan(u3nt(u3_blip, c3__ames, u3_nul),
               u3nc(c3__kick, u3k(u3A->now)));
  }
  _lo_poll();

#if 0
  u3_loom_save(u3A->ent_d);

  u3_Host.sav_u.ent_d = rec_u->ent_d;
#endif

  if ( c3y == u3_Host.ops_u.nuu ) {
    if ( u3_Host.ops_u.who_c ) {
      u3_term_ef_ticket(u3_Host.ops_u.who_c, u3_Host.ops_u.tic_c);
    }
    u3_term_ef_boil(1);
  }

  if ( c3y == u3_Host.ops_u.veb ) {
    u3_term_ef_verb();
  }

#if 1
  _lo_slow();
#endif
}

#if 0
/* _lo_mark_reck(): mark a reck.
*/
static c3_w
_lo_mark_reck(u3_reck* rec_u)
{
  c3_w siz_w = 0;
  c3_w egg_w;

  siz_w += u3m_mark_noun(rec_u->ken);
  siz_w += u3m_mark_noun(rec_u->roc);

  siz_w += u3m_mark_noun(rec_u->yot);
  siz_w += u3m_mark_noun(rec_u->now);
  siz_w += u3m_mark_noun(rec_u->wen);
  siz_w += u3m_mark_noun(rec_u->sen);
  siz_w += u3m_mark_noun(rec_u->own);
  siz_w += u3m_mark_noun(rec_u->roe);
  siz_w += u3m_mark_noun(rec_u->key);

  {
    u3_cart* egg_u;

    egg_w = 0;
    for ( egg_u = rec_u->ova.egg_u; egg_u; egg_u = egg_u->nex_u ) {
      egg_w += u3m_mark_noun(egg_u->vir);
    }
    siz_w += egg_w;
  }
#if 0
  fprintf(stderr, "ken %d, roc %d, yot %d, roe %d, egg %d\r\n",
                   ken_w, roc_w, yot_w, roe_w, egg_w);
#endif
  return siz_w;
}

/* _lo_mark(): mark the whole vere system.
*/
static c3_w
_lo_mark()
{
  c3_w siz_w;

  siz_w = u3m_mark_internal();
  siz_w += _lo_mark_reck(u3_Host.arv_u);

  return siz_w;
}
#endif

#if 0
/* _lo_word(): print a word to the passed stream.
*/
static void
_lo_word(FILE* fil_u, c3_w wod_w)
{
  u3_noun top = c3y;

  if ( wod_w / (1000 * 1000 * 1000) ) {
    fprintf(fil_u, "%u.", wod_w / (1000 * 1000 * 1000));
    wod_w %= (1000 * 1000 * 1000);
    top = c3n;
  }
  if ( wod_w / (1000 * 1000) ) {
    fprintf(fil_u, ((top == c3y) ? "%u." : "%03u."),
                   wod_w / (1000 * 1000));
    wod_w %= (1000 * 1000);
    top = c3n;
  }
  if ( wod_w / 1000 ) {
    fprintf(fil_u, ((top == c3y) ? "%u." : "%03u."), wod_w / 1000);
    wod_w %= 1000;
    top = c3n;
  }
  fprintf(fil_u, ((top == c3y) ? "%u" : "%03u"), wod_w);
}

/* u3_lo_grab(): garbage-collect the world, plus roots.
*/
void
u3_lo_grab(c3_c* cap_c, u3_noun som, ...)
{
  c3_w siz_w, lec_w;

  siz_w = _lo_mark();
  {
    va_list vap;
    u3_noun tur;

    va_start(vap, som);

    if ( som != u3_none ) {
      siz_w += u3m_mark_noun(som);

      while ( u3_none != (tur = va_arg(vap, u3_noun)) ) {
        siz_w += u3m_mark_noun(tur);
      }
    }
    va_end(vap);
  }
  lec_w = u3m_sweep(siz_w);

  // if ( lec_w || (c3y == u3_Flag_Verbose) )
  if ( lec_w  || !strcmp("init", cap_c) ) {
    FILE* fil_u = uH;
    fprintf(fil_u, "%s: gc: ", cap_c);
    if ( lec_w ) {
      _lo_word(fil_u, 4 * lec_w);
      fprintf(fil_u, " bytes shed; ");
    }
    _lo_word(fil_u, 4 * siz_w);
    uL(fprintf(fil_u, " bytes live\n"));

#if 0
    if ( lec_w ) {
      uL(fprintf(uH, "zero garbage tolerance!\n"));
      u3_lo_exit();
      c3_assert(0);
      exit(1);
    }
#endif
  }
  u3_wire_lan(u3_Wire) = c3y;
}
#endif
