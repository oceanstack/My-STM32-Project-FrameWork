#include "includes.h"


int main(void)
{
    SysTick_Init();
    NVIC_Configuration();
    USART1_Debug_Init(115200);

    
	Debug_Module_Printf(SYS," SYS Start \n");
	  
    while (1)
    {
		Debug_Module_Printf(SYS," Tick \n");
		Delay_ms(1000);

    }
}

