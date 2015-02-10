/* ReadME : One Channel ADC Init and can read ADC value once . 
 * It can useful  in certain specified scenes .
 *
 * Data : 2015 02 10 
 *
 * */



void  ADC_Init(void)
{ 	
    ADC_InitTypeDef ADC_InitStructure; 
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟


    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

    //PA1 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
    GPIO_Init(GPIOA, &GPIO_InitStructure);	

    ADC_DeInit(ADC1);  //复位ADC1,

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//单通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//工作在单次转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;	
    ADC_Init(ADC1, &ADC_InitStructure);	


    ADC_Cmd(ADC1, ENABLE);	

    ADC_ResetCalibration(ADC1);	  

    while(ADC_GetResetCalibrationStatus(ADC1));	

    ADC_StartCalibration(ADC1);	 

    while(ADC_GetCalibrationStatus(ADC1));	

    //	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}


u16 ADCValue_Get(void)   
{
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);	

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));

    return ADC_GetConversionValue(ADC1);	
}


