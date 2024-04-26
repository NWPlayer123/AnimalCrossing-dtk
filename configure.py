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
