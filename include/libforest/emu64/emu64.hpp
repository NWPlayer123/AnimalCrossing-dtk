#ifndef EMU64_H
#define EMU64_H

#include "types.h"
#include "va_args.h"
#include "MSL_C/printf.h"
#include "libforest/gbi_extensions.h"
#include "libforest/emu64/texture_cache.h"
#include "dolphin/os/__ppc_eabi_init.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "sys_ucode.h"

#ifdef EMU64_DEBUG
#define AFLAGS_MAX 100
#else
#define AFLAGS_MAX 0
#endif

#define EMU64_TEX_BLOCK_SIZE_X 4
#define EMU64_TEX_BLOCK_SIZE_Y 4

#define NUM_COMMANDS 64
#define NUM_SEGMENTS 16
#define DL_MAX_STACK_LEVEL 18
#define DL_HISTORY_COUNT 16
#define NUM_TILES 8
#define NUM_TLUTS 16
#define MTX_STACK_SIZE 10
#define VTX_COUNT 128
#define NUM_LIGHTS 8

#define EMU64_WARNING_COUNT 10
#define EMU64_WARN_IDX_DL 4

#define EMU64_WARN_TIME 600


#define EMU64_DIRTY_FLAG_PRIM_COLOR 0
#define EMU64_DIRTY_FLAG_ENV_COLOR 1
#define EMU64_DIRTY_FLAG_BLEND_COLOR 2
#define EMU64_DIRTY_FLAG_FOG 3
#define EMU64_DIRTY_FLAG_FILL_COLOR 4
#define EMU64_DIRTY_FLAG_FILL_TEV_COLOR 5
#define EMU64_DIRTY_FLAG_COMBINE 6
#define EMU64_DIRTY_FLAG_OTHERMODE_HIGH 7
#define EMU64_DIRTY_FLAG_OTHERMODE_LOW 8
#define EMU64_DIRTY_FLAG_GEOMETRYMODE 9
#define EMU64_DIRTY_FLAG_PROJECTION_MTX 10
#define EMU64_DIRTY_FLAG_TEX 11
#define EMU64_DIRTY_FLAG_POSITION_MTX 12
#define EMU64_DIRTY_FLAG_TEX_TILE0 13
#define EMU64_DIRTY_FLAG_TEX_TILE1 14
#define EMU64_DIRTY_FLAG_TEX_TILE2 15
#define EMU64_DIRTY_FLAG_TEX_TILE3 16
#define EMU64_DIRTY_FLAG_TEX_TILE4 17
#define EMU64_DIRTY_FLAG_TEX_TILE5 18
#define EMU64_DIRTY_FLAG_TEX_TILE6 19
#define EMU64_DIRTY_FLAG_TEX_TILE7 20
#define EMU64_DIRTY_FLAG_21 21
#define EMU64_DIRTY_FLAG_22 22
#define EMU64_DIRTY_FLAG_23 23
#define EMU64_DIRTY_FLAG_24 24
#define EMU64_DIRTY_FLAG_25 25
#define EMU64_DIRTY_FLAG_26 26
#define EMU64_DIRTY_FLAG_27 27
#define EMU64_DIRTY_FLAG_28 28
#define EMU64_DIRTY_FLAG_LIGHTS 29
#define EMU64_DIRTY_FLAG_LIGHTING 30
#define EMU64_DIRTY_FLAG_TEX_MTX 31
#define NUM_DIRTY_FLAGS 32

#define EMU64_TLUT_IA16 0x0000
#define EMU64_TLUT_RGBA5551 0x8000

/* TODO: figure out where this actually belongs */
namespace std {
  typedef struct __va_list_struct  __tag_va_List;
}

typedef union {
  GXColor color;
  struct {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
  };
  u32 raw;
} EmuColor;

typedef struct {
  Vec position;
  union {
    struct {
      u16 _pad0:2;
      u16 cull_z_greater:1;
      u16 cull_z_lesser:1;
      u16 cull_y_greater:1;
      u16 cull_y_lesser:1;
      u16 cull_x_greater:1;
      u16 cull_x_lesser:1;
      u16 _pad1:7;
      u16 nonshared:1;
    };
    u16 flag;
  };
  struct {
    s16 s, t;
  } tex_coords;
  short pad;
  Vec normal;
  EmuColor color;
} Vertex;

typedef struct {
  EmuColor color;
  Vec position;

  u8 unk[12];

  struct {
    f32 kc, k1, kq; /*kc = k0, kq = k2 */
  } attenuation;
} EmuLight;

