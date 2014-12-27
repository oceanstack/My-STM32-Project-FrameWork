#ifndef __DELAY_H
#define __DELAY_H
//#include "sys.h"
#include "stm32f10x.h"



typedef enum
{
    TIMER_RESET = 0,
    TIMER_SET = 1,
} TimerStatus;

typedef struct
{
    vu8 flag;
    vu32 ticks ;
} Hinder_Delay_Struct;


void SysTick_Init(void);
void Delay_us(vu32 uTime);
void Delay_ms(vu32 mTime);


#endif /*__DELAY_H*/

