#include "libforest/emu64/emu64.hpp"

#include "boot.h"
#include "terminal.h"
#include "MSL_C/w_math.h"

u32 emu64::seg2k0(u32 segadr) {
    u32 k0;

    if ((segadr >> 28) == 0) {
        if (segadr < 0x03000000) {
            this->Printf0(VT_COL(RED, WHITE) "segadr=%08x" VT_RST "\n", segadr);
            this->panic("segadr is over 0x03000000.", __FILE__, 20);
            k0 = segadr + 0x80000000;
        } else {
            k0 = (u32)this->segments[(segadr >> 24) & 0xF] + (segadr & 0xFFFFFF);
        }
        this->resolved_addresses++;
    } else {
        k0 = segadr;
    }

    if ((k0 >> 31) == 0 || k0 < 0x80000000 || k0 >= 0x83000000) {
        this->Printf0("異常なアドレスです。%08x -> %08x\n", segadr, k0);
        this->panic("異常なアドレスです。", __FILE__, 77);
        this->abnormal_addresses++;
    }

    return k0;
}

/* @unused void guMtxXFMWF(MtxP, float, float, float, float, float, float*, float*, float*, float*) */

/* @unused void guMtxXFM1F(MtxP, float, float, float, float, float*, float*, float*) */

void guMtxXFM1F_dol(MtxP mtx, float x, float y, float z, float* ox, float* oy, float* oz) {
    *ox = mtx[0][0] * x + mtx[0][1] * y + mtx[0][2] * z + mtx[0][3];
    *oy = mtx[1][0] * x + mtx[1][1] * y + mtx[1][2] * z + mtx[1][3];
    *oz = mtx[2][0] * x + mtx[2][1] * y + mtx[2][2] * z + mtx[2][3];
}

void guMtxXFM1F_dol7(MtxP mtx, float x, float y, float z, float* ox, float* oy, float* oz) {
    GC_Mtx inv;

    PSMTXInverse(mtx, inv);
    *ox = inv[0][0] * x + inv[0][1] * y + inv[0][2] * z + inv[0][3];
    *oy = inv[1][0] * x + inv[1][1] * y + inv[1][2] * z + inv[1][3];
    *oz = inv[2][0] * x + inv[2][1] * y + inv[2][2] * z + inv[2][3];
}

void guMtxXFM1F_dol2(MtxP mtx, GXProjectionType type, float x, float y, float z, float* ox, float* oy, float* oz) {
    if (type == GX_PERSPECTIVE) {
        f32 s = -1.0f / z;

        *ox = mtx[0][0] * x * s - mtx[0][2];
        *oy = mtx[1][1] * y * s - mtx[1][2];
        *oz = mtx[2][3] * s - mtx[2][2];
    } else {
        *ox = mtx[0][0] * x + mtx[0][3];
        *oy = mtx[1][1] * y + mtx[1][3];
        *oz = mtx[2][2] * z + mtx[2][3];
    }
}

void guMtxXFM1F_dol2w(MtxP mtx, GXProjectionType type, float x, float y, float z, float* ox, float* oy, float* oz,
                      float* ow) {
    if (type == GX_PERSPECTIVE) {
        *ox = mtx[0][0] * x + mtx[0][2] * z;
        *oy = mtx[1][1] * y + mtx[1][2] * z;
        *oz = mtx[2][3] + mtx[2][2] * z;
        *ow = -z;
    } else {
        *ox = mtx[0][0] * x + mtx[0][3];
        *oy = mtx[1][1] * y + mtx[1][3];
        *oz = mtx[2][2] * z + mtx[2][3];
        *ow = 1.0f;
    }
}

float guMtxXFM1F_dol3(MtxP mtx, GXProjectionType type, float z) {
    if (type == GX_PERSPECTIVE) {
        return -mtx[2][3] / (z + mtx[2][2]);
    } else {
        return (z - mtx[2][3]) / mtx[2][2];
    }
}

