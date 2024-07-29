#ifndef TRK_MSGBUF
#define TRK_MSGBUF
#include "types.h"
#include "TRK/trk.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct MessageBuffer {
    int unk0;
    int unk4;
    int unk8;
    u32 unkC;
    u8 mBuffer[0x87C];
} MessageBuffer;

MessageBuffer* TRKGetBuffer(int);
void TRKReleaseBuffer(int);
TRKResult TRKInitializeMessageBuffers();
void TRKSetBufferPosition(MessageBuffer*, u32);
void TRKReadBuffer1_ui8(MessageBuffer*, u8*);

#ifdef __cplusplus
}
#endif

#endif