typedef struct {
  void* img_addr; /* Texture RAM address */
  u16 width; /* Texture width */
  u16 height; /* Texture height */
  u8 format; /* Texture format */
  u8 size; /* Size in bpp */
  u8 tlut_name; /* Palette/TLUT idx */
  u8 pad;
} emu64_texture_info;

static inline void get_blk_wd_ht(unsigned int siz, unsigned int* blk_wd, unsigned int* blk_ht) {
  static u8 blk_tbl[4][2] = {
    { 8, 8 }, // G_IM_SIZ_4b
    { 8, 4 }, // G_IM_SIZ_8b
    { 4, 4 }, // G_IM_SIZ_16b
    { 4, 4 }  // G_IM_SIZ_32b
  };

  *blk_wd = blk_tbl[siz][0];
  *blk_ht = blk_tbl[siz][1];
}

extern void get_dol_wd_ht(unsigned int siz, unsigned int in_wd, unsigned int in_ht, unsigned int* wd, unsigned int* ht);

static inline unsigned int rgba5551_to_rgb5a3(unsigned int rgba5551) {
  if ((rgba5551 & 1)) {
    return 0x8000 | (rgba5551 >> 1); // no transparency so simply swap
  }
  else {
    return ((rgba5551 >> 3) & 0xF0) | ((rgba5551 >> 4) & ~0xFF) | ((rgba5551 >> 2) & 0x0F);
  }
}

static inline unsigned int get_dol_tex_siz(unsigned int siz, unsigned int in_wd, unsigned int in_ht) {
  unsigned int wd;
  unsigned int ht;

  get_dol_wd_ht(siz, in_wd, in_ht, &wd, &ht);
  return ((wd * ht) << siz) / 2;
}

static inline unsigned int get_dol_tlut_size(unsigned int count) {
  return ALIGN_NEXT(count * sizeof(u16), 32);
}

#define AFLAGS_COMBINE_AUTO 12
#define ALFAGS_TEV_ALPHA_KONST 18
#define AFLAGS_2TRIS 22 /* Draws the current polygon as two triangles */
#define AFLAGS_SKIP_COMBINE_TEV 27
#define AFLAGS_FORCE_TEV_COMBINE_MODE 28 /* 1 = force shade, 2 = force d1 = ENV, Ad1 = ONE */

class aflags_c {
public:
  #ifndef EMU64_DEBUG
  
  int getMaxArray() { return AFLAGS_MAX; }
  void set(u32 idx, u8 val) { this->flags[idx] = val; } /* @fabricated */
  int operator[](u32 idx) { return this->flags[idx]; } /* @fabricated */

  #else

  int getMaxArray() { return AFLAGS_MAX; }
  void set(u32 idx, u8 val) {  }
  int operator[](u32 idx) { return 0; }

  #endif

private:
  int flags[AFLAGS_MAX];
};

class Texture {
public:
  /* @weak */
  Texture(void* img_p, u16 w, u16 h, u8 fmt, u8 bpp) {
    this->img_p = img_p;
    this->width = w;
    this->height = h;
    this->n64_bpp = bpp;
    this->n64_fmt = fmt;
    this->blockX_size = EMU64_TEX_BLOCK_SIZE_X;
    this->blockY_size = EMU64_TEX_BLOCK_SIZE_Y;
  }

  /* @??? (maybe not weak?) */
  ~Texture() { };

  /* @weak */
  u32 getOffset(int x, int y) {
    const int size_x = sizeof(u16) * EMU64_TEX_BLOCK_SIZE_X;
    const int size_y = sizeof(u16) * EMU64_TEX_BLOCK_SIZE_Y;
    
    return ((((u32)x / 8) + ((u32)(((u32)y / 8) * this->width) / 8)) * (size_x * size_y)) +
      ((u32)y & 7) * size_x +
      ((u32)x & 7);
  }

  /* @weak */
  u32 getTexel(int block_x, int block_y) {
    int x = (1 << this->blockX_size) - 1;
    int y = (1 << this->blockY_size) - 1;
    
    block_x &= x;
    block_y &= y;

    u32 ofs = this->getOffset(block_x, block_y);

    switch (this->n64_bpp) {
      case G_IM_SIZ_4b:
      {
        u8* img_p = ((u8*)this->img_p) + ofs / 2;
        if ((block_x & 1) == 0) {
          return *img_p >> 4;
        }
        else {
          return *img_p & 0xF;
        }
      }

      case G_IM_SIZ_8b:
      {
        return ((u8*)this->img_p)[ofs];
      }

      case G_IM_SIZ_16b:
      {
        return ((u16*)this->img_p)[ofs];
      }

      default:
      {
        return ((u32*)this->img_p)[ofs];
      }
    }
  }

