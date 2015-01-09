#include "includes.h"


/**
  * @brief  unlock and check erase (only one page)  
  * @param  None
  * @retval FLASH_Status
  */
FLASH_Status Flash_PageCheckStart(u32 WriteAddr,u16 HalfNum)
{
    FLASH_Status Status = FLASH_COMPLETE;
    u8 i ;
    u32 AddrInc = WriteAddr;

    FLASH_Unlock();

    for(i=0; i < HalfNum; i++)
	{
		if(0XFFFF != *(vu16*)AddrInc)break;  // need to erase
		AddrInc+=2;
	}

    if(i < HalfNum ){
        Status= FLASH_ErasePage(WriteAddr);
    }

    return Status ;
}




/**
  * @brief  lock
  * @param  None
  * @retval None
  */
void Flash_PageWriteDone(void)
{
    FLASH_Lock();
}



/**
  * @brief  write some data to one flash page
  * @param  
  * @retval
  */
u8 Flash_DataPageWrite(u32 addr, u16 *pData , u16 size)
{
    vu16 *pDataTemp = (vu16 *)pData;
    vu32 temp = addr;

    if(FLASH_COMPLETE != Flash_PageCheckStart(addr ,size)){
        return FLASH_ERROR;
    }

    for (; temp < (addr + size * 2); pDataTemp++, temp += 2)
    {
        FLASH_ProgramHalfWord(temp, *pDataTemp);
        if (*pDataTemp != *(vu16 *)temp)
        {
            return FLASH_ERROR;
        }
    }
    
    Flash_PageWriteDone();

    return FLASH_SUCCESS;
}


u16 Flash_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}

#define TEST_LEN 30
void Flash_Test(void)
{
    u8 i ; 
    u16 Buf[TEST_LEN];
    u32 addr = CH_DATA_SAVE_PAGE;
    for(i = 0 ; i < TEST_LEN ; i ++){
         Buf[i] = 200 + i;
         printf(" %d: %d ",i ,Buf[i]);
    }
    printf("\n");

    
    for(i = 0 ; i < TEST_LEN ; i ++){
         printf(" %d : %d ",i ,Flash_ReadHalfWord(addr));
         addr += 2;
    }
    printf("\n");

    
    if(FLASH_SUCCESS == Flash_DataPageWrite(CH_DATA_SAVE_PAGE, Buf ,TEST_LEN)){
        printf(" \n write success \n");
        addr = CH_DATA_SAVE_PAGE;
        for(i = 0 ; i < TEST_LEN ; i ++){
             printf("%d : %d ",i ,Flash_ReadHalfWord(addr));
             addr += 2;
        }
    }

       
    if(FLASH_SUCCESS == Flash_DataPageWrite(CH_DATA_SAVE_PAGE + 16, Buf ,TEST_LEN)){
        printf(" \n write success \n");
        addr = CH_DATA_SAVE_PAGE + 16;
        for(i = 0 ; i < TEST_LEN ; i ++){
             printf("%d : %d ",i ,Flash_ReadHalfWord(addr));
             addr += 2;
        }
    }
    
    addr = CH_DATA_SAVE_PAGE;
    for(i = 0 ; i < TEST_LEN ; i ++){
         printf(" %d : %d ",i ,Flash_ReadHalfWord(addr));
         addr += 2;
    }
    printf("\n");

}


