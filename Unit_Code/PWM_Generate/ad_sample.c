#include "includes.h"

vu16   ADC_Value_Raw  ;
vu16   ADC_Value_Filtered  ;
u8  ADC_DMATransmit_Status;

/**
  * @brief  AD����DMA��ʽ���ͺ�����ʼ��   
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
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_Value_Raw;//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	    /* ����DMA��������ж� */
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);  
#endif 
	/* ADC1 configuration */
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //ɨ��ģʽ���ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//��������ת��ģʽ������ͣ�ؽ���ADCת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	 	//Ҫת����ͨ����Ŀ 
	ADC_Init(ADC1, &ADC_InitStructure);

	
	/*����ADC1��ͨ��Ϊ55.	5���������ڣ�����Ϊ */ 

#if 1
    RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1,  ADC_SampleTime_55Cycles5);
	
#endif	

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	
	/*��λУ׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	/*�ȴ�У׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ADCУ׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ�У׼���*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	/* Enable DMA channel1 */
//	DMA_Cmd(DMA1_Channel1, ENABLE); 

}




/**
  * @brief  AD������ֵ�˲�
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
  * @brief  DMAͨ��1��������жϣ�����AD��������.
  * @param  None
  * @retval None
  */
void DMA1_Channel1_IRQHandler(void)
{
    ADC_MidFilter_Process_IRQ();

    DMA_ClearITPendingBit(DMA1_IT_GL1);	
}




