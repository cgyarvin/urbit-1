/* j/tree.c
**
*/
#include "all.h"

  static u3j_harm _hex_down_mark_a[] = {{".2", u3wg_down, c3y}, {}};
    static u3j_core _hex_down_d[] =
      { { "mark", _hex_down_mark_a },
        {}
      };

  static u3j_harm _hex__lore_a[] = {{".2", u3we_lore}, {}};
  static u3j_harm _hex__loss_a[] = {{".2", u3we_loss}, {}};
  static u3j_harm _hex__lune_a[] = {{".2", u3we_lune}, {}};

static u3j_harm _hex__coed__ed_puck_a[] = {{".2", u3wee_puck}, {}};
static u3j_harm _hex__coed__ed_sign_a[] = {{".2", u3wee_sign}, {}};
static u3j_harm _hex__coed__ed_veri_a[] = {{".2", u3wee_veri}, {}};
static u3j_harm _hex__coed__ed_shar_a[] = {{".2", u3wee_shar}, {}};

static u3j_core _hex__coed__ed_d[] =
  { { "sign", _hex__coed__ed_sign_a },
    { "puck", _hex__coed__ed_puck_a },
    { "veri", _hex__coed__ed_veri_a },
    { "shar", _hex__coed__ed_shar_a },
    {}
  };
static u3j_core _hex__coed_d[] =
  { { "ed", 0, _hex__coed__ed_d },
    {}
  };

static u3j_harm _hex__scr_hsh_a[] = {{".2", u3wes_hsh}, {}};
static u3j_harm _hex__scr_hsl_a[] = {{".2", u3wes_hsl}, {}};
static u3j_harm _hex__scr_pbk_a[] = {{".2", u3wes_pbk}, {}};
static u3j_harm _hex__scr_pbl_a[] = {{".2", u3wes_pbl}, {}};
static u3j_core _hex__scr_d[] =
  {  { "hsh", _hex__scr_hsh_a },
     { "hsl", _hex__scr_hsl_a },
     { "pbk", _hex__scr_pbk_a },
     { "pbl", _hex__scr_pbl_a },
     {}
  };

static u3j_harm _hex__aes_ecba_en_a[] = {{".2", u3wea_ecba_en}, {}};
static u3j_harm _hex__aes_ecba_de_a[] = {{".2", u3wea_ecba_de}, {}};
static u3j_harm _hex__aes_ecbb_en_a[] = {{".2", u3wea_ecbb_en}, {}};
static u3j_harm _hex__aes_ecbb_de_a[] = {{".2", u3wea_ecbb_de}, {}};
static u3j_harm _hex__aes_ecbc_en_a[] = {{".2", u3wea_ecbc_en}, {}};
static u3j_harm _hex__aes_ecbc_de_a[] = {{".2", u3wea_ecbc_de}, {}};

static u3j_harm _hex__aes_cbca_en_a[] = {{".2", u3wea_cbca_en}, {}};
static u3j_harm _hex__aes_cbca_de_a[] = {{".2", u3wea_cbca_de}, {}};
static u3j_harm _hex__aes_cbcb_en_a[] = {{".2", u3wea_cbcb_en}, {}};
static u3j_harm _hex__aes_cbcb_de_a[] = {{".2", u3wea_cbcb_de}, {}};
static u3j_harm _hex__aes_cbcc_en_a[] = {{".2", u3wea_cbcc_en}, {}};
static u3j_harm _hex__aes_cbcc_de_a[] = {{".2", u3wea_cbcc_de}, {}};

static u3j_core _hex__aes_ecba_d[] =
  { { "en", _hex__aes_ecba_en_a },
    { "de", _hex__aes_ecba_de_a },
    {}
  };

static u3j_core _hex__aes_ecbb_d[] =
  { { "en", _hex__aes_ecbb_en_a },
    { "de", _hex__aes_ecbb_de_a },
    {}
  };

static u3j_core _hex__aes_ecbc_d[] =
  { { "en", _hex__aes_ecbc_en_a },
    { "de", _hex__aes_ecbc_de_a },
    {}
  };

static u3j_core _hex__aes_cbca_d[] =
  { { "en", _hex__aes_cbca_en_a },
    { "de", _hex__aes_cbca_de_a },
    {}
  };

static u3j_core _hex__aes_cbcb_d[] =
  { { "en", _hex__aes_cbcb_en_a },
    { "de", _hex__aes_cbcb_de_a },
    {}
  };

static u3j_core _hex__aes_cbcc_d[] =
  { { "en", _hex__aes_cbcc_en_a },
    { "de", _hex__aes_cbcc_de_a },
    {}
  };

