#ifndef __PWM_CAPTURE_H
#define __PWM_CAPTURE_H

#include "includes.h"

#define  PWM_F  PAin(15)

typedef struct
{
    float puls ;
    
    u8 flag  ;
    u8 step ;
    
    vu16 a ;
    vu16 b ;
    vu16 c ;
    
} pwm_inupt_struct ;


extern pwm_inupt_struct  pwm_f ;


void PwmCapture_Config(void);

#endif

