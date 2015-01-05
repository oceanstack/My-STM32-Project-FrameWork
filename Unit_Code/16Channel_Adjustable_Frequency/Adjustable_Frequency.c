#include "includes.h"

CH_Frq_Out_Set_Struct CH_Data[CH_COUNT] ; 


void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void Tim1_PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    /* PWM*/
    /* TIM clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    /* GPIOA and GPIOB clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);     //add 0918
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 |  GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 65535;       //
    TIM_TimeBaseStructure.TIM_Prescaler = 35;       //  1us
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;    //
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    /* CH1 Mode configuration: */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;     //
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);     //
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);     //
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);     //
    //    TIM_ARRPreloadConfig(TIM1, ENABLE);            //
    TIM_OC1PreloadConfig(TIM1 , TIM_OCPreload_Disable);
    TIM_OC2PreloadConfig(TIM1 , TIM_OCPreload_Disable);
    TIM_OC3PreloadConfig(TIM1 , TIM_OCPreload_Disable);
    TIM_OC4PreloadConfig(TIM1 , TIM_OCPreload_Disable);
    TIM_ITConfig(TIM1 , TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 , ENABLE);
    /* TIM enable counter */
    TIM_Cmd(TIM1, ENABLE);                   //
    TIM_CtrlPWMOutputs(TIM1, ENABLE); //add 0918 required
}


void Tim2_PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    /* PWM*/
    /* TIM clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    /* GPIOA and GPIOB clock enable */
    //    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
    //    GPIO_AFIODeInit();
    //    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);  //add 0917
    //    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);     //add 0917
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ; //default changed 0918
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 65535;       //
    TIM_TimeBaseStructure.TIM_Prescaler = 35;       //  1us
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;    //
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    /* CH1 Mode configuration: */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;     //
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    //    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //
    //    TIM_OCInitStructure.TIM_Pulse = 500 ;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);     //
    //    TIM_OCInitStructure.TIM_Pulse = 500 ;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);     //
    //    TIM_OCInitStructure.TIM_Pulse = 250 ;
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);     //
    //    TIM_OCInitStructure.TIM_Pulse = 125 ;
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);     //
    //   TIM_ARRPreloadConfig(TIM2, ENABLE);            //
    TIM_OC1PreloadConfig(TIM2 , TIM_OCPreload_Disable);
    TIM_OC2PreloadConfig(TIM2 , TIM_OCPreload_Disable);
    TIM_OC3PreloadConfig(TIM2 , TIM_OCPreload_Disable);
    TIM_OC4PreloadConfig(TIM2 , TIM_OCPreload_Disable);
    TIM_ITConfig(TIM2 , TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 , ENABLE);
    /* TIM enable counter */
    TIM_Cmd(TIM2, ENABLE);                   //
}


void Tim3_PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    /* PWM*/
    /* TIM clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    /* clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);     //add 0918
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 65535;       //
    TIM_TimeBaseStructure.TIM_Prescaler = 35;       //  1us
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;    //
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    /* CH1 Mode configuration: */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;     //
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //
    //    TIM_OCInitStructure.TIM_Pulse = 750 ;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);     //
    //    TIM_OCInitStructure.TIM_Pulse = 500 ;
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);     //
    //    TIM_OCInitStructure.TIM_Pulse =  250;
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);     //
    //    TIM_OCInitStructure.TIM_Pulse = 125 ;
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);     //
    //    TIM_ARRPreloadConfig(TIM3, ENABLE);            //
    TIM_OC1PreloadConfig(TIM3 , TIM_OCPreload_Disable);
    TIM_OC2PreloadConfig(TIM3 , TIM_OCPreload_Disable);
    TIM_OC3PreloadConfig(TIM3 , TIM_OCPreload_Disable);
    TIM_OC4PreloadConfig(TIM3 , TIM_OCPreload_Disable);
    TIM_ITConfig(TIM3 , TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 , ENABLE);
    /* TIM enable counter */
    TIM_Cmd(TIM3, ENABLE);                       //
}


void Tim4_PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    /* PWM*/
    /* TIM clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    /* GPIOA and GPIOB clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 |  GPIO_Pin_14 | GPIO_Pin_15 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 65535;       
    TIM_TimeBaseStructure.TIM_Prescaler = 35;       //  1us
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    /* CH1 Mode configuration: */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;     
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);     //
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);     //
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);     //
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);     //
    //    TIM_ARRPreloadConfig(TIM4, ENABLE);            
    TIM_OC1PreloadConfig(TIM4 , TIM_OCPreload_Disable);
    TIM_OC2PreloadConfig(TIM4 , TIM_OCPreload_Disable);
    TIM_OC3PreloadConfig(TIM4 , TIM_OCPreload_Disable);
    TIM_OC4PreloadConfig(TIM4 , TIM_OCPreload_Disable);
    TIM_ITConfig(TIM4 , TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 , ENABLE);
    /* TIM enable counter */
    TIM_Cmd(TIM4, ENABLE);                   
}



