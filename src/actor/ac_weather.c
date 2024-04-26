#include "ac_weather.h"

#include "ac_weather_fine.h"
#include "ac_weather_rain.h"
#include "ac_weather_snow.h"
#include "ac_weather_sakura.h"
#include "ac_weather_leaf.h"

#include "m_common_data.h"
#include "m_rcp.h"
#include "m_field_info.h"
#include "m_malloc.h"
#include "m_player_lib.h"
#include "m_event.h"
#include "libultra/libultra.h"

static void Weather_Actor_ct(ACTOR* actor, GAME* game);
static void Weather_Actor_dt(ACTOR* actor, GAME* game);
static void Weather_Actor_move(ACTOR* actor, GAME* game);
static void Weather_Actor_draw(ACTOR* actor, GAME* game);

static void aWeather_SetNowProfile(WEATHER_ACTOR* weather, s16 id);
static int aWeather_CountWeatherPrivate(WEATHER_ACTOR* weather);

static aWeather_Profile_c* profile_tbl[] = {
    &iam_weather_fine, &iam_weather_rain, &iam_weather_snow, &iam_weather_sakura, &iam_weather_leaf,
};

ACTOR_PROFILE Weather_Profile = {
    mAc_PROFILE_WEATHER,
    ACTOR_PART_CONTROL,
    ACTOR_STATE_CAN_MOVE_IN_DEMO_SCENES | ACTOR_STATE_NO_MOVE_WHILE_CULLED | ACTOR_STATE_NO_DRAW_WHILE_CULLED,
    EMPTY_NO,
    ACTOR_OBJ_BANK_KEEP,
    sizeof(WEATHER_ACTOR),
    Weather_Actor_ct,
    Weather_Actor_dt,
    Weather_Actor_move,
    Weather_Actor_draw,
    NULL,
};

static void aWeather_SysLevCall_MoveEnd(WEATHER_ACTOR* weather) {

    if (weather->basement_event == 1) {
        weather->stop_sound_effect = 0;
        weather->start_sound_effect = 0;
    } else {
        if (weather->stop_sound_effect != 0) {
            sAdo_SysLevStop(weather->stop_sound_effect);
        }
        if (weather->start_sound_effect != 0) {
            sAdo_SysLevStart(weather->start_sound_effect);
        }
        weather->stop_sound_effect = 0;
        weather->start_sound_effect = 0;
    }
}

static void aWeather_SysLevStart(u8 flag) {
    Common_Get(clip.weather_clip)->actor->start_sound_effect = flag;
}

static void aWeather_SysLevStop(u8 flag) {
    int stopFlag = flag;
    WEATHER_ACTOR* weather = Common_Get(clip.weather_clip)->actor;

    if (weather->start_sound_effect == stopFlag) {
        weather->start_sound_effect = 0;
    } else {
        weather->stop_sound_effect = stopFlag;
    }
}

static void aWeather_weatherinfo_CommonSet(s16 type, s16 intensity) {

    if (type >= mEnv_WEATHER_LEAVES) {
        type = 0;
    }

    Common_Set(weather, type);
    Common_Set(weather_intensity, intensity);
}

static void aWeather_RequestChangeWeather(ACTOR* actor, s16 status, s16 level) {
    WEATHER_ACTOR* weather = (WEATHER_ACTOR*)actor;
    if (mEnv_ReqeustChangeWeatherEnviroment(weather->current_status, status) != 0) {
        if (status != weather->current_status) {
            weather->next_status = status;
            weather->next_level = level;
            weather->request_change = TRUE;
            weather->current_aim_level = 0;
        } else {
            weather->current_aim_level = level;
        }
    }
}

extern void aWeather_RequestChangeWeatherToIsland() {
    aWeather_RequestChangeWeather(&Common_Get(clip.weather_clip)->actor->actor_class, Common_Get(island_weather),
                                  Common_Get(island_weather_intensity));
}

extern void aWeather_RequestChangeWeatherFromIsland() {
    aWeather_RequestChangeWeather(&Common_Get(clip.weather_clip)->actor->actor_class,
                                  mEnv_SAVE_GET_WEATHER_TYPE(Save_Get(weather)),
                                  mEnv_SAVE_GET_WEATHER_INTENSITY(Save_Get(weather)));
}

