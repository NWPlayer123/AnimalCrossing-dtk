#include "bootdata.h"
#include "libforest/gbi_extensions.h"

static u8 gam_win_moji3_tex[] ATTRIBUTE_ALIGN(32) = {
#include "assets/bootdata/gam_win_moji3_tex.inc"
};

static u8 gam_win_moji4_tex[] ATTRIBUTE_ALIGN(32) = {
#include "assets/bootdata/gam_win_moji4_tex.inc"
};

static u8 att_win_waku1_tex[] ATTRIBUTE_ALIGN(32) = {
#include "assets/bootdata/att_win_waku1_tex_1.inc"
};

static u8 att_win_waku2_tex[] ATTRIBUTE_ALIGN(32) = {
#include "assets/bootdata/att_win_waku2_tex_1.inc"
};

static u8 att_win_waku3_tex[] ATTRIBUTE_ALIGN(32) = {
#include "assets/bootdata/att_win_waku3_tex_1.inc"
};

static u8 att_win_waku4_tex[] ATTRIBUTE_ALIGN(32) = {
#include "assets/bootdata/att_win_waku4_tex_1.inc"
};

static u8 att_win_waku5_tex[] ATTRIBUTE_ALIGN(32) = {
#include "assets/bootdata/att_win_waku5_tex_1.inc"
};

static u8 att_win_waku6_tex[] ATTRIBUTE_ALIGN(32) = {
#include "assets/bootdata/att_win_waku6_tex_1.inc"
};

static Vtx gam_win2_v[] = {
#include "assets/bootdata/gam_win2_v.inc"
};

Gfx gam_win2_moji_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetCombineMode(G_CC_BLENDPRIMDECALA, G_CC_PASS2),
    gsDPSetPrimColor(0, 255, 50, 50, 60, 255),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_ZB_XLU_SURF2),
    gsDPLoadTextureBlock_4b_Dolphin(gam_win_moji4_tex, G_IM_FMT_I, 160, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_SHADING_SMOOTH),
    gsSPVertex(&gam_win2_v[0], 8, 0),
    gsSPNTrianglesInit_5b(2,       // tri count
                          0, 1, 2, // tri0
                          1, 3, 2, // tri1
                          0, 0, 0  // tri2
                          ),
    gsDPLoadTextureBlock_4b_Dolphin(gam_win_moji3_tex, G_IM_FMT_I, 160, 16, 15, GX_CLAMP, GX_CLAMP, 0, 0),
    gsSPNTrianglesInit_5b(2,       // tri count
                          4, 5, 6, // tri0
                          5, 7, 6, // tri1
                          0, 0, 0  // tri2
                          ),
    gsSPEndDisplayList(),
};

Gfx gam_win2_winT_model[] = {
    gsSPTexture(0, 0, 0, 0, G_ON),
    gsDPSetCombineMode(G_CC_BLENDPRIMDECALA, G_CC_PASS2),
    gsDPSetPrimColor(0, 255, 225, 255, 255, 255),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_ZB_XLU_SURF2),
    gsDPLoadTextureBlock_4b_Dolphin(att_win_waku6_tex, G_IM_FMT_I, 16, 16, 15, GX_MIRROR, GX_MIRROR, 0, 0),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_SHADING_SMOOTH),
    gsSPVertex(&gam_win2_v[8], 28, 0),
    gsSPNTrianglesInit_5b(2,       // tri count
                          0, 1, 2, // tri0
                          1, 3, 2, // tri1
                          0, 0, 0  // tri2
                          ),
    gsDPLoadTextureBlock_4b_Dolphin(att_win_waku5_tex, G_IM_FMT_I, 64, 32, 15, GX_MIRROR, GX_MIRROR, 0, 0),
    gsSPNTrianglesInit_5b(4,       // tri count
                          4, 5, 6, // tri0
                          7, 4, 6, // tri1
                          8, 9, 10 // tri2
                          ),
    gsSPNTriangles_5b(9, 11, 10, // tri0
                      0, 0, 0,   // tri1
                      0, 0, 0,   // tri2
                      0, 0, 0    // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(att_win_waku4_tex, G_IM_FMT_I, 64, 32, 15, GX_MIRROR, GX_MIRROR, 0, 0),
    gsSPNTrianglesInit_5b(4,          // tri count
                          12, 13, 14, // tri0
                          15, 12, 14, // tri1
                          16, 17, 18  // tri2
                          ),
    gsSPNTriangles_5b(17, 19, 18, // tri0
                      0, 0, 0,    // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(att_win_waku3_tex, G_IM_FMT_I, 32, 64, 15, GX_MIRROR, GX_MIRROR, 0, 0),
    gsSPNTrianglesInit_5b(4,          // tri count
                          20, 21, 22, // tri0
                          23, 20, 22, // tri1
                          24, 25, 26  // tri2
                          ),
    gsSPNTriangles_5b(25, 27, 26, // tri0
                      0, 0, 0,    // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(att_win_waku2_tex, G_IM_FMT_I, 64, 32, 15, GX_MIRROR, GX_MIRROR, 0, 0),
    gsSPVertex(&gam_win2_v[36], 16, 0),
    gsSPNTrianglesInit_5b(4,       // tri count
                          0, 1, 2, // tri0
                          1, 3, 2, // tri1
                          4, 5, 6  // tri2
                          ),
    gsSPNTriangles_5b(7, 4, 6, // tri0
                      0, 0, 0, // tri1
                      0, 0, 0, // tri2
                      0, 0, 0  // tri3
                      ),
    gsDPLoadTextureBlock_4b_Dolphin(att_win_waku1_tex, G_IM_FMT_I, 64, 32, 15, GX_MIRROR, GX_MIRROR, 0, 0),
    gsSPNTrianglesInit_5b(4,         // tri count
                          8, 9, 10,  // tri0
                          11, 8, 10, // tri1
                          12, 13, 14 // tri2
                          ),
    gsSPNTriangles_5b(13, 15, 14, // tri0
                      0, 0, 0,    // tri1
                      0, 0, 0,    // tri2
                      0, 0, 0     // tri3
                      ),
    gsSPEndDisplayList(),
};
