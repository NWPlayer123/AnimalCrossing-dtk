#ifndef M_PLAYER_H
#define M_PLAYER_H

#include "types.h"
#include "m_actor.h"
#include "m_lib.h"
#include "c_keyframe.h"
#include "libultra/ultratypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define mPlayer_DEBT0 17400  /* Buy house */
#define mPlayer_DEBT1 148000 /* 1st upgrade main floor */
#define mPlayer_DEBT2 398000 /* 2nd upgrade main floor */
#define mPlayer_DEBT3 49800  /* Basement */
#define mPlayer_DEBT4 798000 /* Upper floor */

typedef struct player_actor_s PLAYER_ACTOR;

#define mPlayer_FORCE_POSITION_ANGLE_NONE 0
// where is (1 << 0) ??
#define mPlayer_FORCE_POSITION_ANGLE_POSX (1 << 1)
#define mPlayer_FORCE_POSITION_ANGLE_POSY (1 << 2)
#define mPlayer_FORCE_POSITION_ANGLE_POSZ (1 << 3)
#define mPlayer_FORCE_POSITION_ANGLE_ROTX (1 << 4)
#define mPlayer_FORCE_POSITION_ANGLE_ROTY (1 << 5)
#define mPlayer_FORCE_POSITION_ANGLE_ROTZ (1 << 6)

#define mPlayer_JOINT_NUM 26

enum {
    mPlayer_ADDRESSABLE_TRUE,
    mPlayer_ADDRESSABLE_FALSE_MOVEMENT,
    mPlayer_ADDRESSABLE_FALSE_TALKING,
    mPlayer_ADDRESSABLE_FALSE_USING_TOOL,

    mPlayer_ADDRESSABLE_NUM
};

enum {
    mPlayer_COMPLETE_PAYMENT_TYPE_NONE,
    mPlayer_COMPLETE_PAYMENT_TYPE_ARBEIT,
    mPlayer_COMPLETE_PAYMENT_TYPE_HOUSE,

    mPlayer_COMPLETE_PAYMENT_TYPE_NUM
};

enum {
    mPlayer_INDEX_DMA,
    mPlayer_INDEX_INTRO,
    mPlayer_INDEX_REFUSE,
    mPlayer_INDEX_REFUSE_PICKUP,
    mPlayer_INDEX_RETURN_DEMO,
    mPlayer_INDEX_RETURN_OUTDOOR,
    mPlayer_INDEX_RETURN_OUTDOOR2,
    mPlayer_INDEX_WAIT,
    mPlayer_INDEX_WALK,
    mPlayer_INDEX_RUN,
    mPlayer_INDEX_DASH,
    mPlayer_INDEX_TUMBLE,
    mPlayer_INDEX_TUMBLE_GETUP,
    mPlayer_INDEX_TURN_DASH,
    mPlayer_INDEX_FALL,
    mPlayer_INDEX_WADE,
    mPlayer_INDEX_DOOR,
    mPlayer_INDEX_OUTDOOR,
    mPlayer_INDEX_INVADE,
    mPlayer_INDEX_HOLD,
    mPlayer_INDEX_PUSH,
    mPlayer_INDEX_PULL,
    mPlayer_INDEX_ROTATE_FURNITURE,
    mPlayer_INDEX_OPEN_FURNITURE,
    mPlayer_INDEX_WAIT_OPEN_FURNITURE,
    mPlayer_INDEX_CLOSE_FURNITURE,
    mPlayer_INDEX_LIE_BED,
    mPlayer_INDEX_WAIT_BED,
    mPlayer_INDEX_ROLL_BED,
    mPlayer_INDEX_STANDUP_BED,
    mPlayer_INDEX_PICKUP,
    mPlayer_INDEX_PICKUP_JUMP,
    mPlayer_INDEX_PICKUP_FURNITURE,
    mPlayer_INDEX_PICKUP_EXCHANGE,
    mPlayer_INDEX_SITDOWN,
    mPlayer_INDEX_SITDOWN_WAIT,
    mPlayer_INDEX_STANDUP,
    mPlayer_INDEX_SWING_AXE,
    mPlayer_INDEX_AIR_AXE,
    mPlayer_INDEX_REFLECT_AXE,
    mPlayer_INDEX_BROKEN_AXE,
    mPlayer_INDEX_SLIP_NET,
    mPlayer_INDEX_READY_NET,
    mPlayer_INDEX_READY_WALK_NET,
    mPlayer_INDEX_SWING_NET,
    mPlayer_INDEX_PULL_NET,
    mPlayer_INDEX_STOP_NET,
    mPlayer_INDEX_NOTICE_NET,
    mPlayer_INDEX_PUTAWAY_NET,
    mPlayer_INDEX_READY_ROD,
    mPlayer_INDEX_CAST_ROD,
    mPlayer_INDEX_AIR_ROD,
    mPlayer_INDEX_RELAX_ROD,
    mPlayer_INDEX_COLLECT_ROD,
    mPlayer_INDEX_VIB_ROD,
    mPlayer_INDEX_FLY_ROD,
    mPlayer_INDEX_NOTICE_ROD,
    mPlayer_INDEX_PUTAWAY_ROD,
    mPlayer_INDEX_DIG_SCOOP,
    mPlayer_INDEX_FILL_SCOOP,
    mPlayer_INDEX_REFLECT_SCOOP,
    mPlayer_INDEX_AIR_SCOOP,
    mPlayer_INDEX_GET_SCOOP,
    mPlayer_INDEX_PUTAWAY_SCOOP,
    mPlayer_INDEX_PUTIN_SCOOP,
    mPlayer_INDEX_TALK,
    mPlayer_INDEX_RECIEVE_WAIT,
    mPlayer_INDEX_RECIEVE_STRETCH,
    mPlayer_INDEX_RECIEVE,
    mPlayer_INDEX_RECIEVE_PUTAWAY,
    mPlayer_INDEX_GIVE,
    mPlayer_INDEX_GIVE_WAIT,
    mPlayer_INDEX_TAKEOUT_ITEM,
    mPlayer_INDEX_PUTIN_ITEM,
    mPlayer_INDEX_DEMO_WAIT,
    mPlayer_INDEX_DEMO_WALK,
    mPlayer_INDEX_DEMO_GETON_TRAIN,
    mPlayer_INDEX_DEMO_GETON_TRAIN_WAIT,
    mPlayer_INDEX_DEMO_GETOFF_TRAIN,
    mPlayer_INDEX_DEMO_STANDING_TRAIN,
    mPlayer_INDEX_DEMO_WADE,
    mPlayer_INDEX_HIDE,
    mPlayer_INDEX_GROUNDHOG,
    mPlayer_INDEX_RELEASE_CREATURE,
    mPlayer_INDEX_WASH_CAR,
    mPlayer_INDEX_TIRED,
    mPlayer_INDEX_ROTATE_OCTAGON,
    mPlayer_INDEX_THROW_MONEY,
    mPlayer_INDEX_PRAY,
    mPlayer_INDEX_SHAKE_TREE,
    mPlayer_INDEX_MAIL_JUMP,
    mPlayer_INDEX_MAIL_LAND,
    mPlayer_INDEX_READY_PITFALL,
    mPlayer_INDEX_FALL_PITFALL,
    mPlayer_INDEX_STRUGGLE_PITFALL,
    mPlayer_INDEX_CLIMBUP_PITFALL,
    mPlayer_INDEX_STUNG_BEE,
    mPlayer_INDEX_NOTICE_BEE,
    mPlayer_INDEX_REMOVE_GRASS,
    mPlayer_INDEX_SHOCK,
    mPlayer_INDEX_KNOCK_DOOR,
    mPlayer_INDEX_CHANGE_CLOTH,
    mPlayer_INDEX_PUSH_SNOWBALL,
    mPlayer_INDEX_ROTATE_UMBRELLA,
    mPlayer_INDEX_WADE_SNOWBALL,
    mPlayer_INDEX_COMPLETE_PAYMENT,
    mPlayer_INDEX_FAIL_EMU,
    mPlayer_INDEX_STUNG_MOSQUITO,
    mPlayer_INDEX_NOTICE_MOSQUITO,
    mPlayer_INDEX_SWING_FAN,
    mPlayer_INDEX_SWITCH_ON_LIGHTHOUSE,
    mPlayer_INDEX_RADIO_EXERCISE,
    mPlayer_INDEX_DEMO_GETON_BOAT,
    mPlayer_INDEX_DEMO_GETON_BOAT_SITDOWN,
    mPlayer_INDEX_DEMO_GETON_BOAT_WAIT,
    mPlayer_INDEX_DEMO_GETON_BOAT_WADE,
    mPlayer_INDEX_DEMO_GETOFF_BOAT_STANDUP,
    mPlayer_INDEX_DEMO_GETOFF_BOAT,
    mPlayer_INDEX_DEMO_GET_GOLDEN_ITEM,
    mPlayer_INDEX_DEMO_GET_GOLDEN_ITEM2,
    mPlayer_INDEX_DEMO_GET_GOLDEN_AXE_WAIT
};