static u3j_core _hex__aes_d[] =
  { { "ecba", 0, _hex__aes_ecba_d },
    { "ecbb", 0, _hex__aes_ecbb_d },
    { "ecbc", 0, _hex__aes_ecbc_d },
    { "cbca", 0, _hex__aes_cbca_d },
    { "cbcb", 0, _hex__aes_cbcb_d },
    { "cbcc", 0, _hex__aes_cbcc_d },
    {}
  };

static u3j_core _hex_d[] =
  { { "down", 0, _hex_down_d },
    { "coed", 0, _hex__coed_d },
    { "scr", 0, _hex__scr_d },
    { "aes", 0, _hex__aes_d },
    { "lore", _hex__lore_a },
    { "loss", _hex__loss_a },
    { "lune", _hex__lune_a },
    {}
  };

/* layer five
*/
  static u3j_harm _pen__cell_a[] = {{".2", u3wf_cell}, {}};
  static u3j_harm _pen__comb_a[] = {{".2", u3wf_comb}, {}};
  static u3j_harm _pen__cons_a[] = {{".2", u3wf_cons}, {}};
  static u3j_harm _pen__core_a[] = {{".2", u3wf_core}, {}};
  static u3j_harm _pen__face_a[] = {{".2", u3wf_face}, {}};
  static u3j_harm _pen__fitz_a[] = {{".2", u3wf_fitz}, {}};
  static u3j_harm _pen__flan_a[] = {{".2", u3wf_flan}, {}};
  static u3j_harm _pen__flip_a[] = {{".2", u3wf_flip}, {}};
  static u3j_harm _pen__flor_a[] = {{".2", u3wf_flor}, {}};
  static u3j_harm _pen__fork_a[] = {{".2", u3wf_fork}, {}};
  static u3j_harm _pen__hike_a[] = {{".2", u3wf_hike}, {}};
  static u3j_harm _pen__look_a[] = {{".2", u3wf_look}, {}};

    static u3j_harm _pen__ut_burn_a[] = {{".2", u3wfu_burn}, {}};
    static u3j_harm _pen__ut_conk_a[] = {{".2", u3wfu_conk}, {}};
    static u3j_harm _pen__ut_crop_a[] = {{".2", u3wfu_crop}, {}};
    // static u3j_harm _pen__ut_fire_a[] = {{".2", u3wfu_fire}, {}};
    static u3j_harm _pen__ut_fond_a[] = {{".2", u3wfu_fond}, {}};
    static u3j_harm _pen__ut_fish_a[] = {{".2", u3wfu_fish}, {}};
    static u3j_harm _pen__ut_fuse_a[] = {{".2", u3wfu_fuse}, {}};
    static u3j_harm _pen__ut_mint_a[] = {{".2", u3wfu_mint}, {}};
    static u3j_harm _pen__ut_mull_a[] = {{".2", u3wfu_mull}, {}};
    static u3j_harm _pen__ut_nest_a[] = {{".2", u3wfu_nest}, {}};
    static u3j_harm _pen__ut_peek_a[] = {{".2", u3wfu_peek}, {}};
    static u3j_harm _pen__ut_play_a[] = {{".2", u3wfu_play}, {}};
    static u3j_harm _pen__ut_rest_a[] = {{".2", u3wfu_rest}, {}};
    static u3j_harm _pen__ut_toss_a[] = {{".2", u3wfu_toss}, {}};
    static u3j_harm _pen__ut_wrap_a[] = {{".2", u3wfu_wrap}, {}};
  static u3j_core _pen__ut_d[] =
    { 
      { "burn", _pen__ut_burn_a },
      { "conk", _pen__ut_conk_a },
      { "crop", _pen__ut_crop_a },
      { "fond", _pen__ut_fond_a },
    //  { "fire", _pen__ut_fire_a },
      { "fish", _pen__ut_fish_a },
      { "fuse", _pen__ut_fuse_a },
      { "mint", _pen__ut_mint_a },
      { "mull", _pen__ut_mull_a },
      { "nest", _pen__ut_nest_a },
      { "peek", _pen__ut_peek_a },
      { "play", _pen__ut_play_a },
      { "rest", _pen__ut_rest_a },
      { "toss", _pen__ut_toss_a },
      { "wrap", _pen__ut_wrap_a },
      {}
    };
  static u3j_harm _pen__ut_a[] = 
    { {"burn", u3wfu_burn},
      {"repo", u3wfu_repo},
      {}
    };

#if 0
  static u3j_harm _pen__ap_a[] = 
    { {"open", u3wfp_open},
      {"rake", u3wfp_rake},
      {}
    };
  static u3j_harm _pen__al_a[] = 
    { {"bunt", u3wfl_bunt},
      {"whip", u3wfl_whip},
      {}
    };
