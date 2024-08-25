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
import os
import sys
import requests
import re
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
    "-v",
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
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
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
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.debug = args.debug
config.generate_map = args.map
config.non_matching = args.non_matching
config.sjiswrap_path = args.sjiswrap
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-1"
config.compilers_tag = "20240706"
config.dtk_tag = "v0.9.4"
config.objdiff_tag = "v2.0.0-beta.3"
config.sjiswrap_tag = "v1.1.1"
config.wibo_tag = "0.6.11"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"-I build/{config.version}/bin",
    f"--defsym version={version_num}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
    "-warn off",
]
# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    # Define the platform
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    # For Multibyte
    "-multibyte",
    "-char unsigned",
    # Platform specific
    "-fp hardware",
    "-Cpp_exceptions off",
    '-pragma "cats off"',
    # Default compiler flags (turn off if needed)
    # "-W all",
    # "-O4,p",
    # "-inline auto",
    '-pragma "warn_notinlined off"',
    # Helpful linker flags
    "-maxerrors 1",
    "-nosyspath",
    # dtk-specific includes
    "-i include",
    f"-i build/{config.version}/include",
    f"-DVERSION={version_num}",
]

# Debug flags
if config.debug:
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.extend(["-sym on", "-DNDEBUG=1"])

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-gccinc",
    "-common off",
    "-inline auto",
]

cflags_common = [
    "-d _LANGUAGE_C",
    "-d F3DEX_GBI_2",
    "-d MUST_MATCH",
]

# DOL flags
cflags_static = [
    *cflags_base,
    *cflags_common,
    "-O4,s",
]

# REL flags
cflags_foresta = [
    *cflags_base,
    "-O4,s",
    "-sdata 0",
    "-sdata2 0",
    # Needed for N64 SDK
    "-d _LANGUAGE_C",
    "-d F3DEX_GBI_2",
    # Project-specific stuff
    "-d MUST_MATCH",
    "-d IS_REL",
]

config.linker_version = "GC/1.3.2"


# Helper function for Dolphin libraries
def DolphinLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5n",
        "cflags": [
            *cflags_static,
            "-O4,p",
            "-inline auto",
        ],
        "host": False,
        "src_dir": "src/static",
        "objects": objects,
    }


# Helper function for JSystem libraries
def JSystemLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2",
        "cflags": [*cflags_static, "-char signed"],
        "host": False,
        "src_dir": "src/static",
        "objects": objects,
    }


# Helper function for REL script objects
def Rel(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2r",
        "cflags": cflags_foresta,
        "host": False,
        "objects": objects,
    }


Matching = True  # Object matches and should be linked
NonMatching = False  # Object does not match and should not be linked
Equivalent = (
    config.non_matching
)  # Object should be linked when configured with --non-matching

