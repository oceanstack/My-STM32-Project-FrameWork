#include "includes.h"

vu16   ADC_Value_Raw  ;
vu16   ADC_Value_Filtered  ;
u8  ADC_DMATransmit_Status;

/**
  * @brief  AD采样DMA方式传送函数初始化   
  * @param  None
  * @retval None
  */
void  ADCSample_DMATransmit_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure; 
	
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
	
	/* Configure PC.  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
//    GPIO_ResetBits(GPIOC , GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);


#if 1	
	/* Enable DMA clock */	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_Value_Raw;//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	    /* 开启DMA传输完成中断 */
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);  
#endif 
	/* ADC1 configuration */
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立ADC模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //扫描模式用于多通道采集
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//开启连续转换模式，即不停地进行ADC转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部触发转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//采集数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	 	//要转换的通道数目 
	ADC_Init(ADC1, &ADC_InitStructure);

	
	/*配置ADC1的通道为55.	5个采样周期，序列为 */ 

#if 1
    RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1,  ADC_SampleTime_55Cycles5);
	
#endif	

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	
	/*复位校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	/*等待校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ADC校准 */
	ADC_StartCalibration(ADC1);
	/* 等待校准完成*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	/* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	/* Enable DMA channel1 */
//	DMA_Cmd(DMA1_Channel1, ENABLE); 

}




/**
  * @brief  AD采样中值滤波
  * @param  None
  * @retval None
  */
static void  ADC_MidFilter_Process_IRQ(void)
{
    u8 y = 0; 
    static u8 y_offset = 0;

    u16 adc_min = 0;
	u16 adc_max = 0;
    
    u32 adc_sum = 0;
    
    static u16 ADC_Samble_Buf[MID_FILTER_CNT]; 

    if(y_offset >= MID_FILTER_CNT){
        y_offset = 0 ;
      
        adc_min = ADC_Samble_Buf[0]; 
        adc_max = ADC_Samble_Buf[0];
		
        for(y = 0; y < MID_FILTER_CNT ; y++){
                if(adc_min > ADC_Samble_Buf[y] ){
                    adc_min = ADC_Samble_Buf[y];
                }

                if(adc_max < ADC_Samble_Buf[y] ){
                    adc_max = ADC_Samble_Buf[y];
                }
                adc_sum +=  ADC_Samble_Buf[y];
        }
       

        ADC_Value_Filtered = (adc_sum - adc_min - adc_max)/(MID_FILTER_CNT - 2 ); // 
        adc_sum = 0;
        adc_max = 0;
        adc_min = 0;      
        ADC_DMATransmit_Status = ADC_DMATRANSMIT_FINISHED;


    }
    
    if(y_offset < MID_FILTER_CNT){     
        ADC_Samble_Buf[y_offset] = ADC_Value_Raw; 
        y_offset ++ ;
    }
}




/**
  * @brief  DMA通道1传输完成中断，用于AD采样数据.
  * @param  None
  * @retval None
  */
void DMA1_Channel1_IRQHandler(void)
{
    ADC_MidFilter_Process_IRQ();

    DMA_ClearITPendingBit(DMA1_IT_GL1);	
}