#endif

static u3j_core _pen__d[] =
  { { "hex", 0, _hex_d },

    { "cell", _pen__cell_a },
    { "comb", _pen__comb_a },
    { "cons", _pen__cons_a },
    { "core", _pen__core_a },
    { "face", _pen__face_a },
    { "fitz", _pen__fitz_a },
    { "flan", _pen__flan_a },
    { "flip", _pen__flip_a },
    { "flor", _pen__flor_a },
    { "fork", _pen__fork_a },
    { "hike", _pen__hike_a },
    { "look", _pen__look_a },

    // { "ap", _pen__ap_a },
    // { "al", _pen__al_a },
    { "ut", _pen__ut_a, _pen__ut_d },

    {}
  };

/* layer four
*/
  static u3j_harm _qua_trip_a[] = {{".2", u3we_trip}, {}};

  static u3j_harm _qua__po_ind_a[] = {{".2", u3wcp_ind}, {}};
  static u3j_harm _qua__po_ins_a[] = {{".2", u3wcp_ins}, {}};
  static u3j_harm _qua__po_tod_a[] = {{".2", u3wcp_tod}, {}};
  static u3j_harm _qua__po_tos_a[] = {{".2", u3wcp_tos}, {}};
    static u3j_core _qua__po_d[] =
      { { "ind", _qua__po_ind_a },
        { "ins", _qua__po_ins_a },
        { "tod", _qua__po_tod_a },
        { "tos", _qua__po_tos_a },
        {}
      };

  static u3j_harm _qua__bend_fun_a[] = {{".2", u3we_bend_fun}, {}};
    static u3j_core _qua__bend_d[] =
      { { "fun", _qua__bend_fun_a },
        {}
      };

  static u3j_harm _qua__cold_fun_a[] = {{".2", u3we_cold_fun}, {}};
    static u3j_core _qua__cold_d[] =
      { { "fun", _qua__cold_fun_a },
        {}
      };

  static u3j_harm _qua__cook_fun_a[] = {{".2", u3we_cook_fun}, {}};
    static u3j_core _qua__cook_d[] =
      { { "fun", _qua__cook_fun_a },
        {}
      };

  static u3j_harm _qua__comp_fun_a[] = {{".2", u3we_comp_fun}, {}};
    static u3j_core _qua__comp_d[] =
      { { "fun", _qua__comp_fun_a },
        {}
      };

  static u3j_harm _qua__easy_fun_a[] = {{".2", u3we_easy_fun}, {}};
    static u3j_core _qua__easy_d[] =
      { { "fun", _qua__easy_fun_a },
        {}
      };

  static u3j_harm _qua__glue_fun_a[] = {{".2", u3we_glue_fun}, {}};
    static u3j_core _qua__glue_d[] =
      { { "fun", _qua__glue_fun_a },
        {}
      };

  static u3j_harm _qua__here_fun_a[] = {{".2", u3we_here_fun}, {}};
    static u3j_core _qua__here_d[] =
      { { "fun", _qua__here_fun_a },
        {}
      };

  static u3j_harm _qua__just_fun_a[] = {{".2", u3we_just_fun}, {}};
    static u3j_core _qua__just_d[] =
      { { "fun", _qua__just_fun_a },
        {}
      };

  static u3j_harm _qua__mask_fun_a[] = {{".2", u3we_mask_fun}, {}};
    static u3j_core _qua__mask_d[] =
      { { "fun", _qua__mask_fun_a },
        {}
      };

  static u3j_harm _qua__shim_fun_a[] = {{".2", u3we_shim_fun}, {}};
    static u3j_core _qua__shim_d[] =
      { { "fun", _qua__shim_fun_a },
        {}
      };

  static u3j_harm _qua__stag_fun_a[] = {{".2", u3we_stag_fun}, {}};
    static u3j_core _qua__stag_d[] =
      { { "fun", _qua__stag_fun_a },
        {}
      };

  static u3j_harm _qua__stew_fun_a[] = {{".2", u3we_stew_fun}, {}};
    static u3j_core _qua__stew_d[] =
      { { "fun", _qua__stew_fun_a },
        {}
      };
 
  static u3j_harm _qua__stir_fun_a[] = {{".2", u3we_stir_fun}, {}};
    static u3j_core _qua__stir_d[] =
      { { "fun", _qua__stir_fun_a },
        {}
      };

  static u3j_harm _qua_pfix_a[] = {{".2", u3we_pfix}, {}};
  static u3j_harm _qua_plug_a[] = {{".2", u3we_plug}, {}};
  static u3j_harm _qua_pose_a[] = {{".2", u3we_pose}, {}};
  static u3j_harm _qua_sfix_a[] = {{".2", u3we_sfix}, {}};

  static u3j_harm _qua_mink_a[] = {{".2", u3we_mink}, {}};
  static u3j_harm _qua_mule_a[] = {{".2", u3we_mule}, {}};

