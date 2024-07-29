#include "ac_set_ovl_insect.h"
#include "m_field_info.h"
#include "m_field_make.h"
#include "m_random_field_h.h"
#include "m_name_table.h"
#include "libultra/libultra.h"
#include "libc64/qrand.h"
#include "m_common_data.h"
#include "m_time.h"
#include "m_event.h"
#include "m_field_assessment.h"
#include "m_kankyo.h"

typedef struct insect_birth_sum_s {
    u8 min_birth_count;
    u8 additional_range; // min_birth_count + [0, n)
} aSOI_insect_birth_sum_c;

static aSOI_insect_birth_sum_c l_insect_birth_sum[aSOI_INSECT_TYPE_EXTENDED_NUM] = {
    { 1, 0 }, // aSOI_INSECT_TYPE_COMMON_BUTTERFLY
    { 1, 0 }, // aSOI_INSECT_TYPE_YELLOW_BUTTERFLY
    { 1, 0 }, // aSOI_INSECT_TYPE_TIGER_BUTTERFLY
    { 1, 0 }, // aSOI_INSECT_TYPE_PURPLE_BUTTERFLY
    { 1, 0 }, // aSOI_INSECT_TYPE_ROBUST_CICADA
    { 1, 0 }, // aSOI_INSECT_TYPE_WALKER_CICADA
    { 1, 0 }, // aSOI_INSECT_TYPE_EVENING_CICADA
    { 1, 0 }, // aSOI_INSECT_TYPE_BROWN_CICADA
    { 1, 0 }, // aSOI_INSECT_TYPE_BEE
    { 1, 0 }, // aSOI_INSECT_TYPE_COMMON_DRAGONFLY
    { 6, 3 }, // aSOI_INSECT_TYPE_RED_DRAGONFLY
    { 1, 0 }, // aSOI_INSECT_TYPE_DARNER_DRAGONFLY
    { 1, 0 }, // aSOI_INSECT_TYPE_BANDED_DRAGONFLY
    { 1, 0 }, // aSOI_INSECT_TYPE_LONG_LOCUST
    { 1, 0 }, // aSOI_INSECT_TYPE_MIGRATORY_LOCUST
    { 1, 0 }, // aSOI_INSECT_TYPE_CRICKET
    { 1, 0 }, // aSOI_INSECT_TYPE_GRASSHOPPER
    { 1, 0 }, // aSOI_INSECT_TYPE_BELL_CRICKET
    { 1, 0 }, // aSOI_INSECT_TYPE_PINE_CRICKET
    { 1, 0 }, // aSOI_INSECT_TYPE_DRONE_BEETLE
    { 1, 0 }, // aSOI_INSECT_TYPE_DYNASTID_BEETLE
    { 1, 0 }, // aSOI_INSECT_TYPE_FLAT_STAG_BEETLE
    { 1, 0 }, // aSOI_INSECT_TYPE_JEWEL_BEETLE
    { 1, 0 }, // aSOI_INSECT_TYPE_LONGHORN_BEETLE
    { 1, 0 }, // aSOI_INSECT_TYPE_LADYBUG
    { 1, 0 }, // aSOI_INSECT_TYPE_SPOTTED_LADYBUG
    { 1, 0 }, // aSOI_INSECT_TYPE_MANTIS
    { 6, 3 }, // aSOI_INSECT_TYPE_FIREFLY
    { 1, 0 }, // aSOI_INSECT_TYPE_COCKROACH
    { 1, 0 }, // aSOI_INSECT_TYPE_SAW_STAG_BEETLE
    { 1, 0 }, // aSOI_INSECT_TYPE_MOUNTAIN_BEETLE
    { 1, 0 }, // aSOI_INSECT_TYPE_GIANT_BEETLE
    { 1, 0 }, // aSOI_INSECT_TYPE_SNAIL
    { 1, 0 }, // aSOI_INSECT_TYPE_MOLE_CRICKET
    { 1, 0 }, // aSOI_INSECT_TYPE_POND_SKATER
    { 1, 0 }, // aSOI_INSECT_TYPE_BAGWORM
    { 1, 0 }, // aSOI_INSECT_TYPE_PILL_BUG
    { 1, 0 }, // aSOI_INSECT_TYPE_SPIDER
    { 1, 0 }, // aSOI_INSECT_TYPE_ANT
    { 1, 0 }, // aSOI_INSECT_TYPE_MOSQUITO
    { 1, 0 }, // aSOI_INSECT_TYPE_SPIRIT
};

/* sizeof(aSOI_term_info_c) == 0xC */
typedef struct term_info_s {
    /* 0x00 */ int type;      /* insect type (aSOI_INSECT_TYPE_*) */
    /* 0x04 */ u8 spawn_area; /* spawn area (aSOI_SPAWN_AREA_*) */
    /* 0x05 */ u8 weight;     /* weight value */
} aSOI_term_info_c;

/* sizeof (aSOI_term_list_c) == 8 */
typedef struct term_list_s {
    /* 0x00 */ u8 count;                    /* number of term info items */
    /* 0x04 */ aSOI_term_info_c* term_info; /* pointer to term info items */
} aSOI_term_list_c;

#define INSECT_SPAWN(insect, area, weight) { aSOI_INSECT_TYPE_##insect, aSOI_SPAWN_AREA_##area, weight }

static aSOI_term_info_c l_insect_m3_t1[5] = {
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 1),       /* 5.26% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 1),         /* 5.26% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),       /* 26.32% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 10), /* 52.63% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),           /* 10.53% */
};

static aSOI_term_info_c l_insect_m3_t2[5] = {
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 1),       /* 5.26% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 1),         /* 5.26% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),       /* 26.32% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 10), /* 52.63% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),           /* 10.53% */
};

static aSOI_term_info_c l_insect_m3_t3[9] = {
    INSECT_SPAWN(COMMON_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 5), /* 8.47% */
    INSECT_SPAWN(YELLOW_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 5), /* 8.47% */
    INSECT_SPAWN(LADYBUG, ON_FLOWER, 15),                             /* 25.42% */
    INSECT_SPAWN(SPOTTED_LADYBUG, ON_FLOWER, 15),                     /* 25.42% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 1),                            /* 1.69% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 1),                              /* 1.69% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),                            /* 8.47% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 10),                      /* 16.95% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),                                /* 3.39% */
};

static aSOI_term_info_c l_insect_m3_t4[7] = {
    INSECT_SPAWN(LADYBUG, ON_FLOWER, 5),         /* 17.24% */
    INSECT_SPAWN(SPOTTED_LADYBUG, ON_FLOWER, 5), /* 17.24% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 1),       /* 3.45% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 1),         /* 3.45% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),       /* 17.24% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 10), /* 34.48% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),           /* 6.90% */
};

static aSOI_term_info_c l_insect_m3_t5[5] = {
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 1),       /* 5.26% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 1),         /* 5.26% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),       /* 26.32% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 10), /* 52.63% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),           /* 10.53% */
};

static aSOI_term_info_c l_insect_m3_t6[5] = {
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 1),       /* 5.26% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 1),         /* 5.26% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),       /* 26.32% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 10), /* 52.63% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),           /* 10.53% */
};

static aSOI_term_info_c l_insect_m4_t1[6] = {
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),       /* 8.00% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),         /* 8.00% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),       /* 20.00% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 10), /* 40.00% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),            /* 8.00% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),   /* 16.00% */
};

static aSOI_term_info_c l_insect_m4_t2[6] = {
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),       /* 8.00% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),         /* 8.00% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),       /* 20.00% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 10), /* 40.00% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),            /* 8.00% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),   /* 16.00% */
};

static aSOI_term_info_c l_insect_m4_t3[11] = {
    INSECT_SPAWN(COMMON_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 25), /* 25.00% */
    INSECT_SPAWN(YELLOW_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 24), /* 24.00% */
    INSECT_SPAWN(TIGER_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 2),   /* 2.00% */
    INSECT_SPAWN(LADYBUG, ON_FLOWER, 12),                              /* 12.00% */
    INSECT_SPAWN(SPOTTED_LADYBUG, ON_FLOWER, 12),                      /* 12.00% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),                             /* 2.00% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),                               /* 2.00% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),                             /* 5.00% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 10),                       /* 10.00% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),                                  /* 2.00% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),                         /* 4.00% */
};

static aSOI_term_info_c l_insect_m4_t4[10] = {
    INSECT_SPAWN(COMMON_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 5), /* 9.09% */
    INSECT_SPAWN(YELLOW_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 5), /* 9.09% */
    INSECT_SPAWN(LADYBUG, ON_FLOWER, 10),                             /* 18.18% */
    INSECT_SPAWN(SPOTTED_LADYBUG, ON_FLOWER, 10),                     /* 18.18% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),                            /* 3.64% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),                              /* 3.64% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),                            /* 9.09% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 10),                      /* 18.18% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),                                 /* 3.64% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),                        /* 7.27% */
};

static aSOI_term_info_c l_insect_m4_t5[6] = {
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),       /* 8.00% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),         /* 8.00% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),       /* 20.00% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 10), /* 40.00% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),            /* 8.00% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),   /* 16.00% */
};

static aSOI_term_info_c l_insect_m4_t6[6] = {
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),       /* 8.00% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),         /* 8.00% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),       /* 20.00% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 10), /* 40.00% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),            /* 8.00% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),   /* 16.00% */
};

static aSOI_term_info_c l_insect_m5_t1[6] = {
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),      /* 11.11% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),        /* 11.11% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),      /* 27.78% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 3), /* 16.67% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),           /* 11.11% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),  /* 22.22% */
};

static aSOI_term_info_c l_insect_m5_t2[6] = {
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),      /* 11.11% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),        /* 11.11% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),      /* 27.78% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 3), /* 16.67% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),           /* 11.11% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),  /* 22.22% */
};

static aSOI_term_info_c l_insect_m5_t3[12] = {
    INSECT_SPAWN(COMMON_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 20), /* 20.00% */
    INSECT_SPAWN(YELLOW_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 20), /* 20.00% */
    INSECT_SPAWN(TIGER_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 5),   /* 5.00% */
    INSECT_SPAWN(COMMON_DRAGONFLY, FLYING, 13),                        /* 13.00% */
    INSECT_SPAWN(LADYBUG, ON_FLOWER, 12),                              /* 12.00% */
    INSECT_SPAWN(SPOTTED_LADYBUG, ON_FLOWER, 12),                      /* 12.00% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),                             /* 2.00% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),                               /* 2.00% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),                             /* 5.00% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 3),                        /* 3.00% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),                                  /* 2.00% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),                         /* 4.00% */
};

static aSOI_term_info_c l_insect_m5_t4[12] = {
    INSECT_SPAWN(COMMON_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 5), /* 7.81% */
    INSECT_SPAWN(YELLOW_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 5), /* 7.81% */
    INSECT_SPAWN(TIGER_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 1),  /* 1.56% */
    INSECT_SPAWN(COMMON_DRAGONFLY, FLYING, 15),                       /* 23.44% */
    INSECT_SPAWN(LADYBUG, ON_FLOWER, 10),                             /* 15.62% */
    INSECT_SPAWN(SPOTTED_LADYBUG, ON_FLOWER, 10),                     /* 15.62% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),                            /* 3.12% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),                              /* 3.12% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),                            /* 7.81% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 3),                       /* 4.69% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),                                 /* 3.12% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),                        /* 6.25% */
};

static aSOI_term_info_c l_insect_m5_t5[7] = {
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),      /* 10.00% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),        /* 10.00% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),      /* 25.00% */
    INSECT_SPAWN(MOSQUITO, FLYING, 2),          /* 10.00% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 3), /* 15.00% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),           /* 10.00% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),  /* 20.00% */
};

static aSOI_term_info_c l_insect_m5_t6[7] = {
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),      /* 10.53% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),        /* 10.53% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),      /* 26.32% */
    INSECT_SPAWN(MOSQUITO, FLYING, 1),          /* 5.26% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 3), /* 15.79% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),           /* 10.53% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),  /* 21.05% */
};

static aSOI_term_info_c l_insect_m6_t1[7] = {
    INSECT_SPAWN(FLAT_STAG_BEETLE, ON_TREE, 2),   /* 5.41% */
    INSECT_SPAWN(FIREFLY, FLYING_NEAR_WATER, 20), /* 54.05% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),        /* 5.41% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),          /* 5.41% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),        /* 13.51% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),             /* 5.41% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),    /* 10.81% */
};

