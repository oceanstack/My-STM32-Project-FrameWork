#ifndef __OLED242_
#define __OLED242_
#include "stm32f10x.h"

//OLED控制用函数
void OLED_Init(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Clear(void);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 

#endif

