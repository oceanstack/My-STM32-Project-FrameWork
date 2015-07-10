#include "includes.h"


pwm_inupt_struct  pwm_f = {0.0 , 0, 0, 0, 0, 0};


/**
  * @brief  基本定时器初始化   //10 us
  * @param  None
  * @retval None
  */
static void Timer3_Tick_Init(void)
{
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
    TIM_TimeBaseInitStructure.TIM_Prescaler = 719;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 60000;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    //  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_Cmd(TIM3, ENABLE);
}



static void PWM_Capture_ExitIOConfig(void)
{
    EXTI_InitTypeDef  EXTI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;        //设定中断源为PA15
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    //使能IO复用功能，使用中断功能重要！！！
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);  //PA15 PWM_F
    EXTI_InitStructure.EXTI_Line =  EXTI_Line15;//
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  //
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;               //
    EXTI_Init(&EXTI_InitStructure);
}


void PwmCapture_Config(void)
{
    PWM_Capture_ExitIOConfig();
    Timer3_Tick_Init();
}


void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line15) != RESET) //判断是否发生中断，发生则中断标志置1
    {
        if ((0 == pwm_f.step) && (1 == PWM_F))
        {
            pwm_f.step = 1 ;
            pwm_f.a = TIM3->CNT;
        }

        if ((1 == pwm_f.step) && (0 == PWM_F))
        {
            pwm_f.step = 2 ;
            pwm_f.b =  TIM3->CNT;
        }

        if ((2 == pwm_f.step) && (1 == PWM_F))
        {
            pwm_f.step = 3 ;
            pwm_f.c =  TIM3->CNT;
        }

        if (3 == pwm_f.step)
        {
            pwm_f.step = 0 ;  // 20 ms  == 2000 * 100 us

            if ((pwm_f.c > pwm_f.a)  && (pwm_f.c - pwm_f.a) > 1600  && (pwm_f.c - pwm_f.a) < 2400)  //TODO 
            {
                pwm_f.puls = (float)(pwm_f.b - pwm_f.a) / (pwm_f.c - pwm_f.a) ;
                pwm_f.flag = 1 ;
            }
        }

    }

    EXTI_ClearITPendingBit(EXTI_Line15);        //
}

