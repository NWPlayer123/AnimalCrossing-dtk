#ifndef XRINTF_H
#define XRINTF_H

#include "types.h"
#include "va_args.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    /* 0x0 */ union {
        /* 0x0 */ s64 ll;
        /* 0x0 */ f64 ld;
    } v;
    /* 0x8 */ char* s;
    /* 0xC */ s32 n0;
    /* 0x10 */ s32 nz0;
    /* 0x14 */ s32 n1;
    /* 0x18 */ s32 nz1;
    /* 0x1C */ s32 n2;
    /* 0x20 */ s32 nz2;
    /* 0x24 */ s32 prec;
    /* 0x28 */ s32 width;
    /* 0x2C */ u32 nchar;
    /* 0x30 */ u32 flags;
    /* 0x34 */ u8 qual;
} _Pft; // size = 0x38

typedef void* (*PrintCallback)(void*, const char*, int);

#define FLAGS_SPACE 1
#define FLAGS_PLUS 2
#define FLAGS_MINUS 4
#define FLAGS_HASH 8
#define FLAGS_ZERO 16

static void _Litob(_Pft* args, u8 type);
static void _Ldtob(_Pft* args, u8 type);
extern int _Printf(void* (*prout_func)(void*, const char*, int), void* arg, const char* fmt, va_list ap);

#ifdef __cplusplus
}
#endif
#endif
