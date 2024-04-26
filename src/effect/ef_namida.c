#include "ef_effect_control.h"

static void eNamida_init(xyz_t pos, int prio, s16 angle, GAME* game, u16 item_name, s16 arg0, s16 arg1);
static void eNamida_ct(eEC_Effect_c* effect, GAME* game, void* ct_arg);
static void eNamida_mv(eEC_Effect_c* effect, GAME* game);
static void eNamida_dw(eEC_Effect_c* effect, GAME* game);

eEC_PROFILE_c iam_ef_namida = {
    // clang-format off
    &eNamida_init,
    &eNamida_ct,
    &eNamida_mv,
    &eNamida_dw,
    eEC_IGNORE_DEATH,
    eEC_NO_CHILD_ID,
    eEC_DEFAULT_DEATH_DIST,
    // clang-format on
};

static void eNamida_init(xyz_t pos, int prio, s16 angle, GAME* game, u16 item_name, s16 arg0, s16 arg1) {
    // TODO
}

static void eNamida_ct(eEC_Effect_c* effect, GAME* game, void* ct_arg) {
    // TODO
}

static void eNamida_mv(eEC_Effect_c* effect, GAME* game) {
    // TODO
}

static void eNamida_dw(eEC_Effect_c* effect, GAME* game) {
    // TODO
}
