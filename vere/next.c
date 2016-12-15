/* vere/next.c
**
**  implements noun blob messages with trivial framing.
**
**  a message is a 64-bit byte count, followed
**  by the indicated number of bytes.  the bytes
**  are the ++cue of of a noun.
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
      typedef void (*u3_poke)(void*, u3_noun);

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
        c3_d             len_d;
        c3_y             hun_y[0];
      } u3_meat;

    /* u3_moat: inbound message stream.
    */
      typedef struct _u3_moat {
        uv_pipe_t*       pyp_u;             //  input stream
        u3_poke          pok_f;             //  action function
        void*            kop_v;             //  action subject
        struct _u3_mess* mes_u;             //  message in progress
        c3_d             len_d;             //  length of stray bytes
        c3_y*            rag_y;             //  stray bytes
      } u3_moat;

    /* u3_mojo: outbound message stream.
    */
      typedef struct _u3_mojo {
        uv_pipe_t*       
      } u3_mojo;


/* _next_consume(): advance buffer processing.
*/
static void
_next_consume(u3_moat* mot_u)
{
  /*  process stray bytes, trying to create a new message
  **  or add a block to an existing one.
  */
  while ( 1 ) {
    if ( mot_u->rag_y ) {
      /* if there is a live message, add a block to the queue.
      */
      if ( mot_u->mes_u ) {
        u3_meat* met_u;

        /* create block
        */
        met_u = c3_malloc(mot_u->len_d + (c3_d) sizeof(u3_meat));
        met_u->nex_u = 0;
        met_u->len_d = mot_u->len_d;
        memcpy(met_u->hun_y, mot_u->rag_y, mot_u->len_d);

        /* enqueue block
        */
        if ( !mot_u->mes_u->meq_u ) {
          mot_u->mes_u->meq_u = mot_u->mes_u->qem_u = met_u;
        }
        else {
          mot_u->mes_u->qem_u->nex_u = met_u;
        }
        mot_u->mes_u->len_d += met_u->len_d;

        mot_u->len_d = 0;
        c3_free(mot_u->hun_y);
      }
      else {
        /* if we have enough stray bytes to fill in a length field,
        ** collect them and create a new message.
        */
        if ( mot_u->len_d < 8ULL ) {
          c3_d nel_d = 0;

          nel_d |= ((c3_d) mot_u->rag_y[0]) << 0ULL;
          nel_d |= ((c3_d) mot_u->rag_y[1]) << 8ULL;
          nel_d |= ((c3_d) mot_u->rag_y[2]) << 16ULL;
          nel_d |= ((c3_d) mot_u->rag_y[3]) << 24ULL;
          nel_d |= ((c3_d) mot_u->rag_y[4]) << 32ULL;
          nel_d |= ((c3_d) mot_u->rag_y[5]) << 40ULL;
          nel_d |= ((c3_d) mot_u->rag_y[6]) << 48ULL;
          nel_d |= ((c3_d) mot_u->rag_y[7]) << 56ULL;

          mot_u->len_d -= 8ULL;

          mot_u->mes_u = c3_malloc(sizeof u3_mess);
          mot_u->mes_u->len_d = nel_d;
          mot_u->mes_u->has_d = 0;
          mot_u->mes_u->meq_u = mot_u->mes_u->qem_u = 0;

          if ( !mot_u->len_d ) {
            c3_free(mot_u->rag_y);
            mot_u->rag_y = 0;
          }
          else {
            /* chop off the length field, put the rest back.
            */
            c3_y buf_y = c3_malloc(mot_u->len_d); 
            
            memcpy(buf_y, mot_u->rag_y[8], mot_u->len_d);

            c3_free(mot_u->rag_y);
            mot_u->rag_y = buf_y;

            /* see what we can do with the rest of the buffer
            */
            continue;
          }
        }
      }
    }

    /*  check for message completions
    */
    if ( mot_u->mes_u ) {
      c3_assert(!mot_u->rag_y);

      if ( mot_u->mes_u->has_d >= mot_u->mes_u->len_d ) {
        c3_d     len_d = mot_u->mes_u->len_d;
        c3_y*    buf_y = c3_malloc(len_d)
        c3_d     pat_d = 0;
        u3_meat* met_u;

        /* collect queue blocks
        */
        {
          met_u = mot_u->mes_u->meq_u;
          while ( pat_d < len_d ) {
            u3_meat* nex_u = met_u->nex_u;

            memcpy(buf_y + pat_d, met_u->hun_y, met_u->len_d);
            pat_d += met_u->len_d;

            c3_free(met_u);
            met_u = nex_u;
          }
        }

        /* build and send the object
        */
        {
          u3_noun jar = u3i_bytes((c3_w) len_d, buf_y);
          u3_noun job = u3ke_cue(jar);

          mot_u->pok_f(mot_u->kop_v, job);
        }

        /* return any spare meat to the rag
        */
        if ( met_u ) {
          /* this should have been claimed already
          */
          c3_assert(!met_u->nex_u);

          /* somewhat wasteful copying, but this is rare
          */
          mot_u->rag_y = c3_malloc(met_u->len_d);
          memcpy(mot_u->rag_y, met_u->buf_y, met_u->len_d);
          mot_u->len_d = met_u->len_d;

          /* reprocess this remainder
          */
          continue;
        }
      }
    }
    break;
  }
}

/* _next_read_empty(): input with no message in process
*/
void
_next_read_empty(u3_moat* mot_u,
                 c3_d     len_d,
                 c3_y*    buf_y)
{
  /* put initial input in stray buffer
  */


  }
      else {
      }
    }
  }
}

/* _next_handle_read(): process stream input
*/
void
_next_handle_read(uv_stream_t*    str_u,
                  ssize_t         len_i,
                  const uv_buf_t* buf_u)
{
  c3_d     len_d = (c3_d) len_i;
  u3_moat* mot_u = (void *)str_u;
  c3_y*    bas_y;

  if ( mot_u->rag_y ) {
    mot_u->rag_y = c3_realloc(mot_u->rag_y, mot_u->len_d + len_d);
    memcpy(mot_u->rag_y + mot_u->len_d, buf_y, len_d);
    c3_free(buf_y);
  }
  else {
    mot_u->rag_y = buf_y;
    mot_u->len_d = len_d;
  }
  _next_consume();
}

/* _next_