enum {
    mPlayer_ANIM_WAIT1,
    mPlayer_ANIM_WALK1,
    mPlayer_ANIM_AXE1,
    mPlayer_ANIM_RUN1,
    mPlayer_ANIM_PUSH1,
    mPlayer_ANIM_PULL1,
    mPlayer_ANIM_HOLD_WAIT1,
    mPlayer_ANIM_PICKUP1,
    mPlayer_ANIM_LTURN1,
    mPlayer_ANIM_RTURN1,
    mPlayer_ANIM_GET1,
    mPlayer_ANIM_GET_CHANGE1,
    mPlayer_ANIM_GET_PUTAWAY1,
    mPlayer_ANIM_OPEN1,
    mPlayer_ANIM_PUTAWAY1,
    mPlayer_ANIM_TRANS_WAIT1,
    mPlayer_ANIM_TRANSFER1,
    mPlayer_ANIM_UMB_OPEN1,
    mPlayer_ANIM_UMBRELLA1,
    mPlayer_ANIM_DASH1,
    mPlayer_ANIM_RUN_SLIP1,
    mPlayer_ANIM_GET_PULL1,
    mPlayer_ANIM_GET_M1,
    mPlayer_ANIM_KAMAE_MOVE_M1,
    mPlayer_ANIM_KAMAE_WAIT_M1,
    mPlayer_ANIM_KOKERU_A1,
    mPlayer_ANIM_KOKERU_GETUP_A1,
    mPlayer_ANIM_KOKERU_GETUP_N1,
    mPlayer_ANIM_KOKERU_N1,
    mPlayer_ANIM_NET1,
    mPlayer_ANIM_NET_SWING1,
    mPlayer_ANIM_AXE_SWING1,
    mPlayer_ANIM_KAMAE_SLIP_M1,
    mPlayer_ANIM_KOKERU1,
    mPlayer_ANIM_KOKERU_GETUP1,
    mPlayer_ANIM_SITDOWN1,
    mPlayer_ANIM_SITDOWN_WAIT1,
    mPlayer_ANIM_STANDUP1,
    mPlayer_ANIM_PUTAWAY_M1,
    mPlayer_ANIM_BED_WAIT1,
    mPlayer_ANIM_INBED_L1,
    mPlayer_ANIM_INBED_R1,
    mPlayer_ANIM_INTRAIN1,
    mPlayer_ANIM_KAGU_OPEN_D1,
    mPlayer_ANIM_KAGU_OPEN_H1,
    mPlayer_ANIM_KAGU_OPEN_K1,
    mPlayer_ANIM_NEGAERI_L1,
    mPlayer_ANIM_NEGAERI_R1,
    mPlayer_ANIM_OUTBED_L1,
    mPlayer_ANIM_OUTBED_R1,
    mPlayer_ANIM_OUTTRAIN1,
    mPlayer_ANIM_KAGU_CLOSE_D1,
    mPlayer_ANIM_KAGU_CLOSE_H1,
    mPlayer_ANIM_KAGU_CLOSE_K1,
    mPlayer_ANIM_KAGU_WAIT_D1,
    mPlayer_ANIM_KAGU_WAIT_H1,
    mPlayer_ANIM_KAGU_WAIT_K1,
    mPlayer_ANIM_GO_OUT_O1,
    mPlayer_ANIM_GO_OUT_S1,
    mPlayer_ANIM_INTO_S1,
    mPlayer_ANIM_AXE_HANE1,
    mPlayer_ANIM_AXE_SUKA1,
    mPlayer_ANIM_HOLD_WAIT_H1,
    mPlayer_ANIM_HOLD_WAIT_O1,
    mPlayer_ANIM_GET_T1,
    mPlayer_ANIM_GET_T2,
    mPlayer_ANIM_PUTAWAY_T1,
    mPlayer_ANIM_SAO1,
    mPlayer_ANIM_SAO_SWING1,
    mPlayer_ANIM_TURI_HIKI1,
    mPlayer_ANIM_TURI_WAIT1,
    mPlayer_ANIM_NOT_GET_T1,
    mPlayer_ANIM_MENU_CATCH1,
    mPlayer_ANIM_MENU_CHANGE1,
    mPlayer_ANIM_UMB_CLOSE1,
    mPlayer_ANIM_NOT_SAO_SWING1,
    mPlayer_ANIM_INTRAIN_WAIT1,
    mPlayer_ANIM_CLEAR_TABLE1,
    mPlayer_ANIM_DIG1,
    mPlayer_ANIM_FILL_UP1,
    mPlayer_ANIM_NOT_DIG1,
    mPlayer_ANIM_CLEAR_TABLE_L1,
    mPlayer_ANIM_PICKUP_L1,
    mPlayer_ANIM_SCOOP1,
    mPlayer_ANIM_CONFIRM1,
    mPlayer_ANIM_DIG_SUKA1,
    mPlayer_ANIM_GET_D1,
    mPlayer_ANIM_PUTAWAY_D1,
    mPlayer_ANIM_DIG_KABU1,
    mPlayer_ANIM_FILL_UP_I1,
    mPlayer_ANIM_SEND_MAIL1,
    mPlayer_ANIM_GET_F1,
    mPlayer_ANIM_GET_PULL_F1,
    mPlayer_ANIM_GET_PUTAWAY_F1,
    mPlayer_ANIM_TRANS_WAIT_F1,
    mPlayer_ANIM_TRANSFER_F1,
    mPlayer_ANIM_SHAKE1,
    mPlayer_ANIM_TIRED1,
    mPlayer_ANIM_WASH1,
    mPlayer_ANIM_WASH2,
    mPlayer_ANIM_WASH3,
    mPlayer_ANIM_WASH4,
    mPlayer_ANIM_WASH5,
    mPlayer_ANIM_FUKUBIKI1,
    mPlayer_ANIM_OMAIRI1,
    mPlayer_ANIM_SAISEN1,
    mPlayer_ANIM_RETURN_MAIL1,
    mPlayer_ANIM_RETURN_MAIL2,
    mPlayer_ANIM_RETURN_MAIL3,
    mPlayer_ANIM_EAT1,
    mPlayer_ANIM_GAAAN1,
    mPlayer_ANIM_GAAAN2,
    mPlayer_ANIM_DERU1,
    mPlayer_ANIM_GURATUKU1,
    mPlayer_ANIM_MOGAKU1,
    mPlayer_ANIM_OTIRU1,
    mPlayer_ANIM_ZASSOU1,
    mPlayer_ANIM_KNOCK1,
    mPlayer_ANIM_BIKU1,
    mPlayer_ANIM_HATI1,
    mPlayer_ANIM_HATI2,
    mPlayer_ANIM_HATI3,
    mPlayer_ANIM_PUSH_YUKI1,
    mPlayer_ANIM_DERU2,
    mPlayer_ANIM_OTIRU2,
    mPlayer_ANIM_ITAZURA1,
    mPlayer_ANIM_UMB_ROT1,
    mPlayer_ANIM_PICKUP_WAIT1,
    mPlayer_ANIM_YATTA1,
    mPlayer_ANIM_YATTA2,
    mPlayer_ANIM_YATTA3,
    mPlayer_ANIM_KAZA1,
    mPlayer_ANIM_MOSQUITO1,
    mPlayer_ANIM_MOSQUITO2,
    mPlayer_ANIM_RIDE1,
    mPlayer_ANIM_RIDE2,
    mPlayer_ANIM_RIDEWAIT,
    mPlayer_ANIM_GETOFF1,
    mPlayer_ANIM_GETOFF2,
    mPlayer_ANIM_UTIWA_WAIT1,
    mPlayer_ANIM_UTIWA_D1,
    mPlayer_ANIM_AXE_BREAK1,
    mPlayer_ANIM_AXE_BREAKWAIT1,
    mPlayer_ANIM_LIGHT_ON1,
    mPlayer_ANIM_TAISOU1,
    mPlayer_ANIM_TAISOU2_1,
    mPlayer_ANIM_TAISOU2_2,
    mPlayer_ANIM_TAISOU3,
    mPlayer_ANIM_TAISOU4_1,
    mPlayer_ANIM_TAISOU4_2,
    mPlayer_ANIM_TAISOU5_1,
    mPlayer_ANIM_TAISOU5_2,
    mPlayer_ANIM_TAISOU6_1,
    mPlayer_ANIM_TAISOU6_2,
    mPlayer_ANIM_TAISOU7_1,
    mPlayer_ANIM_TAISOU7_2,
    mPlayer_ANIM_OMAIRI_US1,

