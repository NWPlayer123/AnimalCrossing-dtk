#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import yaml
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "GAFE01",  # 0
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--no-asm",
    action="store_true",
    help="don't incorporate .s files from asm directory",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = args.version
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.debug = args.debug
config.generate_map = args.map
config.sjiswrap_path = args.sjiswrap
if not is_windows():
    config.wrapper = args.wrapper
if args.no_asm:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-1"
config.compilers_tag = "20231018"
config.dtk_tag = "v0.7.6"
config.sjiswrap_tag = "v1.1.1"
config.wibo_tag = "0.6.13"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym version={version_num}",
]
config.ldflags = [
    "-nodefaults",
    "-fp hardware",
    "-W off",
]

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    #define the platform
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    
    # for multibyte
    "-multibyte",
    "-char unsigned",
    
    #platform specific
    "-fp hardware",
    "-Cpp_exceptions off",
    '-pragma "cats off"',
    
    #default compiler flags
    #"-W all",
    "-O4,p",
    #"-inline auto",
    '-pragma "warn_notinlined off"',
    
    #helpful linking stuff
    "-maxerrors 1",
    "-nosyspath",
    
    #dtk-specific includes
    "-i include",
    f"-i build/{config.version}/include",
    f"-DVERSION={version_num}",
]

# Debug flags
if config.debug:
    cflags_base.extend(["-sym on", "-D_DEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    #"-gccinc",
    "-common off",
]

# Game-specific flags
cflags_static = [
    *cflags_base,
]

# REL flags
cflags_rel = [
    *cflags_base,

    # Disable padding warnings
    '-pragma "warn_padding off"',
    "-inline on",
    "-once",
    "-use_lmw_stmw off",
    "-O4,s",
    "-sdata 0",
    "-sdata2 0",

    # These are needed for the N64 SDK
    "-d _LANGUAGE_C",
    "-d F3DEX_GBI_2",

    # Project-specific stuff
    "-d MUST_MATCH",
    #"-d IS_REL",
    #"-d OPTIMIZED_SQRTF",
]

config.linker_version = "GC/1.3.2r"


# Helper function for Dolphin libraries
def DolphinLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_base,
        "host": False,
        "objects": objects,
    }


# Helper function for REL script objects
def Rel(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2r",
        "cflags": cflags_rel,
        "host": True,
        "objects": objects,
    }


Matching = True
NonMatching = False

