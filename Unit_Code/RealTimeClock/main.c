#include "includes.h"

int main(void)
{
    SysTick_Init();
    NVIC_Configuration();
    USART1_Debug_Init(115200);
    
	Debug_Module_Printf(SYS," SYS Start \n");
	
	Debug_Module_Printf(SYS," Build %s\n " , __DATE__);

    RTC_CheckAndConfig(&RTC_BJTime);

    Time_Show(&RTC_BJTime);	
}

