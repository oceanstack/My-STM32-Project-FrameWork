#ifndef __Usart_Idle_IRQ_H
#define __Usart_Idle_IRQ_H

#include "includes.h"

#define USART_RX_BUF  512 

#define MaxQueueCacheLen  (1024) // 1K
#define RW_OK   0
#define ERR_Full    1
#define ERR_Empty   2


typedef enum {BEEN_RECEIVED = 0, RECEIVING = !BEEN_RECEIVED} Receive_Status;

typedef struct
{
    u8  MemRWFlag ;
    u16 MemFrontSendIndex ;
    u16 MemRearRecvIndex ;
    u16 MemCurDataLen ;   //当前指令长度
    u16 MemLength ;
    u8  MemDataBuf[MaxQueueCacheLen];
} Queue_MemCache_Struct , * Queue_MemCache_Struct_p ;


extern u8 USART3_RecvStatus;

extern Queue_MemCache_Struct Recv_Queue ;

void USART3_Init(void);
void USART_Recv_Send_Process(void);

#endif