static aSOI_term_info_c l_insect_m6_t2[6] = {
    INSECT_SPAWN(FLAT_STAG_BEETLE, ON_TREE, 2), /* 11.76% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),      /* 11.76% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),        /* 11.76% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),      /* 29.41% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),           /* 11.76% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),  /* 23.53% */
};

static aSOI_term_info_c l_insect_m6_t3[15] = {
    INSECT_SPAWN(COMMON_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 15), /* 15.00% */
    INSECT_SPAWN(YELLOW_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 15), /* 15.00% */
    INSECT_SPAWN(TIGER_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 5),   /* 5.00% */
    INSECT_SPAWN(PURPLE_BUTTERFLY, ON_TREE, 1),                        /* 1.00% */
    INSECT_SPAWN(COMMON_DRAGONFLY, FLYING, 21),                        /* 21.00% */
    INSECT_SPAWN(DARNER_DRAGONFLY, FLYING, 8),                         /* 8.00% */
    INSECT_SPAWN(LONGHORN_BEETLE, ON_TREE, 5),                         /* 5.00% */
    INSECT_SPAWN(LADYBUG, ON_FLOWER, 5),                               /* 5.00% */
    INSECT_SPAWN(SPOTTED_LADYBUG, ON_FLOWER, 5),                       /* 5.00% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),                             /* 2.00% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),                               /* 2.00% */
    INSECT_SPAWN(POND_SKATER, ON_WATER, 5),                            /* 5.00% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),                             /* 5.00% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),                                  /* 2.00% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),                         /* 4.00% */
};

static aSOI_term_info_c l_insect_m6_t4[15] = {
    INSECT_SPAWN(COMMON_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 5), /* 6.10% */
    INSECT_SPAWN(YELLOW_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 5), /* 6.10% */
    INSECT_SPAWN(TIGER_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 1),  /* 1.22% */
    INSECT_SPAWN(PURPLE_BUTTERFLY, ON_TREE, 1),                       /* 1.22% */
    INSECT_SPAWN(COMMON_DRAGONFLY, FLYING, 25),                       /* 30.49% */
    INSECT_SPAWN(DARNER_DRAGONFLY, FLYING, 10),                       /* 12.20% */
    INSECT_SPAWN(LONGHORN_BEETLE, ON_TREE, 5),                        /* 6.10% */
    INSECT_SPAWN(LADYBUG, ON_FLOWER, 5),                              /* 6.10% */
    INSECT_SPAWN(SPOTTED_LADYBUG, ON_FLOWER, 5),                      /* 6.10% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),                            /* 2.44% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),                              /* 2.44% */
    INSECT_SPAWN(POND_SKATER, ON_WATER, 5),                           /* 6.10% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),                            /* 6.10% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),                                 /* 2.44% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),                        /* 4.88% */
};

static aSOI_term_info_c l_insect_m6_t5[7] = {
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),     /* 6.67% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),       /* 6.67% */
    INSECT_SPAWN(POND_SKATER, ON_WATER, 5),    /* 16.67% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),     /* 16.67% */
    INSECT_SPAWN(MOSQUITO, FLYING, 10),        /* 33.33% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),          /* 6.67% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4), /* 13.33% */
};

static aSOI_term_info_c l_insect_m6_t6[8] = {
    INSECT_SPAWN(FLAT_STAG_BEETLE, ON_TREE, 2),   /* 2.00% */
    INSECT_SPAWN(FIREFLY, FLYING_NEAR_WATER, 76), /* 76.00% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),        /* 2.00% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),          /* 2.00% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),        /* 5.00% */
    INSECT_SPAWN(MOSQUITO, FLYING, 7),            /* 7.00% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),             /* 2.00% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),    /* 4.00% */
};

static aSOI_term_info_c l_insect_m7_t1[11] = {
    INSECT_SPAWN(DRONE_BEETLE, ON_TREE, 10),    /* 21.74% */
    INSECT_SPAWN(DYNASTID_BEETLE, ON_TREE, 5),  /* 10.87% */
    INSECT_SPAWN(FLAT_STAG_BEETLE, ON_TREE, 5), /* 10.87% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),      /* 4.35% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),        /* 4.35% */
    INSECT_SPAWN(SAW_STAG_BEETLE, ON_TREE, 5),  /* 10.87% */
    INSECT_SPAWN(MOUNTAIN_BEETLE, ON_TREE, 5),  /* 10.87% */
    INSECT_SPAWN(GIANT_BEETLE, ON_TREE, 1),     /* 2.17% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),      /* 10.87% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),           /* 4.35% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),  /* 8.70% */
};

static aSOI_term_info_c l_insect_m7_t2[12] = {
    INSECT_SPAWN(EVENING_CICADA, ON_TREE, 10),  /* 17.86% */
    INSECT_SPAWN(DRONE_BEETLE, ON_TREE, 10),    /* 17.86% */
    INSECT_SPAWN(DYNASTID_BEETLE, ON_TREE, 5),  /* 8.93% */
    INSECT_SPAWN(FLAT_STAG_BEETLE, ON_TREE, 5), /* 8.93% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),      /* 3.57% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),        /* 3.57% */
    INSECT_SPAWN(SAW_STAG_BEETLE, ON_TREE, 5),  /* 8.93% */
    INSECT_SPAWN(MOUNTAIN_BEETLE, ON_TREE, 5),  /* 8.93% */
    INSECT_SPAWN(GIANT_BEETLE, ON_TREE, 1),     /* 1.79% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),      /* 8.93% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),           /* 3.57% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),  /* 7.14% */
};

static aSOI_term_info_c l_insect_m7_t3[21] = {
    INSECT_SPAWN(COMMON_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 3), /* 3.95% */
    INSECT_SPAWN(YELLOW_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 3), /* 3.95% */
    INSECT_SPAWN(TIGER_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 3),  /* 3.95% */
    INSECT_SPAWN(PURPLE_BUTTERFLY, ON_TREE, 2),                       /* 2.63% */
    INSECT_SPAWN(ROBUST_CICADA, ON_TREE, 5),                          /* 6.58% */
    INSECT_SPAWN(WALKER_CICADA, ON_TREE, 2),                          /* 2.63% */
    INSECT_SPAWN(BROWN_CICADA, ON_TREE, 6),                           /* 7.89% */
    INSECT_SPAWN(COMMON_DRAGONFLY, FLYING, 8),                        /* 10.53% */
    INSECT_SPAWN(DARNER_DRAGONFLY, FLYING, 5),                        /* 6.58% */
    INSECT_SPAWN(BANDED_DRAGONFLY, FLYING, 5),                        /* 6.58% */
    INSECT_SPAWN(DRONE_BEETLE, ON_TREE, 4),                           /* 5.26% */
    INSECT_SPAWN(JEWEL_BEETLE, ON_TREE, 2),                           /* 2.63% */
    INSECT_SPAWN(LONGHORN_BEETLE, ON_TREE, 4),                        /* 5.26% */
    INSECT_SPAWN(LADYBUG, ON_FLOWER, 2),                              /* 2.63% */
    INSECT_SPAWN(SPOTTED_LADYBUG, ON_FLOWER, 2),                      /* 2.63% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),                            /* 2.63% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),                              /* 2.63% */
    INSECT_SPAWN(POND_SKATER, ON_WATER, 5),                           /* 6.58% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),                            /* 6.58% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),                                 /* 2.63% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),                        /* 5.26% */
};

static aSOI_term_info_c l_insect_m7_t4[19] = {
    INSECT_SPAWN(COMMON_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 3), /* 3.90% */
    INSECT_SPAWN(YELLOW_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 3), /* 3.90% */
    INSECT_SPAWN(TIGER_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 1),  /* 1.30% */
    INSECT_SPAWN(PURPLE_BUTTERFLY, ON_TREE, 1),                       /* 1.30% */
    INSECT_SPAWN(EVENING_CICADA, ON_TREE, 10),                        /* 12.99% */
    INSECT_SPAWN(BROWN_CICADA, ON_TREE, 1),                           /* 1.30% */
    INSECT_SPAWN(COMMON_DRAGONFLY, FLYING, 8),                        /* 10.39% */
    INSECT_SPAWN(DARNER_DRAGONFLY, FLYING, 5),                        /* 6.49% */
    INSECT_SPAWN(BANDED_DRAGONFLY, FLYING, 5),                        /* 6.49% */
    INSECT_SPAWN(DRONE_BEETLE, ON_TREE, 8),                           /* 10.39% */
    INSECT_SPAWN(LONGHORN_BEETLE, ON_TREE, 5),                        /* 6.49% */
    INSECT_SPAWN(LADYBUG, ON_FLOWER, 3),                              /* 3.90% */
    INSECT_SPAWN(SPOTTED_LADYBUG, ON_FLOWER, 2),                      /* 2.60% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),                            /* 2.60% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),                              /* 2.60% */
    INSECT_SPAWN(POND_SKATER, ON_WATER, 7),                           /* 9.09% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),                            /* 6.49% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),                                 /* 2.60% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),                        /* 5.19% */
};

static aSOI_term_info_c l_insect_m7_t5[9] = {
    INSECT_SPAWN(EVENING_CICADA, ON_TREE, 15), /* 22.06% */
    INSECT_SPAWN(DRONE_BEETLE, ON_TREE, 10),   /* 14.71% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),     /* 2.94% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),       /* 2.94% */
    INSECT_SPAWN(POND_SKATER, ON_WATER, 8),    /* 11.76% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),     /* 7.35% */
    INSECT_SPAWN(MOSQUITO, FLYING, 20),        /* 29.41% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),          /* 2.94% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4), /* 5.88% */
};

static aSOI_term_info_c l_insect_m7_t6[11] = {
    INSECT_SPAWN(DRONE_BEETLE, ON_TREE, 10),    /* 16.67% */
    INSECT_SPAWN(DYNASTID_BEETLE, ON_TREE, 5),  /* 8.33% */
    INSECT_SPAWN(FLAT_STAG_BEETLE, ON_TREE, 5), /* 8.33% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),      /* 3.33% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),        /* 3.33% */
    INSECT_SPAWN(SAW_STAG_BEETLE, ON_TREE, 5),  /* 8.33% */
    INSECT_SPAWN(MOUNTAIN_BEETLE, ON_TREE, 5),  /* 8.33% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),      /* 8.33% */
    INSECT_SPAWN(MOSQUITO, FLYING, 15),         /* 25.00% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),           /* 3.33% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),  /* 6.67% */
};

static aSOI_term_info_c l_insect_m8_t1[11] = {
    INSECT_SPAWN(DRONE_BEETLE, ON_TREE, 10),    /* 21.74% */
    INSECT_SPAWN(DYNASTID_BEETLE, ON_TREE, 5),  /* 10.87% */
    INSECT_SPAWN(FLAT_STAG_BEETLE, ON_TREE, 5), /* 10.87% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),      /* 4.35% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),        /* 4.35% */
    INSECT_SPAWN(SAW_STAG_BEETLE, ON_TREE, 5),  /* 10.87% */
    INSECT_SPAWN(MOUNTAIN_BEETLE, ON_TREE, 5),  /* 10.87% */
    INSECT_SPAWN(GIANT_BEETLE, ON_TREE, 1),     /* 2.17% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),      /* 10.87% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),           /* 4.35% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),  /* 8.70% */
};

static aSOI_term_info_c l_insect_m8_t2[12] = {
    INSECT_SPAWN(EVENING_CICADA, ON_TREE, 10),  /* 16.39% */
    INSECT_SPAWN(DRONE_BEETLE, ON_TREE, 15),    /* 24.59% */
    INSECT_SPAWN(DYNASTID_BEETLE, ON_TREE, 5),  /* 8.20% */
    INSECT_SPAWN(FLAT_STAG_BEETLE, ON_TREE, 5), /* 8.20% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),      /* 3.28% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),        /* 3.28% */
    INSECT_SPAWN(SAW_STAG_BEETLE, ON_TREE, 5),  /* 8.20% */
    INSECT_SPAWN(MOUNTAIN_BEETLE, ON_TREE, 5),  /* 8.20% */
    INSECT_SPAWN(GIANT_BEETLE, ON_TREE, 1),     /* 1.64% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),      /* 8.20% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),           /* 3.28% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),  /* 6.56% */
};

