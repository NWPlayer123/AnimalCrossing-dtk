#ifndef LIB_C_MATH_H
#define LIB_C_MATH_H

#include "types.h"

#define SQRT_OF_2_F 1.41421356237309504880f
#define SQRT_OF_3_F 1.73205080756887729353f

#define SQRT_3_OVER_3_F (SQRT_OF_3_F / 3.0f)

#define M_PI 3.14159265358979323846f

s16 sins(u16);
s16 coss(u16);

f32 fatan2(f32, f32);

f64 fsqrt(f32);

f32 facos(f32);
#endif
