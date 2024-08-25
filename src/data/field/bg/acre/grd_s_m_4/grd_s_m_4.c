#include "libforest/gbi_extensions.h"

extern u8 wave3_tex_dummy[];
extern u8 wave2_tex_dummy[];
extern u8 wave1_tex_dummy[];
extern u8 sand_tex_dummy[];
extern u8 beach_pal_dummy2[];
extern u8 beach2_tex_dummy2[];
extern u8 beach1_tex_dummy2[];
extern u8 bush_pal_dummy[];
extern u8 earth_pal_dummy[];
extern u8 grass_tex_dummy[];
extern u8 bush_b_tex_dummy[];
extern u8 bush_a_tex_dummy[];
extern u8 earth_tex_dummy[];

static Vtx grd_s_m_4_v[] = {
#include "assets/field/bg/grd_s_m_4_v.inc"
};

extern Gfx grd_s_m_4_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, 0, 0, 0, 0, COMBINED, 0, SHADE, 0, 0, 0, 0, COMBINED),
    gsDPSetPrimColor(0, 255, 32, 48, 144, 255),
    gsSPDisplayList(0x0C000000),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPLoadTextureBlock_4b_Dolphin(beach2_tex_dummy2, G_IM_FMT_I, 32, 16, 15, GX_REPEAT, GX_CLAMP, 0, 0),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&grd_s_m_4_v[116], 26, 0),
    gsSPNTrianglesInit_5b(
        30, // tri count
        0, 1, 2, // tri0
        3, 0, 2, // tri1
        3, 2, 4 // tri2
    ),
    gsSPNTriangles_5b(
        3, 4, 5, // tri0
        6, 3, 5, // tri1
        6, 5, 7, // tri2
        6, 7, 8 // tri3
    ),
    gsSPNTriangles_5b(
        9, 6, 8, // tri0
        10, 9, 8, // tri1
        5, 4, 11, // tri2
        5, 11, 12 // tri3
    ),
    gsSPNTriangles_5b(
        7, 5, 12, // tri0
        7, 12, 13, // tri1
        8, 7, 13, // tri2
        8, 13, 14 // tri3
    ),
    gsSPNTriangles_5b(
        8, 14, 15, // tri0
        10, 8, 15, // tri1
        16, 10, 15, // tri2
        2, 17, 18 // tri3
    ),
    gsSPNTriangles_5b(
        2, 18, 19, // tri0
        4, 2, 19, // tri1
        4, 19, 20, // tri2
        4, 20, 11 // tri3
    ),
    gsSPNTriangles_5b(
        1, 21, 22, // tri0
        1, 22, 23, // tri1
        2, 1, 23, // tri2
        2, 23, 17 // tri3
    ),
    gsSPNTriangles_5b(
        21, 24, 22, // tri0
        25, 21, 1, // tri1
        0, 25, 1, // tri2
        0, 0, 0 // tri3
    ),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPLoadTLUT_Dolphin(15, 16, 1, bush_pal_dummy),
    gsDPLoadTextureBlock_4b_Dolphin(bush_a_tex_dummy, G_IM_FMT_CI, 64, 64, 15, GX_REPEAT, GX_CLAMP, 0, 0),
    gsDPSetPrimColor(0, 128, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&grd_s_m_4_v[103], 13, 0),
    gsSPNTrianglesInit_5b(
        6, // tri count
        0, 1, 2, // tri0
        3, 0, 4, // tri1
        5, 3, 6 // tri2
    ),
    gsSPNTriangles_5b(
        7, 5, 8, // tri0
        9, 7, 10, // tri1
        11, 9, 12, // tri2
        0, 0, 0 // tri3
    ),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPLoadTLUT_Dolphin(15, 16, 1, beach_pal_dummy2),
    gsDPLoadTextureBlock_4b_Dolphin(sand_tex_dummy, G_IM_FMT_CI, 64, 32, 15, GX_REPEAT, GX_CLAMP, 0, 0),
    gsSPVertex(&grd_s_m_4_v[0], 20, 0),
    gsSPNTrianglesInit_5b(
        18, // tri count
        0, 1, 2, // tri0
        1, 3, 2, // tri1
        2, 4, 5 // tri2
    ),
    gsSPNTriangles_5b(
        2, 3, 4, // tri0
        5, 6, 7, // tri1
        5, 4, 6, // tri2
        7, 6, 8 // tri3
    ),
    gsSPNTriangles_5b(
        6, 9, 8, // tri0
        9, 10, 8, // tri1
        10, 9, 11, // tri2
        9, 12, 11 // tri3
    ),
    gsSPNTriangles_5b(
        12, 13, 11, // tri0
        13, 14, 11, // tri1
        14, 13, 15, // tri2
        13, 16, 15 // tri3
    ),
    gsSPNTriangles_5b(
        16, 17, 15, // tri0
        17, 16, 18, // tri1
        16, 19, 18, // tri2
        0, 0, 0 // tri3
    ),
    gsDPLoadTLUT_Dolphin(15, 16, 1, earth_pal_dummy),
    gsDPLoadTextureBlock_4b_Dolphin(earth_tex_dummy, G_IM_FMT_CI, 64, 64, 15, GX_REPEAT, GX_CLAMP, 0, 0),
    gsSPVertex(&grd_s_m_4_v[20], 17, 0),
    gsSPNTrianglesInit_5b(
        10, // tri count
        0, 1, 2, // tri0
        0, 3, 4, // tri1
        5, 4, 3 // tri2
    ),
    gsSPNTriangles_5b(
        6, 5, 3, // tri0
        7, 8, 6, // tri1
        9, 10, 7, // tri2
        11, 12, 9 // tri3
    ),
    gsSPNTriangles_5b(
        11, 13, 12, // tri0
        11, 14, 13, // tri1
        14, 15, 16, // tri2
        0, 0, 0 // tri3
    ),
    gsDPLoadTextureBlock_4b_Dolphin(grass_tex_dummy, G_IM_FMT_CI, 32, 32, 15, GX_REPEAT, GX_REPEAT, 0, 0),
    gsSPVertex(&grd_s_m_4_v[37], 31, 0),
    gsSPNTrianglesInit_5b(
        32, // tri count
        0, 1, 2, // tri0
        1, 3, 2, // tri1
        4, 5, 6 // tri2
    ),
    gsSPNTriangles_5b(
        5, 7, 6, // tri0
        5, 8, 7, // tri1
        5, 9, 8, // tri2
        10, 7, 8 // tri3
    ),
    gsSPNTriangles_5b(
        5, 11, 9, // tri0
        11, 12, 9, // tri1
        12, 13, 9, // tri2
        12, 14, 13 // tri3
    ),
    gsSPNTriangles_5b(
        14, 15, 13, // tri0
        14, 16, 15, // tri1
        5, 17, 11, // tri2
        5, 18, 17 // tri3
    ),
    gsSPNTriangles_5b(
        18, 19, 17, // tri0
        18, 20, 19, // tri1
        20, 21, 19, // tri2
        20, 22, 21 // tri3
    ),
    gsSPNTriangles_5b(
        23, 24, 22, // tri0
        16, 25, 24, // tri1
        25, 22, 24, // tri2
        16, 14, 25 // tri3
    ),
    gsSPNTriangles_5b(
        6, 26, 4, // tri0
        26, 27, 4, // tri1
        26, 28, 27, // tri2
        28, 3, 27 // tri3
    ),
    gsSPNTriangles_5b(
        10, 29, 7, // tri0
        10, 0, 29, // tri1
        0, 2, 29, // tri2
        3, 28, 2 // tri3
    ),
    gsSPNTriangles_5b(
        1, 30, 3, // tri0
        0, 0, 0, // tri1
        0, 0, 0, // tri2
        0, 0, 0 // tri3
    ),
    gsDPLoadTLUT_Dolphin(15, 16, 1, bush_pal_dummy),
    gsDPLoadTextureBlock_4b_Dolphin(bush_b_tex_dummy, G_IM_FMT_CI, 64, 32, 15, GX_REPEAT, GX_CLAMP, 0, 0),
    gsSPVertex(&grd_s_m_4_v[68], 13, 0),
    gsSPNTrianglesInit_5b(
        6, // tri count
        0, 1, 2, // tri0
        3, 0, 4, // tri1
        5, 6, 7 // tri2
    ),
    gsSPNTriangles_5b(
        1, 5, 8, // tri0
        9, 10, 11, // tri1
        10, 3, 12, // tri2
        0, 0, 0 // tri3
    ),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, 0, 0, 0, TEXEL0, COMBINED, 0, SHADE, 0, 0, 0, 0, COMBINED),
    gsDPSetPrimColor(0, 255, 206, 189, 148, 255),
    gsSPDisplayList(0x0C000000),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPLoadTextureBlock_4b_Dolphin(beach1_tex_dummy2, G_IM_FMT_I, 64, 32, 15, GX_REPEAT, GX_CLAMP, 0, 0),
    gsSPVertex(&grd_s_m_4_v[81], 22, 0),
    gsSPNTrianglesInit_5b(
        20, // tri count
        0, 1, 2, // tri0
        3, 0, 2, // tri1
        3, 2, 4 // tri2
    ),
    gsSPNTriangles_5b(
        5, 3, 4, // tri0
        6, 5, 4, // tri1
        6, 4, 7, // tri2
        6, 7, 8 // tri3
    ),
    gsSPNTriangles_5b(
        9, 6, 8, // tri0
        10, 9, 8, // tri1
        10, 8, 11, // tri2
        12, 10, 11 // tri3
    ),
    gsSPNTriangles_5b(
        13, 12, 11, // tri0
        13, 11, 14, // tri1
        15, 13, 14, // tri2
        15, 14, 16 // tri3
    ),
    gsSPNTriangles_5b(
        17, 15, 16, // tri0
        18, 19, 17, // tri1
        18, 17, 16, // tri2
        20, 21, 2 // tri3
    ),
    gsSPNTriangles_5b(
        1, 20, 2, // tri0
        0, 0, 0, // tri1
        0, 0, 0, // tri2
        0, 0, 0 // tri3
    ),
    gsSPEndDisplayList(),
};

