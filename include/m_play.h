#ifndef M_PLAY_H
#define M_PLAY_H

#include "types.h"
#include "game.h"
#include "libultra/ultratypes.h"
#include "m_view.h"
#include "m_camera2.h"
#include "m_submenu.h"
#include "m_kankyo.h"
#include "m_lights.h"
#include "m_pause.h"
#include "m_field_info.h"
#include "m_event.h"
#include "m_fbdemo_wipe.h"
#include "m_collision_obj.h"
#include "m_play_h.h"
#include "m_scene.h"
#include "PreRender.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*DRAW_CHK_PROC)(ACTOR*, GAME_PLAY*);
typedef void (*PLAY_WIPE_PROC)(GAME_PLAY*);

/* sizeof(struct game_play_s) == 0x2600 */
struct game_play_s {
    /* 0x0000 */ GAME game;
    // TODO: finish
    /* 0x00E0 */ s16 scene_id;
    /* 0x00E4 */ mFI_block_tbl_c block_table;
    /* 0x00F4 */ mFI_block_tbl_c last_block_table;
    /* 0x0104 */ u8 _0104[0x010C - 0x0104];
    /* 0x010C */ Scene_Word_u* current_scene_data;
    /* 0x0110 */ Object_Exchange_c object_exchange;
    /* 0x1A68 */ View view;
    /* 0x1B88 */ Camera2 camera;
    /* 0x1CC0 */ Kankyo kankyo;
    /* 0x1D90 */ Global_light global_light;
    /* 0x1DA0 */ pause_t pause;
    /* 0x1DA8 */ Actor_info actor_info;
    /* 0x1DEC */ Submenu submenu;
    /* 0x1FA4 */ s8 submenu_ground_idx;
    /* 0x1FA8 */ char* submenu_ground_tex[2];
    /* 0x1FB0 */ char* submenu_ground_pallet[2];
    /* 0x1FB8 */ PreRender prerender;
    /* 0x2000 */ Door_info_c door_info;
    /* 0x2008 */ int next_scene_no;
    /* 0x200C */ MtxF projection_matrix;
    /* 0x204C */ MtxF billboard_matrix;
    /* 0x208C */ Mtx* billboard_mtx_p;
    /* 0x2090 */ u32 game_frame;
    /* 0x2094 */ u8 _2094;
    /* 0x2095 */ u8 actor_data_num;
    /* 0x2096 */ u8 ctrl_actor_data_num;
    /* 0x2097 */ u8 obj_bank_data_num;
    /* 0x2098 */ Actor_data* player_data;
    /* 0x209C */ Actor_data* actor_data;
    /* 0x20A0 */ s16* ctrl_actor_data;
    /* 0x20A4 */ s16* obj_bank_data;
    /* 0x20A8 */ int _20A8;
    /* 0x20AC */ Event_c event;
    /* 0x20D0 */ u8 fb_fade_type;
    /* 0x20D1 */ u8 fb_wipe_type;
    /* 0x20D2 */ u8 fb_mode;
    /* 0x20D3 */ u8 fb_wipe_mode;
    /* 0x20D8 */ fbdemo_wipe fbdemo_wipe;
    /* 0x2318 */ fbdemo_fade color_fade;
    /* 0x2328 */ CollisionCheck_c collision_check;
    /* 0x23F8 */ DRAW_CHK_PROC draw_chk_proc; // only used by mikanbox actor
    /* 0x23FC */ rgba8888 fade_color_value;
    /* 0x2400 */ Scene_data_status_c* scene_data_status;
    /* 0x2404 */ u8 _2400[0x2600 - 0x2404];
};

extern fbdemo_c fbdemo;

extern void Game_play_Reset_destiny();
extern void event_title_flag_on();
extern void event_title_flag_off();
extern void Game_play_camera_proc(GAME_PLAY*);
extern void Game_play_fbdemo_wipe_destroy(GAME_PLAY*);
extern void Game_play_fbdemo_wipe_create_sub(GAME_PLAY*);
extern void Game_play_fbdemo_wipe_create(GAME_PLAY*);
extern void Game_play_fbdemo_wipe_init(GAME_PLAY*);
extern void Game_play_fbdemo_fade_in_move_end(GAME_PLAY*);
extern void Game_play_fbdemo_fade_out_start_emu_move_end(GAME_PLAY*);
extern void Game_play_fbdemo_fade_out_game_end_move_end(GAME_PLAY*);
extern void Game_play_change_scene_move_end(GAME_PLAY*);
extern void Game_play_fbdemo_wipe_move(GAME_PLAY*);
extern void Game_play_fbdemo_wipe_proc(GAME_PLAY*);
extern Gfx* game_play_set_fog(GAME_PLAY*, Gfx*);
extern void Game_play_fbdemo_proc(GAME_PLAY*);
extern void play_cleanup(GAME*);
extern void VR_Box_ct(GAME_PLAY*);
extern void play_init(GAME*);
extern void Game_play_move_fbdemo_not_move(GAME_PLAY*);
extern void Game_play_move(GAME_PLAY*);
extern void setupFog(GAME_PLAY*, GRAPH*);
extern void setupViewer(GAME_PLAY*);
extern void setupViewMatrix(GAME_PLAY*, GRAPH*, GRAPH*);
extern void copy_efb_to_texture(Gfx**, void*);
extern int makeBumpTexture(GAME_PLAY*, GRAPH*, GRAPH*);
extern void draw_version(GRAPH*);
extern void Game_play_draw(GAME*);
extern void play_main(GAME*);
extern void Gameplay_Scene_Init(GAME_PLAY*);
extern u8 mPl_SceneNo2SoundRoomType(int);
extern void Gameplay_Scene_Read(GAME_PLAY*, s16);

#ifdef __cplusplus
}
#endif

#endif
