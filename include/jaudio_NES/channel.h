#ifndef CHANNEL_H
#define CHANNEL_H

#include "types.h"
#include "jaudio_NES/audiowork.h"

extern s32 OverwriteBank(s32 type, s32 bankId, s32 idx, s32 voicetable);
extern channel* Nas_AllocationOnRequest(note* n);

extern wtstr* NoteToVoice(voicetable* voicetbl, s32 note);
extern voicetable* ProgToVp(s32 prog, s32 note);
extern percvoicetable* PercToPp(s32 perc, s32 note);
extern veffvoicetable* VpercToVep(s32 vperc, s32 note);

extern void Nas_UpdateChannel(void);

extern void Nas_DeAllocAllVoices(chnode* node);
extern void Nas_AllocVoices(chnode* node, s32 num);
extern void Nas_InitChNode(chnode* node);
extern void Nas_Release_Channel(note* n);
extern void Nas_Release_Channel_Force(note* n);

#endif
