#include "includes.h"

vu16  g_TIM_PWM_Ticks =  750 ; 


// 占空比  750us - 1950us

/**
  * @brief  基本定时器初始化，用于非阻塞延时用.
  * @param  None
  * @retval None
  */
void Timer2_PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/* 定义 TIM_OCInit 初始化结构体 TIM_OCInitStructure */
	TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
    //    GPIO_AFIODeInit();
    //    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);  //add 0917
    //    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);     //add 0917

    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 20000;   //
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OCInitStructure.TIM_Pulse = g_TIM_PWM_Ticks;	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  

//    TIM_OC1PreloadConfig(TIM2 , TIM_OCPreload_Enable);  //Notice   下次预装载到来时更新
    TIM_OC1PreloadConfig(TIM2 , TIM_OCPreload_Disable);  // 立即     TIM2->CCR1 = g_TIM_PWM_Ticks ;
    
	TIM_ARRPreloadConfig(TIM2, ENABLE);

    TIM_Cmd(TIM2, ENABLE);
}



