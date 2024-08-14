#include "c_keyframe.h"
#include "libforest/gbi_extensions.h"

static Vtx rhn_1_v[] = {
#include "assets/npc/mdl/rhn_1_v.inc"
};

static Gfx head_rhn_model[] = {
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 7, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&rhn_1_v[21], 8, 0),
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 13.0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_3_txt, G_IM_FMT_CI, 32, 32, 14, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetTileSize(0, 0, 0, 252, 124),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&rhn_1_v[29], 19, 8),
    gsSPNTrianglesInit_5b(13,      // tri count
                          0, 8, 6, // tri0
                          5, 9, 4, // tri1
                          3, 10, 5 // tri2
                          ),
    gsSPNTriangles_5b(0, 11, 12, // tri0
                      4, 13, 14, // tri1
                      2, 15, 16, // tri2
                      3, 17, 18  // tri3
                      ),
    gsSPNTriangles_5b(5, 19, 20, // tri0
                      0, 4, 21,  // tri1
                      3, 1, 22,  // tri2
                      7, 23, 2   // tri3
                      ),
    gsSPNTriangles_5b(6, 24, 25, // tri0
                      6, 26, 7,  // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x080, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPVertex(&rhn_1_v[48], 27, 0),
    gsSPNTrianglesInit_5b(2,       // tri count
                          0, 1, 2, // tri0
                          2, 3, 4, // tri1
                          0, 0, 0  // tri2
                          ),
    gsSPNTrianglesInit_5b(2,       // tri count
                          5, 6, 7, // tri0
                          8, 9, 5, // tri1
                          0, 0, 0  // tri2
                          ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x0C0, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPNTrianglesInit_5b(15,         // tri count
                          10, 11, 12, // tri0
                          13, 14, 15, // tri1
                          16, 17, 14  // tri2
                          ),
    gsSPNTriangles_5b(18, 19, 20, // tri0
                      11, 21, 14, // tri1
                      19, 13, 22, // tri2
                      23, 22, 13  // tri3
                      ),
    gsSPNTriangles_5b(24, 17, 16, // tri0
                      14, 21, 25, // tri1
                      15, 26, 13, // tri2
                      13, 26, 23  // tri3
                      ),
    gsSPNTriangles_5b(25, 15, 14, // tri0
                      14, 13, 16, // tri1
                      14, 12, 11, // tri2
                      22, 20, 19  // tri3
                      ),
    gsSPVertex(&rhn_1_v[75], 17, 0),
    gsSPNTrianglesInit_5b(15,      // tri count
                          0, 1, 2, // tri0
                          3, 4, 5, // tri1
                          6, 7, 8  // tri2
                          ),
    gsSPNTriangles_5b(2, 1, 9,    // tri0
                      10, 11, 12, // tri1
                      9, 8, 2,    // tri2
                      2, 13, 0    // tri3
                      ),
    gsSPNTriangles_5b(8, 9, 6,   // tri0
                      8, 4, 3,   // tri1
                      11, 10, 8, // tri2
                      3, 2, 8    // tri3
                      ),
    gsSPNTriangles_5b(14, 15, 16, // tri0
                      8, 7, 11,   // tri1
                      13, 2, 15,  // tri2
                      15, 14, 13  // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x140, G_IM_FMT_CI, 32, 16, 15, GX_CLAMP, GX_MIRROR, 0, 0),
    gsDPSetTileSize(0, 0, 0, 124, 124),
    gsSPVertex(&rhn_1_v[92], 29, 0),
    gsSPNTrianglesInit_5b(39,      // tri count
                          0, 1, 2, // tri0
                          3, 4, 5, // tri1
                          2, 6, 7  // tri2
                          ),
    gsSPNTriangles_5b(4, 3, 0, // tri0
                      4, 0, 7, // tri1
                      4, 8, 5, // tri2
                      4, 9, 8  // tri3
                      ),
    gsSPNTriangles_5b(10, 11, 12, // tri0
                      13, 14, 12, // tri1
                      15, 10, 6,  // tri2
                      8, 13, 16   // tri3
                      ),
    gsSPNTriangles_5b(6, 9, 7,   // tri0
                      17, 18, 6, // tri1
                      10, 17, 6, // tri2
                      18, 17, 19 // tri3
                      ),
    gsSPNTriangles_5b(10, 14, 19, // tri0
                      20, 21, 22, // tri1
                      12, 14, 10, // tri2
                      18, 9, 6    // tri3
                      ),
    gsSPNTriangles_5b(2, 7, 0,    // tri0
                      22, 12, 11, // tri1
                      19, 17, 10, // tri2
                      6, 2, 15    // tri3
                      ),
    gsSPNTriangles_5b(8, 18, 23,  // tri0
                      8, 23, 13,  // tri1
                      24, 25, 26, // tri2
                      19, 23, 18  // tri3
                      ),
    gsSPNTriangles_5b(19, 14, 13, // tri0
                      25, 24, 22, // tri1
                      12, 24, 13, // tri2
                      8, 9, 18    // tri3
                      ),
    gsSPNTriangles_5b(7, 9, 4,    // tri0
                      24, 12, 22, // tri1
                      5, 27, 3,   // tri2
                      13, 23, 19  // tri3
                      ),
    gsSPNTriangles_5b(16, 5, 8,   // tri0
                      22, 11, 20, // tri1
                      22, 21, 25, // tri2
                      28, 20, 11  // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_1_txt, G_IM_FMT_CI, 32, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPVertex(&rhn_1_v[121], 25, 0),
    gsSPNTrianglesInit_5b(27,      // tri count
                          0, 1, 2, // tri0
                          0, 3, 4, // tri1
                          5, 6, 7  // tri2
                          ),
    gsSPNTriangles_5b(8, 9, 10,  // tri0
                      11, 0, 2,  // tri1
                      12, 5, 7,  // tri2
                      13, 14, 15 // tri3
                      ),
    gsSPNTriangles_5b(16, 7, 14,  // tri0
                      13, 16, 14, // tri1
                      2, 15, 11,  // tri2
                      17, 18, 4   // tri3
                      ),
    gsSPNTriangles_5b(3, 0, 19,  // tri0
                      20, 3, 21, // tri1
                      4, 18, 0,  // tri2
                      7, 16, 12  // tri3
                      ),
    gsSPNTriangles_5b(4, 22, 23,  // tri0
                      7, 8, 14,   // tri1
                      14, 11, 15, // tri2
                      0, 18, 1    // tri3
                      ),
    gsSPNTriangles_5b(9, 8, 24,  // tri0
                      4, 23, 17, // tri1
                      8, 7, 24,  // tri2
                      24, 7, 6   // tri3
                      ),
    gsSPNTriangles_5b(22, 4, 20, // tri0
                      20, 4, 3,  // tri1
                      19, 21, 3, // tri2
                      10, 14, 8  // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x240, G_IM_FMT_CI, 32, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPVertex(&rhn_1_v[146], 16, 0),
    gsSPNTrianglesInit_5b(16,      // tri count
                          0, 1, 2, // tri0
                          2, 3, 4, // tri1
                          5, 6, 7  // tri2
                          ),
    gsSPNTriangles_5b(7, 0, 5, // tri0
                      6, 5, 8, // tri1
                      4, 9, 2, // tri2
                      8, 10, 6 // tri3
                      ),
    gsSPNTriangles_5b(2, 1, 3,  // tri0
                      4, 11, 8, // tri1
                      1, 0, 12, // tri2
                      11, 10, 8 // tri3
                      ),
    gsSPNTriangles_5b(8, 9, 4,   // tri0
                      13, 6, 10, // tri1
                      14, 3, 1,  // tri2
                      10, 11, 15 // tri3
                      ),
    gsSPNTriangles_5b(2, 5, 0, // tri0
                      0, 0, 0, // tri1
                      0, 0, 0, // tri2
                      0, 0, 0  // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_2_txt, G_IM_FMT_CI, 32, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPVertex(&rhn_1_v[162], 27, 0),
    gsSPNTrianglesInit_5b(22,      // tri count
                          0, 1, 2, // tri0
                          3, 4, 5, // tri1
                          6, 3, 5  // tri2
                          ),
    gsSPNTriangles_5b(2, 7, 0,  // tri0
                      8, 9, 10, // tri1
                      3, 6, 10, // tri2
                      1, 0, 3   // tri3
                      ),
    gsSPNTriangles_5b(11, 12, 8, // tri0
                      13, 14, 2, // tri1
                      8, 13, 1,  // tri2
                      1, 9, 8    // tri3
                      ),
    gsSPNTriangles_5b(8, 15, 11, // tri0
                      3, 9, 1,   // tri1
                      3, 0, 7,   // tri2
                      10, 9, 3   // tri3
                      ),
    gsSPNTriangles_5b(16, 5, 4,   // tri0
                      10, 15, 8,  // tri1
                      12, 11, 17, // tri2
                      14, 13, 8   // tri3
                      ),
    gsSPNTriangles_5b(2, 1, 13,  // tri0
                      7, 4, 3,   // tri1
                      8, 12, 14, // tri2
                      0, 0, 0    // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x040, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_MIRROR, 0, 0),
    gsDPSetTileSize(0, 0, 0, 60, 60),
    gsSPNTrianglesInit_5b(3,          // tri count
                          18, 19, 20, // tri0
                          21, 18, 20, // tri1
                          21, 20, 22  // tri2
                          ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x2C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_MIRROR, 0, 0),
    gsDPSetTileSize(0, 0, 0, 60, 60),
    gsSPNTrianglesInit_5b(2,          // tri count
                          23, 24, 25, // tri0
                          26, 23, 25, // tri1
                          0, 0, 0     // tri2
                          ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x300, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPVertex(&rhn_1_v[189], 14, 0),
    gsSPNTrianglesInit_5b(4,       // tri count
                          0, 1, 2, // tri0
                          3, 2, 1, // tri1
                          0, 2, 4  // tri2
                          ),
    gsSPNTriangles_5b(3, 4, 2, // tri0
                      0, 0, 0, // tri1
                      0, 0, 0, // tri2
                      0, 0, 0  // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x2C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_MIRROR, 0, 0),
    gsDPSetTileSize(0, 0, 0, 60, 60),
    gsSPNTrianglesInit_5b(2,       // tri count
                          5, 6, 7, // tri0
                          5, 7, 8, // tri1
                          0, 0, 0  // tri2
                          ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x300, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPNTrianglesInit_5b(4,         // tri count
                          9, 10, 11, // tri0
                          10, 9, 12, // tri1
                          13, 9, 11  // tri2
                          ),
    gsSPNTriangles_5b(9, 13, 12, // tri0
                      0, 0, 0,   // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx mouth_rhn_model[] = {
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 13.0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&rhn_1_v[0], 5, 0),
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 12.0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x000, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&rhn_1_v[5], 16, 5),
    gsSPNTrianglesInit_5b(4,       // tri count
                          0, 5, 6, // tri0
                          2, 7, 8, // tri1
                          0, 4, 9  // tri2
                          ),
    gsSPNTriangles_5b(4, 2, 10, // tri0
                      0, 0, 0,  // tri1
                      0, 0, 0,  // tri2
                      0, 0, 0   // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x040, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_MIRROR, 0, 0),
    gsDPSetTileSize(0, 0, 0, 60, 60),
    gsSPNTrianglesInit_5b(2,         // tri count
                          3, 1, 11,  // tri0
                          1, 13, 12, // tri1
                          0, 0, 0    // tri2
                          ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x000, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPNTrianglesInit_5b(2,          // tri count
                          14, 15, 16, // tri0
                          16, 15, 17, // tri1
                          0, 0, 0     // tri2
                          ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x040, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_MIRROR, 0, 0),
    gsDPSetTileSize(0, 0, 0, 60, 60),
    gsSPNTrianglesInit_5b(1,          // tri count
                          18, 19, 20, // tri0
                          0, 0, 0,    // tri1
                          0, 0, 0     // tri2
                          ),
    gsSPEndDisplayList(),
};

static Gfx Rarm2_rhn_model[] = {
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 10.0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&rhn_1_v[237], 5, 0),
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 11.0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x340, G_IM_FMT_CI, 16, 16, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetTileSize(0, 0, 0, 124, 60),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&rhn_1_v[242], 18, 5),
    gsSPNTrianglesInit_5b(8,       // tri count
                          3, 5, 6, // tri0
                          3, 0, 7, // tri1
                          3, 8, 9  // tri2
                          ),
    gsSPNTriangles_5b(0, 1, 10,  // tri0
                      2, 12, 11, // tri1
                      3, 13, 4,  // tri2
                      2, 4, 14   // tri3
                      ),
    gsSPNTriangles_5b(1, 16, 15, // tri0
                      0, 0, 0,   // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPNTrianglesInit_5b(2,          // tri count
                          17, 18, 19, // tri0
                          20, 21, 22, // tri1
                          0, 0, 0     // tri2
                          ),
    gsSPEndDisplayList(),
};

static Gfx Rarm1_rhn_model[] = {
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 7, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&rhn_1_v[260], 4, 0),
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 10.0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x340, G_IM_FMT_CI, 16, 16, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetTileSize(0, 0, 0, 124, 60),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&rhn_1_v[264], 11, 4),
    gsSPNTrianglesInit_5b(7,       // tri count
                          0, 4, 5, // tri0
                          0, 1, 6, // tri1
                          3, 0, 7  // tri2
                          ),
    gsSPNTriangles_5b(3, 8, 9,   // tri0
                      1, 10, 11, // tri1
                      2, 3, 12,  // tri2
                      3, 13, 14  // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Larm2_rhn_model[] = {
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 8, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&rhn_1_v[203], 4, 0),
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 9, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x340, G_IM_FMT_CI, 16, 16, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetTileSize(0, 0, 0, 124, 60),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&rhn_1_v[207], 16, 4),
    gsSPNTrianglesInit_5b(8,       // tri count
                          2, 3, 4, // tri0
                          0, 2, 5, // tri1
                          0, 6, 7  // tri2
                          ),
    gsSPNTriangles_5b(3, 1, 8,   // tri0
                      1, 0, 9,   // tri1
                      0, 10, 11, // tri2
                      3, 12, 13  // tri3
                      ),
    gsSPNTriangles_5b(3, 14, 15, // tri0
                      0, 0, 0,   // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPNTrianglesInit_5b(2,          // tri count
                          16, 17, 18, // tri0
                          17, 16, 19, // tri1
                          0, 0, 0     // tri2
                          ),
    gsSPEndDisplayList(),
};

static Gfx Larm1_rhn_model[] = {
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 7, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&rhn_1_v[223], 3, 0),
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 8, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x340, G_IM_FMT_CI, 16, 16, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetTileSize(0, 0, 0, 124, 60),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&rhn_1_v[226], 11, 3),
    gsSPNTrianglesInit_5b(7,       // tri count
                          0, 1, 3, // tri0
                          0, 5, 4, // tri1
                          2, 7, 6  // tri2
                          ),
    gsSPNTriangles_5b(2, 0, 8,   // tri0
                      1, 2, 9,   // tri1
                      1, 10, 11, // tri2
                      1, 12, 13  // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx chest_rhn_model[] = {
    gsSPMatrix(anime_6_model, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&rhn_1_v[275], 7, 0),
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 7, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_3_txt, G_IM_FMT_CI, 32, 32, 14, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetTileSize(0, 0, 0, 252, 124),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&rhn_1_v[282], 24, 7),
    gsSPNTrianglesInit_5b(14,       // tri count
                          6, 5, 7,  // tri0
                          6, 9, 8,  // tri1
                          0, 10, 11 // tri2
                          ),
    gsSPNTriangles_5b(0, 12, 13, // tri0
                      3, 14, 15, // tri1
                      5, 2, 16,  // tri2
                      5, 17, 18  // tri3
                      ),
    gsSPNTriangles_5b(4, 19, 1,  // tri0
                      4, 20, 21, // tri1
                      1, 22, 23, // tri2
                      2, 24, 25  // tri3
                      ),
    gsSPNTriangles_5b(3, 6, 26,  // tri0
                      5, 27, 28, // tri1
                      2, 0, 29,  // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPNTrianglesInit_5b(1,        // tri count
                          0, 1, 30, // tri0
                          0, 0, 0,  // tri1
                          0, 0, 0   // tri2
                          ),
    gsSPEndDisplayList(),
};

static Gfx tail1_rhn_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x3C0, G_IM_FMT_CI, 16, 8, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&rhn_1_v[328], 8, 0),
    gsSPNTrianglesInit_5b(6,       // tri count
                          0, 1, 2, // tri0
                          2, 3, 0, // tri1
                          4, 5, 6  // tri2
                          ),
    gsSPNTriangles_5b(4, 7, 1, // tri0
                      6, 7, 4, // tri1
                      1, 0, 4, // tri2
                      0, 0, 0  // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx tail2_rhn_model[] = {
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 5, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&rhn_1_v[306], 4, 0),
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 6, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x3C0, G_IM_FMT_CI, 16, 8, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&rhn_1_v[310], 18, 4),
    gsSPNTrianglesInit_5b(6,       // tri count
                          1, 4, 5, // tri0
                          1, 0, 6, // tri1
                          3, 7, 8  // tri2
                          ),
    gsSPNTriangles_5b(2, 9, 10, // tri0
                      3, 2, 11, // tri1
                      0, 3, 12, // tri2
                      0, 0, 0   // tri3
                      ),
    gsSPNTrianglesInit_5b(9,          // tri count
                          13, 14, 15, // tri0
                          16, 17, 18, // tri1
                          14, 17, 15  // tri2
                          ),
    gsSPNTriangles_5b(19, 20, 14, // tri0
                      18, 21, 16, // tri1
                      19, 13, 17, // tri2
                      17, 13, 15  // tri3
                      ),
    gsSPNTriangles_5b(14, 13, 19, // tri0
                      17, 16, 19, // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Rfoot2_rhn_model[] = {
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 3, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&rhn_1_v[388], 4, 0),
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 4, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x340, G_IM_FMT_CI, 16, 16, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetTileSize(0, 0, 0, 124, 60),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&rhn_1_v[392], 17, 4),
    gsSPNTrianglesInit_5b(8,       // tri count
                          2, 4, 5, // tri0
                          2, 1, 6, // tri1
                          1, 0, 7  // tri2
                          ),
    gsSPNTriangles_5b(0, 8, 9,  // tri0
                      0, 3, 10, // tri1
                      3, 2, 11, // tri2
                      0, 13, 12 // tri3
                      ),
    gsSPNTriangles_5b(2, 15, 14, // tri0
                      0, 0, 0,   // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPNTrianglesInit_5b(4,          // tri count
                          16, 17, 18, // tri0
                          18, 19, 16, // tri1
                          17, 16, 20  // tri2
                          ),
    gsSPNTriangles_5b(20, 18, 17, // tri0
                      0, 0, 0,    // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Rfoot1_rhn_model[] = {
    gsSPMatrix(anime_6_model, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&rhn_1_v[409], 5, 0),
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 3, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_3_txt, G_IM_FMT_CI, 32, 32, 14, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetTileSize(0, 0, 0, 252, 124),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&rhn_1_v[414], 20, 5),
    gsSPNTrianglesInit_5b(7,       // tri count
                          2, 6, 5, // tri0
                          3, 2, 7, // tri1
                          0, 8, 9  // tri2
                          ),
    gsSPNTriangles_5b(1, 0, 10, // tri0
                      0, 4, 11, // tri1
                      1, 12, 2, // tri2
                      1, 13, 14 // tri3
                      ),
    gsSPNTrianglesInit_5b(1,          // tri count
                          15, 16, 17, // tri0
                          0, 0, 0,    // tri1
                          0, 0, 0     // tri2
                          ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x340, G_IM_FMT_CI, 16, 16, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetTileSize(0, 0, 0, 124, 60),
    gsSPNTrianglesInit_5b(7,          // tri count
                          18, 19, 20, // tri0
                          21, 20, 22, // tri1
                          18, 23, 24  // tri2
                          ),
    gsSPNTriangles_5b(22, 23, 21, // tri0
                      22, 24, 23, // tri1
                      24, 19, 18, // tri2
                      20, 21, 18  // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Lfoot2_rhn_model[] = {
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 1, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&rhn_1_v[336], 5, 0),
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 2, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x340, G_IM_FMT_CI, 16, 16, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetTileSize(0, 0, 0, 124, 60),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&rhn_1_v[341], 19, 5),
    gsSPNTrianglesInit_5b(8,       // tri count
                          3, 2, 5, // tri0
                          2, 1, 6, // tri1
                          2, 8, 7  // tri2
                          ),
    gsSPNTriangles_5b(1, 0, 9,   // tri0
                      2, 10, 11, // tri1
                      0, 12, 13, // tri2
                      0, 4, 14   // tri3
                      ),
    gsSPNTriangles_5b(0, 16, 15, // tri0
                      0, 0, 0,   // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPNTrianglesInit_5b(4,          // tri count
                          17, 18, 19, // tri0
                          20, 21, 18, // tri1
                          18, 22, 20  // tri2
                          ),
    gsSPNTriangles_5b(20, 22, 23, // tri0
                      0, 0, 0,    // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Lfoot1_rhn_model[] = {
    gsSPMatrix(anime_6_model, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&rhn_1_v[360], 6, 0),
    gsSPMatrix(anime_6_model + sizeof(Mtx) * 1, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_3_txt, G_IM_FMT_CI, 32, 32, 14, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetTileSize(0, 0, 0, 252, 124),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&rhn_1_v[366], 22, 6),
    gsSPNTrianglesInit_5b(7,       // tri count
                          1, 7, 6, // tri0
                          0, 3, 8, // tri1
                          3, 9, 10 // tri2
                          ),
    gsSPNTriangles_5b(3, 4, 11, // tri0
                      5, 2, 12, // tri1
                      0, 13, 1, // tri2
                      0, 14, 15 // tri3
                      ),
    gsSPNTrianglesInit_5b(1,          // tri count
                          16, 17, 18, // tri0
                          0, 0, 0,    // tri1
                          0, 0, 0     // tri2
                          ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x340, G_IM_FMT_CI, 16, 16, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetTileSize(0, 0, 0, 124, 60),
    gsSPNTrianglesInit_5b(7,          // tri count
                          19, 20, 21, // tri0
                          22, 23, 24, // tri1
                          19, 21, 25  // tri2
                          ),
    gsSPNTriangles_5b(24, 26, 22, // tri0
                      27, 19, 25, // tri1
                      25, 23, 27, // tri2
                      23, 22, 27  // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx base_rhn_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_3_txt, G_IM_FMT_CI, 32, 32, 14, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetTileSize(0, 0, 0, 252, 124),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&rhn_1_v[434], 14, 0),
    gsSPNTrianglesInit_5b(12,      // tri count
                          0, 1, 2, // tri0
                          3, 4, 5, // tri1
                          3, 6, 7  // tri2
                          ),
    gsSPNTriangles_5b(5, 0, 2, // tri0
                      8, 9, 1, // tri1
                      4, 0, 5, // tri2
                      10, 1, 9 // tri3
                      ),
    gsSPNTriangles_5b(11, 9, 12, // tri0
                      0, 8, 1,   // tri1
                      11, 10, 9, // tri2
                      7, 13, 3   // tri3
                      ),
    gsSPNTriangles_5b(5, 6, 3, // tri0
                      0, 0, 0, // tri1
                      0, 0, 0, // tri2
                      0, 0, 0  // tri3
                      ),
    gsSPEndDisplayList(),
};

// clang-format off
static cKF_Joint_R_c cKF_je_r_rhn_1_tbl[] = {
    /* joint 0 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 0,1000,0 },
    /* joint 1 */ { base_rhn_model, 4, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 2 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 0,-450,0 },
    /* joint 3 */ { Lfoot1_rhn_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 4 */ { Lfoot2_rhn_model, 1, cKF_JOINT_FLAG_DISP_OPA, 500,0,0 },
    /* joint 5 */ { NULL, 0, cKF_JOINT_FLAG_DISP_OPA, 400,0,0 },
    /* joint 6 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 0,450,0 },
    /* joint 7 */ { Rfoot1_rhn_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 8 */ { Rfoot2_rhn_model, 1, cKF_JOINT_FLAG_DISP_OPA, 500,0,0 },
    /* joint 9 */ { NULL, 0, cKF_JOINT_FLAG_DISP_OPA, 400,0,0 },
    /* joint 10 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 75,0,-725 },
    /* joint 11 */ { tail1_rhn_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 12 */ { tail2_rhn_model, 0, cKF_JOINT_FLAG_DISP_OPA, 400,0,0 },
    /* joint 13 */ { chest_rhn_model, 3, cKF_JOINT_FLAG_DISP_OPA, 329,0,0 },
    /* joint 14 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 625,-850,100 },
    /* joint 15 */ { Larm1_rhn_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 16 */ { Larm2_rhn_model, 0, cKF_JOINT_FLAG_DISP_OPA, 450,0,0 },
    /* joint 17 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 625,850,100 },
    /* joint 18 */ { Rarm1_rhn_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 19 */ { Rarm2_rhn_model, 1, cKF_JOINT_FLAG_DISP_OPA, 450,0,0 },
    /* joint 20 */ { NULL, 0, cKF_JOINT_FLAG_DISP_OPA, 450,0,0 },
    /* joint 21 */ { NULL, 2, cKF_JOINT_FLAG_DISP_OPA, 883,0,0 },
    /* joint 22 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 450,0,600 },
    /* joint 23 */ { mouth_rhn_model, 0, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 24 */ { head_rhn_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 25 */ { NULL, 0, cKF_JOINT_FLAG_DISP_OPA, 1200,0,0 },
};
// clang-format on

extern cKF_Skeleton_R_c cKF_bs_r_rhn_1 = { 26, 14, cKF_je_r_rhn_1_tbl };