static u3j_core _qua_d[] =
  { { "pen", 0, _pen__d },

    { "po", 0, _qua__po_d },

    { "trip", _qua_trip_a },

    { "bend", 0, _qua__bend_d },
    { "cold", 0, _qua__cold_d },
    { "comp", 0, _qua__comp_d },
    { "cook", 0, _qua__cook_d },
    { "easy", 0, _qua__easy_d },
    { "glue", 0, _qua__glue_d },
    { "here", 0, _qua__here_d },
    { "just", 0, _qua__just_d },
    { "mask", 0, _qua__mask_d },
    { "shim", 0, _qua__shim_d },
    { "stag", 0, _qua__stag_d },
    { "stew", 0, _qua__stew_d },
    { "stir", 0, _qua__stir_d },

    { "pfix", _qua_pfix_a },
    { "plug", _qua_plug_a },
    { "pose", _qua_pose_a },
    { "sfix", _qua_sfix_a },

    { "mink", _qua_mink_a },
    { "mule", _qua_mule_a },
    {}
  };

/* layer three
*/
    static u3j_harm _tri__rd_add_a[] = {{".2", u3wer_add}, {}};
    static u3j_harm _tri__rd_sub_a[] = {{".2", u3wer_sub}, {}};
    static u3j_harm _tri__rd_mul_a[] = {{".2", u3wer_mul}, {}};
    static u3j_harm _tri__rd_div_a[] = {{".2", u3wer_div}, {}};
    static u3j_harm _tri__rd_sqt_a[] = {{".2", u3wer_sqt}, {}};
    static u3j_harm _tri__rd_fma_a[] = {{".2", u3wer_fma}, {}};
    static u3j_harm _tri__rd_lth_a[] = {{".2", u3wer_lth}, {}};
    static u3j_harm _tri__rd_lte_a[] = {{".2", u3wer_lte}, {}};
    static u3j_harm _tri__rd_equ_a[] = {{".2", u3wer_equ}, {}};
    static u3j_harm _tri__rd_gte_a[] = {{".2", u3wer_gte}, {}};
    static u3j_harm _tri__rd_gth_a[] = {{".2", u3wer_gth}, {}};
  static u3j_core _tri__rd_d[] =
    { { "add", _tri__rd_add_a },
      { "sub", _tri__rd_sub_a },
      { "mul", _tri__rd_mul_a },
      { "div", _tri__rd_div_a },
      { "sqt", _tri__rd_sqt_a },
      { "fma", _tri__rd_fma_a },
      { "lth", _tri__rd_lth_a },
      { "lte", _tri__rd_lte_a },
      { "equ", _tri__rd_equ_a },
      { "gte", _tri__rd_gte_a },
      { "gth", _tri__rd_gth_a },
      {}
    };

    static u3j_harm _tri__cofl__drg_a[] = {{".2", u3wef_drg}, {}};
    static u3j_harm _tri__cofl__lug_a[] = {{".2", u3wef_lug}, {}};
  static u3j_core _tri__cofl_d[] =
    { { "drg", _tri__cofl__drg_a },
      { "lug", _tri__cofl__lug_a },
      {}
    };

    static u3j_harm _tri__rs_add_a[] = {{".2", u3wet_add}, {}};
    static u3j_harm _tri__rs_sub_a[] = {{".2", u3wet_sub}, {}};
    static u3j_harm _tri__rs_mul_a[] = {{".2", u3wet_mul}, {}};
    static u3j_harm _tri__rs_div_a[] = {{".2", u3wet_div}, {}};
    static u3j_harm _tri__rs_sqt_a[] = {{".2", u3wet_sqt}, {}};
    static u3j_harm _tri__rs_fma_a[] = {{".2", u3wet_fma}, {}};
    static u3j_harm _tri__rs_lth_a[] = {{".2", u3wet_lth}, {}};
    static u3j_harm _tri__rs_lte_a[] = {{".2", u3wet_lte}, {}};
    static u3j_harm _tri__rs_equ_a[] = {{".2", u3wet_equ}, {}};
    static u3j_harm _tri__rs_gte_a[] = {{".2", u3wet_gte}, {}};
    static u3j_harm _tri__rs_gth_a[] = {{".2", u3wet_gth}, {}};
  static u3j_core _tri__rs_d[] =
    { { "add", _tri__rs_add_a },
      { "sub", _tri__rs_sub_a },
      { "mul", _tri__rs_mul_a },
      { "div", _tri__rs_div_a },
      { "sqt", _tri__rs_sqt_a },
      { "fma", _tri__rs_fma_a },
      { "lth", _tri__rs_lth_a },
      { "lte", _tri__rs_lte_a },
      { "equ", _tri__rs_equ_a },
      { "gte", _tri__rs_gte_a },
      { "gth", _tri__rs_gth_a },
      {}
    };

    static u3j_harm _tri__rq_add_a[] = {{".2", u3weq_add}, {}};
    static u3j_harm _tri__rq_sub_a[] = {{".2", u3weq_sub}, {}};
    static u3j_harm _tri__rq_mul_a[] = {{".2", u3weq_mul}, {}};
    static u3j_harm _tri__rq_div_a[] = {{".2", u3weq_div}, {}};
    static u3j_harm _tri__rq_sqt_a[] = {{".2", u3weq_sqt}, {}};
    static u3j_harm _tri__rq_fma_a[] = {{".2", u3weq_fma}, {}};
    static u3j_harm _tri__rq_lth_a[] = {{".2", u3weq_lth}, {}};
    static u3j_harm _tri__rq_lte_a[] = {{".2", u3weq_lte}, {}};
    static u3j_harm _tri__rq_equ_a[] = {{".2", u3weq_equ}, {}};
    static u3j_harm _tri__rq_gte_a[] = {{".2", u3weq_gte}, {}};
    static u3j_harm _tri__rq_gth_a[] = {{".2", u3weq_gth}, {}};
  static u3j_core _tri__rq_d[] =
    { { "add", _tri__rq_add_a },
      { "sub", _tri__rq_sub_a },
      { "mul", _tri__rq_mul_a },
      { "div", _tri__rq_div_a },
      { "sqt", _tri__rq_sqt_a },
      { "fma", _tri__rq_fma_a },
      { "lth", _tri__rq_lth_a },
      { "lte", _tri__rq_lte_a },
      { "equ", _tri__rq_equ_a },
      { "gte", _tri__rq_gte_a },
      { "gth", _tri__rq_gth_a },
      {}
    };

    static u3j_harm _tri__aesc_en_a[] = {{".2", u3wea_en}, {}};
    static u3j_harm _tri__aesc_de_a[] = {{".2", u3wea_de}, {}};
  static u3j_core _tri__aesc_d[] =
    { { "en", _tri__aesc_en_a },
      { "de", _tri__aesc_de_a },
      {}
    };

    static u3j_harm _tri__og_raw_a[] = {{".2", u3weo_raw}, {}};
  static u3j_core _tri__og_d[] =
    { { "raw", _tri__og_raw_a },
      {}
    };

  static u3j_harm _tri_shax_a[] = {{".2", u3we_shax}, {}};
  static u3j_harm _tri_shay_a[] = {{".2", u3we_shay}, {}};
  static u3j_harm _tri_shas_a[] = {{".2", u3we_shas}, {}};
  static u3j_harm _tri_shal_a[] = {{".2", u3we_shal}, {}};

