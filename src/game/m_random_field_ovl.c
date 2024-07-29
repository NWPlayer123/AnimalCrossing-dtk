#include "m_random_field.h"
#include "m_random_field_ovl.h"

#include "m_common_data.h"

enum {
    mRF_BIT_SLOPE_LEFT,
    mRF_BIT_SLOPE_RIGHT,
    mRF_BIT_BRIDGE_UPPER,
    mRF_BIT_BRIDGE_LOWER,
    mRF_BIT_SHRINE,
    mRF_BIT_POLICE,
    mRF_BIT_MUSEUM,
    mRF_BIT_POOL,
    mRF_BIT_NEEDLEWORK,

    mRF_BIT_NUM
};

#define mRF_TO_BIT(type) (1 << (mRF_BIT_##type))

enum {
    mRF_BLOCK_GROUP_CLIFF,
    mRF_BLOCK_GROUP_RIVER,
    mRF_BLOCK_GROUP_BRIDGE,
    mRF_BLOCK_GROUP_SLOPE,
    mRF_BLOCK_GROUP_RIVER_CLIFF_ANY,
    mRF_BLOCK_GROUP_RIVER_CLIFF_1,
    mRF_BLOCK_GROUP_RIVER_CLIFF_2,
    mRF_BLOCK_GROUP_RIVER_CLIFF_3,
    mRF_BLOCK_GROUP_CLIFF_ANY,

    mRF_BLOCK_GROUP_NUM
};

enum {
    mRF_STEPMODE_TWO,
    mRF_STEPMODE_THREE,

    mRF_STEPMODE_NUM
};

enum {
    mRF_CLIFF_HEIGHT_ABOVE,
    mRF_CLIFF_HEIGHT_BELOW,
    mRF_CLIFF_HEIGHT_BOTH,

    mRF_CLIFF_HEIGHT_NUM
};

enum {
    mRF_RIVER_SIDE_LEFT,
    mRF_RIVER_SIDE_RIGHT,
    mRF_RIVER_SIDE_BOTH,

    mRF_RIVER_SIDE_NUM
};

static u8 l_base_blocks[BLOCK_TOTAL_NUM] = { 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x09, 0x0c, 0x0c, 0x0b, 0x0c,
                                             0x0c, 0x0a, 0x02, 0x27, 0x27, 0x0e, 0x27, 0x27, 0x04, 0x02, 0x27, 0x27,
                                             0x27, 0x27, 0x27, 0x04, 0x02, 0x27, 0x27, 0x27, 0x27, 0x27, 0x04, 0x02,
                                             0x27, 0x27, 0x27, 0x27, 0x27, 0x04, 0x02, 0x27, 0x27, 0x27, 0x27, 0x27,
                                             0x04, 0x65, 0x65, 0x65, 0x65, 0x65, 0x65, 0x65, 0x53, 0x53, 0x53, 0x66,
                                             0x62, 0x63, 0x66, 0x53, 0x53, 0x53, 0x67, 0x67, 0x67, 0x67 };

typedef struct cliff_data_s {
    u8* types;
    int count;
} mRF_trace_data_c;

static u8 l_cliff_startA_factor[] = { mFM_BLOCK_TYPE_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_CLIFF_TOP_LEFT_CORNER };
static mRF_trace_data_c l_cliff_startA = { l_cliff_startA_factor, ARRAY_COUNT(l_cliff_startA_factor) };

static u8 l_cliff_startB_factor[] = { mFM_BLOCK_TYPE_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_CLIFF_BOTTOM_RIGHT_CORNER,
                                      mFM_BLOCK_TYPE_CLIFF_TOP_LEFT_CORNER };
static mRF_trace_data_c l_cliff_startB = { l_cliff_startB_factor, ARRAY_COUNT(l_cliff_startB_factor) };

static u8 l_cliff_startC_factor[] = { mFM_BLOCK_TYPE_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_CLIFF_BOTTOM_RIGHT_CORNER };
static mRF_trace_data_c l_cliff_startC = { l_cliff_startC_factor, ARRAY_COUNT(l_cliff_startC_factor) };

static mRF_trace_data_c* l_cliff_start_table[] = { &l_cliff_startA, &l_cliff_startA, &l_cliff_startB, &l_cliff_startC };

static u8 l_cliff_next_direct[] = { mRF_DIRECT_EAST,  mRF_DIRECT_NORTH, mRF_DIRECT_NORTH, mRF_DIRECT_EAST,
                                    mRF_DIRECT_SOUTH, mRF_DIRECT_SOUTH, mRF_DIRECT_EAST };

static u8 l_cliff1_next[] = { mFM_BLOCK_TYPE_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_CLIFF_BOTTOM_RIGHT_CORNER,
                              mFM_BLOCK_TYPE_CLIFF_TOP_LEFT_CORNER };
static mRF_trace_data_c l_cliff1_next_class = { l_cliff1_next, ARRAY_COUNT(l_cliff1_next) };

static u8 l_cliff2_next[] = { mFM_BLOCK_TYPE_CLIFF_VERTICAL_RIGHT, mFM_BLOCK_TYPE_CLIFF_TOP_RIGHT_CORNER };
static mRF_trace_data_c l_cliff2_next_class = { l_cliff2_next, ARRAY_COUNT(l_cliff2_next) };

static u8 l_cliff3_next[] = { mFM_BLOCK_TYPE_CLIFF_VERTICAL_RIGHT, mFM_BLOCK_TYPE_CLIFF_TOP_RIGHT_CORNER };
static mRF_trace_data_c l_cliff3_next_class = { l_cliff3_next, ARRAY_COUNT(l_cliff3_next) };

static u8 l_cliff4_next[] = { mFM_BLOCK_TYPE_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_CLIFF_BOTTOM_RIGHT_CORNER,
                              mFM_BLOCK_TYPE_CLIFF_TOP_LEFT_CORNER };
static mRF_trace_data_c l_cliff4_next_class = { l_cliff4_next, ARRAY_COUNT(l_cliff4_next) };

static u8 l_cliff5_next[] = { mFM_BLOCK_TYPE_CLIFF_VERTICAL_LEFT, mFM_BLOCK_TYPE_CLIFF_BOTTOM_LEFT_CORNER };
static mRF_trace_data_c l_cliff5_next_class = { l_cliff5_next, ARRAY_COUNT(l_cliff5_next) };

static u8 l_cliff6_next[] = { mFM_BLOCK_TYPE_CLIFF_VERTICAL_LEFT, mFM_BLOCK_TYPE_CLIFF_BOTTOM_LEFT_CORNER };
static mRF_trace_data_c l_cliff6_next_class = { l_cliff6_next, ARRAY_COUNT(l_cliff6_next) };

static u8 l_cliff7_next[] = { mFM_BLOCK_TYPE_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_CLIFF_BOTTOM_RIGHT_CORNER,
                              mFM_BLOCK_TYPE_CLIFF_TOP_LEFT_CORNER };
static mRF_trace_data_c l_cliff7_next_class = { l_cliff7_next, ARRAY_COUNT(l_cliff7_next) };

static mRF_trace_data_c* l_cliff_next_data[] = { &l_cliff1_next_class, &l_cliff2_next_class, &l_cliff3_next_class,
                                                 &l_cliff4_next_class, &l_cliff5_next_class, &l_cliff6_next_class,
                                                 &l_cliff7_next_class };

static u8 l_river1_next[] = { mFM_BLOCK_TYPE_RIVER_SOUTH, mFM_BLOCK_TYPE_RIVER_SOUTH_EAST,
                              mFM_BLOCK_TYPE_RIVER_SOUTH_WEST };
static mRF_trace_data_c l_river1_next_class = { l_river1_next, ARRAY_COUNT(l_river1_next) };

static u8 l_river2_next[] = { mFM_BLOCK_TYPE_RIVER_EAST, mFM_BLOCK_TYPE_RIVER_EAST_SOUTH };
static mRF_trace_data_c l_river2_next_class = { l_river2_next, ARRAY_COUNT(l_river2_next) };

static u8 l_river3_next[] = { mFM_BLOCK_TYPE_RIVER_WEST, mFM_BLOCK_TYPE_RIVER_WEST_SOUTH };
static mRF_trace_data_c l_river3_next_class = { l_river3_next, ARRAY_COUNT(l_river3_next) };

static u8 l_river4_next[] = { mFM_BLOCK_TYPE_RIVER_EAST, mFM_BLOCK_TYPE_RIVER_EAST_SOUTH };
static mRF_trace_data_c l_river4_next_class = { l_river4_next, ARRAY_COUNT(l_river4_next) };

static u8 l_river5_next[] = { mFM_BLOCK_TYPE_RIVER_SOUTH, mFM_BLOCK_TYPE_RIVER_SOUTH_EAST,
                              mFM_BLOCK_TYPE_RIVER_SOUTH_WEST };
static mRF_trace_data_c l_river5_next_class = { l_river5_next, ARRAY_COUNT(l_river5_next) };

static u8 l_river6_next[] = { mFM_BLOCK_TYPE_RIVER_WEST, mFM_BLOCK_TYPE_RIVER_WEST_SOUTH };
static mRF_trace_data_c l_river6_next_class = { l_river6_next, ARRAY_COUNT(l_river6_next) };

static u8 l_river7_next[] = { mFM_BLOCK_TYPE_RIVER_SOUTH, mFM_BLOCK_TYPE_RIVER_SOUTH_EAST,
                              mFM_BLOCK_TYPE_RIVER_SOUTH_WEST };
static mRF_trace_data_c l_river7_next_class = { l_river7_next, ARRAY_COUNT(l_river7_next) };

