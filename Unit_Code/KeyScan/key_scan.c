#include "includes.h"

Key_Scan_Status_Struct Key_Status ;

Key_FMode_Get_Struct  Key_FMode = {KEY_FMode_Getting,NO_KEY} ;

void Key_Scan_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_Init(GPIOE, &GPIO_InitStructure);//GPIOE1,2,3,4,5,6
}

/**
 * @brief  基本定时器初始化，用于按键扫描.
 * @param  None
 * @retval None
 */
void Key_Scan_IRQ_Tim3Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;   

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 999;   // 1 ms 
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    //	TIM_ARRPreloadConfig(TIM7, DISABLE);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM3, ENABLE);
}

void Key_Scan_Init(void)
{
    Key_Scan_GPIO_Init();
    Key_Scan_IRQ_Tim3Init();

}


void Key_Scan_IRQ_Process(void)
{ 
    static vu8 times = 0;
    static vu8 last_status = 0;
    //    static u8 Key_Flag = 0 ;

    //core code :
    Key_Status.PinRead = ( (KEY1_IPU_IDL ? 0xFF : (~(0x01<<1))) & 
            (KEY2_IPU_IDL ? 0xFF : (~(0x01<<2))) &
            (KEY3_IPU_IDL ? 0xFF : (~(0x01<<3))) &
            (KEY4_IPU_IDL ? 0xFF : (~(0x01<<4))) &
            (KEY5_IPU_IDL ? 0xFF : (~(0x01<<5))) &
            (KEY6_IPU_IDL ? 0xFF : (~(0x01<<6))) ) ^ 0xFF;


    Key_Status.Trigger =  Key_Status.PinRead & ( Key_Status.PinRead ^ Key_Status.Continus);

    Key_Status.Continus = Key_Status.PinRead ;
    // === 

    //    printf("%x \n" ,Key_Status.PinRead );
    //    
    //    printf("%x \n" ,Key_Status.Trigger);

    //    printf("%x \n" ,Key_Status.Continus);

    if((0x00 == Key_Status.Trigger) && (last_status == Key_Status.Continus) )
    {
        switch(Key_Status.Continus){     
            case KEY1 :
            case KEY2 :
            case KEY3 : 
            case KEY4 : 
            case KEY5 :
            case KEY6 :
                if(++ times >= KEY_STAY_DELAY){
                    times = 0;
                    Key_FMode.GetStatus = KEY_FMode_BeenGetted ;
                    Key_FMode.Mode = Key_Status.Continus;
                    //                    printf("%x \n",Key_Status.Continus);
                }

                //                printf("%x \n",Key_Status.Continus);
                last_status  = Key_Status.Continus ;
                break;
            default :
                times  = 0 ;
                last_status  = Key_Status.Continus ;
                break;             
        }
    }
    else
    {
        last_status  = Key_Status.Continus ;
        times  = 0 ;
    }

}

void TIM3_IRQHandler(void)   
{	
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  
    {
        Key_Scan_IRQ_Process();
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);		
    }
}

