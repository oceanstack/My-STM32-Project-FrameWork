#include "includes.h"

uint8_t const *WEEK_STR[] = {"日", "一", "二", "三", "四", "五", "六"};

vu8 TimeSecTick = 0;
rtc_time_t RTC_BJTime ;

static void RTC_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);     //
}


static void RTC_Configuration(void)
{
    u8 overtime  = 0 ;
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);
    /* Reset Backup Domain */
    BKP_DeInit();
    /* Enable LSE */
    RCC_LSEConfig(RCC_LSE_ON);
    
    /* Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {
        if (++overtime >= 250)
        {
            return ;
        }
        
        Delay_ms(10) ;
    }
    
    /* Select LSE as RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    /* Enable RTC Clock */
    RCC_RTCCLKCmd(ENABLE);
    /* Wait for RTC registers synchronization
     * 因为RTC时钟是低速的，内环时钟是高速的，所以要同步
     */
    RTC_WaitForSynchro();
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Set RTC prescaler: set RTC period to 1sec */
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}


static void Time_Adjust(rtc_time_t  *tm)
{
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    tm->tm_year = 2015;
    tm->tm_mon = 3;
    tm->tm_mday = 2;
    tm->tm_hour = 6;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    /* Get wday */
    GregorianDay(tm);
    /*config the   RTC cnt */
    RTC_SetCounter(mktimev(tm));
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}


void RTC_CheckAndConfig(rtc_time_t *tm)
{
    RTC_NVIC_Config();
    
    if (BKP_ReadBackupRegister(BKP_DR1) != 0x5AA5)
    {
        /* RTC Configuration */
        RTC_Configuration();
        /* Adjust time  */
        Time_Adjust(tm);
        BKP_WriteBackupRegister(BKP_DR1, 0x5AA5);
    }
    else
    {
        RTC_WaitForSynchro();
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();
    }
}


void Time_Show(rtc_time_t  *tm)
{
    uint32_t BJ_TimeVar;
    
    /* Infinite loop */
    while (1)
    {
        /* tick 1s */
        if (1 == TimeSecTick)
        {
            BJ_TimeVar = RTC_GetCounter() + 8 * 60 * 60;
            to_tm(BJ_TimeVar, tm);

            printf("当前时间为: %d年 %d月 %d日 (星期%s) %0.2d:%0.2d:%0.2d \n UNIX时间戳 = %d \r", 
                   tm->tm_year,  tm->tm_mon, tm->tm_mday,
                   WEEK_STR[tm->tm_wday], tm->tm_hour,
                   tm->tm_min, tm->tm_sec,BJ_TimeVar - 8 * 60 * 60);


            /* Display current time */
            TimeSecTick = 0;
        }
    }
}



void RTC_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
        /* Enable time update */
        TimeSecTick = 1;
    }
    
    if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
    {
        RTC_ClearITPendingBit(RTC_IT_ALR);
    }
    
    RTC_ClearITPendingBit(RTC_IT_SEC | RTC_IT_OW);
    RTC_WaitForLastTask();
}