static mRF_trace_data_c* l_river_next_data[] = { &l_river1_next_class, &l_river2_next_class, &l_river3_next_class,
                                                 &l_river4_next_class, &l_river5_next_class, &l_river6_next_class,
                                                 &l_river7_next_class };

static u8 l_mRF_step3_blocks3[BLOCK_TOTAL_NUM] = {
    0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x08, 0x09, 0x0d, 0x0c, 0x0b, 0x0c, 0x0c, 0x0a, 0x02, 0x2b, 0x2c, 0x0e,
    0x12, 0x0f, 0x3e, 0x3d, 0x0f, 0x16, 0x0f, 0x10, 0x12, 0x3e, 0x3d, 0x0f, 0x1a, 0x27, 0x27, 0x11, 0x04, 0x02,
    0x27, 0x1c, 0x0f, 0x0f, 0x10, 0x04, 0x50, 0x27, 0x28, 0x27, 0x27, 0x27, 0x51, 0x65, 0x65, 0x65, 0x65, 0x65,
    0x65, 0x65, 0x53, 0x53, 0x53, 0x66, 0x62, 0x63, 0x66, 0x53, 0x53, 0x53, 0x67, 0x67, 0x67, 0x67
};

static u8 l_mRF_step3_blocks7[BLOCK_TOTAL_NUM] = {
    0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x08, 0x09, 0x0d, 0x0c, 0x0b, 0x0c, 0x0c, 0x0a, 0x02, 0x2b, 0x2c, 0x0e,
    0x27, 0x12, 0x3e, 0x3d, 0x0f, 0x16, 0x0f, 0x0f, 0x10, 0x04, 0x02, 0x27, 0x28, 0x12, 0x0f, 0x0f, 0x3e, 0x3d,
    0x0f, 0x16, 0x10, 0x27, 0x27, 0x04, 0x50, 0x27, 0x28, 0x27, 0x27, 0x27, 0x51, 0x65, 0x65, 0x65, 0x65, 0x65,
    0x65, 0x65, 0x53, 0x53, 0x53, 0x66, 0x62, 0x63, 0x66, 0x53, 0x53, 0x53, 0x67, 0x67, 0x67, 0x67
};

static u8 l_mRF_step3_blocks7R[BLOCK_TOTAL_NUM] = {
    0x05, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x09, 0x0c, 0x0c, 0x0b, 0x0c, 0x0d, 0x0a, 0x3d, 0x13, 0x27, 0x0e,
    0x2e, 0x2d, 0x04, 0x02, 0x15, 0x0f, 0x0f, 0x16, 0x0f, 0x3e, 0x3d, 0x0f, 0x0f, 0x13, 0x28, 0x27, 0x04, 0x02,
    0x27, 0x27, 0x15, 0x16, 0x0f, 0x3e, 0x50, 0x27, 0x27, 0x27, 0x28, 0x27, 0x51, 0x65, 0x65, 0x65, 0x65, 0x65,
    0x65, 0x65, 0x53, 0x53, 0x53, 0x66, 0x62, 0x63, 0x66, 0x53, 0x53, 0x53, 0x67, 0x67, 0x67, 0x67
};

static u8 l_mRF_step3_blocks8[BLOCK_TOTAL_NUM] = {
    0x05, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x09, 0x0c, 0x0c, 0x0b, 0x0c, 0x0d, 0x0a, 0x3d, 0x0f, 0x13, 0x0e,
    0x2e, 0x2d, 0x04, 0x02, 0x27, 0x15, 0x0f, 0x1a, 0x12, 0x3e, 0x3d, 0x0f, 0x0f, 0x13, 0x1c, 0x10, 0x04, 0x02,
    0x27, 0x27, 0x15, 0x16, 0x0f, 0x3e, 0x50, 0x27, 0x27, 0x27, 0x28, 0x27, 0x51, 0x65, 0x65, 0x65, 0x65, 0x65,
    0x65, 0x65, 0x53, 0x53, 0x53, 0x66, 0x62, 0x63, 0x66, 0x53, 0x53, 0x53, 0x67, 0x67, 0x67, 0x67
};

static u8 l_mRF_step3_blocksB[BLOCK_TOTAL_NUM] = {
    0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x09, 0x0c, 0x0c, 0x0b, 0x0d, 0x0c, 0x0a, 0x3d, 0x0f, 0x13, 0x0e,
    0x28, 0x12, 0x3e, 0x02, 0x27, 0x15, 0x0f, 0x16, 0x10, 0x04, 0x3d, 0x0f, 0x13, 0x2e, 0x2d, 0x27, 0x04, 0x02,
    0x27, 0x15, 0x16, 0x0f, 0x0f, 0x3e, 0x50, 0x27, 0x27, 0x28, 0x27, 0x27, 0x51, 0x65, 0x65, 0x65, 0x65, 0x65,
    0x65, 0x65, 0x53, 0x53, 0x53, 0x66, 0x62, 0x63, 0x66, 0x53, 0x53, 0x53, 0x67, 0x67, 0x67, 0x67
};

static u8 l_mRF_step3_blocksBR[BLOCK_TOTAL_NUM] = {
    0x05, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x09, 0x0c, 0x0d, 0x0b, 0x0c, 0x0c, 0x0a, 0x3d, 0x13, 0x28, 0x0e,
    0x12, 0x0f, 0x3e, 0x02, 0x15, 0x16, 0x0f, 0x10, 0x27, 0x04, 0x02, 0x27, 0x2b, 0x2c, 0x12, 0x0f, 0x3e, 0x3d,
    0x0f, 0x0f, 0x16, 0x10, 0x27, 0x04, 0x50, 0x27, 0x27, 0x28, 0x27, 0x27, 0x51, 0x65, 0x65, 0x65, 0x65, 0x65,
    0x65, 0x65, 0x53, 0x53, 0x53, 0x66, 0x62, 0x63, 0x66, 0x53, 0x53, 0x53, 0x67, 0x67, 0x67, 0x67
};

static u8 l_mRF_step3_blocksE[BLOCK_TOTAL_NUM] = {
    0x05, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x09, 0x0c, 0x0d, 0x0b, 0x0c, 0x0c, 0x0a, 0x3d, 0x13, 0x28, 0x0e,
    0x12, 0x0f, 0x3e, 0x02, 0x15, 0x16, 0x0f, 0x10, 0x27, 0x04, 0x02, 0x27, 0x2b, 0x2c, 0x12, 0x0f, 0x3e, 0x3d,
    0x0f, 0x0f, 0x16, 0x10, 0x27, 0x04, 0x50, 0x27, 0x27, 0x28, 0x27, 0x27, 0x51, 0x65, 0x65, 0x65, 0x65, 0x65,
    0x65, 0x65, 0x53, 0x53, 0x53, 0x66, 0x62, 0x63, 0x66, 0x53, 0x53, 0x53, 0x67, 0x67, 0x67, 0x67
};

static u8 l_mRF_step3_blocksER[BLOCK_TOTAL_NUM] = {
    0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x09, 0x0c, 0x0c, 0x0b, 0x0d, 0x0c, 0x0a, 0x3d, 0x0f, 0x13, 0x0e,
    0x28, 0x12, 0x3e, 0x02, 0x27, 0x15, 0x0f, 0x16, 0x10, 0x04, 0x3d, 0x0f, 0x13, 0x2e, 0x2d, 0x27, 0x04, 0x02,
    0x27, 0x15, 0x16, 0x0f, 0x0f, 0x3e, 0x50, 0x27, 0x27, 0x28, 0x27, 0x27, 0x51, 0x65, 0x65, 0x65, 0x65, 0x65,
    0x65, 0x65, 0x53, 0x53, 0x53, 0x66, 0x62, 0x63, 0x66, 0x53, 0x53, 0x53, 0x67, 0x67, 0x67, 0x67
};

static u8 l_mRF_step3_blocksF[BLOCK_TOTAL_NUM] = {
    0x05, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x09, 0x0c, 0x0d, 0x0b, 0x0c, 0x0c, 0x0a, 0x02, 0x27, 0x28, 0x0e,
    0x12, 0x0f, 0x3e, 0x3d, 0x0f, 0x16, 0x0f, 0x10, 0x27, 0x04, 0x02, 0x27, 0x2b, 0x29, 0x2c, 0x27, 0x04, 0x3d,
    0x0f, 0x0f, 0x0f, 0x16, 0x0f, 0x3e, 0x50, 0x27, 0x27, 0x27, 0x28, 0x27, 0x51, 0x65, 0x65, 0x65, 0x65, 0x65,
    0x65, 0x65, 0x53, 0x53, 0x53, 0x66, 0x62, 0x63, 0x66, 0x53, 0x53, 0x53, 0x67, 0x67, 0x67, 0x67
};

static u8 l_mRF_step3_blocksFR[BLOCK_TOTAL_NUM] = {
    0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x09, 0x0c, 0x0c, 0x0b, 0x0d, 0x0c, 0x0a, 0x3d, 0x0f, 0x13, 0x0e,
    0x28, 0x27, 0x04, 0x02, 0x27, 0x15, 0x0f, 0x16, 0x0f, 0x3e, 0x02, 0x27, 0x2e, 0x2a, 0x2d, 0x27, 0x04, 0x3d,
    0x0f, 0x16, 0x0f, 0x0f, 0x0f, 0x3e, 0x50, 0x27, 0x28, 0x27, 0x27, 0x27, 0x51, 0x65, 0x65, 0x65, 0x65, 0x65,
    0x65, 0x65, 0x53, 0x53, 0x53, 0x66, 0x62, 0x63, 0x66, 0x53, 0x53, 0x53, 0x67, 0x67, 0x67, 0x67
};

