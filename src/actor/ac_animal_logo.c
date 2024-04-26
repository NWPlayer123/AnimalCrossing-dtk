#include "ac_animal_logo.h"

#include "m_common_data.h"
#include "m_malloc.h"
#include "m_event.h"
#include "m_play.h"
#include "m_bgm.h"
#include "m_npc.h"
#include "libc64/qrand.h"
#include "m_name_table.h"
#include "padmgr.h"
#include "audio.h"
#include "Famicom/famicom.h"
#include "m_land.h"
#include "m_titledemo.h"
#include "m_card.h"
#include "m_rcp.h"
#include "m_cpak.h"
#include "sys_matrix.h"
#include "m_time.h"
#include "m_font.h"
#include "libultra/libultra.h"
#include "m_flashrom.h"

#define G_CC_TITLE PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, 0, TEXEL0, 0
#define G_CC_TM 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0
#define G_CC_BACK 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0
#define G_CC_PRESS_START PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, 0, TEXEL0, 0

#define TITLE_WIDTH 64
#define TITLE_HEIGHT 16

extern u8 log_win_nintendo1_tex[];
extern u8 log_win_nintendo2_tex[];
extern u8 log_win_nintendo3_tex[];

extern Gfx logo_us_tm_model[];

extern Gfx logo_us_backA_model[];
extern Gfx logo_us_backB_model[];
extern Gfx logo_us_backC_model[];
extern Gfx logo_us_backD_model[];

extern u8 log_win_logo3_tex[];
extern u8 log_win_logo4_tex[];

extern cKF_Skeleton_R_c cKF_bs_r_logo_us_animal;
extern cKF_Skeleton_R_c cKF_bs_r_logo_us_cros;
extern cKF_Skeleton_R_c cKF_bs_r_logo_us_sing;

static void aAL_actor_ct(ACTOR* actor, GAME* game);
static void aAL_actor_dt(ACTOR* actor, GAME* game);
static void aAL_actor_move(ACTOR* actor, GAME* game);
static void aAL_actor_draw(ACTOR* actor, GAME* game);

ACTOR_PROFILE Animal_Logo_Profile = {
  mAc_PROFILE_ANIMAL_LOGO,
  ACTOR_PART_BG,
  ACTOR_STATE_NO_MOVE_WHILE_CULLED | ACTOR_STATE_NO_DRAW_WHILE_CULLED,
  EMPTY_NO,
  ACTOR_OBJ_BANK_KEEP,
  sizeof(ANIMAL_LOGO_ACTOR),
  &aAL_actor_ct,
  &aAL_actor_dt,
  &aAL_actor_move,
  &aAL_actor_draw,
  NULL
};


// TODO: once proper building is possible, fix this
//#include "../src/ac_animal_logo_misc.c"

static void aAL_setupAction(ANIMAL_LOGO_ACTOR* actor, GAME* game, int action);
static void aAL_title_decide_p_sel_npc();

static void aAL_actor_ct(ACTOR* actor, GAME* game) {
  ANIMAL_LOGO_ACTOR* logo_actor = (ANIMAL_LOGO_ACTOR*)actor;
  GAME_PLAY* play = (GAME_PLAY*)game;
  Clip_c* clip = Common_GetPointer(clip);
  aAL_SkeletonInfo_c* skeleton_info;

  if (clip->animal_logo_clip == NULL) {
    clip->animal_logo_clip = (aAL_Clip_c*)zelda_malloc(sizeof(aAL_Clip_c));
    clip->animal_logo_clip->data_init_proc = &title_action_data_init_start_select;
  }

  skeleton_info = &logo_actor->animal;
  skeleton_info->work_area_p = logo_actor->animal_work_area;
  skeleton_info->morph_area_p = logo_actor->animal_morph_area;
  cKF_SkeletonInfo_R_ct(&skeleton_info->skeleton, &cKF_bs_r_logo_us_animal, NULL, skeleton_info->work_area_p, skeleton_info->morph_area_p);

  skeleton_info = &logo_actor->cros;
  skeleton_info->work_area_p = logo_actor->cros_work_area;
  skeleton_info->morph_area_p = logo_actor->cros_morph_area;
  cKF_SkeletonInfo_R_ct(&skeleton_info->skeleton, &cKF_bs_r_logo_us_cros, NULL, skeleton_info->work_area_p, skeleton_info->morph_area_p);


  skeleton_info = &logo_actor->sing;
  skeleton_info->work_area_p = logo_actor->sing_work_area;
  skeleton_info->morph_area_p = logo_actor->sing_morph_area;
  cKF_SkeletonInfo_R_ct(&skeleton_info->skeleton, &cKF_bs_r_logo_us_sing, NULL, skeleton_info->work_area_p, skeleton_info->morph_area_p);

  aAL_setupAction(logo_actor, (GAME*)play, aAL_ACTION_IN);
}

