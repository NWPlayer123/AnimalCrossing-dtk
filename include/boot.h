#ifndef BOOT_H
#define BOOT_H

#include "types.h"
#include "dolphin/os/OSModule.h"
#include "dolphin/os/OSTime.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct boot_tbl_s {
  const char* msg;
  u16 param_upper;
  u16 param_lower;
} boot_tbl_t;

extern u32 convert_partial_address(u32 partial_addr);
extern void HotResetIplMenu();

extern void* boot_copyDate;
extern void* HotStartEntry;
extern OSTime InitialStartTime;
extern u8 boot_sound_initializing;

typedef void(*HotStartProc)();

#define NMISaveArea (void*)0x811FFFC0
OSModuleHeader* BaseModule AT_ADDRESS(0x800030C8);

// TODO: this macro should probably go elsewhere
#define MAKE_PARTIAL_ADDRESS(segment, ofs) \
  (((segment) << 28) + (ofs & 0x01FFFFFF))

#ifdef __cplusplus
}
#endif

#endif
