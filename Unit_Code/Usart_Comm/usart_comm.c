#include "includes.h"
 
Queue_Mem_Struct Queue_Recv ;
 
boolean QueueMemRecvDataInsert(u8 data)
{
    if (MAX_QUEUE_LEN == Queue_Recv.MemLength)
    {
        return FULL_ERROR;
    }
    else
    {
        Queue_Recv.MemDataBuf[Queue_Recv.MemRearRecvIndex] = data ;
        //        if(++Queue_Recv.MemRearRecvIndex >= MAX_QUEUE_LEN){Queue_Recv.MemRearRecvIndex = 0;}
        Queue_Recv.MemRearRecvIndex = (Queue_Recv.MemRearRecvIndex + 1) % MAX_QUEUE_LEN;
        Queue_Recv.MemLength ++ ;
        return RW_OK;
    }
}
 
boolean QueueMemRecvDataDel(u8 *data)
{
    if (0 == Queue_Recv.MemLength)
    {
        return EMPTY_ERROR;
    }
    else
    {
        *data = Queue_Recv.MemDataBuf[Queue_Recv.MemFrontSendIndex]  ;
        Queue_Recv.MemFrontSendIndex = (Queue_Recv.MemFrontSendIndex + 1) % MAX_QUEUE_LEN;
        Queue_Recv.MemLength -- ;
        return RW_OK;
    }
}


void USART2_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
    
    USART_DeInit(USART2);  
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽   
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure); 

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);   

    USART_ClearFlag(USART2, USART_FLAG_TC); //防止第一个数据被覆盖
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);                   

}

//recv
void USART2_ByteRecvIrq(u8 data)
{
    QueueMemRecvDataInsert(data);
}

//recv data analysis
void USART2_ByteRecvTakeIrq(u8 data)
{
    QueueMemRecvDataDel(data);
}


void USART2_Link_Comm_Init(void)
{
    USART2_Init(57600);
}


void USART2_IRQHandler(void)
{
    u8 res;

    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //Recv Irq
    {
        res = USART_ReceiveData(USART2); //(USART->DR);	
        USART2_ByteRecvIrq(res);
        //res = res;
    }   
}

