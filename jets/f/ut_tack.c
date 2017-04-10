/* j/6/tack.c
**
*/
#include "all.h"

 u3_noun
  u3qfu_tack(u3_noun van,
             u3_noun sut,
             u3_noun hyp,
             u3_noun mur)
  {
    u3_noun von = u3i_molt(u3k(van), u3x_sam, u3k(sut), 0);
    u3_noun gat = u3j_hook(von, "tack");

    if ( (0x657b7fbb == u3r_mug(sut)) && 
         (0x6f0b4869 == u3r_mug(mur)) )
    {
      u3m_p("tack: hyp", hyp);
      u3m_p("h_sut", u3h(sut));
      u3m_p("h_mur", u3h(mur));

      u3qfu_dump(van, "sut", sut);
      u3qfu_dump(van, "mur", mur);
    }

    return u3n_kick_on(u3i_molt(gat, 
                                u3x_sam_2, 
                                u3k(hyp), 
                                u3x_sam_3, 
                                u3k(mur),
                                0));
  } 