static u8* l_mRF_step3_blockss[10] = { l_mRF_step3_blocks3, l_mRF_step3_blocks7,  l_mRF_step3_blocks7R,
                                       l_mRF_step3_blocks8, l_mRF_step3_blocksB,  l_mRF_step3_blocksBR,
                                       l_mRF_step3_blocksE, l_mRF_step3_blocksER, l_mRF_step3_blocksF,
                                       l_mRF_step3_blocksFR };

static int mRF_GetSystemBlockInfo(u8 type) {
    static int system_block_info[mFM_BLOCK_TYPE_NUM] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04,
        0x08, 0x10, 0x20, 0x40, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x01, 0x02, 0x04, 0x08, 0x10, 0x01, 0x08,
        0x10, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x08,
        0x08, 0x10, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x10
    };

    return system_block_info[type];
}

static int mRF_BlockType2CliffIndex(u8 type) {
    static int cliff_info[7] = { 1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6 };

    int block_info = mRF_GetSystemBlockInfo(type);
    int i;

    for (i = 0; i < 7; i++) {
        if ((block_info & cliff_info[i]) != 0) {
            return i;
        }
    }

    return -1;
}

static int mRF_GetRandom(int max) {
    return RANDOM(max);
}

static int mRF_D2ToD1(int bx, int bz) {
    return bz * BLOCK_X_NUM + bx;
}

static void mRF_Direct2BlockNo(int* bx, int* bz, int base_bx, int base_bz, int direct) {
    static int x_offset[mRF_DIRECT_NUM] = { 0, -1, 0, 1 };
    static int z_offset[mRF_DIRECT_NUM] = { -1, 0, 1, 0 };

    int idx = direct & 3;

    bx[0] = base_bx + x_offset[idx];
    bz[0] = base_bz + z_offset[idx];
}

static int mRF_CheckCorrectBlockNo(int bx, int bz, int bx_min, int bx_max, int bz_min, int bz_max) {
    if (bx < bx_min) {
        return FALSE;
    }

    if (bx > bx_max) {
        return FALSE;
    }

    if (bz < bz_min) {
        return FALSE;
    }

    if (bz > bz_max) {
        return FALSE;
    }

    return TRUE;
}

typedef struct block_group_s {
    int min;
    int max;
} mRF_BlockGroup_c;

static int mRF_CheckBlockGroup(u8 type, int group) {
    static mRF_BlockGroup_c blockGroup[9] = {
        { mFM_BLOCK_TYPE_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_CLIFF_BOTTOM_LEFT_CORNER },
        { mFM_BLOCK_TYPE_RIVER_SOUTH, mFM_BLOCK_TYPE_RIVER_WEST_SOUTH },
        { mFM_BLOCK_TYPE_RIVER_SOUTH_BRIDGE, mFM_BLOCK_TYPE_RIVER_WEST_SOUTH_BRIDGE },
        { mFM_BLOCK_TYPE_SLOPE_HORIZONTAL, mFM_BLOCK_TYPE_SLOPE_BOTTOM_LEFT_CORNER },
        { mFM_BLOCK_TYPE_WATERFALL_STRAIGHT_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_WATERFALL_WEST_CLIFF_BOTTOM_LEFT_CORNER },
        { mFM_BLOCK_TYPE_WATERFALL_STRAIGHT_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_RIVER_STRAIGHT_CLIFF_BOTTOM_LEFT_CORNER },
        { mFM_BLOCK_TYPE_RIVER_STRAIGHT_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_RIVER_EAST_CLIFF_TOP_LEFT_CORNER },
        { mFM_BLOCK_TYPE_RIVER_WEST_CLIFF_HORIZONTAL, mFM_BLOCK_TYPE_WATERFALL_WEST_CLIFF_BOTTOM_LEFT_CORNER },
        { 0, 0 }
    };

    if (group == mRF_BLOCK_GROUP_CLIFF_ANY) {
        /* Check any acre with a cliff in it, even if it has rivers or slopes */
        if ((type >= blockGroup[mRF_BLOCK_GROUP_CLIFF].min && type <= blockGroup[mRF_BLOCK_GROUP_CLIFF].max) ||
            (type >= blockGroup[mRF_BLOCK_GROUP_SLOPE].min && type <= blockGroup[mRF_BLOCK_GROUP_SLOPE].max) ||
            (type >= blockGroup[mRF_BLOCK_GROUP_RIVER_CLIFF_ANY].min &&
             type <= blockGroup[mRF_BLOCK_GROUP_RIVER_CLIFF_ANY].max)) {
            return TRUE;
        }
        return FALSE;
    } else {
        mRF_BlockGroup_c block_group = blockGroup[group];

        if (type >= block_group.min && type <= block_group.max) {
            return TRUE;
        }
    }

    return FALSE;
}

static void mRF_CpyBlockData(u8* dst, u8* src) {
    int i;

    for (i = 0; i < BLOCK_TOTAL_NUM; i++) {
        *dst++ = *src++;
    }
}

static int mRF_TraceCliffBlock(u8* block_data, int bx, int bz) {
    int n_bx0;
    int n_bz0;
    int n_bx1;
    int n_bz1;

    int block_num = mRF_D2ToD1(bx, bz);
    u8 type = block_data[block_num] - mFM_BLOCK_TYPE_CLIFF_HORIZONTAL;
    int res = 0;
    int i;

    while (res == 0) {
        int next_idx = mRF_GetRandom(l_cliff_next_data[type]->count);
        u8 next = l_cliff_next_data[type]->types[next_idx];
        int next_direct;

        mRF_Direct2BlockNo(&n_bx0, &n_bz0, bx, bz, l_cliff_next_direct[type]);
        mRF_Direct2BlockNo(&n_bx1, &n_bz1, n_bx0, n_bz0,
                           l_cliff_next_direct[(u8)(next - mFM_BLOCK_TYPE_CLIFF_HORIZONTAL)]);

        if (mRF_CheckCorrectBlockNo(n_bx0, n_bz0, 1, 5, 2, 5) != FALSE &&
            mRF_CheckCorrectBlockNo(n_bx1, n_bz1, 1, 6, 2, 5) != 0) {
            if (block_data[mRF_D2ToD1(n_bx1, n_bz1)] != mFM_BLOCK_TYPE_PLAYER_HOUSE &&
                block_data[mRF_D2ToD1(n_bx0, n_bz0)] == mFM_BLOCK_TYPE_FLAT) {
                block_data[mRF_D2ToD1(n_bx0, n_bz0)] = next;

                if (n_bx0 == 5) {
                    res = 2; // final cliff reached
                } else {
                    res = 1; // still tracing cliff
                }
            } else {
                return FALSE;
            }
        } else {
            return 0;
        }
    }

    if (res == 1) {
        /* Trace next cliff block */
        if (mRF_TraceCliffBlock(block_data, n_bx0, n_bz0) == TRUE) {
            return TRUE;
        }
    } else if (res == 2) {
        return TRUE; // done tracing cliff
    }

    return FALSE;
}

static void mRF_SetEndCliffBlock(u8* block_data) {
    int end_z = 0;
    u8 direct;
    int i;
    int cliff_type = 0;

    for (i = 0; i < FG_BLOCK_Z_NUM; i++) {
        int bnum = mRF_D2ToD1(5, i);
        u8 type = block_data[bnum] - mFM_BLOCK_TYPE_CLIFF_HORIZONTAL;

        if (block_data[bnum] >= mFM_BLOCK_TYPE_CLIFF_HORIZONTAL &&
            block_data[bnum] <= mFM_BLOCK_TYPE_CLIFF_BOTTOM_LEFT_CORNER) {
            end_z = i;
            cliff_type = type;
        }
    }

    direct = l_cliff_next_direct[cliff_type];
    if (direct != mRF_DIRECT_EAST) {
        /* Cliff does not end facing the eastern wall, so we must fix that */
        // mRF_trace_data_c* trace = l_cliff_next_data[cliff_type];
        u8* types = l_cliff_next_data[cliff_type]->types;

        for (i = 0; i < l_cliff_next_data[cliff_type]->count; i++) {
            u32 t = types[i] - mFM_BLOCK_TYPE_CLIFF_HORIZONTAL;

            if (l_cliff_next_direct[t] == mRF_DIRECT_EAST) {
                int bx;
                int bz;

                mRF_Direct2BlockNo(&bx, &bz, 5, end_z, direct);
                block_data[mRF_D2ToD1(bx, bz)] = types[i];
                block_data[mRF_D2ToD1(bx + 1, bz)] = mFM_BLOCK_TYPE_BORDER_CLIFF_RIGHT_TRANSITION;
                break;
            }
        }
    } else {
        block_data[mRF_D2ToD1(6, end_z)] = mFM_BLOCK_TYPE_BORDER_CLIFF_RIGHT_TRANSITION;
    }
}

static int mRF_LastCheckCliff(u8* block_data, int bx, int bz) {
    int n_bx;
    int n_bz;
    int last_x;
    int last_z;
    u8 type;

    last_x = bx;
    last_z = bz;
    type = block_data[mRF_D2ToD1(bx, bz)];

    while (mRF_CheckBlockGroup(type, mRF_BLOCK_GROUP_CLIFF) != FALSE) {
        u32 idx = type - mFM_BLOCK_TYPE_CLIFF_HORIZONTAL;
        u8 direct = l_cliff_next_direct[idx];

        mRF_Direct2BlockNo(&n_bx, &n_bz, last_x, last_z, direct);
        type = block_data[mRF_D2ToD1(n_bx, n_bz)];
        last_x = n_bx;
        last_z = n_bz;
    }

    if (last_x > 5) {
        return bz != last_z; // Cliff cannot start and end on the same Z acre
    }

    return FALSE;
}

