#ifndef __IO_SPI_H
#define __IO_SPI_H 

#define N_NOPS  5
#define ASM_NOPS(N)  {u32 i = N; do{__nop();}while(i--);}

#define   SCK  PBout(13)
#define   MOSI  PBout(15)
#define   MISO  PBin(14)

void AD_SPI_Init(void);
u8 AD_SPI_Read_Data(void); //
void AD_SPI_Send_Data(unsigned char byte);

#endif 