static void aAL_actor_dt(ACTOR* actor, GAME* game) {
  ANIMAL_LOGO_ACTOR* logo_actor = (ANIMAL_LOGO_ACTOR*)actor;

  if (Common_Get(clip.animal_logo_clip) != NULL) {
    zelda_free(Common_Get(clip.animal_logo_clip));
    Common_Set(clip.animal_logo_clip, NULL);
  }

  if (mEv_CheckTitleDemo() != -1) {
    mEv_SetTitleDemo(0);
  }

  cKF_SkeletonInfo_R_dt(&logo_actor->animal.skeleton);
  cKF_SkeletonInfo_R_dt(&logo_actor->cros.skeleton);
  cKF_SkeletonInfo_R_dt(&logo_actor->sing.skeleton);

  if (mFRm_CheckSaveData() == TRUE) {
    aAL_title_decide_p_sel_npc();
  }
}

static void aAL_title_game_data_init_start_select(ANIMAL_LOGO_ACTOR* actor, GAME* game) {
  GAME_PLAY* play = (GAME_PLAY*)game;

  play->fb_fade_type = 8;
  play->fb_wipe_type = 3;
  Common_Set(transition.wipe_type, 3);
  mBGMPsComp_make_ps_wipe(0x1168);
}

static void aAL_title_decide_p_sel_npc() {
  int selected;
  mActor_name_t npc_name;
  int idx;

  while (TRUE) {
    selected = (int)(fqrand() * (f32)ANIMAL_NUM_MAX);
    if (mNpc_CheckFreeAnimalPersonalID(Save_GetPointer(animals[selected].id)) == FALSE) {
      npc_name = Save_Get(animals[selected].id.npc_id);
      break;
    }
  }
  
  idx = mNpc_SearchAnimalinfo(Save_Get(animals), npc_name, ANIMAL_NUM_MAX);
  mNpc_RegistEventNpc(SP_NPC_P_SEL2, npc_name, npc_name, Save_Get(animals[idx].cloth));
}

static int aAL_wipe_end_check(GAME* game) {
  GAME_PLAY* play = (GAME_PLAY*)game;
  int res = FALSE;
  fbdemo_wipe* wipe = &play->fbdemo_wipe;

  if ((*wipe->wipe_procs.isfinished_proc)(&wipe->wipe_data)) {
    res = TRUE;
  }

  return res;
}

static int aAL_chk_start_key() {
  int res = FALSE;

  if (padmgr_isConnectedController(PAD0) && ((gamePT->pads[PAD0].on.button & BUTTON_START) == BUTTON_START || (gamePT->pads[PAD0].on.button & BUTTON_A) == BUTTON_A)) {
    res = TRUE;
  }

  return res;
}

static int aAL_chk_start_key2(ANIMAL_LOGO_ACTOR* actor, GAME* game) {
  int res = FALSE;

  if (aAL_chk_start_key() == TRUE) {
    aAL_setupAction(actor, game, aAL_ACTION_START_KEY_CHK_START);
    res = TRUE;
  }

  return res;
}