static aSOI_term_info_c l_insect_m8_t3[21] = {
    INSECT_SPAWN(COMMON_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 3), /* 3.85% */
    INSECT_SPAWN(YELLOW_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 2), /* 2.56% */
    INSECT_SPAWN(TIGER_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 3),  /* 3.85% */
    INSECT_SPAWN(PURPLE_BUTTERFLY, ON_TREE, 2),                       /* 2.56% */
    INSECT_SPAWN(ROBUST_CICADA, ON_TREE, 4),                          /* 5.13% */
    INSECT_SPAWN(WALKER_CICADA, ON_TREE, 3),                          /* 3.85% */
    INSECT_SPAWN(BROWN_CICADA, ON_TREE, 4),                           /* 5.13% */
    INSECT_SPAWN(BANDED_DRAGONFLY, FLYING, 5),                        /* 6.41% */
    INSECT_SPAWN(DARNER_DRAGONFLY, FLYING, 5),                        /* 6.41% */
    INSECT_SPAWN(LONG_LOCUST, ON_GROUND, 3),                          /* 3.85% */
    INSECT_SPAWN(GRASSHOPPER, IN_BUSH, 8),                            /* 10.26% */
    INSECT_SPAWN(DRONE_BEETLE, ON_TREE, 4),                           /* 5.13% */
    INSECT_SPAWN(JEWEL_BEETLE, ON_TREE, 2),                           /* 2.56% */
    INSECT_SPAWN(LONGHORN_BEETLE, ON_TREE, 3),                        /* 3.85% */
    INSECT_SPAWN(MANTIS, ON_FLOWER, 4),                               /* 5.13% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),                            /* 2.56% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),                              /* 2.56% */
    INSECT_SPAWN(POND_SKATER, ON_WATER, 8),                           /* 10.26% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),                            /* 6.41% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),                                 /* 2.56% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),                        /* 5.13% */
};

static aSOI_term_info_c l_insect_m8_t4[19] = {
    INSECT_SPAWN(COMMON_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 3), /* 3.75% */
    INSECT_SPAWN(YELLOW_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 3), /* 3.75% */
    INSECT_SPAWN(TIGER_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 1),  /* 1.25% */
    INSECT_SPAWN(PURPLE_BUTTERFLY, ON_TREE, 1),                       /* 1.25% */
    INSECT_SPAWN(EVENING_CICADA, ON_TREE, 9),                         /* 11.25% */
    INSECT_SPAWN(BROWN_CICADA, ON_TREE, 1),                           /* 1.25% */
    INSECT_SPAWN(DARNER_DRAGONFLY, FLYING, 5),                        /* 6.25% */
    INSECT_SPAWN(BANDED_DRAGONFLY, FLYING, 5),                        /* 6.25% */
    INSECT_SPAWN(LONG_LOCUST, ON_GROUND, 3),                          /* 3.75% */
    INSECT_SPAWN(GRASSHOPPER, IN_BUSH, 8),                            /* 10.00% */
    INSECT_SPAWN(DRONE_BEETLE, ON_TREE, 9),                           /* 11.25% */
    INSECT_SPAWN(LONGHORN_BEETLE, ON_TREE, 3),                        /* 3.75% */
    INSECT_SPAWN(MANTIS, ON_FLOWER, 4),                               /* 5.00% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),                            /* 2.50% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),                              /* 2.50% */
    INSECT_SPAWN(POND_SKATER, ON_WATER, 10),                          /* 12.50% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),                            /* 6.25% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),                                 /* 2.50% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),                        /* 5.00% */
};

static aSOI_term_info_c l_insect_m8_t5[10] = {
    INSECT_SPAWN(EVENING_CICADA, ON_TREE, 14), /* 19.44% */
    INSECT_SPAWN(LONG_LOCUST, ON_GROUND, 3),   /* 4.17% */
    INSECT_SPAWN(DRONE_BEETLE, ON_TREE, 10),   /* 13.89% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),     /* 2.78% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),       /* 2.78% */
    INSECT_SPAWN(POND_SKATER, ON_WATER, 10),   /* 13.89% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),     /* 6.94% */
    INSECT_SPAWN(MOSQUITO, FLYING, 20),        /* 27.78% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),          /* 2.78% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4), /* 5.56% */
};

static aSOI_term_info_c l_insect_m8_t6[11] = {
    INSECT_SPAWN(DRONE_BEETLE, ON_TREE, 10),    /* 16.67% */
    INSECT_SPAWN(DYNASTID_BEETLE, ON_TREE, 5),  /* 8.33% */
    INSECT_SPAWN(FLAT_STAG_BEETLE, ON_TREE, 5), /* 8.33% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),      /* 3.33% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),        /* 3.33% */
    INSECT_SPAWN(SAW_STAG_BEETLE, ON_TREE, 5),  /* 8.33% */
    INSECT_SPAWN(MOUNTAIN_BEETLE, ON_TREE, 5),  /* 8.33% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),      /* 8.33% */
    INSECT_SPAWN(MOSQUITO, FLYING, 15),         /* 25.00% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),           /* 3.33% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),  /* 6.67% */
};

static aSOI_term_info_c l_insect_m9_t1[8] = {
    INSECT_SPAWN(CRICKET, IN_BUSH, 43),        /* 43.00% */
    INSECT_SPAWN(BELL_CRICKET, IN_BUSH, 15),   /* 15.00% */
    INSECT_SPAWN(PINE_CRICKET, IN_BUSH, 27),   /* 27.00% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),     /* 2.00% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),       /* 2.00% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),     /* 5.00% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),          /* 2.00% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4), /* 4.00% */
};

static aSOI_term_info_c l_insect_m9_t2[8] = {
    INSECT_SPAWN(CRICKET, IN_BUSH, 20),        /* 46.51% */
    INSECT_SPAWN(BELL_CRICKET, IN_BUSH, 3),    /* 6.98% */
    INSECT_SPAWN(PINE_CRICKET, IN_BUSH, 5),    /* 11.63% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),     /* 4.65% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),       /* 4.65% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),     /* 11.63% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),          /* 4.65% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4), /* 9.30% */
};

static aSOI_term_info_c l_insect_m9_t3[15] = {
    INSECT_SPAWN(COMMON_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 3), /* 3.09% */
    INSECT_SPAWN(YELLOW_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 3), /* 3.09% */
    INSECT_SPAWN(TIGER_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 3),  /* 3.09% */
    INSECT_SPAWN(WALKER_CICADA, ON_TREE, 2),                          /* 2.06% */
    INSECT_SPAWN(RED_DRAGONFLY, FLYING, 5),                           /* 5.15% */
    INSECT_SPAWN(LONG_LOCUST, ON_GROUND, 19),                         /* 19.59% */
    INSECT_SPAWN(MIGRATORY_LOCUST, ON_GROUND, 19),                    /* 19.59% */
    INSECT_SPAWN(GRASSHOPPER, IN_BUSH, 11),                           /* 11.34% */
    INSECT_SPAWN(MANTIS, ON_FLOWER, 14),                              /* 14.43% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),                            /* 2.06% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),                              /* 2.06% */
    INSECT_SPAWN(POND_SKATER, ON_WATER, 3),                           /* 3.09% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),                            /* 5.15% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),                                 /* 2.06% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),                        /* 4.12% */
};

static aSOI_term_info_c l_insect_m9_t4[11] = {
    INSECT_SPAWN(RED_DRAGONFLY, FLYING, 50),       /* 50.00% */
    INSECT_SPAWN(LONG_LOCUST, ON_GROUND, 10),      /* 10.00% */
    INSECT_SPAWN(MIGRATORY_LOCUST, ON_GROUND, 10), /* 10.00% */
    INSECT_SPAWN(GRASSHOPPER, IN_BUSH, 5),         /* 5.00% */
    INSECT_SPAWN(MANTIS, ON_FLOWER, 5),            /* 5.00% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),         /* 2.00% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),           /* 2.00% */
    INSECT_SPAWN(POND_SKATER, ON_WATER, 5),        /* 5.00% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),         /* 5.00% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),              /* 2.00% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),     /* 4.00% */
};

static aSOI_term_info_c l_insect_m9_t5[13] = {
    INSECT_SPAWN(RED_DRAGONFLY, FLYING, 5),        /* 6.41% */
    INSECT_SPAWN(LONG_LOCUST, ON_GROUND, 10),      /* 12.82% */
    INSECT_SPAWN(MIGRATORY_LOCUST, ON_GROUND, 10), /* 12.82% */
    INSECT_SPAWN(CRICKET, IN_BUSH, 20),            /* 25.64% */
    INSECT_SPAWN(BELL_CRICKET, IN_BUSH, 3),        /* 3.85% */
    INSECT_SPAWN(PINE_CRICKET, IN_BUSH, 5),        /* 6.41% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),         /* 2.56% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),           /* 2.56% */
    INSECT_SPAWN(POND_SKATER, ON_WATER, 5),        /* 6.41% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),         /* 6.41% */
    INSECT_SPAWN(MOSQUITO, FLYING, 5),             /* 6.41% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),              /* 2.56% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4),     /* 5.13% */
};

static aSOI_term_info_c l_insect_m9_t6[9] = {
    INSECT_SPAWN(CRICKET, IN_BUSH, 40),        /* 40.00% */
    INSECT_SPAWN(BELL_CRICKET, IN_BUSH, 15),   /* 15.00% */
    INSECT_SPAWN(PINE_CRICKET, IN_BUSH, 28),   /* 28.00% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),     /* 2.00% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),       /* 2.00% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),     /* 5.00% */
    INSECT_SPAWN(MOSQUITO, FLYING, 2),         /* 2.00% */
    INSECT_SPAWN(SPIDER, ON_TREE, 2),          /* 2.00% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 4), /* 4.00% */
};

static aSOI_term_info_c l_insect_m10_t1[7] = {
    INSECT_SPAWN(CRICKET, IN_BUSH, 30),      /* 42.25% */
    INSECT_SPAWN(BELL_CRICKET, IN_BUSH, 10), /* 14.08% */
    INSECT_SPAWN(PINE_CRICKET, IN_BUSH, 20), /* 28.17% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),   /* 2.82% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),     /* 2.82% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),   /* 7.04% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),       /* 2.82% */
};

static aSOI_term_info_c l_insect_m10_t2[7] = {
    INSECT_SPAWN(CRICKET, IN_BUSH, 5),      /* 20.83% */
    INSECT_SPAWN(BELL_CRICKET, IN_BUSH, 3), /* 12.50% */
    INSECT_SPAWN(PINE_CRICKET, IN_BUSH, 5), /* 20.83% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),  /* 8.33% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),    /* 8.33% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),  /* 20.83% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),      /* 8.33% */
};

static aSOI_term_info_c l_insect_m10_t3[12] = {
    INSECT_SPAWN(COMMON_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 3), /* 3.66% */
    INSECT_SPAWN(YELLOW_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 3), /* 3.66% */
    INSECT_SPAWN(RED_DRAGONFLY, FLYING, 5),                           /* 6.10% */
    INSECT_SPAWN(LONG_LOCUST, ON_GROUND, 15),                         /* 18.29% */
    INSECT_SPAWN(MIGRATORY_LOCUST, ON_GROUND, 15),                    /* 18.29% */
    INSECT_SPAWN(LADYBUG, ON_FLOWER, 10),                             /* 12.20% */
    INSECT_SPAWN(SPOTTED_LADYBUG, ON_FLOWER, 10),                     /* 12.20% */
    INSECT_SPAWN(MANTIS, ON_FLOWER, 10),                              /* 12.20% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),                            /* 2.44% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),                              /* 2.44% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),                            /* 6.10% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),                                /* 2.44% */
};

static aSOI_term_info_c l_insect_m10_t4[10] = {
    INSECT_SPAWN(RED_DRAGONFLY, FLYING, 54),       /* 54.00% */
    INSECT_SPAWN(LONG_LOCUST, ON_GROUND, 10),      /* 10.00% */
    INSECT_SPAWN(MIGRATORY_LOCUST, ON_GROUND, 10), /* 10.00% */
    INSECT_SPAWN(LADYBUG, ON_FLOWER, 5),           /* 5.00% */
    INSECT_SPAWN(SPOTTED_LADYBUG, ON_FLOWER, 5),   /* 5.00% */
    INSECT_SPAWN(MANTIS, ON_FLOWER, 5),            /* 5.00% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),         /* 2.00% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),           /* 2.00% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),         /* 5.00% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),             /* 2.00% */
};

