#include "ef_effect_control.h"

#include "m_common_data.h"
#include "m_debug.h"
#include "sys_matrix.h"
#include "m_rcp.h"

#define eHM_TIMER 2000

static void eMH_init(xyz_t pos, int prio, s16 angle, GAME* game, u16 item_name, s16 arg0, s16 arg1) {
    (*eEC_CLIP->make_effect_proc)(eEC_EFFECT_MAKE_HEM, pos, NULL, game, NULL, item_name, prio, arg0, arg1);
}

static void eMH_ct(eEC_Effect_c* effect, GAME* game, void* ct_arg) {
    effect->timer = eHM_TIMER;
    effect->scale.x = 0.0f;
    effect->scale.y = 0.0f;
    effect->scale.z = 0.0f;

    if (effect->arg0 == 1) {
        effect->position.y += 30.0f;
    } else {
        effect->position.y += 20.0f;
    }

    if (effect->arg0 == 1) {
        int* const point_light_num = &eMH_special_point_light_num;

        if (*point_light_num == -1) {
            xyz_t pos = effect->position;

            pos.z += 20.0f;
            *point_light_num = mEnv_ReservePointLight((GAME_PLAY*)game, &pos, 0, 0, 0, GETREG(MYKREG, 0) + 100);
        }

        (*eEC_CLIP->effect_make_proc)(eEC_EFFECT_MAKE_HEM_LIGHT, effect->position, 2, 0, game, RSV_NO, 0, 0);
        sAdo_OngenTrgStart(0x468, &effect->position);
    } else {
        (*eEC_CLIP->effect_kill_proc)(eEC_EFFECT_MAKE_HEM_LIGHT, RSV_NO);
        sAdo_OngenTrgStart(0x469, &effect->position);
    }
}

static void eMH_mv(eEC_Effect_c* effect, GAME* game) {
    f32 default_col = 255.0f;
    s16 elapsed_time = eHM_TIMER - effect->timer;
    f32 light_intensity;
    u8 col;
    int i;

    if (elapsed_time <= 120) {
        if (effect->arg0 == 1) {
            light_intensity = (*eEC_CLIP->calc_adjust_proc)(elapsed_time, 0, 120, 0.0f, 1.0f);
        } else {
            light_intensity = 1.0f;
        }

        effect->effect_specific[0] = (int)(255.0f * (*eEC_CLIP->calc_adjust_proc)(elapsed_time, 0, 120, 0.0f, 1.0f));
    } else if (elapsed_time <= 150) {
        light_intensity = 1.0f;
        effect->effect_specific[0] = (int)default_col;

        if (elapsed_time == 150) {
            if (Common_Get(hem_visible) == FALSE) {
                Common_Set(hem_visible, TRUE);
            } else {
                Common_Set(hem_visible, FALSE);
            }

            /* Spawn 16 twinkle effects at random points around the central effect */
            for (i = 0; i < 16; i++) {
                xyz_t pos = effect->position;
                s16 rnd_angle = RANDOM_F(66536.0f); // lol nice typo here.. should be 65536.0f
                f32 z = 20.0f * cos_s(rnd_angle);
                f32 x = 20.0f * sin_s(rnd_angle);
                f32 y = -10.0f + RANDOM_F(25.0f);

                pos.x += x;
                pos.y += y;
                pos.z += z;

                (*eEC_CLIP->effect_make_proc)(eEC_EFFECT_MAKE_HEM_KIRA, pos, 2, 0, game, RSV_NO, 0, 0);
            }
        }
    } else {
        if (effect->arg0 == 1) {
            light_intensity = 1.0f;
        } else {
            light_intensity = (*eEC_CLIP->calc_adjust_proc)(elapsed_time, 151, 195, 1.0f, 0.0f);
        }

        effect->effect_specific[0] = (int)(255.0f * (*eEC_CLIP->calc_adjust_proc)(elapsed_time, 151, 195, 1.0f, 0.0f));

        if (elapsed_time == 196) {
            effect->timer = 0;

            if (effect->arg0 == 0) {
                int* const point_light_num = &eMH_special_point_light_num;

                if (*point_light_num != -1) {
                    *point_light_num = mEnv_CancelReservedPointLight(*point_light_num, (GAME_PLAY*)game);
                }
            }
        }
    }

    add_calc(&effect->scale.x, 0.575f, 0.05f, 0.1f, 0.001f);
    col = (int)(255.0f * light_intensity);
    mEnv_OperateReservedPointLight_Color(eMH_special_point_light_num, col, col, col);
}

