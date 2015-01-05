#ifndef __TIM_FRQ_H
#define __TIM_FRQ_H

#include "includes.h"

#define CH_COUNT   (16 + 1)  //16 

typedef struct CH_Frq_Set_Data
{
    u16   freq_v_buf;
    u16   freq_compare_set;
} CH_Frq_Out_Set_Struct , * CH_Frq_Out_Set_Struct_p ;

//void Tim1_PWM_Init(void);
//void Tim2_PWM_Init(void);
//void Tim3_PWM_Init(void);
//void Tim4_PWM_Init(void);

void CH_Frq_Out_Init(void);

#endif /*__TIM_FRQ_H */

