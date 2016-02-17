/* v/fuse.c
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

static void
_inode_init(void)
{
  u3_fuse* fus_u = &u3_Host.fus_u;
  {
    fus_u->rot_u.ino_i = FUSE_ROOT_ID;
    fus_u->rot_u
    fus_u->rot_u.val_u = 0;
    fus_u->rot_u.nam_c = strdup("/");
    fus_u->rot_u.pax_c = 0;
    fus_u->rot_u.val_u = 0;
    fus_u->rot_u.ref_w = 0x1fffffff;
    fus_u->rot_u.par_u = 0;
    fus_u->rot_u.kid_u = 0;
    fus_u->rot_u.nxt_u = 0;
  }
  {
    fus_u->ion_u.ino_i = FUSE_ROOT_ID + 1;
    fus_u->ion_u.len_w = 64;
    fus_u->ion_u.nod_u = calloc(64, sizeof(struct fnod *));
    fus_u->ion_u.nod_u[FUSE_ROOT_ID] = &fus_u->rot_u;
  }
}

/* _inode_get(): look up an inode.
*/
static u3_fnod*
_inode_get(fuse_ino_t ino_i)
{
  c3_assert(ino_i < fus_u->ion_u.ino_i);

  return fus_u->ion_u.nod_u[ino_i];
}

/* _inode_new(): create an inode.
*/
static u3_fnod*
_inode_new(void)
{
  u3_fnod* nod_u = calloc(1, sizeof(u3_fnod));

  nod_u->ino_i = fus_u->ion_u.ino_i;
  fus_u->ion_u.nod_u[nod_u->ion_i] = nod_u;
  fus_u->ion_u.ino_i++;

  if ( fus_u->ion_u.len_w == fus_u->ion_u.ion_i ) {
    fus_u->ion_u.len_w *= 2;
    fus_u->ion_u.nod_u = realloc(fus_u->ion_u.nod_u, 
                                 (fus_u->ion_u.len_w * 
                                  sizeof(struct fnod *)));
  }
  return nod_u;
}

/* _inode_stat(): fill stat buffer from inode; return c3y if available
*/
static c3_o
_inode_stat(struct u3_fnod* nod_u, struct stat* buf_u)
{
  memset(buf_u, 0, sizeof(struct stat));

  if ( nod_u->ext_c ) {
    if ( 0 == nod_u->val_u ) {
      return c3n;
    }
    else {
      buf_u->st_mode = S_IFREG | 0444;
      buf_u->st_nlink = 1;
      buf_u->st_size = val_u->siz_z;

      return c3y;
    }
  }
  else {
    buf_u->st_mode = S_IFDIR | 0555;
    buf_u->st_nlink = 2;

    return c3y;
  }
}

/* _inode_path(): map inode path to noun.
*/
static u3_noun
_inode_path(struct u3_fnod* nod_u, u3_noun end)
{
  if ( nod_u->par_u == 0 ) {
    return end; 
  }
  else {
    if ( nod_u->ext_c ) {
      end = u3nc(u3i_string(nod_u->ext_c), end);
    }
    end = u3nc(u3i_string(nod_u->nam_c), end);
 
    return _inode_path(nod_u->par_u, end);
  }
}

/* _arch_
/* _inode_load(): load inode value with peek.
*/
static c3_o
_inod_load(struct u3_fnod* nod_u)
{
  if ( u3_nul == u3A->own ) {
    return c3n;
  } 
  else {
    u3_noun our = u3dc("scot", 'p', u3k(u3h(u3A->own)));
    u3_noun hap = _inode_path(nod_u);

    if ( nod_u->ext_c ) {
      hap = u3nc(c3__cx, u3nq(our, c3__home, u3k(u3A->wen), hap));A
      val = 
    }
  }
}

	/**
	 * Initialize filesystem
	 *
	 * Called before any other filesystem method
	 *
	 * There's no reply to this function
	 *
	 * @param userdata the user data passed to fuse_lowlevel_new()
	 */