static int mRF_DecideBaseCliff(u8* block_data) {
    int start_z = mRF_GetRandom(4);
    int bz = start_z + 2;
    int idx = mRF_GetRandom(l_cliff_start_table[start_z]->count);
    u8 start_type = l_cliff_start_table[start_z]->types[idx];

    block_data[mRF_D2ToD1(1, bz)] = start_type;
    block_data[mRF_D2ToD1(0, bz)] = mFM_BLOCK_TYPE_BORDER_CLIFF_LEFT_TRANSITION;
    mRF_TraceCliffBlock(block_data, 1, bz);
    mRF_SetEndCliffBlock(block_data);

    return mRF_LastCheckCliff(block_data, 1, bz) != FALSE;
}

static u8 mRF_RiverAlbuminCliff(u8 cliff_type, u8 river_type) {
    static u8 river1_album_data[7] = { mFM_BLOCK_TYPE_WATERFALL_STRAIGHT_CLIFF_HORIZONTAL,
                                       mFM_BLOCK_TYPE_WATERFALL_STRAIGHT_CLIFF_BOTTOM_RIGHT_CORNER,
                                       mFM_BLOCK_TYPE_RIVER_STRAIGHT_CLIFF_VERTICAL_RIGHT,
                                       mFM_BLOCK_TYPE_RIVER_STRAIGHT_CLIFF_TOP_RIGHT_CORNER,
                                       mFM_BLOCK_TYPE_WATERFALL_STRAIGHT_CLIFF_TOP_LEFT_CORNER,
                                       mFM_BLOCK_TYPE_RIVER_STRAIGHT_CLIFF_VERTICAL_LEFT,
                                       mFM_BLOCK_TYPE_RIVER_STRAIGHT_CLIFF_BOTTOM_LEFT_CORNER };

    static u8 river2_album_data[7] = { mFM_BLOCK_TYPE_RIVER_STRAIGHT_CLIFF_HORIZONTAL,
                                       mFM_BLOCK_TYPE_WATERFALL_EAST_CLIFF_BOTTOM_RIGHT_CORNER,
                                       mFM_BLOCK_TYPE_WATERFALL_EAST_CLIFF_VERTICAL_RIGHT,
                                       mFM_BLOCK_TYPE_RIVER_EAST_CLIFF_TOP_RIGHT_CORNER,
                                       mFM_BLOCK_TYPE_RIVER_EAST_CLIFF_TOP_LEFT_CORNER,
                                       mFM_BLOCK_TYPE_NONE,
                                       mFM_BLOCK_TYPE_NONE };

    static u8 river3_album_data[7] = { mFM_BLOCK_TYPE_RIVER_WEST_CLIFF_HORIZONTAL,
                                       mFM_BLOCK_TYPE_NONE,
                                       mFM_BLOCK_TYPE_NONE,
                                       mFM_BLOCK_TYPE_RIVER_WEST_CLIFF_TOP_RIGHT_CORNER,
                                       mFM_BLOCK_TYPE_RIVER_WEST_CLIFF_TOP_LEFT_CORNER,
                                       mFM_BLOCK_TYPE_WATERFALL_WEST_CLIFF_VERTICAL_LEFT,
                                       mFM_BLOCK_TYPE_WATERFALL_WEST_CLIFF_BOTTOM_LEFT_CORNER };

    static u8 river_no_album_data[7] = { mFM_BLOCK_TYPE_NONE, mFM_BLOCK_TYPE_NONE, mFM_BLOCK_TYPE_NONE,
                                         mFM_BLOCK_TYPE_NONE, mFM_BLOCK_TYPE_NONE, mFM_BLOCK_TYPE_NONE,
                                         mFM_BLOCK_TYPE_NONE };

    static u8* river_cliff_album_data[7] = {
        river1_album_data,   /* south river */
        river2_album_data,   /* east river */
        river3_album_data,   /* west river */
        river_no_album_data, /* south -> east river */
        river_no_album_data, /* east -> south river */
        river_no_album_data, /* south -> west river */
        river_no_album_data  /* west -> south river */
    };

    if (mRF_CheckBlockGroup(cliff_type, mRF_BLOCK_GROUP_CLIFF) != FALSE &&
        mRF_CheckBlockGroup(river_type, mRF_BLOCK_GROUP_RIVER) != FALSE) {
        int river = (u32)(river_type - mFM_BLOCK_TYPE_RIVER_SOUTH);
        int cliff = (u32)(cliff_type - mFM_BLOCK_TYPE_CLIFF_HORIZONTAL);

        if (river >= 0 && river < 7 && cliff >= 0 && cliff < 7) {
            return river_cliff_album_data[river][cliff];
        }
    }

    return mFM_BLOCK_TYPE_NONE;
}

static int mRF_DecideRiverAlbuminCliff(u8* cliff_blocks, u8* river_blocks) {
    int bx;
    int bz;

    for (bz = 0; bz < BLOCK_Z_NUM - 2; bz++) {
        for (bx = 0; bx < BLOCK_X_NUM; bx++) {
            int block_num = mRF_D2ToD1(bx, bz);
            u8 river_type = river_blocks[block_num];
            u8 river_album_type = mRF_RiverAlbuminCliff(cliff_blocks[block_num], river_type);

            if (river_album_type != mFM_BLOCK_TYPE_NONE) {
                cliff_blocks[block_num] = river_album_type;
            } else if (mRF_CheckBlockGroup(river_type, mRF_BLOCK_GROUP_RIVER) != FALSE ||
                       river_type == mFM_BLOCK_TYPE_BORDER_CLIFF_RIVER || river_type == mFM_BLOCK_TYPE_TRACKS_RIVER) {
                cliff_blocks[block_num] = river_blocks[block_num];
            }
        }
    }

    return TRUE;
}

static int mRF_GetCenterCrossZoneRiverCount(u8* block_data) {
    int i;
    int cross_count = 0;

    for (i = 2; i <= (BLOCK_X_NUM - 2); i++) {
        /* Check if the river enters the any 3 acre */
        if (mRF_CheckBlockGroup(block_data[mRF_D2ToD1(3, i)], mRF_BLOCK_GROUP_RIVER) != FALSE) {
            cross_count++;
        }
    }

    return cross_count;
}

static int mRF_TraceRiverPart2(u8* river_data, u8* cliff_data, int bx, int bz, u8* challenge_flag) {
    u8 river_type = river_data[mRF_D2ToD1(bx, bz)] - mFM_BLOCK_TYPE_RIVER_SOUTH;
    int next_direct = mRF_RiverIdx2NextDirect(river_type);
    int res = 0;
    int n_bx0;
    int n_bz0;
    int n_bx1;
    int n_bz1;

    while (res == 0) {
        u8* next_river_types = l_river_next_data[river_type]->types;
        u8 next_type = next_river_types[mRF_GetRandom(l_river_next_data[river_type]->count)];
        int next_next_direct = mRF_RiverIdx2NextDirect((u8)(next_type - mFM_BLOCK_TYPE_RIVER_SOUTH));

        mRF_Direct2BlockNo(&n_bx0, &n_bz0, bx, bz, next_direct);
        if (n_bz0 == 6) {
            next_type = mFM_BLOCK_TYPE_RIVER_SOUTH;
            next_next_direct = mRF_RiverIdx2NextDirect(0);
        }

        mRF_Direct2BlockNo(&n_bx1, &n_bz1, n_bx0, n_bz0, next_next_direct);

        if (mRF_CheckCorrectBlockNo(n_bx0, n_bz0, 1, 5, 1, 6) != FALSE) {
            if (mRF_CheckCorrectBlockNo(n_bx1, n_bz1, 1, 5, 1, 7) != FALSE) {
                int next_bnum = mRF_D2ToD1(n_bx0, n_bz0);
                int next_next_bnum = mRF_D2ToD1(n_bx1, n_bz1);

                if (cliff_data[next_next_bnum] != mFM_BLOCK_TYPE_PLAYER_HOUSE) {
                    int wrote = FALSE;

                    if (mRF_CheckBlockGroup(cliff_data[next_bnum], mRF_BLOCK_GROUP_CLIFF) != FALSE) {
                        if (mRF_RiverAlbuminCliff(cliff_data[next_bnum], next_river_types[0]) != mFM_BLOCK_TYPE_NONE) {
                            river_data[next_bnum] = next_river_types[0];
                            wrote = TRUE;
                        } else {
                            return FALSE;
                        }
                    } else {
                        river_data[next_bnum] = next_type;
                        wrote = TRUE;
                    }

                    if (wrote != FALSE) {
                        if (n_bz1 == 7) {
                            res = 2; // river has reached the ocean
                        } else {
                            res = 1; // continue tracing river
                        }
                    }
                }
            }
        } else {
            return FALSE;
        }
    }

    if (res == 1) {
        if (mRF_TraceRiverPart2(river_data, cliff_data, n_bx0, n_bz0, challenge_flag) == TRUE) {
            return TRUE;
        }
    } else if (res == 2) {
        if (mRF_GetCenterCrossZoneRiverCount(river_data) == 0) {
            return FALSE; // river did not cross the center-line on the X coordinate
        }

        if (n_bx0 == 1 || n_bx0 == 5) {
            return FALSE; // river cannot end in F-1 or F-5 acres
        }

        return TRUE;
    }

    return FALSE;
}

