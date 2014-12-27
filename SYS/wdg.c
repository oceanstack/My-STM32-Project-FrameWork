#include "includes.h"

//保存WWDG计数器的设置值,默认为最大.
u8 WWDG_CNT = 0x7f;

/**
  * @brief  初始化独立看门狗
  * @param  prer : IWDG预分频值 ,  rlr 重装载值
  * @retval None
  */
void IWDG_Init(u8 prer, u16 rlr)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //使能对寄存器IWDG_PR和IWDG_RLR的写操作
    IWDG_SetPrescaler(prer);  //设置IWDG预分频值:设置IWDG预分频值为64
    IWDG_SetReload(rlr);  //设置IWDG重装载值
    IWDG_ReloadCounter();  //按照IWDG重装载寄存器的值重装载IWDG计数器
    IWDG_Enable();  //使能IWDG
    IWDG_Feed();
}


void IWDG_Feed(void)
{
    IWDG_ReloadCounter();//reload
}


//WWDG_Init(0X7F,0X4B,WWDG_Prescaler_8);// 约47ms之后喂狗( 47ms ~58.3ms)

//窗口看门狗中断服务程序
void WWDG_NVIC_Init()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;    //WWDG中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStructure);//NVIC初始化
}

/**
  * @brief  初始化窗口看门狗
  * @param  tr   :T[6:0],计数器值 ,  wr   :W[6:0],窗口值
    @param  fprer:分频系数（WDGTB）,仅最低2位有效 , Fwwdg=PCLK1/(4096*2^fprer).
  * @retval None
  */
void WWDG_Init(u8 tr, u8 wr, u32 fprer)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);  //   WWDG时钟使能
    WWDG_CNT = tr & WWDG_CNT; //初始化WWDG_CNT.
    WWDG_SetPrescaler(fprer);////设置IWDG预分频值
    WWDG_SetWindowValue(wr);//设置窗口值
    WWDG_Enable(WWDG_CNT);	 //使能看门狗 ,	设置 counter .
    WWDG_ClearFlag();//清除提前唤醒中断标志位
    WWDG_NVIC_Init();//初始化窗口看门狗 NVIC
    WWDG_EnableIT(); //开启窗口看门狗中断
}

//重设置WWDG计数器的值
void WWDG_Set_Counter(u8 cnt)
{
    WWDG_Enable(cnt);//使能看门狗 ,	设置 counter .
}


static void WWDG_Feed(void)
{
    WWDG_SetCounter(WWDG_CNT);	  //当禁掉此句后,窗口看门狗将产生复位
    //WWDG_ClearFlag();	  //清除提前唤醒中断标志位
}

void WWDG_Feed_Process(void)
{
//    if (TIMER_SET == bSystem50Msec)
    {
        WWDG_Feed();
    }
}


/**
  * @brief  软件运行异常时看门狗中断函数，用于复位前各种数据的保存.
  * @param  None
  * @retval None
  */
void WWDG_IRQHandler(void)
{
    WWDG_ClearFlag();
}