static aSOI_term_info_c l_insect_m10_t5[10] = {
    INSECT_SPAWN(RED_DRAGONFLY, FLYING, 5),       /* 12.82% */
    INSECT_SPAWN(LONG_LOCUST, ON_GROUND, 5),      /* 12.82% */
    INSECT_SPAWN(MIGRATORY_LOCUST, ON_GROUND, 5), /* 12.82% */
    INSECT_SPAWN(CRICKET, IN_BUSH, 5),            /* 12.82% */
    INSECT_SPAWN(BELL_CRICKET, IN_BUSH, 3),       /* 7.69% */
    INSECT_SPAWN(PINE_CRICKET, IN_BUSH, 5),       /* 12.82% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),        /* 5.13% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),          /* 5.13% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),        /* 12.82% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),            /* 5.13% */
};

static aSOI_term_info_c l_insect_m10_t6[7] = {
    INSECT_SPAWN(CRICKET, IN_BUSH, 30),      /* 42.25% */
    INSECT_SPAWN(BELL_CRICKET, IN_BUSH, 10), /* 14.08% */
    INSECT_SPAWN(PINE_CRICKET, IN_BUSH, 20), /* 28.17% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 2),   /* 2.82% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 2),     /* 2.82% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),   /* 7.04% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),       /* 2.82% */
};

static aSOI_term_info_c l_insect_m11_t1[6] = {
    INSECT_SPAWN(CRICKET, IN_BUSH, 10),         /* 45.45% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 1),      /* 4.55% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 1),        /* 4.55% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),      /* 22.73% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 3), /* 13.64% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),          /* 9.09% */
};

static aSOI_term_info_c l_insect_m11_t2[6] = {
    INSECT_SPAWN(CRICKET, IN_BUSH, 3),          /* 20.00% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 1),      /* 6.67% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 1),        /* 6.67% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),      /* 33.33% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 3), /* 20.00% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),          /* 13.33% */
};

static aSOI_term_info_c l_insect_m11_t3[7] = {
    INSECT_SPAWN(LONG_LOCUST, ON_GROUND, 5),      /* 22.73% */
    INSECT_SPAWN(MIGRATORY_LOCUST, ON_GROUND, 5), /* 22.73% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 1),        /* 4.55% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 1),          /* 4.55% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),        /* 22.73% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 3),   /* 13.64% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),            /* 9.09% */
};

static aSOI_term_info_c l_insect_m11_t4[7] = {
    INSECT_SPAWN(LONG_LOCUST, ON_GROUND, 5),      /* 22.73% */
    INSECT_SPAWN(MIGRATORY_LOCUST, ON_GROUND, 5), /* 22.73% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 1),        /* 4.55% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 1),          /* 4.55% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),        /* 22.73% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 3),   /* 13.64% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),            /* 9.09% */
};

static aSOI_term_info_c l_insect_m11_t5[6] = {
    INSECT_SPAWN(CRICKET, IN_BUSH, 3),          /* 20.00% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 1),      /* 6.67% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 1),        /* 6.67% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),      /* 33.33% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 3), /* 20.00% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),          /* 13.33% */
};

static aSOI_term_info_c l_insect_m11_t6[6] = {
    INSECT_SPAWN(CRICKET, IN_BUSH, 10),         /* 45.45% */
    INSECT_SPAWN(COCKROACH, ON_FLOWER, 1),      /* 4.55% */
    INSECT_SPAWN(COCKROACH, ON_TREE, 1),        /* 4.55% */
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),      /* 22.73% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 3), /* 13.64% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),          /* 9.09% */
};

static aSOI_term_info_c l_insect_m_other_t[3] = {
    INSECT_SPAWN(PILL_BUG, UNDER_ROCK, 5),       /* 29.41% */
    INSECT_SPAWN(MOLE_CRICKET, UNDERGROUND, 10), /* 58.82% */
    INSECT_SPAWN(BAGWORM, ON_TREE, 2),           /* 11.76% */
};

static aSOI_term_info_c l_insect_isl_t1[8] = {
    INSECT_SPAWN(DRONE_BEETLE, ON_TREE, 6),     /* 6.12% */
    INSECT_SPAWN(DYNASTID_BEETLE, ON_TREE, 3),  /* 3.06% */
    INSECT_SPAWN(FLAT_STAG_BEETLE, ON_TREE, 3), /* 3.06% */
    INSECT_SPAWN(SAW_STAG_BEETLE, ON_TREE, 2),  /* 2.04% */
    INSECT_SPAWN(MOUNTAIN_BEETLE, ON_TREE, 2),  /* 2.04% */
    INSECT_SPAWN(GIANT_BEETLE, ON_TREE, 1),     /* 1.02% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 3),  /* 3.06% */
    INSECT_SPAWN(NONE, NOTHING, 78),            /* 79.59% */
};

static aSOI_term_info_c l_insect_isl_t2[9] = {
    INSECT_SPAWN(EVENING_CICADA, ON_TREE, 30),  /* 30.00% */
    INSECT_SPAWN(DRONE_BEETLE, ON_TREE, 8),     /* 8.00% */
    INSECT_SPAWN(DYNASTID_BEETLE, ON_TREE, 3),  /* 3.00% */
    INSECT_SPAWN(FLAT_STAG_BEETLE, ON_TREE, 3), /* 3.00% */
    INSECT_SPAWN(SAW_STAG_BEETLE, ON_TREE, 2),  /* 2.00% */
    INSECT_SPAWN(MOUNTAIN_BEETLE, ON_TREE, 2),  /* 2.00% */
    INSECT_SPAWN(GIANT_BEETLE, ON_TREE, 1),     /* 1.00% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 3),  /* 3.00% */
    INSECT_SPAWN(NONE, NOTHING, 48),            /* 48.00% */
};

static aSOI_term_info_c l_insect_isl_t3[17] = {
    INSECT_SPAWN(COMMON_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 8), /* 7.41% */
    INSECT_SPAWN(YELLOW_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 8), /* 7.41% */
    INSECT_SPAWN(TIGER_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 10), /* 9.26% */
    INSECT_SPAWN(PURPLE_BUTTERFLY, ON_TREE, 6),                       /* 5.56% */
    INSECT_SPAWN(ROBUST_CICADA, ON_TREE, 10),                         /* 9.26% */
    INSECT_SPAWN(WALKER_CICADA, ON_TREE, 8),                          /* 7.41% */
    INSECT_SPAWN(BROWN_CICADA, ON_TREE, 18),                          /* 16.67% */
    INSECT_SPAWN(DARNER_DRAGONFLY, FLYING, 5),                        /* 4.63% */
    INSECT_SPAWN(BANDED_DRAGONFLY, FLYING, 5),                        /* 4.63% */
    INSECT_SPAWN(DRONE_BEETLE, ON_TREE, 3),                           /* 2.78% */
    INSECT_SPAWN(JEWEL_BEETLE, ON_TREE, 2),                           /* 1.85% */
    INSECT_SPAWN(LONGHORN_BEETLE, ON_TREE, 3),                        /* 2.78% */
    INSECT_SPAWN(LADYBUG, ON_FLOWER, 4),                              /* 3.70% */
    INSECT_SPAWN(SPOTTED_LADYBUG, ON_FLOWER, 4),                      /* 3.70% */
    INSECT_SPAWN(MANTIS, ON_FLOWER, 4),                               /* 3.70% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 3),                        /* 2.78% */
    INSECT_SPAWN(NONE, NOTHING, 7),                                   /* 6.48% */
};

static aSOI_term_info_c l_insect_isl_t4[15] = {
    INSECT_SPAWN(COMMON_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 8), /* 8.00% */
    INSECT_SPAWN(YELLOW_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 8), /* 8.00% */
    INSECT_SPAWN(TIGER_BUTTERFLY, FLYING_NEAR_FLOWERS_OR_AROUND, 10), /* 10.00% */
    INSECT_SPAWN(PURPLE_BUTTERFLY, ON_TREE, 5),                       /* 5.00% */
    INSECT_SPAWN(EVENING_CICADA, ON_TREE, 27),                        /* 27.00% */
    INSECT_SPAWN(BROWN_CICADA, ON_TREE, 3),                           /* 3.00% */
    INSECT_SPAWN(DARNER_DRAGONFLY, FLYING, 5),                        /* 5.00% */
    INSECT_SPAWN(BANDED_DRAGONFLY, FLYING, 5),                        /* 5.00% */
    INSECT_SPAWN(DRONE_BEETLE, ON_TREE, 6),                           /* 6.00% */
    INSECT_SPAWN(LONGHORN_BEETLE, ON_TREE, 3),                        /* 3.00% */
    INSECT_SPAWN(LADYBUG, ON_FLOWER, 4),                              /* 4.00% */
    INSECT_SPAWN(SPOTTED_LADYBUG, ON_FLOWER, 4),                      /* 4.00% */
    INSECT_SPAWN(MANTIS, ON_FLOWER, 4),                               /* 4.00% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 3),                        /* 3.00% */
    INSECT_SPAWN(NONE, NOTHING, 5),                                   /* 5.00% */
};

static aSOI_term_info_c l_insect_isl_t5[5] = {
    INSECT_SPAWN(EVENING_CICADA, ON_TREE, 42), /* 42.00% */
    INSECT_SPAWN(DRONE_BEETLE, ON_TREE, 8),    /* 8.00% */
    INSECT_SPAWN(MOSQUITO, FLYING, 10),        /* 10.00% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 3), /* 3.00% */
    INSECT_SPAWN(NONE, NOTHING, 37),           /* 37.00% */
};

static aSOI_term_info_c l_insect_isl_t6[8] = {
    INSECT_SPAWN(DRONE_BEETLE, ON_TREE, 8),     /* 8.00% */
    INSECT_SPAWN(DYNASTID_BEETLE, ON_TREE, 3),  /* 3.00% */
    INSECT_SPAWN(FLAT_STAG_BEETLE, ON_TREE, 3), /* 3.00% */
    INSECT_SPAWN(SAW_STAG_BEETLE, ON_TREE, 2),  /* 2.00% */
    INSECT_SPAWN(MOUNTAIN_BEETLE, ON_TREE, 2),  /* 2.00% */
    INSECT_SPAWN(MOSQUITO, FLYING, 7),          /* 7.00% */
    INSECT_SPAWN(SNAIL, RAINING_ON_FLOWER, 3),  /* 3.00% */
    INSECT_SPAWN(NONE, NOTHING, 72),            /* 72.00% */
};

static aSOI_term_info_c l_hitodama_time_table = INSECT_SPAWN(SPIRIT, FLYING, 100);