static u3j_core _tri_d[] =
  { { "qua", 0, _qua_d },

    { "cofl", 0, _tri__cofl_d },
    { "rd", 0, _tri__rd_d },
    { "rs", 0, _tri__rs_d },
    { "rq", 0, _tri__rq_d },
    { "og", 0, _tri__og_d },
    { "shax", _tri_shax_a },
    { "shay", _tri_shay_a },
    { "shas", _tri_shas_a },
    { "shal", _tri_shal_a },
    { "aesc", 0, _tri__aesc_d },

    {}
  };

/* layer two
*/
  static u3j_harm _two_flop_a[] = {{".2", u3wb_flop, c3y}, {}};
  static u3j_harm _two_lent_a[] = {{".2", u3wb_lent, c3y}, {}};
  static u3j_harm _two_levy_a[] = {{".2", u3wb_levy, c3y}, {}};
  static u3j_harm _two_lien_a[] = {{".2", u3wb_lien, c3y}, {}};
  static u3j_harm _two_murn_a[] = {{".2", u3wb_murn, c3y}, {}};
  static u3j_harm _two_need_a[] = {{".2", u3wb_need, c3y}, {}};
  static u3j_harm _two_reap_a[] = {{".2", u3wb_reap, c3y}, {}};
  static u3j_harm _two_reel_a[] = {{".2", u3wb_reel, c3y}, {}};
  static u3j_harm _two_roll_a[] = {{".2", u3wb_roll, c3y}, {}};
  static u3j_harm _two_skid_a[] = {{".2", u3wb_skid, c3y}, {}};
  static u3j_harm _two_skim_a[] = {{".2", u3wb_skim, c3y}, {}};
  static u3j_harm _two_skip_a[] = {{".2", u3wb_skip, c3y}, {}};
  static u3j_harm _two_scag_a[] = {{".2", u3wb_scag, c3y}, {}};
  static u3j_harm _two_slag_a[] = {{".2", u3wb_slag, c3y}, {}};
  static u3j_harm _two_snag_a[] = {{".2", u3wb_snag, c3y}, {}};
  // static u3j_harm _two_sort_a[] = {{".2", u3wb_sort, c3y}, {}};
  static u3j_harm _two_turn_a[] = {{".2", u3wb_turn, c3y}, {}};
  static u3j_harm _two_weld_a[] = {{".2", u3wb_weld, c3y}, {}};

  static u3j_harm _two_bex_a[] = {{".2", u3wc_bex, c3y}, {}};
  static u3j_harm _two_xeb_a[] = {{".2", u3wc_xeb, c3y}, {}};
  static u3j_harm _two_can_a[] = {{".2", u3wc_can, c3y}, {}};
  static u3j_harm _two_cat_a[] = {{".2", u3wc_cat, c3y}, {}};
  static u3j_harm _two_cut_a[] = {{".2", u3wc_cut, c3y}, {}};
  static u3j_harm _two_end_a[] = {{".2", u3wc_end, c3y}, {}};
  static u3j_harm _two_lsh_a[] = {{".2", u3wc_lsh, c3y}, {}};
  static u3j_harm _two_met_a[] = {{".2", u3wc_met, c3y}, {}};
  static u3j_harm _two_rap_a[] = {{".2", u3wc_rap, c3y}, {}};
  static u3j_harm _two_rip_a[] = {{".2", u3wc_rip, c3y}, {}};
  static u3j_harm _two_rsh_a[] = {{".2", u3wc_rsh, c3y}, {}};

  static u3j_harm _two_con_a[] = {{".2", u3wc_con, c3y}, {}};
  static u3j_harm _two_dis_a[] = {{".2", u3wc_dis, c3y}, {}};
  static u3j_harm _two_mix_a[] = {{".2", u3wc_mix, c3y}, {}};

  static u3j_harm _two_mug_a[] = {{".2", u3wc_mug, c3y}, {}};
  static u3j_harm _two_muk_a[] = {{".2", u3wc_muk, c3y}, {}};
  static u3j_harm _two_mum_a[] = {{".2", u3wc_mum, c3y}, {}};

  static u3j_harm _two_dor_a[] = {{".2", u3wc_dor, c3y}, {}};
  static u3j_harm _two_gor_a[] = {{".2", u3wc_gor, c3y}, {}};
  static u3j_harm _two_gyr_a[] = {{".2", u3wc_gyr, c3n}, {}};
  static u3j_harm _two_hor_a[] = {{".2", u3wc_hor, c3y}, {}};
  static u3j_harm _two_hyr_a[] = {{".2", u3wc_hyr, c3y}, {}};
  static u3j_harm _two_vor_a[] = {{".2", u3wc_vor, c3y}, {}};
  static u3j_harm _two_vyr_a[] = {{".2", u3wc_vyr, c3y}, {}};

  static u3j_harm _two_pow_a[] = {{".2", u3wc_pow, c3y}, {}};
  static u3j_harm _two_sqt_a[] = {{".2", u3wc_sqt, c3y}, {}};

    static u3j_harm _two__in_bif_a[] = {{".2", u3wdi_bif}, {}};
    static u3j_harm _two__in_dif_a[] = {{".2", u3wdi_dif}, {}};
    static u3j_harm _two__in_gas_a[] = {{".2", u3wdi_gas}, {}};
    static u3j_harm _two__in_has_a[] = {{".2", u3wdi_has}, {}};
    static u3j_harm _two__in_mer_a[] = {{".2", u3wdi_mer}, {}};
    // static u3j_harm _two__in_int_a[] = {{".2", u3wdi_int}, {}};
    static u3j_harm _two__in_put_a[] = {{".2", u3wdi_put}, {}};
    static u3j_harm _two__in_tap_a[] = {{".2", u3wdi_tap}, {}};
    // static u3j_harm _two__in_uni_a[] = {{".2", u3wdi_uni}, {}};
  static u3j_core _two__in_d[] =
    { { "bif", _two__in_bif_a },
      { "dif", _two__in_dif_a },
      { "gas", _two__in_gas_a },
      { "has", _two__in_has_a },
      { "mer", _two__in_mer_a },
      // { "int", _two__in_int_a },
      { "put", _two__in_put_a },
      { "tap", _two__in_tap_a },
      // { "uni", _two__in_uni_a },
      {}
    };

    static u3j_harm _two__yn_bif_a[] = {{".2", u3wdy_bif}, {}};
    static u3j_harm _two__yn_dif_a[] = {{".2", u3wdy_dif}, {}};
    static u3j_harm _two__yn_gas_a[] = {{".2", u3wdy_gas}, {}};
    static u3j_harm _two__yn_has_a[] = {{".2", u3wdy_has}, {}};
    static u3j_harm _two__yn_mer_a[] = {{".2", u3wdy_mer}, {}};
    // static u3j_harm _two__yn_ynt_a[] = {{".2", u3wdy_ynt}, {}};
    static u3j_harm _two__yn_put_a[] = {{".2", u3wdy_put}, {}};
    static u3j_harm _two__yn_tap_a[] = {{".2", u3wdy_tap}, {}};
    // static u3j_harm _two__yn_uni_a[] = {{".2", u3wdy_uni}, {}};
  static u3j_core _two__yn_d[] =
    { { "bif", _two__yn_bif_a },
      { "dif", _two__yn_dif_a },
      { "gas", _two__yn_gas_a },
      { "has", _two__yn_has_a },
      { "mer", _two__yn_mer_a },
      // { "ynt", _two__yn_ynt_a },
      { "put", _two__yn_put_a },
      { "tap", _two__yn_tap_a },
      // { "uni", _two__yn_uni_a },
      {}
    };

    static u3j_harm _two__by_bif_a[] = {{".2", u3wdb_bif, c3y}, {}};
    static u3j_harm _two__by_dif_a[] = {{".2", u3wdb_dif, c3y}, {}};
    static u3j_harm _two__by_gas_a[] = {{".2", u3wdb_gas, c3y}, {}};
    static u3j_harm _two__by_get_a[] = {{".2", u3wdb_get, c3y}, {}};
    static u3j_harm _two__by_has_a[] = {{".2", u3wdb_has, c3y}, {}};
    // static u3j_harm _two__by_int_a[] = {{".2", u3wdb_int, c3y}, {}};
    static u3j_harm _two__by_put_a[] = {{".2", u3wdb_put, c3y}, {}};
    static u3j_harm _two__by_tap_a[] = {{".2", u3wdb_tap, c3y}, {}};
    // static u3j_harm _two__by_uni_a[] = {{".2", u3wdb_uni, c3y}, {}};
  static u3j_core _two__by_d[] =
    { { "bif", _two__by_bif_a },
      { "dif", _two__by_dif_a },
      { "gas", _two__by_gas_a },
      { "get", _two__by_get_a },
      { "has", _two__by_has_a },
      // { "int", _two__by_int_a },
      { "put", _two__by_put_a },
      { "tap", _two__by_tap_a },
      // { "uni", _two__by_uni_a },
      {}
    };

    static u3j_harm _two__bz_bif_a[] = {{".2", u3wdz_bif, c3y}, {}};
    static u3j_harm _two__bz_dif_a[] = {{".2", u3wdz_dif, c3y}, {}};
    static u3j_harm _two__bz_gas_a[] = {{".2", u3wdz_gas, c3y}, {}};
    static u3j_harm _two__bz_get_a[] = {{".2", u3wdz_get, c3y}, {}};
    static u3j_harm _two__bz_has_a[] = {{".2", u3wdz_has, c3y}, {}};
    // static u3j_harm _two__bz_int_a[] = {{".2", u3wdz_int, c3y}, {}};
    static u3j_harm _two__bz_put_a[] = {{".2", u3wdz_put, c3y}, {}};
    static u3j_harm _two__bz_tap_a[] = {{".2", u3wdz_tap, c3y}, {}};
    // static u3j_harm _two__bz_uni_a[] = {{".2", u3wdz_uni, c3y}, {}};
  static u3j_core _two__bz_d[] =
    { { "bif", _two__bz_bif_a },
      { "dif", _two__bz_dif_a },
      { "gas", _two__bz_gas_a },
      { "get", _two__bz_get_a },
      { "has", _two__bz_has_a },
      // { "int", _two__bz_int_a },
      { "put", _two__bz_put_a },
      { "tap", _two__bz_tap_a },
      // { "uni", _two__bz_uni_a },
      {}
    };

  static u3j_harm _two_cue_a[] = {{".2", u3we_cue}, {}};
  static u3j_harm _two_jam_a[] = {{".2", u3we_jam}, {}};
  static u3j_harm _two_mat_a[] = {{".2", u3we_mat}, {}};
  static u3j_harm _two_rub_a[] = {{".2", u3we_rub}, {}};

