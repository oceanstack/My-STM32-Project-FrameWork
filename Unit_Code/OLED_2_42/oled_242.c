#include "includes.h"

#include "oledfont.h"  
//********************************************************************************

#define SIZE 16
//#define SIZE 8  //  0714


//OLEDģʽ����
//0:4�ߴ���ģʽ
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

//-----------------OLED�˿ڶ���----------------  					   


#define OLED_CS_Clr()  GPIO_ResetBits(GPIOD,GPIO_Pin_3)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOD,GPIO_Pin_3)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_4)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOD,GPIO_Pin_4)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_5)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOD,GPIO_Pin_5)

#define OLED_WR_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_14)
#define OLED_WR_Set() GPIO_SetBits(GPIOG,GPIO_Pin_1)

#define OLED_RD_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_13)
#define OLED_RD_Set() GPIO_SetBits(GPIOG,GPIO_Pin_13)


//ʹ��4�ߴ��нӿ�ʱʹ�� 
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_6)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOD,GPIO_Pin_6)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_7)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOD,GPIO_Pin_7)


//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 

//�ӿڶ������oled.h�ļ�
//����SPI��Ҫ���߸��ߣ�����RES���Բ��ӣ����ǲ���ʱ���������
// GND,VCC,CS,RES,D/C,CLK,DIN
//���ڵ�ѹ���⣻����ͻ���Ҫ��ʱ���ȶ�����������ȫ������3.3V��Դ����ƽ�ź�Ҳ����3.3V��
//����ͻ���Ҫ��5V��Դ����Ȼ5V����������������ʽ���ǲ�Ҫ��ʱ�乤�����������ܻ����������������VCC�˴�һС������ܻ��һЩ��


//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
    u8 i;			  
    if(cmd)
        OLED_DC_Set();
    else 
        OLED_DC_Clr();		  
    OLED_CS_Clr();
    for(i=0;i<8;i++)
    {			  
        OLED_SCLK_Clr();
        if(dat&0x80)
            OLED_SDIN_Set();
        else 
            OLED_SDIN_Clr();
        OLED_SCLK_Set();
        dat<<=1;   
    }				 		  
    OLED_CS_Set();
    OLED_DC_Set();   	  
} 
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
    OLED_WR_Byte(0xb0+y,OLED_CMD);
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
    OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
    OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
    OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
    OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
    u8 i,n;		    
    for(i=0;i<8;i++)  
    {  
        OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
        OLED_WR_Byte (0x02,OLED_CMD);      //������ʾλ�á��е͵�ַ
        OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
        for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
    } //������ʾ
}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{      	

    unsigned char c=0,i=0;	
    c=chr-' ';//�õ�ƫ�ƺ��ֵ			
    if(x>Max_Column-1){x=0;y=y+2;}
    if(SIZE ==16)
    {
        OLED_Set_Pos(x,y);	
        for(i=0;i<8;i++)
            OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
        OLED_Set_Pos(x,y+1);
        for(i=0;i<8;i++)
            OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
    }
    else {	
        OLED_Set_Pos(x,y+1);
        for(i=0;i<6;i++)
            OLED_WR_Byte(F6x8[c][i],OLED_DATA);

    }

}
//m^n����
u32 oled_pow(u8 m,u8 n)
{
    u32 result=1;	 
    while(n--)result*=m;    
    return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
    u8 t,temp;
    u8 enshow=0;						   
    for(t=0;t<len;t++)
    {
        temp=(num/oled_pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                OLED_ShowChar(x+(size/2)*t,y,' ');
                continue;
            }else enshow=1; 

        }
        OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
    }
} 
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
    unsigned char j=0;
    while (chr[j]!='\0')
    {		OLED_ShowChar(x,y,chr[j]);
        x+=8;
        if(x>120){x=0;y+=2;}
        j++;
    }
}


//��ʼ��SSD1309					    
void OLED_Init(void)
{ 	

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PC,D,G�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_3|GPIO_Pin_8;	 //PD3,PD6�������  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
    GPIO_Init(GPIOD, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
    GPIO_SetBits(GPIOD,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_3|GPIO_Pin_8);	//PD3,PD6 �����


    OLED_RST_Set();
    Delay_ms(100);
    OLED_RST_Clr();
    Delay_ms(100);
    OLED_RST_Set(); 
    /* 
       OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
       OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
       OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
       OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
       OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
       OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
       OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
       OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
       OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
       OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
       OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
       OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
       OLED_WR_Byte(0x00,OLED_CMD);//-not offset
       OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
       OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
       OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
       OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
       OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
       OLED_WR_Byte(0x12,OLED_CMD);
       OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
       OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
       OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
       OLED_WR_Byte(0x02,OLED_CMD);//
       OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
       OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
       OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
       OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
       OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel

    */

    OLED_WR_Byte(0xFD,OLED_CMD); //Command Lock
    OLED_WR_Byte(0x12,OLED_CMD); //		

    OLED_WR_Byte(0xAE,OLED_CMD); //Set Display Off 

    OLED_WR_Byte(0xD5,OLED_CMD); //Set Display Clock Divide Ratio/Oscillator Frequency
    OLED_WR_Byte(0xA0,OLED_CMD); //

    OLED_WR_Byte(0xA8,OLED_CMD); //Set Multiplex Ratio 
    OLED_WR_Byte(0x3F,OLED_CMD); // 

    OLED_WR_Byte(0xD3,OLED_CMD); //Set Display Offset
    OLED_WR_Byte(0X00,OLED_CMD); //

    OLED_WR_Byte(0x40,OLED_CMD); //Set Display Start Line 

    OLED_WR_Byte(0XA1,OLED_CMD); //Set Segment Re-Map

    OLED_WR_Byte(0xC8,OLED_CMD); //Set COM Output Scan Direction 

    OLED_WR_Byte(0xDA,OLED_CMD); //Set COM Pins Hardware Configuration
    OLED_WR_Byte(0x12,OLED_CMD); //

    OLED_WR_Byte(0x81,OLED_CMD); //Set Current Control 
    OLED_WR_Byte(0xDF,OLED_CMD); //

    OLED_WR_Byte(0xD9,OLED_CMD); //Set Pre-Charge Period  
    OLED_WR_Byte(0x82,OLED_CMD); //

    OLED_WR_Byte(0xDB,OLED_CMD); //Set VCOMH Deselect Level 	 
    OLED_WR_Byte(0x34,OLED_CMD); //

    OLED_WR_Byte(0xA4,OLED_CMD); //Set Entire Display On/Off

    OLED_WR_Byte(0xA6,OLED_CMD); //Set Normal/Inverse Display 


    OLED_WR_Byte(0xAF,OLED_CMD); //Set Display On 
    OLED_Clear();
    OLED_Set_Pos(0,0); 

}  



