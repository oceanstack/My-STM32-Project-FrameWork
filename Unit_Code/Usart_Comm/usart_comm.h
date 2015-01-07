#ifndef __USART_COMM_
#define __USART_COMM_


#define MAX_QUEUE_LEN  (4096) // 4K
#define RW_OK   0
#define FULL_ERROR    1
#define EMPTY_ERROR   2
 
typedef uint8_t boolean;
 
typedef struct
{
    u16 MemFrontSendIndex ;
    u16 MemRearRecvIndex ;
    u16 MemLength ;
    u8  MemDataBuf[MAX_QUEUE_LEN];
} Queue_Mem_Struct , * Queue_Mem_Struct_p ;



void USART2_Link_Comm_Init(void);
void USART2_ByteRecvTakeIrq(u8 data);

#endif /*__USART_COMM_H*/