static int mRF_TraceRiverPart1(u8* river_blocks, u8* cliff_blocks, int* start_bx, int* start_bz) {
    static int startX_table[4] = { 1, 2, 4, 5 };

    int n_bx0;
    int n_bz0;
    int n_bx1;
    int n_bz1;
    int end = FALSE;

    while (end == FALSE) {
        int bx = startX_table[mRF_GetRandom(4)];
        u8* start_types = l_river_next_data[0]->types;
        u8 start_type = start_types[mRF_GetRandom(l_river_next_data[0]->count)];
        int next_bnum;
        int next_next_bnum;

        mRF_Direct2BlockNo(&n_bx0, &n_bz0, bx, 1, mRF_DIRECT_SOUTH);
        next_bnum = mRF_D2ToD1(n_bx0, n_bz0);

        mRF_Direct2BlockNo(&n_bx1, &n_bz1, n_bx0, n_bz0,
                           mRF_RiverIdx2NextDirect((u8)(start_type - mFM_BLOCK_TYPE_RIVER_SOUTH)));
        next_next_bnum = mRF_D2ToD1(n_bx1, n_bz1);

        if (mRF_CheckCorrectBlockNo(n_bx0, n_bz0, 1, 5, 1, 6) != FALSE &&
            cliff_blocks[next_bnum] != mFM_BLOCK_TYPE_PLAYER_HOUSE) {
            if (mRF_CheckBlockGroup(cliff_blocks[next_bnum], mRF_BLOCK_GROUP_CLIFF) != FALSE) {
                if (mRF_RiverAlbuminCliff(cliff_blocks[next_bnum], mFM_BLOCK_TYPE_RIVER_SOUTH) != mFM_BLOCK_TYPE_NONE) {
                    if (cliff_blocks[mRF_D2ToD1(n_bx0, n_bz0 + 1)] != mFM_BLOCK_TYPE_PLAYER_HOUSE) {
                        river_blocks[next_bnum] = mFM_BLOCK_TYPE_RIVER_SOUTH;
                        start_bx[0] = n_bx0;
                        start_bz[0] = n_bz0;
                        end = TRUE;
                    }
                } else {
                    return FALSE;
                }
            } else if (cliff_blocks[next_next_bnum] != mFM_BLOCK_TYPE_PLAYER_HOUSE) {
                river_blocks[next_bnum] = start_type;
                start_bx[0] = n_bx0;
                start_bz[0] = n_bz0;
                end = TRUE;
            }
        }

        if (end == TRUE) {
            int border_river_n = mRF_D2ToD1(bx, 0);
            int rail_river_n = mRF_D2ToD1(bx, 1);

            river_blocks[border_river_n] = mFM_BLOCK_TYPE_BORDER_CLIFF_RIVER;
            river_blocks[rail_river_n] = mFM_BLOCK_TYPE_TRACKS_RIVER;
        }
    }

    return TRUE;
}

static void mRF_InitFlag(u8* flags, int num) {
    int i;

    for (i = 0; i < num; i++) {
        *flags++ = 0;
    }
}

static int mRF_LastCheckRiver(u8* river_blocks, int bx, int bz) {
    int x = bx;
    int z = bz;
    int bnum = mRF_D2ToD1(bx, bz);
    u8 type = river_blocks[bnum];

    /* Re-trace each river block */
    while (mRF_CheckBlockGroup(type, mRF_BLOCK_GROUP_RIVER) != FALSE) {
        int n_bx;
        int n_bz;
        u8 direct = mRF_RiverIdx2NextDirect(type - mFM_BLOCK_TYPE_RIVER_SOUTH);

        mRF_Direct2BlockNo(&n_bx, &n_bz, x, z, direct);
        type = river_blocks[mRF_D2ToD1(n_bx, n_bz)];
        x = n_bx;
        z = n_bz;
    }

    if (z > 6) {
        return TRUE;
    }

    return FALSE; // river must have ended at the beach acres
}

static int mRF_DecideBaseRiver(u8* river_blocks, u8* cliff_blocks) {
    static u8 keep_blocks[BLOCK_TOTAL_NUM];
    static u8 challenge_flag[BLOCK_TOTAL_NUM];

    int start_bx;
    int start_bz;

    mRF_CpyBlockData(keep_blocks, cliff_blocks);
    mRF_CpyBlockData(river_blocks, keep_blocks);
    mRF_CpyBlockData(cliff_blocks, keep_blocks); // ???

    if (mRF_TraceRiverPart1(river_blocks, cliff_blocks, &start_bx, &start_bz) == FALSE) {
        return FALSE;
    }

    mRF_InitFlag(challenge_flag, (BLOCK_Z_NUM - 2) * BLOCK_X_NUM);
    if (mRF_TraceRiverPart2(river_blocks, cliff_blocks, start_bx, start_bz, challenge_flag) != FALSE) {
        return mRF_LastCheckRiver(river_blocks, start_bx, start_bz);
    }

    return FALSE;
}

static int mRF_SetRandomBlockData(u8* river_blocks, u8* cliff_blocks) {
    if (mRF_DecideBaseCliff(cliff_blocks) != FALSE) {
        return mRF_DecideBaseRiver(river_blocks, cliff_blocks);
    }

    return FALSE;
}

static int mRF_GetRiverCrossCliffInfo(int* bx, int* bz, u8* blocks) {
    static u8 cross_data[7] = { mFM_BLOCK_TYPE_WATERFALL_STRAIGHT_CLIFF_HORIZONTAL,
                                mFM_BLOCK_TYPE_WATERFALL_STRAIGHT_CLIFF_BOTTOM_RIGHT_CORNER,
                                mFM_BLOCK_TYPE_WATERFALL_STRAIGHT_CLIFF_TOP_LEFT_CORNER,
                                mFM_BLOCK_TYPE_WATERFALL_EAST_CLIFF_BOTTOM_RIGHT_CORNER,
                                mFM_BLOCK_TYPE_WATERFALL_EAST_CLIFF_VERTICAL_RIGHT,
                                mFM_BLOCK_TYPE_WATERFALL_WEST_CLIFF_VERTICAL_LEFT,
                                mFM_BLOCK_TYPE_WATERFALL_WEST_CLIFF_BOTTOM_LEFT_CORNER };

    int i;
    int j;
    int crossings = 0;

    for (i = 0; i < ((BLOCK_Z_NUM - 2) * BLOCK_X_NUM); i++) {
        for (j = 0; j < 7; j++) {
            if (cross_data[j] == blocks[i]) {
                /* Only log the first crossing position */
                if (crossings == 0) {
                    bz[0] = i / BLOCK_X_NUM;
                    bx[0] = i % BLOCK_X_NUM;
                }

                crossings++;
            }
        }
    }

    return crossings;
}

static int mRF_SetBridgeBlock(u8* blocks, int stepmode) {
    int before_cross;
    int after_cross;
    int bx;
    int bz;
    int cross_bx;
    int cross_bz;
    int flags;
    u8* blocks_p;
    u8* blocks_p2;
    int rng;
    int cross_num;

    before_cross = 0;
    after_cross = 0;
    flags = 0;
    blocks_p = blocks;
    blocks_p2 = blocks;
    rng = mRF_GetRandom(10) & 1;
    mRF_GetRiverCrossCliffInfo(&cross_bx, &cross_bz, blocks);

    for (bz = 0; bz < (BLOCK_Z_NUM - 2); bz++) {
        for (bx = 0; bx < BLOCK_X_NUM; bx++) {
            if (mRF_CheckBlockGroup(*blocks, mRF_BLOCK_GROUP_RIVER) != FALSE) {
                if (bz < cross_bz) {
                    before_cross++;
                } else if (bz > cross_bz) {
                    after_cross++;
                }
            }

            blocks++;
        }
    }

    if (before_cross != 0) {
        int selected = mRF_GetRandom(before_cross);
        int cross_num = 0;

        for (bz = 0; bz < (BLOCK_Z_NUM - 2); bz++) {
            for (bx = 0; bx < BLOCK_X_NUM; bx++) {
                if (mRF_CheckBlockGroup(*blocks_p, mRF_BLOCK_GROUP_RIVER) != FALSE && bz < cross_bz) {
                    if (cross_num == selected) {
                        blocks_p[0] += 7; // change to a bridge block type
                        flags |= mRF_TO_BIT(BRIDGE_UPPER);
                        // no break?
                    }

                    cross_num++;
                }

                blocks_p++;
            }
        }
    }

    if (after_cross != 0 && stepmode == mRF_STEPMODE_TWO && rng != 0) {
        int selected = mRF_GetRandom(after_cross);
        int cross_num = 0;

        for (bz = 0; bz < (BLOCK_Z_NUM - 2); bz++) {
            for (bx = 0; bx < BLOCK_X_NUM; bx++) {
                if (mRF_CheckBlockGroup(*blocks_p2, mRF_BLOCK_GROUP_RIVER) != FALSE && bz > cross_bz) {
                    if (cross_num == selected) {
                        blocks_p2[0] += 7; // change to a bridge block type
                        flags |= mRF_TO_BIT(BRIDGE_LOWER);
                        // no break?
                    }

                    cross_num++;
                }

                blocks_p2++;
            }
        }
    }

    return flags;
}

static s16 l_use_data[BLOCK_TOTAL_NUM];

static void mRF_GetBlockBase(u8* base_table, u8* blocks) {
    int bx;

    for (bx = 0; bx < BLOCK_X_NUM; bx++) {
        int bz;
        int height = mRF_FIELD_STEP1;

        for (bz = (BLOCK_Z_NUM - 1); bz >= 0; bz--) {
            u8 type = blocks[mRF_D2ToD1(bx, bz)];

            base_table[mRF_D2ToD1(bx, bz)] = height;

            if ((mRF_GetSystemBlockInfo(type) & 1) != 0 || (mRF_GetSystemBlockInfo(type) & 8) != 0 ||
                (mRF_GetSystemBlockInfo(type) & 16) != 0 || (type == mFM_BLOCK_TYPE_BORDER_CLIFF_LEFT_TRANSITION) ||
                (type == mFM_BLOCK_TYPE_BORDER_CLIFF_RIGHT_TRANSITION)) {
                height++;
            }
        }
    }
}

