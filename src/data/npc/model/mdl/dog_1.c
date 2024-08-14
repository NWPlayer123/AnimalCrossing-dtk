#include "c_keyframe.h"
#include "libforest/gbi_extensions.h"

static Vtx dog_1_v[] = {
#include "assets/npc/mdl/dog_1_v.inc"
};

static Gfx head_dog_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*7, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&dog_1_v[0], 7, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*12, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_3_txt, G_IM_FMT_CI, 32, 32, 14, GX_REPEAT, GX_CLAMP, 0, 0),
    gsDPSetTileSize(0, 0, 0, 508, 124),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&dog_1_v[7], 14, 7),
    gsSPNTrianglesInit_5b(10,       // tri count
                          3, 7, 8,  // tri0
                          3, 1, 9,  // tri1
                          1, 11, 10 // tri2
                          ),
    gsSPNTriangles_5b(4, 2, 12, // tri0
                      0, 13, 2, // tri1
                      1, 4, 14, // tri2
                      5, 3, 15  // tri3
                      ),
    gsSPNTriangles_5b(0, 16, 17, // tri0
                      0, 6, 18,  // tri1
                      2, 19, 20, // tri2
                      0, 0, 0    // tri3
                      ),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPLoadTextureBlock_4b_Dolphin(anime_2_txt, G_IM_FMT_CI, 32, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPVertex(&dog_1_v[21], 31, 0),
    gsSPNTrianglesInit_5b(32,      // tri count
                          0, 1, 2, // tri0
                          3, 4, 5, // tri1
                          2, 1, 6  // tri2
                          ),
    gsSPNTriangles_5b(7, 0, 8,   // tri0
                      9, 4, 3,   // tri1
                      5, 10, 8,  // tri2
                      11, 12, 13 // tri3
                      ),
    gsSPNTriangles_5b(10, 14, 15, // tri0
                      5, 14, 10,  // tri1
                      6, 16, 9,   // tri2
                      7, 17, 0    // tri3
                      ),
    gsSPNTriangles_5b(18, 19, 16, // tri0
                      5, 2, 3,    // tri1
                      2, 6, 3,    // tri2
                      3, 6, 9     // tri3
                      ),
    gsSPNTriangles_5b(18, 16, 6, // tri0
                      13, 14, 4, // tri1
                      4, 11, 13, // tri2
                      6, 1, 18   // tri3
                      ),
    gsSPNTriangles_5b(13, 12, 20, // tri0
                      21, 19, 18, // tri1
                      8, 10, 7,   // tri2
                      17, 1, 0    // tri3
                      ),
    gsSPNTriangles_5b(7, 10, 15, // tri0
                      4, 14, 5,  // tri1
                      9, 11, 4,  // tri2
                      8, 2, 5    // tri3
                      ),
    gsSPNTriangles_5b(8, 0, 2,    // tri0
                      20, 14, 13, // tri1
                      18, 1, 21,  // tri2
                      21, 1, 17   // tri3
                      ),
    gsSPNTriangles_5b(15, 14, 20, // tri0
                      0, 0, 0,    // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x000, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPNTrianglesInit_5b(13,         // tri count
                          22, 23, 24, // tri0
                          25, 26, 27, // tri1
                          22, 24, 28  // tri2
                          ),
    gsSPNTriangles_5b(26, 22, 28, // tri0
                      26, 25, 22, // tri1
                      29, 24, 30, // tri2
                      24, 23, 30  // tri3
                      ),
    gsSPNTriangles_5b(27, 28, 29, // tri0
                      30, 22, 29, // tri1
                      22, 25, 29, // tri2
                      30, 23, 22  // tri3
                      ),
    gsSPNTriangles_5b(25, 27, 29, // tri0
                      29, 28, 24, // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_1_txt, G_IM_FMT_CI, 32, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPVertex(&dog_1_v[52], 21, 0),
    gsSPNTrianglesInit_5b(27,      // tri count
                          0, 1, 2, // tri0
                          0, 2, 3, // tri1
                          4, 5, 6  // tri2
                          ),
    gsSPNTriangles_5b(7, 5, 8,   // tri0
                      9, 6, 5,   // tri1
                      9, 7, 10,  // tri2
                      11, 10, 12 // tri3
                      ),
    gsSPNTriangles_5b(10, 13, 12, // tri0
                      5, 7, 9,    // tri1
                      4, 6, 14,   // tri2
                      4, 14, 15   // tri3
                      ),
    gsSPNTriangles_5b(16, 17, 13, // tri0
                      17, 16, 18, // tri1
                      14, 6, 0,   // tri2
                      18, 16, 8   // tri3
                      ),
    gsSPNTriangles_5b(14, 0, 3,   // tri0
                      15, 14, 19, // tri1
                      10, 16, 13, // tri2
                      3, 19, 14   // tri3
                      ),
    gsSPNTriangles_5b(7, 16, 10,  // tri0
                      9, 10, 20,  // tri1
                      10, 11, 20, // tri2
                      20, 11, 1   // tri3
                      ),
    gsSPNTriangles_5b(1, 0, 20, // tri0
                      20, 0, 9, // tri1
                      6, 9, 0,  // tri2
                      7, 8, 16  // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x080, G_IM_FMT_CI, 32, 32, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPVertex(&dog_1_v[73], 28, 0),
    gsSPNTrianglesInit_5b(23,      // tri count
                          0, 1, 2, // tri0
                          3, 4, 5, // tri1
                          6, 7, 8  // tri2
                          ),
    gsSPNTriangles_5b(9, 8, 10,  // tri0
                      11, 5, 4,  // tri1
                      2, 12, 11, // tri2
                      13, 2, 1   // tri3
                      ),
    gsSPNTriangles_5b(8, 5, 12,  // tri0
                      11, 12, 5, // tri1
                      5, 7, 3,   // tri2
                      8, 9, 6    // tri3
                      ),
    gsSPNTriangles_5b(10, 14, 9,  // tri0
                      4, 15, 11,  // tri1
                      16, 14, 10, // tri2
                      13, 17, 16  // tri3
                      ),
    gsSPNTriangles_5b(13, 16, 2, // tri0
                      2, 16, 10, // tri1
                      12, 10, 8, // tri2
                      11, 0, 2   // tri3
                      ),
    gsSPNTriangles_5b(18, 1, 0,  // tri0
                      0, 11, 15, // tri1
                      8, 7, 5,   // tri2
                      10, 12, 2  // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x000, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPNTrianglesInit_5b(13,         // tri count
                          19, 20, 21, // tri0
                          19, 22, 23, // tri1
                          19, 24, 22  // tri2
                          ),
    gsSPNTriangles_5b(22, 25, 23, // tri0
                      19, 21, 24, // tri1
                      26, 20, 19, // tri2
                      26, 25, 22  // tri3
                      ),
    gsSPNTriangles_5b(21, 27, 24, // tri0
                      20, 26, 22, // tri1
                      20, 22, 27, // tri2
                      22, 24, 27  // tri3
                      ),
    gsSPNTriangles_5b(23, 26, 19, // tri0
                      23, 25, 26, // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Rarm2_dog_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*10, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&dog_1_v[132], 4, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*11, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x280, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&dog_1_v[136], 13, 4),
    gsSPNTrianglesInit_5b(7,       // tri count
                          1, 4, 5, // tri0
                          2, 6, 1, // tri1
                          0, 1, 7  // tri2
                          ),
    gsSPNTriangles_5b(3, 8, 9,   // tri0
                      0, 11, 10, // tri1
                      3, 0, 12,  // tri2
                      3, 13, 2   // tri3
                      ),
    gsSPNTrianglesInit_5b(1,          // tri count
                          14, 15, 16, // tri0
                          0, 0, 0,    // tri1
                          0, 0, 0     // tri2
                          ),
    gsSPEndDisplayList(),
};

static Gfx Rarm_dog_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*7, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&dog_1_v[149], 3, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*10, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x280, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&dog_1_v[152], 11, 3),
    gsSPNTrianglesInit_5b(7,       // tri count
                          2, 3, 4, // tri0
                          0, 6, 5, // tri1
                          0, 2, 7  // tri2
                          ),
    gsSPNTriangles_5b(1, 8, 9,  // tri0
                      2, 1, 10, // tri1
                      1, 0, 11, // tri2
                      0, 12, 13 // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Larm2_dog_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*8, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&dog_1_v[101], 4, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*9, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x280, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&dog_1_v[105], 13, 4),
    gsSPNTrianglesInit_5b(7,       // tri count
                          1, 4, 5, // tri0
                          1, 6, 2, // tri1
                          1, 0, 7  // tri2
                          ),
    gsSPNTriangles_5b(3, 8, 9,   // tri0
                      0, 11, 10, // tri1
                      0, 3, 12,  // tri2
                      2, 13, 3   // tri3
                      ),
    gsSPNTrianglesInit_5b(1,          // tri count
                          14, 15, 16, // tri0
                          0, 0, 0,    // tri1
                          0, 0, 0     // tri2
                          ),
    gsSPEndDisplayList(),
};

static Gfx Larm1_dog_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*7, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&dog_1_v[118], 3, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*8, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x280, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&dog_1_v[121], 11, 3),
    gsSPNTrianglesInit_5b(7,       // tri count
                          2, 3, 4, // tri0
                          0, 6, 5, // tri1
                          2, 0, 7  // tri2
                          ),
    gsSPNTriangles_5b(1, 8, 9,   // tri0
                      1, 2, 10,  // tri1
                      0, 11, 12, // tri2
                      0, 1, 13   // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx chest_dog_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_3_txt, G_IM_FMT_CI, 32, 32, 14, GX_REPEAT, GX_CLAMP, 0, 0),
    gsDPSetTileSize(0, 0, 0, 508, 124),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&dog_1_v[163], 32, 0),
    gsSPNTrianglesInit_5b(29,      // tri count
                          0, 1, 2, // tri0
                          3, 4, 5, // tri1
                          3, 6, 4  // tri2
                          ),
    gsSPNTriangles_5b(2, 7, 8,    // tri0
                      5, 9, 3,    // tri1
                      10, 11, 12, // tri2
                      13, 14, 15  // tri3
                      ),
    gsSPNTriangles_5b(16, 17, 18, // tri0
                      13, 19, 14, // tri1
                      20, 7, 2,   // tri2
                      21, 9, 5    // tri3
                      ),
    gsSPNTriangles_5b(22, 1, 0,   // tri0
                      23, 6, 3,   // tri1
                      10, 12, 24, // tri2
                      24, 25, 10  // tri3
                      ),
    gsSPNTriangles_5b(18, 25, 24, // tri0
                      19, 26, 21, // tri1
                      3, 9, 23,   // tri2
                      0, 27, 22   // tri3
                      ),
    gsSPNTriangles_5b(2, 1, 20,  // tri0
                      5, 14, 21, // tri1
                      0, 2, 8,   // tri2
                      28, 7, 19  // tri3
                      ),
    gsSPNTriangles_5b(29, 7, 28,  // tri0
                      22, 27, 30, // tri1
                      20, 19, 7,  // tri2
                      8, 27, 0    // tri3
                      ),
    gsSPNTriangles_5b(19, 20, 26, // tri0
                      22, 30, 31, // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPVertex(&dog_1_v[195], 12, 0),
    gsSPNTrianglesInit_5b(5,       // tri count
                          0, 1, 2, // tri0
                          2, 1, 3, // tri1
                          4, 5, 6  // tri2
                          ),
    gsSPNTriangles_5b(5, 7, 8,   // tri0
                      9, 10, 11, // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx tail1_dog_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x300, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&dog_1_v[234], 15, 0),
    gsSPNTrianglesInit_5b(7,       // tri count
                          0, 1, 2, // tri0
                          3, 4, 5, // tri1
                          6, 7, 0  // tri2
                          ),
    gsSPNTriangles_5b(8, 9, 10,  // tri0
                      11, 8, 12, // tri1
                      10, 13, 8, // tri2
                      5, 1, 14   // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx tail2_dog_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*5, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&dog_1_v[207], 6, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*6, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x300, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&dog_1_v[213], 21, 6),
    gsSPNTrianglesInit_5b(8,        // tri count
                          1, 6, 7,  // tri0
                          3, 8, 9,  // tri1
                          4, 10, 11 // tri2
                          ),
    gsSPNTriangles_5b(3, 12, 13, // tri0
                      3, 2, 14,  // tri1
                      1, 0, 15,  // tri2
                      0, 4, 16   // tri3
                      ),
    gsSPNTriangles_5b(5, 3, 17, // tri0
                      0, 0, 0,  // tri1
                      0, 0, 0,  // tri2
                      0, 0, 0   // tri3
                      ),
    gsSPNTrianglesInit_5b(4,          // tri count
                          18, 19, 20, // tri0
                          21, 22, 23, // tri1
                          20, 22, 24  // tri2
                          ),
    gsSPNTriangles_5b(25, 18, 26, // tri0
                      0, 0, 0,    // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Rfoot2_dog_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*3, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&dog_1_v[294], 6, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*4, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x380, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&dog_1_v[300], 18, 6),
    gsSPNTrianglesInit_5b(9,       // tri count
                          4, 3, 6, // tri0
                          3, 7, 8, // tri1
                          2, 9, 3  // tri2
                          ),
    gsSPNTriangles_5b(1, 0, 10,  // tri0
                      2, 11, 12, // tri1
                      0, 13, 14, // tri2
                      1, 15, 2   // tri3
                      ),
    gsSPNTriangles_5b(1, 16, 17, // tri0
                      0, 5, 18,  // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPNTrianglesInit_5b(2,          // tri count
                          19, 20, 21, // tri0
                          21, 22, 23, // tri1
                          0, 0, 0     // tri2
                          ),
    gsSPEndDisplayList(),
};

static Gfx Rfoot1_dog_model[] = {
    gsSPMatrix(anime_6_model, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&dog_1_v[318], 6, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*3, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x380, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&dog_1_v[324], 15, 6),
    gsSPNTrianglesInit_5b(10,      // tri count
                          1, 7, 6, // tri0
                          1, 0, 8, // tri1
                          3, 4, 9  // tri2
                          ),
    gsSPNTriangles_5b(4, 10, 11, // tri0
                      2, 1, 12,  // tri1
                      2, 13, 14, // tri2
                      0, 15, 16  // tri3
                      ),
    gsSPNTriangles_5b(0, 3, 17,  // tri0
                      3, 18, 19, // tri1
                      5, 2, 20,  // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Lfoot2_dog_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*1, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&dog_1_v[249], 6, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*2, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x380, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&dog_1_v[255], 18, 6),
    gsSPNTrianglesInit_5b(9,       // tri count
                          3, 4, 6, // tri0
                          3, 7, 8, // tri1
                          3, 9, 2  // tri2
                          ),
    gsSPNTriangles_5b(0, 1, 10,  // tri0
                      2, 11, 12, // tri1
                      0, 13, 14, // tri2
                      2, 15, 1   // tri3
                      ),
    gsSPNTriangles_5b(1, 16, 17, // tri0
                      5, 0, 18,  // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPNTrianglesInit_5b(2,          // tri count
                          19, 20, 21, // tri0
                          22, 23, 19, // tri1
                          0, 0, 0     // tri2
                          ),
    gsSPEndDisplayList(),
};

static Gfx Lfoot1_dog_model[] = {
    gsSPMatrix(anime_6_model, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&dog_1_v[273], 6, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*1, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x380, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&dog_1_v[279], 15, 6),
    gsSPNTrianglesInit_5b(10,      // tri count
                          1, 7, 6, // tri0
                          0, 1, 8, // tri1
                          4, 3, 9  // tri2
                          ),
    gsSPNTriangles_5b(4, 10, 11, // tri0
                      1, 2, 12,  // tri1
                      2, 13, 14, // tri2
                      0, 15, 16  // tri3
                      ),
    gsSPNTriangles_5b(3, 0, 17,  // tri0
                      3, 18, 19, // tri1
                      2, 5, 20,  // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx base_dog_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x380, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&dog_1_v[339], 24, 0),
    gsSPNTrianglesInit_5b(11,      // tri count
                          0, 1, 2, // tri0
                          0, 3, 4, // tri1
                          5, 2, 6  // tri2
                          ),
    gsSPNTriangles_5b(7, 5, 6, // tri0
                      2, 5, 8, // tri1
                      9, 4, 3, // tri2
                      8, 0, 2  // tri3
                      ),
    gsSPNTriangles_5b(0, 8, 3,  // tri0
                      5, 7, 10, // tri1
                      4, 1, 0,  // tri2
                      4, 11, 1  // tri3
                      ),
    gsSPNTrianglesInit_5b(11,         // tri count
                          12, 13, 14, // tri0
                          15, 16, 17, // tri1
                          16, 18, 17  // tri2
                          ),
    gsSPNTriangles_5b(19, 20, 15, // tri0
                      13, 15, 20, // tri1
                      19, 17, 21, // tri2
                      17, 19, 15  // tri3
                      ),
    gsSPNTriangles_5b(13, 16, 15, // tri0
                      20, 14, 13, // tri1
                      12, 14, 22, // tri2
                      23, 22, 14  // tri3
                      ),
    gsSPEndDisplayList(),
};

// clang-format off
static cKF_Joint_R_c cKF_je_r_dog_1_tbl[] = {
    /* joint 0 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 0,1000,0 },
    /* joint 1 */ { base_dog_model, 4, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 2 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 0,-250,0 },
    /* joint 3 */ { Lfoot1_dog_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 4 */ { Lfoot2_dog_model, 1, cKF_JOINT_FLAG_DISP_OPA, 500,0,0 },
    /* joint 5 */ { NULL, 0, cKF_JOINT_FLAG_DISP_OPA, 400,0,0 },
    /* joint 6 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 0,250,0 },
    /* joint 7 */ { Rfoot1_dog_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 8 */ { Rfoot2_dog_model, 1, cKF_JOINT_FLAG_DISP_OPA, 500,0,0 },
    /* joint 9 */ { NULL, 0, cKF_JOINT_FLAG_DISP_OPA, 400,0,0 },
    /* joint 10 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 125,0,-675 },
    /* joint 11 */ { tail1_dog_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 12 */ { tail2_dog_model, 0, cKF_JOINT_FLAG_DISP_OPA, 525,0,0 },
    /* joint 13 */ { chest_dog_model, 3, cKF_JOINT_FLAG_DISP_OPA, 626,0,0 },
    /* joint 14 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 425,-700,100 },
    /* joint 15 */ { Larm1_dog_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 16 */ { Larm2_dog_model, 0, cKF_JOINT_FLAG_DISP_OPA, 426,0,0 },
    /* joint 17 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 425,700,100 },
    /* joint 18 */ { Rarm_dog_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 19 */ { Rarm2_dog_model, 1, cKF_JOINT_FLAG_DISP_OPA, 425,0,0 },
    /* joint 20 */ { NULL, 0, cKF_JOINT_FLAG_DISP_OPA, 275,0,0 },
    /* joint 21 */ { NULL, 2, cKF_JOINT_FLAG_DISP_OPA, 549,0,0 },
    /* joint 22 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 500,0,600 },
    /* joint 23 */ { NULL, 0, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 24 */ { head_dog_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 25 */ { NULL, 0, cKF_JOINT_FLAG_DISP_OPA, 1325,0,0 },
};
// clang-format on

extern cKF_Skeleton_R_c cKF_bs_r_dog_1 = { 26, 13, cKF_je_r_dog_1_tbl };
