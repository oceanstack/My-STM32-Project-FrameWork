#include "includes.h"


//static vu32 TimingTicks = 0;

static Hinder_Delay_Struct Hinder_Delay ; 


void SysTick_Init(void)
{
    /* SystemFrequency / 1000     1ms�ж�һ��
     * SystemFrequency / 100000	  10us
     * SystemFrequency / 1000000  1us
     */
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	  //TODO  ����˾��Զ���������� 0828 
	
    if (SysTick_Config(SystemCoreClock / 1000000))	// ST3.5.0   1us
    {
        /* Capture error */
        while (1);
    }


    
    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;   
 //   SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
}

/**
  * @brief  ϵͳ�δ�������ʱ us
  * @param  uTime ����us
  * @retval None
  */
void Delay_us(vu32 uTime)
{
    Hinder_Delay.ticks = uTime;
    Hinder_Delay.flag  = 1;
    
    SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
    
    while (0 != Hinder_Delay.ticks);

    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;

    Hinder_Delay.flag  = 0;
}

/**
  * @brief  ϵͳ�δ�������ʱ ms
  * @param  mTime ����ms
  * @retval None
  */
void Delay_ms(vu32 mTime)
{
    Hinder_Delay.ticks = mTime * 1000;
    Hinder_Delay.flag  = 1;
    
    SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
    
    while (0 != Hinder_Delay.ticks);

    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
   
    Hinder_Delay.flag  = 0;
}



void Hinder_TimingTicks_Decrement(void)
{
    if(1 == Hinder_Delay.flag){
      if (0 != Hinder_Delay.ticks)
      {
            Hinder_Delay.ticks--;
      }
    } 
}


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    Hinder_TimingTicks_Decrement();
}