static void mRF_InitBlockBase(u8* base_table) {
    int i;

    for (i = 0; i < BLOCK_TOTAL_NUM; i++) {
        *base_table++ = mRF_FIELD_STEP1;
    }
}

static void mRF_SetShortData(s16* data, s16 value, int num) {
    int i;

    for (i = 0; i != num; i++) {
        *data++ = value;
    }
}

static int mRF_SearchAlreadyUse(s16 value) {
    int i;

    for (i = 0; i < BLOCK_TOTAL_NUM; i++) {
        if (value == l_use_data[i]) {
            return TRUE;
        }
    }

    return FALSE;
}

static int mRF_TypeCombCount(mFM_combo_info_c* combi_info, int combi_count, u8 type, s16 reuse) {
    int i;
    int count = 0;

    for (i = 0; i < combi_count; i++) {
        if (type == combi_info[i].type) {
            if (reuse == FALSE) {
                if (mRF_SearchAlreadyUse(i) == FALSE) {
                    count++;
                }
            } else if (reuse == TRUE) {
                count++;
            }
        }
    }

    return count;
}

static s16 mRF_IndexInType2BlockNo(mFM_combo_info_c* combi_info, int combi_count, u8 type, int idx, s16 reuse) {
    int i;
    int count = 0;

    for (i = 0; i < combi_count; i++) {
        if (type == combi_info[i].type) {
            if (reuse == FALSE) {
                if (mRF_SearchAlreadyUse(i) == FALSE) {
                    if (count == idx) {
                        return i;
                    }

                    count++;
                }
            } else if (reuse == TRUE) {
                if (count == idx) {
                    return i;
                }

                count++;
            }
        }
    }

    return -1;
}

static mActor_name_t mRF_GetExceptionalSeaBgDownBgName(mActor_name_t sea_bg) {
    static mActor_name_t exceptional_table[27][2] = {
        { 0x0050, 0x0051 }, { 0x0055, 0x0056 }, { 0x0078, 0x0090 }, { 0x0079, 0x0091 }, { 0x007a, 0x0092 },
        { 0x007b, 0x0093 }, { 0x007c, 0x0094 }, { 0x007d, 0x0095 }, { 0x007e, 0x0096 }, { 0x007f, 0x0097 },
        { 0x0080, 0x0098 }, { 0x0081, 0x0099 }, { 0x0082, 0x009c }, { 0x0083, 0x009d }, { 0x0084, 0x009e },
        { 0x0085, 0x009f }, { 0x0086, 0x00a0 }, { 0x0087, 0x00a1 }, { 0x0088, 0x00a2 }, { 0x0089, 0x00a3 },
        { 0x008a, 0x00a4 }, { 0x008b, 0x00a5 }, { 0x008c, 0x00a6 }, { 0x008d, 0x00a7 }, { 0x008e, 0x00a8 },
        { 0x008f, 0x00a9 }, { 0x0125, 0x0125 }
    };

    mActor_name_t* exceptional_p = exceptional_table[0];

    while (exceptional_p[0] != 0x125) {
        if (exceptional_p[0] == sea_bg) {
            return exceptional_p[1];
        }

        exceptional_p += 2;
    }

    return sea_bg;
}

static u16 mRF_BgName2RandomConbiNo(mActor_name_t bg_name, mFM_combo_info_c* combi_info, int combi_count) {
    int count = 0;
    mFM_combo_info_c* combi_info_p;
    int i;

    combi_info_p = combi_info;
    for (i = 0; i < combi_count; i++) {
        if (combi_info_p->bg_id == bg_name && combi_info_p->type != mFM_BLOCK_TYPE_NONE) {
            count++;
        }

        combi_info_p++;
    }

    if (count != 0) {
        int n_count = 0;

/* @BUG - this always selects the first entry instead of a random one */
#ifndef BUGFIXES
        int selected = mRF_GetRandom(0); // what the hell?
#else
        int selected = mRF_GetRandom(count);
#endif

        combi_info_p = combi_info;
        for (i = 0; i < combi_count; i++) {
            if (combi_info_p->bg_id == bg_name && combi_info_p->type != 0xFF) {
                if (selected == n_count) {
                    return i;
                }

                n_count++;
            }

            combi_info_p++;
        }
    }

    return combi_count;
}

static void mRF_SelectBlock(mFM_combination_c* combi_table, mFM_combo_info_c* combi_info, int combi_count, u8* blocks) {
    int bx;
    int bz;

    mRF_SetShortData(l_use_data, -1, BLOCK_TOTAL_NUM);

    for (bz = 0; bz < BLOCK_Z_NUM; bz++) {
        for (bx = 0; bx < BLOCK_X_NUM; bx++) {
            int bnum = mRF_D2ToD1(bx, bz);
            u8 type = blocks[mRF_D2ToD1(bx, bz)];

            if (type == mFM_BLOCK_TYPE_SEA_EXCEPTIONAL) {
                mActor_name_t down_bg_name = mRF_GetExceptionalSeaBgDownBgName(
                    combi_info[combi_table[mRF_D2ToD1(bx, bz - 1)].combination_type].bg_id);
                combi_table[bnum].combination_type = mRF_BgName2RandomConbiNo(down_bg_name, combi_info, combi_count);
            } else {
                int type_count = mRF_TypeCombCount(combi_info, combi_count, type, FALSE);
                int selected;
                s16 block_no;

                if (type_count != 0) {
                    selected = mRF_GetRandom(type_count);
                    block_no = mRF_IndexInType2BlockNo(combi_info, combi_count, type, selected, FALSE);

                    if (block_no != -1) {
                        combi_table[bnum].combination_type = block_no;
                        l_use_data[bnum] = block_no;
                    } else {
                        selected = mRF_GetRandom(mRF_TypeCombCount(combi_info, combi_count, type, TRUE));
                        block_no = mRF_IndexInType2BlockNo(combi_info, combi_count, type, selected, TRUE);

                        if (block_no != -1) {
                            combi_table[bnum].combination_type = block_no;
                            l_use_data[bnum] = block_no;
                        } else {
                            combi_table[bnum].combination_type = 161;
                        }
                    }
                } else {
                    selected = mRF_GetRandom(mRF_TypeCombCount(combi_info, combi_count, type, TRUE));
                    block_no = mRF_IndexInType2BlockNo(combi_info, combi_count, type, selected, TRUE);

                    if (block_no != -1) {
                        combi_table[bnum].combination_type = block_no;
                        l_use_data[bnum] = block_no;
                    } else {
                        combi_table[bnum].combination_type = 161;
                    }
                }
            }

            if (type == mFM_BLOCK_TYPE_ISLAND_RIGHT || type == mFM_BLOCK_TYPE_ISLAND_LEFT) {
                mActor_name_t bg_id = combi_info[combi_table[bnum].combination_type].bg_id;

                if (type == mFM_BLOCK_TYPE_ISLAND_LEFT) {
                    Save_Get(island).bg_data[0] = bg_id - 116;
                } else {
                    Save_Get(island).bg_data[1] = bg_id - 112;
                }
            }
        }
    }
}

static void mRF_CopyBlockBaseHeightData(mFM_combination_c* combi_table, u8* base_height) {
    int i;

    for (i = 0; i < BLOCK_TOTAL_NUM; i++) {
        combi_table->height = *base_height;
        combi_table++;
        base_height++;
    }
}

static void mRF_MakeBaseLandformStep2(u8* cliff_blocks, u8* river_blocks) {
    mRF_CpyBlockData(cliff_blocks, l_base_blocks);
    mRF_CpyBlockData(river_blocks, l_base_blocks);

    while (mRF_SetRandomBlockData(river_blocks, cliff_blocks) == FALSE) {
        mRF_CpyBlockData(cliff_blocks, l_base_blocks);
        mRF_CpyBlockData(river_blocks, l_base_blocks);
    }

    mRF_DecideRiverAlbuminCliff(cliff_blocks, river_blocks);
}

static int mRF_CountDirectedInfoCliff(u8* blocks, int bx, int bz, int half) {
    int x;
    int z;
    int bnum;
    u8 type;
    int side;
    int count;
    int n_bx;
    int n_bz;

    x = bx + 1;
    z = bz;
    bnum = mRF_D2ToD1(x, z);
    side = mRF_RIVER_SIDE_LEFT;
    count = 0;
    type = blocks[bnum];

    while (type != mFM_BLOCK_TYPE_BORDER_CLIFF_RIGHT_TRANSITION) {
        int idx = mRF_BlockType2CliffIndex(type);

        if (mRF_CheckBlockGroup(type, mRF_BLOCK_GROUP_RIVER_CLIFF_ANY) != FALSE) {
            /* We just hit the river, so we're on the right "half" of the town now */
            side = mRF_RIVER_SIDE_RIGHT;
        } else if (side == half) {
            count++;
        }

        mRF_Direct2BlockNo(&n_bx, &n_bz, x, z, l_cliff_next_direct[idx]);
        mRF_D2ToD1(n_bx, n_bz); // why is this here? maybe setting bnum?
        x = n_bx;
        z = n_bz;
        type = blocks[mRF_D2ToD1(n_bx, n_bz)];
    }

    return count;
}

