/* j/4/in_has.c
**
*/
#include "all.h"


/* functions
*/
  u3_noun
  u3qdi_dig(u3_noun a,
            u3_noun b)
  {
    u3_noun axe = 1;

    while  ( 1 ) {
    if ( u3_nul == a ) {
      u3z(axe);
      return u3_nul;
    } 
    else {
      u3_noun l_a, n_a, r_a;

      if ( (c3n == u3r_mean(a, 2, &n_a, 6, &l_a, 7, &r_a, 0)) ) {
        return u3m_bail(c3__exit);
      }
      else {
        if ( (c3y == u3r_sing(b, n_a)) ) {
          u3_noun nex = u3qc_peg(
          return u3nc(u3_nul, axe);
        }
        else {
          if ( c3y == u3qc_hor(b, n_a) ) {
            u3_noun nex = u3qc_peg(axe, 
            a = l_a;
          }
          else {
            a = r_a;
          }
          continue;
        }
      }
    }
  }

  u3_noun
  u3wdi_dig(u3_noun cor)
  {
    u3_noun a, b;

    if ( c3n == u3r_mean(cor, u3x_sam, &b, u3x_con_sam, &a, 0) ) {
      return u3m_bail(c3__exit);
    } else {
      return u3qdi_dig(a, b);
    }
  }
  u3_noun
  u3kdi_dig(u3_noun a,
            u3_noun b)
  {
    u3_weak c = u3qdi_dig(a, b);

    u3z(a); u3z(b);
    if ( u3_none == c ) {
      return u3m_bail(c3__exit);
    }
    else return c;
  }

