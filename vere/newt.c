/* vere/newt.c
**
**  implements noun blob messages with trivial framing.
**
**  a message is a 64-bit little-endian byte count, followed
**  by the indicated number of bytes.  the bytes are the
**  the ++cue of of a noun.
**
**  the implementation is relatively inefficient and could
**  lose a few copies, mallocs, etc.
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

/* _newt_consume(): advance buffer processing.
*/
static void
_newt_consume(u3_moat* mot_u)
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
          mot_u->mes_u->qem_u = met_u;
        }
        mot_u->mes_u->len_d += met_u->len_d;

        /* free consumed stray bytes
        */
        mot_u->len_d = 0;
        c3_free(mot_u->rag_y);
      }
      else {
        /* no message, but enough stray bytes to fill in 
        ** a length; collect them and create a message.
        */
        if ( mot_u->len_d >= 8ULL ) {
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

          mot_u->mes_u = c3_malloc(sizeof(u3_mess));
          mot_u->mes_u->len_d = nel_d;
          mot_u->mes_u->has_d = 0;
          mot_u->mes_u->meq_u = mot_u->mes_u->qem_u = 0;

          if ( !mot_u->len_d ) {
            c3_free(mot_u->rag_y);
            mot_u->rag_y = 0;
          }
          else {
            /* remove consumed length from stray bytes
            */
            c3_y* buf_y = c3_malloc(mot_u->len_d); 
            
            memcpy(buf_y, mot_u->rag_y + 8, mot_u->len_d);

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
      c3_assert(!mot_u->len_d);

      /* if we have a message, deliver it
      */
      if ( mot_u->mes_u->has_d >= mot_u->mes_u->len_d ) {
        c3_d     len_d = mot_u->mes_u->len_d;
        c3_y*    buf_y = c3_malloc(len_d);
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

          mot_u->pok_f(mot_u, job);
        }

        /* return any spare meat to the rag
        */
        if ( met_u ) {
          /* this should have been claimed already; there should
          ** be no way to build up more than one extra buffer
          */
          c3_assert(!met_u->nex_u);

          /* somewhat wasteful copying, but this is rare
          */
          mot_u->rag_y = c3_malloc(met_u->len_d);
          memcpy(mot_u->rag_y, met_u->hun_y, met_u->len_d);
          mot_u->len_d = met_u->len_d;
        }

        /* free the message and its buffers
        */
        {
          met_u = mot_u->mes_u->meq_u;

          while ( met_u ) {
            u3_meat* nex_u = met_u;

            c3_free(met_u);
            met_u = nex_u;
          }
          c3_free(mot_u->mes_u);
          mot_u->mes_u = 0;
        }
      }

      /* continue; spare data may need processing
      */
      continue;
    }
    break;
  }
}

/* _raft_alloc(): libuv-style allocator for raft.
*/
static void
_newt_alloc(uv_handle_t* had_u, 
            size_t len_i,
            uv_buf_t* buf_u)
{
  void* ptr_v = c3_malloc(len_i);

  *buf_u = uv_buf_init(ptr_v, len_i);
}

/* _newt_read_cb(): stream input callback.
*/
void
_newt_read_cb(uv_stream_t*    str_u,
              ssize_t         len_i,
              const uv_buf_t* buf_u)
{
  c3_d     len_d = (c3_d) len_i;
  u3_moat* mot_u = (void *)str_u;

  if ( mot_u->rag_y ) {
    mot_u->rag_y = c3_realloc(mot_u->rag_y, mot_u->len_d + len_d);
    memcpy(mot_u->rag_y + mot_u->len_d, buf_u->base, len_d);
    c3_free(buf_u->base);
  }
  else {
    mot_u->rag_y = (c3_y *)buf_u->base;
    mot_u->len_d = len_d;
  }
  _newt_consume(mot_u);
}

/* u3_newt_read_start(): start stream reading.
*/
void
u3_newt_read_start(u3_moat* mot_u,
                   u3_poke  pok_f,
                   u3_bail  bal_f)
{
  c3_i err_i;

  mot_u->pok_f = pok_f;
  mot_u->bal_f = bal_f;
  mot_u->mes_u = 0;
  mot_u->len_d = 0;
  mot_u->rag_y = 0;

  err_i = uv_read_start((uv_stream_t*) &mot_u->pyp_u,
                        _newt_alloc,
                        _newt_read_cb);

  if ( err_i != 0 ) {
    mot_u->bal_f(mot_u, err_i);
  }
}

/* An unusual lameness in libuv.
*/
struct _u3_write_t {
  uv_write_t wri_u;
  u3_mojo*   moj_u;
  c3_y*      buf_y;
};

/* _newt_write_cb(): generic write callback.
*/
static void
_newt_write_cb(uv_write_t* wri_u, c3_i sas_i)
{
  struct _u3_write_t* req_u = (struct _u3_write_t*)wri_u;
  u3_mojo*            moj_u = req_u->moj_u;

  free(req_u->buf_y);
  free(req_u);

  if ( 0 != sas_i ) {
    moj_u->bal_f(moj_u, sas_i);
  }
}

/* u3_newt_write(): write noun to stream.
*/
void
u3_newt_write(u3_mojo* moj_u,
              u3_noun  job)
{
  u3_noun             jam   = u3ke_jam(job);
  c3_w                len_w = u3r_met(3, jam);
  c3_y*               buf_y = c3_malloc(len_w + 8);
  struct _u3_write_t* req_u = c3_malloc(sizeof(*req_u));
  uv_buf_t            buf_u;
  c3_i                err_i;

  /* write header; c3_d is futureproofing
  */
  buf_y[0] = ((len_w >> 0) & 0xff);
  buf_y[1] = ((len_w >> 8) & 0xff);
  buf_y[2] = ((len_w >> 16) & 0xff);
  buf_y[3] = ((len_w >> 24) & 0xff);
  buf_y[4] = buf_y[5] = buf_y[6] = buf_y[7] = 0;
  u3r_bytes(0, len_w, buf_y + 8, jam);

  req_u->buf_y = buf_y;
  buf_u.base = (c3_c*) buf_y;
  buf_u.len = len_w;

  if ( 0 != (err_i = uv_write((uv_write_t*)req_u,
                              (uv_stream_t*)&moj_u->pyp_u,
                              &buf_u,
                              1,
                              _newt_write_cb)) )
  {
    moj_u->bal_f(moj_u, err_i);
  }
}
