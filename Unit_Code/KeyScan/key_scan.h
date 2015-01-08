#ifndef __KEY_SCAN_H
#define __KEY_SCAN_H

#include "includes.h"

#define KEY_STAY_DELAY 20



#define KEY1_IPU_IDL   PEin(1) //.. 
#define KEY2_IPU_IDL   PEin(2) //.. 
#define KEY3_IPU_IDL   PEin(3) //.. 
#define KEY4_IPU_IDL   PEin(4) //.. 
#define KEY5_IPU_IDL   PEin(5) //.. 
#define KEY6_IPU_IDL   PEin(6) //.. 

typedef enum {KEY_FMode_Getting = 0, KEY_FMode_BeenGetted = !KEY_FMode_Getting} KeyFMode_Status;       

typedef enum
{
    NO_KEY  = 1<<0,
    KEY1    = 1<<1,
    KEY2    = 1<<2,
    KEY3    = 1<<3, 
    KEY4    = 1<<4,
    KEY5    = 1<<5,
    KEY6    = 1<<6,
}KEY_NUMBER_ENUM;

typedef struct Key_Scan{
    u8 PinRead ;
    u8 Trigger ; 
    u8 Continus; 
    u8 PinNumGet; 
}Key_Scan_Status_Struct ;


typedef struct Key_FMode{
    u8 GetStatus ;
    u8 Mode ;  
}Key_FMode_Get_Struct ;

extern Key_FMode_Get_Struct  Key_FMode ;

void Key_Scan_Init(void);
void Key_Scan_IRQ_Process(void);



#endif /*__KEY_SCAN_H*/


