#include "io_spi.h"

// IO Ä£Äâ SPI 0117 ²âÊÔ
void AD_SPI_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA , ENABLE);

    // SPI for AD  pins:SCK,   MOSI
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // SPI for AD  pins: MISO
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}



u8 AD_SPI_Read_Data(void)  //2015 0118
{
    unsigned char  n =8; 
    unsigned char tdata = 0;      

    MOSI = 0;
    while(n--)
    {
        SCK=0;

        if(MISO==1)
            tdata=tdata|0x01;
        else
            tdata=tdata&0xFE;
        tdata=tdata<<1;
        SCK=1;
        ASM_NOPS(N_NOPS);

    }    
    return tdata;
}


void AD_SPI_Send_Data(unsigned char byte)
{
    unsigned char  n =8;         // 8bit
    SCK = 1 ;                     

    while(n--)
    {
        ASM_NOPS(N_NOPS);
        SCK = 0 ; 

        if((byte&0x80) == 0x80)        
        {
            MOSI = 1;                
        }
        else
        {
            MOSI = 0;                
        }   

        ASM_NOPS(N_NOPS);
        SCK = 1 ;
        byte = byte<<1;   
    }
}

