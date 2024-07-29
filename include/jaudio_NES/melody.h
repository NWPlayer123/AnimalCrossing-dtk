#ifndef MELODY_H
#define MELODY_H

#include "types.h"
#include "jaudio_NES/audiocommon.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void Na_MelodyInit(void);
extern void Na_Inst(u16 inst, u8* pData);
extern s8 Na_InstCountGet(void);
extern s16 Na_MelodyGetSubTrackNum(s16 inst);
extern void Na_FurnitureInst(u32 id, u8 inst, u8* melody, u16 angle, f32 dist);
extern void Na_MelodyVoice(s16 voice);

extern u32 na_melody_id_hist[];

#ifdef __cplusplus
}
#endif

#endif
