#ifndef __DEV_SYSTICK_H
#define __DEV_SYSTICK_H

#include "system.h"

void Delay_Cycle(u32 ulCycle);

void Delay_us(u32 nus);

void Delay_ms(u32 dlyTicks);

void Init_SysTick(void);

#endif