static void aAL_logo_in(ANIMAL_LOGO_ACTOR* actor, GAME* game) {
  int animal_done;
  int cros_done;
  int sing_done;
    
  if (aAL_chk_start_key2(actor, game) == FALSE) {
    animal_done = cKF_SkeletonInfo_R_play(&actor->animal.skeleton);
    cros_done = cKF_SkeletonInfo_R_play(&actor->cros.skeleton);
    sing_done = cKF_SkeletonInfo_R_play(&actor->sing.skeleton);
    if (animal_done == TRUE && cros_done == TRUE && sing_done == TRUE) {
      aAL_setupAction(actor, game, aAL_ACTION_BACK_FADE_IN);
    }
  }
}

static void aAL_back_fadein(ANIMAL_LOGO_ACTOR* actor, GAME* game) {
  if (aAL_chk_start_key2(actor, game) == FALSE) {
    s16 opacity = actor->back_opacity;
    opacity += aAL_BACK_FADEIN_RATE;
    
    if (opacity > aAL_BACK_FADEIN_MAX) {
      opacity = aAL_BACK_FADEIN_MAX;
      aAL_setupAction(actor, game, aAL_ACTION_START_KEY_CHK_START);
    }

    actor->back_opacity = opacity;
  }
}

static void aAL_start_key_chk_start_wait(ANIMAL_LOGO_ACTOR* actor, GAME* game) {
  if (padmgr_isConnectedController(PAD0) && actor->title_timer <= 0 && famicom_mount_archive_end_check()) {
    aAL_setupAction(actor, game, aAL_ACTION_GAME_START);
  }
}

static void aAL_game_start_wait(ANIMAL_LOGO_ACTOR* actor, GAME* game) {
  GAME_PLAY* play = (GAME_PLAY*)game;
  f32 start_opacity;
  s16 t;
  s16 new_opacity_timer;

  new_opacity_timer = actor->start_opacity_timer;
  new_opacity_timer += (s16)(32768.0f / (actor->start_opacity_timer > 0 ? 50.0f : 22.0f));
  start_opacity = 127.5f * sin_s(new_opacity_timer) + 127.5f; // 127.5f + 127.5f * [0, 1] = [127.5f, 255.0f] (opacity)

  if (start_opacity > 255.0f) {
    start_opacity = 255.0f;
  }
  else if (start_opacity < 0.0f) {
    start_opacity = 0.0f;
  }

  actor->press_start_opacity = start_opacity;
  actor->start_opacity_timer = new_opacity_timer;

  if (play->fb_fade_type == 10) {
    aAL_setupAction(actor, game, aAL_ACTION_6);
  }
  else if (
     ((gamePT->pads[PAD0].on.button & BUTTON_START) == BUTTON_START || (gamePT->pads[PAD0].on.button & BUTTON_A) == BUTTON_A) &&
     mLd_CheckStartFlag() == TRUE &&
     aAL_wipe_end_check(game) == TRUE &&
     mTD_tdemo_button_ok_check()
  ) {
    aAL_setupAction(actor, game, aAL_ACTION_FADE_OUT_START);
  }
}

static void aAL_fade_out_start_wait(ANIMAL_LOGO_ACTOR* actor, GAME* game) {
  if (actor->title_timer <= 0) {
    aAL_title_game_data_init_start_select(actor, game);
    aAL_setupAction(actor, game, aAL_ACTION_OUT);
  }
}

extern cKF_Animation_R_c cKF_ba_r_logo_us_animal;
extern cKF_Animation_R_c cKF_ba_r_logo_us_cros;
extern cKF_Animation_R_c cKF_ba_r_logo_us_sing;

