#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"

struct spinlock rtc_lock;

uint32
rtc_read_low(void) {
  return *(volatile uint32*)RTC_LOW;
}

uint32
rtc_read_high(void) {
  return *(volatile uint32*)RTC_HIGH;
}

uint64
rtc_get_time(void) {
  acquire(&rtc_lock);
  uint32 lo = rtc_read_low();
  uint32 hi = rtc_read_high();
  release(&rtc_lock);
  return ((uint64)hi << 32) | lo;
}

void
rtcinit(void) {
  initlock(&rtc_lock, "rtc");
}
