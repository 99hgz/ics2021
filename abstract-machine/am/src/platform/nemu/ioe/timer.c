#include <am.h>
#include <nemu.h>
#include <stdio.h>
static uint64_t uptime_t = 0;

void __am_timer_init() {
  uptime_t = ((uint64_t)inl(0xa0000048)<<32)|inl(0xa0000048);   
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  static uint64_t tmp_uptime_t = 0;
  tmp_uptime_t = ((uint64_t)inl(0xa000004c)<<32)|inl(0xa0000048);   
  uptime->us = tmp_uptime_t - uptime_t;
  printf("%llu\n",tmp_uptime_t);
} 

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
   