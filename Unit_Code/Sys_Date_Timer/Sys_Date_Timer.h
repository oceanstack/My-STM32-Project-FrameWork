#ifndef __SYS_DATE_TIMER_H
#define __SYS_DATE_TIMER_H 

/*
 *README :
 * Second minute hour .. ticks .
 * It can be used simulate a clock  or timer count ..
 *
 * */

typedef struct
{
	u8 Month;
	u8 Day;
	u8 Hour;
	u8 Minute;
	u8 Second;	
	u16 Year;
} Sys_Date_Time_Ticks_Struct,*Sys_Date_Time_Ticks_Struct_p;

extern Sys_Date_Time_Ticks_Struct Sys_Time ;

void SysTime_Ticks_Init(void);
void SysTime_Ticks_IRQ(void);

#endif

