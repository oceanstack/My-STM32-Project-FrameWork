#ifndef __GregorianCalendar_H
#define __GregorianCalendar_H

#include "stm32f10x.h"

typedef struct rtc_time
{
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;  // month day
    int tm_mon;   // month
    int tm_year;
    int tm_wday;  //week  day 
} rtc_time_t;

void GregorianDay(rtc_time_t *tm);
uint32_t mktimev(rtc_time_t *tm);
void to_tm(uint32_t tim,  rtc_time_t *tm);

#endif
