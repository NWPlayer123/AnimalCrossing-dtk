#ifndef AC_MISIN_H
#define AC_MISIN_H

#include "types.h"
#include "m_actor.h"
#include "m_needlework.h"
#include "c_keyframe.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct misin_actor_s MISIN_ACTOR;

typedef void (*aMSN_REQUEST_MISIN_STOP_PROC)(void);
typedef void (*aMSN_REQUEST_MISIN_MOVE_PROC)(void);

typedef void (*aMSN_REQUEST_DUSTCLOTH_STOP_PROC)(void);
typedef void (*aMSN_REQUEST_DUSTCLOTH_MOVE_PROC)(void);

typedef struct misin_clip_s {
    MISIN_ACTOR* misin_actor_p;
    aMSN_REQUEST_MISIN_STOP_PROC request_misin_stop_proc;
    aMSN_REQUEST_MISIN_MOVE_PROC request_misin_move_proc;
    aMSN_REQUEST_DUSTCLOTH_STOP_PROC request_dustcloth_stop_proc;
    aMSN_REQUEST_DUSTCLOTH_MOVE_PROC request_dustcloth_move_proc;
} aMSN_Clip_c;

#define aMSN_GetClip() ((aMSN_Clip_c*)Common_Get(clip).misin_clip)

typedef struct misin_dustcloth_s {
    s16 switch_flag;
    s16 frame;
    xyz_t pos;
    xyz_t target_pos;
    int target_idx;
    s16 target_angle_y;
    int moving_flag;
    mNW_original_design_c* original_p;
    u16* palette_p;
} aMSN_DustCloth_c;

typedef struct misin_misin_s {
    s16 status;
    f32 speed;
    xyz_t pos;
    f32 needle_y_counter;
    xyz_t needle_offset;
    s16 y_scroll;
    cKF_SkeletonInfo_R_c keyframe;
    s_xyz work[17];
    s_xyz morph[17];
    Mtx mtx[2][16] ATTRIBUTE_ALIGN(8);
} aMSN_Misin_c;

struct misin_actor_s {
    ACTOR actor_class;
    aMSN_DustCloth_c dustcloth;
    aMSN_Misin_c misin;
    aMSN_Clip_c clip;
};

extern ACTOR_PROFILE Misin_Profile;

#ifdef __cplusplus
}
#endif

#endif
