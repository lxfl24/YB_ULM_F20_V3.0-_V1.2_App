#ifndef  __DEV_CLOCK_H
#define  __DEV_CLOCK_H

#include "system.h"

#define   HClOCK_32M   0x01    
#define   LClOCK_32K   0x02  

void Select_Clock(u8 Mode);


#endif