void guMtxXFM1F_dol6w(MtxP mtx, GXProjectionType type, float x, float y, float z, float w, float* ox, float* oy,
                      float* oz, float* ow) {
    if (type == GX_PERSPECTIVE) {
        float xScale = mtx[0][0];
        float yScale = mtx[1][1];
        float zScale = mtx[2][2];

        float xRatioScaling = mtx[0][2];
        float yRatioScaling = mtx[1][2];
        float zSkew = mtx[2][3];

        *ox = (yScale * zSkew * (x + xRatioScaling * w)) / (xScale * (yScale * zSkew));
        *oy = (xScale * zSkew * (y + yRatioScaling * w)) / (xScale * (yScale * zSkew));
        *oz = -w;
        *ow = (xScale * yScale * (z + zScale * w)) / (xScale * (yScale * zSkew));
    } else {
        float xScale = mtx[0][0];
        float xSkew = mtx[0][3];

        float yScale = mtx[1][1];
        float ySkew = mtx[1][3];

        float zScale = mtx[2][2];
        float zSkew = mtx[2][3];

        float n = 1.0f / (xScale * yScale * zScale);

        *ox = n * (yScale * zScale * (x - xSkew));
        *oy = n * (zScale * xScale * (y - ySkew));
        *oz = n * (xScale * yScale * (z - zSkew));
        *ow = 1.0f;
    }
}

void guMtxXFM1F_dol6w1(MtxP mtx, GXProjectionType type, float x, float y, float z, float w, float* ox, float* oy,
                       float* oz) {
    if (type == GX_PERSPECTIVE) {
        float xScale = mtx[0][0];
        float yScale = mtx[1][1];
        float zScale = mtx[2][2];

        float xRatioScaling = mtx[0][2];
        float yRatioScaling = mtx[1][2];
        float zSkew = mtx[2][3];

        float temp_f7 = 1.0f / (xScale * yScale * (z + (zScale * w)));

        *ox = temp_f7 * (yScale * zSkew * (x + (xRatioScaling * w)));
        *oy = temp_f7 * (xScale * zSkew * (y + (yRatioScaling * w)));
        *oz = temp_f7 * (yScale * zSkew * xScale * -w);
    } else {
        float translateX = mtx[0][3];
        float translateY = mtx[1][3];
        float translateZ = mtx[2][3];

        float scaleX = mtx[0][0];
        float scaleY = mtx[1][1];
        float scaleZ = mtx[2][2];

        *ox = (x - translateX) / scaleX;
        *oy = (y - translateY) / scaleY;
        *oz = (z - translateZ) / scaleZ;
    }
}

/* @unused void guMtxXFMWL(N64Mtx*, float, float, float, float, float*, float*, float*, float*) */

void guMtxNormalize(GC_Mtx mtx) {
    for (int i = 0; i < 3; i++) {
        float magnitude = sqrtf(mtx[i][0] * mtx[i][0] + mtx[i][1] * mtx[i][1] + mtx[i][2] * mtx[i][2]);

        mtx[i][0] *= 1.0f / magnitude;
        mtx[i][1] *= 1.0f / magnitude;
        mtx[i][2] *= 1.0f / magnitude;
    }
}

/* TODO: Mtx -> N64Mtx, GC_Mtx -> Mtx */
void N64Mtx_to_DOLMtx(const Mtx* n64, MtxP gc) {
    s16* fixed = ((s16*)n64) + 0;
    u16* frac = ((u16*)n64) + 16;
    int i;

    for (i = 0; i < 4; i++) {
        gc[0][i] = fastcast_float(&fixed[0]) + fastcast_float(&frac[0]) * (1.0f / 65536.0f);
        gc[1][i] = fastcast_float(&fixed[1]) + fastcast_float(&frac[1]) * (1.0f / 65536.0f);
        gc[2][i] = fastcast_float(&fixed[2]) + fastcast_float(&frac[2]) * (1.0f / 65536.0f);

        fixed += 4;
        frac += 4;
    }
}

/* @unused my_guMtxL2F(MtxP, const N64Mtx*) */
