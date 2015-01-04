#ifndef __AD_SAMPLE_H
#define __AD_SAMPLE_H

#include "includes.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)


#define ADC_DMATRANSMIT_FINISHED   1   
#define ADC_DMATRANSMIT_CLEAR    0

#define MID_FILTER_CNT  50


extern vu16   ADC_Value_Raw  ;
extern vu16   ADC_Value_Filtered  ;
extern u8  ADC_DMATransmit_Status;



void  ADCSample_DMATransmit_Init(void);


#endif