config.warn_missing_config = False
config.warn_missing_source = True
config.libs = [
    {
        "lib": "bootdata",
        "mw_version": config.linker_version,
        "cflags": cflags_static,
        "host": False,
        "src_dir": "src/static",
        "objects": [
            Object(Matching, "bootdata/gam_win1.c"),
            Object(Matching, "bootdata/gam_win2.c"),
            Object(Matching, "bootdata/gam_win3.c"),
            Object(Matching, "bootdata/logo_nin.c"),
        ],
    },
    DolphinLib(
        "amcstubs",
        [
            Object(Matching, "dolphin/amcstubs/AmcExi2Stubs.c"),
        ],
    ),
    DolphinLib(
        "base",
        [
            Object(Matching, "dolphin/base/PPCArch.c"),
        ],
    ),
    DolphinLib(
        "gx",
        [
            Object(Matching, "dolphin/gx/GXStubs.c"),
        ],
    ),
    DolphinLib(
        "odenotstub",
        [
            Object(Matching, "dolphin/odenotstub/odenotstub.c"),
        ],
    ),
    DolphinLib(
        "os",
        [
            Object(Matching, "dolphin/os/__ppc_eabi_init.cpp"),
            Object(Matching, "dolphin/os/__start.c"),
            Object(Matching, "dolphin/os/OSAlarm.c"),
            Object(Matching, "dolphin/os/OSArena.c"),
            Object(Matching, "dolphin/os/OSAudioSystem.c"),
            Object(Matching, "dolphin/os/OSCache.c"),
            Object(Matching, "dolphin/os/OSContext.c"),
            Object(Matching, "dolphin/os/OSError.c"),
            Object(NonMatching, "dolphin/os/OSInterrupt.c"),
            Object(NonMatching, "dolphin/os/OSMemory.c"),
            Object(Matching, "dolphin/os/OSRtc.c"),
        ],
    ),
    {
        "lib": "Famicom",
        "mw_version": "GC/1.3.2r",
        "cflags": [
            *cflags_static,
            "-sdata 0",
            "-sdata2 0",
            "-inline on",
        ],
        "host": False,
        "src_dir": "src/static",
        "objects": [
            Object(Matching, "Famicom/famicom.cpp"),
            Object(Matching, "Famicom/famicom_nesinfo.cpp"),
        ],
    },
    {
        "lib": "gba",
        "mw_version": "GC/1.2.5n",
        "cflags": [*cflags_static, "-inline auto", "-O4,p"],
        "host": False,
        "src_dir": "src/static",
        "objects": [
            Object(Matching, "gba/GBA.c"),
            Object(Matching, "gba/GBAGetProcessStatus.c"),
            Object(Matching, "gba/GBAJoyBoot.c"),
            Object(Matching, "gba/GBARead.c"),
            Object(Matching, "gba/GBAWrite.c"),
            Object(Matching, "gba/GBAXfer.c"),
        ],
    },
    {
        "lib": "gba2",
        "mw_version": config.linker_version,
        "cflags": [
            *cflags_static,
            "-sdata 0",
            "-sdata2 0",
            "-inline on",
        ],
        "host": False,
        "src_dir": "src/static",
        "objects": [
            Object(Matching, "gba2/JoyBoot.c"),
        ],
    },
    {
        "lib": "jaudio_NES_game",
        "mw_version": "GC/1.3.2",
        "cflags": [*cflags_static, "-O0", "-inline off", "-lang=c++"],
        "host": False,
        "src_dir": "src/static",
        "objects": [
            Object(Matching, "jaudio_NES/game/audioheaders.c"),
            Object(Matching, "jaudio_NES/game/dummyprobe.c"),
            Object(NonMatching, "jaudio_NES/game/emusound.c"),
            Object(NonMatching, "jaudio_NES/game/game64.c"),
            Object(Matching, "jaudio_NES/game/kappa.c"),
            Object(Matching, "jaudio_NES/game/melody.c"),
            Object(Matching, "jaudio_NES/game/radio.c"),
            Object(Matching, "jaudio_NES/game/rhythm.c"),
            Object(Matching, "jaudio_NES/game/staff.c"),
            Object(Matching, "jaudio_NES/game/verysimple.c"),
        ],
    },
    {
        "lib": "jaudio_NES_internal",
        "mw_version": "GC/1.3.2",
        "cflags": [
            *cflags_static,
            "-lang=c++",
            "-common on",
            "-func_align 32",
            "-inline off",
            "-str readonly",
            "-char signed",
            "-sdata 8",
            "-sdata2 8",
        ],
        "host": False,
        "src_dir": "src/static",
        "objects": [
            Object(Matching, "jaudio_NES/internal/aictrl.c"),
            Object(Matching, "jaudio_NES/internal/astest.c"),
            Object(Matching, "jaudio_NES/internal/audiothread.c"),
            Object(Matching, "jaudio_NES/internal/cpubuf.c"),
            Object(Matching, "jaudio_NES/internal/dspboot.c"),
            Object(Matching, "jaudio_NES/internal/dspbuf.c"),
            Object(Matching, "jaudio_NES/internal/dspproc.c"),
            Object(Matching, "jaudio_NES/internal/dummyrom.c"),
            Object(Matching, "jaudio_NES/internal/dvdthread.c"),
            Object(Matching, "jaudio_NES/internal/neosthread.c"),
            Object(Matching, "jaudio_NES/internal/os.c"),
            Object(Matching, "jaudio_NES/internal/playercall.c"),
            Object(Matching, "jaudio_NES/internal/random.c"),
            Object(Matching, "jaudio_NES/internal/sample.c"),
            Object(Matching, "jaudio_NES/internal/streamctrl.c"),
            Object(Matching, "jaudio_NES/internal/sub_sys.c"),
            Object(Matching, "jaudio_NES/internal/track.c"),
        ],
    },
    JSystemLib(
        "JGadget",
        [
            Object(Matching, "JSystem/JGadget/linklist.cpp"),
        ],
    ),
    JSystemLib(
        "JKernel",
        [
            Object(Matching, "JSystem/JKernel/JKRAram.cpp"),
            Object(Matching, "JSystem/JKernel/JKRAramArchive.cpp"),
            Object(Matching, "JSystem/JKernel/JKRAramBlock.cpp"),
            Object(Matching, "JSystem/JKernel/JKRAramHeap.cpp"),
            Object(Matching, "JSystem/JKernel/JKRAramPiece.cpp"),
            Object(Matching, "JSystem/JKernel/JKRAramStream.cpp"),
            Object(NonMatching, "JSystem/JKernel/JKRArchivePri.cpp"),
            Object(NonMatching, "JSystem/JKernel/JKRArchivePub.cpp"),
            Object(Matching, "JSystem/JKernel/JKRCompArchive.cpp"),
            Object(Matching, "JSystem/JKernel/JKRDecomp.cpp"),
            Object(Matching, "JSystem/JKernel/JKRDisposer.cpp"),
            Object(Matching, "JSystem/JKernel/JKRDvdAramRipper.cpp"),
            Object(Matching, "JSystem/JKernel/JKRDvdArchive.cpp"),
            Object(Matching, "JSystem/JKernel/JKRDvdFile.cpp"),
            Object(Matching, "JSystem/JKernel/JKRDvdRipper.cpp"),
            Object(Matching, "JSystem/JKernel/JKRExpHeap.cpp"),
            Object(Matching, "JSystem/JKernel/JKRFileFinder.cpp"),
            Object(Matching, "JSystem/JKernel/JKRFileLoader.cpp"),
            Object(Matching, "JSystem/JKernel/JKRHeap.cpp"),
            Object(Matching, "JSystem/JKernel/JKRMemArchive.cpp"),
            Object(Matching, "JSystem/JKernel/JKRThread.cpp"),
        ],
    ),
    JSystemLib(
        "JSupport",
        [
            Object(NonMatching, "JSystem/JSupport/JSUFileStream.cpp"),
            Object(Matching, "JSystem/JSupport/JSUInputStream.cpp"),
        ],
    ),
    JSystemLib(
        "JUtility",
        [
            Object(Matching, "JSystem/JUtility/JUTFontData_Ascfont_fix12.s"),
            Object(Matching, "JSystem/JUtility/JUTGamePad.cpp"),
        ],
    ),
    {
        "lib": "libc64",
        "mw_version": config.linker_version,
        "cflags": cflags_static,
        "host": False,
        "src_dir": "src/static",
        "objects": [
            Object(NonMatching, "libc64/__osMalloc.c"),
            Object(Matching, "libc64/aprintf.c"),
            Object(Matching, "libc64/malloc.c"),
            Object(Matching, "libc64/math64.c"),
            Object(Matching, "libc64/qrand.c"),
            Object(Matching, "libc64/sleep.c"),
            Object(Matching, "libc64/sprintf.c"),
        ],
    },
    {
        "lib": "libforest",
        "mw_version": config.linker_version,
        "cflags": cflags_static,
        "host": False,
        "src_dir": "src/static",
        "objects": [
            Object(NonMatching, "libforest/emu64/emu64.cc"),
            Object(Matching, "libforest/fault.c"),
            Object(Matching, "libforest/osreport.c"),
            Object(Matching, "libforest/ReconfigBATs.c"),
        ],
    },
    {
        "lib": "libu64",
        "mw_version": config.linker_version,
        "cflags": cflags_static,
        "host": False,
        "src_dir": "src/static",
        "objects": [
            Object(Matching, "libu64/debug.c"),
            Object(Matching, "libu64/gfxprint.c"),
            Object(Matching, "libu64/gfxprint_data.c"),
            Object(Matching, "libu64/pad.c"),
        ],
    },
    {
        "lib": "libultra",
        "mw_version": config.linker_version,
        "cflags": [*cflags_static, "-O4,p", "-inline on"],
        "host": False,
        "src_dir": "src/static",
        "objects": [
            Object(NonMatching, "libultra/gu/lookathil.c"),
            Object(NonMatching, "libultra/gu/mtxutil.c"),
            Object(Matching, "libultra/gu/normalize.c"),
            Object(NonMatching, "libultra/gu/ortho.c"),
            Object(Matching, "libultra/gu/scale.c"),
            Object(Matching, "libultra/gu/sins.c"),
            Object(Matching, "libultra/gu/translate.c"),
            Object(NonMatching, "libultra/contreaddata.c"),
            Object(NonMatching, "libultra/initialize.c"),
            Object(Matching, "libultra/ultra.c"),
            Object(Matching, "libultra/xldtob.c"),
            Object(Matching, "libultra/xlitob.c"),
            Object(NonMatching, "libultra/xprintf.c"),
        ],
    },
    {
        "lib": "MSL_C.PPCEABI.bare.H",
        "mw_version": config.linker_version,
        "cflags": [*cflags_static, "-O4,p"],
        "host": False,
        "src_dir": "src/static",
        "objects": [
            Object(Matching, "MSL_C.PPCEABI.bare.H/rand.c"),
        ],
    },
    {
        "lib": "Runtime.PPCEABI.H",
        "mw_version": config.linker_version,
        "cflags": [*cflags_base, "-O4,s"],
        "host": False,
        "src_dir": "src/static",
        "objects": [
            Object(Matching, "Runtime.PPCEABI.H/global_destructor_chain.c"),
            Object(Matching, "Runtime.PPCEABI.H/__init_cpp_exceptions.cpp"),
            Object(Matching, "Runtime.PPCEABI.H/__mem.c"),
        ],
    },
    {
        "lib": "TRK_MINNOW_DOLPHIN",
        "mw_version": config.linker_version,
        "cflags": [
            *cflags_base,
            *cflags_common,
            "-O4,p",
            "-sdata 0",
            "-sdata2 0",
            "-inline deferred",
            "-rostr",
        ],
        "host": False,
        "src_dir": "src/static",
        "objects": [
            Object(Matching, "TRK_MINNOW_DOLPHIN/__exception.s"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/dispatch.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/dolphin_trk.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/mainloop.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/mem_TRK.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/nubevent.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/nubinit.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/usr_put.c"),
        ],
    },
    Rel(
        "foresta",
        [
            Object(Matching, "audio.c"),
            Object(Matching, "bg_cherry_item.c"),
            Object(Matching, "bg_item.c"),
            Object(Matching, "bg_police_item.c"),
            Object(Matching, "bg_post_item.c"),
            Object(Matching, "bg_winter_item.c"),
            Object(Matching, "bg_xmas_item.c"),
            Object(Matching, "c_keyframe.c"),
            Object(Matching, "evw_anime.c"),
            Object(Matching, "ev_cherry_manager.c"),
            Object(Matching, "executor.c"),
            Object(Matching, "famicom_emu.c"),
            Object(Matching, "first_game.c"),
            Object(Matching, "f_furniture.c"),
            Object(Matching, "game.c"),
            Object(Matching, "gamealloc.c"),
            Object(Matching, "gfxalloc.c"),
            Object(Matching, "graph.c"),
            Object(Matching, "irqmgr.c"),
            Object(Matching, "lb_reki.c"),
            Object(Matching, "lb_rtc.c"),
            Object(Matching, "main.c"),
            Object(Matching, "padmgr.c"),
            Object(Matching, "player_select.c"),
            Object(Matching, "PreRender.c"),
            Object(Matching, "save_menu.c"),
            Object(Matching, "second_game.c"),
            Object(Matching, "THA_GA.c"),
            Object(Matching, "TwoHeadArena.c"),
            Object(Matching, "zurumode.c"),
        ],
    ),
    Rel(
        "dataobject",
        [
            Object(NonMatching, "data/combi/data_combi.c"),
            Object(NonMatching, "data/field/bg/acre/bg_data.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_player_select/grd_player_select.c"
            ),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_player_select/grd_player_select_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_post_office/grd_post_office.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c1_1/grd_s_c1_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c1_2/grd_s_c1_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c1_3/grd_s_c1_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c1_4/grd_s_c1_4.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c1_5/grd_s_c1_5.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c1_r1_1/grd_s_c1_r1_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c1_r1_1/grd_s_c1_r1_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c1_r1_2/grd_s_c1_r1_2.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c1_r1_2/grd_s_c1_r1_2_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c1_r1_3/grd_s_c1_r1_3.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c1_r1_3/grd_s_c1_r1_3_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c1_r2_1/grd_s_c1_r2_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c1_r2_1/grd_s_c1_r2_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c1_r2_2/grd_s_c1_r2_2.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c1_r2_2/grd_s_c1_r2_2_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c1_r2_3/grd_s_c1_r2_3.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c1_r2_3/grd_s_c1_r2_3_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c1_r3_1/grd_s_c1_r3_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c1_r3_1/grd_s_c1_r3_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c1_r3_2/grd_s_c1_r3_2.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c1_r3_2/grd_s_c1_r3_2_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c1_r3_3/grd_s_c1_r3_3.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c1_r3_3/grd_s_c1_r3_3_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c1_s_1/grd_s_c1_s_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c1_s_2/grd_s_c1_s_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c1_s_3/grd_s_c1_s_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c1_s_4/grd_s_c1_s_4.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c2_1/grd_s_c2_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c2_2/grd_s_c2_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c2_3/grd_s_c2_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c2_3/grd_s_c2_3_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c2_r1_1/grd_s_c2_r1_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c2_r1_1/grd_s_c2_r1_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c2_r1_2/grd_s_c2_r1_2.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c2_r1_2/grd_s_c2_r1_2_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c2_r2_1/grd_s_c2_r2_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c2_r2_1/grd_s_c2_r2_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c2_r2_2/grd_s_c2_r2_2.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c2_r2_2/grd_s_c2_r2_2_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c2_s_1/grd_s_c2_s_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c2_s_2/grd_s_c2_s_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c2_s_3/grd_s_c2_s_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c3_1/grd_s_c3_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c3_2/grd_s_c3_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c3_2/grd_s_c3_2_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c3_3/grd_s_c3_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c3_r1_1/grd_s_c3_r1_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c3_r1_1/grd_s_c3_r1_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c3_r1_2/grd_s_c3_r1_2.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c3_r1_2/grd_s_c3_r1_2_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c3_r2_1/grd_s_c3_r2_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c3_r2_1/grd_s_c3_r2_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c3_r2_2/grd_s_c3_r2_2.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c3_r2_2/grd_s_c3_r2_2_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c3_s_1/grd_s_c3_s_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_c3_s_1/grd_s_c3_s_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c4_1/grd_s_c4_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c4_2/grd_s_c4_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c4_3/grd_s_c4_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c4_r1_1/grd_s_c4_r1_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c4_r1_1/grd_s_c4_r1_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c4_r1_2/grd_s_c4_r1_2.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c4_r1_2/grd_s_c4_r1_2_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c4_r2_1/grd_s_c4_r2_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c4_r2_1/grd_s_c4_r2_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c4_r2_2/grd_s_c4_r2_2.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c4_r2_2/grd_s_c4_r2_2_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c4_r3_1/grd_s_c4_r3_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c4_r3_1/grd_s_c4_r3_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c4_r3_2/grd_s_c4_r3_2.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c4_r3_2/grd_s_c4_r3_2_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c4_s_1/grd_s_c4_s_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c4_s_2/grd_s_c4_s_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c5_1/grd_s_c5_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c5_2/grd_s_c5_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c5_3/grd_s_c5_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c5_r1_1/grd_s_c5_r1_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c5_r1_1/grd_s_c5_r1_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c5_r1_2/grd_s_c5_r1_2.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c5_r1_2/grd_s_c5_r1_2_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c5_r2_1/grd_s_c5_r2_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c5_r2_1/grd_s_c5_r2_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c5_r2_2/grd_s_c5_r2_2.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c5_r2_2/grd_s_c5_r2_2_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c5_r3_1/grd_s_c5_r3_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c5_r3_1/grd_s_c5_r3_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c5_r3_2/grd_s_c5_r3_2.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c5_r3_2/grd_s_c5_r3_2_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c5_s_1/grd_s_c5_s_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c5_s_2/grd_s_c5_s_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c6_1/grd_s_c6_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c6_2/grd_s_c6_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c6_3/grd_s_c6_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c6_r1_1/grd_s_c6_r1_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c6_r1_1/grd_s_c6_r1_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c6_r1_2/grd_s_c6_r1_2.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c6_r1_2/grd_s_c6_r1_2_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c6_r3_1/grd_s_c6_r3_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c6_r3_1/grd_s_c6_r3_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c6_s_1/grd_s_c6_s_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c7_1/grd_s_c7_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c7_2/grd_s_c7_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c7_3/grd_s_c7_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c7_3/grd_s_c7_3_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c7_r1_1/grd_s_c7_r1_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c7_r1_1/grd_s_c7_r1_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c7_r1_2/grd_s_c7_r1_2.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c7_r1_2/grd_s_c7_r1_2_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c7_r3_1/grd_s_c7_r3_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c7_r3_1/grd_s_c7_r3_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c7_r3_2/grd_s_c7_r3_2.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_c7_r3_2/grd_s_c7_r3_2_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_c7_s_1/grd_s_c7_s_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c7_s_2/grd_s_c7_s_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_c7_s_3/grd_s_c7_s_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_e1_1/grd_s_e1_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_e1_r1_1/grd_s_e1_r1_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_e1_r1_1/grd_s_e1_r1_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_e2_1/grd_s_e2_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_e2_c1_1/grd_s_e2_c1_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_e2_m_1/grd_s_e2_m_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_e2_o_1/grd_s_e2_o_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_e2_t_1/grd_s_e2_t_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_e3_1/grd_s_e3_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_e3_c1_1/grd_s_e3_c1_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_e3_m_1/grd_s_e3_m_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_e3_o_1/grd_s_e3_o_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_e3_t_1/grd_s_e3_t_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_e4_1/grd_s_e4_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_e5_1/grd_s_e5_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_1/grd_s_f_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_10/grd_s_f_10.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_2/grd_s_f_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_3/grd_s_f_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_4/grd_s_f_4.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_4/grd_s_f_4_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_5/grd_s_f_5.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_5/grd_s_f_5_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_6/grd_s_f_6.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_6/grd_s_f_6_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_7/grd_s_f_7.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_8/grd_s_f_8.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_9/grd_s_f_9.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_ko_1/grd_s_f_ko_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_ko_2/grd_s_f_ko_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_ko_3/grd_s_f_ko_3.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_f_ko_3/grd_s_f_ko_3_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_mh_1/grd_s_f_mh_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_mh_2/grd_s_f_mh_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_mh_3/grd_s_f_mh_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_mu_1/grd_s_f_mu_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_mu_2/grd_s_f_mu_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_mu_3/grd_s_f_mu_3.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_f_mu_3/grd_s_f_mu_3_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_pk_1/grd_s_f_pk_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_pk_2/grd_s_f_pk_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_f_pk_3/grd_s_f_pk_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_hole_test/grd_s_hole_test.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_il_1/grd_s_il_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_il_2/grd_s_il_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_il_3/grd_s_il_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_il_4/grd_s_il_4.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_ir_1/grd_s_ir_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_ir_2/grd_s_ir_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_ir_3/grd_s_ir_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_ir_4/grd_s_ir_4.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_1/grd_s_m_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_10/grd_s_m_10.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_2/grd_s_m_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_3/grd_s_m_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_4/grd_s_m_4.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_5/grd_s_m_5.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_6/grd_s_m_6.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_7/grd_s_m_7.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_8/grd_s_m_8.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_9/grd_s_m_9.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_r1_1/grd_s_m_r1_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_m_r1_1/grd_s_m_r1_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_r1_2/grd_s_m_r1_2.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_m_r1_2/grd_s_m_r1_2_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_r1_3/grd_s_m_r1_3.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_m_r1_3/grd_s_m_r1_3_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_r1_4/grd_s_m_r1_4.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_m_r1_4/grd_s_m_r1_4_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_r1_5/grd_s_m_r1_5.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_m_r1_5/grd_s_m_r1_5_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_r1_b_1/grd_s_m_r1_b_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_m_r1_b_1/grd_s_m_r1_b_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_r1_b_2/grd_s_m_r1_b_2.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_m_r1_b_2/grd_s_m_r1_b_2_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_r1_b_3/grd_s_m_r1_b_3.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_m_r1_b_3/grd_s_m_r1_b_3_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_ta_1/grd_s_m_ta_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_m_ta_1/grd_s_m_ta_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_ta_2/grd_s_m_ta_2.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_m_ta_2/grd_s_m_ta_2_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_ta_3/grd_s_m_ta_3.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_m_ta_3/grd_s_m_ta_3_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_wf_1/grd_s_m_wf_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_m_wf_1/grd_s_m_wf_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_wf_2/grd_s_m_wf_2.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_m_wf_2/grd_s_m_wf_2_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_m_wf_3/grd_s_m_wf_3.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_m_wf_3/grd_s_m_wf_3_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_1/grd_s_o_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_1/grd_s_o_1_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_10/grd_s_o_10.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_10/grd_s_o_10_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_2/grd_s_o_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_2/grd_s_o_2_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_3/grd_s_o_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_3/grd_s_o_3_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_4/grd_s_o_4.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_4/grd_s_o_4_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_5/grd_s_o_5.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_5/grd_s_o_5_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_6/grd_s_o_6.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_6/grd_s_o_6_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_7/grd_s_o_7.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_7/grd_s_o_7_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_8/grd_s_o_8.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_8/grd_s_o_8_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_9/grd_s_o_9.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_9/grd_s_o_9_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_i_1/grd_s_o_i_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_i_2/grd_s_o_i_2.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_o_i_2/grd_s_o_i_2_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_r1_1/grd_s_o_r1_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_o_r1_1/grd_s_o_r1_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_r1_2/grd_s_o_r1_2.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_o_r1_2/grd_s_o_r1_2_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_r1_3/grd_s_o_r1_3.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_o_r1_3/grd_s_o_r1_3_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_r1_4/grd_s_o_r1_4.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_o_r1_4/grd_s_o_r1_4_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_r1_5/grd_s_o_r1_5.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_o_r1_5/grd_s_o_r1_5_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_r1_b_1/grd_s_o_r1_b_1.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_o_r1_b_1/grd_s_o_r1_b_1_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_r1_b_2/grd_s_o_r1_b_2.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_o_r1_b_2/grd_s_o_r1_b_2_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_r1_b_3/grd_s_o_r1_b_3.c"),
            Object(
                NonMatching,
                "data/field/bg/acre/grd_s_o_r1_b_3/grd_s_o_r1_b_3_evw_anime.c",
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_ta_1/grd_s_o_ta_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_o_ta_1/grd_s_o_ta_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_ta_2/grd_s_o_ta_2.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_o_ta_2/grd_s_o_ta_2_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_ta_3/grd_s_o_ta_3.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_o_ta_3/grd_s_o_ta_3_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_wf_1/grd_s_o_wf_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_o_wf_1/grd_s_o_wf_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_wf_2/grd_s_o_wf_2.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_o_wf_2/grd_s_o_wf_2_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_o_wf_3/grd_s_o_wf_3.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_o_wf_3/grd_s_o_wf_3_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r1_1/grd_s_r1_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r1_1/grd_s_r1_1_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r1_2/grd_s_r1_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r1_2/grd_s_r1_2_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r1_3/grd_s_r1_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r1_3/grd_s_r1_3_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r1_4/grd_s_r1_4.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r1_4/grd_s_r1_4_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r1_b_1/grd_s_r1_b_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r1_b_1/grd_s_r1_b_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r1_b_2/grd_s_r1_b_2.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r1_b_2/grd_s_r1_b_2_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r1_b_3/grd_s_r1_b_3.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r1_b_3/grd_s_r1_b_3_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r1_p_1/grd_s_r1_p_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r1_p_1/grd_s_r1_p_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r2_1/grd_s_r2_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r2_1/grd_s_r2_1_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r2_2/grd_s_r2_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r2_2/grd_s_r2_2_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r2_3/grd_s_r2_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r2_3/grd_s_r2_3_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r2_4/grd_s_r2_4.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r2_4/grd_s_r2_4_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r2_b_1/grd_s_r2_b_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r2_b_1/grd_s_r2_b_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r2_b_2/grd_s_r2_b_2.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r2_b_2/grd_s_r2_b_2_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r2_b_3/grd_s_r2_b_3.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r2_b_3/grd_s_r2_b_3_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r2_p_1/grd_s_r2_p_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r2_p_1/grd_s_r2_p_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r3_1/grd_s_r3_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r3_1/grd_s_r3_1_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r3_2/grd_s_r3_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r3_2/grd_s_r3_2_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r3_3/grd_s_r3_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r3_3/grd_s_r3_3_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r3_4/grd_s_r3_4.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r3_4/grd_s_r3_4_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r3_b_1/grd_s_r3_b_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r3_b_1/grd_s_r3_b_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r3_b_2/grd_s_r3_b_2.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r3_b_2/grd_s_r3_b_2_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r3_b_3/grd_s_r3_b_3.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r3_b_3/grd_s_r3_b_3_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r3_p_1/grd_s_r3_p_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r3_p_1/grd_s_r3_p_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r4_1/grd_s_r4_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r4_1/grd_s_r4_1_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r4_2/grd_s_r4_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r4_2/grd_s_r4_2_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r4_3/grd_s_r4_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r4_3/grd_s_r4_3_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r4_b_1/grd_s_r4_b_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r4_b_1/grd_s_r4_b_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r4_b_2/grd_s_r4_b_2.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r4_b_2/grd_s_r4_b_2_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r4_p_1/grd_s_r4_p_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r4_p_1/grd_s_r4_p_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r5_1/grd_s_r5_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r5_1/grd_s_r5_1_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r5_2/grd_s_r5_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r5_2/grd_s_r5_2_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r5_3/grd_s_r5_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r5_3/grd_s_r5_3_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r5_b_1/grd_s_r5_b_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r5_b_1/grd_s_r5_b_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r5_b_2/grd_s_r5_b_2.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r5_b_2/grd_s_r5_b_2_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r5_p_1/grd_s_r5_p_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r5_p_1/grd_s_r5_p_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r6_1/grd_s_r6_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r6_1/grd_s_r6_1_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r6_2/grd_s_r6_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r6_2/grd_s_r6_2_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r6_3/grd_s_r6_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r6_3/grd_s_r6_3_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r6_b_1/grd_s_r6_b_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r6_b_1/grd_s_r6_b_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r6_b_2/grd_s_r6_b_2.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r6_b_2/grd_s_r6_b_2_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r6_p_1/grd_s_r6_p_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r6_p_1/grd_s_r6_p_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r7_1/grd_s_r7_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r7_1/grd_s_r7_1_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r7_2/grd_s_r7_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r7_2/grd_s_r7_2_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r7_3/grd_s_r7_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r7_3/grd_s_r7_3_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_r7_b_1/grd_s_r7_b_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r7_b_1/grd_s_r7_b_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r7_b_2/grd_s_r7_b_2.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r7_b_2/grd_s_r7_b_2_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_r7_p_1/grd_s_r7_p_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_r7_p_1/grd_s_r7_p_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_1/grd_s_t_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_1/grd_s_t_1_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_10/grd_s_t_10.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_2/grd_s_t_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_3/grd_s_t_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_4/grd_s_t_4.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_4/grd_s_t_4_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_5/grd_s_t_5.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_5/grd_s_t_5_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_6/grd_s_t_6.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_7/grd_s_t_7.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_8/grd_s_t_8.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_9/grd_s_t_9.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_po_1/grd_s_t_po_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_po_2/grd_s_t_po_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_po_3/grd_s_t_po_3.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_t_po_3/grd_s_t_po_3_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_r1_1/grd_s_t_r1_1.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_t_r1_1/grd_s_t_r1_1_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_r1_2/grd_s_t_r1_2.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_t_r1_2/grd_s_t_r1_2_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_r1_3/grd_s_t_r1_3.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_t_r1_3/grd_s_t_r1_3_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_r1_4/grd_s_t_r1_4.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_t_r1_4/grd_s_t_r1_4_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_r1_5/grd_s_t_r1_5.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_t_r1_5/grd_s_t_r1_5_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_sh_1/grd_s_t_sh_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_sh_2/grd_s_t_sh_2.c"),
            Object(
                NonMatching, "data/field/bg/acre/grd_s_t_sh_2/grd_s_t_sh_2_evw_anime.c"
            ),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_sh_3/grd_s_t_sh_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_st1_1/grd_s_t_st1_1.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_st1_2/grd_s_t_st1_2.c"),
            Object(NonMatching, "data/field/bg/acre/grd_s_t_st1_3/grd_s_t_st1_3.c"),
            Object(NonMatching, "data/field/bg/acre/grd_yamishop/grd_yamishop.c"),
            Object(NonMatching, "data/field/bg/acre/myr_etc/myr_etc.c"),
            Object(NonMatching, "data/field/bg/acre/police_indoor/police_indoor.c"),
            Object(NonMatching, "data/field/bg/acre/rom_museum1/rom_museum1.c"),
            Object(NonMatching, "data/field/bg/acre/rom_museum2/rom_museum2.c"),
            Object(NonMatching, "data/field/bg/acre/rom_museum3/rom_museum3.c"),
            Object(NonMatching, "data/field/bg/acre/rom_museum5/rom_museum5.c"),
            Object(NonMatching, "data/field/bg/acre/rom_shop4_1/rom_shop4_1.c"),
            Object(NonMatching, "data/field/bg/acre/rom_shop4_fuku/rom_shop4_fuku.c"),
            Object(NonMatching, "data/field/bg/acre/rom_tailor/rom_tailor.c"),
            Object(NonMatching, "data/field/bg/acre/rom_tent/rom_tent.c"),
            Object(NonMatching, "data/field/bg/acre/rom_toudai/rom_toudai.c"),
            Object(NonMatching, "data/field/bg/acre/rom_train_in/rom_train_in.c"),
            Object(NonMatching, "data/field/bg/acre/rom_uranai/rom_uranai.c"),
            Object(NonMatching, "data/field/bg/acre/rom_uranai/rom_uranai_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/room01/room01.c"),
            Object(NonMatching, "data/field/bg/acre/tmp/tmp.c"),
            Object(NonMatching, "data/field/bg/acre/tmp2/tmp2.c"),
            Object(NonMatching, "data/field/bg/acre/tmp3/tmp3.c"),
            Object(NonMatching, "data/field/bg/acre/tmp4/tmp4.c"),
            Object(NonMatching, "data/field/bg/acre/tmpr/tmpr.c"),
            Object(NonMatching, "data/field/bg/acre/tmpr/tmpr_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/tmpr2/tmpr2.c"),
            Object(NonMatching, "data/field/bg/acre/tmpr2/tmpr2_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/tmpr3/tmpr3.c"),
            Object(NonMatching, "data/field/bg/acre/tmpr3/tmpr3_evw_anime.c"),
            Object(NonMatching, "data/field/bg/acre/tmpr4/tmpr4.c"),
            Object(NonMatching, "data/field/bg/acre/tmpr4/tmpr4_evw_anime.c"),
            Object(NonMatching, "data/field/bg/beach_pal.c"),
            Object(NonMatching, "data/field/bg/bush_pal.c"),
            Object(NonMatching, "data/field/bg/cliff_pal.c"),
            Object(NonMatching, "data/field/bg/earth_pal.c"),
            Object(NonMatching, "data/field/bg/flower/obj_flower.c"),
            Object(NonMatching, "data/field/bg/flower/obj_flower_leaf.c"),
            Object(NonMatching, "data/field/bg/rail_pal.c"),
            Object(NonMatching, "data/item/item_name.c"),
            Object(NonMatching, "data/model/mural/obj_mural.c"),
            Object(NonMatching, "data/npc/default_list.c"),
            Object(NonMatching, "data/npc/grow_list.c"),
            Object(NonMatching, "data/npc/house_list.c"),
            Object(NonMatching, "data/npc/model/mdl/ant_1.c"),
            Object(NonMatching, "data/npc/model/mdl/bea_1.c"),
            Object(NonMatching, "data/npc/model/mdl/bev_1.c"),
            Object(NonMatching, "data/npc/model/mdl/boa_1.c"),
            Object(NonMatching, "data/npc/model/mdl/bpt_1.c"),
            Object(NonMatching, "data/npc/model/mdl/brd_1.c"),
            Object(NonMatching, "data/npc/model/mdl/bul_1.c"),
            Object(NonMatching, "data/npc/model/mdl/cat_1.c"),
            Object(NonMatching, "data/npc/model/mdl/cbr_1.c"),
            Object(NonMatching, "data/npc/model/mdl/chn_1.c"),
            Object(NonMatching, "data/npc/model/mdl/cml_1.c"),
            Object(NonMatching, "data/npc/model/mdl/cow_1.c"),
            Object(NonMatching, "data/npc/model/mdl/crd_1.c"),
            Object(NonMatching, "data/npc/model/mdl/dog_1.c"),
            Object(NonMatching, "data/npc/model/mdl/duk_1.c"),
            Object(NonMatching, "data/npc/model/mdl/elp_1.c"),
            Object(NonMatching, "data/npc/model/mdl/end_1.c"),
            Object(NonMatching, "data/npc/model/mdl/flg_1.c"),
            Object(NonMatching, "data/npc/model/mdl/fob_1.c"),
            Object(NonMatching, "data/npc/model/mdl/fox_1.c"),
            Object(NonMatching, "data/npc/model/mdl/goa_1.c"),
            Object(NonMatching, "data/npc/model/mdl/gor_1.c"),
            Object(NonMatching, "data/npc/model/mdl/grf_1.c"),
            Object(NonMatching, "data/npc/model/mdl/gst_1.c"),
            Object(NonMatching, "data/npc/model/mdl/hem_1.c"),
            Object(NonMatching, "data/npc/model/mdl/hgh_1.c"),
            Object(NonMatching, "data/npc/model/mdl/hgs_1.c"),
            Object(NonMatching, "data/npc/model/mdl/hip_1.c"),
            Object(NonMatching, "data/npc/model/mdl/hrs_1.c"),
            Object(NonMatching, "data/npc/model/mdl/kab_1.c"),
            Object(NonMatching, "data/npc/model/mdl/kal_1.c"),
            Object(NonMatching, "data/npc/model/mdl/kgr_1.c"),
            Object(NonMatching, "data/npc/model/mdl/lon_1.c"),
            Object(NonMatching, "data/npc/model/mdl/lrc_1.c"),
            Object(NonMatching, "data/npc/model/mdl/mka_1.c"),
            Object(NonMatching, "data/npc/model/mdl/mnk_1.c"),
            Object(NonMatching, "data/npc/model/mdl/mob_1.c"),
            Object(NonMatching, "data/npc/model/mdl/mol_1.c"),
            Object(NonMatching, "data/npc/model/mdl/mos_1.c"),
            Object(NonMatching, "data/npc/model/mdl/mus_1.c"),
            Object(NonMatching, "data/npc/model/mdl/oct_1.c"),
            Object(NonMatching, "data/npc/model/mdl/ost_1.c"),
            Object(NonMatching, "data/npc/model/mdl/owl_1.c"),
            Object(NonMatching, "data/npc/model/mdl/pbr_1.c"),
            Object(NonMatching, "data/npc/model/mdl/pga_1.c"),
            Object(NonMatching, "data/npc/model/mdl/pgb_1.c"),
            Object(NonMatching, "data/npc/model/mdl/pgn_1.c"),
            Object(NonMatching, "data/npc/model/mdl/pig_1.c"),
            Object(NonMatching, "data/npc/model/mdl/pkn_1.c"),
            Object(NonMatching, "data/npc/model/mdl/pla_1.c"),
            Object(NonMatching, "data/npc/model/mdl/plb_1.c"),
            Object(NonMatching, "data/npc/model/mdl/plc_1.c"),
            Object(NonMatching, "data/npc/model/mdl/plj_1.c"),
            Object(NonMatching, "data/npc/model/mdl/rbt_1.c"),
            Object(NonMatching, "data/npc/model/mdl/rcc_1.c"),
            Object(NonMatching, "data/npc/model/mdl/rcd_1.c"),
            Object(NonMatching, "data/npc/model/mdl/rcf_1.c"),
            Object(NonMatching, "data/npc/model/mdl/rcn_1.c"),
            Object(NonMatching, "data/npc/model/mdl/rcs_1.c"),
            Object(NonMatching, "data/npc/model/mdl/rhn_1.c"),
            Object(NonMatching, "data/npc/model/mdl/seg_1.c"),
            Object(NonMatching, "data/npc/model/mdl/shp_1.c"),
            Object(NonMatching, "data/npc/model/mdl/snt_1.c"),
            Object(NonMatching, "data/npc/model/mdl/squ_1.c"),
            Object(NonMatching, "data/npc/model/mdl/tig_1.c"),
            Object(NonMatching, "data/npc/model/mdl/ttl_1.c"),
            Object(NonMatching, "data/npc/model/mdl/tuk_1.c"),
            Object(NonMatching, "data/npc/model/mdl/wip_1.c"),
            Object(NonMatching, "data/npc/model/mdl/wls_1.c"),
            Object(NonMatching, "data/npc/model/mdl/wol_1.c"),
            Object(NonMatching, "data/npc/model/mdl/xct_1.c"),
            Object(NonMatching, "data/npc/model/mdl/xsq_1.c"),
            Object(NonMatching, "data/npc/model/tex/ant_1.c"),
            Object(NonMatching, "data/npc/model/tex/ant_2.c"),
            Object(NonMatching, "data/npc/model/tex/ant_3.c"),
            Object(NonMatching, "data/npc/model/tex/ant_4.c"),
            Object(NonMatching, "data/npc/model/tex/ant_5.c"),
            Object(NonMatching, "data/npc/model/tex/bea_1.c"),
            Object(NonMatching, "data/npc/model/tex/bea_10.c"),
            Object(NonMatching, "data/npc/model/tex/bea_2.c"),
            Object(NonMatching, "data/npc/model/tex/bea_3.c"),
            Object(NonMatching, "data/npc/model/tex/bea_5.c"),
            Object(NonMatching, "data/npc/model/tex/bea_6.c"),
            Object(NonMatching, "data/npc/model/tex/bea_7.c"),
            Object(NonMatching, "data/npc/model/tex/bea_8.c"),
            Object(NonMatching, "data/npc/model/tex/bea_9.c"),
            Object(NonMatching, "data/npc/model/tex/bev_1.c"),
            Object(NonMatching, "data/npc/model/tex/boa_1.c"),
            Object(NonMatching, "data/npc/model/tex/bpt_1.c"),
            Object(NonMatching, "data/npc/model/tex/brd_1.c"),
            Object(NonMatching, "data/npc/model/tex/brd_10.c"),
            Object(NonMatching, "data/npc/model/tex/brd_11.c"),
            Object(NonMatching, "data/npc/model/tex/brd_2.c"),
            Object(NonMatching, "data/npc/model/tex/brd_3.c"),
            Object(NonMatching, "data/npc/model/tex/brd_4.c"),
            Object(NonMatching, "data/npc/model/tex/brd_5.c"),
            Object(NonMatching, "data/npc/model/tex/brd_6.c"),
            Object(NonMatching, "data/npc/model/tex/brd_7.c"),
            Object(NonMatching, "data/npc/model/tex/brd_8.c"),
            Object(NonMatching, "data/npc/model/tex/brd_9.c"),
            Object(NonMatching, "data/npc/model/tex/bul_1.c"),
            Object(NonMatching, "data/npc/model/tex/bul_2.c"),
            Object(NonMatching, "data/npc/model/tex/bul_3.c"),
            Object(NonMatching, "data/npc/model/tex/bul_4.c"),
            Object(NonMatching, "data/npc/model/tex/cat_1.c"),
            Object(NonMatching, "data/npc/model/tex/cat_10.c"),
            Object(NonMatching, "data/npc/model/tex/cat_11.c"),
            Object(NonMatching, "data/npc/model/tex/cat_12.c"),
            Object(NonMatching, "data/npc/model/tex/cat_13.c"),
            Object(NonMatching, "data/npc/model/tex/cat_14.c"),
            Object(NonMatching, "data/npc/model/tex/cat_15.c"),
            Object(NonMatching, "data/npc/model/tex/cat_2.c"),
            Object(NonMatching, "data/npc/model/tex/cat_3.c"),
            Object(NonMatching, "data/npc/model/tex/cat_4.c"),
            Object(NonMatching, "data/npc/model/tex/cat_5.c"),
            Object(NonMatching, "data/npc/model/tex/cat_6.c"),
            Object(NonMatching, "data/npc/model/tex/cat_7.c"),
            Object(NonMatching, "data/npc/model/tex/cat_8.c"),
            Object(NonMatching, "data/npc/model/tex/cat_9.c"),
            Object(NonMatching, "data/npc/model/tex/cbr_1.c"),
            Object(NonMatching, "data/npc/model/tex/cbr_10.c"),
            Object(NonMatching, "data/npc/model/tex/cbr_11.c"),
            Object(NonMatching, "data/npc/model/tex/cbr_2.c"),
            Object(NonMatching, "data/npc/model/tex/cbr_3.c"),
            Object(NonMatching, "data/npc/model/tex/cbr_4.c"),
            Object(NonMatching, "data/npc/model/tex/cbr_5.c"),
            Object(NonMatching, "data/npc/model/tex/cbr_6.c"),
            Object(NonMatching, "data/npc/model/tex/cbr_7.c"),
            Object(NonMatching, "data/npc/model/tex/cbr_8.c"),
            Object(NonMatching, "data/npc/model/tex/cbr_9.c"),
            Object(NonMatching, "data/npc/model/tex/chn_1.c"),
            Object(NonMatching, "data/npc/model/tex/chn_2.c"),
            Object(NonMatching, "data/npc/model/tex/chn_3.c"),
            Object(NonMatching, "data/npc/model/tex/chn_4.c"),
            Object(NonMatching, "data/npc/model/tex/chn_5.c"),
            Object(NonMatching, "data/npc/model/tex/chn_6.c"),
            Object(NonMatching, "data/npc/model/tex/chn_7.c"),
            Object(NonMatching, "data/npc/model/tex/chn_8.c"),
            Object(NonMatching, "data/npc/model/tex/chn_9.c"),
            Object(NonMatching, "data/npc/model/tex/cml_1.c"),
            Object(NonMatching, "data/npc/model/tex/cow_1.c"),
            Object(NonMatching, "data/npc/model/tex/cow_2.c"),
            Object(NonMatching, "data/npc/model/tex/cow_3.c"),
            Object(NonMatching, "data/npc/model/tex/cow_4.c"),
            Object(NonMatching, "data/npc/model/tex/crd_1.c"),
            Object(NonMatching, "data/npc/model/tex/crd_2.c"),
            Object(NonMatching, "data/npc/model/tex/crd_3.c"),
            Object(NonMatching, "data/npc/model/tex/crd_4.c"),
            Object(NonMatching, "data/npc/model/tex/dog_1.c"),
            Object(NonMatching, "data/npc/model/tex/dog_10.c"),
            Object(NonMatching, "data/npc/model/tex/dog_2.c"),
            Object(NonMatching, "data/npc/model/tex/dog_3.c"),
            Object(NonMatching, "data/npc/model/tex/dog_4.c"),
            Object(NonMatching, "data/npc/model/tex/dog_5.c"),
            Object(NonMatching, "data/npc/model/tex/dog_6.c"),
            Object(NonMatching, "data/npc/model/tex/dog_7.c"),
            Object(NonMatching, "data/npc/model/tex/dog_8.c"),
            Object(NonMatching, "data/npc/model/tex/dog_9.c"),
            Object(NonMatching, "data/npc/model/tex/duk_1.c"),
            Object(NonMatching, "data/npc/model/tex/duk_10.c"),
            Object(NonMatching, "data/npc/model/tex/duk_11.c"),
            Object(NonMatching, "data/npc/model/tex/duk_2.c"),
            Object(NonMatching, "data/npc/model/tex/duk_3.c"),
            Object(NonMatching, "data/npc/model/tex/duk_4.c"),
            Object(NonMatching, "data/npc/model/tex/duk_5.c"),
            Object(NonMatching, "data/npc/model/tex/duk_6.c"),
            Object(NonMatching, "data/npc/model/tex/duk_7.c"),
            Object(NonMatching, "data/npc/model/tex/duk_8.c"),
            Object(NonMatching, "data/npc/model/tex/duk_9.c"),
            Object(NonMatching, "data/npc/model/tex/elp_1.c"),
            Object(NonMatching, "data/npc/model/tex/elp_2.c"),
            Object(NonMatching, "data/npc/model/tex/elp_3.c"),
            Object(NonMatching, "data/npc/model/tex/elp_4.c"),
            Object(NonMatching, "data/npc/model/tex/elp_5.c"),
            Object(NonMatching, "data/npc/model/tex/elp_6.c"),
            Object(NonMatching, "data/npc/model/tex/elp_7.c"),
            Object(NonMatching, "data/npc/model/tex/end_1.c"),
            Object(NonMatching, "data/npc/model/tex/flg_1.c"),
            Object(NonMatching, "data/npc/model/tex/flg_10.c"),
            Object(NonMatching, "data/npc/model/tex/flg_11.c"),
            Object(NonMatching, "data/npc/model/tex/flg_12.c"),
            Object(NonMatching, "data/npc/model/tex/flg_13.c"),
            Object(NonMatching, "data/npc/model/tex/flg_2.c"),
            Object(NonMatching, "data/npc/model/tex/flg_3.c"),
            Object(NonMatching, "data/npc/model/tex/flg_4.c"),
            Object(NonMatching, "data/npc/model/tex/flg_5.c"),
            Object(NonMatching, "data/npc/model/tex/flg_6.c"),
            Object(NonMatching, "data/npc/model/tex/flg_7.c"),
            Object(NonMatching, "data/npc/model/tex/flg_8.c"),
            Object(NonMatching, "data/npc/model/tex/flg_9.c"),
            Object(NonMatching, "data/npc/model/tex/fob_1.c"),
            Object(NonMatching, "data/npc/model/tex/fox_1.c"),
            Object(NonMatching, "data/npc/model/tex/goa_1.c"),
            Object(NonMatching, "data/npc/model/tex/goa_2.c"),
            Object(NonMatching, "data/npc/model/tex/goa_3.c"),
            Object(NonMatching, "data/npc/model/tex/goa_4.c"),
            Object(NonMatching, "data/npc/model/tex/goa_5.c"),
            Object(NonMatching, "data/npc/model/tex/goa_6.c"),
            Object(NonMatching, "data/npc/model/tex/gor_1.c"),
            Object(NonMatching, "data/npc/model/tex/gor_2.c"),
            Object(NonMatching, "data/npc/model/tex/gor_3.c"),
            Object(NonMatching, "data/npc/model/tex/gor_4.c"),
            Object(NonMatching, "data/npc/model/tex/gor_5.c"),
            Object(NonMatching, "data/npc/model/tex/grf_1.c"),
            Object(NonMatching, "data/npc/model/tex/gst_1.c"),
            Object(NonMatching, "data/npc/model/tex/hem_1.c"),
            Object(NonMatching, "data/npc/model/tex/hgh_1.c"),
            Object(NonMatching, "data/npc/model/tex/hgs_1.c"),
            Object(NonMatching, "data/npc/model/tex/hip_1.c"),
            Object(NonMatching, "data/npc/model/tex/hip_2.c"),
            Object(NonMatching, "data/npc/model/tex/hip_3.c"),
            Object(NonMatching, "data/npc/model/tex/hip_4.c"),
            Object(NonMatching, "data/npc/model/tex/hip_5.c"),
            Object(NonMatching, "data/npc/model/tex/hip_6.c"),
            Object(NonMatching, "data/npc/model/tex/hip_7.c"),
            Object(NonMatching, "data/npc/model/tex/hrs_1.c"),
            Object(NonMatching, "data/npc/model/tex/hrs_2.c"),
            Object(NonMatching, "data/npc/model/tex/hrs_3.c"),
            Object(NonMatching, "data/npc/model/tex/hrs_4.c"),
            Object(NonMatching, "data/npc/model/tex/hrs_5.c"),
            Object(NonMatching, "data/npc/model/tex/hrs_6.c"),
            Object(NonMatching, "data/npc/model/tex/hrs_7.c"),
            Object(NonMatching, "data/npc/model/tex/hrs_8.c"),
            Object(NonMatching, "data/npc/model/tex/kab_1.c"),
            Object(NonMatching, "data/npc/model/tex/kal_1.c"),
            Object(NonMatching, "data/npc/model/tex/kal_2.c"),
            Object(NonMatching, "data/npc/model/tex/kal_3.c"),
            Object(NonMatching, "data/npc/model/tex/kal_4.c"),
            Object(NonMatching, "data/npc/model/tex/kal_5.c"),
            Object(NonMatching, "data/npc/model/tex/kal_6.c"),
            Object(NonMatching, "data/npc/model/tex/kgr_1.c"),
            Object(NonMatching, "data/npc/model/tex/kgr_2.c"),
            Object(NonMatching, "data/npc/model/tex/kgr_3.c"),
            Object(NonMatching, "data/npc/model/tex/kgr_4.c"),
            Object(NonMatching, "data/npc/model/tex/kgr_5.c"),
            Object(NonMatching, "data/npc/model/tex/kgr_6.c"),
            Object(NonMatching, "data/npc/model/tex/lon_1.c"),
            Object(NonMatching, "data/npc/model/tex/lon_2.c"),
            Object(NonMatching, "data/npc/model/tex/lon_3.c"),
            Object(NonMatching, "data/npc/model/tex/lon_4.c"),
            Object(NonMatching, "data/npc/model/tex/lrc_1.c"),
            Object(NonMatching, "data/npc/model/tex/mka_1.c"),
            Object(NonMatching, "data/npc/model/tex/mnk_1.c"),
            Object(NonMatching, "data/npc/model/tex/mob_1.c"),
            Object(NonMatching, "data/npc/model/tex/mol_1.c"),
            Object(NonMatching, "data/npc/model/tex/mos_1.c"),
            Object(NonMatching, "data/npc/model/tex/mus_1.c"),
            Object(NonMatching, "data/npc/model/tex/mus_10.c"),
            Object(NonMatching, "data/npc/model/tex/mus_2.c"),
            Object(NonMatching, "data/npc/model/tex/mus_3.c"),
            Object(NonMatching, "data/npc/model/tex/mus_4.c"),
            Object(NonMatching, "data/npc/model/tex/mus_5.c"),
            Object(NonMatching, "data/npc/model/tex/mus_6.c"),
            Object(NonMatching, "data/npc/model/tex/mus_7.c"),
            Object(NonMatching, "data/npc/model/tex/mus_8.c"),
            Object(NonMatching, "data/npc/model/tex/mus_9.c"),
            Object(NonMatching, "data/npc/model/tex/oct_1.c"),
            Object(NonMatching, "data/npc/model/tex/ost_1.c"),
            Object(NonMatching, "data/npc/model/tex/ost_2.c"),
            Object(NonMatching, "data/npc/model/tex/ost_3.c"),
            Object(NonMatching, "data/npc/model/tex/ost_4.c"),
            Object(NonMatching, "data/npc/model/tex/owl_1.c"),
            Object(NonMatching, "data/npc/model/tex/pbr_1.c"),
            Object(NonMatching, "data/npc/model/tex/pbr_2.c"),
            Object(NonMatching, "data/npc/model/tex/pbr_3.c"),
            Object(NonMatching, "data/npc/model/tex/pbr_4.c"),
            Object(NonMatching, "data/npc/model/tex/pbr_5.c"),
            Object(NonMatching, "data/npc/model/tex/pga_1.c"),
            Object(NonMatching, "data/npc/model/tex/pgb_1.c"),
            Object(NonMatching, "data/npc/model/tex/pgn_1.c"),
            Object(NonMatching, "data/npc/model/tex/pgn_2.c"),
            Object(NonMatching, "data/npc/model/tex/pgn_3.c"),
            Object(NonMatching, "data/npc/model/tex/pgn_4.c"),
            Object(NonMatching, "data/npc/model/tex/pgn_5.c"),
            Object(NonMatching, "data/npc/model/tex/pgn_6.c"),
            Object(NonMatching, "data/npc/model/tex/pgn_7.c"),
            Object(NonMatching, "data/npc/model/tex/pgn_8.c"),
            Object(NonMatching, "data/npc/model/tex/pig_1.c"),
            Object(NonMatching, "data/npc/model/tex/pig_10.c"),
            Object(NonMatching, "data/npc/model/tex/pig_11.c"),
            Object(NonMatching, "data/npc/model/tex/pig_2.c"),
            Object(NonMatching, "data/npc/model/tex/pig_3.c"),
            Object(NonMatching, "data/npc/model/tex/pig_4.c"),
            Object(NonMatching, "data/npc/model/tex/pig_5.c"),
            Object(NonMatching, "data/npc/model/tex/pig_6.c"),
            Object(NonMatching, "data/npc/model/tex/pig_7.c"),
            Object(NonMatching, "data/npc/model/tex/pig_8.c"),
            Object(NonMatching, "data/npc/model/tex/pig_9.c"),
            Object(NonMatching, "data/npc/model/tex/pkn_1.c"),
            Object(NonMatching, "data/npc/model/tex/pla_1.c"),
            Object(NonMatching, "data/npc/model/tex/plb_1.c"),
            Object(NonMatching, "data/npc/model/tex/plc_1.c"),
            Object(NonMatching, "data/npc/model/tex/plj_1.c"),
            Object(NonMatching, "data/npc/model/tex/rbt_1.c"),
            Object(NonMatching, "data/npc/model/tex/rbt_10.c"),
            Object(NonMatching, "data/npc/model/tex/rbt_11.c"),
            Object(NonMatching, "data/npc/model/tex/rbt_2.c"),
            Object(NonMatching, "data/npc/model/tex/rbt_3.c"),
            Object(NonMatching, "data/npc/model/tex/rbt_4.c"),
            Object(NonMatching, "data/npc/model/tex/rbt_5.c"),
            Object(NonMatching, "data/npc/model/tex/rbt_6.c"),
            Object(NonMatching, "data/npc/model/tex/rbt_7.c"),
            Object(NonMatching, "data/npc/model/tex/rbt_8.c"),
            Object(NonMatching, "data/npc/model/tex/rbt_9.c"),
            Object(NonMatching, "data/npc/model/tex/rcc_1.c"),
            Object(NonMatching, "data/npc/model/tex/rcd_1.c"),
            Object(NonMatching, "data/npc/model/tex/rcf_1.c"),
            Object(NonMatching, "data/npc/model/tex/rcn_1.c"),
            Object(NonMatching, "data/npc/model/tex/rcs_1.c"),
            Object(NonMatching, "data/npc/model/tex/rhn_1.c"),
            Object(NonMatching, "data/npc/model/tex/rhn_2.c"),
            Object(NonMatching, "data/npc/model/tex/rhn_3.c"),
            Object(NonMatching, "data/npc/model/tex/rhn_4.c"),
            Object(NonMatching, "data/npc/model/tex/seg_1.c"),
            Object(NonMatching, "data/npc/model/tex/shp_1.c"),
            Object(NonMatching, "data/npc/model/tex/shp_2.c"),
            Object(NonMatching, "data/npc/model/tex/shp_3.c"),
            Object(NonMatching, "data/npc/model/tex/shp_4.c"),
            Object(NonMatching, "data/npc/model/tex/shp_5.c"),
            Object(NonMatching, "data/npc/model/tex/shp_6.c"),
            Object(NonMatching, "data/npc/model/tex/snt_1.c"),
            Object(NonMatching, "data/npc/model/tex/squ_1.c"),
            Object(NonMatching, "data/npc/model/tex/squ_10.c"),
            Object(NonMatching, "data/npc/model/tex/squ_11.c"),
            Object(NonMatching, "data/npc/model/tex/squ_2.c"),
            Object(NonMatching, "data/npc/model/tex/squ_3.c"),
            Object(NonMatching, "data/npc/model/tex/squ_4.c"),
            Object(NonMatching, "data/npc/model/tex/squ_5.c"),
            Object(NonMatching, "data/npc/model/tex/squ_6.c"),
            Object(NonMatching, "data/npc/model/tex/squ_7.c"),
            Object(NonMatching, "data/npc/model/tex/squ_8.c"),
            Object(NonMatching, "data/npc/model/tex/squ_9.c"),
            Object(NonMatching, "data/npc/model/tex/tig_1.c"),
            Object(NonMatching, "data/npc/model/tex/tig_2.c"),
            Object(NonMatching, "data/npc/model/tex/tig_3.c"),
            Object(NonMatching, "data/npc/model/tex/tig_4.c"),
            Object(NonMatching, "data/npc/model/tex/ttl_1.c"),
            Object(NonMatching, "data/npc/model/tex/tuk_1.c"),
            Object(NonMatching, "data/npc/model/tex/wip_1.c"),
            Object(NonMatching, "data/npc/model/tex/wls_1.c"),
            Object(NonMatching, "data/npc/model/tex/wol_1.c"),
            Object(NonMatching, "data/npc/model/tex/wol_2.c"),
            Object(NonMatching, "data/npc/model/tex/wol_3.c"),
            Object(NonMatching, "data/npc/model/tex/wol_4.c"),
            Object(NonMatching, "data/npc/model/tex/wol_5.c"),
            Object(NonMatching, "data/npc/model/tex/wol_6.c"),
            Object(NonMatching, "data/npc/model/tex/xct_1.c"),
            Object(NonMatching, "data/npc/model/tex/xsq_1.c"),
            Object(NonMatching, "data/npc/npc_draw_data.c"),
            Object(NonMatching, "data/submenu/map/kan_tizu.c"),
            Object(NonMatching, "data/submenu/map/kan_tizu2.c"),
            Object(NonMatching, "data/titledemo/pact0.c"),
            Object(NonMatching, "data/titledemo/pact1.c"),
            Object(NonMatching, "data/titledemo/pact2.c"),
            Object(NonMatching, "data/titledemo/pact3.c"),
            Object(NonMatching, "data/titledemo/pact4.c"),
        ],
    ),
    Rel(
        "actor",
        [
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
            Object(Matching, "actor/ac_goza.c"),
            Object(Matching, "actor/ac_go_home_npc.c"),
            Object(Matching, "actor/ac_groundhog_control.c"),
            Object(Matching, "actor/ac_handOverItem.c"),
            Object(Matching, "actor/ac_haniwa.c"),
            Object(Matching, "actor/ac_insect.c"),
            Object(Matching, "actor/ac_ins_amenbo.c"),
            Object(Matching, "actor/ac_ins_batta.c"),
            Object(Matching, "actor/ac_ins_chou.c"),
            Object(Matching, "actor/ac_ins_dango.c"),
            Object(Matching, "actor/ac_ins_goki.c"),
            Object(Matching, "actor/ac_ins_hitodama.c"),
            Object(Matching, "actor/ac_ins_ka.c"),
            Object(Matching, "actor/ac_ins_kabuto.c"),
            Object(Matching, "actor/ac_intro_demo.c"),
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
            Object(Matching, "actor/ac_npc_guide.c"),
            Object(Matching, "actor/ac_npc_guide2.c"),
            Object(Matching, "actor/ac_npc_rcn_guide.c"),
            Object(Matching, "actor/ac_npc_rcn_guide2.c"),
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
            Object(Matching, "actor/ac_sample.c"),
            Object(Matching, "actor/ac_set_manager.c"),
            Object(Matching, "actor/ac_set_ovl_gyoei.c"),
            Object(Matching, "actor/ac_set_ovl_insect.c"),
            Object(Matching, "actor/ac_shop.c"),
            Object(Matching, "actor/ac_shop_design.c"),
            Object(Matching, "actor/ac_shrine.c"),
            Object(Matching, "actor/ac_structure.c"),
            Object(Matching, "actor/ac_s_car.c"),
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
    ),
    Rel(
        "actor_tool",
        [
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
    ),
    Rel(
        "effect",
        [
            Object(Matching, "effect/ef_ami_mizu.c"),
            Object(Matching, "effect/ef_anahikari.c"),
            Object(Matching, "effect/ef_ase.c"),
            Object(Matching, "effect/ef_ase2.c"),
            Object(Matching, "effect/ef_ase_ch.c"),
            Object(Matching, "effect/ef_break_axe.c"),
            Object(Matching, "effect/ef_bubu.c"),
            Object(Matching, "effect/ef_buruburu.c"),
            Object(Matching, "effect/ef_bush_happa.c"),
            Object(Matching, "effect/ef_bush_yuki.c"),
            Object(Matching, "effect/ef_car_blight.c"),
            Object(Matching, "effect/ef_car_light.c"),
            Object(Matching, "effect/ef_clacker.c"),
            Object(Matching, "effect/ef_coin.c"),
            Object(Matching, "effect/ef_dash_asimoto.c"),
            Object(Matching, "effect/ef_dig_hole.c"),
            Object(Matching, "effect/ef_dig_mud.c"),
            Object(Matching, "effect/ef_dig_scoop.c"),
            Object(Matching, "effect/ef_douzou_light.c"),
            Object(Matching, "effect/ef_doyon.c"),
            Object(NonMatching, "effect/ef_dust.c"),
            Object(Matching, "effect/ef_effect_control.c"),
            Object(Matching, "effect/ef_flash.c"),
            Object(Matching, "effect/ef_footprint.c"),
            Object(NonMatching, "effect/ef_furo_yuge.c"),
            Object(NonMatching, "effect/ef_gimonhu.c"),
            Object(NonMatching, "effect/ef_goki.c"),
            Object(NonMatching, "effect/ef_ha.c"),
            Object(Matching, "effect/ef_halloween.c"),
            Object(NonMatching, "effect/ef_halloween_smoke.c"),
            Object(NonMatching, "effect/ef_hanabira.c"),
            Object(NonMatching, "effect/ef_hanabi_botan1.c"),
            Object(NonMatching, "effect/ef_hanabi_botan2.c"),
            Object(Matching, "effect/ef_hanabi_dummy.c"),
            Object(NonMatching, "effect/ef_hanabi_hoshi.c"),
            Object(NonMatching, "effect/ef_hanabi_set.c"),
            Object(NonMatching, "effect/ef_hanabi_switch.c"),
            Object(NonMatching, "effect/ef_hanabi_yanagi.c"),
            Object(Matching, "effect/ef_hanatiri.c"),
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
    ),
    Rel(
        "game",
        [
            Object(Matching, "game/m_actor.c"),
            Object(Matching, "game/m_actor_dlftbls.c"),
            Object(Matching, "game/m_actor_shadow.c"),
            Object(Matching, "game/m_all_grow.c"),
            Object(Matching, "game/m_all_grow_ovl.c"),
            Object(Matching, "game/m_bank_ovl.c"),
            Object(Matching, "game/m_banti.c"),
            Object(Matching, "game/m_bgm.c"),
            Object(Matching, "game/m_bg_item.c"),
            Object(Matching, "game/m_bg_tex.c"),
            Object(Matching, "game/m_birthday_ovl.c"),
            Object(NonMatching, "game/m_board_ovl.c"),
            Object(Matching, "game/m_calendar.c"),
            Object(Matching, "game/m_camera2.c"),
            Object(NonMatching, "game/m_card.c"),
            Object(Matching, "game/m_catalog_ovl.c"),
            Object(Matching, "game/m_choice.c"),
            Object(Matching, "game/m_clip.c"),
            Object(Matching, "game/m_cockroach.c"),
            Object(NonMatching, "game/m_collision_bg.c"),
            Object(Matching, "game/m_collision_obj.c"),
            Object(Matching, "game/m_common_data.c"),
            Object(Matching, "game/m_controller.c"),
            Object(Matching, "game/m_cpak.c"),
            Object(Matching, "game/m_debug.c"),
            Object(Matching, "game/m_debug_display.c"),
            Object(Matching, "game/m_debug_hayakawa.c"),
            Object(Matching, "game/m_debug_mode.c"),
            Object(Matching, "game/m_demo.c"),
            Object(Matching, "game/m_eappli.c"),
            Object(Matching, "game/m_editEndChk_ovl.c"),
            Object(Matching, "game/m_editor_ovl.c"),
            Object(NonMatching, "game/m_event.c"),
            Object(Matching, "game/m_fbdemo.c"),
            Object(Matching, "game/m_fbdemo_fade.c"),
            Object(Matching, "game/m_fbdemo_triforce.c"),
            Object(Matching, "game/m_fbdemo_wipe1.c"),
            Object(Matching, "game/m_field_assessment.c"),
            Object(Matching, "game/m_field_info.c"),
            Object(Matching, "game/m_field_make.c"),
            Object(Matching, "game/m_fishrecord.c"),
            Object(Matching, "game/m_flashrom.c"),
            Object(Matching, "game/m_font.c"),
            Object(Matching, "game/m_fuusen.c"),
            Object(Matching, "game/m_game_dlftbls.c"),
            Object(NonMatching, "game/m_handbill.c"),
            Object(Matching, "game/m_hand_ovl.c"),
            Object(Matching, "game/m_haniwaPortrait_ovl.c"),
            Object(Matching, "game/m_hboard_ovl.c"),
            Object(Matching, "game/m_home.c"),
            Object(Matching, "game/m_house.c"),
            Object(Matching, "game/m_huusui_room.c"),
            Object(Matching, "game/m_huusui_room_ovl.c"),
            Object(Matching, "game/m_inventory_ovl.c"),
            Object(Matching, "game/m_island.c"),
            Object(Matching, "game/m_item_name.c"),
            Object(Matching, "game/m_kabu_manager.c"),
            Object(Matching, "game/m_kankyo.c"),
            Object(Matching, "game/m_land.c"),
            Object(Matching, "game/m_lib.c"),
            Object(Matching, "game/m_lights.c"),
            Object(Matching, "game/m_mail.c"),
            Object(Matching, "game/m_mail_check.c"),
            Object(Matching, "game/m_mail_check_ovl.c"),
            Object(Matching, "game/m_mail_password_check.c"),
            Object(Matching, "game/m_malloc.c"),
            Object(Matching, "game/m_map_ovl.c"),
            Object(Matching, "game/m_mark_room.c"),
            Object(Matching, "game/m_mark_room_ovl.c"),
            Object(Matching, "game/m_melody.c"),
            Object(Matching, "game/m_msg.c"),
            Object(Matching, "game/m_museum.c"),
            Object(Matching, "game/m_museum_display.c"),
            Object(Matching, "game/m_mushroom.c"),
            Object(NonMatching, "game/m_music_ovl.c"),
            Object(Matching, "game/m_name_table.c"),
            Object(Matching, "game/m_needlework.c"),
            Object(NonMatching, "game/m_notice.c"),
            Object(Matching, "game/m_notice_ovl.c"),
            Object(Matching, "game/m_npc.c"),
            Object(Matching, "game/m_npc_schedule.c"),
            Object(Matching, "game/m_npc_walk.c"),
            Object(Matching, "game/m_olib.c"),
            Object(Matching, "game/m_passwordChk_ovl.c"),
            Object(Matching, "game/m_passwordMake_ovl.c"),
            Object(Matching, "game/m_pause.c"),
            Object(Matching, "game/m_play.c"),
            Object(Matching, "game/m_player_call.c"),
            Object(Matching, "game/m_police_box.c"),
            Object(Matching, "game/m_post_office.c"),
            Object(Matching, "game/m_prenmi.c"),
            Object(Matching, "game/m_private.c"),
            Object(Matching, "game/m_quest.c"),
            Object(Matching, "game/m_random_field.c"),
            Object(Matching, "game/m_random_field_ovl.c"),
            Object(Matching, "game/m_rcp.c"),
            Object(Matching, "game/m_repay_ovl.c"),
            Object(Matching, "game/m_roll_lib.c"),
            Object(Matching, "game/m_room_type.c"),
            Object(Matching, "game/m_scene.c"),
            Object(Matching, "game/m_scene_ftr.c"),
            Object(Matching, "game/m_select.c"),
            Object(NonMatching, "game/m_shop.c"),
            Object(Matching, "game/m_skin_matrix.c"),
            Object(Matching, "game/m_snowman.c"),
            Object(Matching, "game/m_soncho.c"),
            Object(Matching, "game/m_start_data_init.c"),
            Object(Matching, "game/m_string.c"),
            Object(Matching, "game/m_submenu.c"),
            Object(Matching, "game/m_submenu_ovl.c"),
            Object(Matching, "game/m_time.c"),
            Object(Matching, "game/m_timeIn_ovl.c"),
            Object(NonMatching, "game/m_titledemo.c"),
            Object(Matching, "game/m_trademark.c"),
            Object(NonMatching, "game/m_train_control.c"),
            Object(Matching, "game/m_vibctl.c"),
            Object(Matching, "game/m_view.c"),
            Object(Matching, "game/m_warning_ovl.c"),
            Object(NonMatching, "game/m_watch_my_step.c"),
        ],
    ),
    Rel(
        "system",
        [
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
    ),
]

# Define our custom asset processing scripts
config.custom_build_rules = [
    {
        "name": "convert_pal16",
        "command": "$python tools/converters/pal16dis.py $in $out",
        "description": "CONVERT $palette",
    },
    {
        "name": "convert_pal16_chunked",
        "command": "$python tools/converters/pal16dis_chunked.py $in $out",
        "description": "CONVERT $palette",
    },
    {
        "name": "convert_vtx",
        "command": "$python tools/converters/vtxdis.py $in $out",
        "description": "CONVERT $vtxdata",
    },
]
config.custom_build_steps = {}

# Grab the specific GameID so we can format our strings properly
version = VERSIONS[version_num]


# This generates the build steps needed for preprocessing
def emit_build_rule(asset):
    match asset["type"]:
        case "pal16":
            symbol = asset["symbol"]
            binary_path = asset["binary"]
            include_path = binary_path.replace(".bin", ".inc")

            config.custom_build_steps.setdefault("pre-compile", []).append(
                {
                    "rule": "convert_pal16",
                    "inputs": f"build/{version}/bin/{binary_path}",
                    "outputs": f"build/{version}/include/{include_path}",
                    "variables": {
                        "palette": f"{symbol}",
                    },
                }
            )

        case "pal16c":
            symbol = asset["symbol"]
            binary_path = asset["binary"]
            include_path = binary_path.replace(".bin", ".inc")

            config.custom_build_steps.setdefault("pre-compile", []).append(
                {
                    "rule": "convert_pal16_chunked",
                    "inputs": f"build/{version}/bin/{binary_path}",
                    "outputs": f"build/{version}/include/{include_path}",
                    "variables": {
                        "palette": f"{symbol}",
                    },
                }
            )

        case "vtx":
            symbol = asset["symbol"]
            binary_path = asset["binary"]
            include_path = binary_path.replace(".bin", ".inc")

            config.custom_build_steps.setdefault("pre-compile", []).append(
                {
                    "rule": "convert_vtx",
                    "inputs": f"build/{version}/bin/{binary_path}",
                    "outputs": f"build/{version}/include/{include_path}",
                    "variables": {
                        "vtxdata": f"{symbol}",
                    },
                }
            )

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

# TODO: make this more robust? right now, the download is all or none
N64_SDK_urls = [
    "https://raw.githubusercontent.com/decompals/ultralib/main/include/PR/abi.h",
    "https://raw.githubusercontent.com/decompals/ultralib/main/include/PR/gbi.h",
    "https://raw.githubusercontent.com/decompals/ultralib/main/include/PR/gs2dex.h",
    "https://raw.githubusercontent.com/decompals/ultralib/main/include/PR/mbi.h",
    "https://raw.githubusercontent.com/decompals/ultralib/main/include/PR/ultratypes.h",
    "https://raw.githubusercontent.com/decompals/ultralib/main/include/gcc/stdlib.h",
]

# If we don't have the N64 headers downloaded, we need to grab them
if not os.path.exists("include/PR") or not os.path.exists("include/gcc"):
    if not os.path.exists("include/PR"):
        os.mkdir("include/PR")
    if not os.path.exists("include/gcc"):
        os.mkdir("include/gcc")
    for url in N64_SDK_urls:
        filename = "/".join(url.split("/")[-2:])
        response = requests.get(url)
        if response.status_code == 200:
            content = response.content

            if filename == "PR/gbi.h":
                content = re.sub(
                    rb"unsigned char\s+param:8;", b"unsigned int\tparam:8;", content
                )

            with open(f"include/{filename}", "wb") as file:
                file.write(content)

        else:
            print(f"Failed to download N64 header {filename}")

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress and write progress.json
    config.progress_each_module = args.verbose
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