  /* @weak */
  void putTexel(int block_x, int block_y, u32 texel) {
    int x = (1 << this->blockX_size) - 1;
    int y = (1 << this->blockY_size) - 1;
    
    block_x &= x;
    block_y &= y;

    u32 ofs = this->getOffset(block_x, block_y);

    switch (this->n64_bpp) {
      case G_IM_SIZ_4b:
      {
        u8* img_p = ((u8*)this->img_p) + ofs / 2;
        if ((block_x & 1) == 0) {
          *img_p = (*img_p & 0x0F) | (texel << 4);
        }
        else {
          *img_p = (*img_p & 0xF0) | (texel & 0xF);
        }
      }

      case G_IM_SIZ_8b:
      {
        ((u8*)this->img_p)[ofs] = texel;
      }

      case G_IM_SIZ_16b:
      {
        ((u16*)this->img_p)[ofs] = texel;
      }

      case G_IM_SIZ_32b:
      {
        ((u32*)this->img_p)[ofs] = texel;
      }
    }
  }

  /* Member variables */
  void* img_p;
  u16 width;
  u16 height;
  u8 blockX_size;
  u8 blockY_size;
  u8 n64_fmt;
  u8 n64_bpp;
};

#define EMU64_PRINTF_FLAG (1 << 1)
#define EMU64_PRINTF1_FLAG (1 << 2)
#define EMU64_PRINTF2_FLAG (1 << 3)
#define EMU64_PRINTF3_FLAG (1 << 4)

class emu64_print {
public:
  void Printf(const char* fmt, ...) {
    va_list list;
    
    if ((this->print_flags & EMU64_PRINTF_FLAG)) {
      va_start(list, fmt);
      this->Vprintf(fmt, list);
      va_end(list);
    }
  }

  void Printf0(const char* fmt, ...) {
    va_list list;

    va_start(list, fmt);
    this->Vprintf(fmt, list);
    va_end(list);
  }

  void Printf1(const char* fmt, ...) {
    va_list list;
    
    if ((this->print_flags & EMU64_PRINTF1_FLAG)) {
      va_start(list, fmt);
      this->Vprintf(fmt, list);
      va_end(list);
    }
  }

  void Printf2(const char* fmt, ...) {
    va_list list;
    
    if ((this->print_flags & EMU64_PRINTF2_FLAG)) {
      va_start(list, fmt);
      this->Vprintf(fmt, list);
      va_end(list);
    }
  }

  void Printf3(const char* fmt, ...) {
    va_list list;
    
    if ((this->print_flags & EMU64_PRINTF3_FLAG)) {
      va_start(list, fmt);
      this->Vprintf(fmt, list);
      va_end(list);
    }
  }

protected:
  u8 print_flags;

private:
  void Vprintf(const char* fmt, std::__tag_va_List* va_list) { vprintf(fmt, va_list); }
};

