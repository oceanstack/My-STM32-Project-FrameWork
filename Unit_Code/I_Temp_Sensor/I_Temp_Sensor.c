#include "includes.h"


void T_Adc_Init(void)  
{
    ADC_InitTypeDef ADC_InitStructure; 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);  

    ADC_DeInit(ADC1);  //将外设 ADC1 的全部寄存器重设为缺省值

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;	
    ADC_Init(ADC1, &ADC_InitStructure);	

    ADC_TempSensorVrefintCmd(ENABLE); //开启内部温度传感器


    ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1

    ADC_ResetCalibration(ADC1);	//重置指定的ADC1的复位寄存器

    while(ADC_GetResetCalibrationStatus(ADC1));	

    ADC_StartCalibration(ADC1);	 //

    while(ADC_GetCalibrationStatus(ADC1));	
}


//Temp=(1.43-ADCValue/4095.0*3.3)/0.0043 + 25.0;  
float T_Adc_GetTemp(void)   
{
    float temperate;
    u16 adc_raw ;

    ADC_RegularChannelConfig(ADC1, ADC_Channel_16 , 1, ADC_SampleTime_239Cycles5 );  			    

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
    adc_raw =  ADC_GetConversionValue(ADC1);	
    temperate = (1.43- (float)adc_raw /4095.0*3.3)/0.0043 + 25.0;  
    return temperate ;
}


