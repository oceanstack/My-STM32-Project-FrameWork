#include "includes.h"

/******************************************************************************
 **Copyright (c), 

 **Name    : :  void LCD1602_GPIO_Init(void)
 **Function: :  LCD1602��������˿ڵĳ�ʼ��
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
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //�������50MHZ
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin  =GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);

}



/******************************************************************************
 **Copyright (c), 

 **Name    : :  void LCD_FBUF(void)
 **Function: :  LCD1602Һ����æ״̬
 **Input   : :  None
 **Return  : :  None

 **Author  : : 
 **Date    : :        
 *******************************************************************************/
static void LCD_FBUF(void)
{
    LCD1602_RS0; //RS�ź�����

    LCD1602_RW1; //RW�ź�����

    LCD1602_EN1;//EN�ź�����

    //	while((DATA_OUT & 0x80) == 0x80);//�ж����ݶ˿�PB7 �Ƿ�һֱ��
    Delay_ms(1);

    LCD1602_RW1;//RW�ź�����

    LCD1602_EN0;//EN�ź�����
}



/******************************************************************************
 **Copyright (c),

 **Name    : :  void Write_LCD1602_Command(u8 Com)
 **Function: :  LCD1602дָ���Ӻ���
 **Input   : :  Com
 **Return  : :  None

 **Author  : : 
 **Date    : :        
 *******************************************************************************/
static void Write_LCD1602_Command(u8 Com)
{	
    LCD_FBUF();                       //��æ״̬

    LCD1602_RS0;                      //RS�ź�����дָ��

    LCD1602_RW0;                      //RW�ź�����

    LCD1602_EN0;                      //EN�ź�����

    Delay_us(1);                      //��ʱ1us

    LCD1602_EN1;                      //EN�ź�һ��������

    DATA_OUT = Com;                   //���ݶ˿�ָ������

    Delay_us(1);                      //��ʱ1us

    LCD1602_EN0;
}



/******************************************************************************
 **Copyright (c). All rights reserved

 **Name    : :  void Write_LCD1602_Date(u8 Data)
 **Function: :  LCD1602д�����Ӻ���
 **Input   : :  Data
 **Return  : :  None

 **Author  : :  
 **Date    : :        
 *******************************************************************************/
static void Write_LCD1602_Date(u8 Data)
{
    LCD_FBUF();                 //��æ״̬  

    LCD1602_RS1;                //RS�ź�����д����ָ��

    LCD1602_RW0; 

    LCD1602_EN0;                //EN�ź�����

    Delay_us(1);                      //��ʱ1us

    LCD1602_EN1;                      //EN�ź�һ��������

    DATA_OUT = Data;                   //���ݶ˿�ָ������

    Delay_us(1);                      //��ʱ1us

    LCD1602_EN0;
}



/******************************************************************************
 **Copyright (c), 

 **Name    : :  void LCD1602_Init(void)
 **Function: :  LCD1602Һ����ʾ���Ƴ�ʼ��
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
    Write_LCD1602_Command(0x08);   //��ʾģʽ���ã�����ʾ������ʾ���

    LCD_FBUF();
    Write_LCD1602_Command(0x06); //����ƶ���ʽ ��ָ���1 ����1 

    LCD_FBUF();
    Write_LCD1602_Command(0x0c);  //��ʾ��

    LCD_FBUF();
    Write_LCD1602_Command(0x01);  //��ʾ����

}


void LCD1602_Clear(void)
{

    LCD_FBUF();
    Write_LCD1602_Command(0x01);  //��ʾ����
    Delay_ms(1);
}

/******************************************************************************
 **Copyright (c), 

 **Name    : :  void Display_One_Char(u8 x , u8 y , u8 DData)
 **Function: :  LCD1602��ʾһ���ַ����ƺ���
 **Input   : :  x,y,DData(λ�ü�Ҫ��ʾ������)
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
 **Function: :  LCD1602��ʾһ�����ַ��Ӻ���
 **Input   : :  Line�� *DData 
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