static void aAL_logo_in_init(ANIMAL_LOGO_ACTOR* actor, GAME* game) {
  cKF_SkeletonInfo_R_init(&actor->animal.skeleton, actor->animal.skeleton.skeleton, &cKF_ba_r_logo_us_animal, 1.0f, 121.0f, 1.0f, 0.5f, 0.0f, cKF_FRAMECONTROL_STOP, NULL);
  cKF_SkeletonInfo_R_init(&actor->cros.skeleton, actor->cros.skeleton.skeleton, &cKF_ba_r_logo_us_cros, 1.0f, 121.0f, 1.0f, 0.5f, 0.0f, cKF_FRAMECONTROL_STOP, NULL);
  cKF_SkeletonInfo_R_init(&actor->sing.skeleton, actor->sing.skeleton.skeleton, &cKF_ba_r_logo_us_sing, 1.0f, 121.0f, 1.0f, 0.5f, 0.0f, cKF_FRAMECONTROL_STOP, NULL);

  actor->copyright_opacity = 0;
  actor->titledemo_no = mTD_get_titledemo_no();

  mCD_set_aram_save_data();
  lbRTC_GetTime(Common_GetPointer(time.rtc_time));
  Common_Set(player_no, 0);
  Common_Set(player_data_mode, 0);
  Common_Set(scene_from_title_demo, -1);
}

static void aAL_back_fadein_init(ANIMAL_LOGO_ACTOR* actor, GAME* game) {
  actor->back_opacity = 0;
}

static void aAL_start_key_chk_start_wait_init(ANIMAL_LOGO_ACTOR* actor, GAME* game) {
  /* move all animations to the final frame (in case animation was skipped) */
  actor->animal.skeleton.frame_control.current_frame = actor->animal.skeleton.frame_control.end_frame;
  actor->cros.skeleton.frame_control.current_frame = actor->cros.skeleton.frame_control.end_frame;
  actor->sing.skeleton.frame_control.current_frame = actor->sing.skeleton.frame_control.end_frame;

  cKF_SkeletonInfo_R_play(&actor->animal.skeleton);
  cKF_SkeletonInfo_R_play(&actor->cros.skeleton);
  cKF_SkeletonInfo_R_play(&actor->sing.skeleton);

  actor->copyright_opacity = 255;
  actor->back_opacity = aAL_BACK_FADEIN_MAX;
  actor->title_timer = aAL_TIMER;
}

static void aAL_fade_out_start_wait_init(ANIMAL_LOGO_ACTOR* actor, GAME* game) {
  sAdo_SysTrgStart(0x44D);
  actor->title_timer = aAL_FADEOUT_TIMER;
  actor->press_start_opacity = 255.0f;
}

static void aAL_setupAction(ANIMAL_LOGO_ACTOR* actor, GAME* game, int action) {
  static const ANIMAL_LOGO_ACTION_PROC init_proc[aAL_ACTION_NUM] = {
    &aAL_logo_in_init,
    &aAL_back_fadein_init,
    &aAL_start_key_chk_start_wait_init,
    (ANIMAL_LOGO_ACTION_PROC)&none_proc1,
    &aAL_fade_out_start_wait_init,
    (ANIMAL_LOGO_ACTION_PROC)&none_proc1,
    (ANIMAL_LOGO_ACTION_PROC)&none_proc1
  };

  static ANIMAL_LOGO_ACTION_PROC process[aAL_ACTION_NUM] = {
    &aAL_logo_in,
    &aAL_back_fadein,
    &aAL_start_key_chk_start_wait,
    &aAL_game_start_wait,
    &aAL_fade_out_start_wait,
    (ANIMAL_LOGO_ACTION_PROC)&none_proc1,
    (ANIMAL_LOGO_ACTION_PROC)&none_proc1
  };

  (*init_proc[action])(actor, game);
  actor->action = action;
  actor->action_proc = process[action];
}

static void aAL_actor_move(ACTOR* actor, GAME* game) {
  ANIMAL_LOGO_ACTOR* logo_actor = (ANIMAL_LOGO_ACTOR*)actor;

  lbRTC_Sampling();
  if (logo_actor->title_timer > 0) {
    logo_actor->title_timer--;
  }

  (*logo_actor->action_proc)(logo_actor, game);
}