config.warn_missing_config = False
config.warn_missing_source = False
config.libs = [
    {
        "lib": "Runtime.PPCEABI.H",
        "mw_version": config.linker_version,
        "cflags": cflags_runtime,
        "host": False,
        "objects": [
            Object(NonMatching, "Runtime.PPCEABI.H/global_destructor_chain.c"),
            Object(NonMatching, "Runtime.PPCEABI.H/__init_cpp_exceptions.cpp"),
        ],
    },
    {
        "lib": "actor",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "actor/ac_airplane.c"),
            Object(Matching, "actor/ac_animal_logo.c"),
            Object(Matching, "actor/ac_aprilfool_control.c"),
            Object(Matching, "actor/ac_ball.c"),
            Object(Matching, "actor/ac_birth_control.c"),
            Object(Matching, "actor/ac_boat.c"),
            Object(Matching, "actor/ac_boat_demo.c"),
            Object(Matching, "actor/ac_boxManager.c"),
            Object(Matching, "actor/ac_boxMove.c"),
            Object(Matching, "actor/ac_boxTrick01.c"),
            Object(Matching, "actor/ac_br_shop.c"),
            Object(Matching, "actor/ac_buggy.c"),
            Object(Matching, "actor/ac_cottage.c"),
            Object(Matching, "actor/ac_douzou.c"),
            Object(Matching, "actor/ac_dummy.c"),
            Object(Matching, "actor/ac_dump.c"),
            Object(Matching, "actor/ac_ev_castaway.c"),
            Object(Matching, "actor/ac_ev_dokutu.c"),
            Object(Matching, "actor/ac_ev_ghost.c"),
            Object(Matching, "actor/ac_ev_gypsy.c"),
            Object(Matching, "actor/ac_ev_kabuPeddler.c"),
            Object(Matching, "actor/ac_ev_majin.c"),
            Object(Matching, "actor/ac_ev_santa.c"),
            Object(Matching, "actor/ac_ev_soncho.c"),
            Object(Matching, "actor/ac_fallS.c"),
            Object(Matching, "actor/ac_fallSESW.c"),
            Object(Matching, "actor/ac_field_draw.c"),
            Object(Matching, "actor/ac_flag.c"),
            Object(Matching, "actor/ac_fuusen.c"),
            Object(Matching, "actor/ac_go_home_npc.c"),
            Object(Matching, "actor/ac_goza.c"),
            Object(Matching, "actor/ac_groundhog_control.c"),
            Object(Matching, "actor/ac_handOverItem.c"),
            Object(Matching, "actor/ac_haniwa.c"),
            Object(Matching, "actor/ac_kago.c"),
            Object(Matching, "actor/ac_kamakura.c"),
            Object(Matching, "actor/ac_koinobori.c"),
            Object(Matching, "actor/ac_lotus.c"),
            Object(Matching, "actor/ac_mbg.c"),
            Object(Matching, "actor/ac_mikuji.c"),
            Object(Matching, "actor/ac_misin.c"),
            Object(Matching, "actor/ac_mural.c"),
            Object(Matching, "actor/ac_museum.c"),
            Object(Matching, "actor/ac_museum_fossil.c"),
            Object(Matching, "actor/ac_museum_picture.c"),
            Object(Matching, "actor/ac_my_house.c"),
            Object(Matching, "actor/ac_my_indoor.c"),
            Object(Matching, "actor/ac_my_room.c"),
            Object(Matching, "actor/ac_nameplate.c"),
            Object(Matching, "actor/ac_needlework_shop.c"),
            Object(Matching, "actor/ac_npc_curator.c"),
            Object(Matching, "actor/ac_npc_engineer.c"),
            Object(Matching, "actor/ac_npc_rtc.c"),
            Object(Matching, "actor/ac_npc_sendo.c"),
            Object(Matching, "actor/ac_police_box.c"),
            Object(Matching, "actor/ac_post_office.c"),
            Object(Matching, "actor/ac_psnowman.c"),
            Object(Matching, "actor/ac_quest_manager.c"),
            Object(Matching, "actor/ac_quest_talk_fj_init.c"),
            Object(Matching, "actor/ac_quest_talk_greeting.c"),
            Object(Matching, "actor/ac_quest_talk_init.c"),
            Object(Matching, "actor/ac_radio.c"),
            Object(Matching, "actor/ac_reserve.c"),
            Object(Matching, "actor/ac_rope.c"),
            Object(Matching, "actor/ac_s_car.c"),
            Object(Matching, "actor/ac_sample.c"),
            Object(Matching, "actor/ac_set_manager.c"),
            Object(Matching, "actor/ac_set_ovl_gyoei.c"),
            Object(Matching, "actor/ac_set_ovl_insect.c"),
            Object(Matching, "actor/ac_shop.c"),
            Object(Matching, "actor/ac_shrine.c"),
            Object(Matching, "actor/ac_structure.c"),
            Object(Matching, "actor/ac_tama.c"),
            Object(Matching, "actor/ac_tent.c"),
            Object(Matching, "actor/ac_tools.c"),
            Object(Matching, "actor/ac_toudai.c"),
            Object(Matching, "actor/ac_train0.c"),
            Object(Matching, "actor/ac_train1.c"),
            Object(Matching, "actor/ac_train_door.c"),
            Object(Matching, "actor/ac_train_window.c"),
            Object(Matching, "actor/ac_tukimi.c"),
            Object(Matching, "actor/ac_weather.c"),
            Object(Matching, "actor/ac_weather_fine.c"),
            Object(Matching, "actor/ac_weather_leaf.c"),
            Object(Matching, "actor/ac_weather_rain.c"),
            Object(Matching, "actor/ac_weather_sakura.c"),
            Object(Matching, "actor/ac_weather_snow.c"),
            Object(Matching, "actor/ac_windmill.c"),
            Object(Matching, "actor/ac_yatai.c"),
        ],
    },
    {
        "lib": "actor_tool",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "actor/tool/ac_t_anrium1.c"),
            Object(Matching, "actor/tool/ac_t_bag1.c"),
            Object(Matching, "actor/tool/ac_t_bag2.c"),
            Object(Matching, "actor/tool/ac_t_biscus1.c"),
            Object(Matching, "actor/tool/ac_t_biscus2.c"),
            Object(Matching, "actor/tool/ac_t_biscus3.c"),
            Object(Matching, "actor/tool/ac_t_biscus4.c"),
            Object(Matching, "actor/tool/ac_t_cobra1.c"),
            Object(Matching, "actor/tool/ac_t_cracker.c"),
            Object(Matching, "actor/tool/ac_t_flag.c"),
            Object(Matching, "actor/tool/ac_t_hanabi.c"),
            Object(Matching, "actor/tool/ac_t_hasu1.c"),
            Object(Matching, "actor/tool/ac_t_hat1.c"),
            Object(Matching, "actor/tool/ac_t_hat2.c"),
            Object(Matching, "actor/tool/ac_t_hat3.c"),
            Object(Matching, "actor/tool/ac_t_keitai.c"),
            Object(Matching, "actor/tool/ac_t_npc_sao.c"),
            Object(Matching, "actor/tool/ac_t_pistol.c"),
            Object(Matching, "actor/tool/ac_t_rei1.c"),
            Object(Matching, "actor/tool/ac_t_rei2.c"),
            Object(Matching, "actor/tool/ac_t_tama.c"),
            Object(Matching, "actor/tool/ac_t_tumbler.c"),
            Object(Matching, "actor/tool/ac_t_umbrella.c"),
            Object(Matching, "actor/tool/ac_t_utiwa.c"),
            Object(Matching, "actor/tool/ac_t_zinnia1.c"),
            Object(Matching, "actor/tool/ac_t_zinnia2.c"),
        ],
    },
    {
        "lib": "effects",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "effect/ef_ami_mizu.c"),
            Object(Matching, "effect/ef_anahikari.c"),
            Object(Matching, "effect/ef_ase.c"),
            Object(Matching, "effect/ef_ase2.c"),
            Object(Matching, "effect/ef_ase_ch.c"),
            Object(NonMatching, "effect/ef_break_axe.c"),
            Object(Matching, "effect/ef_bubu.c"),
            Object(Matching, "effect/ef_buruburu.c"),
            Object(Matching, "effect/ef_bush_happa.c"),
            Object(Matching, "effect/ef_bush_yuki.c"),
            Object(Matching, "effect/ef_car_blight.c"),
            Object(Matching, "effect/ef_car_light.c"),
            Object(Matching, "effect/ef_clacker.c"),
            Object(Matching, "effect/ef_coin.c"),
            Object(NonMatching, "effect/ef_dash_asimoto.c"),
            Object(NonMatching, "effect/ef_dig_hole.c"),
            Object(NonMatching, "effect/ef_dig_mud.c"),
            Object(NonMatching, "effect/ef_dig_scoop.c"),
            Object(NonMatching, "effect/ef_douzou_light.c"),
            Object(NonMatching, "effect/ef_doyon.c"),
            Object(NonMatching, "effect/ef_dust.c"),
            Object(Matching, "effect/ef_effect_control.c"),
            Object(NonMatching, "effect/ef_effect_lib.c"),
            Object(Matching, "effect/ef_flash.c"),
            Object(Matching, "effect/ef_footprint.c"),
            Object(NonMatching, "effect/ef_furo_yuge.c"),
            Object(NonMatching, "effect/ef_gimonhu.c"),
            Object(NonMatching, "effect/ef_goki.c"),
            Object(NonMatching, "effect/ef_ha.c"),
            Object(NonMatching, "effect/ef_halloween.c"),
            Object(NonMatching, "effect/ef_halloween_smoke.c"),
            Object(NonMatching, "effect/ef_hanabira.c"),
            Object(NonMatching, "effect/ef_hanabi_botan1.c"),
            Object(NonMatching, "effect/ef_hanabi_botan2.c"),
            Object(NonMatching, "effect/ef_hanabi_dummy.c"),
            Object(NonMatching, "effect/ef_hanabi_hoshi.c"),
            Object(NonMatching, "effect/ef_hanabi_set.c"),
            Object(NonMatching, "effect/ef_hanabi_switch.c"),
            Object(NonMatching, "effect/ef_hanabi_yanagi.c"),
            Object(NonMatching, "effect/ef_hanatiri.c"),
            Object(NonMatching, "effect/ef_hirameki_den.c"),
            Object(NonMatching, "effect/ef_hirameki_hikari.c"),
            Object(NonMatching, "effect/ef_ikigire.c"),
            Object(NonMatching, "effect/ef_impact_star.c"),
            Object(NonMatching, "effect/ef_kagu_happa.c"),
            Object(NonMatching, "effect/ef_kamifubuki.c"),
            Object(NonMatching, "effect/ef_kangaeru.c"),
            Object(NonMatching, "effect/ef_kantanhu.c"),
            Object(NonMatching, "effect/ef_kasamizu.c"),
            Object(NonMatching, "effect/ef_kasamizutama.c"),
            Object(NonMatching, "effect/ef_kaze.c"),
            Object(NonMatching, "effect/ef_kaze_happa.c"),
            Object(NonMatching, "effect/ef_kigae.c"),
            Object(NonMatching, "effect/ef_kigae_light.c"),
            Object(NonMatching, "effect/ef_kikuzu.c"),
            Object(Matching, "effect/ef_killer.c"),
            Object(NonMatching, "effect/ef_kisha_kemuri.c"),
            Object(NonMatching, "effect/ef_konpu.c"),
            Object(NonMatching, "effect/ef_kpun.c"),
            Object(NonMatching, "effect/ef_kyousou_onpu.c"),
            Object(Matching, "effect/ef_lamp_light.c"),
            Object(NonMatching, "effect/ef_lovelove.c"),
            Object(NonMatching, "effect/ef_lovelove2.c"),
            Object(NonMatching, "effect/ef_lovelove_heart.c"),
            Object(Matching, "effect/ef_make_hem.c"),
            Object(NonMatching, "effect/ef_mizutama.c"),
            Object(NonMatching, "effect/ef_motiyuge.c"),
            Object(NonMatching, "effect/ef_muka.c"),
            Object(NonMatching, "effect/ef_naku.c"),
            Object(NonMatching, "effect/ef_namida.c"),
            Object(NonMatching, "effect/ef_neboke.c"),
            Object(NonMatching, "effect/ef_neboke_akubi.c"),
            Object(NonMatching, "effect/ef_neboke_awa.c"),
            Object(NonMatching, "effect/ef_night13_moon.c"),
            Object(NonMatching, "effect/ef_night15_moon.c"),
            Object(NonMatching, "effect/ef_ongen.c"),
            Object(NonMatching, "effect/ef_otikomi.c"),
            Object(NonMatching, "effect/ef_otosiana.c"),
            Object(NonMatching, "effect/ef_pun.c"),
            Object(NonMatching, "effect/ef_pun_sekimen.c"),
            Object(NonMatching, "effect/ef_pun_yuge.c"),
            Object(NonMatching, "effect/ef_reset_hole.c"),
            Object(Matching, "effect/ef_room_sunshine.c"),
            Object(Matching, "effect/ef_room_sunshine_minsect.c"),
            Object(Matching, "effect/ef_room_sunshine_museum.c"),
            Object(Matching, "effect/ef_room_sunshine_police.c"),
            Object(Matching, "effect/ef_room_sunshine_posthouse.c"),
            Object(NonMatching, "effect/ef_sandsplash.c"),
            Object(NonMatching, "effect/ef_shock.c"),
            Object(NonMatching, "effect/ef_shooting.c"),
            Object(NonMatching, "effect/ef_shooting_kira.c"),
            Object(NonMatching, "effect/ef_shooting_set.c"),
            Object(NonMatching, "effect/ef_siawase_hana.c"),
            Object(NonMatching, "effect/ef_siawase_hana_ch.c"),
            Object(NonMatching, "effect/ef_siawase_hikari.c"),
            Object(NonMatching, "effect/ef_sibuki.c"),
            Object(NonMatching, "effect/ef_situren.c"),
            Object(NonMatching, "effect/ef_slip.c"),
            Object(NonMatching, "effect/ef_slip_footprint.c"),
            Object(NonMatching, "effect/ef_soba_yuge.c"),
            Object(NonMatching, "effect/ef_steam.c"),
            Object(NonMatching, "effect/ef_string.c"),
            Object(NonMatching, "effect/ef_suisou_awa.c"),
            Object(NonMatching, "effect/ef_swing_axe.c"),
            Object(NonMatching, "effect/ef_swing_net.c"),
            Object(NonMatching, "effect/ef_swing_rod.c"),
            Object(NonMatching, "effect/ef_taberu.c"),
            Object(NonMatching, "effect/ef_takurami.c"),
            Object(NonMatching, "effect/ef_takurami_kira.c"),
            Object(NonMatching, "effect/ef_tamaire.c"),
            Object(NonMatching, "effect/ef_tape.c"),
            Object(Matching, "effect/ef_tent_lamp.c"),
            Object(NonMatching, "effect/ef_tumble.c"),
            Object(NonMatching, "effect/ef_tumble_bodyprint.c"),
            Object(NonMatching, "effect/ef_tumble_dust.c"),
            Object(NonMatching, "effect/ef_turi_hamon.c"),
            Object(NonMatching, "effect/ef_turi_hane0.c"),
            Object(NonMatching, "effect/ef_turi_hane1.c"),
            Object(NonMatching, "effect/ef_turi_mizu.c"),
            Object(NonMatching, "effect/ef_turi_suiteki.c"),
            Object(NonMatching, "effect/ef_turn_asimoto.c"),
            Object(NonMatching, "effect/ef_turn_footprint.c"),
            Object(NonMatching, "effect/ef_uranai.c"),
            Object(NonMatching, "effect/ef_wait_asimoto.c"),
            Object(NonMatching, "effect/ef_walk_asimoto.c"),
            Object(NonMatching, "effect/ef_warau.c"),
            Object(NonMatching, "effect/ef_yajirushi.c"),
            Object(NonMatching, "effect/ef_young_tree.c"),
            Object(Matching, "effect/ef_yukidama.c"),
            Object(Matching, "effect/ef_yukidaruma.c"),
            Object(Matching, "effect/ef_yukihane.c"),
        ],
    },
    {
        "lib": "furniture",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "f_furniture.c"),
        ],
    },
    {
        "lib": "game",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "game/m_actor.c"),
            Object(Matching, "game/m_actor_dlftbls.c"),
            Object(Matching, "game/m_actor_shadow.c"),
            Object(Matching, "game/m_all_grow.c"),
            Object(NonMatching, "game/m_all_grow_ovl.c"),
            Object(NonMatching, "game/m_bank_ovl.c"),
            Object(Matching, "game/m_banti.c"),
            Object(Matching, "game/m_bg_item.c"),
            Object(Matching, "game/m_bg_tex.c"),
            Object(Matching, "game/m_bgm.c"),
            Object(NonMatching, "game/m_birthday_ovl.c"),
            Object(NonMatching, "game/m_board_ovl.c"),
            Object(Matching, "game/m_calendar.c"),
            Object(NonMatching, "game/m_camera2.c"),
            Object(NonMatching, "game/m_card.c"),
            Object(NonMatching, "game/m_catalog_ovl.c"),
            Object(Matching, "game/m_choice.c"),
            Object(Matching, "game/m_clip.c"),
            Object(Matching, "game/m_cockroach.c"),
            Object(NonMatching, "game/m_collision_bg.c"),
            Object(Matching, "game/m_collision_obj.c"),
            Object(Matching, "game/m_common_data.c"),
            Object(Matching, "game/m_controller.c"),
            Object(NonMatching, "game/m_cpak.c"),
            Object(Matching, "game/m_debug.c"),
            Object(Matching, "game/m_debug_display.c"),
            Object(Matching, "game/m_debug_hayakawa.c"),
            Object(Matching, "game/m_debug_mode.c"),
            Object(Matching, "game/m_demo.c"),
            Object(Matching, "game/m_eappli.c"),
            Object(NonMatching, "game/m_editEndChk_ovl.c"),
            Object(NonMatching, "game/m_editor_ovl.c"),
            Object(NonMatching, "game/m_event.c"),
            Object(Matching, "game/m_fbdemo.c"),
            Object(Matching, "game/m_fbdemo_fade.c"),
            Object(Matching, "game/m_fbdemo_triforce.c"),
            Object(Matching, "game/m_fbdemo_wipe1.c"),
            Object(NonMatching, "game/m_field_assessment.c"),
            Object(Matching, "game/m_field_info.c"),
            Object(Matching, "game/m_field_make.c"),
            Object(Matching, "game/m_fishrecord.c"),
            Object(Matching, "game/m_flashrom.c"),
            Object(Matching, "game/m_font.c"),
            Object(Matching, "game/m_fuusen.c"),
            Object(Matching, "game/m_game_dlftbls.c"),
            Object(NonMatching, "game/m_hand_ovl.c"),
            Object(NonMatching, "game/m_handbill.c"),
            Object(NonMatching, "game/m_haniwaPortrait_ovl.c"),
            Object(NonMatching, "game/m_hboard_ovl.c"),
            Object(Matching, "game/m_home.c"),
            Object(Matching, "game/m_house.c"),
            Object(Matching, "game/m_huusui_room.c"),
            Object(NonMatching, "game/m_huusui_room_ovl.c"),
            Object(NonMatching, "game/m_island.c"),
            Object(NonMatching, "game/m_item_name.c"),
            Object(NonMatching, "game/m_kabu_manager.c"),
            Object(NonMatching, "game/m_kankyo.c"),
            Object(Matching, "game/m_land.c"),
            Object(NonMatching, "game/m_lib.c"),
            Object(NonMatching, "game/m_lights.c"),
            Object(NonMatching, "game/m_mail.c"),
            Object(NonMatching, "game/m_mail_check.c"),
            Object(NonMatching, "game/m_mail_check_ovl.c"),
            Object(NonMatching, "game/m_mail_password_check.c"),
            Object(NonMatching, "game/m_malloc.c"),
            Object(NonMatching, "game/m_map_ovl.c"),
            Object(NonMatching, "game/m_mark_room.c"),
            Object(NonMatching, "game/m_mark_room_ovl.c"),
            Object(NonMatching, "game/m_melody.c"),
            Object(NonMatching, "game/m_msg.c"),
            Object(NonMatching, "game/m_museum.c"),
            Object(NonMatching, "game/m_museum_display.c"),
            Object(NonMatching, "game/m_mushroom.c"),
            Object(NonMatching, "game/m_music_ovl.c"),
            Object(NonMatching, "game/m_name_table.c"),
            Object(NonMatching, "game/m_needlework.c"),
            Object(NonMatching, "game/m_notice.c"),
            Object(NonMatching, "game/m_notice_ovl.c"),
            Object(NonMatching, "game/m_npc.c"),
            Object(NonMatching, "game/m_npc_schedule.c"),
            Object(NonMatching, "game/m_npc_walk.c"),
            Object(NonMatching, "game/m_olib.c"),
            Object(NonMatching, "game/m_passwordChk_ovl.c"),
            Object(NonMatching, "game/m_passwordMake_ovl.c"),
            Object(NonMatching, "game/m_pause.c"),
            Object(NonMatching, "game/m_play.c"),
            Object(NonMatching, "game/m_player_call.c"),
            Object(NonMatching, "game/m_police_box.c"),
            Object(NonMatching, "game/m_post_office.c"),
            Object(NonMatching, "game/m_prenmi.c"),
            Object(NonMatching, "game/m_private.c"),
            Object(NonMatching, "game/m_quest.c"),
            Object(NonMatching, "game/m_random_field.c"),
            Object(NonMatching, "game/m_random_field_ovl.c"),
            Object(NonMatching, "game/m_rcp.c"),
            Object(NonMatching, "game/m_repay_ovl.c"),
            Object(NonMatching, "game/m_roll_lib.c"),
            Object(NonMatching, "game/m_room_type.c"),
            Object(NonMatching, "game/m_scene.c"),
            Object(NonMatching, "game/m_scene_ftr.c"),
            Object(NonMatching, "game/m_select.c"),
            Object(NonMatching, "game/m_shop.c"),
            Object(NonMatching, "game/m_skin_matrix.c"),
            Object(NonMatching, "game/m_snowman.c"),
            Object(NonMatching, "game/m_soncho.c"),
            Object(NonMatching, "game/m_start_data_init.c"),
            Object(NonMatching, "game/m_string.c"),
            Object(NonMatching, "game/m_submenu.c"),
            Object(NonMatching, "game/m_submenu_ovl.c"),
            Object(NonMatching, "game/m_time.c"),
            Object(NonMatching, "game/m_timeIn_ovl.c"),
            Object(NonMatching, "game/m_titledemo.c"),
            Object(NonMatching, "game/m_trademark.c"),
            Object(NonMatching, "game/m_train_control.c"),
            Object(NonMatching, "game/m_vibctl.c"),
            Object(NonMatching, "game/m_view.c"),
            Object(NonMatching, "game/m_warning_ovl.c"),
            Object(NonMatching, "game/m_watch_my_step.c"),
        ],
    },
    {
        "lib": "system",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "system/sys_dynamic.c"),
            Object(Matching, "system/sys_math.c"),
            Object(Matching, "system/sys_math3d.c"),
            Object(Matching, "system/sys_math_atan.c"),
            Object(Matching, "system/sys_matrix.c"),
            Object(Matching, "system/sys_romcheck.c"),
            Object(Matching, "system/sys_stacks.c"),
            Object(Matching, "system/sys_ucode.c"),
            Object(Matching, "system/sys_vimgr.c"),
        ],
    },
]