static int aWeather_GetWeatherPrvNum(ACTOR* actor) {
    WEATHER_ACTOR* weather = (WEATHER_ACTOR*)actor;
    aWeather_Priv* priv = weather->priv;
    int i;
    int num = 0;

    for (i = 100; i != 0; i--, priv++, num++) {
        if (priv->use == 0) {
            return num;
        }
    }

    return -1;
}

static void aWeather_AbolishPrivate(ACTOR* actor, int num) {
    WEATHER_ACTOR* weather = (WEATHER_ACTOR*)actor;
    aWeather_Priv* priv = weather->priv;

    if (priv != NULL) {
        priv = &priv[num];
        if (priv->use != 0) {
            priv->use = 0;
        }
    }
}

static aWeather_Priv* aWeather_GetWeatherPrv(u8 status, s16 timer, xyz_t* pos, xyz_t* speed, ACTOR* actor, int id) {
    WEATHER_ACTOR* weather = (WEATHER_ACTOR*)actor;
    aWeather_Priv* priv = weather->priv;

    if ((id != -1) && (id < 100)) {
        if (priv[id].use == 0) {
            priv[id].use = 1;
            priv[id].status = status;
            priv[id].timer = timer;
            priv[id].id = id;
            if (pos != NULL) {
                priv[id].pos = *pos;
            }
            if (speed != NULL) {
                priv[id].speed = *speed;
            }
            return &priv[id];
        }
        return NULL;
    }
    return NULL;
}

static int aWeather_StopSysLevSE() {
    WEATHER_ACTOR* weather = Common_Get(clip.weather_clip)->actor;

    u8 current = weather->current_sound_effect;

    if (weather->sound_flag == 0) {
        if (((u8)(current - 7) <= 2U) || (u8)(current - 18) <= 1U || current == 20) {
            aWeather_SysLevStop(current);
        }
        weather->sound_flag = 1;
        return 1;
    } else {
        return 0;
    }
}

static int aWeather_StartSysLevSE() {
    WEATHER_ACTOR* weather = Common_Get(clip.weather_clip)->actor;

    u8 current = weather->current_sound_effect;

    if (weather->sound_flag == 1) {
        if (mEnv_SAVE_GET_WEATHER_TYPE(Save_Get(weather)) == 1) {
            switch (mEnv_SAVE_GET_WEATHER_INTENSITY(Save_Get(weather))) {
                case 1:
                    aWeather_SysLevStart(7);
                    weather->current_sound_effect = 7;
                    break;

                case 2:
                    aWeather_SysLevStart(8);
                    weather->current_sound_effect = 8;
                    break;

                case 3:
                    aWeather_SysLevStart(9);
                    weather->current_sound_effect = 9;
                    break;
            }
        }
        weather->sound_flag = 2;
    }
    return 0;
}

static void aWeather_ChangeWeatherInstance(WEATHER_ACTOR* weather, s16 status, s16 level) {

    weather->next_status = status;
    weather->next_level = level;
    weather->current_status = status;
    weather->current_level = 0;
    weather->request_change = TRUE;

    if (!aWeather_CountWeatherPrivate(weather)) {
        weather->current_status = weather->next_status;
        aWeather_SetNowProfile(weather, weather->current_status);
        aWeather_weatherinfo_CommonSet(weather->current_status, weather->next_level);
        weather->current_level = level;
        weather->current_aim_level = weather->next_level;
        weather->request_change = FALSE;
    }
}

static int aWeather_IsLand_Event_Check() {

    if (((mFI_CheckBeforeScenePerpetual() != 0) || (mFI_GetClimate() == 1)) &&
        (Common_Get(island_weather) != (s16)mEnv_SAVE_GET_WEATHER_TYPE(Save_Get(weather)))) {
        return 1;
    }
    return 0;
}

static int aWeather_Basement_Event_Check(WEATHER_ACTOR* weather) {

    if ((weather->basement_event == 1) || (weather->basement_event == 2)) {
        return 1;
    }
    return 0;
}

