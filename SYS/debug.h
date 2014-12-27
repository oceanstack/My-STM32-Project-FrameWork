#ifndef __DEBUG_H
#define __DEBUG_H

#include "includes.h"

#define SYS_DEBUG
//#define WDG_DEBUG

//-------
#ifdef  SYS_DEBUG
#define SYS   (1<<0)
#else
#define SYS    0
#endif
//-------
#ifdef  WDG_DEBUG
#define WDG   (1<<1)
#else
#define WDG    0
#endif

//-------
#define DEBUG_CNT  2
//-------

#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收


void USART1_Debug_Init(u32 bound);
void Debug_Module_Printf(int module, char *fmt, ...);

#endif