static void aAL_copyright_draw(ANIMAL_LOGO_ACTOR* actor, GRAPH* graph) {
  static const u32 draw_pos_x[3] = { 61, 125, 189 };
  static const u32 draw_pos_y[3] = { 198, 198, 198 };

  Gfx* gfx;

  actor->copyright_opacity += aAL_COPYRIGHT_ALPHA_RATE;
  if (actor->copyright_opacity >= 255) {
    actor->copyright_opacity = 255;
  }

  OPEN_DISP(graph);

  gfx = NOW_FONT_DISP;
  gDPSetPrimColor(gfx++, 0, 255, 40, 40, 45, actor->copyright_opacity);
  gDPSetEnvColor(gfx++, 210, 210, 215, 0);
  gDPSetOtherMode(gfx++, G_AD_DISABLE | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, G_AC_THRESHOLD | G_ZS_PRIM | G_RM_CLD_SURF | G_RM_CLD_SURF2);
  gSPLoadGeometryMode(gfx++, 0);
  gDPSetCombineMode(gfx++, G_CC_TITLE, G_CC_TITLE);

  gDPLoadTextureTile(
    gfx++,
    log_win_nintendo1_tex,
    G_IM_FMT_IA, G_IM_SIZ_8b,
    TITLE_WIDTH, TITLE_HEIGHT,
    0, 0, TITLE_WIDTH - 1, TITLE_HEIGHT - 1,
    0,
    G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
    G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD
  );
  gfx = gfx_gSPTextureRectangle1(
    gfx,
    draw_pos_x[0] << 2, draw_pos_y[0] << 2,
    (TITLE_WIDTH + draw_pos_x[0]) << 2, (TITLE_HEIGHT + draw_pos_y[0]) << 2,
    0,
    0 << 5, 0 << 5,
    1 << 10, 1 << 10
  );

  gDPLoadTextureTile(
    gfx++,
    log_win_nintendo2_tex,
    G_IM_FMT_IA, G_IM_SIZ_8b,
    TITLE_WIDTH, TITLE_HEIGHT,
    0, 0, TITLE_WIDTH - 1, TITLE_HEIGHT - 1,
    0,
    G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
    G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD
  );
  gfx = gfx_gSPTextureRectangle1(
    gfx,
    draw_pos_x[1] << 2, draw_pos_y[1] << 2,
    (TITLE_WIDTH + draw_pos_x[1]) << 2, (TITLE_HEIGHT + draw_pos_y[1]) << 2,
    0,
    0 << 5, 0 << 5,
    1 << 10, 1 << 10
  );

  gDPLoadTextureTile(
    gfx++,
    log_win_nintendo3_tex,
    G_IM_FMT_IA, G_IM_SIZ_8b,
    TITLE_WIDTH, TITLE_HEIGHT,
    0, 0, TITLE_WIDTH - 1, TITLE_HEIGHT - 1,
    0,
    G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
    G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD
  );
  gfx = gfx_gSPTextureRectangle1(
    gfx,
    draw_pos_x[2] << 2, draw_pos_y[2] << 2,
    (TITLE_WIDTH + draw_pos_x[2]) << 2, (TITLE_HEIGHT + draw_pos_y[2]) << 2,
    0,
    0 << 5, 0 << 5,
    1 << 10, 1 << 10
  );

  SET_FONT_DISP(gfx);

  CLOSE_DISP(graph);
}