static aSOI_term_list_c l_insect_month[lbRTC_MONTHS_MAX][aSOI_TERM_NUM] = {
    // January
    {
        { 3, l_insect_m_other_t }, // t1
        { 3, l_insect_m_other_t }, // t2
        { 3, l_insect_m_other_t }, // t3
        { 3, l_insect_m_other_t }, // t4
        { 3, l_insect_m_other_t }, // t5
        { 3, l_insect_m_other_t }, // t6
    },

    // February
    {
        { 3, l_insect_m_other_t }, // t1
        { 3, l_insect_m_other_t }, // t2
        { 3, l_insect_m_other_t }, // t3
        { 3, l_insect_m_other_t }, // t4
        { 3, l_insect_m_other_t }, // t5
        { 3, l_insect_m_other_t }, // t6
    },

    // March
    {
        { 5, l_insect_m3_t1 },
        { 5, l_insect_m3_t2 },
        { 9, l_insect_m3_t3 },
        { 7, l_insect_m3_t4 },
        { 5, l_insect_m3_t5 },
        { 5, l_insect_m3_t6 },
    },

    // April
    {
        { 6, l_insect_m4_t1 },
        { 6, l_insect_m4_t2 },
        { 11, l_insect_m4_t3 },
        { 10, l_insect_m4_t4 },
        { 6, l_insect_m4_t5 },
        { 6, l_insect_m4_t6 },
    },

    // May
    {
        { 6, l_insect_m5_t1 },
        { 6, l_insect_m5_t2 },
        { 12, l_insect_m5_t3 },
        { 12, l_insect_m5_t4 },
        { 7, l_insect_m5_t5 },
        { 7, l_insect_m5_t6 },
    },

    // June
    {
        { 7, l_insect_m6_t1 },
        { 6, l_insect_m6_t2 },
        { 15, l_insect_m6_t3 },
        { 15, l_insect_m6_t4 },
        { 7, l_insect_m6_t5 },
        { 8, l_insect_m6_t6 },
    },

    // July
    {
        { 11, l_insect_m7_t1 },
        { 12, l_insect_m7_t2 },
        { 21, l_insect_m7_t3 },
        { 19, l_insect_m7_t4 },
        { 9, l_insect_m7_t5 },
        { 11, l_insect_m7_t6 },
    },

    // August
    {
        { 11, l_insect_m8_t1 },
        { 12, l_insect_m8_t2 },
        { 21, l_insect_m8_t3 },
        { 19, l_insect_m8_t4 },
        { 10, l_insect_m8_t5 },
        { 11, l_insect_m8_t6 },
    },

    // September
    {
        { 8, l_insect_m9_t1 },
        { 8, l_insect_m9_t2 },
        { 15, l_insect_m9_t3 },
        { 11, l_insect_m9_t4 },
        { 13, l_insect_m9_t5 },
        { 9, l_insect_m9_t6 },
    },

    // October
    {
        { 7, l_insect_m10_t1 },
        { 7, l_insect_m10_t2 },
        { 12, l_insect_m10_t3 },
        { 10, l_insect_m10_t4 },
        { 10, l_insect_m10_t5 },
        { 7, l_insect_m10_t6 },
    },

    // November
    {
        { 6, l_insect_m11_t1 },
        { 6, l_insect_m11_t2 },
        { 7, l_insect_m11_t3 },
        { 7, l_insect_m11_t4 },
        { 6, l_insect_m11_t5 },
        { 6, l_insect_m11_t6 },
    },

    // December
    {
        { 3, l_insect_m_other_t }, // t1
        { 3, l_insect_m_other_t }, // t2
        { 3, l_insect_m_other_t }, // t3
        { 3, l_insect_m_other_t }, // t4
        { 3, l_insect_m_other_t }, // t5
        { 3, l_insect_m_other_t }, // t6
    },
};

static aSOI_term_list_c l_insect_island[aSOI_TERM_NUM] = {
    { 8, l_insect_isl_t1 },  { 9, l_insect_isl_t2 }, { 17, l_insect_isl_t3 },
    { 15, l_insect_isl_t4 }, { 5, l_insect_isl_t5 }, { 8, l_insect_isl_t6 },
};

typedef struct insect_set_data_s {
    u8 type;
    u8 ut_x;
    u8 ut_z;
    u8 should_spawn;
    int extra_data;
    u16 live_ut[UT_Z_NUM - 4];
    int num_live_ut;
} aSOI_set_data_c;

static void aSOI_ins_clear_set_data(aSOI_set_data_c* set_data) {
    set_data->type = 0; /* should this be aSOI_INSECT_TYPE_COMMON_BUTTERFLY? */
    set_data->ut_x = -1;
    set_data->ut_z = -1;
    set_data->should_spawn = FALSE;
    set_data->extra_data = -1;
    bzero(set_data->live_ut, (UT_Z_NUM - 4) * sizeof(u16));
    set_data->num_live_ut = 0;
}

static void aSOI_set_live_ut(aSOI_set_data_c* set_data, int ut_x, int ut_z, int set) {
    set_data->live_ut[ut_z - 2] = (u16)(set_data->live_ut[ut_z - 2] & ~(1 << ut_x)) | (set << ut_x);

    if (set == TRUE) {
        set_data->num_live_ut++;
        return;
    }

    set_data->num_live_ut--;
    if (set_data->num_live_ut >= 0) {
        return;
    }

    set_data->num_live_ut = 0;
}

static int aSOI_get_live_ut_sub(aSOI_set_data_c* set_data, int ut_x, int ut_z) {
    int res = FALSE;
    if ((set_data->live_ut[ut_z - 2] >> ut_x) & 1) {
        res = TRUE;
    }
    return res;
}

static int aSOI_get_live_ut(aSOI_set_data_c* set_data, int* ut_x_out, int* ut_z_out) {
    int z;
    int x;
    int selected;
    if (set_data->num_live_ut <= 0) {
        return FALSE;
    }

    selected = (int)(fqrand() * (f32)set_data->num_live_ut);

    for (z = 2; z < UT_Z_NUM - 2; z++) {
        for (x = 2; x < UT_X_NUM - 2; x++) {
            if (aSOI_get_live_ut_sub(set_data, x, z) == TRUE) {
                if (selected <= 0) {
                    *ut_x_out = x;
                    *ut_z_out = z;
                    aSOI_set_live_ut(set_data, x, z, FALSE); // remove unit from "set" units
                    return TRUE;
                }

                selected--;
            }
        }
    }

    return FALSE;
}

static int aSOI_hour_to_term(int hour) {
    int term = aSOI_TERM0;
    if (hour >= aSOI_TERM1_START_HR) {
        if (hour < aSOI_TERM2_START_HR) {
            term = aSOI_TERM1;
        } else if (hour < aSOI_TERM3_START_HR) {
            term = aSOI_TERM2;
        } else if (hour < aSOI_TERM4_START_HR) {
            term = aSOI_TERM3;
        } else if (hour < aSOI_TERM5_START_HR) {
            term = aSOI_TERM4;
        } else if (hour < aSOI_TERM0_START_HR) {
            term = aSOI_TERM5;
        }
    }

    return term;
}

static int aSOI_ins_chk_live_insect(SET_MANAGER* set_manager, GAME_PLAY* play) {
    int live = FALSE;
    if (Common_Get(clip.insect_clip)->chk_live_insect_proc != NULL) {
        live = (*Common_Get(clip.insect_clip)->chk_live_insect_proc)(set_manager->player_pos.next_bx,
                                                                     set_manager->player_pos.next_bz, (GAME*)play);
    }

    return live;
}

static int aSOI_ins_block_check(SET_MANAGER* set_manager, GAME_PLAY* play) {
    int res = FALSE;
    if (aSOI_ins_chk_live_insect(set_manager, play) == FALSE) {
        u32 block_kind = mFI_BkNum2BlockKind(set_manager->player_pos.next_bx, set_manager->player_pos.next_bz);

        // no insects in the sea acres between town & the island
        if ((block_kind & mRF_BLOCKKIND_OFFING) == mRF_BLOCKKIND_OFFING) {
            res = TRUE;
        }
    } else {
        res = TRUE;
    }

    return res;
}

static void aSOI_ins_renewal_time(aSOI_insect_keep_c* keep) {
    int term = aSOI_hour_to_term(Common_Get(time.rtc_time.hour));
    keep->now_month = Common_Get(time.rtc_time.month);
    keep->now_term = term;
}

static int aSOI_ins_not_cmp_time(aSOI_insect_keep_c* keep) {
    lbRTC_time_c* rtc_time = Common_GetPointer(time.rtc_time);
    int term = aSOI_hour_to_term(rtc_time->hour);
    int month = rtc_time->month;

    return (keep->now_month != month || keep->now_term != term);
}

static void aSOI_ins_renew_term_info(int term) {
    Save_Set(insect_term, term);
    Save_Set(insect_term_transition_offset, (fqrand() * (f32)(aSOI_TERM_TRANSITION_MAX_DAYS + 1)));
}

static void aSOI_ins_chk_term_info(int* term0, int* term1, f32* term0_rate) {
    static f32 rate[aSOI_TERM_TRANSITION_MAX_DAYS] = { 5.0f / 6.0f, 4.0f / 6.0f, 3.0f / 6.0f, 2.0f / 6.0f,
                                                       1.0f / 6.0f };

    int now_term;
    u32 next_term;
    int saved_term;

    saved_term = Save_Get(insect_term);
    now_term = Common_Get(time.rtc_time.month) - 1;
    next_term = now_term == (lbRTC_DECEMBER - 1) ? 0 : now_term + 1;

    if (ABS(saved_term - now_term) > 1 && saved_term != 0 && now_term != lbRTC_DECEMBER - 1) {
        aSOI_ins_renew_term_info(next_term);
        *term0 = now_term;
        *term1 = now_term;
        *term0_rate = 1.0f;
    } else {
        lbRTC_time_c next_term_time;
        lbRTC_time_c end_term_transition_time;
        lbRTC_TimeCopy(&next_term_time, Common_GetPointer(time.rtc_time));
        next_term_time.month = saved_term + 1;
        next_term_time.hour = 0;
        next_term_time.min = 0;
        next_term_time.sec = 0;

        if (saved_term != now_term && now_term == lbRTC_DECEMBER - 1) {
            next_term_time.year++;
        }

        next_term_time.day = 1;

        lbRTC_Sub_DD(&next_term_time, Save_Get(insect_term_transition_offset));
        lbRTC_TimeCopy(&end_term_transition_time, &next_term_time);
        lbRTC_Add_DD(&end_term_transition_time, aSOI_TERM_TRANSITION_MAX_DAYS);
        if (lbRTC_IsOverRTC(&end_term_transition_time) == lbRTC_OVER) {
            aSOI_ins_renew_term_info(next_term);
            *term0 = now_term;
            *term1 = now_term;
            *term0_rate = 1.0f;
        } else {
            if (lbRTC_IsOverRTC(&next_term_time) == lbRTC_OVER) {
                int day_interval = lbRTC_GetIntervalDays(&next_term_time, Common_GetPointer(time.rtc_time));
                *term0 = now_term;
                *term1 = saved_term;
                *term0_rate = rate[day_interval];
            } else {
                *term0 = now_term;
                *term1 = now_term;
                *term0_rate = 1.0f;
            }
        }
    }
}

static void aSOI_ins_add_range_info(aSOI_insect_spawn_info_f_c* spawn_info, u8* spawn_info_num) {
    static aSOI_insect_spawn_info_f_c additional_data[3] = { INSECT_SPAWN(ANT, ON_CANDY, 1.0f),
                                                             INSECT_SPAWN(ANT, ON_TRASH, 1.0f),
                                                             INSECT_SPAWN(COCKROACH, ON_TRASH, 1.0f) };

    int i;

    for (i = 0; i < 3; i++) {
        bcopy(additional_data + i, spawn_info, sizeof(aSOI_insect_spawn_info_f_c));
        spawn_info++;
    }

    (*spawn_info_num) += 3;
}

static void aSOI_ins_make_insect_normal_range_data(aSOI_insect_keep_c* keep, int month, int term, f32 rate) {
    aSOI_term_list_c* month_term_list;
    aSOI_term_info_c* month_term_info;
    aSOI_insect_spawn_info_f_c* dst_info;
    u8 info_num;
    int month_info_num;
    int i;

    info_num = keep->info_num;
    month_term_list = &l_insect_month[month][term];
    month_info_num = month_term_list->count;
    month_term_info = month_term_list->term_info;

    dst_info = keep->spawn_info + info_num;
    keep->info_num = info_num + month_info_num;

    for (i = 0; i != month_info_num; i++) {
        dst_info->type = month_term_info->type;
        dst_info->spawn_area = month_term_info->spawn_area;
        dst_info->weight = ((f32)month_term_info->weight) * rate;
        month_term_info++;
        dst_info++;
    }
}

static void aSOI_ins_make_insect_island_range_data(aSOI_insect_keep_c* keep, int term) {
    u8 info_num;
    aSOI_insect_spawn_info_f_c* dst_info;
    aSOI_term_info_c* island_term_info;
    aSOI_term_list_c* island_term_list;
    int island_info_num;
    int i;

    info_num = keep->info_num;
    island_term_list = &l_insect_island[term];
    island_info_num = island_term_list->count;
    island_term_info = island_term_list->term_info;

    dst_info = keep->spawn_info + info_num;
    keep->info_num = info_num + island_info_num;

    for (i = 0; i != island_info_num; i++) {
        dst_info->type = island_term_info->type;
        dst_info->spawn_area = island_term_info->spawn_area;
        dst_info->weight = (f32)island_term_info->weight;
        island_term_info++;
        dst_info++;
    }
}

