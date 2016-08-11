/* j/4/in_int.c
**
*/
#include "all.h"


/* functions
*/
  u3_noun
  u3qdy_int(u3_noun a,
            u3_noun b)
  {
    if ( u3_nul == a ) {
      return u3k(u3_nul);
    }
    else if ( u3_nul == b ) {
      return u3k(u3_nul);
    }
    else {
      u3_noun l_a, n_a, r_a, lr_a;
      u3_noun l_b, n_b, r_b, lr_b;
      u3_noun c;

      if ( (c3n == u3r_cell(a, &n_a, &lr_a)) ) {
        return u3m_bail(c3__exit);
      }
      else if ( (c3n == u3r_cell(b, &n_b, &lr_b)) ) {
        return u3m_bail(c3__exit);
      }
      else {
        if ( c3y == u3qc_vyr(n_b, n_a) ) {
          c = a;    a = b;       b = c;
          c = n_a;  n_a = n_b;   n_b = c;
          c = lr_a; lr_a = lr_b; lr_b = c;
        }
        if ( c3n == u3r_cell(lr_a, &l_a, &r_a) ) {
          return u3m_bail(c3__exit);
        }
        else if ( c3n == u3r_cell(lr_b, &l_b, &r_b) ) {
          return u3m_bail(c3__exit);
        }
        else if ( c3y == u3r_sing(n_a, n_b) ) {
          return u3nt(u3k(n_a),
                      u3qdy_int(l_a, l_b),
                      u3qdy_int(r_a, r_b));
        }
        else if ( c3y == u3qc_hyr(n_b, n_a) ) {
          return u3qdy_uni(u3qdy_int(l_a,
                                     u3nt(n_b,
                                          l_b,
                                          u3_nul)),
                           u3qdy_int(a,
                                     r_b));
        }
        else {
          return u3qdy_uni(u3qdy_int(r_a,
                                     u3nt(n_b,
                                          u3_nul,
                                          r_b)),
                           u3qdy_int(a,
                                     l_b));
        }
      }
    }
  }
  u3_noun
  u3wdy_int(u3_noun cor)
  {
    u3_noun a, b;

    if ( c3n == u3r_mean(cor, u3x_sam, &b, u3x_con_sam, &a, 0) ) {
      return u3m_bail(c3__exit);
    } else {
      return u3qdy_int(a, b);
    }
  }
