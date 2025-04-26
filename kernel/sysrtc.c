#include "types.h"
#include "riscv.h"
#include "defs.h"

uint64
sys_getrtc(void) {
  return rtc_get_time();
}