static void aWeather_ChangeEnvSE(WEATHER_ACTOR* weather, GAME_PLAY* play, s16 status, s16 level) {
    u8 current = weather->current_sound_effect;

    if (weather->sound_flag == 1 || weather->sound_flag == 2)
        return;

    if (((u8)(current - 7) <= 2U) || (u8)(current - 18) <= 1U || current == 20) {
        aWeather_SysLevStop(current);
        weather->current_sound_effect = -1000;
    }

    if (mEv_CheckTitleDemo() <= 0) {
        if (Save_Get(scene_no) == SCENE_START_DEMO || Save_Get(scene_no) == SCENE_START_DEMO2 ||
            Save_Get(scene_no) == SCENE_START_DEMO3) {
            return;
        }
        if ((status == 1) && (weather->current_sound_effect == -1000)) {
            switch (level) {
                case 1:
                    if (mPlib_check_player_open_umbrella(play) != 0) {
                        aWeather_SysLevStart(0x12);
                        weather->current_sound_effect = 0x12;
                        return;
                    }
                    aWeather_SysLevStart(7);
                    weather->current_sound_effect = 7;
                    break;
                case 2:
                    if (mPlib_check_player_open_umbrella(play) != 0) {
                        aWeather_SysLevStart(0x13);
                        weather->current_sound_effect = 0x13;
                        return;
                    }
                    aWeather_SysLevStart(8);
                    weather->current_sound_effect = 8;
                    break;
                case 3:
                    if (mPlib_check_player_open_umbrella(play) != 0) {
                        aWeather_SysLevStart(0x14);
                        weather->current_sound_effect = 0x14;
                        return;
                    }
                    aWeather_SysLevStart(9);
                    weather->current_sound_effect = 9;
                    break;
            }
        }
    }
}

static void aWeather_EndEnvSE(ACTOR* actor) {
    WEATHER_ACTOR* weather = (WEATHER_ACTOR*)actor;
    u8 current = weather->current_sound_effect;

    if ((sAdo_GameframeEnd_Check() == 1) ||
        ((sAdo_GameframeEnd_Check() == 2) &&
         ((aWeather_IsLand_Event_Check() != 0) || (aWeather_Basement_Event_Check(weather) != 0)))) {
        if (((u8)(current - 7) <= 2) || ((u8)(current - 0x12) <= 1) || (u8)(current == 0x14)) {
            aWeather_SysLevStop(current);
            weather->current_sound_effect = -1000;
            Common_Set(current_sound_effect, -1000);
        }
    } else if (sAdo_GameframeEnd_Check() == 2) {
        Common_Set(current_sound_effect, weather->current_sound_effect);
    }
    aWeather_SysLevCall_MoveEnd(weather);
}

static void aWeather_SetNowProfile(WEATHER_ACTOR* weather, s16 id) {

    if (!mFI_GET_TYPE(mFI_GetFieldId())) {
        weather->current_profile = profile_tbl[id];
    } else {
        weather->current_profile = NULL;
    }
}

static void aWeather_SecureWeatherPrivateWork(WEATHER_ACTOR* weather) {
    int i;

    weather->priv = zelda_malloc(sizeof(aWeather_Priv) * 100);

    if (weather->priv != NULL) {
        for (i = 0; i < 100; i++) {
            bzero(&weather->priv[i], sizeof(aWeather_Priv));
        }
    }
}

static void aWeather_SetClip(WEATHER_ACTOR* weather, int flag) {
    aWeather_Clip_c* clip;
    if (flag != 0) {
        Common_Set(clip.weather_clip, NULL);
    } else {
        clip = &weather->clip;

        clip->actor = weather;
        clip->change_weather = aWeather_RequestChangeWeather;
        clip->get_priv_num = aWeather_GetWeatherPrvNum;
        clip->remove_priv = aWeather_AbolishPrivate;
        clip->get_priv = aWeather_GetWeatherPrv;
        clip->stop_sound = aWeather_StopSysLevSE;
        clip->start_sound = aWeather_StartSysLevSE;
        clip->change_weather_instance = aWeather_ChangeWeatherInstance;

        Common_Set(clip.weather_clip, clip);
    }
}

static void aWeather_RenewWindInfo(WEATHER_ACTOR* weather) {
    s_xyz pos = Common_Get(wind); // multiply by 1 means inline ?
    f32 speed = Common_Get(wind_speed) * 0.01f;
    f32 factor = 1.0f;

    weather->wind_info.x = pos.x * speed * factor;
    weather->wind_info.y = pos.y * speed * factor;
    weather->wind_info.z = pos.z * speed * factor;
}

static void aWeather_SnowInAdvance(WEATHER_ACTOR* weather, GAME_PLAY* play, int moves) {

    int i;

    for (i = 0; i < moves; i++) {
        Weather_Actor_move(&weather->actor_class, &play->game);
    }
}

