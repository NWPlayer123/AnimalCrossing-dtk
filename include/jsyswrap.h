#ifndef JSYSWRAP_H
#define JSYSWRAP_H

#include "types.h"
#include "libjsys/jsyswrapper.h"
#include "dolphin/gx.h"
#include "dolphin/dvd.h"
#include "dolphin/pad.h"

#ifdef __cplusplus
extern "C" {
#endif

enum resource_index {
  RESOURCE_FGDATA,
  RESOURCE_MAIL,
  RESOURCE_MAIL_TABLE,
  RESOURCE_MAILA,
  RESOURCE_MAILA_TABLE,
  RESOURCE_MAILB,
  RESOURCE_MAILB_TABLE,
  RESOURCE_MAILC,
  RESOURCE_MAILC_TABLE,
  RESOURCE_PALLET_BOY,
  RESOURCE_PS,
  RESOURCE_PS_TABLE,
  RESOURCE_PSZ,
  RESOURCE_PSZ_TABLE,
  RESOURCE_SELECT,
  RESOURCE_SELECT_TABLE,
  RESOURCE_STRING,
  RESOURCE_STRING_TABLE,
  RESOURCE_SUPERZ,
  RESOURCE_SUPERZ_TABLE,
  RESOURCE_SUPER,
  RESOURCE_SUPER_TABLE,
  RESOURCE_TEX_BOY,
  RESOURCE_FACE_BOY,
  RESOURCE_FGNPCDATA,
  RESOURCE_MESSAGE,
  RESOURCE_MESSAGE_TABLE,
  RESOURCE_MY_ORIGINAL,
  RESOURCE_NEEDLEWORK_JOYBOOT,
  RESOURCE_PLAYER_ROOM_FLOOR,
  RESOURCE_PLAYER_ROOM_WALL,
  RESOURCE_NPC_NAME_STR_TABLE,
  RESOURCE_D_OBJ_NPC_STOCK_SCH,
  RESOURCE_D_OBJ_NPC_STOCK_SCL,
  RESOURCE_TITLE,
  RESOURCE_MURA_SPRING,
  RESOURCE_MURA_SUMMER,
  RESOURCE_MURA_FALL,
  RESOURCE_MURA_WINTER,
  RESOURCE_ODEKAKE,
  RESOURCE_OMAKE,
  RESOURCE_EKI1,
  RESOURCE_EKI1_2,
  RESOURCE_EKI1_3,
  RESOURCE_EKI1_4,
  RESOURCE_EKI1_5,
  RESOURCE_EKI2,
  RESOURCE_EKI2_2,
  RESOURCE_EKI2_3,
  RESOURCE_EKI2_4,
  RESOURCE_EKI2_5,
  RESOURCE_EKI3,
  RESOURCE_EKI3_2,
  RESOURCE_EKI3_3,
  RESOURCE_EKI3_4,
  RESOURCE_EKI3_5,
  RESOURCE_TEGAMI,
  RESOURCE_TEGAMI2,
  RESOURCE_FAMIKON,
  RESOURCE_BOY1,
  RESOURCE_BOY2,
  RESOURCE_BOY3,
  RESOURCE_BOY4,
  RESOURCE_BOY5,
  RESOURCE_BOY6,
  RESOURCE_BOY7,
  RESOURCE_BOY8,
  RESOURCE_GIRL11,
  RESOURCE_GIRL12,
  RESOURCE_GIRL13,
  RESOURCE_GIRL14,
  RESOURCE_GIRL15,
  RESOURCE_GIRL16,
  RESOURCE_GIRL17,
  RESOURCE_GIRL18,
  RESOURCE_D_BG_ISLAND_SCH,

  RESOURCE_NUM
};

extern void JW_UpdateVideoMode();
extern void JW_SetProgressiveMode(int enabled);
extern void JW_SetLowResoMode(int enabled);
extern void JW_SetFamicomMode(int enabled);
extern void JW_SetVideoPan(u16 origin_x, u16 origin_y, u16 width, u16 height);
extern void JW_SetLogoMode(int enabled);
extern void JW_JUTGamePad_read();
extern void JW_getPadStatus(PADStatus* padStatus);
extern int JW_JUTGamepad_getErrorStatus();
extern u32 JW_JUTGamepad_getButton();
extern u32 JW_JUTGamepad_getTrigger();
extern f32 JW_JUTGamepad_getSubStickValue();
extern int JW_JUTGamepad_getSubStickAngle();
extern void JW_BeginFrame();
extern void JW_EndFrame();
extern int JW_setClearColor(u8 r, u8 g, u8 b);
extern u32 JW_GetAramAddress(int res_no);
extern u8* _JW_GetResourceAram(u32 aram_addr, u8* dst, u32 size);
extern u32 JW_GetResSizeFileNo(int res_no);
extern void JW_Init();
extern void JW_Init2();
extern void JW_Init3();
extern void JW_Cleanup();

#ifdef __cplusplus
}
#endif

#endif
