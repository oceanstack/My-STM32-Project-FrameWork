#include "includes.h"

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
    
};
FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
_sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
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
  * @brief  初始化IO 串口1
  * @param  bound:波特率
  * @retval None
  */
void USART1_Debug_Init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
    USART_DeInit(USART1);  //复位串口1
    //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

        //USART 初始化设置
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART1, &USART_InitStructure); //初始化串口


    USART_ClearFlag(USART1, USART_FLAG_TC);//防止第一个数据被覆盖
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断

    USART_Cmd(USART1, ENABLE);                    //使能串口
}


/**
  * @brief  检测询要打印的模块
  * @param  module: 宏定义模块名
  * @retval 对应模块字符串偏移量
  */
static u8 check_debug_sw(u8 module)  //u8 module 模块变多后此处要注意
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
  * @brief  可进行模块选择的可变参数的打印调试函数
  * @param  module: 宏定义模块名  ...
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
  * @brief  串口1 主要调试用.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
    u8 res;
    
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
    {
        res = USART_ReceiveData(USART1); //(USART->DR); //读取接收到的数据
        res = res ;
    }
}

