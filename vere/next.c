/* vere/next.c
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

    /* u3_poke: poke, or u3_none for an error.
    */
      typedef void (*u3_poke)(u3_noun);

    /* u3_mess: blob message in process.
    */
      typedef struct _u3_mess {
        c3_d             len_d;             //  blob length in bytes
        c3_d             has_d;             //  currently held             
        struct _u3_meat* meq_u;             //  exit of message queue
        struct _u3_meat* qem_u;             //  entry of message queue
      } u3_mess;

    /* u3_meat: blob message block.
    */
      typedef struct _u3_meat {
        struct _u3_hbod* nex_u;
        c3_w             len_w;
        c3_y             hun_y[0];
      } u3_meat;

    /* u3_moat: inbound message stream.
    */
      typedef struct _u3_moat {
        uv_pipe_t*       pyp_u;             //  input stream
        struct _u3_mess* mes_u;             //  message in progress
        c3_d             len_d;             //  length of stray bytes
        c3_y*            rag_y;             //  stray bytes
        u3_poke          pok_f;             //  action function
      } u3_moat;

    /* u3_mojo: outbound message stream.
    */
      typedef struct _u3_mojo {
        uv_pipe_t*       
      } u3_mojo;


/* _next_read_empty(): input to empty buffer.
*/
void
_next_read_empty(u3_moat* mot_u,
                 c3_d     len_d,
                 c3_y*    buf_y)
{
  /* put initial input in stray buffer
  */
  if ( mot_u->rag_y ) {
    mot_u->rag_y = c3_realloc(mot_u->rag_y, mot_u->len_d + len_d);
    memcpy(mot_u->rag_y + mot_u->len_d, buf_y, len_d);
  }
  else {
    mot_u->rag_y = buf_y;
    mot_u->len_d = len_d;
  }

  /* if we have enough input 
  if ( mot_u->len_d > 8ULL ) {
    c3_d nel_d;
  }
}

/* _next_handle_read(): process stream input
*/
void
_next_handle_read(uv_stream_t*    str_u,
                  ssize_t         len_i,
                  const uv_buf_t* buf_u)
{
  u3_moat* mot_u = (void *)str_u;
  c3_y*    bas_y;

  if ( mot_u->mes_u ) {
    struct _u3_meat
  }
}

/* _next_