void CH_Data_Freq_Set_Init(void)
{
    u8 i ;
    CH_Data[0].freq_v_buf = 0 ;     //NULL
    CH_Data[1].freq_v_buf = 1500 ;  //PA0
    CH_Data[2].freq_v_buf = 1500 ;  //PA1
    CH_Data[3].freq_v_buf = 1000 ;  //PA2
    CH_Data[4].freq_v_buf = 1500 ;  //PA3
    CH_Data[5].freq_v_buf = 1165 ;  //PC6
    CH_Data[6].freq_v_buf = 1500 ;  //PC7
    CH_Data[7].freq_v_buf = 1000 ;  //PC8
    CH_Data[8].freq_v_buf = 1000 ;  //PC9
    CH_Data[9].freq_v_buf  = 1000 ; //PD12
    CH_Data[10].freq_v_buf = 1000 ; //PD13
    CH_Data[11].freq_v_buf = 1000 ; //PD14
    CH_Data[12].freq_v_buf = 1000 ; //PD15
    CH_Data[13].freq_v_buf = 1000 ; //PE9
    CH_Data[14].freq_v_buf = 1000 ; //PE11
    CH_Data[15].freq_v_buf = 1000 ; //PE13
    CH_Data[16].freq_v_buf = 1000 ; //PE14
    
    for (i = 1; i < CH_COUNT ; i++)
    {
        CH_Data[i].freq_compare_set = CH_Pwm_Out_Freq_Convert(CH_Data[i].freq_v_buf);
    }
}

void CH_Frq_Out_Init(void)
{
    CH_Data_Freq_Set_Init();
    NVIC_Configuration();

    Tim1_PWM_Init();
    Tim2_PWM_Init();
    Tim3_PWM_Init();
    Tim4_PWM_Init();

}


void TIM2_IRQHandler(void)
{   
    u16	capture = 0;
    
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 );
       
        capture = TIM_GetCapture1(TIM2);
	    TIM_SetCompare1(TIM2, capture + CH_Data[1].freq_compare_set);      
		
	}

	if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
		capture = TIM_GetCapture2(TIM2);
		TIM_SetCompare2(TIM2, capture + CH_Data[2].freq_compare_set);
	}

	if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
		capture = TIM_GetCapture3(TIM2);
		TIM_SetCompare3(TIM2, capture + CH_Data[3].freq_compare_set);
	}

	if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
		capture = TIM_GetCapture4(TIM2);
		TIM_SetCompare4(TIM2, capture + CH_Data[4].freq_compare_set);
	}
}


void TIM3_IRQHandler(void)   //TIM3中断
{  
    u16	capture = 0;
    
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 );
       
        capture = TIM_GetCapture1(TIM3);
	    TIM_SetCompare1(TIM3, capture + CH_Data[5].freq_compare_set);      
		
	}

	if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		capture = TIM_GetCapture2(TIM3);
		TIM_SetCompare2(TIM3, capture + CH_Data[6].freq_compare_set);
	}

	if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
		capture = TIM_GetCapture3(TIM3);
		TIM_SetCompare3(TIM3, capture + CH_Data[7].freq_compare_set);
	}

	if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
		capture = TIM_GetCapture4(TIM3);
		TIM_SetCompare4(TIM3, capture + CH_Data[8].freq_compare_set);
	}
}



void TIM4_IRQHandler(void)
{
    u16	capture = 0;
    
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 );
       
        capture = TIM_GetCapture1(TIM4);
	    TIM_SetCompare1(TIM4, capture + CH_Data[9].freq_compare_set);      
		
	}

	if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
		capture = TIM_GetCapture2(TIM4);
		TIM_SetCompare2(TIM4, capture + CH_Data[10].freq_compare_set);
	}

	if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
		capture = TIM_GetCapture3(TIM4);
		TIM_SetCompare3(TIM4, capture + CH_Data[11].freq_compare_set);
	}

	if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
		capture = TIM_GetCapture4(TIM4);
		TIM_SetCompare4(TIM4, capture + CH_Data[12].freq_compare_set);
	}
}



void TIM1_CC_IRQHandler(void)
{
    u16	capture = 0;
    
	if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1 );
       
        capture = TIM_GetCapture1(TIM1);
	    TIM_SetCompare1(TIM1, capture + CH_Data[13].freq_compare_set);      
		
	}

	if (TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
		capture = TIM_GetCapture2(TIM1);
		TIM_SetCompare2(TIM1, capture + CH_Data[14].freq_compare_set);
	}

	if (TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
		capture = TIM_GetCapture3(TIM1);
		TIM_SetCompare3(TIM1, capture + CH_Data[15].freq_compare_set);
	}

	if (TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);
		capture = TIM_GetCapture4(TIM1);
		TIM_SetCompare4(TIM1, capture + CH_Data[16].freq_compare_set);
	}

}