extern Gfx grd_s_m_4_modelT[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetCombineLERP(TEXEL1, 0, TEXEL0, TEXEL0, TEXEL1, 0, TEXEL0, 0, PRIMITIVE, 0, SHADE, COMBINED, 0, 0, 0, COMBINED),
    gsDPSetPrimColor(0, 255, 60, 120, 255, 255),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2),
    gsDPSetTextureImage_Dolphin(G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, wave1_tex_dummy),
    gsDPSetTile_Dolphin(G_DOLPHIN_TLUT_DEFAULT_MODE, 0, 0, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetTextureImage_Dolphin(G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, wave2_tex_dummy),
    gsDPSetTile_Dolphin(G_DOLPHIN_TLUT_DEFAULT_MODE, 1, 0, GX_REPEAT, GX_CLAMP, 0, 0),
    gsSPDisplayList(0x0B000000),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&grd_s_m_4_v[142], 32, 0),
    gsSPNTrianglesInit_5b(
        41, // tri count
        0, 1, 2, // tri0
        0, 2, 3, // tri1
        0, 3, 4 // tri2
    ),
    gsSPNTriangles_5b(
        5, 0, 4, // tri0
        6, 5, 4, // tri1
        6, 4, 7, // tri2
        6, 7, 8 // tri3
    ),
    gsSPNTriangles_5b(
        9, 6, 8, // tri0
        10, 9, 8, // tri1
        10, 8, 11, // tri2
        12, 10, 11 // tri3
    ),
    gsSPNTriangles_5b(
        12, 11, 13, // tri0
        14, 12, 13, // tri1
        15, 14, 13, // tri2
        15, 13, 16 // tri3
    ),
    gsSPNTriangles_5b(
        17, 15, 16, // tri0
        17, 16, 18, // tri1
        19, 17, 18, // tri2
        20, 19, 18 // tri3
    ),
    gsSPNTriangles_5b(
        21, 22, 20, // tri0
        21, 20, 18, // tri1
        20, 22, 23, // tri2
        22, 24, 23 // tri3
    ),
    gsSPNTriangles_5b(
        25, 0, 26, // tri0
        25, 1, 0, // tri1
        0, 5, 26, // tri2
        5, 27, 26 // tri3
    ),
    gsSPNTriangles_5b(
        5, 6, 27, // tri0
        6, 28, 27, // tri1
        6, 9, 28, // tri2
        9, 10, 28 // tri3
    ),
    gsSPNTriangles_5b(
        10, 29, 28, // tri0
        10, 12, 29, // tri1
        12, 14, 29, // tri2
        14, 30, 29 // tri3
    ),
    gsSPNTriangles_5b(
        14, 31, 30, // tri0
        14, 15, 31, // tri1
        15, 17, 31, // tri2
        17, 23, 31 // tri3
    ),
    gsSPNTriangles_5b(
        17, 19, 23, // tri0
        19, 20, 23, // tri1
        0, 0, 0, // tri2
        0, 0, 0 // tri3
    ),
    gsDPSetTextureImage_Dolphin(G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, wave1_tex_dummy),
    gsDPSetTile_Dolphin(G_DOLPHIN_TLUT_DEFAULT_MODE, 0, 0, GX_REPEAT, GX_REPEAT, 0, 0),
    gsDPSetTextureImage_Dolphin(G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, wave3_tex_dummy),
    gsDPSetTile_Dolphin(G_DOLPHIN_TLUT_DEFAULT_MODE, 1, 0, GX_REPEAT, GX_REPEAT, 0, 0),
    gsSPDisplayList(0x0D000000),
    gsSPVertex(&grd_s_m_4_v[174], 24, 0),
    gsSPNTrianglesInit_5b(
        22, // tri count
        0, 1, 2, // tri0
        0, 3, 1, // tri1
        3, 4, 1 // tri2
    ),
    gsSPNTriangles_5b(
        3, 5, 4, // tri0
        3, 6, 5, // tri1
        6, 7, 5, // tri2
        6, 8, 7 // tri3
    ),
    gsSPNTriangles_5b(
        8, 9, 7, // tri0
        8, 10, 9, // tri1
        8, 11, 10, // tri2
        11, 12, 10 // tri3
    ),
    gsSPNTriangles_5b(
        12, 13, 10, // tri0
        12, 14, 13, // tri1
        12, 15, 14, // tri2
        12, 16, 15 // tri3
    ),
    gsSPNTriangles_5b(
        16, 17, 15, // tri0
        17, 18, 15, // tri1
        17, 19, 18, // tri2
        17, 20, 19 // tri3
    ),
    gsSPNTriangles_5b(
        20, 21, 19, // tri0
        20, 22, 21, // tri1
        22, 23, 21, // tri2
        0, 0, 0 // tri3
    ),
    gsSPEndDisplayList(),
};