#include "includes.h"

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
    int handle;
    
};
FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
_sys_exit(int x)
{
    x = x;
}
//�ض���fputc����
int fputc(int ch, FILE *f)
{
    vu32 delay_cnt = 0;
#if 1
    
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {
        if (++delay_cnt >= USART_TIMEOUT)
        {
            break;
        }
    }
    
    if (delay_cnt < USART_TIMEOUT)
    {
        USART_SendData(USART1, (u8)ch);
    }
    else
    {
    }
    
    delay_cnt = 0;
    return ch;
#endif
}

char *DEBUG_MODULE_STR[] =
{
    "NOTHING",
    "SYS",
    "WDG",
};


/**
  * @brief  ��ʼ��IO ����1
  * @param  bound:������
  * @retval None
  */
void USART1_Debug_Init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
    USART_DeInit(USART1);  //��λ����1
    //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

        //USART ��ʼ������
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure); //��ʼ������


    USART_ClearFlag(USART1, USART_FLAG_TC);//��ֹ��һ�����ݱ�����
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�

    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���
}


/**
  * @brief  ���ѯҪ��ӡ��ģ��
  * @param  module: �궨��ģ����
  * @retval ��Ӧģ���ַ���ƫ����
  */
static u8 check_debug_sw(u8 module)  //u8 module ģ�����˴�Ҫע��
{
    u8 i ;
    
    if (0 == module)
    {
        return 0;
    }
    else
    {
        for (i = 0; i < DEBUG_CNT; i++)
        {
            if (module & (1 << i))
            {
                return (i + 1);
            }
        }
    }
    
    return 0;
}


/**
  * @brief  �ɽ���ģ��ѡ��Ŀɱ�����Ĵ�ӡ���Ժ���
  * @param  module: �궨��ģ����  ...
  * @retval None
  */

void Debug_Module_Printf(int module, char *fmt, ...)
{
    va_list ap;

    u8 flag = 0 ;

    if (0 != (flag = check_debug_sw(module)))
    {
        printf("[ %s ]", DEBUG_MODULE_STR[flag]);

        va_start(ap, fmt);

        vprintf(fmt,ap);

        va_end(ap);
    }
}


/**
  * @brief  ����1 ��Ҫ������.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
    u8 res;
    
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
    {
        res = USART_ReceiveData(USART1); //(USART->DR); //��ȡ���յ�������
        res = res ;
    }
}

