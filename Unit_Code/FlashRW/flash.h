#ifndef __FLASH_H
#define __FLASH_H

#define STM32_FLASH_BASE_ADDR ((uint32_t)(0x08000000))  

#define CH_DATA_SAVE_PAGE ((uint32_t)(0x0807F800))   // page 255

typedef enum {FLASH_SUCCESS = 0, FLASH_ERROR = !FLASH_SUCCESS} Flash_Status;



u8 Flash_DataPageWrite(u32 addr, u16 *pData , u16 size);
u16 Flash_ReadHalfWord(u32 faddr);

void Flash_Test(void);

#endif /*__FLASH_H*/