    mPlayer_ANIM_NUM
};

enum {
    mPlayer_ITEM_KIND_AXE,
    mPlayer_ITEM_KIND_AXE_USE_1,
    mPlayer_ITEM_KIND_AXE_USE_2,
    mPlayer_ITEM_KIND_AXE_USE_3,
    mPlayer_ITEM_KIND_AXE_USE_4,
    mPlayer_ITEM_KIND_AXE_USE_5,
    mPlayer_ITEM_KIND_AXE_USE_6,
    mPlayer_ITEM_KIND_AXE_USE_7,
    mPlayer_ITEM_KIND_GOLD_AXE,

    mPlayer_ITEM_KIND_NET,
    mPlayer_ITEM_KIND_GOLD_NET,

    mPlayer_ITEM_KIND_UMBRELLA00,
    mPlayer_ITEM_KIND_UMBRELLA01,
    mPlayer_ITEM_KIND_UMBRELLA02,
    mPlayer_ITEM_KIND_UMBRELLA03,
    mPlayer_ITEM_KIND_UMBRELLA04,
    mPlayer_ITEM_KIND_UMBRELLA05,
    mPlayer_ITEM_KIND_UMBRELLA06,
    mPlayer_ITEM_KIND_UMBRELLA07,
    mPlayer_ITEM_KIND_UMBRELLA08,
    mPlayer_ITEM_KIND_UMBRELLA09,
    mPlayer_ITEM_KIND_UMBRELLA10,
    mPlayer_ITEM_KIND_UMBRELLA11,
    mPlayer_ITEM_KIND_UMBRELLA12,
    mPlayer_ITEM_KIND_UMBRELLA13,
    mPlayer_ITEM_KIND_UMBRELLA14,
    mPlayer_ITEM_KIND_UMBRELLA15,
    mPlayer_ITEM_KIND_UMBRELLA16,
    mPlayer_ITEM_KIND_UMBRELLA17,
    mPlayer_ITEM_KIND_UMBRELLA18,
    mPlayer_ITEM_KIND_UMBRELLA19,
    mPlayer_ITEM_KIND_UMBRELLA20,
    mPlayer_ITEM_KIND_UMBRELLA21,
    mPlayer_ITEM_KIND_UMBRELLA22,
    mPlayer_ITEM_KIND_UMBRELLA23,
    mPlayer_ITEM_KIND_UMBRELLA24,
    mPlayer_ITEM_KIND_UMBRELLA25,
    mPlayer_ITEM_KIND_UMBRELLA26,
    mPlayer_ITEM_KIND_UMBRELLA27,
    mPlayer_ITEM_KIND_UMBRELLA28,
    mPlayer_ITEM_KIND_UMBRELLA29,
    mPlayer_ITEM_KIND_UMBRELLA30,
    mPlayer_ITEM_KIND_UMBRELLA31,

    mPlayer_ITEM_KIND_ORG_UMBRELLA00,
    mPlayer_ITEM_KIND_ORG_UMBRELLA01,
    mPlayer_ITEM_KIND_ORG_UMBRELLA02,
    mPlayer_ITEM_KIND_ORG_UMBRELLA03,
    mPlayer_ITEM_KIND_ORG_UMBRELLA04,
    mPlayer_ITEM_KIND_ORG_UMBRELLA05,
    mPlayer_ITEM_KIND_ORG_UMBRELLA06,
    mPlayer_ITEM_KIND_ORG_UMBRELLA07,

    mPlayer_ITEM_KIND_ROD,
    mPlayer_ITEM_KIND_GOLD_ROD,

    mPlayer_ITEM_KIND_SHOVEL,
    mPlayer_ITEM_KIND_GOLD_SHOVEL,

    mPlayer_ITEM_KIND_RED_BALLOON,
    mPlayer_ITEM_KIND_YELLOW_BALLOON,
    mPlayer_ITEM_KIND_BLUE_BALLOON,
    mPlayer_ITEM_KIND_GREEN_BALLOON,
    mPlayer_ITEM_KIND_PURPLE_BALLOON,
    mPlayer_ITEM_KIND_BUNNY_P_BALLOON,
    mPlayer_ITEM_KIND_BUNNY_B_BALLOON,
    mPlayer_ITEM_KIND_BUNNY_O_BALLOON,

    mPlayer_ITEM_KIND_YELLOW_PINWHEEL,
    mPlayer_ITEM_KIND_RED_PINWHEEL,
    mPlayer_ITEM_KIND_TIGER_PINWHEEL,
    mPlayer_ITEM_KIND_GREEN_PINWHEEL,
    mPlayer_ITEM_KIND_PINK_PINWHEEL,
    mPlayer_ITEM_KIND_STRIPED_PINWHEEL,
    mPlayer_ITEM_KIND_FLOWER_PINWHEEL,
    mPlayer_ITEM_KIND_FANCY_PINWHEEL,

    mPlayer_ITEM_KIND_BLUEBELL_FAN,
    mPlayer_ITEM_KIND_PLUM_FAN,
    mPlayer_ITEM_KIND_BAMBOO_FAN,
    mPlayer_ITEM_KIND_CLOUD_FAN,
    mPlayer_ITEM_KIND_MAPLE_FAN,
    mPlayer_ITEM_KIND_FAN_FAN,
    mPlayer_ITEM_KIND_FLOWER_FAN,
    mPlayer_ITEM_KIND_LEAF_FAN,

    mPlayer_ITEM_KIND_NUM /* Are there more? */
};

enum {
    mPlayer_ITEM_DATA_AXE,      // model
    mPlayer_ITEM_DATA_AXE_B,    // model
    mPlayer_ITEM_DATA_AXE_C,    // model
    mPlayer_ITEM_DATA_GOLD_AXE, // model

    mPlayer_ITEM_DATA_NET,            // skeleton
    mPlayer_ITEM_DATA_GOLD_NET,       // skeleton
    mPlayer_ITEM_DATA_NET_GET_M,      // animation
    mPlayer_ITEM_DATA_NET_SWING,      // animation
    mPlayer_ITEM_DATA_KAMAE_MAIN_M,   // animation
    mPlayer_ITEM_DATA_KOKERU_GETUP_N, // animation
    mPlayer_ITEM_DATA_KOKERU_N,       // animation
    mPlayer_ITEM_DATA_SWING_WAIT,     // animation
    mPlayer_ITEM_DATA_YATTA_M,        // animation

    mPlayer_ITEM_DATA_ROD,           // skeleton
    mPlayer_ITEM_DATA_GOLD_ROD,      // skeleton
    mPlayer_ITEM_DATA_ROD_GET_T,     // animation
    mPlayer_ITEM_DATA_ROD_MOVE,      // animation
    mPlayer_ITEM_DATA_ROD_SINARI,    // animation
    mPlayer_ITEM_DATA_ROD_SWING,     // animation
    mPlayer_ITEM_DATA_ROD_WAIT,      // animation
    mPlayer_ITEM_DATA_ROD_NOT_SWING, // animation

    mPlayer_ITEM_DATA_SHOVEL,      // model
    mPlayer_ITEM_DATA_GOLD_SHOVEL, // model

