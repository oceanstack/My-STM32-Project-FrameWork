#ifndef  __REALTIME_CLOCK_H
#define  __REALTIME_CLOCK_H

#include "includes.h"

extern vu8 TimeSecTick ;

extern rtc_time_t RTC_BJTime ;

void RTC_CheckAndConfig(rtc_time_t *tm) ;

void Time_Show(rtc_time_t  *tm) ;

#endif
