/* j/6/ut_repo.c
**
*/
#include "all.h"


/* logic
*/
  u3_noun
  u3qxu_repo(u3_noun van,
             u3_noun sut)
  {
    u3_noun p_sut, q_sut;

    if ( c3n == u3du(sut) ) switch ( sut ) {
      default: return u3k(sut);

      case c3__noun: {
        return u3kx_fork(u3nt(u3nt(c3__atom, u3_blip, u3_nul),
                              u3nt(c3__cell, c3__noun, c3__noun),
                              u3_nul));
      }
    }
    else switch ( u3h(sut) ) {
      default: {
        u3m_p("head", u3h(sut));
        return u3m_error("repo-flat");
      }

      case c3__core: {
        if ( c3n == u3r_cell(u3t(sut), &p_sut, &q_sut) ) {
          return u3m_bail(c3__fail);
        } else {
          return u3nt(c3__cell, c3__noun, u3k(p_sut));
        }
      }
      case c3__face: {
        if ( c3n == u3r_cell(u3t(sut), &p_sut, &q_sut)) {
          return u3m_bail(c3__fail);
        } else {
          return u3k(q_sut);
        }
      }
      case c3__hold: {
        if ( c3n == u3r_cell(u3t(sut), &p_sut, &q_sut)) {
          return u3m_bail(c3__fail);
        } else {
          u3_noun old = u3nc(u3nc(u3k(p_sut), u3k(q_sut)), u3_nul);
          u3_noun ret;
 
          ret = u3qxu_rest(van, sut, old);
          u3z(old);
          return ret;
        }
      }
    }
  }

  u3_noun
  u3wxu_repo(u3_noun cor)
  {
    u3_noun sut;

    if ( u3_none == (sut = u3r_at(u3x_sam, cor)) ) {
      return u3m_bail(c3__fail);
    } else {
      return u3qxu_repo(cor, sut);
    }
  }
