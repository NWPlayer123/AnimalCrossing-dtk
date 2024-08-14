#include "c_keyframe.h"
#include "libforest/gbi_extensions.h"

static Vtx owl_1_v[] = {
#include "assets/npc/mdl/owl_1_v.inc"
};

static Gfx head_owl_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*8, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&owl_1_v[14], 9, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*14, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x140, G_IM_FMT_CI, 32, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&owl_1_v[23], 21, 9),
    gsSPNTrianglesInit_5b(10,        // tri count
                          0, 1, 9,   // tri0
                          1, 11, 10, // tri1
                          4, 2, 12   // tri2
                          ),
    gsSPNTriangles_5b(2, 14, 13, // tri0
                      7, 0, 15,  // tri1
                      0, 17, 16, // tri2
                      1, 4, 18   // tri3
                      ),
    gsSPNTriangles_5b(4, 20, 19, // tri0
                      5, 7, 21,  // tri1
                      7, 23, 22, // tri2
                      0, 0, 0    // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x1C0, G_IM_FMT_CI, 32, 8, 15, GX_MIRROR, GX_CLAMP, 0, 0),
    gsDPSetTileSize(0, 0, 0, 252, 28),
    gsDPSetPrimColor(0, 128, 255, 250, 255, 255),
    gsSPNTrianglesInit_5b(4,        // tri count
                          3, 8, 24, // tri0
                          8, 6, 25, // tri1
                          6, 27, 26 // tri2
                          ),
    gsSPNTriangles_5b(8, 29, 28, // tri0
                      0, 0, 0,   // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x0C0, G_IM_FMT_CI, 32, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&owl_1_v[44], 30, 0),
    gsSPNTrianglesInit_5b(2,       // tri count
                          0, 1, 2, // tri0
                          2, 3, 0, // tri1
                          0, 0, 0  // tri2
                          ),
    gsSPNTrianglesInit_5b(2,       // tri count
                          4, 5, 6, // tri0
                          6, 7, 4, // tri1
                          0, 0, 0  // tri2
                          ),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPLoadTextureBlock_4b_Dolphin(anime_1_txt, G_IM_FMT_CI, 32, 16, 15, GX_MIRROR, GX_CLAMP, 0, 0),
    gsDPSetTileSize(0, 0, 0, 252, 60),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPNTrianglesInit_5b(24,         // tri count
                          8, 9, 10,   // tri0
                          11, 12, 13, // tri1
                          11, 13, 14  // tri2
                          ),
    gsSPNTriangles_5b(15, 14, 13, // tri0
                      12, 16, 17, // tri1
                      12, 17, 13, // tri2
                      18, 19, 20  // tri3
                      ),
    gsSPNTriangles_5b(17, 21, 8,  // tri0
                      12, 11, 22, // tri1
                      20, 23, 16, // tri2
                      24, 25, 23  // tri3
                      ),
    gsSPNTriangles_5b(26, 9, 8,   // tri0
                      16, 12, 20, // tri1
                      8, 13, 17,  // tri2
                      21, 27, 28  // tri3
                      ),
    gsSPNTriangles_5b(20, 19, 29, // tri0
                      20, 29, 24, // tri1
                      28, 26, 8,  // tri2
                      28, 8, 21   // tri3
                      ),
    gsSPNTriangles_5b(23, 20, 24, // tri0
                      10, 13, 8,  // tri1
                      20, 12, 18, // tri2
                      22, 18, 12  // tri3
                      ),
    gsSPNTriangles_5b(13, 10, 15, // tri0
                      0, 0, 0,    // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x140, G_IM_FMT_CI, 32, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPVertex(&owl_1_v[74], 32, 0),
    gsSPNTrianglesInit_5b(46,      // tri count
                          0, 1, 2, // tri0
                          0, 2, 3, // tri1
                          4, 5, 6  // tri2
                          ),
    gsSPNTriangles_5b(4, 6, 7,   // tri0
                      8, 9, 10,  // tri1
                      11, 2, 12, // tri2
                      8, 6, 13   // tri3
                      ),
    gsSPNTriangles_5b(14, 15, 16, // tri0
                      17, 18, 16, // tri1
                      2, 1, 19,   // tri2
                      19, 1, 7    // tri3
                      ),
    gsSPNTriangles_5b(19, 7, 20,  // tri0
                      15, 14, 21, // tri1
                      22, 23, 14, // tri2
                      19, 24, 17  // tri3
                      ),
    gsSPNTriangles_5b(20, 7, 6,   // tri0
                      25, 26, 12, // tri1
                      14, 18, 22, // tri2
                      20, 8, 27   // tri3
                      ),
    gsSPNTriangles_5b(18, 17, 24, // tri0
                      24, 19, 20, // tri1
                      24, 20, 28, // tri2
                      23, 22, 28  // tri3
                      ),
    gsSPNTriangles_5b(22, 24, 28, // tri0
                      24, 22, 18, // tri1
                      28, 27, 23, // tri2
                      29, 21, 14  // tri3
                      ),
    gsSPNTriangles_5b(16, 18, 14, // tri0
                      14, 23, 29, // tri1
                      27, 10, 29, // tri2
                      13, 9, 8    // tri3
                      ),
    gsSPNTriangles_5b(12, 26, 11, // tri0
                      6, 8, 20,   // tri1
                      19, 12, 2,  // tri2
                      29, 23, 27  // tri3
                      ),
    gsSPNTriangles_5b(16, 25, 17, // tri0
                      12, 17, 25, // tri1
                      17, 12, 19, // tri2
                      27, 28, 20  // tri3
                      ),
    gsSPNTriangles_5b(10, 27, 8, // tri0
                      4, 7, 1,   // tri1
                      4, 1, 0,   // tri2
                      3, 2, 11   // tri3
                      ),
    gsSPNTriangles_5b(11, 30, 3, // tri0
                      13, 6, 5,  // tri1
                      5, 31, 13, // tri2
                      0, 0, 0    // tri3
                      ),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x000, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPVertex(&owl_1_v[106], 17, 0),
    gsSPNTrianglesInit_5b(1,       // tri count
                          0, 1, 2, // tri0
                          0, 0, 0, // tri1
                          0, 0, 0  // tri2
                          ),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x040, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPNTrianglesInit_5b(3,       // tri count
                          3, 4, 5, // tri0
                          5, 6, 3, // tri1
                          3, 7, 4  // tri2
                          ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x1C0, G_IM_FMT_CI, 32, 8, 15, GX_MIRROR, GX_CLAMP, 0, 0),
    gsDPSetTileSize(0, 0, 0, 252, 28),
    gsDPSetPrimColor(0, 128, 255, 250, 255, 255),
    gsSPNTrianglesInit_5b(7,          // tri count
                          8, 9, 10,   // tri0
                          11, 12, 13, // tri1
                          14, 15, 9   // tri2
                          ),
    gsSPNTriangles_5b(14, 9, 8,   // tri0
                      11, 13, 14, // tri1
                      16, 12, 11, // tri2
                      14, 13, 15  // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx mouth_owl_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*14, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&owl_1_v[0], 6, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*13, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x000, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&owl_1_v[6], 8, 6),
    gsSPNTrianglesInit_5b(1,       // tri count
                          2, 3, 6, // tri0
                          0, 0, 0, // tri1
                          0, 0, 0  // tri2
                          ),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x040, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPNTrianglesInit_5b(5,       // tri count
                          1, 5, 7, // tri0
                          4, 0, 8, // tri1
                          0, 1, 9  // tri2
                          ),
    gsSPNTriangles_5b(1, 10, 11, // tri0
                      0, 12, 13, // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Rarm2_owl_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*11, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&owl_1_v[174], 6, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*12, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x240, G_IM_FMT_CI, 32, 24, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&owl_1_v[180], 25, 6),
    gsSPNTrianglesInit_5b(9,       // tri count
                          4, 6, 7, // tri0
                          0, 2, 8, // tri1
                          5, 4, 9  // tri2
                          ),
    gsSPNTriangles_5b(3, 5, 10,  // tri0
                      0, 11, 12, // tri1
                      4, 1, 13,  // tri2
                      0, 14, 15  // tri3
                      ),
    gsSPNTriangles_5b(3, 16, 17, // tri0
                      4, 18, 19, // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPNTrianglesInit_5b(1,          // tri count
                          20, 21, 22, // tri0
                          0, 0, 0,    // tri1
                          0, 0, 0     // tri2
                          ),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPNTrianglesInit_5b(3,          // tri count
                          23, 24, 25, // tri0
                          26, 27, 23, // tri1
                          23, 25, 26  // tri2
                          ),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPNTrianglesInit_5b(1,          // tri count
                          28, 29, 30, // tri0
                          0, 0, 0,    // tri1
                          0, 0, 0     // tri2
                          ),
    gsSPVertex(&owl_1_v[205], 6, 0),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPNTrianglesInit_5b(1,       // tri count
                          0, 1, 2, // tri0
                          0, 0, 0, // tri1
                          0, 0, 0  // tri2
                          ),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPNTrianglesInit_5b(1,       // tri count
                          3, 4, 5, // tri0
                          0, 0, 0, // tri1
                          0, 0, 0  // tri2
                          ),
    gsSPEndDisplayList(),
};

static Gfx Rarm1_owl_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*8, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&owl_1_v[211], 3, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*11, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x240, G_IM_FMT_CI, 32, 24, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&owl_1_v[214], 11, 3),
    gsSPNTrianglesInit_5b(7,       // tri count
                          1, 2, 3, // tri0
                          0, 4, 5, // tri1
                          2, 6, 7  // tri2
                          ),
    gsSPNTriangles_5b(1, 8, 9,   // tri0
                      2, 10, 11, // tri1
                      0, 1, 12,  // tri2
                      2, 0, 13   // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Larm2_owl_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*9, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&owl_1_v[123], 6, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*10, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x240, G_IM_FMT_CI, 32, 24, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&owl_1_v[129], 25, 6),
    gsSPNTrianglesInit_5b(9,       // tri count
                          4, 6, 7, // tri0
                          2, 0, 8, // tri1
                          4, 5, 9  // tri2
                          ),
    gsSPNTriangles_5b(5, 3, 10,  // tri0
                      0, 11, 12, // tri1
                      1, 4, 13,  // tri2
                      0, 14, 15  // tri3
                      ),
    gsSPNTriangles_5b(3, 16, 17, // tri0
                      4, 18, 19, // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPNTrianglesInit_5b(1,          // tri count
                          20, 21, 22, // tri0
                          0, 0, 0,    // tri1
                          0, 0, 0     // tri2
                          ),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPNTrianglesInit_5b(3,          // tri count
                          23, 24, 25, // tri0
                          25, 26, 27, // tri1
                          27, 23, 25  // tri2
                          ),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPNTrianglesInit_5b(1,          // tri count
                          28, 29, 30, // tri0
                          0, 0, 0,    // tri1
                          0, 0, 0     // tri2
                          ),
    gsSPVertex(&owl_1_v[154], 6, 0),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPNTrianglesInit_5b(1,       // tri count
                          0, 1, 2, // tri0
                          0, 0, 0, // tri1
                          0, 0, 0  // tri2
                          ),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPNTrianglesInit_5b(1,       // tri count
                          3, 4, 5, // tri0
                          0, 0, 0, // tri1
                          0, 0, 0  // tri2
                          ),
    gsSPEndDisplayList(),
};

static Gfx Larm1_owl_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*8, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&owl_1_v[160], 3, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*9, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x240, G_IM_FMT_CI, 32, 24, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&owl_1_v[163], 11, 3),
    gsSPNTrianglesInit_5b(7,       // tri count
                          2, 1, 3, // tri0
                          0, 4, 5, // tri1
                          2, 6, 7  // tri2
                          ),
    gsSPNTriangles_5b(1, 8, 9,   // tri0
                      2, 10, 11, // tri1
                      1, 0, 12,  // tri2
                      0, 2, 13   // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx chest_owl_model[] = {
    gsSPMatrix(anime_6_model, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&owl_1_v[225], 9, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*8, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x3C0, G_IM_FMT_CI, 32, 24, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 252, 255, 255),
    gsSPVertex(&owl_1_v[234], 23, 9),
    gsSPNTrianglesInit_5b(14,       // tri count
                          3, 8, 9,  // tri0
                          7, 2, 10, // tri1
                          7, 11, 12 // tri2
                          ),
    gsSPNTriangles_5b(6, 13, 14, // tri0
                      4, 15, 16, // tri1
                      2, 5, 17,  // tri2
                      6, 18, 19  // tri3
                      ),
    gsSPNTriangles_5b(0, 20, 21, // tri0
                      1, 22, 23, // tri1
                      8, 24, 25, // tri2
                      0, 7, 26   // tri3
                      ),
    gsSPNTriangles_5b(1, 6, 27,  // tri0
                      2, 28, 29, // tri1
                      3, 30, 31, // tri2
                      0, 0, 0    // tri3
                      ),
    gsSPVertex(&owl_1_v[257], 5, 9),
    gsSPNTrianglesInit_5b(4,         // tri count
                          4, 3, 9,   // tri0
                          5, 10, 11, // tri1
                          8, 1, 12   // tri2
                          ),
    gsSPNTriangles_5b(6, 0, 13, // tri0
                      0, 0, 0,  // tri1
                      0, 0, 0,  // tri2
                      0, 0, 0   // tri3
                      ),
    gsSPVertex(&owl_1_v[262], 21, 0),
    gsSPNTrianglesInit_5b(19,      // tri count
                          0, 1, 2, // tri0
                          3, 4, 5, // tri1
                          0, 6, 7  // tri2
                          ),
    gsSPNTriangles_5b(8, 9, 10,  // tri0
                      11, 8, 10, // tri1
                      10, 9, 12, // tri2
                      13, 12, 14 // tri3
                      ),
    gsSPNTriangles_5b(10, 12, 13, // tri0
                      2, 15, 0,   // tri1
                      16, 8, 17,  // tri2
                      14, 12, 15  // tri3
                      ),
    gsSPNTriangles_5b(18, 17, 11, // tri0
                      13, 14, 19, // tri1
                      1, 0, 7,    // tri2
                      20, 5, 4    // tri3
                      ),
    gsSPNTriangles_5b(5, 16, 3,  // tri0
                      17, 3, 16, // tri1
                      15, 2, 14, // tri2
                      17, 8, 11  // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx tail1_owl_model[] = {
    gsSPMatrix(anime_6_model, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&owl_1_v[283], 5, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*7, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x540, G_IM_FMT_CI, 16, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&owl_1_v[288], 16, 5),
    gsSPNTrianglesInit_5b(3,       // tri count
                          2, 4, 5, // tri0
                          4, 3, 6, // tri1
                          3, 1, 7  // tri2
                          ),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPNTrianglesInit_5b(2,         // tri count
                          2, 8, 9,   // tri0
                          1, 10, 11, // tri1
                          0, 0, 0    // tri2
                          ),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPNTrianglesInit_5b(2,        // tri count
                          1, 0, 12, // tri0
                          0, 2, 13, // tri1
                          0, 0, 0   // tri2
                          ),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPNTrianglesInit_5b(6,          // tri count
                          14, 15, 16, // tri0
                          16, 15, 17, // tri1
                          16, 18, 14  // tri2
                          ),
    gsSPNTriangles_5b(16, 19, 20, // tri0
                      17, 19, 16, // tri1
                      20, 18, 16, // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Rfoot3_owl_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*5, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&owl_1_v[354], 4, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*6, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x5C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&owl_1_v[358], 20, 4),
    gsSPNTrianglesInit_5b(6,       // tri count
                          1, 4, 5, // tri0
                          1, 6, 3, // tri1
                          3, 7, 8  // tri2
                          ),
    gsSPNTriangles_5b(2, 0, 9,   // tri0
                      0, 3, 10,  // tri1
                      2, 11, 12, // tri2
                      0, 0, 0    // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x600, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPNTrianglesInit_5b(1,          // tri count
                          13, 14, 15, // tri0
                          0, 0, 0,    // tri1
                          0, 0, 0     // tri2
                          ),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPNTrianglesInit_5b(6,          // tri count
                          16, 17, 18, // tri0
                          18, 17, 19, // tri1
                          18, 19, 20  // tri2
                          ),
    gsSPNTriangles_5b(18, 21, 22, // tri0
                      18, 22, 16, // tri1
                      20, 23, 18, // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Rfoot2_owl_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*4, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&owl_1_v[378], 4, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*5, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x5C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&owl_1_v[382], 14, 4),
    gsSPNTrianglesInit_5b(7,       // tri count
                          3, 4, 5, // tri0
                          1, 3, 6, // tri1
                          2, 7, 8  // tri2
                          ),
    gsSPNTriangles_5b(0, 2, 9,   // tri0
                      3, 0, 10,  // tri1
                      3, 11, 12, // tri2
                      1, 13, 14  // tri3
                      ),
    gsSPNTrianglesInit_5b(1,          // tri count
                          15, 16, 17, // tri0
                          0, 0, 0,    // tri1
                          0, 0, 0     // tri2
                          ),
    gsSPEndDisplayList(),
};

static Gfx Rfoot1_owl_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x5C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&owl_1_v[396], 8, 0),
    gsSPNTrianglesInit_5b(6,       // tri count
                          0, 1, 2, // tri0
                          1, 0, 3, // tri1
                          2, 4, 0  // tri2
                          ),
    gsSPNTriangles_5b(5, 6, 4, // tri0
                      3, 7, 1, // tri1
                      4, 2, 5, // tri2
                      0, 0, 0  // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Lfoot3_owl_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*2, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&owl_1_v[304], 4, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*3, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x5C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&owl_1_v[308], 20, 4),
    gsSPNTrianglesInit_5b(6,       // tri count
                          3, 4, 5, // tri0
                          3, 6, 1, // tri1
                          3, 7, 8  // tri2
                          ),
    gsSPNTriangles_5b(0, 2, 9,   // tri0
                      3, 0, 10,  // tri1
                      2, 11, 12, // tri2
                      0, 0, 0    // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x600, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPNTrianglesInit_5b(1,          // tri count
                          13, 14, 15, // tri0
                          0, 0, 0,    // tri1
                          0, 0, 0     // tri2
                          ),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPNTrianglesInit_5b(6,          // tri count
                          16, 17, 18, // tri0
                          19, 17, 16, // tri1
                          20, 19, 16  // tri2
                          ),
    gsSPNTriangles_5b(21, 22, 16, // tri0
                      18, 21, 16, // tri1
                      16, 23, 20, // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx Lfoot2_owl_model[] = {
    gsSPMatrix(anime_6_model+sizeof(Mtx)*1, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&owl_1_v[328], 4, 0),
    gsSPMatrix(anime_6_model+sizeof(Mtx)*2, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x5C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&owl_1_v[332], 14, 4),
    gsSPNTrianglesInit_5b(7,       // tri count
                          3, 4, 5, // tri0
                          3, 1, 6, // tri1
                          2, 7, 8  // tri2
                          ),
    gsSPNTriangles_5b(2, 0, 9,   // tri0
                      0, 3, 10,  // tri1
                      3, 11, 12, // tri2
                      1, 13, 14  // tri3
                      ),
    gsSPNTrianglesInit_5b(1,          // tri count
                          15, 16, 17, // tri0
                          0, 0, 0,    // tri1
                          0, 0, 0     // tri2
                          ),
    gsSPEndDisplayList(),
};

static Gfx Lfoot1_owl_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x5C0, G_IM_FMT_CI, 16, 8, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&owl_1_v[346], 8, 0),
    gsSPNTrianglesInit_5b(6,       // tri count
                          0, 1, 2, // tri0
                          3, 2, 1, // tri1
                          2, 4, 0  // tri2
                          ),
    gsSPNTriangles_5b(4, 5, 6, // tri0
                      1, 7, 3, // tri1
                      6, 0, 4, // tri2
                      0, 0, 0  // tri3
                      ),
    gsSPEndDisplayList(),
};

static Gfx base_owl_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock_4b_Dolphin(anime_4_txt + 0x3C0, G_IM_FMT_CI, 32, 24, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 252, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&owl_1_v[404], 21, 0),
    gsSPNTrianglesInit_5b(23,      // tri count
                          0, 1, 2, // tri0
                          2, 3, 0, // tri1
                          4, 1, 5  // tri2
                          ),
    gsSPNTriangles_5b(4, 6, 7,  // tri0
                      3, 2, 7,  // tri1
                      8, 9, 5,  // tri2
                      5, 10, 11 // tri3
                      ),
    gsSPNTriangles_5b(7, 2, 4,    // tri0
                      12, 13, 14, // tri1
                      15, 16, 17, // tri2
                      10, 5, 9    // tri3
                      ),
    gsSPNTriangles_5b(13, 12, 18, // tri0
                      5, 11, 4,   // tri1
                      7, 18, 3,   // tri2
                      9, 15, 10   // tri3
                      ),
    gsSPNTriangles_5b(4, 11, 19, // tri0
                      6, 13, 7,  // tri1
                      0, 8, 1,   // tri2
                      1, 4, 2    // tri3
                      ),
    gsSPNTriangles_5b(5, 1, 8,    // tri0
                      17, 10, 15, // tri1
                      14, 20, 12, // tri2
                      18, 7, 13   // tri3
                      ),
    gsSPEndDisplayList(),
};

// clang-format off
static cKF_Joint_R_c cKF_je_r_owl_1_tbl[] = {
    /* joint 0 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 0,1000,0 },
    /* joint 1 */ { base_owl_model, 4, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 2 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 0,-400,0 },
    /* joint 3 */ { Lfoot1_owl_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 4 */ { Lfoot2_owl_model, 1, cKF_JOINT_FLAG_DISP_OPA, 500,0,0 },
    /* joint 5 */ { Lfoot3_owl_model, 0, cKF_JOINT_FLAG_DISP_OPA, 400,0,0 },
    /* joint 6 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 0,400,0 },
    /* joint 7 */ { Rfoot1_owl_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 8 */ { Rfoot2_owl_model, 1, cKF_JOINT_FLAG_DISP_OPA, 500,0,0 },
    /* joint 9 */ { Rfoot3_owl_model, 0, cKF_JOINT_FLAG_DISP_OPA, 400,0,0 },
    /* joint 10 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,-1000 },
    /* joint 11 */ { tail1_owl_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 12 */ { NULL, 0, cKF_JOINT_FLAG_DISP_OPA, 650,0,0 },
    /* joint 13 */ { chest_owl_model, 3, cKF_JOINT_FLAG_DISP_OPA, 600,0,0 },
    /* joint 14 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 600,-1400,0 },
    /* joint 15 */ { Larm1_owl_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 16 */ { Larm2_owl_model, 0, cKF_JOINT_FLAG_DISP_OPA, 700,0,0 },
    /* joint 17 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 600,1400,0 },
    /* joint 18 */ { Rarm1_owl_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 19 */ { Rarm2_owl_model, 1, cKF_JOINT_FLAG_DISP_OPA, 700,0,0 },
    /* joint 20 */ { NULL, 0, cKF_JOINT_FLAG_DISP_OPA, 700,0,0 },
    /* joint 21 */ { NULL, 2, cKF_JOINT_FLAG_DISP_OPA, 1000,0,0 },
    /* joint 22 */ { NULL, 1, cKF_JOINT_FLAG_DISP_OPA, 700,0,1750 },
    /* joint 23 */ { mouth_owl_model, 0, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 24 */ { head_owl_model, 1, cKF_JOINT_FLAG_DISP_OPA, 0,0,0 },
    /* joint 25 */ { NULL, 0, cKF_JOINT_FLAG_DISP_OPA, 1300,0,0 },
};
// clang-format on

extern cKF_Skeleton_R_c cKF_bs_r_owl_1 = { 26, 15, cKF_je_r_owl_1_tbl };
