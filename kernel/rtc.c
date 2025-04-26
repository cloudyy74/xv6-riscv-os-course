#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "memlayout.h"

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
  uint32 lo = rtc_read_low();
  uint32 hi = rtc_read_high();
  return ((uint64)hi << 32) | lo;
}