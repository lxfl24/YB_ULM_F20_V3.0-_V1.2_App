#ifndef __DEV_RTC_H
#define __DEV_RTC_H



#include "efm32.h"
#include "em_cmu.h"



#ifdef __cplusplus
extern "C" {
#endif

void RTC_Delay(uint32_t msec);

void RTC_IRQHandler(void);

void RTC_Trigger(uint32_t msec, void (*cb)(void));

void RTC_Setup(CMU_Select_TypeDef osc);

#ifdef __cplusplus
}
#endif













#endif

