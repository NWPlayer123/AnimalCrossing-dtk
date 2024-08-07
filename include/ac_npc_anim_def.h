#ifndef AC_NPC_ANIM_DEF_H
#define AC_NPC_ANIM_DEF_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {
    aNPC_ANIM_RUN1,
    aNPC_ANIM_WALK1,
    aNPC_ANIM_WALK_KI1,
    aNPC_ANIM_WALK_DO1,
    aNPC_ANIM_WALK_AI1,
    aNPC_ANIM_WAIT1,
    aNPC_ANIM_WAIT_KI1,
    aNPC_ANIM_WAIT_DO1,
    aNPC_ANIM_WAIT_AI1,
    aNPC_ANIM_WAIT_NEMU1,
    aNPC_ANIM_AISATU1,
    aNPC_ANIM_AISATU2,
    aNPC_ANIM_AISATU3,
    aNPC_ANIM_AISATU4,
    aNPC_ANIM_GURATUKU1,
    aNPC_ANIM_OTIRU1,
    aNPC_ANIM_OTIRU2,
    aNPC_ANIM_MOGAKU1,
    aNPC_ANIM_DERU1,
    aNPC_ANIM_DERU2,
    aNPC_ANIM_TALK_TURN1,
    aNPC_ANIM_TALK1,
    aNPC_ANIM_OPEN1,
    aNPC_ANIM_GO_OUT1,
    aNPC_ANIM_UMB_OPEN1,
    aNPC_ANIM_UMB_CLOSE1,
    aNPC_ANIM_TRANSFER1,
    aNPC_ANIM_TRANS_WAIT1,
    aNPC_ANIM_GET1,
    aNPC_ANIM_GET_PULL1,
    aNPC_ANIM_GET_PULL_WAIT1,
    aNPC_ANIM_GET_PUTAWAY1,
    aNPC_ANIM_GET_EAT1,
    aNPC_ANIM_GET_CHANGE1,
    aNPC_ANIM_GET_RETURN1,
    aNPC_ANIM_TRANSFER_F1,
    aNPC_ANIM_TRANS_WAIT_F1,
    aNPC_ANIM_GET_F1,
    aNPC_ANIM_GET_PULL_F1,
    aNPC_ANIM_GET_PULL_WAIT_F1,
    aNPC_ANIM_GET_PUTAWAY_F1,
    aNPC_ANIM_GET_RETURN_F1,
    aNPC_ANIM_ESTIMATE_F1,
    aNPC_ANIM_CONTRACT1,
    aNPC_ANIM_CONTRACT2,
    aNPC_ANIM_CONTRACT3,
    aNPC_ANIM_NOT_CONTRACT1,
    aNPC_ANIM_FLY1,
    aNPC_ANIM_FLYAWAY1,
    aNPC_ANIM_DELIVERY1,
    aNPC_ANIM_KYORO1,
    aNPC_ANIM_LANDING1,
    aNPC_ANIM_LANDING2,
    aNPC_ANIM_LANDING3,
    aNPC_ANIM_OPEN_D1,
    aNPC_ANIM_SITDOWN_D1,
    aNPC_ANIM_SITDOWN_WAIT_D1,
    aNPC_ANIM_STANDUP_D1,
    aNPC_ANIM_OPEN_D2,
    aNPC_ANIM_TO_DECK_D1,
    aNPC_ANIM_KEITAI_ON1,
    aNPC_ANIM_KEITAI_TALK1,
    aNPC_ANIM_KEITAI_TALK2,
    aNPC_ANIM_KEITAI_OFF1,
    aNPC_ANIM_WAIT_E1,
    aNPC_ANIM_ENSOU_E1,
    aNPC_ANIM_BANZAI1,
    aNPC_ANIM_CLAP1,
    aNPC_ANIM_KOKKURI_D1,
    aNPC_ANIM_KOKKURI_D2,
    aNPC_ANIM_SAISEN1,
    aNPC_ANIM_OMAIRI1,
    aNPC_ANIM_SITDOWN_WAIT1,
    aNPC_ANIM_SITDOWN_DRINK1,
    aNPC_ANIM_SITDOWN_CLAP1,
    aNPC_ANIM_DANCE1,
    aNPC_ANIM_KUISINBO1,
    aNPC_ANIM_SANBASI1,
    aNPC_ANIM_WAIT_R1,
    aNPC_ANIM_APPEAR1,
    aNPC_ANIM_GO_UG1,
    aNPC_ANIM_INTRO1_A,
    aNPC_ANIM_INTRO1_B,
    aNPC_ANIM_TAISOU1,
    aNPC_ANIM_TAISOU2,
    aNPC_ANIM_TAISOU3_A,
    aNPC_ANIM_TAISOU3_B,
    aNPC_ANIM_TAISOU4_A,
    aNPC_ANIM_TAISOU4_B,
    aNPC_ANIM_TAISOU5_A,
    aNPC_ANIM_TAISOU5_B,
    aNPC_ANIM_TAISOU6_A,
    aNPC_ANIM_TAISOU6_B,
    aNPC_ANIM_TAISOU7,
    aNPC_ANIM_KIDUKU1,
    aNPC_ANIM_KIDUKU2,
    aNPC_ANIM_CRACKER_WAIT1,
    aNPC_ANIM_CRACKER_FIRE1,
    aNPC_ANIM_CRACKER_RUN1,
    aNPC_ANIM_CRACKER_COUNT1,
    aNPC_ANIM_WARMUP1,
    aNPC_ANIM_READY1,
    aNPC_ANIM_ASIHUMI1,
    aNPC_ANIM_KOKERU1,
    aNPC_ANIM_KOKERU_GETUP1,
    aNPC_ANIM_TIRED1,
    aNPC_ANIM_TAMAKOME1,
    aNPC_ANIM_YOUI1,
    aNPC_ANIM_DON1,
    aNPC_ANIM_TUNAHIKI_AIKO1,
    aNPC_ANIM_TUNAHIKI_YURI1,
    aNPC_ANIM_TUNAHIKI_FURI1,
    aNPC_ANIM_HATAFURI1,
    aNPC_ANIM_TAMAHIROI1,
    aNPC_ANIM_TAMANAGE1,
    aNPC_ANIM_GETUP_SEG1,
    aNPC_ANIM_GETUP_WAIT_SEG1,
    aNPC_ANIM_PIKU_SEG1,
    aNPC_ANIM_4HAKU_E1,
    aNPC_ANIM_3HAKU_E1,
    aNPC_ANIM_SEND_MAIL1,
    aNPC_ANIM_TURI_WAIT1,
    aNPC_ANIM_MISIN1,
    aNPC_ANIM_WIPKOGU1,
    aNPC_ANIM_WIPWAIT1,
    aNPC_ANIM_WIPWAIT2,
    aNPC_ANIM_GSTWAIT1,
    aNPC_ANIM_GYAFUN1,
    aNPC_ANIM_GYAFUN2,
    aNPC_ANIM_TKYKYORO1,
    aNPC_ANIM_MUKA1,
    aNPC_ANIM_MUKA2,
    aNPC_ANIM_GAAAN1,
    aNPC_ANIM_GAAAN2,
    aNPC_ANIM_SMILE1,
    aNPC_ANIM_SMILE2,
    aNPC_ANIM_HA1,
    aNPC_ANIM_HA2,
    aNPC_ANIM_PUNPUN1,
    aNPC_ANIM_PUNPUN2,
    aNPC_ANIM_A1,
    aNPC_ANIM_A2,
    aNPC_ANIM_ASERU1,
    aNPC_ANIM_ASERU2,
    aNPC_ANIM_BURUBURU1,
    aNPC_ANIM_BURUBURU2,
    aNPC_ANIM_GOUKYU1,
    aNPC_ANIM_GOUKYU2,
    aNPC_ANIM_HAPPY1,
    aNPC_ANIM_HAPPY2,
    aNPC_ANIM_HATE1,
    aNPC_ANIM_HATE2,
    aNPC_ANIM_HIRAMEKI1,
    aNPC_ANIM_HIRAMEKI2,
    aNPC_ANIM_HYUUU1,
    aNPC_ANIM_HYUUU2,
    aNPC_ANIM_LOVELOVE1,
    aNPC_ANIM_LOVELOVE2,
    aNPC_ANIM_MUUUUU1,
    aNPC_ANIM_MUUUUU2,
    aNPC_ANIM_OTIKOMU1,
    aNPC_ANIM_OTIKOMU2,
    aNPC_ANIM_SHITUREN1,
    aNPC_ANIM_SHITUREN2,
    aNPC_ANIM_WARUDAKUMI1,
    aNPC_ANIM_WARUDAKUMI2,
    aNPC_ANIM_NEBOKE1,
    aNPC_ANIM_NEBOKE2,
    aNPC_ANIM_LOVE1,
    aNPC_ANIM_LOVE2,
    aNPC_ANIM_NIKO1,
    aNPC_ANIM_MUSU1,
    aNPC_ANIM_KOMARI1,
    aNPC_ANIM_HATE_F1,
    aNPC_ANIM_HATE_F2,
    aNPC_ANIM_HA_F1,
    aNPC_ANIM_HA_F2,
    aNPC_ANIM_HAPPY_F1,
    aNPC_ANIM_HAPPY_F2,
    aNPC_ANIM_ASERU_F1,
    aNPC_ANIM_ASERU_F2,
    aNPC_ANIM_MUUUUU_F1,
    aNPC_ANIM_MUUUUU_F2,
    aNPC_ANIM_LOVELOVE_F1,
    aNPC_ANIM_LOVELOVE_F2,
    aNPC_ANIM_A_F1,
    aNPC_ANIM_A_F2,
    aNPC_ANIM_SMILE_F1,
    aNPC_ANIM_SMILE_F2,
    aNPC_ANIM_KOMARI_F1,
    aNPC_ANIM_NIKO_F1,
    aNPC_ANIM_MUSU_F1,
    aNPC_ANIM_HATE_I1,
    aNPC_ANIM_HATE_I2,
    aNPC_ANIM_HA_I1,
    aNPC_ANIM_HA_I2,
    aNPC_ANIM_HAPPY_I1,
    aNPC_ANIM_HAPPY_I2,
    aNPC_ANIM_ASERU_I1,
    aNPC_ANIM_ASERU_I2,
    aNPC_ANIM_MUUUUU_I1,
    aNPC_ANIM_MUUUUU_I2,
    aNPC_ANIM_LOVE_I1,
    aNPC_ANIM_LOVE_I2,
    aNPC_ANIM_SHITUREN_I1,
    aNPC_ANIM_SHITUREN_I2,
    aNPC_ANIM_KOMARI_I1,
    aNPC_ANIM_NIKO_I1,
    aNPC_ANIM_MUSU_I1,
    aNPC_ANIM_SMILE_D1,
    aNPC_ANIM_SMILE_D2,
    aNPC_ANIM_GAAAN_D1,
    aNPC_ANIM_GAAAN_D2,
    aNPC_ANIM_HIRAMEKI_D1,
    aNPC_ANIM_HIRAMEKI_D2,
    aNPC_ANIM_HA_D1,
    aNPC_ANIM_HA_D2,
    aNPC_ANIM_MUSU_D1,
    aNPC_ANIM_NIKO_D1,
    aNPC_ANIM_KOMARI_D1,
    aNPC_ANIM_HATE_D1,
    aNPC_ANIM_HATE_D2,
    aNPC_ANIM_PUNPUN_R1,
    aNPC_ANIM_PUNPUN_R2,
    aNPC_ANIM_MUSU_R1,
    aNPC_ANIM_HYUUU_R1,
    aNPC_ANIM_HYUUU_R2,
    aNPC_ANIM_A_R1,
    aNPC_ANIM_A_R2,
    aNPC_ANIM_A2_R1,
    aNPC_ANIM_AKIRERU_R1,
    aNPC_ANIM_AKIRERU_R2,
    aNPC_ANIM_MATAROU_R1,
    aNPC_ANIM_MATAROU_R2,
    aNPC_ANIM_GEKIDO_R1,
    aNPC_ANIM_GEKIDO_R2,
    aNPC_ANIM_HA_E1,
    aNPC_ANIM_HA_E2,
    aNPC_ANIM_KEIREI1,
    aNPC_ANIM_KIEEEEI1,
    aNPC_ANIM_KIEEEEI2,
    aNPC_ANIM_UMBRELLA1,
    aNPC_ANIM_UTIWA_WAIT1,
    aNPC_ANIM_TUE1,
    aNPC_ANIM_KUTIPAKU1,
    aNPC_ANIM_KUTIPAKU2,

    aNPC_ANIM_NUM
};

#ifdef __cplusplus
}
#endif

#endif
