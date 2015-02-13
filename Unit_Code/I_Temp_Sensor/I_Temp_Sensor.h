#ifndef  __I_TEMP_SENSOR_H
#define  __I_TEMP_SENSOR_H

/*
 *README : Get the stm32 internal  temperature .
 *
 * */

void T_Adc_Init(void);
float T_Adc_GetTemp(void);

#endif
