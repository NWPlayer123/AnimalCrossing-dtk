#include "types.h"
#include "dolphin/os.h"
#include "dolphin/PPCArch.h"
#include "dolphin/os/__ppc_eabi_init.h"
#ifdef __cplusplus
extern "C" {
#endif

static void __init_cpp(void);

__declspec(section ".init") asm void __init_hardware(void) {
    nofralloc mfmsr r0 ori r0, r0, 0x2000 mtmsr r0 mflr r31 bl __OSPSInit bl __OSCacheInit mtlr r31 blr
}

__declspec(section ".init") asm void __flush_cache(void*, size_t) {
    nofralloc lis r5, 0xFFFFFFF1 @h ori r5, r5, 0xFFFFFFF1 @l and r5, r5, r3 subf r3, r5, r3 add r4, r4,
        r3 loop : dcbst 0, r5 sync icbi 0, r5 addic r5, r5, 8 addic.r4, r4, -8 bge loop isync blr
}

void __init_user(void) {
    __init_cpp();
}

static void __init_cpp(void) {
    voidfunctionptr* constructor;
    /*
     *	call static initializers
     */
    for (constructor = _ctors; *constructor; constructor++) {
        (*constructor)();
    }
}

void __fini_cpp(void) {
    // UNUSED FUNCTION
}

void _ExitProcess(void) {
    PPCHalt();
}
#ifdef __cplusplus
}
#endif