static u3j_core _two_d[] =
  { { "tri", 0, _tri_d },
    { "flop", _two_flop_a },
    { "lent", _two_lent_a },
    { "levy", _two_levy_a },
    { "lien", _two_lien_a },
    { "murn", _two_murn_a },
    { "need", _two_need_a },
    { "reap", _two_reap_a },
    { "reel", _two_reel_a },
    { "roll", _two_roll_a },
    { "skid", _two_skid_a },
    { "skim", _two_skim_a },
    { "skip", _two_skip_a },
    { "scag", _two_scag_a },
    { "slag", _two_slag_a },
    { "snag", _two_snag_a },
//  { "sort", _two_sort_a },
    { "turn", _two_turn_a },
    { "weld", _two_weld_a },

    { "bex", _two_bex_a },
    { "xeb", _two_xeb_a },
    { "can", _two_can_a },
    { "cat", _two_cat_a },
    { "cut", _two_cut_a },
    { "end", _two_end_a },
    { "lsh", _two_lsh_a },
    { "met", _two_met_a },
    { "rap", _two_rap_a },
    { "rip", _two_rip_a },
    { "rsh", _two_rsh_a },

    { "con", _two_con_a },
    { "dis", _two_dis_a },
    { "mix", _two_mix_a },

    { "muk", _two_muk_a },
    { "mum", _two_mum_a },
    { "mug", _two_mug_a },

    { "dor", _two_dor_a },

    { "gor", _two_gor_a },
    { "gyr", _two_gyr_a },
    { "hor", _two_hor_a },
    { "hyr", _two_hyr_a },
    { "vor", _two_vor_a },
    { "vyr", _two_vyr_a },

    { "pow", _two_pow_a },
    { "sqt", _two_sqt_a },

    { "by", 0, _two__by_d },
    { "in", 0, _two__in_d },

    { "bz", 0, _two__bz_d },
    { "yn", 0, _two__yn_d },

    { "cue", _two_cue_a },
    { "jam", _two_jam_a },
    { "mat", _two_mat_a },
    { "rub", _two_rub_a },
  };


