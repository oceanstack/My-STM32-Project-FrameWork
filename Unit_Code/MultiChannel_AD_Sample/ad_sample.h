#ifndef __AD_SAMPLE_H
#define __AD_SAMPLE_H

#include "includes.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)
#define ADC3_DR_Address    ((u32)0x40013C00+0x4c)


#define ADC_DMATRANSMIT_FINISHED   1   
#define ADC_DMATRANSMIT_CLEAR    0 

#define MID_FILTER_CNT  10

#define ADC_RANK_NUM 8
#define ADC_CH_NUM  ADC_RANK_NUM

typedef enum {
	RANK_NO_1 = 1,
	RANK_NO_2 ,
	RANK_NO_3 ,
	RANK_NO_4 ,
	RANK_NO_5 ,
	RANK_NO_6 ,
	RANK_NO_7 ,
	RANK_NO_8 ,	
}RANK_NO_ENUM;


typedef enum {
	 ROLL = 0,
	 PITCH,
	 THROTTLE,
	 YAW,
	 GIMBALPITCH ,
	 GIMBALYAW ,
	 BAK1,
	 BAK2,
}ADC_CH_OFFSET_ENUM;


extern vu16   ADC_Value_Raw[ADC_CH_NUM] ;
extern vu16   ADC_Value_Filtered[ADC_CH_NUM]  ;
extern u8  ADC_DMATransmit_Status;
extern vu16 ADC_DMA_Ticks ;



void ADC_Sample_Control_Init(void);

void ADC_Value_Show_Test(void);
void ADC_MidFilter_Process_IRQ(void);

#endif /*__AD_SAMPLE_H*/