static void Weather_Actor_ct(ACTOR* actor, GAME* game) {
    static s16 DemoWeatherTbl[5][2] = {
        3, 1, 1, 2, 0, 0, 0, 0, 2, 1,
    };
    WEATHER_ACTOR* weather = (WEATHER_ACTOR*)actor;
    GAME_PLAY* play = (GAME_PLAY*)game;

    int cur;
    xyz_t* pos = Camera2_getCenterPos_p();

    aWeather_SetClip(weather, 0);

    if (mEv_CheckTitleDemo() > 0) {
        cur = mEv_CheckTitleDemo() - 1;

        weather->current_status = DemoWeatherTbl[cur][0];
        weather->next_status = DemoWeatherTbl[cur][0];
        weather->current_level = DemoWeatherTbl[cur][1];
        weather->current_aim_level = DemoWeatherTbl[cur][1];
    } else if (mFI_GetClimate() == 1) {
        weather->current_status = Common_Get(island_weather);
        weather->next_status = Common_Get(island_weather);
        weather->current_level = Common_Get(island_weather_intensity);
        weather->current_aim_level = Common_Get(island_weather_intensity);
    } else {
        weather->current_status = mEnv_SAVE_GET_WEATHER_TYPE(Save_Get(weather));
        weather->next_status = weather->current_status;
        weather->current_level = mEnv_SAVE_GET_WEATHER_INTENSITY(Save_Get(weather));
        weather->current_aim_level = Common_Get(weather_intensity);
    }

    weather->ptr = NULL;
    weather->priv = NULL;
    weather->request_change = FALSE;

    weather->pos = *pos;

    weather->timer = 0;
    weather->timer2 = 0;
    weather->lightning_timer = 0;
    weather->lightning_timer2 = 30;

    weather->sound_flag = 0;
    aWeather_RenewWindInfo(weather);

    if (!mFI_GET_TYPE(mFI_GetFieldId())) {
        aWeather_SecureWeatherPrivateWork(weather);
    }

    aWeather_SetNowProfile(weather, weather->current_status);

    if ((weather->current_status == 2) || (weather->current_status == 3)) {
        weather->pos.y -= 50.0f;
        aWeather_SnowInAdvance(weather, play, 0x28);
        weather->pos.y += 50.0f;
    }

    weather->stop_sound_effect = 0;
    weather->start_sound_effect = 0;

    if ((Save_Get(scene_no) == SCENE_MY_ROOM_BASEMENT_S) || ((Save_Get(scene_no) - SCENE_MY_ROOM_BASEMENT_M) <= 1U) ||
        (Save_Get(scene_no) == SCENE_MY_ROOM_BASEMENT_LL1)) {
        weather->basement_event = 1;
    } else if (((Common_Get(last_scene_no) == SCENE_MY_ROOM_BASEMENT_S) ||
                ((Common_Get(last_scene_no) - SCENE_MY_ROOM_BASEMENT_M) <= 1U) ||
                (Common_Get(last_scene_no) == SCENE_MY_ROOM_BASEMENT_LL1)) &&
               (play->fb_wipe_type == 6)) {
        weather->basement_event = 2;
    } else {
        weather->basement_event = 0;
    }
    if ((play->fb_wipe_type == 6) && (aWeather_IsLand_Event_Check() == 0) &&
        (aWeather_Basement_Event_Check(weather) == 0)) {
        weather->current_sound_effect = Common_Get(current_sound_effect);
    } else {
        weather->current_sound_effect = -1000;
        aWeather_ChangeEnvSE(weather, play, weather->current_status, weather->current_level);
        aWeather_SysLevCall_MoveEnd(weather);
    }
}

static void Weather_Actor_dt(ACTOR* actor, GAME* game) {
    WEATHER_ACTOR* weather = (WEATHER_ACTOR*)actor;

    if (weather->priv != NULL) {
        zelda_free(weather->priv);
    }

    aWeather_SetClip(weather, 1);
}

