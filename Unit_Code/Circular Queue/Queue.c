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

Queue_Mem_Struct Queue_Recv ;

boolean QueueMemDataInsert(u8 data)
{
    if (MAX_QUEUE_LEN == Queue_Recv.MemLength)
    {
        return FULL_ERROR;
    }
    else
    {
        Queue_Recv.MemDataBuf[Queue_Recv.MemRearRecvIndex] = data ;
        Queue_Recv.MemRearRecvIndex = (Queue_Recv.MemRearRecvIndex + 1) % MAX_QUEUE_LEN;
        Queue_Recv.MemLength ++ ;
        return RW_OK;
    }
}

boolean QueueMemDataDel(u8 *data)
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

