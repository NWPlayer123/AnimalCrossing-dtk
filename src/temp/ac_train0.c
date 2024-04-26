#include "ac_train0.h"

#include "m_name_table.h"
#include "m_common_data.h"
#include "m_field_info.h"
#include "m_event.h"
#include "sys_matrix.h"
#include "m_play.h"
#include "m_rcp.h"

static void aTR0_actor_ct(ACTOR* actor, GAME* game);
static void aTR0_actor_dt(ACTOR* actor, GAME* game);
static void aTR0_actor_move(ACTOR* actor, GAME* game);
static void aTR0_actor_draw(ACTOR* actor, GAME* game);

ACTOR_PROFILE Train0_Profile = {
    mAc_PROFILE_TRAIN0,
    ACTOR_PART_ITEM,
    ACTOR_STATE_CAN_MOVE_IN_DEMO_SCENES | ACTOR_STATE_TA_SET | ACTOR_STATE_NO_MOVE_WHILE_CULLED,
    TRAIN0,
    ACTOR_OBJ_BANK_KEEP,
    sizeof(TRAIN0_ACTOR),
    &aTR0_actor_ct,
    &aTR0_actor_dt,
    &aTR0_actor_move,
    &aTR0_actor_draw,
    NULL,
};

extern cKF_Skeleton_R_c cKF_bs_r_obj_train1_1;
extern cKF_Animation_R_c cKF_ba_r_obj_train1_1;

extern Gfx obj_train1_2_model[];

static void aTR0_actor_ct(ACTOR* actor, GAME* GAME) {
    TRAIN0_ACTOR* train0 = (TRAIN0_ACTOR*)actor;

    cKF_SkeletonInfo_R_ct(&train0->keyframe, &cKF_bs_r_obj_train1_1, NULL, train0->work, train0->target);
    cKF_SkeletonInfo_R_init(&train0->keyframe, train0->keyframe.skeleton, &cKF_ba_r_obj_train1_1, 1.0f, 25.0f, 1.0f,
                            0.5f, 0.0f, 1, NULL);
    cKF_SkeletonInfo_R_play(&train0->keyframe);
    train0->actor_class.cull_width = 600.0f;
    train0->actor_class.world.angle.y = 16384;
    train0->tr_action = 5;
}

static void aTR0_actor_dt(ACTOR* actor, GAME* game) {

    TRAIN0_ACTOR* train0 = (TRAIN0_ACTOR*)actor;

    xyz_t tr_home_pos;

    tr_home_pos = train0->actor_class.home.position;

    mFI_SetFG_common(0, tr_home_pos, 0);

    if (train0->tr_actor_p != NULL) {
        Actor_delete(train0->tr_actor_p);
        train0->tr_actor_p = NULL;
    }
    cKF_SkeletonInfo_R_dt(&train0->keyframe);
    Common_Set(train_exists_flag, 0);
}

#include "../src/ac_train0_move.c_inc"

#include "../src/ac_train0_draw.c_inc"
