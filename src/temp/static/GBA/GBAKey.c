#include "GBA/GBAPriv.h"

static volatile u8 D35[] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   33,  2,   255, 0,   33,  19,  6,   18,  3,   18,  4,   19,  5,   0,   146, 0,   255, 0,   136,
    255, 255, 0,   137, 255, 255, 0,   138, 255, 255, 0,   139, 255, 255, 143, 0,   139, 0,   140, 0,   2,   191, 128,
    126, 22,  252, 220, 209, 22,  253, 0,   0,   22,  251, 0,   1,   2,   191, 128, 120, 36,  255, 2,   128, 171, 186,
    2,   148, 0,   41,  142, 0,   2,   191, 128, 120, 32,  255, 2,   64,  15,  255, 31,  94,  0,   155, 0,   0,   0,
    153, 0,   32,  0,   135, 0,   0,   0,   128, 0,   65,  2,   159, 128, 188, 2,   191, 0,   140, 2,   191, 128, 126,
    22,  252, 220, 209, 22,  253, 0,   3,   22,  251, 0,   1,   143, 0,   2,   191, 128, 120, 2,   128, 205, 209, 2,
    148, 0,   76,  38,  255, 2,   128, 0,   1,   2,   149, 0,   94,  2,   128, 0,   2,   2,   149, 128, 0,   2,   159,
    0,   76,  0,   33,  142, 0,   2,   191, 128, 120, 36,  255, 2,   191, 128, 120, 36,  255, 2,   191, 128, 120, 36,
    255, 2,   191, 128, 120, 0,   197, 255, 255, 2,   64,  15,  255, 28,  158, 2,   191, 128, 120, 0,   199, 255, 255,
    2,   191, 128, 120, 0,   198, 255, 255, 2,   191, 128, 120, 0,   192, 255, 255, 2,   191, 128, 120, 32,  255, 2,
    64,  15,  255, 31,  94,  2,   191, 128, 120, 33,  255, 2,   191, 128, 120, 35,  255, 18,  5,   18,  6,   2,   159,
    128, 181, 0,   33,  129, 0,   0,   129, 0,   16,  16,  32,  27,  62,  0,   223, 20,  86,  3,   64,  255, 208, 132,
    23,  0,   128, 0,   0,   0,   134, 0,   0,   0,   130, 0,   31,  0,   222, 21,  246, 20,  8,   0,   223, 23,  102,
    3,   64,  0,   255, 31,  95,  2,   191, 136, 229, 31,  28,  129, 30,  25,  30,  20,  120, 31,  252, 31,  94,  2,
    191, 136, 9,   2,   191, 135, 35,  0,   6,   129, 6,   0,   222, 22,  108, 20,  4,   2,   64,  255, 0,   0,   223,
    18,  49,  21,  120, 3,   64,  0,   255, 31,  95,  2,   191, 136, 229, 31,  28,  129, 30,  25,  30,  20,  120, 31,
    252, 31,  94,  2,   191, 136, 9,   2,   191, 135, 35,  129, 0,   137, 0,   0,   209, 0,   5,   153, 0,   130, 0,
    2,   149, 0,   229, 2,   145, 0,   243, 0,   130, 0,   16,  0,   134, 0,   1,   0,   208, 23,  27,  145, 0,   125,
    0,   77,  0,   21,  1,   31,  95,  0,   223, 0,   3,   21,  4,   2,   191, 136, 9,   2,   159, 1,   2,   0,   130,
    0,   17,  0,   223, 0,   3,   21,  1,   31,  95,  0,   222, 16,  67,  2,   64,  255, 240, 2,   191, 136, 229, 2,
    159, 1,   2,   0,   130, 0,   16,  0,   134, 0,   1,   0,   208, 18,  133, 145, 0,   77,  0,   21,  1,   0,   222,
    0,   3,   20,  4,   31,  94,  2,   191, 136, 9,   0,   131, 0,   19,  27,  126, 137, 35,  0,   131, 0,   19,  0,
    223, 0,   7,   0,   222, 17,  184, 2,   64,  255, 240, 31,  94,  2,   191, 129, 244, 241, 0,   2,   191, 132, 88,
    143, 0,   0,   130, 0,   21,  0,   222, 0,   6,   0,   218, 22,  91,  2,   191, 136, 229, 20,  253, 20,  3,   27,
    94,  27,  92,  0,   130, 0,   22,  0,   222, 23,  35,  20,  244, 0,   218, 22,  107, 2,   191, 136, 229, 177, 0,
    2,   144, 1,   46,  129, 0,   20,  253, 142, 0,   0,   223, 20,  145, 3,   64,  208, 240, 28,  191, 0,   223, 20,
    104, 0,   209, 17,  252, 21,  124, 28,  223, 0,   209, 17,  184, 153, 0,   20,  24,  20,  120, 31,  94,  31,  254,
    31,  101, 54,  0,   20,  2,   31,  102, 55,  0,   21,  1,   76,  0,   21,  24,  153, 0,   53,  0,   76,  0,   0,
    223, 0,   18,  63,  0,   0,   255, 0,   18,  20,  112, 0,   223, 0,   17,  63,  0,   0,   255, 0,   17,  31,  165,
    21,  1,   31,  230, 241, 0,   21,  248, 245, 0,   31,  95,  31,  125, 129, 0,   0,   222, 0,   17,  52,  0,   137,
    0,   0,   223, 0,   18,  53,  0,   76,  0,   0,   223, 0,   18,  21,  120, 76,  0,   137, 0,   31,  254, 21,  8,
    59,  0,   0,   222, 0,   17,  62,  0,   0,   223, 0,   18,  59,  0,   28,  191, 0,   218, 21,  241, 53,  0,   2,
    149, 1,   146, 0,   223, 16,  226, 21,  8,   31,  95,  0,   223, 16,  59,  121, 0,   57,  0,   48,  128, 0,   254,
    0,   34,  0,   220, 18,  41,  0,   221, 17,  248, 92,  0,   240, 0,   31,  229, 48,  128, 2,   159, 1,   165, 0,
    223, 16,  202, 21,  8,   31,  95,  0,   223, 16,  67,  117, 0,   57,  0,   48,  128, 0,   254, 0,   34,  0,   220,
    18,  89,  0,   221, 22,  254, 76,  0,   240, 0,   31,  229, 48,  128, 0,   254, 0,   35,  0,   218, 0,   8,   0,
    216, 0,   9,   0,   155, 0,   32,  0,   153, 0,   8,   0,   135, 0,   0,   2,   191, 128, 139, 2,   223, 0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