static void
_fuse_init(void*                  usr_v,
           struct fuse_conn_info* con_u)
{
  u3_fuse* fus_u = &u3_Host.fus_u;
  {
    _fine_init();
  }
}

	/**
	 * Look up a directory entry by name and get its attributes.
	 *
	 * Valid replies:
	 *   fuse_reply_entry
	 *   fuse_reply_err
	 *
	 * @param req request handle
	 * @param parent inode number of the parent directory
	 * @param name the name to look up
	 */
static void 
_fuse_ll_lookup(fuse_req_t  req_u, 
                fuse_ino_t  pno_i, 
                const c3_c* nam_c)
{
  u3_fuse* fus_u = &u3_Host.fus_u;
  {
    c3_c*    ext_c = strrchr(nam_c, '.');
    c3_w     len_w = (ext_c ? (ext_c - nam_c) : strlen(nam_c));
    u3_fnod* par_u = _inode_get(pno_i);
    u3_fnod* nod_u;

    //  Find, then make.
    {
      for ( nod_u = par_u->kid_u; nod_u; nod_u = nod_u->nex_u ) {
        if ( !strncmp(nam_c, nod_u->nam_c, len_w) &&
             ((!ext_c && !nod_u->ext_c) ||
              (ext_c && nod_u->ext_c && !strcmp(ext_c, nod_u->ext_c))) )
        {
          // reply nod_u->ino_i
        }
      }

      if ( !nod_u ) {
      }
    }

    nod_u = _inode_new();
    {
      nod_u->nam_c = malloc(len_w + 1);
      strncpy(nod_u->nam_c, nam_c, len_w);
      nod_u->nam_c[len_w] = 0;
    }
    nod_u->ext_c = strdup(ext_c);
    nod_u->par_u = par_u;
    nod_u->nex_u = par_u->kid_u;
    par_u->kid_u = nod_u;
  }
}

	/**
	 * Get file attributes
	 *
	 * Valid replies:
	 *   fuse_reply_attr
	 *   fuse_reply_err
	 *
	 * @param req request handle
	 * @param ino the inode number
	 * @param fi for future use, currently always NULL
	 */
static void
_fuse_ll_getattr(fuse_req_t             req_u,
                 fuse_ino_t             ino_i,
                 struct fuse_file_info* ffi_u)
{
  u3_fuse* fus_u = &u3_Host.fus_u;
  {
  }
}

	/**
	 * Read directory
	 *
	 * Send a buffer filled using fuse_add_direntry(), with size not
	 * exceeding the requested size.  Send an empty buffer on end of
	 * stream.
	 *
	 * fi->fh will contain the value set by the opendir method, or
	 * will be undefined if the opendir method didn't set any value.
	 *
	 * Valid replies:
	 *   fuse_reply_buf
	 *   fuse_reply_err
	 *
	 * @param req request handle
	 * @param ino the inode number
	 * @param size maximum number of bytes to send
	 * @param off offset to continue reading the directory stream
	 * @param fi file information
	 */
static void
_fuse_ll_readdir(fuse_req_t             req_u,
                 fuse_ino_t             ino_i,
                 c3_z                   siz_z,
                 c3_f                   off_f,
                 struct fuse_file_info* ffi_u)
{
  u3_fuse* fus_u = &u3_Host.fus_u;
  {
  }
}

	/**
	 * Open a file
	 *
	 * Open flags (with the exception of O_CREAT, O_EXCL, O_NOCTTY and
	 * O_TRUNC) are available in fi->flags.
	 *
	 * Filesystem may store an arbitrary file handle (pointer, index,
	 * etc) in fi->fh, and use this in other all other file operations
	 * (read, write, flush, release, fsync).
	 *
	 * Filesystem may also implement stateless file I/O and not store
	 * anything in fi->fh.
	 *
	 * There are also some flags (direct_io, keep_cache) which the
	 * filesystem may set in fi, to change the way the file is opened.
	 * See fuse_file_info structure in <fuse_common.h> for more details.
	 *
	 * Valid replies:
	 *   fuse_reply_open
	 *   fuse_reply_err
	 *
	 * @param req request handle
	 * @param ino the inode number
	 * @param fi file information
	 */