    mPlayer_ITEM_DATA_BALLOON1,      // skeleton
    mPlayer_ITEM_DATA_BALLOON2,      // skeleton
    mPlayer_ITEM_DATA_BALLOON3,      // skeleton
    mPlayer_ITEM_DATA_BALLOON4,      // skeleton
    mPlayer_ITEM_DATA_BALLOON5,      // skeleton
    mPlayer_ITEM_DATA_BALLOON6,      // skeleton
    mPlayer_ITEM_DATA_BALLOON7,      // skeleton
    mPlayer_ITEM_DATA_BALLOON8,      // skeleton
    mPlayer_ITEM_DATA_BALLOON_WAIT,  // animation
    mPlayer_ITEM_DATA_BALLOON_GYAZA, // animation

    mPlayer_ITEM_DATA_PINWHEEL1,     // skeleton
    mPlayer_ITEM_DATA_PINWHEEL2,     // skeleton
    mPlayer_ITEM_DATA_PINWHEEL3,     // skeleton
    mPlayer_ITEM_DATA_PINWHEEL4,     // skeleton
    mPlayer_ITEM_DATA_PINWHEEL5,     // skeleton
    mPlayer_ITEM_DATA_PINWHEEL6,     // skeleton
    mPlayer_ITEM_DATA_PINWHEEL7,     // skeleton
    mPlayer_ITEM_DATA_PINWHEEL8,     // skeleton
    mPlayer_ITEM_DATA_PINWHEEL_WAIT, // animation

    mPlayer_ITEM_DATA_FAN1, // model
    mPlayer_ITEM_DATA_FAN2, // model
    mPlayer_ITEM_DATA_FAN3, // model
    mPlayer_ITEM_DATA_FAN4, // model
    mPlayer_ITEM_DATA_FAN5, // model
    mPlayer_ITEM_DATA_FAN6, // model
    mPlayer_ITEM_DATA_FAN7, // model
    mPlayer_ITEM_DATA_FAN8, // model

    mPlayer_ITEM_DATA_NUM
};

enum {
    mPlayer_ITEM_DATA_TYPE_GFX,
    mPlayer_ITEM_DATA_TYPE_SKELETON,
    mPlayer_ITEM_DATA_TYPE_NET_ANIMATION,
    mPlayer_ITEM_DATA_TYPE_ROD_ANIMATION,
    mPlayer_ITEM_DATA_TYPE_BALLOON_ANIMATION,
    mPlayer_ITEM_DATA_TYPE_PINWHEEL_ANIMATION,

    mPlayer_ITEM_DATA_TYPE_NUM
};

enum {
    mPlayer_PART_TABLE_NORMAL,
    mPlayer_PART_TABLE_AXE,
    mPlayer_PART_TABLE_PICK_UP,
    mPlayer_PART_TABLE_NET,

    mPlayer_PART_TABLE_NUM
};

enum {
    mPlayer_CREATURE_INSECT,
    mPlayer_CREATURE_GYOEI,
    mPlayer_CREATURE_BALLOON,

    mPlayer_CREATURE_NUM
};

enum {
    mPlayer_BED_ACTION_NONE,
    mPlayer_BED_ACTION_ROLL,
    mPlayer_BED_ACTION_OUT,

    mPlayer_BED_ACTION_NUM
};

typedef struct player_request_return_demo_s {
    int prev_main_index;
    f32 time;
} mPlayer_request_return_demo_c;

typedef struct player_request_wait_s {
    f32 morph_speed;
    f32 _04;
    int flags;
} mPlayer_request_wait_c;

typedef struct player_request_talk_s {
    ACTOR* talk_actor;
    int turn_flag;
    f32 morph_speed;
    int umbrella_flag;
} mPlayer_request_talk_c;

typedef struct player_request_hold_s {
    int ftr_no;
    s16 angle_y;
    xyz_t player_pos;
    f32 morph_speed;
    int flags;
} mPlayer_request_hold_c;

typedef struct player_request_recieve_wait_s {
    ACTOR* talk_actor;
    int turn_flag;
    int prev_main_index;
    mActor_name_t item;
    int on_surface_flag;
} mPlayer_request_recieve_wait_c;

typedef struct player_request_give_s {
    ACTOR* talk_actor;
    int turn_flag;
    int prev_main_index;
    mActor_name_t item;
    int mode;
    int present_flag;
    int on_surface_flag;
} mPlayer_request_give_c;

typedef struct player_request_sitdown_s {
    s16 angle;
    xyz_t pos;
    int ftrID;
} mPlayer_request_sitdown_c;

typedef struct player_request_close_funriture_s {
    int anim_idx;
} mPlayer_request_close_furniture_c;

typedef struct player_request_lie_bed_s {
    int direct;
    s16 angle;
    xyz_t wpos;
    int ftr_name;
} mPlayer_request_lie_bed_c;

typedef struct player_request_door_s {
    s16 angle_y;
    xyz_t player_pos;
    int type;
    u32 label;
} mPlayer_request_door_c;

typedef struct player_request_outdoor_s {
    int type;
    int is_start_demo;
} mPlayer_request_outdoor_c;

typedef struct player_request_demo_wait_s {
    int umbrella_flag;
    int _04;
} mPlayer_request_demo_wait_c;

typedef struct player_request_demo_walk_s {
    f32 goal_pos_x;
    f32 goal_pos_z;
    f32 speed;
    int _0C;
} mPlayer_request_demo_walk_c;

typedef struct player_request_demo_geton_train_s {
    s16 angle_y;
    xyz_t player_pos;
} mPlayer_request_demo_geton_train_c;

typedef struct player_request_demo_getoff_train_s {
    s16 angle_y;
    xyz_t player_pos;
} mPlayer_request_demo_getoff_train_c;

typedef struct player_request_rotate_octagon_s {
    ACTOR* talk_actor;
    int turn_flag;
    int prev_main_index;
    xyz_t pos;
    s16 angle_y;
} mPlayer_request_rotate_octagon_c;

typedef struct player_request_wash_car_s {
    xyz_t car_pos;
    xyz_t orig_pos;
    s16 orig_angle_y;
    ACTOR* parent_actor;
} mPlayer_request_wash_car_c;

typedef struct player_request_throw_money_s {
    xyz_t pos;
    s16 angle_y;
} mPlayer_request_throw_money_c;

typedef struct player_request_pray_s {
    xyz_t pos;
    s16 angle_y;
} mPlayer_request_pray_c;

typedef struct player_request_mail_jump_s {
    xyz_t pos;
    s16 angle_y;
} mPlayer_request_mail_jump_c;

typedef struct player_request_shock_s {
    f32 timer;
    s16 target_angle_y;
    u8 bee_chase_bgm_flag;
    int axe_broke_flag;
} mPlayer_request_shock_c;

typedef struct player_request_push_snowball_s {
    u32 type;
    int wade_type;
} mPlayer_request_push_snowball_c;

typedef struct player_request_stung_mosquito_s {
    u32 label;
} mPlayer_request_stung_mosquito_c;

typedef struct player_request_switch_on_lighthouse_s {
    s16 angle_y;
    xyz_t pos;
} mPlayer_request_switch_on_lighthouse_c;

typedef struct player_request_demo_geton_boat_s {
    xyz_t pos;
    s16 angle_y;
} mPlayer_request_demo_geton_boat_c;

typedef struct player_request_demo_getoff_boat_standup_s {
    xyz_t pos;
    s16 angle_y;
} mPlayer_request_demo_getoff_boat_standup_c;

typedef struct player_request_demo_get_golden_item_s {
    u32 label;
} mPlayer_request_demo_get_golden_item_c;

typedef struct player_request_walk_s {
    xyz_t pos;
    f32 morph_speed;
    int flags;
} mPlayer_request_walk_c;

typedef struct player_request_run_s {
    f32 morph_speed;
    int flags;
} mPlayer_request_run_c;

typedef struct player_request_dash_s {
    f32 morph_speed;
    int flags;
} mPlayer_request_dash_c;

typedef struct player_request_turn_dash_s {
    s16 target_angle_y;
} mPlayer_request_turn_dash_c;

typedef struct player_request_fall_s {
    f32 morph_speed;
    int flags;
} mPlayer_request_fall_c;

typedef struct player_request_wade_s {
    int dir;
} mPlayer_request_wade_c;