# Define our custom asset processing scripts
config.custom_build_rules = [
    {
        "name": "convert_pal16",
        "command": "$python tools/converters/pal16dis.py $in $out",
        "description": "CONVERT $palette"
    },
    {
        "name": "convert_vtx",
        "command": "$python tools/converters/vtxdis.py $in $out",
        "description": "CONVERT $vtxdata"
    },
]
config.custom_build_steps = []

# Grab the specific GameID so we can format our strings properly
version = VERSIONS[version_num]

# This generates the build steps needed for preprocessing
def emit_build_rule(asset):
    match asset["type"]:
        case "pal16":
            symbol = asset["symbol"]
            binary_path = asset["binary"]
            include_path = binary_path.replace(".bin", ".inc")

            config.custom_build_steps.append({
                "type": "pre-compile",
                "rule": "convert_pal16",
                "inputs": f"build/{version}/bin/{binary_path}",
                "outputs": f"build/{version}/include/{include_path}",
                "variables": {
                    "palette": f"{symbol}",
                },
            })

        case "vtx":
            symbol = asset["symbol"]
            binary_path = asset["binary"]
            include_path = binary_path.replace(".bin", ".inc")

            config.custom_build_steps.append({
                "type": "pre-compile",
                "rule": "convert_vtx",
                "inputs": f"build/{version}/bin/{binary_path}",
                "outputs": f"build/{version}/include/{include_path}",
                "variables": {
                    "vtxdata": f"{symbol}",
                },
            })

        case _:
            print("Unknown asset type: " + asset["type"])

# Parse the config and create the build rules for all our assets
yaml_data = yaml.safe_load(open(f"config/{version}/config.yml"))

for asset in yaml_data.get("extract", []):
    if "type" in asset:
        emit_build_rule(asset)

for module in yaml_data.get("modules", []):
    for asset in module.get("extract", []):
        if "type" in asset:
            emit_build_rule(asset)



# configure script options
if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress and write progress.json
    config.progress_each_module = args.verbose
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
