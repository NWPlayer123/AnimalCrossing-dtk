#include "libc/math.h"
#include "MSL_C/w_math.h"

f32 fatan2(f32 x, f32 y) {
    return atan2(x, y);
}

f64 fsqrt(f32 x) {
    return sqrtf(x);
}

f32 facos(f32 x) {
    return acos(x);
}