static void aSOI_ins_make_hitodama_range_data(aSOI_insect_keep_c* keep) {
    aSOI_insect_spawn_info_f_c* dst_info = keep->spawn_info + keep->info_num;
    dst_info->type = l_hitodama_time_table.type;
    dst_info->spawn_area = l_hitodama_time_table.spawn_area;
    dst_info->weight = (f32)l_hitodama_time_table.weight;
    keep->info_num = 1;
}

static int aSOI_check_hitodama_block_data(int bx, int bz, mEv_gst_hitodama_block_c* hitodama_block_data) {
    int i;
    int idx;
    int res = TRUE;

    for (i = 0, idx = 0; i < 3; i++) {
        if (bx == hitodama_block_data->block_x[idx] && bz == hitodama_block_data->block_z[idx]) {
            res = FALSE;
            break;
        }
        idx += 2;
    }

    return res;
}

static int aSOI_check_countdown_event(SET_MANAGER* set_manager, mEv_gst_hitodama_block_c* hitodama_block_data) {
    int i;
    int res = FALSE;

    // Check that the ghost event is active
    if (mEv_check_status(mEv_EVENT_NEW_YEARS_EVE_COUNTDOWN, mEv_STATUS_ACTIVE) == TRUE) {
        int n_bx = (u8)set_manager->player_pos.next_bx;
        int n_bz = (u8)set_manager->player_pos.next_bz; // idk if this is a fake match or not
        int pool_bx;
        int pool_bz;

        // Check if the player is transitioning into the lake acre
        // NOTE: this only works for the first lake in town, so hacked towns with multiple lakes
        // would still spawn spirits in subsequent lake acres.
        if (mFI_BlockKind2BkNum(&pool_bx, &pool_bz, mRF_BLOCKKIND_POOL) == TRUE && pool_bx == n_bx && pool_bz == n_bz) {
            // Check that each spirit isn't located in the lake acre
            for (i = 0; i < mEv_GHOST_HITODAMA_NUM; i++) {
                if (pool_bx == hitodama_block_data->block_x[i] && pool_bz == hitodama_block_data->block_z[i]) {
                    // Spirit is located in the lake acre, re-roll its acre
                    int t_bx;
                    int t_bz;
                    do {
                        do {
                            t_bx = 1 + ((int)(fqrand() * (f32)FG_BLOCK_X_NUM));
                            t_bz = 1 + ((int)(fqrand() * (f32)FG_BLOCK_Z_NUM));
                        } while (t_bx == pool_bx);
                    } while (t_bz == pool_bz ||
                             aSOI_check_hitodama_block_data(t_bx, t_bz, hitodama_block_data) != TRUE);

                    hitodama_block_data->block_x[i] = t_bx;
                    hitodama_block_data->block_z[i] = t_bz;
                    res = TRUE;
                    break;
                }
            }
        }
    }

    return res;
}

static int aSOI_check_hitodama_set_block(SET_MANAGER* set_manager, mEv_gst_hitodama_block_c* hitodama_block_data) {
    int res = FALSE;

    // Check that spirits are in a valid acre, and if a spirit was removed from the acre
    // the player is entering, skip logic.
    if (aSOI_check_countdown_event(set_manager, hitodama_block_data) == FALSE) {
        u8 next_bx = set_manager->player_pos.next_bx;
        u8 next_bz = set_manager->player_pos.next_bz;
        int i;

        // Check if any spirit is in the acre the player is transitioning into
        for (i = 0; i < mEv_GHOST_HITODAMA_NUM; i++) {
            if (next_bx == hitodama_block_data->block_x[i] && next_bz == hitodama_block_data->block_z[i]) {
                res = TRUE;
                break;
            }
        }
    }

    return res;
}

static int aSOI_ins_renew_check_range_table(SET_MANAGER* set_manager, aSOI_insect_keep_c* keep) {
    static int renew_table[4] = { 0, 1, 2, 3 };

    u8 spawn_type;
    int renew = 0;
    u32 block_kind = mFI_BkNum2BlockKind(set_manager->player_pos.next_bx, set_manager->player_pos.next_bz);

    if ((block_kind & mRF_BLOCKKIND_ISLAND) == mRF_BLOCKKIND_ISLAND) {
        spawn_type = aSOI_SPAWN_TYPE_ISLAND;
    } else {
        mEv_gst_common_c* ghost_common =
            (mEv_gst_common_c*)mEv_get_common_area(mEv_EVENT_GHOST, 0x37); // TODO: definitions for event type, event id
        if (ghost_common != NULL && (ghost_common->flags & mEv_GHOST_FLAG_ACTIVE) != 0 &&
            aSOI_check_hitodama_set_block(set_manager, &ghost_common->hitodama_block_data) == TRUE) {
            spawn_type = aSOI_SPAWN_TYPE_SPIRIT;
        } else {
            spawn_type = aSOI_SPAWN_TYPE_TOWN;
        }
    }

    if (keep->spawn_type != spawn_type) {
        keep->spawn_type = spawn_type;
        renew = renew_table[spawn_type];
    } else {
        if (spawn_type != aSOI_SPAWN_TYPE_SPIRIT && aSOI_ins_not_cmp_time(keep) == TRUE) {
            renew = renew_table[spawn_type];
        }
    }

    return renew;
}

static void aSOI_ins_make_range_data(SET_MANAGER* set_manager) {
    aSOI_insect_keep_c* keep = &set_manager->keep.insect_keep;
    int day_term;

    switch (aSOI_ins_renew_check_range_table(set_manager, keep)) {
        case aSOI_SPAWN_TYPE_TOWN: {
            int term0, term1;
            f32 term0_rate;
            day_term = aSOI_hour_to_term(Common_Get(time.rtc_time.hour));
            aSOI_ins_chk_term_info(&term0, &term1, &term0_rate);

            bzero(keep->spawn_info, 0x158);
            keep->info_num = 0;
            aSOI_ins_make_insect_normal_range_data(keep, term0, day_term, term0_rate);

            if (term0_rate != 1.0f) {
                aSOI_ins_make_insect_normal_range_data(keep, term1, day_term, 1.0f - term0_rate);
            }

            aSOI_ins_add_range_info(keep->spawn_info + keep->info_num, &keep->info_num);
            aSOI_ins_renewal_time(keep);
            break;
        }

        case aSOI_SPAWN_TYPE_ISLAND: {
            day_term = aSOI_hour_to_term(Common_Get(time.rtc_time.hour));

            bzero(keep->spawn_info, 0x158); // not sure where the 0x158 comes from
            keep->info_num = 0;

            aSOI_ins_make_insect_island_range_data(keep, day_term);
            aSOI_ins_add_range_info(keep->spawn_info + keep->info_num, &keep->info_num);
            aSOI_ins_renewal_time(keep);
            break;
        }

        case aSOI_SPAWN_TYPE_SPIRIT: {
            bzero(keep->spawn_info, 0x158);
            keep->info_num = 0;

            aSOI_ins_make_hitodama_range_data(keep);
            break;
        }
    }
}

static int aSOI_ins_field_fg_check_get(mActor_name_t* fg_items, u16* fg_deposit, mActor_name_t min_item_no,
                                       mActor_name_t max_item_no) {
    int spawn_found = FALSE;

    if (fg_items != NULL && fg_deposit != NULL) {
        int ut_z;
        for (ut_z = 0; ut_z < UT_Z_NUM; ut_z++) {
            int ut_x;
            for (ut_x = 0; ut_x < UT_X_NUM; ut_x++) {
                if (ut_z >= 2 && ut_z < UT_Z_NUM - 2 && ut_x >= 2 && ut_x < UT_X_NUM - 2 && min_item_no <= *fg_items &&
                    *fg_items <= max_item_no && mFI_GetLineDeposit(fg_deposit, ut_x) == FALSE) {
                    spawn_found = TRUE;
                    break;
                }

                fg_items++;
            }

            fg_deposit++;
            if (spawn_found == TRUE) {
                break;
            }
        }
    }

    return spawn_found;
}

static int aSOI_ins_field_attr_check_get_sub(mCoBG_Collision_u* collision_data, int spawn_area) {
    int valid = FALSE;
    u32 attribute = collision_data->data.unit_attribute;

    switch (spawn_area) {
        case aSOI_SPAWN_AREA_ON_GROUND: {
            // valid on: grass, most soil type, bush
            if (attribute <= mCoBG_ATTRIBUTE_SOIL1 || attribute == mCoBG_ATTRIBUTE_BUSH) {
                valid = TRUE;
            }

            break;
        }

        case aSOI_SPAWN_AREA_IN_BUSH: {
            if (attribute == mCoBG_ATTRIBUTE_BUSH) {
                valid = TRUE;
            }

            break;
        }

        case aSOI_SPAWN_AREA_FLYING_NEAR_WATER: {
            if (mCoBG_CheckWaterAttribute(attribute) == TRUE) {
                valid = TRUE;
            }

            break;
        }

        case aSOI_SPAWN_AREA_ON_WATER: {
            if (attribute >= mCoBG_ATTRIBUTE_WATER && attribute <= mCoBG_ATTRIBUTE_RIVER_NE) {
                valid = TRUE;
            }

            break;
        }
    }

    return valid;
}

static int aSOI_ins_field_attr_check_get(mCoBG_Collision_u* collision_data, int spawn_area) {
    int res = FALSE;

    if (collision_data != NULL) {
        int ut_z;

        for (ut_z = 0; ut_z < UT_Z_NUM; ut_z++) {
            int ut_x;

            for (ut_x = 0; ut_x < UT_X_NUM; ut_x++) {
                if (ut_z >= 2 && ut_z < UT_Z_NUM - 2 && ut_x >= 2 && ut_x < UT_X_NUM - 2) {
                    res = aSOI_ins_field_attr_check_get_sub(collision_data, spawn_area);
                }

                if (res == TRUE) {
                    break;
                }

                collision_data++;
            }

            if (res == TRUE) {
                break;
            }
        }
    }

    return res;
}

#define aSOI_tree_check(fg_item)                                                                          \
    ((fg_item) == TREE || (fg_item) == TREE_FTR || (fg_item) == TREE_LIGHTS || (fg_item) == TREE_BELLS || \
     (fg_item) == TREE_APPLE_FRUIT || (fg_item) == TREE_ORANGE_FRUIT || (fg_item) == TREE_PEACH_FRUIT ||  \
     (fg_item) == TREE_PEAR_FRUIT || (fg_item) == TREE_CHERRY_FRUIT || (fg_item) == TREE_1000BELLS ||     \
     (fg_item) == TREE_10000BELLS || (fg_item) == TREE_30000BELLS || (fg_item) == TREE_100BELLS ||        \
     (fg_item) == CEDAR_TREE || (fg_item) == CEDAR_TREE_BELLS || (fg_item) == CEDAR_TREE_FTR ||           \
     (fg_item) == CEDAR_TREE_LIGHTS || (fg_item) == GOLD_TREE || (fg_item) == GOLD_TREE_BELLS ||          \
     (fg_item) == GOLD_TREE_FTR || (fg_item) == GOLD_TREE_SHOVEL)

static int aSOI_ins_field_tree_check_get(mActor_name_t* fg_item) {
    int valid = FALSE;

    if (fg_item != NULL) {
        int ut_z;

        for (ut_z = 0; ut_z < UT_Z_NUM; ut_z++) {
            int ut_x;
            for (ut_x = 0; ut_x < UT_X_NUM; ut_x++) {
                if (ut_z >= 2 && ut_z < UT_Z_NUM - 2 && ut_x >= 2 && ut_x < UT_X_NUM - 2 && aSOI_tree_check(*fg_item)) {
                    valid = TRUE;
                    break;
                }
                fg_item++;
            }

            if (valid == TRUE) {
                break;
            }
        }
    }

    return valid;
}

static int aSOI_ins_field_attr_and_fg_check_get_sub(mCoBG_Collision_u* collision_data, mActor_name_t* fg_item,
                                                    int spawn_area) {
    int valid = FALSE;
    u32 attribute = collision_data->data.unit_attribute;

    switch (spawn_area) {
        case aSOI_SPAWN_AREA_UNDERGROUND: {
            if (mCoBG_CheckHole_OrgAttr(attribute) == TRUE && *fg_item == EMPTY_NO) {
                valid = TRUE;
            }
            break;
        }
    }

    return valid;
}