extern Gfx ef_sphere_light_model[];

static void eMH_dw(eEC_Effect_c* effect, GAME* game) {
    GAME_PLAY* play = (GAME_PLAY*)game;
    xyz_t* pos = &effect->position;
    u8 alpha = effect->effect_specific[0];
    f32 scale = effect->scale.x * 0.01f;

    Setpos_HiliteReflect_xlu_init(pos, play);
    Matrix_translate(pos->x, pos->y, pos->z, 0);
    Matrix_scale(scale, scale, scale, 1);
    _texture_z_light_fog_prim(game->graph);

    OPEN_DISP(game->graph);

    gDPSetPrimColor(NEXT_POLY_XLU_DISP, 0, 255, 255, 255, 255, alpha);
    gSPMatrix(NEXT_POLY_XLU_DISP, _Matrix_to_Mtx_new(game->graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(NEXT_POLY_XLU_DISP, ef_sphere_light_model);

    CLOSE_DISP(game->graph);
}

eEC_PROFILE_c iam_ef_make_hem = {
    // clang-format off
    &eMH_init,
    &eMH_ct,
    &eMH_mv,
    &eMH_dw,
    eEC_IGNORE_DEATH,
    eEC_NO_CHILD_ID,
    eEC_IGNORE_DEATH_DIST,
    // clang-format on
};

static void eMHK_init(xyz_t pos, int prio, s16 angle, GAME* game, u16 item_name, s16 arg0, s16 arg1);
static void eMHK_ct(eEC_Effect_c* effect, GAME* game, void* ct_arg);
static void eMHK_mv(eEC_Effect_c* effect, GAME* game);
static void eMHK_dw(eEC_Effect_c* effect, GAME* game);

eEC_PROFILE_c iam_ef_make_hem_kira = {
    // clang-format off
    &eMHK_init,
    &eMHK_ct,
    &eMHK_mv,
    &eMHK_dw,
    eEC_IGNORE_DEATH,
    eEC_NO_CHILD_ID,
    eEC_IGNORE_DEATH_DIST,
    // clang-format on
};

static void eMHK_init(xyz_t pos, int prio, s16 angle, GAME* game, u16 item_name, s16 arg0, s16 arg1) {
    (*eEC_CLIP->make_effect_proc)(eEC_EFFECT_MAKE_HEM_KIRA, pos, NULL, game, NULL, item_name, prio, arg0, arg1);
}

static void eMHK_ct(eEC_Effect_c* effect, GAME* game, void* ct_arg) {
    static xyz_t xyz0 = { 0.0f, 0.0f, 0.0f };

    effect->timer = 500.0f;

    effect->acceleration = xyz0;
    effect->velocity = xyz0;
    effect->scale = xyz0;

    effect->velocity.y = -(RANDOM_F(0.1f) + 0.2f);
    effect->effect_specific[0] = RANDOM(32);
}

static void eMHK_mv(eEC_Effect_c* effect, GAME* game) {
    s16 elapsed_time = (effect->timer + effect->effect_specific[0]) & 15;
    f32 scale;

    xyz_t_add(&effect->velocity, &effect->acceleration, &effect->velocity);
    xyz_t_add(&effect->position, &effect->velocity, &effect->position);

    /* Shrink then scale */
    if (elapsed_time <= 7) {
        scale = (*eEC_CLIP->calc_adjust_proc)(elapsed_time, 0, 7, 0.0f, 0.0064999999f);
    } else {
        scale = (*eEC_CLIP->calc_adjust_proc)(elapsed_time, 8, 15, 0.0064999999f, 0.0f);
    }

    effect->scale.x = scale;
    effect->scale.y = scale;
    effect->scale.z = scale;

    if (elapsed_time == 0) {
        f32 bg_y = mCoBG_GetBgY_OnlyCenter_FromWpos2(effect->position, 0.0f);

        if (effect->position.y < bg_y) {
            effect->timer = 0;
        }
    }
}

extern Gfx ef_takurami01_normal_render_mode[];
extern Gfx ef_takurami01_kira_modelT[];

static void eMHK_dw(eEC_Effect_c* effect, GAME* game) {
    GAME_PLAY* play = (GAME_PLAY*)game;

    Matrix_translate(effect->position.x, effect->position.y, effect->position.z, 0);
    Matrix_mult(&play->billboard_matrix, 1);
    Matrix_scale(effect->scale.x, effect->scale.y, effect->scale.z, 1);
    _texture_z_light_fog_prim_xlu(game->graph);

    OPEN_DISP(game->graph);

    gSPMatrix(NEXT_POLY_XLU_DISP, _Matrix_to_Mtx_new(game->graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetPrimColor(NEXT_POLY_XLU_DISP, 0, 255, 255, 255, 255, 200);
    gSPSegment(NEXT_POLY_XLU_DISP, G_MWO_SEGMENT_8, ef_takurami01_normal_render_mode);
    gSPDisplayList(NEXT_POLY_XLU_DISP, ef_takurami01_kira_modelT);

    CLOSE_DISP(game->graph);
}

static void eMHL_init(xyz_t pos, int prio, s16 angle, GAME* game, u16 item_name, s16 arg0, s16 arg1) {
    (*eEC_CLIP->make_effect_proc)(eEC_EFFECT_MAKE_HEM_LIGHT, pos, NULL, game, NULL, item_name, prio, arg0, arg1);
}

static void eMHL_ct(eEC_Effect_c* effect, GAME* game, void* ct_arg) {
    effect->timer = 195;
    effect->effect_specific[0] = 0;
    effect->position.y = mCoBG_GetBgY_OnlyCenter_FromWpos2(effect->position, 0.0f);
}

static void eMHL_mv(eEC_Effect_c* effect, GAME* game) {
    (*eEC_CLIP->set_continious_env_proc)(effect, 195, 195);

    switch (effect->state) {
        case eEC_STATE_NORMAL: {
            s16 elapsed_time = 195 - effect->timer;

            if (elapsed_time < 120) {
                effect->effect_specific[0] = (int)(*eEC_CLIP->calc_adjust_proc)(elapsed_time, 0, 119, 170.0f, 0.0f);
            } else if (elapsed_time < 150) {
                effect->effect_specific[0] = 0;
            } else {
                effect->effect_specific[0] = (int)(*eEC_CLIP->calc_adjust_proc)(elapsed_time, 151, 175, 0.0f, 50.0f);
            }
            break;
        }

        case eEC_STATE_FINISHED: {
            s16 elapsed_time = 195 - effect->timer;

            if (elapsed_time < 120) {
                effect->effect_specific[0] = (int)(*eEC_CLIP->calc_adjust_proc)(elapsed_time, 0, 119, 50.0f, 0.0f);
            } else if (elapsed_time < 150) {
                effect->effect_specific[0] = 0;
            } else {
                effect->effect_specific[0] = (int)(*eEC_CLIP->calc_adjust_proc)(elapsed_time, 151, 195, 0.0f, 170.0f);
            }
            break;
        }

        default:
            effect->effect_specific[0] = 50;
    }
}

extern Gfx ef_circle_light_model[];

static void eMHL_dw(eEC_Effect_c* effect, GAME* game) {
    u8 alpha = effect->effect_specific[0];

    Matrix_translate(effect->position.x, effect->position.y + 0.1f, effect->position.z, 0);
    Matrix_scale(0.01f, 0.01f, 0.01f, 1);
    _texture_z_light_fog_prim_shadow(game->graph);

    OPEN_DISP(game->graph);

    gSPMatrix(NEXT_SHADOW_DISP, _Matrix_to_Mtx_new(game->graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetPrimColor(NEXT_SHADOW_DISP, 0, 0, 255, 255, 220, alpha);
    gDPSetEnvColor(NEXT_SHADOW_DISP, 255, 255, 255, 100);
    gSPDisplayList(NEXT_SHADOW_DISP, ef_circle_light_model);

    CLOSE_DISP(game->graph);
}

eEC_PROFILE_c iam_ef_make_hem_light = {
    // clang-format off
    &eMHL_init,
    &eMHL_ct,
    &eMHL_mv,
    &eMHL_dw,
    195,
    eEC_NO_CHILD_ID,
    eEC_IGNORE_DEATH_DIST,
    // clang-format on
};
