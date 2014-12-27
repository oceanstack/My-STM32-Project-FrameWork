#include "includes.h"

//����WWDG������������ֵ,Ĭ��Ϊ���.
u8 WWDG_CNT = 0x7f;

/**
  * @brief  ��ʼ���������Ź�
  * @param  prer : IWDGԤ��Ƶֵ ,  rlr ��װ��ֵ
  * @retval None
  */
void IWDG_Init(u8 prer, u16 rlr)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR��д����
    IWDG_SetPrescaler(prer);  //����IWDGԤ��Ƶֵ:����IWDGԤ��ƵֵΪ64
    IWDG_SetReload(rlr);  //����IWDG��װ��ֵ
    IWDG_ReloadCounter();  //����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������
    IWDG_Enable();  //ʹ��IWDG
    IWDG_Feed();
}


void IWDG_Feed(void)
{
    IWDG_ReloadCounter();//reload
}


//WWDG_Init(0X7F,0X4B,WWDG_Prescaler_8);// Լ47ms֮��ι��( 47ms ~58.3ms)

//���ڿ��Ź��жϷ������
void WWDG_NVIC_Init()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;    //WWDG�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStructure);//NVIC��ʼ��
}

/**
  * @brief  ��ʼ�����ڿ��Ź�
  * @param  tr   :T[6:0],������ֵ ,  wr   :W[6:0],����ֵ
    @param  fprer:��Ƶϵ����WDGTB��,�����2λ��Ч , Fwwdg=PCLK1/(4096*2^fprer).
  * @retval None
  */
void WWDG_Init(u8 tr, u8 wr, u32 fprer)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);  //   WWDGʱ��ʹ��
    WWDG_CNT = tr & WWDG_CNT; //��ʼ��WWDG_CNT.
    WWDG_SetPrescaler(fprer);////����IWDGԤ��Ƶֵ
    WWDG_SetWindowValue(wr);//���ô���ֵ
    WWDG_Enable(WWDG_CNT);	 //ʹ�ܿ��Ź� ,	���� counter .
    WWDG_ClearFlag();//�����ǰ�����жϱ�־λ
    WWDG_NVIC_Init();//��ʼ�����ڿ��Ź� NVIC
    WWDG_EnableIT(); //�������ڿ��Ź��ж�
}

//������WWDG��������ֵ
void WWDG_Set_Counter(u8 cnt)
{
    WWDG_Enable(cnt);//ʹ�ܿ��Ź� ,	���� counter .
}


static void WWDG_Feed(void)
{
    WWDG_SetCounter(WWDG_CNT);	  //�������˾��,���ڿ��Ź���������λ
    //WWDG_ClearFlag();	  //�����ǰ�����жϱ�־λ
}

void WWDG_Feed_Process(void)
{
//    if (TIMER_SET == bSystem50Msec)
    {
        WWDG_Feed();
    }
}


/**
  * @brief  ��������쳣ʱ���Ź��жϺ��������ڸ�λǰ�������ݵı���.
  * @param  None
  * @retval None
  */
void WWDG_IRQHandler(void)
{
    WWDG_ClearFlag();
}