typedef struct player_request_wade_snowball_s {
    int dir;
    xyz_t pos;
    u32 label;
} mPlayer_request_wade_snowball_c;

typedef struct player_request_push_s {
    int ftrNo;
    s16 angle_y;
    xyz_t player_pos;
} mPlayer_request_push_c;

typedef struct player_request_pull_s {
    int ftrNo;
    s16 angle_y;
    xyz_t start_pos;
    xyz_t end_pos;
    xyz_t ofs;
} mPlayer_request_pull_c;

typedef struct player_request_rotate_furniture_s {
    int ftrNo;
    s16 angle_y;
    xyz_t player_pos;
    int rotate_type;
} mPlayer_request_rotate_furniture_c;

typedef struct player_request_wait_open_furniture_s {
    int anim_idx;
} mPlayer_request_wait_open_furniture_c;

typedef struct player_request_wait_close_furniture_s {
    int anim_idx;
} mPlayer_request_wait_close_furniture_c;

typedef struct player_request_roll_bed_s {
    int move_dir;
} mPlayer_request_roll_bed_c;

typedef struct player_request_standup_bed_s {
    int move_dir;
} mPlayer_request_standup_bed_c;

typedef struct player_request_pickup_s {
    int inv_slot;
    mActor_name_t item;
    xyz_t end_pos;
    xyz_t start_pos;
    int flag;
} mPlayer_request_pickup_c;

typedef struct player_request_pickup_jump_s {
    int inv_slot;
    mActor_name_t item;
    xyz_t start_pos;
    int flag;
} mPlayer_request_pickup_jump_c;

typedef struct player_request_pickup_furniture_s {
    int inv_slot;
    mActor_name_t item;
    xyz_t start_pos;
} mPlayer_request_pickup_furniture_c;

typedef struct player_request_pickup_exchange_s {
    xyz_t end_pos;
    mActor_name_t item;
} mPlayer_request_pickup_exchange_c;

typedef struct player_request_sitdown_wait_s {
    int ftrID;
} mPlayer_request_sitdown_wait_c;

typedef struct player_request_standup_s {
    int ftrID;
} mPlayer_request_standup_c;

typedef struct player_request_swing_axe_s {
    xyz_t goal_pos;
    mActor_name_t hit_item;
    u16 axe_damage_no;
    int hit_ut_x;
    int hit_ut_z;
    int break_flag;
} mPlayer_request_swing_axe_c;

typedef struct player_request_reflect_axe_s {
    xyz_t goal_pos;
    mActor_name_t hit_item;
    u16 axe_damage_no;
    ACTOR* hit_actor;
    int _14;
    int break_flag;
} mPlayer_request_reflect_axe_c;

typedef struct player_request_notice_net_s {
    int _00;
} mPlayer_request_notice_net_c;

typedef struct player_request_putaway_net_s {
    int _00;
} mPlayer_request_putaway_net_c;

typedef struct player_request_cast_rod_s {
    xyz_t bobber_target_pos;
} mPlayer_request_cast_rod_c;

typedef struct player_request_notice_rod_s {
    s16 angle_y;
} mPlayer_request_notice_rod_c;

typedef struct player_request_putaway_rod_s {
    s16 angle_y;
    int _04;
} mPlayer_request_putaway_rod_c;

typedef struct player_request_dig_scoop_s {
    xyz_t dig_pos;
    mActor_name_t item;
} mPlayer_request_dig_scoop_c;

typedef struct player_request_fill_scoop_s {
    xyz_t dig_pos;
} mPlayer_request_fill_scoop_c;

typedef struct player_request_reflect_scoop_s {
    xyz_t dig_pos;
    mActor_name_t item;
    ACTOR* hit_actor;
} mPlayer_request_reflect_scoop_c;

typedef struct player_request_putin_scoop_s {
    xyz_t dig_pos;
    mActor_name_t item;
    int _10;
} mPlayer_request_putin_scoop_c;

typedef struct player_request_putaway_scoop_s {
    xyz_t dig_pos;
    mActor_name_t item;
    int _10;
} mPlayer_request_putaway_scoop_c;

typedef struct player_request_get_scoop_s {
    xyz_t dig_pos;
    mActor_name_t item;
} mPlayer_request_get_scoop_c;

typedef struct player_request_recieve_stretch_s {
    ACTOR* talk_actor;
    int turn_flag;
    int prev_main_index;
    mActor_name_t item;
    int on_surface_flag;
} mPlayer_request_recieve_stretch_c;

typedef struct player_request_recieve_s {
    ACTOR* talk_actor;
    int turn_flag;
    int prev_main_index;
    mActor_name_t item;
    int on_surface_flag;
} mPlayer_request_recieve_c;

typedef struct player_request_recieve_putaway_s {
    ACTOR* talk_actor;
    int turn_flag;
    int prev_main_index;
    mActor_name_t item;
    int on_surface_flag;
} mPlayer_request_recieve_putaway_c;

typedef struct player_request_give_wait_s {
    ACTOR* talk_actor;
    int turn_flag;
    int prev_main_index;
    mActor_name_t item;
    int on_surface_flag; // TODO: verify this flag is correct
} mPlayer_request_give_wait_c;

typedef struct player_request_demo_wade_s {
    int dir;
} mPlayer_request_demo_wade_c;

typedef struct player_request_release_creature_gyoei_s {
    s16 angle_y;
    s16 _02;
} mPlayer_request_release_creature_gyoei_c;

typedef struct player_request_release_creature_insect_s {
    xyz_t pos;
    int type;
} mPlayer_request_release_creature_insect_c;

typedef struct player_request_release_creature_balloon_s {
    int _00;
} mPlayer_request_release_creature_balloon_c;

typedef union {
    mPlayer_request_release_creature_gyoei_c gyoei;
    mPlayer_request_release_creature_insect_c insect;
    mPlayer_request_release_creature_balloon_c balloon;
} mPlayer_request_release_creature_u;

typedef struct player_request_release_creature_s {
    int type;
    mPlayer_request_release_creature_u data;
    ACTOR* actor;
} mPlayer_request_release_creature_c;

typedef struct player_request_shake_tree_s {
    xyz_t pos;
    mActor_name_t item;
    int ut_x;
    int ut_z;
} mPlayer_request_shake_tree_c;

typedef struct player_request_ready_pitfall_s {
    xyz_t pos;
} mPlayer_request_ready_pitfall_c;

typedef struct player_request_remove_grass_s {
    xyz_t start_pos;
    xyz_t end_pos;
} mPlayer_request_remove_grass_c;

typedef struct player_request_change_cloth_s {
    ACTOR* talk_actor;
    int turn_flag;
    int prev_main_index;
    mActor_name_t cloth_item;
    u16 cloth_idx;
    int _10;
} mPlayer_request_change_cloth_c;

typedef struct player_request_swing_fan_s {
    int anim_idx;
} mPlayer_request_swing_fan_c;

typedef struct player_request_radio_exercise_s {
    int anim_idx;
    f32 morph_speed;
} mPlayer_request_radio_exercise_c;

typedef struct player_request_demo_geton_boat_wade_s {
    int dir;
    f32 speed; // TODO: check this
} mPlayer_request_demo_geton_boat_wade_c;

typedef struct player_request_demo_getoff_boat_s {
    xyz_t pos;
    s16 angle_y;
} mPlayer_request_demo_getoff_boat_c;

typedef struct player_request_give_from_submenu_s {
    xyz_t unused; // assumed based on size
    mActor_name_t item;
    int mode_after;   // player mode after the action finishes
    int present_flag; // set if the item is wrapped as presnet
    int counter_flag; // set when the item slides across a counter
} mPlayer_request_give_from_submenu_c;

typedef struct player_request_putin_scoop_from_submenu_s {
    xyz_t wpos;
    mActor_name_t item;
    int got_gold_scoop;
} mPlayer_request_putin_scoop_from_submenu_data_c;

typedef struct {
    int type;
    int got_gold_scoop;
    s16 angle_y;
    s16 item;
} mPlayer_request_release_creature_gyoei_from_submenu_c;

typedef struct {
    int type;
    int got_gold_scoop;
    xyz_t wpos;
    int insect_type;
} mPlayer_request_release_creature_insect_from_submenu_c;