static void aWeather_DrawWeatherPrv(ACTOR* actor, GAME* game) {
    WEATHER_ACTOR* weather = (WEATHER_ACTOR*)actor;

    aWeather_Priv* priv = weather->priv;
    aWeather_Profile_c* profile;
    int i;

    _texture_z_light_fog_prim_xlu(game->graph);

    if ((weather->current_profile != NULL) && (priv != NULL)) {
        if (weather->current_profile->set != NULL) {
            weather->current_profile->set(game);
        }
        if (weather->current_profile->draw != NULL) {
            for (i = 0; i < 100; i++, priv++) {
                if (priv->use != 0) {
                    weather->current_profile->draw(priv, game);
                }
            }
        }
    }
}

static void Weather_Actor_draw(ACTOR* actor, GAME* game) {
    aWeather_DrawWeatherPrv(actor, game);
}

static void aWeather_MakeWeatherPrv(ACTOR* actor, GAME* game) {
    WEATHER_ACTOR* weather = (WEATHER_ACTOR*)actor;

    if (weather->current_level != 0) {
        if (weather->current_profile != NULL) {
            if (weather->current_profile->make != NULL) {
                weather->current_profile->make(actor, game);
            }
        }
    }
}

static void aWeather_MoveWeatherPrv(WEATHER_ACTOR* weather, GAME* game) {
    aWeather_Priv* priv;
    int i;

    priv = weather->priv;

    if ((weather->current_profile != NULL) && (priv != NULL) && (weather->current_profile->move != NULL)) {

        for (i = 0; i < 100; i++, priv++) {
            if (priv->use != 0) {
                weather->current_profile->move(priv, game);
                if (priv->timer != -100) {
                    priv->timer--;
                    if (priv->timer <= 0) {
                        aWeather_AbolishPrivate(&weather->actor_class, i);
                    }
                }
            }
        }
    }
}

static int aWeather_CountWeatherPrivate(WEATHER_ACTOR* weather) {
    int i;
    int count;
    aWeather_Priv* priv = weather->priv;

    count = 0;

    for (i = 0; i < 100; i++, priv++) {

        if (priv->use != 0) {
            count++;
        }
    }

    return count;
}

static void aWeather_ChangeWeather(WEATHER_ACTOR* weather, GAME_PLAY* play) {

    if ((weather->request_change == TRUE) && (weather->current_level == 0)) {
        if (mFI_GET_TYPE(mFI_GetFieldId())) {
            weather->current_status = weather->next_status;
            aWeather_weatherinfo_CommonSet(weather->current_status, weather->next_level);
            weather->current_level = 1;
            weather->current_aim_level = weather->next_level;
            aWeather_ChangeEnvSE(weather, play, weather->current_status, weather->current_level);
            weather->request_change = FALSE;
        } else if (aWeather_CountWeatherPrivate(weather) == 0) {
            weather->current_status = weather->next_status;
            aWeather_SetNowProfile(weather, weather->current_status);
            aWeather_weatherinfo_CommonSet(weather->current_status, weather->next_level);
            weather->current_level = 1;
            weather->current_aim_level = weather->next_level;
            aWeather_ChangeEnvSE(weather, play, weather->current_status, weather->current_level);
            weather->request_change = FALSE;
        }
    }
}

static void aWeather_CheckWeatherTimer() {
    s_xyz dir;

    mEnv_DecideWindDirect(&dir, 0x3000, 0x3000);
}

static void aWeather_RenewWeatherLevel(WEATHER_ACTOR* weather, GAME_PLAY* play) {
    s16 level;

    if (weather->current_level != weather->current_aim_level) {
        weather->counter++;
        if (weather->counter >= 180) {
            weather->counter = 0;
            level = weather->current_level;
            if (weather->current_aim_level < level)
                weather->current_level--;
            else
                weather->current_level++;
            aWeather_ChangeEnvSE(weather, play, weather->current_status, weather->current_level);
        }
    }
}