static int aSOI_ins_field_attr_and_fg_check_get(mCoBG_Collision_u* collision_data, mActor_name_t* fg_item,
                                                int spawn_area) {
    int res = FALSE;

    if (collision_data != NULL && fg_item != NULL) {
        int ut_z;
        for (ut_z = 0; ut_z < UT_Z_NUM; ut_z++) {
            int ut_x;
            for (ut_x = 0; ut_x < UT_X_NUM; ut_x++) {
                if (ut_z >= 2 && ut_z < UT_Z_NUM - 2 && ut_x >= 2 && ut_x < UT_X_NUM - 2) {
                    res = aSOI_ins_field_attr_and_fg_check_get_sub(collision_data, fg_item, spawn_area);
                }

                if (res == TRUE) {
                    break;
                }

                collision_data++;
                fg_item++;
            }

            if (res == TRUE) {
                break;
            }
        }
    }

    return res;
}

typedef int (*aSOI_chk_live_area_proc)(aSOI_insect_spawn_info_f_c*, int, mActor_name_t*, u16*, mActor_name_t,
                                       mActor_name_t, mCoBG_Collision_u*, aSetMgr_player_pos_c*, int);

typedef struct insect_check_s {
    mActor_name_t min_item_no;
    mActor_name_t max_item_no;
    aSOI_chk_live_area_proc chk_live_area_proc;
    int spawn_category;
} aSOI_chk_live_area_data_c;

static void aSOI_ins_change_how_to_make(aSOI_insect_spawn_info_f_c* spawn_info, int info_num, u8 new_spawn_area,
                                        u8 target_spawn_area) {
    int i;

    for (i = 0; i < info_num; i++) {
        if (target_spawn_area == spawn_info[i].spawn_area) {
            spawn_info[i].spawn_area = new_spawn_area;
        }
    }
}

static void aSOI_ins_clear_prob(aSOI_insect_spawn_info_f_c* spawn_info, int info_num, int target_spawn_area) {
    int i;

    for (i = 0; i < info_num; i++) {
        if (target_spawn_area == spawn_info->spawn_area) {
            spawn_info->weight = 0.0f;
        }
        spawn_info++;
    }
}

static void aSOI_ins_limit_insect_data(aSOI_insect_spawn_info_f_c* spawn_info, int info_num, int on_candy,
                                       int on_trash) {
    int i;

    for (i = 0; i < info_num; i++) {
        switch (spawn_info->spawn_area) {
            case aSOI_SPAWN_AREA_ON_CANDY: {
                if (on_candy == FALSE) {
                    spawn_info->weight = 0.0f;
                }
                break;
            }

            case aSOI_SPAWN_AREA_ON_TRASH: {
                if (on_trash == FALSE) {
                    spawn_info->weight = 0.0f;
                }
                break;
            }

            default: {
                spawn_info->weight = 0.0f;
                break;
            }
        }
        spawn_info++;
    }
}

static int aSOI_ins_get_idx(aSOI_insect_spawn_info_f_c* spawn_info, int info_num, int spawn_on_candy_or_trash) {
    static f32 env_rate_table[mFAs_FIELDRANK_NUM] = { 0.500f, 0.750f, 0.875f, 1.000f, 1.000f, 1.000f, 1.000f };

    f32 total_weight = 0.0f;
    f32 env_rate;
    f32 selected_weight;
    int selected = aSOI_INSECT_TYPE_INVALID;
    int i;
    int rank;
    aSOI_insect_spawn_info_f_c* spawn_info_p = spawn_info;

    for (i = 0; i < info_num; i++) {
        total_weight += spawn_info_p->weight;
        spawn_info_p++;
    }

    if (total_weight > 0.0f) {
        if (spawn_on_candy_or_trash == FALSE) {
            rank = mFAs_GetFieldRank();

            if (total_weight > 100.0f) {
                selected_weight = fqrand() * total_weight;
            } else {
                selected_weight = fqrand() * 100.0f; // chance to not spawn bug here if below 100.0f total weight
            }

            if (rank < mFAs_FIELDRANK_ZERO || rank > mFAs_FIELDRANK_SIX) {
                if (rank < mFAs_FIELDRANK_ZERO) {
                    rank = mFAs_FIELDRANK_ZERO;
                }

                if (rank > mFAs_FIELDRANK_SIX) {
                    rank = mFAs_FIELDRANK_SIX;
                }
            }

            env_rate = env_rate_table[rank];
        } else {
            env_rate = 1.0f;
            selected_weight = fqrand() * total_weight;
        }

        for (i = 0; i < info_num; i++) {
            selected_weight -= spawn_info->weight * env_rate;
            if (selected_weight < 0.0f) {
                selected = i;
                break;
            }
            spawn_info++;
        }
    }

    return selected;
}

static void aSOI_ins_set_insect_data(aSOI_set_data_c* set_data, int type, aSetMgr_player_pos_c* player_pos) {
    if (type != aSOI_INSECT_TYPE_INVALID) {
        int live_ut_x;
        int live_ut_z;

        if (aSOI_get_live_ut(set_data, &live_ut_x, &live_ut_z) == TRUE) {
            mActor_name_t item;

            set_data->type = type;
            set_data->ut_x = live_ut_x;
            set_data->ut_z = live_ut_z;
            set_data->should_spawn = TRUE;
            set_data->extra_data = 0;

            if (type == aSOI_INSECT_TYPE_COCKROACH &&
                mFI_GetBlockUtNum2FG(&item, player_pos->next_bx, player_pos->next_bz, live_ut_x, live_ut_z) == TRUE) {
                if (aSOI_tree_check(item)) {
                    set_data->extra_data = 4; // TODO: definition (*_ON_TREE)
                } else if (item == 0x2F03) {
                    set_data->extra_data = 6; // TODO: definition (*_ON_SPOILED_KABU)
                }
            }
        } else {
            set_data->should_spawn = FALSE;
        }
    }
}

static int aSOI_ins_chk_live_area_type_free(aSOI_insect_spawn_info_f_c* spawn_info, int info_num,
                                            mActor_name_t* fg_item, u16* fg_deposit, mActor_name_t min_item_no,
                                            mActor_name_t max_item_no, mCoBG_Collision_u* collision_data,
                                            aSetMgr_player_pos_c* player_pos, int spawn_area) {
    return aSOI_ins_field_fg_check_get(fg_item, fg_deposit, min_item_no, max_item_no);
}

static int aSOI_ins_chk_live_area_type_tree(aSOI_insect_spawn_info_f_c* spawn_info, int info_num,
                                            mActor_name_t* fg_item, u16* fg_deposit, mActor_name_t min_item_no,
                                            mActor_name_t max_item_no, mCoBG_Collision_u* collision_data,
                                            aSetMgr_player_pos_c* player_pos, int spawn_area) {
    return aSOI_ins_field_tree_check_get(fg_item);
}

static int aSOI_ins_chk_live_area_type_free_without_rain_and_snow(aSOI_insect_spawn_info_f_c* spawn_info, int info_num,
                                                                  mActor_name_t* fg_item, u16* fg_deposit,
                                                                  mActor_name_t min_item_no, mActor_name_t max_item_no,
                                                                  mCoBG_Collision_u* collision_data,
                                                                  aSetMgr_player_pos_c* player_pos, int spawn_area) {
    int res = FALSE;

    if (Common_Get(weather) != mEnv_WEATHER_RAIN && Common_Get(weather) != mEnv_WEATHER_SNOW) {
        res = aSOI_ins_field_fg_check_get(fg_item, fg_deposit, min_item_no, max_item_no);
    }

    return res;
}

static int aSOI_ins_chk_live_area_type_flower(aSOI_insect_spawn_info_f_c* spawn_info, int info_num,
                                              mActor_name_t* fg_item, u16* fg_deposit, mActor_name_t min_item_no,
                                              mActor_name_t max_item_no, mCoBG_Collision_u* collision_data,
                                              aSetMgr_player_pos_c* player_pos, int spawn_area) {
    int res = FALSE;

    if (Common_Get(weather) != mEnv_WEATHER_RAIN) {
        res = aSOI_ins_field_fg_check_get(fg_item, fg_deposit, min_item_no, max_item_no);
    }

    return res;
}

static int aSOI_ins_chk_live_area_type_flower_or_free(aSOI_insect_spawn_info_f_c* spawn_info, int info_num,
                                                      mActor_name_t* fg_item, u16* fg_deposit,
                                                      mActor_name_t min_item_no, mActor_name_t max_item_no,
                                                      mCoBG_Collision_u* collision_data,
                                                      aSetMgr_player_pos_c* player_pos, int spawn_area) {
    int res = FALSE;

    if (Common_Get(weather) != mEnv_WEATHER_RAIN) {
        u8 area;
        res = aSOI_ins_field_fg_check_get(fg_item, fg_deposit, min_item_no, max_item_no);
        /* prefer flowers, but if unable to spawn like that, revert to flying around */
        area = aSOI_SPAWN_AREA_ON_FLOWER;
        if (!res) {
            area = aSOI_SPAWN_AREA_FLYING;
        }
        aSOI_ins_change_how_to_make(spawn_info, info_num, area, aSOI_SPAWN_AREA_FLYING_NEAR_FLOWERS_OR_AROUND);
    }

    return res;
}

static int aSOI_ins_chk_live_area_type_flower_rain(aSOI_insect_spawn_info_f_c* spawn_info, int info_num,
                                                   mActor_name_t* fg_item, u16* fg_deposit, mActor_name_t min_item_no,
                                                   mActor_name_t max_item_no, mCoBG_Collision_u* collision_data,
                                                   aSetMgr_player_pos_c* player_pos, int spawn_area) {
    int res = FALSE;

    if (Common_Get(weather) == mEnv_WEATHER_RAIN) {
        res = aSOI_ins_field_fg_check_get(fg_item, fg_deposit, min_item_no, max_item_no);
    }

    return res;
}

static int aSOI_ins_chk_live_area_type_pond(aSOI_insect_spawn_info_f_c* spawn_info, int info_num,
                                            mActor_name_t* fg_item, u16* fg_deposit, mActor_name_t min_item_no,
                                            mActor_name_t max_item_no, mCoBG_Collision_u* collision_data,
                                            aSetMgr_player_pos_c* player_pos, int spawn_area) {
    int res = FALSE;
    u32 block_kind = mFI_BkNum2BlockKind(player_pos->next_bx, player_pos->next_bz);

    if ((block_kind & mRF_BLOCKKIND_POOL) == mRF_BLOCKKIND_POOL ||
        ((block_kind & mRF_BLOCKKIND_WATERFALL) != mRF_BLOCKKIND_WATERFALL &&
         (block_kind & mRF_BLOCKKIND_RIVER) != mRF_BLOCKKIND_RIVER &&
         (block_kind & mRF_BLOCKKIND_MARINE) != mRF_BLOCKKIND_MARINE)) {
        res = aSOI_ins_field_attr_check_get(collision_data, spawn_area);
    }

    return res;
}

static int aSOI_ins_chk_live_area_type_other(aSOI_insect_spawn_info_f_c* spawn_info, int info_num,
                                             mActor_name_t* fg_item, u16* fg_deposit, mActor_name_t min_item_no,
                                             mActor_name_t max_item_no, mCoBG_Collision_u* collision_data,
                                             aSetMgr_player_pos_c* player_pos, int spawn_area) {
    return aSOI_ins_field_attr_check_get(collision_data, spawn_area);
}

static int aSOI_ins_chk_live_area_type_dig_hole(aSOI_insect_spawn_info_f_c* spawn_info, int info_num,
                                                mActor_name_t* fg_item, u16* fg_deposit, mActor_name_t min_item_no,
                                                mActor_name_t max_item_no, mCoBG_Collision_u* collision_data,
                                                aSetMgr_player_pos_c* player_pos, int spawn_area) {
    return aSOI_ins_field_attr_and_fg_check_get(collision_data, fg_item, spawn_area);
}

static int aSOI_ins_chk_live_area_type_nothing(aSOI_insect_spawn_info_f_c* spawn_info, int info_num,
                                               mActor_name_t* fg_item, u16* fg_deposit, mActor_name_t min_item_no,
                                               mActor_name_t max_item_no, mCoBG_Collision_u* collision_data,
                                               aSetMgr_player_pos_c* player_pos, int spawn_area) {
    return TRUE;
}