typedef struct {
    int type;
    int got_gold_scoop;
    int balloon_shape_type;
} mPlayer_request_release_creature_balloon_from_submenu_c;

typedef struct {
    int _0;
    ACTOR* speak_actor;
} mPlayer_request_demo_wait_from_submenu_c;

typedef union {
    mPlayer_request_give_from_submenu_c give_from_submenu;
    mPlayer_request_putin_scoop_from_submenu_data_c putin_scoop_from_submenu;
    mPlayer_request_release_creature_gyoei_from_submenu_c release_creature_gyoei_from_submenu;
    mPlayer_request_release_creature_insect_from_submenu_c release_creature_insect_from_submenu;
    mPlayer_request_release_creature_balloon_from_submenu_c release_creature_balloon_from_submenu;
    mPlayer_request_demo_wait_from_submenu_c demo_wait_from_submenu;
    mPlayer_request_return_demo_c return_demo;
    mPlayer_request_wait_c wait;
    mPlayer_request_talk_c talk;
    mPlayer_request_hold_c hold;
    mPlayer_request_recieve_wait_c recieve_wait;
    mPlayer_request_give_c give;
    mPlayer_request_sitdown_c sitdown;
    mPlayer_request_close_furniture_c close_furniture;
    mPlayer_request_lie_bed_c lie_bed;
    mPlayer_request_door_c door;
    mPlayer_request_outdoor_c outdoor;
    mPlayer_request_demo_wait_c demo_wait;
    mPlayer_request_demo_walk_c demo_walk;
    mPlayer_request_demo_geton_train_c demo_geton_train;
    mPlayer_request_demo_getoff_train_c demo_getoff_train;
    mPlayer_request_rotate_octagon_c rotate_octagon;
    mPlayer_request_wash_car_c wash_car;
    mPlayer_request_throw_money_c throw_money;
    mPlayer_request_pray_c pray;
    mPlayer_request_mail_jump_c mail_jump;
    mPlayer_request_shock_c shock;
    mPlayer_request_push_snowball_c push_snowball;
    mPlayer_request_stung_mosquito_c stung_mosquito;
    mPlayer_request_switch_on_lighthouse_c switch_on_lighthouse;
    mPlayer_request_demo_geton_boat_c demo_geton_boat;
    mPlayer_request_demo_getoff_boat_standup_c demo_getoff_boat_standup;
    mPlayer_request_demo_get_golden_item_c demo_get_golden_item;
    mPlayer_request_walk_c walk;
    mPlayer_request_run_c run;
    mPlayer_request_dash_c dash;
    mPlayer_request_turn_dash_c turn_dash;
    mPlayer_request_fall_c fall;
    mPlayer_request_wade_c wade;
    mPlayer_request_wade_snowball_c wade_snowball;
    mPlayer_request_push_c push;
    mPlayer_request_pull_c pull;
    mPlayer_request_rotate_furniture_c rotate_furniture;
    mPlayer_request_wait_open_furniture_c wait_open_furniture;
    mPlayer_request_wait_close_furniture_c wait_close_furniture;
    mPlayer_request_roll_bed_c roll_bed;
    mPlayer_request_standup_bed_c standup_bed;
    mPlayer_request_pickup_c pickup;
    mPlayer_request_pickup_jump_c pickup_jump;
    mPlayer_request_pickup_furniture_c pickup_furniture;
    mPlayer_request_pickup_exchange_c pickup_exchange;
    mPlayer_request_sitdown_wait_c sitdown_wait;
    mPlayer_request_standup_c standup;
    mPlayer_request_swing_axe_c swing_axe;
    mPlayer_request_reflect_axe_c reflect_axe;
    mPlayer_request_notice_net_c notice_net;
    mPlayer_request_putaway_net_c putaway_net;
    mPlayer_request_cast_rod_c cast_rod;
    mPlayer_request_notice_rod_c notice_rod;
    mPlayer_request_putaway_rod_c putaway_rod;
    mPlayer_request_dig_scoop_c dig_scoop;
    mPlayer_request_fill_scoop_c fill_scoop;
    mPlayer_request_reflect_scoop_c reflect_scoop;
    mPlayer_request_putin_scoop_c putin_scoop;
    mPlayer_request_putaway_scoop_c putaway_scoop;
    mPlayer_request_get_scoop_c get_scoop;
    mPlayer_request_recieve_stretch_c recieve_stretch;
    mPlayer_request_recieve_c recieve;
    mPlayer_request_recieve_putaway_c recieve_putaway;
    mPlayer_request_give_wait_c give_wait;
    mPlayer_request_demo_wade_c demo_wade;
    mPlayer_request_release_creature_c release_creature;
    mPlayer_request_shake_tree_c shake_tree;
    mPlayer_request_ready_pitfall_c ready_pitfall;
    mPlayer_request_remove_grass_c remove_grass;
    mPlayer_request_change_cloth_c change_cloth;
    mPlayer_request_swing_fan_c swing_fan;
    mPlayer_request_radio_exercise_c radio_exercise;
    mPlayer_request_demo_geton_boat_wade_c demo_geton_boat_wade;
    mPlayer_request_demo_getoff_boat_c demo_getoff_boat;
    u64 align; // TODO: is this necessary? it makes the size correct for this and mPlayer_request_main_data
} mPlayer_request_backup_u;

typedef struct player_request_takeout_item_s {
    mPlayer_request_backup_u request_data;
    int request_main_index;
} mPlayer_request_takeout_item_c;

typedef struct player_request_putin_item_s {
    mPlayer_request_backup_u request_data;
    int request_main_index;
} mPlayer_request_putin_item_c;

typedef struct player_request_knock_door_s {
    mPlayer_request_backup_u request_data;
    int request_main_index;
    xyz_t pos;
    s16 angle_y;
} mPlayer_request_knock_door_c;