/* layer one
*/
  static u3j_harm _one_add_a[] = {{".2", u3wa_add, c3y}, {}};
  static u3j_harm _one_dec_a[] = {{".2", u3wa_dec, c3y}, {}};
  static u3j_harm _one_div_a[] = {{".2", u3wa_div, c3y}, {}};
  static u3j_harm _one_dvr_a[] = {{".2", u3wc_dvr, c3y}, {}};
  static u3j_harm _one_gte_a[] = {{".2", u3wa_gte, c3y}, {}};
  static u3j_harm _one_gth_a[] = {{".2", u3wa_gth, c3y}, {}};
  static u3j_harm _one_lte_a[] = {{".2", u3wa_lte, c3y}, {}};
  static u3j_harm _one_lth_a[] = {{".2", u3wa_lth, c3y}, {}};
  static u3j_harm _one_mod_a[] = {{".2", u3wa_mod, c3y}, {}};
  static u3j_harm _one_mul_a[] = {{".2", u3wa_mul, c3y}, {}};
  static u3j_harm _one_sub_a[] = {{".2", u3wa_sub, c3y}, {}};

  static u3j_harm _one_cap_a[] = {{".2", u3wc_cap, c3y}, {}};
  static u3j_harm _one_peg_a[] = {{".2", u3wc_peg, c3y}, {}};
  static u3j_harm _one_mas_a[] = {{".2", u3wc_mas, c3y}, {}};

static u3j_core _one_d[] =
  { { "two", 0, _two_d },
    { "add", _one_add_a },
    { "dec", _one_dec_a },
    { "div", _one_div_a },
    { "dvr", _one_dvr_a },
    { "gte", _one_gte_a },
    { "gth", _one_gth_a },
    { "lte", _one_lte_a },
    { "lth", _one_lth_a },
    { "mod", _one_mod_a },
    { "mul", _one_mul_a },
    { "sub", _one_sub_a },

    { "cap", _one_cap_a },
    { "mas", _one_mas_a },
    { "peg", _one_peg_a },

    {}
  };
static u3j_core _k150_d[] =
  { { "one", 0, _one_d },
    {}
  };


static u3j_core _d[] = {
  { "k150", 0, _k150_d},
  {}
};

u3j_dash 
u3j_Dash = {
  _d,
  0,
  0
};