static void aWeather_ChangeWeatherTime0(WEATHER_ACTOR* weather) {
    s16 rndWeather, rndIntensity;
    s16 evWeather, evIntensity;
    s16 save_weather;

    if ((mEv_CheckTitleDemo() <= 0)) {
        if ((Save_Get(scene_no) == SCENE_START_DEMO) || (Save_Get(scene_no) == SCENE_START_DEMO2) ||
            Save_Get(scene_no) == SCENE_START_DEMO3) {
            return;
        }
        if (!(mFI_CheckPlayerBlockInfo() & 0x400000) && (mTM_check_renew_time(0) != 0)) {
            mEnv_RandomWeather(&rndWeather, &rndIntensity);
            mEv_GetEventWeather(&evWeather, &evIntensity);
            if (evWeather != -1) {
                rndWeather = evWeather;
                rndIntensity = evIntensity;
            }
            if ((mEv_CheckRealArbeit() == 1) && (rndWeather == 1)) {
                rndWeather = 0;
                rndIntensity = 0;
            }
            mTM_off_renew_time(0);

            save_weather = mEnv_SAVE_GET_WEATHER_TYPE(Save_Get(weather));
            if (rndWeather == 0 || rndWeather == 3) {
                if (save_weather == 2 || save_weather == 1) {
                    mEnv_PreRainNowFine_Init();
                }
            }
            Save_Set(weather, rndIntensity | (rndWeather * 16));

            if (((mEv_CheckTitleDemo() != -9) || (weather->sound_flag != 1)) && (mFI_CheckInIsland() == 0)) {
                aWeather_RequestChangeWeather(&weather->actor_class, rndWeather, rndIntensity);
            }
            Common_Set(weather_time, Common_Get(time.rtc_time));
        }
    }
}

static void aWeather_MakeKaminari(WEATHER_ACTOR* weather) {
    lbRTC_time_c time = Common_Get(time.rtc_time);
    u8 month = time.month;
    s16 timer;

    if ((weather->basement_event != 1)) {
        if ((Save_Get(scene_no) == SCENE_START_DEMO) || (Save_Get(scene_no) == SCENE_START_DEMO2) ||
            Save_Get(scene_no) == SCENE_START_DEMO3) {
            return;
        }
        if ((month >= lbRTC_JUNE) && (month <= lbRTC_AUGUST) && (weather->current_status == 1) &&
            (weather->current_level == 3)) {
            timer = weather->lightning_timer % 1000;
            if (((timer == weather->lightning_timer2) || (timer == (weather->lightning_timer2 + 20))) &&
                (Common_Get(clip.effect_clip) != NULL) && ((Save_Get(scene_no) - SCENE_MY_ROOM_BASEMENT_S) > 3U) &&
                ((Save_Get(scene_no) - SCENE_MUSEUM_ROOM_PAINTING) > 1U) &&
                (Save_Get(scene_no) != SCENE_MUSEUM_ROOM_FISH)) {
                rgba_t kaminari_color = { 70, 70, 160, 255 };
                Common_Get(clip.effect_clip)->regist_effect_light(kaminari_color, 2, 35, FALSE);
            }
            if (timer == (weather->lightning_timer2 + 65)) {
                sAdo_SysTrgStart(0x424);
                weather->lightning_timer2 = (100.0f + (RANDOM_F(500.0f)));
            }
            weather->lightning_timer++;
        }
    }
}

static void Weather_Actor_move(ACTOR* actor, GAME* game) {
    GAME_PLAY* play = (GAME_PLAY*)game;
    WEATHER_ACTOR* weather = (WEATHER_ACTOR*)actor;

    xyz_t* pos;
    Camera2* camera;
    s16 angle;
    s16 umbrella;

    pos = Camera2_getCenterPos_p();
    camera = &play->camera;

    angle = search_position_angleY(&camera->lookat.center, &camera->lookat.eye);
    aWeather_MakeKaminari(weather);
    aWeather_CheckWeatherTimer(weather);
    aWeather_MakeWeatherPrv(actor, game);
    aWeather_RenewWeatherLevel(weather, play);
    aWeather_MoveWeatherPrv(weather, game);
    aWeather_ChangeWeather(weather, play);

    weather->pos = *pos;

    aWeather_ChangeWeatherTime0(weather);

    if (Common_Get(weather) == 1) {
        umbrella = mPlib_check_player_open_umbrella(play);
        if (umbrella != weather->umbrella_flag) {
            aWeather_ChangeEnvSE(weather, play, weather->current_status, weather->current_level);
        }

        weather->umbrella_flag = umbrella;
        weather->current_yAngle = angle;
    }

    if (weather->sound_flag == 2) {
        weather->sound_flag = 0;
    }

    aWeather_EndEnvSE(actor);
}

extern int aWeather_ChangingWeather() {

    if (Common_Get(clip.weather_clip) != NULL) {
        if (Common_Get(clip.weather_clip->actor) != NULL) {
            return Common_Get(clip.weather_clip->actor)->request_change == TRUE;
        }
    }

    return FALSE;
}