static int mRF_SetSlopeDirectedInfoCliff(u8* blocks, int bx, int bz, int half, int selected_idx) {
    int x;
    int z;
    int bnum;
    u8 type;
    int side;
    int count;
    int n_bx;
    int n_bz;

    x = bx + 1;
    z = bz;
    bnum = mRF_D2ToD1(x, z);
    side = mRF_RIVER_SIDE_LEFT;
    count = 0;
    type = blocks[bnum];

    while (type != mFM_BLOCK_TYPE_BORDER_CLIFF_RIGHT_TRANSITION) {
        int idx = mRF_BlockType2CliffIndex(type);

        if (mRF_CheckBlockGroup(type, mRF_BLOCK_GROUP_RIVER_CLIFF_ANY) != FALSE) {
            /* We just hit the river, so we're on the right "half" of the town now */
            side = mRF_RIVER_SIDE_RIGHT;
        } else if (side == half) {
            if (count == selected_idx) {
                blocks[bnum] = mFM_BLOCK_TYPE_SLOPE_HORIZONTAL + idx; // set slope
                return TRUE;
            }

            count++;
        }

        mRF_Direct2BlockNo(&n_bx, &n_bz, x, z, l_cliff_next_direct[idx]);
        mRF_D2ToD1(n_bx, n_bz); // why is this here? maybe setting bnum?
        x = n_bx;
        z = n_bz;
        bnum = mRF_D2ToD1(n_bx, n_bz);
        type = blocks[bnum];
    }

    return FALSE;
}

static int mRF_SetSlopeBlock(u8* blocks) {
    int bz;
    int flags = 0;

    for (bz = 0; bz < (BLOCK_Z_NUM - 2); bz++) {
        /* Check for start of cliff */
        if (blocks[mRF_D2ToD1(0, bz)] == mFM_BLOCK_TYPE_BORDER_CLIFF_LEFT_TRANSITION) {
            int possibilities = mRF_CountDirectedInfoCliff(blocks, 0, bz, mRF_RIVER_SIDE_LEFT); // left side of river
            int selected;

            if (possibilities != 0) {
                selected = mRF_GetRandom(possibilities);

                if (mRF_SetSlopeDirectedInfoCliff(blocks, 0, bz, mRF_RIVER_SIDE_LEFT, selected) != FALSE) {
                    flags |= mRF_TO_BIT(SLOPE_LEFT);
                }
            }

            possibilities = mRF_CountDirectedInfoCliff(blocks, 0, bz, mRF_RIVER_SIDE_RIGHT); // right side of river

            if (possibilities != 0) {
                selected = mRF_GetRandom(possibilities);

                if (mRF_SetSlopeDirectedInfoCliff(blocks, 0, bz, mRF_RIVER_SIDE_RIGHT, selected) != FALSE) {
                    flags |= mRF_TO_BIT(SLOPE_RIGHT);
                }
            }
        }
    }

    return flags;
}

static int mRF_SetBridgeAndSlopeBlock(u8* blocks, int stepmode) {
    int flags = 0;

    flags |= mRF_SetBridgeBlock(blocks, stepmode);
    flags |= mRF_SetSlopeBlock(blocks);
    return flags;
}

static void mRF_MakeBaseHeightTable(u8* base_height, u8* blocks) {
    mRF_InitBlockBase(base_height);
    mRF_GetBlockBase(base_height, blocks);
}

static s16 mRF_river_left_right_info[BLOCK_TOTAL_NUM];
static s16 mRF_cliff_up_down_info[BLOCK_TOTAL_NUM];

static void mRF_MakeFlatPlaceInfomation(u8* blocks) {
    int bnum;
    u8 type;
    int bx;
    int bz;
    s16 side;
    int i;

    for (i = 0; i < BLOCK_TOTAL_NUM; i++) {
        mRF_river_left_right_info[i] = mRF_RIVER_SIDE_BOTH; // not set
        mRF_cliff_up_down_info[i] = mRF_CLIFF_HEIGHT_BOTH;  // not set
    }

    /* Mark cliff heights */
    for (bx = 1; bx < (BLOCK_X_NUM - 1); bx++) {
        side = mRF_CLIFF_HEIGHT_ABOVE; // above cliff

        for (bz = 1; bz < (BLOCK_Z_NUM - 1); bz++) {
            bnum = mRF_D2ToD1(bx, bz);
            type = blocks[bnum];

            if (side == mRF_CLIFF_HEIGHT_ABOVE && mRF_CheckBlockGroup(type, mRF_BLOCK_GROUP_CLIFF_ANY) != FALSE) {
                side = mRF_CLIFF_HEIGHT_BELOW; // below cliff
            }

            mRF_cliff_up_down_info[bnum] = side;
        }
    }

    /* Mark river sides */
    for (bz = 1; bz < (BLOCK_Z_NUM - 1); bz++) {
        side = mRF_RIVER_SIDE_LEFT; // left of river

        for (bx = 1; bx < (BLOCK_X_NUM - 1); bx++) {
            bnum = mRF_D2ToD1(bx, bz);
            type = blocks[bnum];

            if (side == mRF_RIVER_SIDE_LEFT && (mRF_CheckBlockGroup(type, mRF_BLOCK_GROUP_RIVER) ||
                                                mRF_CheckBlockGroup(type, mRF_BLOCK_GROUP_RIVER_CLIFF_ANY))) {
                side = mRF_RIVER_SIDE_RIGHT; // right of river
            }

            mRF_river_left_right_info[bnum] = side;
        }
    }
}

static int mRF_JudgeFlatBlock(u8* blocks, int bnum, int river_side, int cliff_height) {
    if (river_side < mRF_RIVER_SIDE_LEFT || river_side >= mRF_RIVER_SIDE_NUM || cliff_height < mRF_CLIFF_HEIGHT_ABOVE ||
        cliff_height >= mRF_CLIFF_HEIGHT_NUM) {
        return FALSE;
    }

    if (bnum < 0 || bnum >= ((BLOCK_Z_NUM - 2) * BLOCK_X_NUM)) {
        return FALSE;
    }

    if (blocks[bnum] == mFM_BLOCK_TYPE_FLAT) {
        if (cliff_height != mRF_CLIFF_HEIGHT_BOTH) {
            if (river_side != mRF_RIVER_SIDE_BOTH) {
                if (river_side == mRF_river_left_right_info[bnum] && cliff_height == mRF_cliff_up_down_info[bnum]) {
                    return TRUE;
                }
            } else if (cliff_height == mRF_cliff_up_down_info[bnum]) {
                return TRUE;
            }
        } else if (river_side != mRF_RIVER_SIDE_BOTH) {
            if (river_side == mRF_river_left_right_info[bnum] && cliff_height == mRF_cliff_up_down_info[bnum]) {
                return TRUE;
            }
        } else if (cliff_height == mRF_cliff_up_down_info[bnum]) {
            return TRUE;
        }
    }

    return FALSE;
}

static int mRF_CountFlatBlock(u8* blocks, int river_side, int cliff_height) {
    int i;
    int count = 0;

    for (i = 0; i < ((BLOCK_Z_NUM - 2) * BLOCK_X_NUM); i++) {
        if (mRF_JudgeFlatBlock(blocks, i, river_side, cliff_height) != FALSE) {
            count++;
        }
    }

    return count;
}

static int mRF_RewriteFlatType(u8* blocks, int idx, u8 type, int river_side, int cliff_height) {
    int i;
    int count = 0;

    for (i = 0; i < ((BLOCK_Z_NUM - 2) * BLOCK_X_NUM); i++) {
        if (mRF_JudgeFlatBlock(blocks, i, river_side, cliff_height) != FALSE) {
            if (count == idx) {
                blocks[i] = type;
                return i;
            }

            count++;
        }
    }

    return -1;
}

static int mRF_FlatBlock2Unique(u8* blocks, u8 unique_type, int river_side, int cliff_height) {
    int num = mRF_CountFlatBlock(blocks, river_side, cliff_height);

    if (num != 0) {
        int selected = mRF_GetRandom(num);

        if (mRF_RewriteFlatType(blocks, selected, unique_type, river_side, cliff_height) != -1) {
            return TRUE;
        }
    }

    return FALSE;
}

static int mRF_SetNeedleworkAndWharfBlock(u8* blocks) {
    int flags = 0;
    int needlework_bx = mRF_GetRandom(3);
    int i;
    int bx = 0;

    if (blocks[mRF_D2ToD1(5, 6)] == mFM_BLOCK_TYPE_BEACH) {
        blocks[mRF_D2ToD1(5, 6)] = mFM_BLOCK_TYPE_PORT;
    } else {
        return 0;
    }

    for (i = 1; i < (BLOCK_X_NUM - 1); i++) {
        int bnum = mRF_D2ToD1(i, 6);

        if (blocks[bnum] == mFM_BLOCK_TYPE_BEACH) {
            if (needlework_bx == bx) {
                blocks[bnum] = mFM_BLOCK_TYPE_NEEDLEWORK;
                flags |= mRF_TO_BIT(NEEDLEWORK);
            }

            bx++;
        }
    }

    return flags;
}