static void
_fuse_ll_open(fuse_req_t             req_u,
              fuse_ino_t             ino_i,
              struct fuse_file_info* ffi_u)
{
  u3_fuse* fus_u = &u3_Host.fus_u;
  {
  }
}

	/**
	 * Read data
	 *
	 * Read should send exactly the number of bytes requested except
	 * on EOF or error, otherwise the rest of the data will be
	 * substituted with zeroes.  An exception to this is when the file
	 * has been opened in 'direct_io' mode, in which case the return
	 * value of the read system call will reflect the return value of
	 * this operation.
	 *
	 * fi->fh will contain the value set by the open method, or will
	 * be undefined if the open method didn't set any value.
	 *
	 * Valid replies:
	 *   fuse_reply_buf
	 *   fuse_reply_err
	 *
	 * @param req request handle
	 * @param ino the inode number
	 * @param size number of bytes to read
	 * @param off offset to read from
	 * @param fi file information
	 */
static void
_fuse_ll_read(fuse_req_t             req_u,
              fuse_ino_t             ino_i,
              c3_z                   siz_z,
              c3_f                   off_f,
              struct fuse_file_info* ffi_u)
{
  u3_fuse* fus_u = &u3_Host.fus_u;
  {
  }
}

static struct fuse_lowlevel_ops fuse_api = {
  .init      = _fuse_ll_init,
  .lookup    = _fuse_ll_lookup, 
	.getattr   = _fuse__ll_getattr,
	.readdir   = _fuse__ll_readdir,
	.open		   = _fuse__ll_open,
	.read		   = _fuse__ll_read,
};

/* _fuse_poll_cb(): 
*/
static void
_fuse_poll_cb(uv_poll_t* wax_u, 
              c3_i       sas_i,
              c3_i       evt_i)
{
  u3_fuse* fus_u = &u3_Host.fus_u;

  uL(fprintf(uH, "fuse_poll_cb\n"));
  {
	  c3_z  buf_z = fuse_chan_bufsize(fus_u->cha_u);
    c3_y* buf_y = malloc(buf_z + 1);
    c3_i  res_i = fuse_chan_recv(fus_u->cha_u, buf_y, buf_z);

    if ( res_i < 0 ) {
      free_cmd(cmd);

			if ( (res_i != -EINTR) && (res_i != -EAGAIN) ) {
        uL(fprintf(uH, "fuse_poll_cb: error: %s\n", strerror(res_i)));
        c3_assert(0);
      }
    }
    else {
      uL(fprintf(uH, "fuse_session_process\n"));

      fuse_session_process(fus_u->sez_u,
                           buf_y,
                           buf_z,
                           fus_u->cha_u);
    }
  }
}

/* u3_fuse_io_init(): initialize FUSE.
*/
void
u3_fuse_io_init(void)
{
  u3_fuse* fus_u = &u3_Host.fus_u;

  fus_u->mnt_c = malloc(strlen(u3_Host.dir_c) + 16);
  strcpy(fus_u->mnt_c, u3_Host.dir_c);
  strncat(fus_u->mnt_c, "/.urb/fun", 14);

  ul(fprintf(uH, "fuse: mounting: %s\n", fus_u->mnt_c));

  if ( !(fus_u->cha_u = fuse_mount(fuse_mountpoint, 0)) ) {
    uL(fprintf(uH, "fuse: could not mount %s\n", fuse_mountpoint));
  }

  if ( !(fus_u->sez_u = fuse_lowlevel_new(0, fuse_api, sizeof(fuse_api), 0)) ) {
    uL(fprintf(uH, "fuse: could not create session\n"));
  }
  fuse_session_add_chan(fus_u->sez_u, fus_u->cha_u);

  {
    c3_i fid_i = fuse_chan_fd(fus_u->chan_a);

    uv_poll_init(u3L, &fus_u->wax_u, fid_i); 
    uv_poll_start(&fus_u->wax_u, UV_READABLE, _fuse_poll_cb);
  }
}


/* u3_fuse_io_exit(): shut down FUSE.
*/
void
u3_fuse_io_exit(void)
{
  u3_fuse* fus_u = &u3_Host.fus_u;

  uv_poll_stop(&fus_u->wax_u);

  fuse_session_remove_chan(fus_u->cha_u);
  fuse_session_destroy(fus_u->sez_u);
  fuse_unmount(fuse_mountpoint, fus_u->cha_u);

}
