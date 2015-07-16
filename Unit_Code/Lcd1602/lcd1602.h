/****************************************Copyright (c)****************************************************
**---------------------------------------File Info-------------------------------------------------------
** File Name           : :    lcd1602.h
** Created By          : :    Zhu ZhiCheng(朱治丞).
** Created date        : :	  
** Last modified Date  : :	  
** Last Version        : :    V1.0
** Description         : :	  V1.0 
**                      
***********************************************************************************************************/


#ifndef __LCD1602_H
#define __LCD1602_H

#include "includes.h"


#define LCD1602_RS0  do{PCout(7) = 0 ;} while(0) 
#define LCD1602_RW0  do{PCout(8) = 0 ;} while(0) 
#define LCD1602_EN0  do{PCout(9) = 0 ;} while(0) //低电平  PE.2

#define LCD1602_RS1  do{PCout(7) = 1 ;} while(0) //高电平  PE.0
#define LCD1602_RW1  do{PCout(8) = 1 ;} while(0) 
#define LCD1602_EN1  do{PCout(9) = 1 ;} while(0)

#define DATA_OUT     (GPIOD->ODR)   //PD7~PD0


//void LCD_FBUF(void);
//void Write_LCD1602_Command(u8 Com);
//void Write_LCD1602_Date(u8 Date);
void LCD1602_Init(void);
void LCD1602_Clear(void);
void Display_One_Char(u8 x , u8 y , u8 DData);
void Display_One_Line( u8 Line , u8 *DData);

#endif

