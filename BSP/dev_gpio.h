#ifndef __DEV_GPIO_H
#define __DEV_GPIO_H

#include "em_gpio.h"

#define  Close_Led()   GPIO->P[gpioPortC].DOUTSET = 1 << 14;
#define  Open_Led()   GPIO->P[gpioPortC].DOUTCLR = 1 << 14;

void Init_All_Gpio(void);


void Test_Gpio();

#endif

