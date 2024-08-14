#include "c_keyframe.h"
#include "libforest/gbi_extensions.h"

static Vtx wls_1_v[] = {
#include "assets/npc/mdl/wls_1_v.inc"
};

static Gfx head_wls_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*7, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&wls_1_v[10], 10, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*13, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x000, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&wls_1_v[20], 20, 10),
    gsSPNTrianglesInit_5b(13,        // tri count
                          1, 10, 11, // tri0
                          4, 12, 6,  // tri1
                          8, 13, 14  // tri2
                          ),
    gsSPNTriangles_5b(9, 15, 16, // tri0
                      7, 17, 5,  // tri1
                      0, 18, 19, // tri2
                      0, 1, 20   // tri3
                      ),
    gsSPNTriangles_5b(0, 21, 22, // tri0
                      4, 23, 24, // tri1
                      5, 25, 3,  // tri2
                      3, 26, 4   // tri3
                      ),
    gsSPNTriangles_5b(5, 27, 28, // tri0
                      1, 8, 29,  // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPVertex(&wls_1_v[40], 11, 10),
    gsSPNTrianglesInit_5b(3,        // tri count
                          9, 0, 10, // tri0
                          7, 9, 11, // tri1
                          8, 6, 12  // tri2
                          ),
    gsSPNTrianglesInit_5b(4,         // tri count
                          2, 14, 13, // tri0
                          2, 16, 15, // tri1
                          2, 18, 17  // tri2
                          ),
    gsSPNTriangles_5b(2, 20, 19, // tri0
                      0, 0, 0,   // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x080, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPVertex(&wls_1_v[51], 31, 0),
    gsSPNTrianglesInit_5b(12,      // tri count
                          0, 1, 2, // tri0
                          0, 2, 3, // tri1
                          3, 2, 4  // tri2
                          ),
    gsSPNTriangles_5b(5, 4, 2, // tri0
                      6, 0, 3, // tri1
                      6, 3, 7, // tri2
                      7, 3, 4  // tri3
                      ),
    gsSPNTriangles_5b(1, 8, 5,  // tri0
                      1, 5, 2,  // tri1
                      8, 9, 10, // tri2
                      8, 10, 5  // tri3
                      ),
    gsSPNTriangles_5b(5, 10, 4, // tri0
                      0, 0, 0,  // tri1
                      0, 0, 0,  // tri2
                      0, 0, 0   // tri3
                      ),
    gsSPNTrianglesInit_5b(12,         // tri count
                          11, 12, 13, // tri0
                          11, 13, 14, // tri1
                          15, 12, 11  // tri2
                          ),
    gsSPNTriangles_5b(12, 15, 16, // tri0
                      17, 11, 14, // tri1
                      17, 14, 18, // tri2
                      15, 11, 17  // tri3
                      ),
    gsSPNTriangles_5b(12, 16, 19, // tri0
                      12, 19, 13, // tri1
                      16, 20, 21, // tri2
                      16, 21, 19  // tri3
                      ),
    gsSPNTriangles_5b(15, 20, 16, // tri0
                      0, 0, 0,    // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x000, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPNTrianglesInit_5b(9,          // tri count
                          22, 23, 24, // tri0
                          25, 26, 27, // tri1
                          28, 29, 26  // tri2
                          ),
    gsSPNTriangles_5b(28, 26, 25, // tri0
                      26, 29, 22, // tri1
                      24, 27, 26, // tri2
                      24, 26, 22  // tri3
                      ),
    gsSPNTriangles_5b(23, 22, 30, // tri0
                      30, 22, 29, // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPVertex(&wls_1_v[82], 21, 0),
    gsSPNTrianglesInit_5b(30,      // tri count
                          0, 1, 2, // tri0
                          2, 3, 4, // tri1
                          5, 6, 7  // tri2
                          ),
    gsSPNTriangles_5b(8, 9, 10, // tri0
                      6, 11, 1, // tri1
                      0, 12, 7, // tri2
                      13, 14, 5 // tri3
                      ),
    gsSPNTriangles_5b(5, 14, 6,   // tri0
                      10, 15, 16, // tri1
                      9, 15, 10,  // tri2
                      3, 8, 4     // tri3
                      ),
    gsSPNTriangles_5b(0, 7, 1,   // tri0
                      15, 9, 17, // tri1
                      11, 6, 14, // tri2
                      9, 8, 3    // tri3
                      ),
    gsSPNTriangles_5b(2, 1, 11,   // tri0
                      16, 18, 10, // tri1
                      13, 12, 0,  // tri2
                      5, 12, 13   // tri3
                      ),
    gsSPNTriangles_5b(2, 19, 0,  // tri0
                      1, 7, 6,   // tri1
                      10, 18, 8, // tri2
                      8, 18, 4   // tri3
                      ),
    gsSPNTriangles_5b(7, 12, 5,  // tri0
                      4, 18, 16, // tri1
                      4, 19, 2,  // tri2
                      17, 3, 2   // tri3
                      ),
    gsSPNTriangles_5b(3, 17, 9,  // tri0
                      20, 2, 11, // tri1
                      17, 2, 20, // tri2
                      0, 0, 0    // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_1_txt, G_IM_FMT_CI, 32, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPVertex(&wls_1_v[103], 11, 0),
    gsSPNTrianglesInit_5b(10,      // tri count
                          0, 1, 2, // tri0
                          3, 4, 5, // tri1
                          0, 3, 6  // tri2
                          ),
    gsSPNTriangles_5b(7, 5, 4, // tri0
                      0, 6, 1, // tri1
                      8, 4, 3, // tri2
                      9, 7, 4  // tri3
                      ),
    gsSPNTriangles_5b(8, 3, 0,  // tri0
                      6, 3, 10, // tri1
                      10, 3, 5, // tri2
                      0, 0, 0   // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx mouth_wls_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x000, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&wls_1_v[0], 10, 0),
    gsSPNTrianglesInit_5b(10,      // tri count
                          0, 1, 2, // tri0
                          1, 0, 3, // tri1
                          1, 3, 4  // tri2
                          ),
    gsSPNTriangles_5b(5, 3, 6, // tri0
                      1, 7, 2, // tri1
                      2, 7, 8, // tri2
                      2, 8, 6  // tri3
                      ),
    gsSPNTriangles_5b(9, 5, 6, // tri0
                      7, 1, 4, // tri1
                      7, 4, 8, // tri2
                      0, 0, 0  // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Rarm2_wls_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*10, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&wls_1_v[149], 5, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*11, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x0C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&wls_1_v[154], 24, 5),
    gsSPNTrianglesInit_5b(8,       // tri count
                          3, 0, 5, // tri0
                          3, 6, 7, // tri1
                          1, 8, 9  // tri2
                          ),
    gsSPNTriangles_5b(1, 10, 0, // tri0
                      4, 3, 11, // tri1
                      2, 4, 12, // tri2
                      4, 13, 14 // tri3
                      ),
    gsSPNTriangles_5b(4, 15, 16, // tri0
                      0, 0, 0,   // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x100, G_IM_FMT_CI, 32, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPNTrianglesInit_5b(2,          // tri count
                          17, 18, 19, // tri0
                          17, 19, 20, // tri1
                          0, 0, 0     // tri2
                          ),
    gsSPNTrianglesInit_5b(2,          // tri count
                          21, 22, 23, // tri0
                          21, 23, 24, // tri1
                          0, 0, 0     // tri2
                          ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x0C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPNTrianglesInit_5b(2,          // tri count
                          25, 26, 27, // tri0
                          25, 27, 28, // tri1
                          0, 0, 0     // tri2
                          ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x200, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPVertex(&wls_1_v[178], 16, 0),
    gsSPNTrianglesInit_5b(8,       // tri count
                          0, 1, 2, // tri0
                          0, 2, 3, // tri1
                          4, 5, 6  // tri2
                          ),
    gsSPNTriangles_5b(4, 6, 7,   // tri0
                      8, 9, 10,  // tri1
                      8, 10, 11, // tri2
                      12, 13, 14 // tri3
                      ),
    gsSPNTriangles_5b(12, 14, 15, // tri0
                      0, 0, 0,    // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Rarm1_wls_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*7, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&wls_1_v[194], 3, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*10, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x0C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&wls_1_v[197], 11, 3),
    gsSPNTrianglesInit_5b(7,       // tri count
                          2, 0, 3, // tri0
                          0, 4, 5, // tri1
                          0, 1, 6  // tri2
                          ),
    gsSPNTriangles_5b(1, 7, 8,   // tri0
                      1, 2, 9,   // tri1
                      2, 10, 11, // tri2
                      0, 12, 13  // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Larm2_wls_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*8, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&wls_1_v[114], 5, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*9, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x0C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&wls_1_v[119], 16, 5),
    gsSPNTrianglesInit_5b(8,       // tri count
                          0, 3, 5, // tri0
                          3, 6, 7, // tri1
                          0, 8, 9  // tri2
                          ),
    gsSPNTriangles_5b(0, 10, 1, // tri0
                      3, 4, 11, // tri1
                      4, 2, 12, // tri2
                      4, 13, 14 // tri3
                      ),
    gsSPNTriangles_5b(4, 15, 16, // tri0
                      0, 0, 0,   // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPNTrianglesInit_5b(2,          // tri count
                          17, 18, 19, // tri0
                          17, 19, 20, // tri1
                          0, 0, 0     // tri2
                          ),
    gsSPEndDisplayList(),
};

static Gfx Larm1_wls_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*7, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&wls_1_v[135], 3, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*8, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x0C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&wls_1_v[138], 11, 3),
    gsSPNTrianglesInit_5b(7,       // tri count
                          0, 3, 4, // tri0
                          0, 5, 6, // tri1
                          1, 0, 7  // tri2
                          ),
    gsSPNTriangles_5b(1, 8, 9,   // tri0
                      2, 1, 10,  // tri1
                      2, 11, 12, // tri2
                      0, 2, 13   // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx chest_wls_model[] = {
    gsSPMatrix(anime_6_model, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&wls_1_v[208], 7, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*7, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x2C0, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&wls_1_v[215], 21, 7),
    gsSPNTrianglesInit_5b(14,       // tri count
                          6, 4, 7,  // tri0
                          0, 8, 9,  // tri1
                          4, 10, 11 // tri2
                          ),
    gsSPNTriangles_5b(2, 6, 12,  // tri0
                      6, 14, 13, // tri1
                      3, 5, 15,  // tri2
                      0, 16, 17  // tri3
                      ),
    gsSPNTriangles_5b(4, 18, 19, // tri0
                      3, 20, 21, // tri1
                      0, 2, 22,  // tri2
                      4, 1, 23   // tri3
                      ),
    gsSPNTriangles_5b(1, 3, 24,  // tri0
                      3, 25, 26, // tri1
                      5, 0, 27,  // tri2
                      0, 0, 0    // tri3
                      ),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x240, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&wls_1_v[236], 26, 0),
    gsSPNTrianglesInit_5b(2,       // tri count
                          0, 1, 2, // tri0
                          0, 2, 3, // tri1
                          0, 0, 0  // tri2
                          ),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x340, G_IM_FMT_CI, 32, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPNTrianglesInit_5b(24,      // tri count
                          4, 5, 6, // tri0
                          4, 6, 7, // tri1
                          8, 9, 10 // tri2
                          ),
    gsSPNTriangles_5b(8, 10, 11,  // tri0
                      12, 13, 10, // tri1
                      14, 15, 16, // tri2
                      14, 16, 17  // tri3
                      ),
    gsSPNTriangles_5b(18, 17, 16, // tri0
                      18, 19, 9,  // tri1
                      11, 10, 20, // tri2
                      5, 21, 22   // tri3
                      ),
    gsSPNTriangles_5b(5, 22, 23, // tri0
                      21, 5, 4,  // tri1
                      6, 15, 14, // tri2
                      9, 19, 13  // tri3
                      ),
    gsSPNTriangles_5b(7, 6, 14,  // tri0
                      6, 24, 23, // tri1
                      18, 9, 8,  // tri2
                      13, 25, 20 // tri3
                      ),
    gsSPNTriangles_5b(13, 20, 10, // tri0
                      5, 23, 24,  // tri1
                      16, 19, 18, // tri2
                      23, 15, 6   // tri3
                      ),
    gsSPNTriangles_5b(13, 12, 9, // tri0
                      0, 0, 0,   // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x440, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPVertex(&wls_1_v[262], 31, 0),
    gsSPNTrianglesInit_5b(4,       // tri count
                          0, 1, 2, // tri0
                          3, 4, 1, // tri1
                          4, 3, 5  // tri2
                          ),
    gsSPNTriangles_5b(1, 5, 6, // tri0
                      0, 0, 0, // tri1
                      0, 0, 0, // tri2
                      0, 0, 0  // tri3
                      ),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x4C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPNTrianglesInit_5b(4,          // tri count
                          7, 8, 9,    // tri0
                          10, 11, 12, // tri1
                          13, 8, 7    // tri2
                          ),
    gsSPNTriangles_5b(9, 8, 13, // tri0
                      0, 0, 0,  // tri1
                      0, 0, 0,  // tri2
                      0, 0, 0   // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x500, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPNTrianglesInit_5b(2,          // tri count
                          14, 15, 16, // tri0
                          16, 15, 17, // tri1
                          0, 0, 0     // tri2
                          ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x540, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPNTrianglesInit_5b(12,         // tri count
                          18, 19, 20, // tri0
                          21, 22, 19, // tri1
                          23, 24, 22  // tri2
                          ),
    gsSPNTriangles_5b(25, 26, 24, // tri0
                      20, 19, 22, // tri1
                      20, 22, 27, // tri2
                      28, 21, 18  // tri3
                      ),
    gsSPNTriangles_5b(19, 18, 21, // tri0
                      22, 21, 23, // tri1
                      24, 23, 25, // tri2
                      26, 25, 29  // tri3
                      ),
    gsSPNTriangles_5b(18, 30, 28, // tri0
                      0, 0, 0,    // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x500, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPVertex(&wls_1_v[293], 29, 0),
    gsSPNTrianglesInit_5b(6,       // tri count
                          0, 1, 2, // tri0
                          0, 2, 3, // tri1
                          4, 0, 3  // tri2
                          ),
    gsSPNTriangles_5b(5, 6, 7, // tri0
                      8, 2, 1, // tri1
                      8, 1, 9, // tri2
                      0, 0, 0  // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x580, G_IM_FMT_CI, 32, 32, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPNTrianglesInit_5b(22,         // tri count
                          10, 11, 12, // tri0
                          10, 12, 13, // tri1
                          13, 12, 14  // tri2
                          ),
    gsSPNTriangles_5b(14, 12, 15, // tri0
                      16, 11, 10, // tri1
                      17, 18, 19, // tri2
                      20, 12, 21  // tri3
                      ),
    gsSPNTriangles_5b(22, 21, 11, // tri0
                      21, 12, 11, // tri1
                      19, 18, 23, // tri2
                      21, 24, 20  // tri3
                      ),
    gsSPNTriangles_5b(25, 24, 26, // tri0
                      26, 23, 25, // tri1
                      21, 22, 19, // tri2
                      11, 27, 22  // tri3
                      ),
    gsSPNTriangles_5b(25, 20, 24, // tri0
                      22, 27, 17, // tri1
                      19, 26, 24, // tri2
                      19, 24, 21  // tri3
                      ),
    gsSPNTriangles_5b(23, 26, 19, // tri0
                      19, 22, 17, // tri1
                      14, 15, 28, // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Rfoot3_wls_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*5, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&wls_1_v[364], 3, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*6, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x0C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&wls_1_v[367], 18, 3),
    gsSPNTrianglesInit_5b(6,       // tri count
                          0, 3, 4, // tri0
                          2, 1, 5, // tri1
                          2, 6, 7  // tri2
                          ),
    gsSPNTriangles_5b(0, 2, 8,   // tri0
                      1, 0, 9,   // tri1
                      0, 10, 11, // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPNTrianglesInit_5b(5,          // tri count
                          12, 13, 14, // tri0
                          15, 16, 12, // tri1
                          12, 14, 15  // tri2
                          ),
    gsSPNTriangles_5b(17, 18, 19, // tri0
                      17, 19, 20, // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Rfoot2_wls_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*4, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&wls_1_v[385], 4, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*5, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x0C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&wls_1_v[389], 10, 4),
    gsSPNTrianglesInit_5b(7,       // tri count
                          2, 4, 5, // tri0
                          2, 3, 6, // tri1
                          0, 7, 8  // tri2
                          ),
    gsSPNTriangles_5b(1, 2, 9,   // tri0
                      3, 10, 11, // tri1
                      3, 0, 12,  // tri2
                      0, 1, 13   // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Rfoot1_wls_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x0C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&wls_1_v[399], 7, 0),
    gsSPNTrianglesInit_5b(7,       // tri count
                          0, 1, 2, // tri0
                          3, 4, 5, // tri1
                          5, 6, 3  // tri2
                          ),
    gsSPNTriangles_5b(5, 0, 6, // tri0
                      2, 1, 4, // tri1
                      2, 6, 0, // tri2
                      3, 2, 4  // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Lfoot3_wls_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*2, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&wls_1_v[322], 3, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*3, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x0C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&wls_1_v[325], 18, 3),
    gsSPNTrianglesInit_5b(6,       // tri count
                          0, 3, 4, // tri0
                          1, 5, 6, // tri1
                          1, 2, 7  // tri2
                          ),
    gsSPNTriangles_5b(2, 0, 8,   // tri0
                      0, 1, 9,   // tri1
                      0, 10, 11, // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPNTrianglesInit_5b(5,          // tri count
                          12, 13, 14, // tri0
                          14, 15, 16, // tri1
                          16, 12, 14  // tri2
                          ),
    gsSPNTriangles_5b(17, 18, 19, // tri0
                      17, 19, 20, // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Lfoot2_wls_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*1, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&wls_1_v[343], 4, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*2, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x0C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&wls_1_v[347], 10, 4),
    gsSPNTrianglesInit_5b(7,       // tri count
                          2, 4, 5, // tri0
                          3, 2, 6, // tri1
                          0, 7, 8  // tri2
                          ),
    gsSPNTriangles_5b(2, 1, 9,   // tri0
                      3, 10, 11, // tri1
                      0, 3, 12,  // tri2
                      1, 0, 13   // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Lfoot1_wls_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x0C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&wls_1_v[357], 7, 0),
    gsSPNTrianglesInit_5b(7,       // tri count
                          0, 1, 2, // tri0
                          3, 4, 5, // tri1
                          5, 6, 3  // tri2
                          ),
    gsSPNTriangles_5b(6, 2, 3, // tri0
                      4, 1, 0, // tri1
                      2, 6, 0, // tri2
                      4, 0, 5  // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx base_wls_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x2C0, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&wls_1_v[406], 15, 0),
    gsSPNTrianglesInit_5b(21,      // tri count
                          0, 1, 2, // tri0
                          1, 0, 3, // tri1
                          1, 3, 4  // tri2
                          ),
    gsSPNTriangles_5b(5, 0, 2, // tri0
                      6, 3, 0, // tri1
                      6, 0, 5, // tri2
                      5, 2, 7  // tri3
                      ),
    gsSPNTriangles_5b(1, 8, 9,    // tri0
                      10, 11, 12, // tri1
                      10, 6, 13,  // tri2
                      9, 7, 2     // tri3
                      ),
    gsSPNTriangles_5b(8, 1, 4,  // tri0
                      9, 2, 1,  // tri1
                      7, 14, 5, // tri2
                      6, 5, 14  // tri3
                      ),
    gsSPNTriangles_5b(10, 4, 3,  // tri0
                      10, 3, 6,  // tri1
                      12, 4, 10, // tri2
                      13, 11, 10 // tri3
                      ),
    gsSPNTriangles_5b(4, 12, 8,  // tri0
                      14, 13, 6, // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPEndDisplayList(),
};

// clang-format off
static cKF_Joint_R_c cKF_je_r_wls_1_tbl[] = {
    /* joint 0 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 0,1000,0 },
    /* joint 1 */ { base_wls_model, 4, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 2 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 0,-250,0 },
    /* joint 3 */ { Lfoot1_wls_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 4 */ { Lfoot2_wls_model, 1, cKF_JOINT_FLAG_DISP_OPA, 500,0,0 },
    /* joint 5 */ { Lfoot3_wls_model, 0, cKF_JOINT_FLAG_DISP_OPA, 400,0,0 },
    /* joint 6 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 0,250,0 },
    /* joint 7 */ { Rfoot1_wls_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 8 */ { Rfoot2_wls_model, 1, cKF_JOINT_FLAG_DISP_OPA, 500,0,0 },
    /* joint 9 */ { Rfoot3_wls_model, 0, cKF_JOINT_FLAG_DISP_OPA, 400,0,0 },
    /* joint 10 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,-700 },
    /* joint 11 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 12 */ { NULL, 0, cKF_JOINT_FLAG_DISP_OPA, 650,0,0 },
    /* joint 13 */ { chest_wls_model, 3, cKF_JOINT_FLAG_DISP_OPA, 600,0,0 },
    /* joint 14 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 1300,-1100,0 },
    /* joint 15 */ { Larm1_wls_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 16 */ { Larm2_wls_model, 0, cKF_JOINT_FLAG_DISP_OPA, 650,0,0 },
    /* joint 17 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 1300,1100,0 },
    /* joint 18 */ { Rarm1_wls_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 19 */ { Rarm2_wls_model, 1, cKF_JOINT_FLAG_DISP_OPA, 650,0,0 },
    /* joint 20 */ { NULL, 0, cKF_JOINT_FLAG_DISP_OPA, 650,0,0 },
    /* joint 21 */ { NULL, 2, cKF_JOINT_FLAG_DISP_OPA, 1400,0,0 },
    /* joint 22 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 100,0,700 },
    /* joint 23 */ { mouth_wls_model, 0, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 24 */ { head_wls_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 25 */ { NULL, 0, cKF_JOINT_FLAG_DISP_OPA, 800,0,0 },
};
// clang-format on

extern cKF_Skeleton_R_c cKF_bs_r_wls_1 = { 26, 14, cKF_je_r_wls_1_tbl };
