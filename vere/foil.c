/* vere/foil.c
**
**  This file is in the public domain.
*/

#include "all.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <setjmp.h>
#include <gmp.h>
#include <dirent.h>
#include <stdint.h>
#include <uv.h>
#include <termios.h>
#include <term.h>
#include <errno.h>
#include <libgen.h>
#include <ftw.h>

#include "vere/vere.h"

    /*  assumptions:
    **    all measurements are in chubs (double-words, c3_d, uint64_t).
    **    little-endian addressing is ASSUMED.
    **    
    **  framing:
    **    the last two chubs of a frame:
    **  
    **      {
    **        64-bit frame length
    **        {
    **          (high 32 bits) mug of frame
    **          (low 32 bits) mug of current address
    **        }
    **      }
    **
    **    we can scan for one of these frames with very low probability
    **    of a false positive.  we always write to and read from the end
    **    of a file.  a frame position points to its end.
    **
    **  protocol:
    **    once the callback is called, all results are fully fsynced.
    **    all callbacks are optional and can be passed 0.
    */
 
    /* u3_foil: abstract chub-addressed file.
    */
      typedef struct _u3_foil {
        uv_file*         fil_u;             //  libuv file handle, if open
        struct _u3_dire* dir_u;             //  parent directory
        c3_c*            nam_c;             //  name within parent
        c3_d             end_d;             //  end of file
        struct _u3_foil* nex_u;             //  next in parent list
      } u3_foil;

    /* u3_dire: simple directory state.
    */
      typedef struct _u3_dire {
        c3_c*    pax_c;                     //  path of directory
        u3_foil* has_u;                     //  
      } u3_dire;


/* u3_foil_folder(): load directory.
*/
void
u3_foil_folder(void (*fun_f)(u3_dire*,     //  directory object, or 0
                             void*),       //  context pointer
               void*  vod_p,               //  context pointer
               c3_c*  pax_c)               //  absolute path
{
}

/* u3_foil_create(): create a new, empty file, not syncing.
*/
void
u3_foil_create(void   (*fun_f)(u3_foil*,    //  file object
                               void*),      //  context pointer
               void*    vod_p,              //  context pointer
               u3_dire* dir_u,              //  directory
               c3_c*    nam_c)              //  name of new file
{
}

/* u3_foil_absorb(): absorb logfile, truncating to last good frame.
*/
void
u3_foil_absorb(void   (*fun_f)(u3_foil*,    //  file object
                               void*,       //  context pointer
                               c3_d),       //  end pointer
               void*    vod_p,              //  context pointer
               u3_foil* fol_u)              //  file
{
}

/* u3_foil_delete(): delete a file; free descriptor.
*/
void
u3_foil_delete(void   (*fun_f)(void*),      //  context pointer
               void*    vod_p,              //  context pointer
               u3_foil* fol_u)              //  file to delete
{
}

/* u3_foil_append(): write a frame at the end of a file, freeing.
*/
void
u3_foil_append(void   (*fun_f)(u3_foil*,    //  file object
                               void*,       //  context pointer
                               c3_d)        //  end of new frame
               u3_foil* fol_u,              //  file
               void*    vod_p,              //  context pointer
               c3_d     pos_d,              //  end of current frame
               c3_d*    buf_d,              //  chub buffer to write from
               c3_d     len_d)              //  length in chubs
{
}

/* u3_foil_reveal(): read the frame before a position, allocating.
*/
void
u3_foil_reveal(void   (*fun_f)(void*,       //  context pointer
                               u3_foil*,    //  file object
                               c3_d*,       //  read buffer (to free)
                               c3_d,        //  buffer length
                               c3_d),       //  end of previous frame
               u3_foil* fol_u,              //  file
               void*    vod_p,              //  context pointer
               c3_d     pos_d,              //  end of current frame
               c3_d     len_d)              //  length in chubs
{
}

/* u3_foil_commit(): reveal from one file, append to another.
*/

void
u3_foil_commit(void   (*fun_f)(void*,       //  context pointer
                               u3_foil*,    //  file from
                               c3_d,        //  previous from
                               u3_foil*,    //  file to
                               c3_d)        //  end of to
               void*    vod_p,              //  context pointer
               u3_foil* del_u,              //  file from
               c3_d     del_d,              //  end of from frame
               u3_foil* unt_u,              //  file to
               c3_d     unt_d)              //  end of to frame
{
}

/* u3_foil_invent(): create a new file with one frame.
*/
void
u3_foil_invent(void   (*fun_f)(void*,       //  context pointer
                               u3_foil*)    //  new file
               void*    vod_p,              //  context pointer
               u3_dire* dir_u,              //  directory
               c3_c*    nam_c,              //  filename
               c3_d*    buf_d,              //  buffer (to free)
               c3_d     len_d)              //  length
{
}