#define aSOI_make_chk_live_data(min_item_no, max_item_no, func, cat) \
    { (min_item_no), (max_item_no), func, aSOI_SPAWN_CATEGORY_##cat }
static aSOI_chk_live_area_data_c aSOI_chk_live_area_data[aSOI_SPAWN_AREA_NUM] = {
    aSOI_make_chk_live_data(TREE, TREE, &aSOI_ins_chk_live_area_type_tree, TREE), /* aSOI_SPAWN_AREA_ON_TREE */
    aSOI_make_chk_live_data(FLOWER_PANSIES0, FLOWER_COSMOS2, &aSOI_ins_chk_live_area_type_flower,
                            FG_ITEM_RANGE), /* aSOI_SPAWN_AREA_ON_FLOWER */
    aSOI_make_chk_live_data(FLOWER_PANSIES0, FLOWER_COSMOS2, &aSOI_ins_chk_live_area_type_flower_rain,
                            FG_ITEM_RANGE), /* aSOI_SPAWN_AREA_RAINING_ON_FLOWER */
    aSOI_make_chk_live_data(EMPTY_NO, EMPTY_NO, &aSOI_ins_chk_live_area_type_free,
                            FG_ITEM_RANGE), /* aSOI_SPAWN_AREA_FLYING */
    aSOI_make_chk_live_data(EMPTY_NO, EMPTY_NO, &aSOI_ins_chk_live_area_type_other,
                            UT_ATTRIBUTE), /* aSOI_SPAWN_AREA_ON_GROUND */
    aSOI_make_chk_live_data(EMPTY_NO, EMPTY_NO, &aSOI_ins_chk_live_area_type_other,
                            UT_ATTRIBUTE), /* aSOI_SPAWN_AREA_IN_BUSH */
    aSOI_make_chk_live_data(EMPTY_NO, EMPTY_NO, &aSOI_ins_chk_live_area_type_other,
                            UT_ATTRIBUTE), /* aSOI_SPAWN_AREA_FLYING_NEAR_WATER */
    aSOI_make_chk_live_data(EMPTY_NO, EMPTY_NO, &aSOI_ins_chk_live_area_type_pond,
                            UT_ATTRIBUTE), /* aSOI_SPAWN_AREA_ON_WATER */
    aSOI_make_chk_live_data(ITM_FOOD_CANDY, ITM_FOOD_CANDY, &aSOI_ins_chk_live_area_type_free_without_rain_and_snow,
                            FG_ITEM_RANGE), /* aSOI_SPAWN_AREA_ON_CANDY */
    aSOI_make_chk_live_data(ITM_KABU_SPOILED, ITM_KABU_SPOILED, &aSOI_ins_chk_live_area_type_free_without_rain_and_snow,
                            FG_ITEM_RANGE), /* aSOI_SPAWN_AREA_ON_TRASH */
    aSOI_make_chk_live_data(ROCK_A, ROCK_E, &aSOI_ins_chk_live_area_type_free,
                            FG_ITEM_RANGE), /* aSOI_SPAWN_AREA_UNDER_ROCK */
    aSOI_make_chk_live_data(EMPTY_NO, EMPTY_NO, &aSOI_ins_chk_live_area_type_dig_hole,
                            UT_ATTRIBUTE_AND_FG_ITEM_RANGE), /* aSOI_SPAWN_AREA_UNDERGROUND */
    aSOI_make_chk_live_data(FLOWER_PANSIES0, FLOWER_COSMOS2, &aSOI_ins_chk_live_area_type_flower_or_free,
                            FG_ITEM_RANGE), /* aSOI_SPAWN_AREA_FLYING_NEAR_FLOWERS_OR_AROUND */
    aSOI_make_chk_live_data(EMPTY_NO, EMPTY_NO, &aSOI_ins_chk_live_area_type_nothing,
                            FG_ITEM_RANGE) /* aSOI_SPAWN_AREA_NOTHING */
};

static void aSOI_make_live_ut(aSOI_set_data_c* set_data, int spawn_area, mActor_name_t* fg_item,
                              mCoBG_Collision_u* collision_data) {
    // Start pointers at the second row of units
    aSOI_chk_live_area_data_c* chk_live_area_data = aSOI_chk_live_area_data + spawn_area;

    mActor_name_t min_item = chk_live_area_data->min_item_no;
    mActor_name_t max_item = chk_live_area_data->max_item_no;

    int ut_z;
    int ut_x;
    mActor_name_t* fg_item_p = fg_item + 2 * UT_X_NUM;
    mCoBG_Collision_u* collision_data_p = collision_data + 2 * UT_X_NUM;

    for (ut_z = 2; ut_z < UT_Z_NUM - 2; ut_z++) {
        for (ut_x = 0; ut_x < UT_X_NUM; ut_x++) {
            if (ut_z >= 2 && ut_z < UT_Z_NUM - 2 && ut_x >= 2 && ut_x < UT_X_NUM - 2) {
                switch (chk_live_area_data->spawn_category) {
                    case aSOI_SPAWN_CATEGORY_FG_ITEM_RANGE: {
                        if (min_item <= *fg_item_p && *fg_item_p <= max_item) {
                            aSOI_set_live_ut(set_data, ut_x, ut_z, TRUE);
                        }
                        break;
                    }

                    case aSOI_SPAWN_CATEGORY_UT_ATTRIBUTE: {
                        if (aSOI_ins_field_attr_check_get_sub(collision_data_p, spawn_area) == TRUE) {
                            aSOI_set_live_ut(set_data, ut_x, ut_z, TRUE);
                        }
                        break;
                    }

                    case aSOI_SPAWN_CATEGORY_TREE: {
                        if (aSOI_tree_check(*fg_item_p)) {
                            aSOI_set_live_ut(set_data, ut_x, ut_z, TRUE);
                        }
                        break;
                    }

                    case aSOI_SPAWN_CATEGORY_UT_ATTRIBUTE_AND_FG_ITEM_RANGE: {
                        if (aSOI_ins_field_attr_and_fg_check_get_sub(collision_data_p, fg_item_p, spawn_area) == TRUE) {
                            aSOI_set_live_ut(set_data, ut_x, ut_z, TRUE);
                        }
                        break;
                    }
                }
            }

            fg_item_p++;
            collision_data_p++;
        }
    }
}

static void aSOI_ins_decide_insect(aSOI_set_data_c* set_data, aSetMgr_player_pos_c* player_pos,
                                   aSOI_insect_spawn_info_f_c* spawn_info, int info_num) {
    mActor_name_t* fg_item;
    u16* deposit;
    mCoBG_Collision_u* collision_data;
    int spawn_idx;
    int i;
    int candy_spawn;
    int trash_spawn;

    candy_spawn = FALSE;
    trash_spawn = FALSE;
    fg_item = mFI_BkNumtoUtFGTop(player_pos->next_bx, player_pos->next_bz);
    deposit = mFI_GetDepositP(player_pos->next_bx, player_pos->next_bz);
    collision_data = mFI_GetBkNum2ColTop(player_pos->next_bx, player_pos->next_bz);

    for (i = aSOI_SPAWN_AREA_ON_TREE; i < aSOI_SPAWN_AREA_NUM; i++) {
        // Call chk_live_area proc to see if an insect with this spawn area can spawn in the acre
        int can_spawn = (*aSOI_chk_live_area_data[i].chk_live_area_proc)(
            spawn_info, info_num, fg_item, deposit, aSOI_chk_live_area_data[i].min_item_no,
            aSOI_chk_live_area_data[i].max_item_no, collision_data, player_pos, i);

        if (can_spawn == TRUE) {
            switch (i) {
                case aSOI_SPAWN_AREA_ON_CANDY: {
                    candy_spawn = TRUE;
                    break;
                }

                case aSOI_SPAWN_AREA_ON_TRASH: {
                    trash_spawn = TRUE;
                    break;
                }
            }
        }
        // If no spawns are available, remove any insect weight for this spawn type from the spawn list
        if (can_spawn == FALSE) {
            aSOI_ins_clear_prob(spawn_info, info_num, i);
        }
    }

    // If a candy spawn or trash spawn (spoiled turnip) is available, remove all other weights
    if (candy_spawn == TRUE || trash_spawn == TRUE) {
        aSOI_ins_limit_insect_data(spawn_info, info_num, candy_spawn, trash_spawn);
    }

    spawn_idx = aSOI_ins_get_idx(spawn_info, info_num, candy_spawn | trash_spawn);
    if (spawn_idx != aSOI_INSECT_TYPE_INVALID) {
        aSOI_insect_spawn_info_f_c* info = spawn_info + spawn_idx;
        int spawn_area = info->spawn_area;

        // Check that the spawn area is an actual valid spawn area
        if (spawn_area < aSOI_SPAWN_AREA_FLYING_NEAR_FLOWERS_OR_AROUND) {
            aSOI_make_live_ut(set_data, spawn_area, fg_item, collision_data);
            aSOI_ins_set_insect_data(set_data, info->type, player_pos);
        }
    }
}

static int aSOI_ins_make_sub(aSOI_set_data_c* set_data, aSetMgr_player_pos_c* player_pos, GAME* game) {
    aINS_Init_c insect_init;
    xyz_t wpos;
    int res = FALSE;

    if (set_data->should_spawn == TRUE) {
        mFI_BkandUtNum2Wpos(&wpos, player_pos->next_bx, player_pos->next_bz, set_data->ut_x, set_data->ut_z);
        insect_init.insect_type = set_data->type;
        wpos.x += mFI_UT_WORLDSIZE_X_F / 2.0f;
        wpos.z += mFI_UT_WORLDSIZE_Z_F / 2.0f;

        insect_init.position = wpos;
        insect_init.extra_data = set_data->extra_data;
        insect_init.game = game;

        if (insect_init.insect_type == aSOI_INSECT_TYPE_ANT) {
            (*Common_Get(clip).insect_clip->make_ant_proc)(&insect_init, player_pos->next_bx, player_pos->next_bz);
            res = TRUE;
        } else {
            if ((*Common_Get(clip).insect_clip->make_insect_proc)(&insect_init, FALSE)) {
                res = TRUE;
            }
        }
        set_data->should_spawn = FALSE;
    }

    return res;
}

static int aSOI_ins_make(aSOI_set_data_c* set_data, aSetMgr_player_pos_c* player_pos, GAME* game) {
    int res = FALSE;

    if (set_data->should_spawn == TRUE && Common_Get(clip).insect_clip->make_insect_proc != NULL &&
        Common_Get(clip).insect_clip->make_ant_proc != NULL) {
        aSOI_insect_birth_sum_c* birth_sum = l_insect_birth_sum + set_data->type;
        int birth_num = birth_sum->min_birth_count + (int)(fqrand() * (f32)birth_sum->additional_range);

        while (birth_num != 0) {
            res = aSOI_ins_make_sub(set_data, player_pos, game);
            if (res == FALSE) {
                break;
            }

            birth_num--;
            if (birth_num > 0) {
                aSOI_ins_set_insect_data(set_data, set_data->type, player_pos);
            }
        }
    }

    return res;
}

extern int aSOI_insect_set(SET_MANAGER* set_manager, GAME_PLAY* play) {
    static aSOI_insect_spawn_info_f_c copy_range[aSetMgr_INSECT_SPAWN_INFO_COUNT];
    static aSOI_set_data_c set_data;

    aSOI_insect_keep_c* keep = &set_manager->keep.insect_keep;
    int res = FALSE;

    if (aSOI_ins_block_check(set_manager, play) == FALSE) {
        aSOI_ins_clear_set_data(&set_data);
        aSOI_ins_make_range_data(set_manager);

        if (keep->info_num > 0) {
            int i;

            // No clue why they didn't do bcopy(set_manager->keep.insect_keep.spawn_info, copy_range,
            // aSetMgr_INSECT_SPAWN_INFO_COUNT * sizeof(aSOI_insect_spawn_info_f_c));
            for (i = 0; i < aSetMgr_INSECT_SPAWN_INFO_COUNT; i++) {
                bcopy(keep->spawn_info + i, copy_range + i, sizeof(aSOI_insect_spawn_info_f_c));
            }

            aSOI_ins_decide_insect(&set_data, &set_manager->player_pos, copy_range, keep->info_num);
            res = aSOI_ins_make(&set_data, &set_manager->player_pos, (GAME*)play);
        }
    }

    return res;
}
