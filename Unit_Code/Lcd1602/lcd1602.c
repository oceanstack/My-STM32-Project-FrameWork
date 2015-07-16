#include "includes.h"

/******************************************************************************
 **Copyright (c), 

 **Name    : :  void LCD1602_GPIO_Init(void)
 **Function: :  LCD1602控制输出端口的初始化
 **Input   : :  None
 **Return  : :  None

 **Author  : :  
 **Date    : :         
 *******************************************************************************/
void LCD1602_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOC,ENABLE);

    GPIO_InitStructure.GPIO_Pin  =GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //推挽输出50MHZ
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin  =GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);

}



/******************************************************************************
 **Copyright (c), 

 **Name    : :  void LCD_FBUF(void)
 **Function: :  LCD1602液晶读忙状态
 **Input   : :  None
 **Return  : :  None

 **Author  : : 
 **Date    : :        
 *******************************************************************************/
static void LCD_FBUF(void)
{
    LCD1602_RS0; //RS信号拉低

    LCD1602_RW1; //RW信号拉高

    LCD1602_EN1;//EN信号拉高

    //	while((DATA_OUT & 0x80) == 0x80);//判断数据端口PB7 是否一直高
    Delay_ms(1);

    LCD1602_RW1;//RW信号拉高

    LCD1602_EN0;//EN信号拉低
}



/******************************************************************************
 **Copyright (c),

 **Name    : :  void Write_LCD1602_Command(u8 Com)
 **Function: :  LCD1602写指令子函数
 **Input   : :  Com
 **Return  : :  None

 **Author  : : 
 **Date    : :        
 *******************************************************************************/
static void Write_LCD1602_Command(u8 Com)
{	
    LCD_FBUF();                       //读忙状态

    LCD1602_RS0;                      //RS信号拉低写指令

    LCD1602_RW0;                      //RW信号拉低

    LCD1602_EN0;                      //EN信号拉低

    Delay_us(1);                      //延时1us

    LCD1602_EN1;                      //EN信号一个高脉冲

    DATA_OUT = Com;                   //数据端口指令输入

    Delay_us(1);                      //延时1us

    LCD1602_EN0;
}



/******************************************************************************
 **Copyright (c). All rights reserved

 **Name    : :  void Write_LCD1602_Date(u8 Data)
 **Function: :  LCD1602写数据子函数
 **Input   : :  Data
 **Return  : :  None

 **Author  : :  
 **Date    : :        
 *******************************************************************************/
static void Write_LCD1602_Date(u8 Data)
{
    LCD_FBUF();                 //读忙状态  

    LCD1602_RS1;                //RS信号拉高写数据指令

    LCD1602_RW0; 

    LCD1602_EN0;                //EN信号拉低

    Delay_us(1);                      //延时1us

    LCD1602_EN1;                      //EN信号一个高脉冲

    DATA_OUT = Data;                   //数据端口指令输入

    Delay_us(1);                      //延时1us

    LCD1602_EN0;
}



/******************************************************************************
 **Copyright (c), 

 **Name    : :  void LCD1602_Init(void)
 **Function: :  LCD1602液晶显示控制初始化
 **Input   : :  None
 **Return  : :  None

 **Author  : : 
 **Date    : :       
 *******************************************************************************/
void LCD1602_Init(void)
{
    LCD1602_GPIO_Init();

    Delay_ms(10);

    Write_LCD1602_Command(0x38);
    Delay_ms(5);

    Write_LCD1602_Command(0x38);
    Delay_ms(5);

    Write_LCD1602_Command(0x38);
    Delay_ms(5);

    LCD_FBUF();
    Write_LCD1602_Command(0x38);

    LCD_FBUF();
    Write_LCD1602_Command(0x08);   //显示模式设置，关显示，不显示光标

    LCD_FBUF();
    Write_LCD1602_Command(0x06); //光标移动方式 ，指针加1 光标加1 

    LCD_FBUF();
    Write_LCD1602_Command(0x0c);  //显示开

    LCD_FBUF();
    Write_LCD1602_Command(0x01);  //显示清屏

}


void LCD1602_Clear(void)
{

    LCD_FBUF();
    Write_LCD1602_Command(0x01);  //显示清屏
    Delay_ms(1);
}

/******************************************************************************
 **Copyright (c), 

 **Name    : :  void Display_One_Char(u8 x , u8 y , u8 DData)
 **Function: :  LCD1602显示一个字符控制函数
 **Input   : :  x,y,DData(位置及要显示的数据)
 **Return  : :  None

 **Author  : : 
 **Date    : :       
 *******************************************************************************/
void Display_One_Char(u8 x , u8 y , u8 DData)
{						
    x&=0xF;						
    if(y)
        x|=0x40;					
    x|=0x80;			
    Write_LCD1602_Command(x);		
    Write_LCD1602_Date(DData);

}

/******************************************************************************
 **Copyright (c),  All rights reserved

 **Name    : :  void Display_One_Line( u8 Line , u8 *DData)
 **Function: :  LCD1602显示一整行字符子函数
 **Input   : :  Line， *DData 
 **Return  : :  None

 **Author  : : oceanstack
 **Date    : :         
 *******************************************************************************/
void Display_One_Line( u8 Line , u8 *DData)
{
    u8 i=0;
    for(i=0; i<16; i++)  
    {
        Display_One_Char(i, Line, *(DData+i));	
    }		
}


