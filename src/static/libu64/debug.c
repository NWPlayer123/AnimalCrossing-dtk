#include "libu64/debug.h"
#include "libultra/osThread.h"
#include "libc64/sprintf.h"

extern void _dbg_hungup(const char* file, int line) {
  char dbg_tbuf[0x100];
    
  sprintf(dbg_tbuf, "*** HungUp in thread %d, [%s:%d] ***\n", osGetThreadId(0), file, line);
}
