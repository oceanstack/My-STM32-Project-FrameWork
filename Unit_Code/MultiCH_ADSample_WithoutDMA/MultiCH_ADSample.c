#include "includes.h"

//ADC1_GPIO配置
static void ADC1_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //CH_10->PC0	CH_11->PC1	CH_12->PC2	CH_13->PC3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//adc模式必须是模拟输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);//

    //CH_0->PA0	CH_1->PA1	CH_2->PA2	CH_3->PA3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //注意，内部温度对应 16通道，无引脚，只需开启adc时钟即可。
    //内部参考电压，对应17 通道。无引脚。只需开启时钟
}


void MultiCH_ADC_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);

    ADC1_GPIO_Configuration();


    /* ADC1 configuration ------------------------------------------------------*/
    //ADC独立模式	 相对于双重模式
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    //扫描模式用于多通道采集
    //而这次虽然也是多通道，单是采用非DMA模式，避免扫描模式因读取数据不及时而造成下一个通道的
    //数据覆盖本组数据，所以采用单次转换模式，即：采集完成本组后，并且数据被读走，才开始下一个
    //通道的工作
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    //开启连续转换模式   当转换完本组（可能是一个）继续重新开始执行
    //相对于单次模式：转换一次后就结束
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    //不使用外部触发转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    //采集数据右对齐
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    //转换组的通道数目
    ADC_InitStructure.ADC_NbrOfChannel =1;
    //执行
    ADC_Init(ADC1, &ADC_InitStructure);

    //配置ADC时钟，为PCLK2的8分频，即9Hz
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    /* ADC1 regular channel11 configuration */ 
    //配置ADC1的通道11为55.5个采样周期
    //默认组，adc1 ，通道11，排序为1,55.5周期

    //----------------------使能温度传感器----------------------------
    ADC_TempSensorVrefintCmd(ENABLE);


    ADC_Cmd(ADC1, ENABLE);
    // ADC_SoftwareStartConvCmd(ADC1, DISABLE);
    /* Enable ADC1 reset calibration register */ 
    //使能ADC1的复位校准寄存器  
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    //等待校准完成
    while(ADC_GetResetCalibrationStatus(ADC1));

    /* Start ADC1 calibration */
    //使能ADC1的开始校准寄存器
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    //等待完成
    while(ADC_GetCalibrationStatus(ADC1));

}


u16 ADC_ConvertedValue_Get(u8 ch)
{
    u16 value = 0 ;

    switch(ch)
    {
        //----------------内部2路模拟输入--------------------------

        case ADC_Channel_17:	  //通道17（内部1.2v参照电压），规则组序号1,239.5clk，时间长采样准确
            ADC_RegularChannelConfig(ADC1, ADC_Channel_17,1, ADC_SampleTime_239Cycles5);break; 

        case ADC_Channel_16:	 //通道16（内部温度），规则组1.。。。
            ADC_RegularChannelConfig(ADC1, ADC_Channel_16,1, ADC_SampleTime_239Cycles5);break;

            //----------------外部8路模拟输入 -------------------------

        case ADC_Channel_4:	 //通道4（外部可变电阻电压），规则组序号1，。。。
            ADC_RegularChannelConfig(ADC1, ADC_Channel_4,1, ADC_SampleTime_239Cycles5);break;

        case ADC_Channel_5:	 //通道5（外部可变电阻电压），规则组序号1，。。。
            ADC_RegularChannelConfig(ADC1, ADC_Channel_5,1, ADC_SampleTime_239Cycles5);break;

        case ADC_Channel_6:	 //通道6（外部可变电阻电压），规则组序号1，。。。
            ADC_RegularChannelConfig(ADC1, ADC_Channel_6,1, ADC_SampleTime_239Cycles5);break;

        case ADC_Channel_7:	 //通道7（外部可变电阻电压），规则组序号1，。。。
            ADC_RegularChannelConfig(ADC1, ADC_Channel_7,1, ADC_SampleTime_239Cycles5);break;

        case ADC_Channel_10:	 //通道10（外部可变电阻电压），规则组序号1，。。。
            ADC_RegularChannelConfig(ADC1, ADC_Channel_10,1, ADC_SampleTime_239Cycles5);break;

        case ADC_Channel_11:	 //通道11（外部可变电阻电压），规则组序号1，。。。
            ADC_RegularChannelConfig(ADC1, ADC_Channel_11,1, ADC_SampleTime_239Cycles5);break;

        case ADC_Channel_12:	 //通道12（外部可变电阻电压），规则组序号1，。。。
            ADC_RegularChannelConfig(ADC1, ADC_Channel_12,1, ADC_SampleTime_239Cycles5);break;

        case ADC_Channel_13:	 //通道13（外部可变电阻电压），规则组序号1，。。。
            ADC_RegularChannelConfig(ADC1, ADC_Channel_13,1, ADC_SampleTime_239Cycles5);break;

        default :
            return 0 ;
    }

    ADC_Cmd(ADC1, ENABLE); //使能ADC1

    if(ch ==ADC_Channel_16 )
    {
        ADC_TempSensorVrefintCmd(ENABLE); //开启内部温度传感器`
    }

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//启动上面设置好的一个通道，进行转换	
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);//等待EOC置位

    value = ADC_GetConversionValue (ADC1);	//把数据寄存器的值读走

    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);	   //清除EOC，DMA时读数据，硬件自动清除

    if(ch ==ADC_Channel_16 )
    {
        ADC_TempSensorVrefintCmd(DISABLE); 
    }

    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
    ADC_Cmd(ADC1, DISABLE);

    return value; 

}

