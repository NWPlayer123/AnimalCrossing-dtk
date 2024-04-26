#include "libforest/gbi_extensions.h"

// clang-format off

static Vtx tmpr_v[] = { }; // TODO: include asset

extern u8 earth_tex_dummy[];
extern u8 river_tex_dummy[];
extern u8 grass_tex_dummy[];
extern u8 water_1_tex_dummy[];
extern u8 water_2_tex_dummy[];
extern u8 bridge_2_tex_dummy[];
extern u16 bridge_2_pal_dummy[];

Gfx tmpr_modelT[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, 1, 0, TEXEL0, TEXEL1, COMBINED, 0, SHADE, TEXEL0, COMBINED, 0, PRIM_LOD_FRAC, PRIMITIVE),
    gsDPSetPrimColor(0, 50, 255, 255, 255, 50),
    gsDPSetEnvColor(0, 100, 255, 255),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_ZB_XLU_SURF2),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock_4b(water_1_tex_dummy, G_IM_FMT_I, 32, 32, 15, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPLoadMultiBlock_4b(water_2_tex_dummy, 0x0040, 1, G_IM_FMT_I, 32, 32, 15, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPDisplayList(0x08000000),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&tmpr_v[152], 22, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 3, 1, 0),
    gsSP2Triangles(4, 0, 5, 0, 0, 2, 5, 0),
    gsSP2Triangles(6, 4, 7, 0, 4, 5, 7, 0),
    gsSP2Triangles(8, 9, 10, 0, 11, 8, 10, 0),
    gsSP2Triangles(12, 11, 10, 0, 10, 9, 13, 0),
    gsSP2Triangles(14, 12, 10, 0, 14, 10, 15, 0),
    gsSP2Triangles(7, 14, 15, 0, 6, 7, 15, 0),
    gsSP2Triangles(16, 6, 15, 0, 17, 16, 15, 0),
    gsSP2Triangles(18, 17, 15, 0, 18, 15, 10, 0),
    gsSP2Triangles(18, 10, 13, 0, 19, 18, 13, 0),
    gsSP2Triangles(19, 13, 20, 0, 21, 19, 20, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPLoadTLUT_pal16(15, 0x80FD90E0),
    gsDPLoadTextureBlock_4b(bridge_2_tex_dummy, G_IM_FMT_CI, 128, 32, 15, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_WRAP, 7, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPVertex(&tmpr_v[174], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(1, 4, 5, 0, 1, 5, 2, 0),
    gsSP2Triangles(6, 7, 8, 0, 6, 8, 9, 0),
    gsSP2Triangles(9, 8, 10, 0, 9, 10, 11, 0),
    gsSP2Triangles(8, 12, 13, 0, 8, 13, 10, 0),
    gsSP2Triangles(14, 15, 16, 0, 14, 16, 17, 0),
    gsSP2Triangles(18, 14, 17, 0, 18, 17, 19, 0),
    gsSP2Triangles(20, 21, 14, 0, 20, 14, 18, 0),
    gsSP2Triangles(22, 23, 24, 0, 22, 24, 25, 0),
    gsSP2Triangles(25, 24, 26, 0, 25, 26, 27, 0),
    gsSP2Triangles(24, 28, 29, 0, 24, 29, 26, 0),
    gsSPVertex(&tmpr_v[204], 8, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 0, 3, 0, 4, 3, 5, 0),
    gsSP2Triangles(6, 7, 0, 0, 6, 0, 4, 0),
    gsSPEndDisplayList(),
};

Gfx tmpr_model[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPLoadTLUT_pal16(15, 0x09000000),
    gsDPLoadTextureBlock_4b(earth_tex_dummy, G_IM_FMT_CI, 64, 64, 15, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&tmpr_v[0], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 4, 0),
    gsSP2Triangles(5, 3, 6, 0, 6, 7, 5, 0),
    gsSP2Triangles(8, 9, 7, 0, 8, 10, 11, 0),
    gsSP2Triangles(8, 11, 9, 0, 8, 12, 10, 0),
    gsSP2Triangles(12, 13, 10, 0, 14, 13, 15, 0),
    gsSP2Triangles(16, 14, 17, 0, 18, 16, 19, 0),
    gsSP2Triangles(19, 20, 18, 0, 20, 21, 18, 0),
    gsSP2Triangles(22, 21, 23, 0, 23, 24, 22, 0),
    gsSP2Triangles(25, 22, 24, 0, 25, 26, 22, 0),
    gsSP2Triangles(27, 26, 28, 0, 29, 30, 31, 0),
    gsSPVertex(&tmpr_v[32], 9, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 4, 0),
    gsSP2Triangles(5, 3, 6, 0, 7, 5, 8, 0),
    gsDPPipeSync(),
    gsDPLoadTLUT_pal16(15, 0x09000000),
    gsDPLoadTextureBlock_4b(river_tex_dummy, G_IM_FMT_CI, 64, 32, 15, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_CLAMP, 6, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPVertex(&tmpr_v[41], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 3, 1, 0),
    gsSP2Triangles(4, 5, 6, 0, 5, 7, 6, 0),
    gsSP2Triangles(8, 9, 10, 0, 9, 11, 10, 0),
    gsSP2Triangles(12, 13, 14, 0, 13, 15, 16, 0),
    gsSP2Triangles(12, 15, 13, 0, 15, 17, 16, 0),
    gsSP2Triangles(18, 19, 17, 0, 15, 18, 17, 0),
    gsSP2Triangles(18, 20, 19, 0, 20, 21, 19, 0),
    gsSP2Triangles(22, 23, 21, 0, 22, 24, 23, 0),
    gsSP2Triangles(24, 25, 26, 0, 25, 27, 26, 0),
    gsSP2Triangles(28, 29, 27, 0, 25, 28, 27, 0),
    gsSP2Triangles(30, 31, 29, 0, 29, 28, 30, 0),
    gsSPVertex(&tmpr_v[73], 27, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 1, 0),
    gsSP2Triangles(1, 0, 3, 0, 5, 6, 4, 0),
    gsSP2Triangles(3, 5, 4, 0, 6, 7, 8, 0),
    gsSP2Triangles(7, 9, 8, 0, 10, 11, 9, 0),
    gsSP2Triangles(10, 12, 11, 0, 13, 14, 15, 0),
    gsSP2Triangles(16, 17, 18, 0, 17, 16, 19, 0),
    gsSP2Triangles(16, 20, 19, 0, 21, 22, 23, 0),
    gsSP2Triangles(24, 25, 22, 0, 22, 21, 24, 0),
    gsSP1Triangle(26, 5, 3, 0),
    gsDPPipeSync(),
    gsDPLoadTLUT_pal16(15, 0x09000000),
    gsDPLoadTextureBlock_4b(grass_tex_dummy, G_IM_FMT_CI, 32, 32, 15, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPVertex(&tmpr_v[100], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
    gsSP2Triangles(4, 5, 3, 0, 5, 2, 3, 0),
    gsSP2Triangles(0, 6, 1, 0, 7, 8, 9, 0),
    gsSP2Triangles(7, 10, 8, 0, 10, 11, 8, 0),
    gsSP2Triangles(10, 12, 11, 0, 13, 14, 9, 0),
    gsSP2Triangles(12, 15, 13, 0, 12, 16, 15, 0),
    gsSP2Triangles(16, 17, 15, 0, 16, 18, 17, 0),
    gsSP2Triangles(16, 19, 18, 0, 19, 20, 21, 0),
    gsSP2Triangles(19, 21, 18, 0, 19, 22, 20, 0),
    gsSP2Triangles(19, 23, 22, 0, 23, 24, 25, 0),
    gsSP2Triangles(23, 25, 22, 0, 26, 27, 28, 0),
    gsSP2Triangles(27, 29, 28, 0, 29, 30, 28, 0),
    gsSP1Triangle(27, 31, 29, 0),
    gsSPVertex(&tmpr_v[132], 20, 0),
    gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
    gsSP2Triangles(1, 4, 3, 0, 1, 5, 4, 0),
    gsSP2Triangles(5, 6, 4, 0, 6, 7, 4, 0),
    gsSP2Triangles(5, 8, 6, 0, 9, 10, 7, 0),
    gsSP2Triangles(11, 12, 13, 0, 12, 14, 13, 0),
    gsSP2Triangles(12, 15, 14, 0, 12, 16, 15, 0),
    gsSP2Triangles(14, 17, 13, 0, 17, 18, 13, 0),
    gsSP1Triangle(17, 19, 18, 0),
    gsSPEndDisplayList(),
};

// clang-format on
