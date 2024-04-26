#ifndef SYS_MATH_H
#define SYS_MATH_H

#include "types.h"
#include "MSL_C/math.h"
#include "libc64/qrand.h"
#include "m_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SHT_MIN_S -32768 /* 0x8000 */
#define SHT_MAX_S 32767  /* 0x7FFF */

#define SHT_MIN ((f32)SHT_MIN_S)
#define SHT_MAX ((f32)SHT_MAX_S)

#define SHT_MINV (1.0f / SHT_MAX)

#define USHT_MIN_S 0
#define USHT_MAX_S 65535

#define USHT_MIN ((f32)USHT_MIN_S)
#define USHT_MAX ((f32)USHT_MAX_S)

/* Macro to generate a random float in the range of [0, n) */
#define RANDOM_F(n) (fqrand() * (f32)(n))

/* Macro to generate a random integer in the range of [0, n) */
#define RANDOM(n) ((int)RANDOM_F(n))

/* Macro to generate a random float in the range of [0, n) */
#define RANDOM2_F(n) (fqrand2() * (f32)(n))

/* Macro to generate a random integer in the range of [0, n) */
#define RANDOM2(n) ((int)RANDOM2_F(n))

extern u16 U_GetAtanTable(f32 y, f32 x);
extern s16 atans_table(f32 x, f32 y);
extern f32 atanf_table(f32 x, f32 y);
extern void init_rnd();
extern f32 sinf_table(f32 x);
extern f32 cosf_table(f32 x);

#ifdef __cplusplus
}
#endif

#endif