static void aAL_tm_draw(GRAPH* graph) {
  static const Gfx init_disp[] = {
    gsSPLoadGeometryMode(G_CULL_BACK),
    gsDPSetOtherMode(G_AD_DISABLE | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PRIM | G_RM_XLU_SURF | G_RM_XLU_SURF2),
    gsDPSetCombineMode(G_CC_TM, G_CC_TM),
    gsSPEndDisplayList()
  };

  Gfx* gfx;

  Matrix_push();
  Matrix_translate(1530.0f, 690.0f, 0.0f, 1);
  Matrix_scale(0.162082675f, 0.162082675f, 0.162082675f, 1);

  OPEN_DISP(graph);

  gfx = NOW_FONT_DISP;
  gSPMatrix(gfx++, _Matrix_to_Mtx_new(graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
  gSPDisplayList(gfx++, init_disp);
  gSPDisplayList(gfx++, logo_us_tm_model);
  SET_FONT_DISP(gfx);
  
  CLOSE_DISP(graph);

  Matrix_pull();
}

static void aAL_back_draw(GRAPH* graph, ANIMAL_LOGO_ACTOR* actor) {
  static const Gfx init_disp[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_CULL_BACK),
    gsDPSetOtherMode(G_AD_DISABLE | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PRIM | G_RM_XLU_SURF | G_RM_XLU_SURF2),
    gsDPSetCombineMode(G_CC_BACK, G_CC_BACK),
    gsSPEndDisplayList()
  };

  Gfx* gfx;

  Matrix_push();
  Matrix_translate(0.0f, 730.0f, 0.0f, 1);
  Matrix_scale(0.135f, 0.135f, 0.135f, 1);

  OPEN_DISP(graph);

  gfx = NOW_FONT_DISP;
  gSPMatrix(gfx++, _Matrix_to_Mtx_new(graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
  gDPSetPrimColor(gfx++, 0, 255, 80, 60, 0, actor->back_opacity);
  gSPDisplayList(gfx++, init_disp);
  gSPDisplayList(gfx++, logo_us_backA_model);
  gSPDisplayList(gfx++, logo_us_backB_model);
  gSPDisplayList(gfx++, logo_us_backC_model);
  gSPDisplayList(gfx++, logo_us_backD_model);
  SET_FONT_DISP(gfx);

  CLOSE_DISP(graph);

  Matrix_pull();
}

static void aAL_press_start_draw(ANIMAL_LOGO_ACTOR* actor, GRAPH* graph) {
  static const u32 draw_pos_x[2] = { 96, 160 };
  static const u32 draw_pos_y[2] = { 159, 159 };

  static const u32 ps_prim_r[5] = { 70, 60, 60, 40, 40 };
  static const u32 ps_prim_g[5] = { 40, 50, 40, 50, 50 };
  static const u32 ps_prim_b[5] = { 40, 30, 60, 70, 60 };
  
  static const u32 ps_env_r[5] = { 255, 255, 255, 120, 165 };
  static const u32 ps_env_g[5] = {  90, 135, 100, 205, 245 };
  static const u32 ps_env_b[5] = {  30,   0, 255, 245,   0 };

  Gfx* gfx;
  int titledemo_no;
  f32 alpha;
  titledemo_no = actor->titledemo_no;
  alpha = actor->press_start_opacity;

  OPEN_DISP(graph);

  gfx = NOW_FONT_DISP;
  gDPSetPrimColor(gfx++, 0, 255, ps_prim_r[titledemo_no], ps_prim_g[titledemo_no], ps_prim_b[titledemo_no], (u32)alpha);
  gDPSetEnvColor(gfx++, ps_env_r[titledemo_no], ps_env_g[titledemo_no], ps_env_b[titledemo_no], 0);
  gDPSetOtherMode(gfx++, G_AD_DISABLE | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, G_AC_THRESHOLD | G_ZS_PRIM | G_RM_XLU_SURF | G_RM_XLU_SURF2);
  gSPLoadGeometryMode(gfx++, 0);
  gDPSetCombineMode(gfx++, G_CC_PRESS_START, G_CC_PRESS_START);

  gDPLoadTextureTile(
    gfx++,
    log_win_logo3_tex,
    G_IM_FMT_IA, G_IM_SIZ_8b,
    TITLE_WIDTH, TITLE_HEIGHT,
    0, 0, TITLE_WIDTH - 1, TITLE_HEIGHT - 1,
    0,
    G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
    G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD
  );

  gfx = gfx_gSPTextureRectangle1(
    gfx,
    draw_pos_x[0] << 2, draw_pos_y[0] << 2,
    (TITLE_WIDTH + draw_pos_x[0]) << 2, (TITLE_HEIGHT + draw_pos_y[0]) << 2,
    0,
    0 << 5, 0 << 5,
    1 << 10, 1 << 10
  );

  gDPLoadTextureTile(
    gfx++,
    log_win_logo4_tex,
    G_IM_FMT_IA, G_IM_SIZ_8b,
    TITLE_WIDTH, TITLE_HEIGHT,
    0, 0, TITLE_WIDTH - 1, TITLE_HEIGHT - 1,
    0,
    G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
    G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD
  );

  gfx = gfx_gSPTextureRectangle1(
    gfx,
    draw_pos_x[1] << 2, draw_pos_y[1] << 2,
    (TITLE_WIDTH + draw_pos_x[1]) << 2, (TITLE_HEIGHT + draw_pos_y[1]) << 2,
    0,
    0 << 5, 0 << 5,
    1 << 10, 1 << 10
  );

  SET_FONT_DISP(gfx);

  CLOSE_DISP(graph);
}

static void aAL_skl_draw(GAME* game, cKF_SkeletonInfo_R_c* skl_keyframe) {
  Mtx* m;

  OPEN_DISP(game->graph);

  m = GRAPH_ALLOC_TYPE(game->graph, Mtx, skl_keyframe->skeleton->num_shown_joints);
  if (m != NULL) {
    cKF_Si3_draw_R_SV(game, skl_keyframe, m, NULL, NULL, NULL);
  }

  CLOSE_DISP(game->graph);
}

static void aAL_title_draw(GAME* game, ANIMAL_LOGO_ACTOR* actor) {
  static const Gfx init_disp[] = {
    gsDPSetOtherMode(G_AD_NOTPATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_RGBA16 | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PRIM | G_RM_XLU_SURF | G_RM_XLU_SURF2),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPLoadGeometryMode(G_CULL_BACK),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPEndDisplayList()
  };

  Gfx* poly_save;
  GRAPH* graph = game->graph;

  Matrix_push();
  Matrix_translate(0.0f, 730.0f, 0.0f, 1);
  Matrix_scale(0.135f, 0.135f, 0.135f, 1);

  OPEN_DISP(graph);

  // we need to save the opaque polygon gfx buffer and swap with font because cKF utilizes opaque polygon gfx,
  // but we want this on the font gfx buffer
  poly_save = NOW_POLY_OPA_DISP;
  SET_POLY_OPA_DISP(NOW_FONT_DISP);
  gSPMatrix(NOW_POLY_OPA_DISP++, _Matrix_to_Mtx_new(graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
  gSPDisplayList(NOW_POLY_OPA_DISP++, init_disp);
  
  aAL_skl_draw(game, &actor->animal.skeleton);
  aAL_skl_draw(game, &actor->cros.skeleton);
  aAL_skl_draw(game, &actor->sing.skeleton);

  SET_FONT_DISP(NOW_POLY_OPA_DISP);
  SET_POLY_OPA_DISP(poly_save);

  CLOSE_DISP(graph);

  Matrix_pull();
}

static void aAL_actor_draw(ACTOR* actor, GAME* game) {
  ANIMAL_LOGO_ACTOR* logo_actor = (ANIMAL_LOGO_ACTOR*)actor;
  GRAPH* graph = game->graph;

  mFont_SetMatrix(graph, mFont_MODE_FONT);

  if (logo_actor->action >= aAL_ACTION_BACK_FADE_IN) {
    aAL_back_draw(graph, logo_actor);
  }

  aAL_title_draw(game, logo_actor);

  if (logo_actor->action >= aAL_ACTION_START_KEY_CHK_START) {
    aAL_copyright_draw(logo_actor, graph);
    aAL_tm_draw(graph);
  }

  mFont_SetMode(graph, mFont_MODE_FONT);
  if (padmgr_isConnectedController(PAD0)) {
    switch (logo_actor->action) {
      case aAL_ACTION_GAME_START:
      case aAL_ACTION_FADE_OUT_START:
      case aAL_ACTION_OUT:
        aAL_press_start_draw(logo_actor, graph);
        break;
    }
  }

  mFont_UnSetMatrix(graph, mFont_MODE_FONT);
  game_debug_draw_last(game, graph);
  game_draw_last(graph);
}
