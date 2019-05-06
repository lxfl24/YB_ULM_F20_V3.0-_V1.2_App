#ifndef __DEV_ADC_H
#define __DEV_ADC_H

#include "em_adc.h"
#include "system.h"


#define POINT_NUM    2000
void Init_SingleADC (void);

void ADC_Switch_Channel(ADC_SingleInput_TypeDef input);

void ADC_Adjust_AcqTime(ADC_AcqTime_TypeDef acqtime);

void ADC_Adjust_Resolution(ADC_Res_TypeDef Resolution);

void Init_ScanADC(void);

void Test_ADC(void);

void Test_ADC_Temp(void);



extern u32 ADC_Temp[POINT_NUM];


#endif 