#define EMU64_ASSERTLINE(cond, line) \
  if (!cond) {  \
    this->panic(#cond, __FILE__, line); \
  }

#define EMU64_ASSERT(cond) EMU64_PANICLINE(cond, __LINE__)

class emu64 : public emu64_print {
public:
  void emu64_init();
  void printInfo();
  void panic(char* fmt, char* file, int line);
  
  void emu64_change_ucode(void* ucode_p);
  void texconv_tile(u8* addr, u8* converted_addr, unsigned int wd, unsigned int fmt, unsigned int siz, unsigned int start_wd, unsigned int start_ht, unsigned int end_wd, unsigned int end_ht, unsigned int line_siz);
  unsigned int tmem_swap(unsigned int ofs, unsigned int blk_siz) { return ofs ^ ((ofs / blk_siz) & 4); }
  void tlutconv_rgba5551(u16* rgba5551_p, u16* rgb5a3_p, unsigned int count);
  void tlutconv_ia16(u16* src_ia16_p, u16* dst_ia16_p, unsigned int count);
  u8* texconv_tile_new(u8* addr, unsigned int wd, unsigned int fmt, unsigned int siz, unsigned int start_wd, unsigned int start_ht, unsigned int end_wd, unsigned int end_ht, unsigned int line_siz);
  u16* tlutconv_new(u16* tlut, unsigned int tlut_fmt, unsigned int count);
  void tlutconv(u16* src_tlut, u16* dst_tlut, unsigned int count, unsigned int tlut_fmt);
  int replace_combine_to_tev(Gfx* g);
  int combine_auto();
  int combine_tev();
  void combine_manual();
  const char* combine_name(u32 param, u32 type);
  const char* combine_alpha(int param, int type);
  void print_combine(u64 combine);

  /* N64 texture format[N64 bit size] -> dol texture format */
  static u16 fmtxtbl[8][4];
  static char* warningString[EMU64_WARNING_COUNT];
  static int warningTime[EMU64_WARNING_COUNT];
  static bool displayWarning;

private:
  /* 0x0000 */ // u8 emu64_print::print_flags;
  /* 0x0001 */ u8 print_commands;
  /* 0x0002 */ bool disable_polygons;
  /* 0x0004 */ u32 err_count;
  /* 0x0008 */ u32 cmds_processed; /* ??? */
  /* 0x000C */ u32 total_vertices;
  /* 0x0010 */ u32 vtx_load_calls;
  /* 0x0014 */ u32 triangles;
  /* 0x0018 */ u32 double_triangles;
  /* 0x001C */ u32 quads;
  /* 0x0020 */ u32 lines;
  /* 0x0024 */ u32 load_ucode_calls;
  /* 0x0028 */ u32 num_unknown_cmds;
  /* 0x002C */ u32 num_unknown_ucodes;
  /* 0x0030 */ int polygons;
  /* 0x0034 */ u32 cullDL_calls;
  /* 0x0038 */ u32 cullDL_outside_obj_count;
  /* 0x003C */ u32 cullDL_visible_obj_count;
  /* 0x0040 */ Gfx* gfx_p;
  /* 0x0048 */ Gfx gfx;
  /* 0x0050 */ u8 gfx_cmd;
  /* 0x0054 */ void* work_ptr;
  /* 0x0058 */ int end_dl;
  /* 0x005C */ s8 ucode_len;
  /* 0x0060 */ ucode_info* ucode_info;
  /* 0x0064 */ int ucode_type; // maybe?
  /* 0x0068 */ int _0068; /* ??? */
  /* 0x006C */ void* segments[NUM_SEGMENTS];
  /* 0x00AC */ Gfx* DL_stack[DL_MAX_STACK_LEVEL];
  /* 0x00F4 */ s8 DL_stack_level;
  /* 0x00F8 */ u32 othermode_high;
  /* 0x00FC */ u32 othermode_low;
  /* 0x0100 */ u32 geometry_mode;
  /* 0x0104 */ u32 _0104;
  /* 0x0108 */ Gfx combine;
  /* 0x0110 */ emu64_texture_info texture_info[NUM_TILES];
  /* 0x0170 */ Gsetimg2 setimg2_cmds[NUM_TILES];
  /* 0x01B0 */ void* tlut_addresses[NUM_TLUTS];
  /* 0x01F0 */ GXTexObj tex_objs[NUM_TILES];
  /* 0x02F0 */ GXTlutObj tlut_objs[NUM_TLUTS];
  /* 0x03B0 */ bool use_dolphin_settile[NUM_TILES];
  /* 0x03B8 */ Gsettile settile_cmds[NUM_TILES];
  /* 0x03F8 */ Gsettile_dolphin settile_dolphin_cmds[NUM_TILES];
  /* 0x0438 */ Gsettilesize_dolphin settilesize_dolphin_cmds[NUM_TILES];
  /* 0x0478 */ Gsetimg_new now_setimg;
  /* 0x0480 */ u8 tex_edge_alpha;

  /* 0x0484 */ union {
      u32 primdepth;
      struct {
          u16 primdepth_z; /* Z value */
          u16 primdepth_dz; /* Delta Z */
      };
  };

  /* 0x0488 */ EmuColor primitive_color; /* GX_TEVREG1 */
  /* 0x048C */ EmuColor environment_color; /* GX_TEVREG2 */
  /* 0x0490 */ EmuColor blend_color;
  /* 0x0494 */ EmuColor fog_color;
  /* 0x0498 */ s16 fog_zmult;
  /* 0x049A */ s16 fog_zoffset;
  /* 0x049C */ EmuColor fill_color;
  /* 0x04A0 */ EmuColor fill_tev_color; /* GX_TEVREG0 */
  /* 0x04A4 */ bool dirty_flags[NUM_DIRTY_FLAGS];
  /* 0x04C4 */ GC_Mtx original_projection_mtx;
  /* 0x04F4 */ GC_Mtx position_mtx;
  /* 0x0524 */ GC_Mtx model_view_mtx_stack[MTX_STACK_SIZE];
  /* 0x0704 */ GC_Mtx position_mtx_stack[MTX_STACK_SIZE];
  /* 0x08E4 */ Mtx44 projection_mtx;
  /* 0x0924 */ struct {
    struct {
      s8 x, y, z;
    } x;
    struct {
      s8 x, y, z;
    } y;
  } lookAt;
  /* 0x092C */ f32 near; /* Near clipping plane */
  /* 0x0930 */ f32 far; /* Far clipping plane */
  /* 0x0934 */ GC_Mtx model_view_mtx;
  /* 0x0964 */ GC_Mtx _0964; /* UNCONFIRMED TYPE */
  /* 0x0994 */ int mtx_stack_size;
  /* 0x0998 */ Gtexture_internal texture_gfx;
  /* 0x09A0 */ f32 texture_scale_s; /* x-scale */
  /* 0x09A4 */ f32 texture_scale_t; /* y-scale */
  /* 0x09A8 */ Mtx44 ortho_mtx;
  /* 0x09E8 */ GXProjectionType projection_type;
  /* 0x09EC */ GC_Mtx perspective_mtx;
  /* 0x0A1C */ u32 _0A1C;
  /* 0x0A20 */ u32 rdpHalf_1;
  /* 0x0A24 */ EmuLight lights[NUM_LIGHTS];
  /* 0x0B64 */ u8 num_lights;
  /* 0x0B68 */ u32 lookatx_cnt;
  /* 0x0B6C */ u32 lookaty_cnt;
  /* 0x0B70 */ u32 _0B70;
  /* 0x0B74 */ u32 texconv_cnt;
  /* 0x0B78 */ u32 vertex_cnt;
  /* 0x0B7C */ u32 texconv_time;
  /* 0x0B80 */ u32 rsprdp_time;
  /* 0x0B84 */ u32 combine_time;
  /* 0x0B88 */ u32 spvertex_time;
  /* 0x0B8C */ u32 setup_time;
  /* 0x0B90 */ u32 loadblock_time;
  /* 0x0B94 */ u32 loadtlut_time;
  /* 0x0B98 */ u32 matrix_time;
  /* 0x0B9C */ u32 polygons_time;
  /* 0x0BA0 */ u32 dirty_check_time;
  /* 0x0BA4 */ u32 dirty_lightX_time;
  /* 0x0BA8 */ u32 dirty_lightX_cnt;
  /* 0x0BAC */ u32 dirty_light_time;
  /* 0x0BB0 */ u32 dirty_light_cnt;
  /* 0x0BB4 */ u32 dirty_tex_time;
  /* 0x0BB8 */ u32 dirty_tex_cnt;
  /* 0x0BBC */ u32 dirty_tex1_time;
  /* 0x0BC0 */ u32 dirty_tex1_cnt;
  /* 0x0BC4 */ u32 dirty_tex2_time;
  /* 0x0BC8 */ u32 dirty_tex2_cnt;
  /* 0x0BCC */ u32 dirty_texmtx_time;
  /* 0x0BD0 */ u32 dirty_texmtx_cnt;
  /* 0x0BD4 */ u32 dirty_proj_time;
  /* 0x0BD8 */ u32 dirty_primcolor_time;
  /* 0x0BDC */ u32 dirty_envcolor_time;
  /* 0x0BE0 */ u32 dirty_fillcolor_time;
  /* 0x0BE4 */ u32 dirty_combine_mode_time;
  /* 0x0BE8 */ u32 dirty_othermodeh_time;
  /* 0x0BEC */ u32 dirty_othermodel_time;
  /* 0x0BF0 */ u32 dirty_geometory_time; /* Yes, the devs misspelt this */
  /* 0x0BF4 */ u32 setuptex_time;
  /* 0x0BF8 */ struct {
    u32 time;
    u32 calls;
  } command_info[NUM_COMMANDS];
  /* 0x0DF8 */ u32 texture_cache_select_time;
  /* 0x0DFC */ u32 guMtxL2F_time;
  /* 0x0E00 */ u32 guMtxL2F_cnt;
  /* 0x0E04 */ u32 combine_auto_cnt[5];
  /* 0x0E18 */ bool using_nonshared_mtx;
  /* 0x0E1C */ Vertex vertices[VTX_COUNT];
  /* 0x201C */ u8 texture_adjust_mode;
  /* 0x2020 */ u32 resolved_addresses;
  /* 0x2024 */ u32 _2024;
  /* 0x2028 */ u32 _2028;
  /* 0x202C */ u32 _202C;
  /* 0x2030 */ u32 abnormal_addresses;
  /* 0x2034 */ bool rdp_pipe_sync_needed;
  /* 0x2035 */ bool segment_set;
  /* 0x2038 */ Gfx* dl_history[DL_HISTORY_COUNT];
  /* 0x2078 */ u8 dl_history_start;
};

#endif