typedef union {
    mPlayer_request_give_from_submenu_c give_from_submenu;
    mPlayer_request_putin_scoop_from_submenu_data_c putin_scoop_from_submenu;
    mPlayer_request_release_creature_gyoei_from_submenu_c release_creature_gyoei_from_submenu;
    mPlayer_request_release_creature_insect_from_submenu_c release_creature_insect_from_submenu;
    mPlayer_request_release_creature_balloon_from_submenu_c release_creature_balloon_from_submenu;
    mPlayer_request_demo_wait_from_submenu_c demo_wait_from_submenu;
    mPlayer_request_return_demo_c return_demo;
    mPlayer_request_wait_c wait;
    mPlayer_request_talk_c talk;
    mPlayer_request_hold_c hold;
    mPlayer_request_recieve_wait_c recieve_wait;
    mPlayer_request_give_c give;
    mPlayer_request_sitdown_c sitdown;
    mPlayer_request_close_furniture_c close_furniture;
    mPlayer_request_lie_bed_c lie_bed;
    mPlayer_request_door_c door;
    mPlayer_request_outdoor_c outdoor;
    mPlayer_request_demo_wait_c demo_wait;
    mPlayer_request_demo_walk_c demo_walk;
    mPlayer_request_demo_geton_train_c demo_geton_train;
    mPlayer_request_demo_getoff_train_c demo_getoff_train;
    mPlayer_request_rotate_octagon_c rotate_octagon;
    mPlayer_request_wash_car_c wash_car;
    mPlayer_request_throw_money_c throw_money;
    mPlayer_request_pray_c pray;
    mPlayer_request_mail_jump_c mail_jump;
    mPlayer_request_shock_c shock;
    mPlayer_request_push_snowball_c push_snowball;
    mPlayer_request_stung_mosquito_c stung_mosquito;
    mPlayer_request_switch_on_lighthouse_c switch_on_lighthouse;
    mPlayer_request_demo_geton_boat_c demo_geton_boat;
    mPlayer_request_demo_getoff_boat_standup_c demo_getoff_boat_standup;
    mPlayer_request_demo_get_golden_item_c demo_get_golden_item;
    mPlayer_request_walk_c walk;
    mPlayer_request_run_c run;
    mPlayer_request_dash_c dash;
    mPlayer_request_turn_dash_c turn_dash;
    mPlayer_request_fall_c fall;
    mPlayer_request_wade_c wade;
    mPlayer_request_wade_snowball_c wade_snowball;
    mPlayer_request_push_c push;
    mPlayer_request_pull_c pull;
    mPlayer_request_rotate_furniture_c rotate_furniture;
    mPlayer_request_wait_open_furniture_c wait_open_furniture;
    mPlayer_request_wait_close_furniture_c wait_close_furniture;
    mPlayer_request_roll_bed_c roll_bed;
    mPlayer_request_standup_bed_c standup_bed;
    mPlayer_request_pickup_c pickup;
    mPlayer_request_pickup_jump_c pickup_jump;
    mPlayer_request_pickup_furniture_c pickup_furniture;
    mPlayer_request_pickup_exchange_c pickup_exchange;
    mPlayer_request_sitdown_wait_c sitdown_wait;
    mPlayer_request_standup_c standup;
    mPlayer_request_swing_axe_c swing_axe;
    mPlayer_request_reflect_axe_c reflect_axe;
    mPlayer_request_notice_net_c notice_net;
    mPlayer_request_putaway_net_c putaway_net;
    mPlayer_request_cast_rod_c cast_rod;
    mPlayer_request_notice_rod_c notice_rod;
    mPlayer_request_putaway_rod_c putaway_rod;
    mPlayer_request_dig_scoop_c dig_scoop;
    mPlayer_request_fill_scoop_c fill_scoop;
    mPlayer_request_reflect_scoop_c reflect_scoop;
    mPlayer_request_putin_scoop_c putin_scoop;
    mPlayer_request_putaway_scoop_c putaway_scoop;
    mPlayer_request_get_scoop_c get_scoop;
    mPlayer_request_recieve_stretch_c recieve_stretch;
    mPlayer_request_recieve_c recieve;
    mPlayer_request_recieve_putaway_c recieve_putaway;
    mPlayer_request_give_wait_c give_wait;
    mPlayer_request_demo_wade_c demo_wade;
    mPlayer_request_release_creature_c release_creature;
    mPlayer_request_shake_tree_c shake_tree;
    mPlayer_request_ready_pitfall_c ready_pitfall;
    mPlayer_request_remove_grass_c remove_grass;
    mPlayer_request_change_cloth_c change_cloth;
    mPlayer_request_swing_fan_c swing_fan;
    mPlayer_request_radio_exercise_c radio_exercise;
    mPlayer_request_demo_geton_boat_wade_c demo_geton_boat_wade;
    mPlayer_request_demo_getoff_boat_c demo_getoff_boat;
    mPlayer_request_takeout_item_c takeout_item;
    mPlayer_request_putin_item_c putin_item;
    mPlayer_request_knock_door_c knock_door;
    /* TODO: others? */
    // u8 force_size[72]; // TEMP
} mPlayer_request_main_data;

/* Current main state data structs */

typedef struct player_main_intro_s {
    f32 timer;
} mPlayer_main_intro_c;

typedef struct player_main_return_demo_s {
    f32 timer;
    f32 max_timer;
    int prev_main_index;
} mPlayer_main_return_demo_c;

typedef struct player_main_return_outdoor_s {
    f32 timer;
    f32 max_timer;
    int prev_main_index;
} mPlayer_main_return_outdoor_c;

typedef struct player_main_return_outdoor2_s {
    f32 timer;
    f32 max_timer;
    int prev_main_index;
} mPlayer_main_return_outdoor2_c;

typedef union {
    mPlayer_main_intro_c intro;
    mPlayer_main_return_demo_c return_demo;
    mPlayer_main_return_outdoor_c return_outdoor;
    mPlayer_main_return_outdoor2_c return_outdoor2;
    u8 force_size[72]; // TEMP
} mPlayer_main_data;

typedef struct {
    int requested_main_index;
    int requested_index_pending;
    mPlayer_request_main_data request_main_data;
} mPlayer_change_data_from_submenu_c;

