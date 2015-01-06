#include "includes.h"


vu16   ADC_Value_Raw[ADC_CH_NUM]  ;
vu16   ADC_Value_Filtered[ADC_CH_NUM]  ;


u8  ADC_DMATransmit_Status;
vu16 ADC_DMA_Ticks = 0 ;  //TODO Del


/**
  * @brief  AD����DMA��ʽ���ͺ�����ʼ��   
  * @param  None
  * @retval None
  */
static void  ADCSample_DMATransmit_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure; 
	
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 |RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	
	/* Configure PC.  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
//    GPIO_ResetBits(GPIOC , GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
//    GPIO_ResetBits(GPIOB , GPIO_Pin_0|GPIO_Pin_1);

#if 1	
	/* Enable DMA clock */	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_Value_Raw;//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = ADC_RANK_NUM;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ����
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
	ADC_InitStructure.ADC_NbrOfChannel = ADC_RANK_NUM;	 	//Ҫת����ͨ����Ŀ 
	ADC_Init(ADC1, &ADC_InitStructure);

	
	/*����ADC1��ͨ��Ϊ55.	5���������ڣ�����Ϊ */ 

#if 1
    RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, RANK_NO_1,  ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, RANK_NO_2,  ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, RANK_NO_3,  ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, RANK_NO_4,  ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, RANK_NO_5,  ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, RANK_NO_6,  ADC_SampleTime_55Cycles5);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_8,  RANK_NO_7,  ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9,  RANK_NO_8,  ADC_SampleTime_55Cycles5);
	
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
void  ADC_MidFilter_Process_IRQ(void)
{
    u8 y = 0 , x = 0; 
    static u8 y_offset = 0;

    u16 adc_min = 0;
	u16 adc_max = 0;
    
    u32 adc_sum = 0;
    
    static u16 ADC_Samble_Buf[ADC_CH_NUM][MID_FILTER_CNT]; 

    if(y_offset >= MID_FILTER_CNT){
        y_offset = 0 ;
        for(x = 0; x < ADC_CH_NUM ; x++){
            adc_min = ADC_Samble_Buf[x][0]; 
            adc_max = ADC_Samble_Buf[x][0];
            for(y = 0; y < MID_FILTER_CNT ; y++){
                if(adc_min > ADC_Samble_Buf[x][y] ){
                    adc_min = ADC_Samble_Buf[x][y];
                }

                if(adc_max < ADC_Samble_Buf[x][y] ){
                    adc_max = ADC_Samble_Buf[x][y];
                }
                adc_sum +=  ADC_Samble_Buf[x][y];

            }
   
            ADC_Value_Filtered[x] = (adc_sum - adc_min - adc_max)/(MID_FILTER_CNT - 2 ); // 
//            ADC_Value_Filtered[x] = (adc_sum)/(MID_FILTER_CNT); // 
            adc_sum = 0;
            adc_max = 0;
            adc_min = 0;

        }
        ADC_DMATransmit_Status = ADC_DMATRANSMIT_FINISHED;


    }
    
    if(y_offset < MID_FILTER_CNT){
        for(x = 0; x < ADC_CH_NUM ; x++ ){
            ADC_Samble_Buf[x][y_offset] = ADC_Value_Raw[x];
        }
        y_offset ++ ;
    }
}

/**
  * @brief  AD���ݲ������˲���ʼ�������������ṩ�Ľӿ�
  * @param  None
  * @retval None
  */
void  ADC_Sample_Control_Init(void)
{
    ADCSample_DMATransmit_Init();
}


void ADC_Value_Show_Test(void)
{
    //    u8 i ;
    static vu16 CH_Test = 1500 ;


    if(ADC_DMATRANSMIT_FINISHED == ADC_DMATransmit_Status)
    {
        ADC_DMATransmit_Status = ADC_DMATRANSMIT_CLEAR  ;


        //            for(i = 0; i < ADC_RANK_NUM ; i++)
        //            {
        //                printf("Rank %d : % d \n",i+1, (ADC_Value_Filtered[i]));
        //            }

        //              printf("Rank %d : % d  , %4.4f \n", 1, (ADC_Value_Raw[0]) , (float)(3.3 * ((float)ADC_Value_Raw[0])/ 4095.0) );

        //            printf("Rank 1 : % f , %d \n" , 3.3 * ADC_Value_Raw.Roll/ 4096.0 );
        //            printf("Rank 2 : % .3f \n" , 3.3 * ADC_Value_Raw.Pitch      / 4096.0 );
        //            printf("Rank 3 : % .3f \n" , 3.3 * ADC_Value_Raw.Throttle   / 4096.0 );
        //            printf("Rank 4 : % .3f \n" , 3.3 * ADC_Value_Raw.Yaw        / 4096.0 );
        //            printf("Rank 5 : % .3f \n" , 3.3 * ADC_Value_Raw.GimbalPitch/ 4096.0 );
        //            printf("Rank 6 : % .3f \n" , 3.3 * ADC_Value_Raw.GimbalYaw  / 4096.0 );
        //            printf("Rank 7 : % .3f \n" , 3.3 * ADC_Value_Raw.Bak1       / 4096.0 );
        //            printf("Rank 8 : % .3f \n" , 3.3 * ADC_Value_Raw.Bak2       / 4096.0 );

        //            printf("Rank 1 : % d \n" , ADC_Value_Raw.Roll);
        //            printf("Rank 2 : % d \n" , ADC_Value_Raw.Pitch);
        //            printf("Rank 3 : % d \n" , ADC_Value_Raw.Throttle);
        //            printf("Rank 4 : % d \n" , ADC_Value_Raw.Yaw);
        //            printf("Rank 5 : % d \n" , ADC_Value_Raw.GimbalPitch);
        //            printf("Rank 6 : % d \n" , ADC_Value_Raw.GimbalYaw);
        //            printf("Rank 7 : % d \n" , ADC_Value_Raw.Bak1);
        //            printf("Rank 8 : % d \n" , ADC_Value_Raw.Bak2);


    }
}


/**
  * @brief  DMAͨ��1��������жϣ�����AD��������.
  * @param  None
  * @retval None
  */
void DMA1_Channel1_IRQHandler(void)
{
    //Debug_Module_Printf( ADC_SAMPLING," 1 Irq \n ");	  
    /* ��� DMA �봫���жϣ������жϺ�ȫ���жϹ����־ */
    ADC_MidFilter_Process_IRQ();
    //    ADC_DMA_Ticks ++ ;
    //    ADC_DMATransmit_Status = ADC_DMATRANSMIT_FINISHED;
    DMA_ClearITPendingBit(DMA1_IT_GL1);	
}

