#ifndef LB_REKI_H
#define LB_REKI_H

#include "types.h"
#include "lb_rtc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define lbRk_YEAR_MIN GAME_YEAR_MIN
#define lbRk_YEAR_MAX GAME_YEAR_MAX
#define lbRk_YEAR_NUM ((lbRk_YEAR_MAX - lbRk_YEAR_MIN) + 1)

#define lbRk_KYUU_MONTH_START 1
#define lbRk_KYUU_MONTH_END 12
#define lbRk_KYUU_LEAP_MONTH 13

#define lbRk_HARVEST_MOON_YEAR_MIN 2002
#define lbRk_HARVEST_MOON_YEAR_MAX 2030
#define lbRk_HARVEST_MOON_YEAR_NUM ((lbRk_HARVEST_MOON_YEAR_MAX - lbRk_HARVEST_MOON_YEAR_MIN) + 1)

#define lbRk_KYUU_DAY_START 1

extern int lbRk_ToSeiyouReki(lbRTC_ymd_c* seiyo_ymd, const lbRTC_ymd_c* kyuu_ymd);
extern int lbRk_ToKyuuReki(lbRTC_ymd_c* kyuu_ymd, const lbRTC_ymd_c* seiyo_ymd);
extern int lbRk_VernalEquinoxDay(int year);
extern int lbRk_AutumnalEquinoxDay(int year);
extern void lbRk_HarvestMoonDay(lbRTC_ymd_c* harvest_moon_day, int year);

#ifdef __cplusplus
}
#endif

#endif
