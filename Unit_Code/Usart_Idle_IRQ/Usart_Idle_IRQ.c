#include "includes.h"

/*Data receive through dma and usart idle irq */

u8 USART3_Rev_Buf[USART_RX_BUF];
u8 USART3_RecvStatus = RECEIVING;
u16 Last_Len = 0 ;

Queue_MemCache_Struct Recv_Queue ;


static boolean UART_Recv_MemEnQueue(u8 data)
{
    if (MaxQueueCacheLen == Recv_Queue.MemLength)
    {
        return ERR_Full;
    }
    else
    {
        Recv_Queue.MemDataBuf[Recv_Queue.MemRearRecvIndex] = data ;
        //        if(++Recv_Queue.MemRearRecvIndex >= MaxQueueCacheLen){Recv_Queue.MemRearRecvIndex = 0;}
        Recv_Queue.MemRearRecvIndex = (Recv_Queue.MemRearRecvIndex + 1) % MaxQueueCacheLen;
        Recv_Queue.MemLength ++ ;
        return RW_OK;
    }
}

static boolean UART_Recv_MemDeQueue(u8 *data)
{
    if (0 == Recv_Queue.MemLength)
    {
        return ERR_Empty;
    }
    else
    {
        *data = Recv_Queue.MemDataBuf[Recv_Queue.MemFrontSendIndex]  ;
        Recv_Queue.MemFrontSendIndex = (Recv_Queue.MemFrontSendIndex + 1) % MaxQueueCacheLen;
        Recv_Queue.MemLength -- ;
        return RW_OK;
    }
}


void USART3_DMA_Init(void)
{
	DMA_InitTypeDef	DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/* DMA clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//DMA1

	
	/* DMA1 Channel5 (triggered by USART3 Rx event) Config */
	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32) ( & (USART3->DR)); // USART3_BASE  0x40000000 + 0x4800   =  0x40004804  0x40004804
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART3_Rev_Buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = USART_RX_BUF;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);
	DMA_ITConfig(DMA1_Channel3, DMA_IT_TE, ENABLE);
	/* Enable USART1 DMA RX request */
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
	
	/*Enable DMA Channel3 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;// 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;// 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_Cmd(DMA1_Channel3, ENABLE);
}


void USART3_IDLE_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    USART_DeInit(USART3);  //复位串口
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  //TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    USART_ClearFlag(USART3, USART_FLAG_TC); //防止第一个数据被覆盖
	
    USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//空闲中断 
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
    USART_Cmd(USART3, ENABLE);
}



void USART3_Init(void)
{
	USART3_IDLE_Init(57600);
	USART3_DMA_Init();
}


void USART_Recv_Send_Process(void)
{
	u16 i ;
	u8 buf = 0 ;
	if(	BEEN_RECEIVED == USART3_RecvStatus)
	{
		for(i = 0; i <Last_Len ; i ++)
		{
			UART_Recv_MemDeQueue(&buf);
			USART1_Data_Send_Debug(buf);				
		}
		Last_Len = 0 ;
		
		USART3_RecvStatus = RECEIVING ;
	}

}



void USART3_IRQHandler(void)
{
    u8 res;
	u16 RecvLen = 0;
	u16 i = 0;
    
    if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) //接收中断 有数据为 1 SET
    {
		DMA_Cmd(DMA1_Channel3, DISABLE);
		RecvLen= USART_RX_BUF - DMA_GetCurrDataCounter(DMA1_Channel3);
		
    
     	if(RecvLen > 0 && RecvLen < USART_RX_BUF)
		{
			if(RecvLen <= (MaxQueueCacheLen - Recv_Queue.MemLength))
			{
				Last_Len = RecvLen ;
				for(i=0;i < RecvLen; i++)
				{
					UART_Recv_MemEnQueue(USART3_Rev_Buf[i]); 
				}
				//USART用DMA传输替代查询方式发送,克服被高优先级中断而产生丢帧现象。
			}
		}
		
		DMA_ClearFlag(DMA1_FLAG_GL3 | DMA1_FLAG_TC3 | DMA1_FLAG_TE3 | DMA1_FLAG_HT3);//清标志
		DMA1_Channel3->CNDTR = USART_RX_BUF;//重装填
		DMA_Cmd(DMA1_Channel3, ENABLE);//处理完,重开DMA
		//读SR后读DR清除Idle
		res = USART3->SR;
		res = USART3->DR;
		res = res ;
		USART3_RecvStatus = BEEN_RECEIVED ;
    }
	
	if(USART_GetITStatus(USART3, USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET)//出错
	{
		USART_ClearITPendingBit(USART3, USART_IT_PE | USART_IT_FE | USART_IT_NE);
	}
	
	USART_ClearITPendingBit(USART3, USART_IT_TC);
	USART_ClearITPendingBit(USART3, USART_IT_IDLE);
}



void DMA1_Channel3_IRQHandler(void)
{
	u16 i = 0;
	if(DMA_GetITStatus(DMA1_IT_TC3) != RESET)
	{
		DMA_ClearITPendingBit(DMA1_IT_GL3);
		DMA_ClearITPendingBit(DMA1_IT_TC3);
		DMA_ClearITPendingBit(DMA1_IT_TE3);
		DMA_Cmd(DMA1_Channel3, DISABLE);//关闭DMA,防止处理其间有数据

		if(USART_RX_BUF <= (MaxQueueCacheLen - Recv_Queue.MemLength))
		{
			for(i=0;i < USART_RX_BUF; i++)
			{
				UART_Recv_MemEnQueue(USART3_Rev_Buf[i]); 
			}
			
		}	
		
		USART3_RecvStatus = BEEN_RECEIVED ;		
		
		DMA1_Channel3->CNDTR = USART_RX_BUF;//重装填
		DMA_Cmd(DMA1_Channel3, ENABLE);//处理完,重开DMA
	}
}

