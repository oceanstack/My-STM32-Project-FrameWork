#include "includes.h"

vu32 SysTimerTicks = 0;


static void Timer_Loop_Tim7_Init(void)
{

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //时钟使能
    TIM_TimeBaseStructure.TIM_Period = 499;   // 5 ms 中断一次
    TIM_TimeBaseStructure.TIM_Prescaler = 719;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM7, TIM_FLAG_Update);
    //
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM7, ENABLE);

}

void Timer_LoopDelay_Init(void)
{
    Timer_Loop_Tim7_Init();

}

void Sys_Ticks1S_LoopTest(void)
{
    static vu32 ticks = 0 ; 
    if (SysTimerTicks >= (ticks + LoopDelay_1S))
    {
        Debug_Module_Printf(SYS_, " Tick \n");

        ticks = SysTimerTicks ;
    }
}


void TIM7_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
    {
        SysTimerTicks ++ ;
        TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);
    }
}

