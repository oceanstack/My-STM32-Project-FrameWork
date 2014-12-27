#ifndef __WDG_H
#define __WDG_H

#include "stm32f10x.h"

//extern u8 WWDG_CNT;

void IWDG_Init(u8 prer, u16 rlr);
void IWDG_Feed(void);

void WWDG_Init(u8 tr, u8 wr, u32 fprer);
void WWDG_Feed_Process(void);
//void WWDG_Feed(void);

#endif