/* sizeof(struct player_actor_s) == 0x13A8 */
struct player_actor_s {
    /* 0x0000 */ ACTOR actor_class;
    /* 0x0174 */ cKF_SkeletonInfo_R_c keyframe0;
    /* 0x01E4 */ cKF_SkeletonInfo_R_c keyframe1;
    /* 0x0252 */ s_xyz joint_data[27];
    /* 0x02F4 */ s_xyz morph_data[27];
    /* 0x0398 */ Mtx work_mtx[2][13];                /* swapped between frames */
    /* 0x0A18 */ cKF_SkeletonInfo_R_c item_keyframe; /* for item animations */
    /* 0x0A88 */ s_xyz item_joint_data[8];
    /* 0x0AB8 */ s_xyz item_morph_data[8];
    /* 0x0AE8 */ Mtx item_work_mtx[2][4]; /* swapped between frames */
    /* 0x0CE8 */ s16 eye_pattern_normal;
    /* 0x0CEA */ s16 eye_pattern_normal_timer;
    /* 0x0CEC */ int blink_count;
    /* 0x0CF0 */ int eye_tex_idx;
    /* 0x0CF4 */ int mouth_tex_idx;
    /* 0x0CF8 */ int now_main_index;
    /* 0x0CFC */ int prev_main_index;
    /* 0x0D00 */ int changed_main_index;
    /* 0x0D04 */ int item_main_index;
    /* 0x0D08 */ int requested_main_index;
    /* 0x0D0C */ int requested_main_index_priority;
    /* 0x0D10 */ int requested_main_index_changed;
    /* 0x0D14 */ int settled_requested_main_index_priority;
    /* 0x0D18 */ mPlayer_main_data main_data;                         // TODO: Union of many types...
    /* 0x0D60 */ mPlayer_request_main_data requested_main_index_data; // Union of many types...
    /* 0x0DA8 */ u8 _0DA8[0x1010 - 0x0DA8];                           /* TODO: finish */
    /* 0x1010 */ ClObjPipe_c col_pipe;
    /* 0x102C */ xyz_t head_pos;
    /* 0x1038 */ xyz_t feel_pos;
    /* 0x1044 */ xyz_t right_hand_pos;
    /* 0x1050 */ xyz_t right_hand_move;
    /* 0x105C */ xyz_t left_hand_pos;
    /* 0x1068 */ MtxF right_hand_mtx;
    /* 0x10A8 */ MtxF left_hand_mtx;
    /* 0x10E8 */ u8 _10E8[0x11FC - 0x10E8]; // TODO
    /* 0x11FC */ int crash_snowball_for_wade;
    /* 0x1200 */ xyz_t snowball_dist;
    /* 0x120C */ int wade_request_flag;
    /* 0x1210 */ u16 cancel_wade_timer;
    /* 0x1214 */ int cancel_wade_flag;
    /* 0x1218 */ f32 geton_boat_wade;
    /* 0x121C */ u16 frame_timer;
    /* 0x121E */ s8 bee_chase_bgm_flag;
    /* 0x121F */ s8 status_for_bee;
    /* 0x1220 */ void* angle_force_speak_label;
    /* 0x1224 */ int player_sunburn_rankup;
    /* 0x1228 */ int player_sunburn_rankdown;
    /* 0x122C */ u8 radio_exercise_command_ring_buffer[8];
    /* 0x1234 */ s8 radio_exercise_ring_buffer_cmd_num;
    /* 0x1238 */ int radio_exercise_command_ring_buffer_index;
    /* 0x123C */ int radio_exercise_continue_cmd_idx;
    /* 0x1240 */ f32 radio_exercise_cmd_timer;
    /* 0x1244 */ int old_sound_frame_counter;
    /* 0x1248 */ s16 boat_angleZ;
    /* 0x124C */ int change_color_request;
    /* 0x1250 */ int change_color_flag;
    /* 0x1254 */ f32 change_color_timer;
    /* 0x1258 */ int change_color_rgb[3];
    /* 0x1264 */ int change_color_near;
    /* 0x1268 */ int change_color_far;
    /* 0x126C */ int refuse_pickup_knife_fork_flag;
    /* 0x1270 */ int (*request_main_invade_all_proc)(GAME*, int);
    /* 0x1274 */ int (*request_main_refuse_all_proc)(GAME*, int);
    /* 0x1278 */ int (*request_main_return_demo_all_proc)(GAME*, int, f32, int);
    /* 0x127C */ int (*request_main_wait_all_proc)(GAME*, f32, int, int);
    /* 0x1280 */ int (*request_main_talk_all_proc)(GAME*, ACTOR*, int, f32, int, int);
    /* 0x1284 */ int (*request_main_hold_all_proc)(GAME*, int, int, const xyz_t*, f32, int, int);
    /* 0x1288 */ int (*request_main_recieve_wait_all_proc)(GAME*, ACTOR*, int, int, mActor_name_t, int, int);
    /* 0x128C */ int (*request_main_give_all_proc)(GAME*, ACTOR*, int, int, mActor_name_t, int, int, int, int);
    /* 0x1290 */ int (*request_main_sitdown_all_proc)(GAME*, int, const xyz_t*, int, int);
    /* 0x1294 */ int (*request_main_close_furniture_all_proc)(GAME*, int);
    /* 0x1298 */ int (*request_main_lie_bed_all_proc)(GAME*, int, const xyz_t*, int, int, int);
    /* 0x129C */ int (*request_main_hide_all_proc)(GAME*, int);
    /* 0x12A0 */ int (*request_main_groundhog_proc)(GAME*, int);
    /* 0x12A4 */ int (*request_main_door_all_proc)(GAME*, const xyz_t*, s16, int, void*, int);
    /* 0x12A8 */ int (*request_main_outdoor_all_proc)(GAME*, int, int, int);
    /* 0x12AC */ int (*request_main_wash_car_all_proc)(GAME*, const xyz_t*, const xyz_t*, s16, ACTOR*, int);
    /* 0x12B0 */ int (*request_main_rotate_octagon_all_proc)(GAME*, ACTOR*, int, int, const xyz_t*, s16, int);
    /* 0x12B4 */ int (*request_main_throw_money_all_proc)(GAME*, const xyz_t*, s16, int);
    /* 0x12B8 */ int (*request_main_pray_all_proc)(GAME*, const xyz_t*, s16, int);
    /* 0x12BC */ int (*request_main_mail_jump_all_proc)(GAME*, const xyz_t*, s16, int);
    /* 0x12C0 */ int (*request_main_demo_wait_all_proc)(GAME*, int, void*, int);
    /* 0x12C4 */ int (*request_main_demo_walk_all_proc)(GAME*, f32, f32, f32, int, int);
    /* 0x12C8 */ int (*request_main_demo_geton_train_all_proc)(GAME*, const xyz_t*, s16, int);
    /* 0x12CC */ int (*request_main_demo_getoff_train_all_proc)(GAME*, const xyz_t*, s16, int);
    /* 0x12D0 */ int (*request_main_demo_standing_train_all_proc)(GAME*, int);
    /* 0x12D4 */ int (*request_main_stung_bee_all_proc)(GAME*, int);
    /* 0x12D8 */ int (*request_main_shock_all_proc)(GAME*, f32, s16, s8, int, int);
    /* 0x12DC */ int (*request_main_change_cloth_forNPC_proc)(GAME*, mActor_name_t, int);
    /* 0x12E0 */ int (*request_main_push_snowball_all_proc)(GAME*, void*, int, int);
    /* 0x12E4 */ int (*request_main_stung_mosquito_all_proc)(GAME*, int, int);
    /* 0x12E8 */ int (*request_main_switch_on_lighthouse_all_proc)(GAME*, const xyz_t*, int, int);
    /* 0x12EC */ int (*request_main_demo_geton_boat_all_proc)(GAME*, const xyz_t*, s16, int);
    /* 0x12F0 */ int (*request_main_demo_getoff_boat_standup_all_proc)(GAME*, const xyz_t*, s16, int);
    /* 0x12F4 */ int (*request_main_demo_get_golden_item2_all_proc)(GAME*, int, int);
    /* 0x12F8 */ int (*request_main_demo_get_golden_axe_wait_all_proc)(GAME*, int);
    /* 0x12FC */ int (*check_request_main_priority_proc)(GAME*, int);
    /* 0x1300 */ void* (*get_door_label_proc)(GAME*);
    /* 0x1304 */ int (*Set_Item_net_catch_request_table_proc)(ACTOR*, GAME*, u32, s8, const xyz_t*, f32);
    /* 0x1308 */ f32 (*Get_Item_net_catch_swing_timer_proc)(ACTOR*, GAME*);
    /* 0x130C */ u8 (*Set_Item_net_catch_request_force_proc)(ACTOR*, GAME*, u32, s8);
    /* 0x1310 */ void (*Set_force_position_angle_proc)(GAME*, const xyz_t*, const s_xyz*, u8);
    /* 0x1314 */ u8 (*Get_force_position_angle_proc)(GAME*, xyz_t*, s_xyz*);
    /* 0x1318 */ int (*Get_WadeEndPos_proc)(GAME*, xyz_t*);
    /* 0x131C */ int (*Check_Label_main_push_snowball_proc)(GAME*, void*);
    /* 0x1320 */ int (*SetParam_for_push_snowball_proc)(GAME*, const xyz_t*, s16, f32);
    /* 0x1324 */ int (*able_submenu_request_main_index_proc)(GAME*);
    /* 0x1328 */ int (*check_able_change_camera_normal_index_proc)(ACTOR*);
    /* 0x132C */ int (*Check_able_force_speak_label_proc)(GAME*, ACTOR*);
    /* 0x1330 */ int (*check_cancel_request_change_proc_index_proc)(int);
    /* 0x1334 */ u32 (*Get_item_net_catch_label_proc)(ACTOR*);
    /* 0x1338 */ int (*Change_item_net_catch_label_proc)(ACTOR*, u32, s8);
    /* 0x133C */ int (*Check_StopNet_proc)(ACTOR*, xyz_t*);
    /* 0x1340 */ int (*Check_HitAxe_proc)(ACTOR*, xyz_t*);
    /* 0x1344 */ int (*Check_VibUnit_OneFrame_proc)(ACTOR*, const xyz_t*);
    /* 0x1348 */ int (*Check_HitScoop_proc)(ACTOR*, xyz_t*);
    /* 0x134C */ int (*Check_DigScoop_proc)(ACTOR*, xyz_t*);
    /* 0x1350 */ int (*check_request_change_item_proc)(GAME*);
    /* 0x1354 */ int (*Check_RotateOctagon_proc)(GAME*);
    /* 0x1358 */ int (*Check_end_stung_bee_proc)(ACTOR*);
    /* 0x135C */ int (*Get_status_for_bee_proc)(ACTOR*);
    /* 0x1360 */ int (*Set_ScrollDemo_forWade_snowball_proc)(ACTOR*, int, const xyz_t*);
    /* 0x1364 */ int (*Check_tree_shaken_proc)(ACTOR*, const xyz_t*);
    /* 0x1368 */ int (*Check_tree_shaken_little_proc)(ACTOR*, const xyz_t*);
    /* 0x136C */ int (*Check_tree_shaken_big_proc)(ACTOR*, const xyz_t*);
    /* 0x1370 */ int (*Check_Label_main_wade_snowball_proc)(GAME*, void*);
    /* 0x1374 */ int (*GetSnowballPos_forWadeSnowball_proc)(ACTOR*, xyz_t*);
    /* 0x1378 */ int (*CheckCondition_forWadeSnowball_proc)(GAME*, const xyz_t*, s16);
    /* 0x137C */ mActor_name_t (*Get_itemNo_forWindow_proc)(ACTOR*);
    /* 0x1380 */ int (*check_cancel_event_without_priority_proc)(GAME*);
    /* 0x1384 */ int (*CheckScene_AbleSubmenu_proc)();
    /* 0x1388 */ int (*Check_stung_mosquito_proc)(GAME*, ACTOR*);
    /* 0x138C */ int a_btn_pressed;
    /* 0x1390 */ int a_btn_triggers_submenu;
    /* 0x1394 */ mActor_name_t item_in_front; /* item directly in front of the player */
    /* 0x1398 */ xyz_t forward_ut_pos;        /* wpos of unit in front of player */
    /* 0x13A4 */ s8 update_scene_bg_mode;
};

void Player_actor_ct(ACTOR*, GAME*);
void Player_actor_dt(ACTOR*, GAME*);
void Player_actor_move(ACTOR*, GAME*);
void Player_actor_draw(ACTOR*, GAME*);

#ifdef __cplusplus
}
#endif

#endif
