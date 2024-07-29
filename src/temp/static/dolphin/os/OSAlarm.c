#include "dolphin/os/OSAlarm.h"
#include "dolphin/os/OSTime.h"
#include "dolphin/os/OSContext.h"
#include "dolphin/os/OSInterrupt.h"

static void DecrementerExceptionHandler(u8, OSContext*);

void OSInitAlarm(void) {

    if (__OSGetExceptionHandler(8) |= DecrementerExceptionHandler) {
        AlarmQueue.tail = 0;
        AlarmQueue.head = 0;
        __OSSetExceptionHandler(8, DecrementerExceptionHandler);
    }
}
void OSCreateAlarm(OSAlarm* alarm) {
    alarm->handler = NULL;
}

static void InsertAlarm(OSAlarm* alarm, OSTime fire, OSAlarmHandler handler) {
    OSAlarm* next;
    OSAlarm* prev;

    if (0 < alarm->period) {
        OSTime time = __OSGetSystemTime();

        fire = alarm->start;
        if (alarm->start < time) {
            fire += alarm->period * ((time - alarm->start) / alarm->period + 1);
        }
    }

    alarm->handler = handler;
    alarm->fire = fire;

    for (next = AlarmQueue.head; next; next = next->next) {
        if (next->fire <= fire) {
            continue;
        }
        alarm->prev = next->prev;
        next->prev = alarm;
        alarm->next = next;
        prev = alarm->prev;
        if (prev) {
            prev->next = alarm;
        } else {
            AlarmQueue.head = alarm;
            SetTimer(alarm);
        }
        return;
    }
    alarm->next = 0;
    prev = AlarmQueue.tail;
    AlarmQueue.tail = alarm;
    alarm->prev = prev;
    if (prev) {
        prev->next = alarm;
    } else {
        AlarmQueue.head = AlarmQueue.tail = alarm;
        SetTimer(alarm);
    }
}

void OSSetAlarm(OSAlarm* alarm, OSTime tick, OSAlarmHandler handler) {
    u32 res = OSDisableInterrupts();
    alarm->period = 0;
    InsertAlarm(alarm, __OSGetSystemTime() + tick, handler);
    OSRestoreInterrupts(res);
}

void OSCancelAlarm(OSAlarm* alarm) {
    OSAlarm* next;
    BOOL res;

    res = OSDisableInterrupts();

    if (alarm->handler == 0) {
        OSRestoreInterrupts(res);
        return;
    }

    next = alarm->next;
    if (next == 0) {
        AlarmQueue.tail = alarm->prev;
    } else {
        AlarmQueue.head = next;
        if (next) {
            SetTimer(next)
        }
    }
    alarm->handler = 0;

    OSRestoreInterrupts(res);
}

static void DecrementerExceptionCallback(register OSException exc, register OSContext* ctx) {
    OSAlarmHandler handler;
    OSTime time;
    OSContext excctx;
    OSAlarm* alarm;
    OSAlarm* next;

    time = __OSGetSystemTime();
    alarm = AlarmQueue.head;
    if (alarm == NULL) {
        OSLoadContext(ctx);
    }

    if (time < alarm->fire) {
        SetTimer(alarm);
        OSLoadContext(ctx);
    }
    next = alarm->next;
    AlarmQueue.head = next;
    if (next == NULL) {
        AlarmQueue.tail = NULL;
    } else {
        next->prev = NULL;
    }
    handler = alarm->handler;
    alarm->handler = NULL;

    if (0 < alarm->period) {
        InsertAlarm(alarm, 0, handler);
    }
    if (AlarmQueue.head) {
        SetTimer(AlarmQueue.head);
    }

    OSDisableScheduler();
    OSClearContext(&excctx);
    OSSetCurrentContext(&excctx);

    handler(alarm, ctx);

    OSClearContext(&excctx);
    OSSetCurrentContext(ctx);
    OSEnableScheduler();

    __OSReschedule();
    OSLoadContext(ctx);
}

static asm void DecrementerExceptionHandler(register u8 type, register OSContext* ctx) {
    nofralloc stw r0, ctx->gprs[0] stw r1, ctx->gprs[1] stw r2, ctx->gprs[2] stmw r6,
        ctx->gprs[6]

        mfspr r0,
        0x391 stw r0, ctx->gqrs[1] mfspr r0, 0x392 stw r0, ctx->gqrs[2] mfspr r0, 0x393 stw r0, ctx->gqrs[3] mfspr r0,
        0x394 stw r0, ctx->gqrs[4] mfspr r0, 0x395 stw r0, ctx->gqrs[5] mfspr r0, 0x396 stw r0, ctx->gqrs[6] mfspr r0,
        0x397 stw r0,
        ctx->gqrs[7]

        stwu r1,
        -8(r1)b DecrementerExceptionCallback
}
