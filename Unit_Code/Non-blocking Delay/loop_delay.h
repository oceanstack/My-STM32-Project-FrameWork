#ifndef __LOOP_DELAY_H
#define __LOOP_DELAY_H


#define LoopDelay_5MS  1  
#define LoopDelay_10MS  2  
#define LoopDelay_20MS  4  
#define LoopDelay_30MS  6  
#define LoopDelay_40MS  8 
#define LoopDelay_50MS  10
#define LoopDelay_60MS  12 
#define LoopDelay_80MS  16 
#define LoopDelay_100MS  20 
#define LoopDelay_200MS  40 
#define LoopDelay_300MS  60 
#define LoopDelay_500MS  100 
#define LoopDelay_1S  200 
#define LoopDelay_2S  400 
#define LoopDelay_3S  600 

extern vu32 SysTimerTicks ;

void Timer_LoopDelay_Init(void);
void Sys_Ticks1S_LoopTest(void);

#endif /*__LOOP_DELAY_H*/