static s32 F152(DSPTaskInfo* task) {
    s32 chan;        // r31
    GBAControl* gba; // r29

    for (chan = 0; chan < 4; ++chan) {
        gba = &__GBA[chan];
        if (&gba->task == task) {
            return chan;
        }
    }

    ASSERT(0 <= chan && chan < 4);
    OSPanic(__FILE__, 169, "GBA - unexpected dsp call");
    return -1;
}

static void F23(void* task) {
    s32 chan;        // r31
    GBAControl* gba; // r30
    chan = F152(task);
    gba = &__GBA[chan];
    DSPSendMailToDSP(0xabba0000);
    while (DSPCheckMailToDSP()) {}
    DSPSendMailToDSP((u32)gba->param);
    while (DSPCheckMailToDSP()) {}
}

void F25(void* task) {
    s32 chan; // r31
    chan = F152(task);
    __GBAX01(chan, 0);
}

void __GBAX02(s32 chan, u8* readbuf) {
    GBAControl* gba;       // r28
    GBABootInfo* bootInfo; // r29
    GBASecParam* param;    // r30
    DSPTaskInfo* task;     // r31

    gba = &__GBA[chan];
    bootInfo = &__GBA[chan].bootInfo;
    param = gba->param;
    memcpy(param, readbuf, 4);
    param->paletteColor = bootInfo->paletteColor;
    param->paletteSpeed = bootInfo->paletteSpeed;
    param->length = bootInfo->length;
    param->out = &param->keyA;
    DCInvalidateRange(&param->keyA, 32);
    DCFlushRange(param, 32);
    task = &gba->task;
    task->priority = 255;
    task->iram_mmem_addr = (u16*)OSCachedToPhysical(D35);
    task->iram_length = sizeof(D35);
    task->iram_addr = 0;
    task->dsp_init_vector = 16;
    task->init_cb = F23;
    task->res_cb = NULL;
    task->done_cb = F25;
    task->req_cb = NULL;
    DSPAddTask(task);
}