static int mRF_SetUniqueFlatBlock(u8* blocks) {
    int flags = 0;
    int side0 = mRF_GetRandom(100) & 1;
    int side1 = (side0 ^ 1) & 1;

    /* Wishing Well should be on one side of the river below the cliff */
    if (mRF_FlatBlock2Unique(blocks, mFM_BLOCK_TYPE_SHRINE, side0, mRF_CLIFF_HEIGHT_BELOW)) {
        flags |= mRF_TO_BIT(SHRINE);
    } else if (mRF_FlatBlock2Unique(blocks, mFM_BLOCK_TYPE_SHRINE, side1, mRF_CLIFF_HEIGHT_BELOW)) {
        flags |= mRF_TO_BIT(SHRINE);
    }

    /* Police Station prefers to be on the opposite side of the river from the Wishing Well if possible */
    if (mRF_FlatBlock2Unique(blocks, mFM_BLOCK_TYPE_POLICE_BOX, side1, mRF_CLIFF_HEIGHT_BELOW)) {
        flags |= mRF_TO_BIT(POLICE);
    } else if (mRF_FlatBlock2Unique(blocks, mFM_BLOCK_TYPE_POLICE_BOX, side0, mRF_CLIFF_HEIGHT_BELOW)) {
        flags |= mRF_TO_BIT(POLICE);
    }

    /* Museum can be on either side of the river below the cliff */
    if (mRF_FlatBlock2Unique(blocks, mFM_BLOCK_TYPE_MUSEUM, mRF_RIVER_SIDE_BOTH, mRF_CLIFF_HEIGHT_BELOW)) {
        flags |= mRF_TO_BIT(MUSEUM);
    }

    return flags;
}

static int mRF_SetUniqueRailBlock(u8* blocks) {
    u8 t0;
    u8 t1;
    int set_t0;
    int set_t1;

    if (mRF_GetRandom(1000) & 1) {
        t0 = mFM_BLOCK_TYPE_TRACKS_SHOP;
        t1 = mFM_BLOCK_TYPE_TRACKS_POST_OFFICE;
    } else {
        t0 = mFM_BLOCK_TYPE_TRACKS_POST_OFFICE;
        t1 = mFM_BLOCK_TYPE_TRACKS_SHOP;
    }

    set_t0 = FALSE;
    set_t1 = FALSE;

    while (set_t0 == FALSE) {
        int bx = 1 + mRF_GetRandom(2);
        int bnum = mRF_D2ToD1(bx, 1); // A-1 or A-2

        if (blocks[bnum] == mFM_BLOCK_TYPE_TRACKS_DUMP) {
            blocks[bnum] = t0;
            set_t0 = TRUE;
        }
    }

    while (set_t1 == FALSE) {
        int bx = 4 + mRF_GetRandom(2);
        int bnum = mRF_D2ToD1(bx, 1); // A-4 or A-5

        if (blocks[bnum] == mFM_BLOCK_TYPE_TRACKS_DUMP) {
            blocks[bnum] = t1;
            set_t1 = TRUE;
        }
    }
}

static int mRF_CountPureRiver(u8* blocks) {
    int count = 0;
    int i;

    for (i = 0; i < ((BLOCK_Z_NUM - 2) * BLOCK_X_NUM); i++) {
        if (blocks[i] == mFM_BLOCK_TYPE_RIVER_SOUTH || blocks[i] == mFM_BLOCK_TYPE_RIVER_EAST ||
            blocks[i] == mFM_BLOCK_TYPE_RIVER_WEST || blocks[i] == mFM_BLOCK_TYPE_RIVER_SOUTH_EAST ||
            blocks[i] == mFM_BLOCK_TYPE_RIVER_EAST_SOUTH || blocks[i] == mFM_BLOCK_TYPE_RIVER_SOUTH_WEST ||
            blocks[i] == mFM_BLOCK_TYPE_RIVER_WEST_SOUTH) {
            count++;
        }
    }

    return count;
}

static int mRF_SetPoolDirectedRiverBlock(u8* blocks, int idx) {
    int count = 0;
    int i;

    for (i = 0; i < ((BLOCK_Z_NUM - 2) * BLOCK_X_NUM); i++) {
        if (blocks[i] == mFM_BLOCK_TYPE_RIVER_SOUTH || blocks[i] == mFM_BLOCK_TYPE_RIVER_EAST ||
            blocks[i] == mFM_BLOCK_TYPE_RIVER_WEST || blocks[i] == mFM_BLOCK_TYPE_RIVER_SOUTH_EAST ||
            blocks[i] == mFM_BLOCK_TYPE_RIVER_EAST_SOUTH || blocks[i] == mFM_BLOCK_TYPE_RIVER_SOUTH_WEST ||
            blocks[i] == mFM_BLOCK_TYPE_RIVER_WEST_SOUTH) {
            if (count == idx) {
                blocks[i] = 29 + blocks[i];
                return TRUE;
            }

            count++;
        }
    }

    return FALSE;
}

static int mRF_SetPoolBlock(u8* blocks) {
    int pure_river_num = mRF_CountPureRiver(blocks);

    if (pure_river_num != 0) {
        int selected = mRF_GetRandom(pure_river_num);

        if (mRF_SetPoolDirectedRiverBlock(blocks, selected) != FALSE) {
            return mRF_TO_BIT(POOL);
        }
    }

    return 0;
}

static void mRF_SetMarinBlock(u8* blocks) {
    int i;

    for (i = 1; i < (BLOCK_X_NUM - 1); i++) {
        int bnum = mRF_D2ToD1(i, 6);

        if (blocks[bnum] == mFM_BLOCK_TYPE_FLAT) {
            blocks[bnum] = mFM_BLOCK_TYPE_BEACH;
        } else if (blocks[bnum] == mFM_BLOCK_TYPE_RIVER_SOUTH) {
            blocks[bnum] = mFM_BLOCK_TYPE_BEACH_RIVER;
        }
    }

    blocks[mRF_D2ToD1(0, 6)] = mFM_BLOCK_TYPE_BORDER_CLIFF_OCEAN_LEFT;
    blocks[mRF_D2ToD1(6, 6)] = mFM_BLOCK_TYPE_BORDER_CLIFF_OCEAN_RIGHT;
}

static void mRF_InitCombTable(mFM_combination_c* combi_table) {
    static mFM_combination_c init_cmb = { 1, mRF_FIELD_STEP1 };
    int i;

    for (i = 0; i < BLOCK_TOTAL_NUM; i++) {
        combi_table[0] = init_cmb;
        combi_table++;
    }
}

static int mRF_SetSeaBlockWithBridgeRiver(u8* blocks, int flags) {
    if ((flags & mRF_TO_BIT(BRIDGE_LOWER)) == 0) {
        int i;

        for (i = 0; i < ((BLOCK_Z_NUM - 2) * BLOCK_X_NUM); i++) {
            if (blocks[i] == mFM_BLOCK_TYPE_BEACH_RIVER) {
                blocks[i] = mFM_BLOCK_TYPE_BEACH_RIVER_BRIDGE;
                return mRF_TO_BIT(BRIDGE_LOWER);
            }
        }
    }

    return 0;
}

/* ??? */
static int mRF_MakePerfectBit() {
    int perfect_bit = 0;
    int i;

    for (i = 0; i < mRF_BIT_NUM; i++) {
        perfect_bit |= (1 << i);
    }

    return perfect_bit;
}

static void mRF_MakeBaseLandformStep3(u8* cliff_blocks, u8* river_blocks) {
    u8* src = l_mRF_step3_blockss[mRF_GetRandom(ARRAY_COUNT(l_mRF_step3_blockss))];
    int i;

    for (i = 0; i < BLOCK_TOTAL_NUM; i++) {
        cliff_blocks[i] = src[i];
        river_blocks[i] = src[i];
    }
}

static void mRF_MakeBaseLandform(u8* cliff_blocks, u8* river_blocks, int stepmode) {
    if (stepmode == mRF_STEPMODE_THREE) {
        mRF_MakeBaseLandformStep3(cliff_blocks, river_blocks);
    } else {
        mRF_MakeBaseLandformStep2(cliff_blocks, river_blocks);
    }
}

static void mRF_ReportRandomFieldBitResult(int bit, int perfect_bit) {
    // code likely only emitted in debug
}

static int mRF_GetRandomStepMode() {
    return mRF_GetRandom(100) < 15;
}

extern void mRF_MakeRandomField_ovl(mFM_combination_c* combi_table, mFM_combo_info_c* combi_info, int combi_count,
                                    GAME* game) {
    static u8 blocks_c[BLOCK_TOTAL_NUM];
    static u8 blocks_r[BLOCK_TOTAL_NUM];
    static u8 base_table[BLOCK_TOTAL_NUM];

    int bit = 0;
    int perfect_bit;
    int stepmode;
    stepmode = mRF_GetRandomStepMode(); // select 2 or 3 step town
    perfect_bit = mRF_MakePerfectBit(); // lol

    while (perfect_bit != (perfect_bit & bit)) {
        bit = 0;
        mRF_InitCombTable(combi_table);                                  // clear save data acres
        mRF_MakeBaseLandform(blocks_c, blocks_r, stepmode);              // make the town base (cliffs + river)
        mRF_MakeFlatPlaceInfomation(blocks_c);                           // log all "flat" blocks (no river or cliff)
        mRF_SetMarinBlock(blocks_c);                                     // make the beach base
        bit |= mRF_SetBridgeAndSlopeBlock(blocks_c, stepmode);           // set bridges and slopes
        bit |= mRF_SetNeedleworkAndWharfBlock(blocks_c);                 // set Able Sister's and dock
        bit |= mRF_SetUniqueFlatBlock(blocks_c);                         // Set Wishing Well, Police Station, & Museum
        mRF_SetUniqueRailBlock(blocks_c);                                // Set Nook's Shop & Post Office
        bit |= mRF_SetPoolBlock(blocks_c);                               // Set lake
        bit |= mRF_SetSeaBlockWithBridgeRiver(blocks_c, bit);            // Set river bridge only if necessary
        mRF_MakeBaseHeightTable(base_table, blocks_c);                   // Create acre height map
        mRF_ReportRandomFieldBitResult(bit, perfect_bit);                // Debug function stubbed
        mRF_SelectBlock(combi_table, combi_info, combi_count, blocks_c); // Choose the actual acres for each block
        mRF_CopyBlockBaseHeightData(combi_table, base_table);            // Set block height
    }
}
