#include "dev_clock.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "YB_Wave.h"

void Select_Clock(u8 Mode)
{
	switch(Mode)
	{
	 case HClOCK_32M:
	 	 //CMU_ClockDivSet(cmuClock_HFPER, cmuClkDiv_1);
		 CMU_OscillatorEnable(cmuOsc_HFXO, true, false);//使能高速晶振
		 CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO); //

		 break;                    /* 使用外部高频晶振             */
	 case LClOCK_32K:
		 //CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
		 //CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO); /* LFA使用LFXO作为时钟源        */
		 //CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);  /* LFB使用LFXO作为时钟源        */
         //CMU_ClockEnable(cmuClock_LFA, true);
         //CMU_ClockEnable(cmuClock_LFB, true); 
		 break;                    /* 使用外部高频晶振             */
	 case 3: break;                    /* 使用外部高频晶振             */
	 case 4: break;                    /* 使用外部高频晶振             */
	 case 5: break;                    /* 使用外部高频晶振             */
	 default:break;
	}

   /* Enable clock for GPIO*/
  CMU_ClockEnable(cmuClock_GPIO, true);                               /* 使能GPIO模块的时钟源         */
  
  /* Enable clock for USART1 */
  CMU_ClockEnable(cmuClock_USART1, true);

  /* Enable clock for TIMER0 */
  CMU_ClockEnable(cmuClock_TIMER0, true);
  
  /* Enable clock for TIMER1 */
  CMU_ClockEnable(cmuClock_TIMER1, true);
  
  CMU_ClockEnable(cmuClock_TIMER2, true); 
  
  /* Enable clock for WDOG*/
  CMU_ClockEnable(cmuClock_CORELE, true);
  
  /* Enable clock for ADC0*/
   CMU_ClockEnable(cmuClock_ADC0, true);   

   /* Enable clock for DMA*/
  CMU_ClockEnable(cmuClock_DMA, true);
   
  /* Enable clock for PRS*/
   CMU_ClockEnable(cmuClock_PRS, true);

  /*设置HFRCO频率为28MHz,用来设置MSC */
  CMU_HFRCOBandSet(cmuHFRCOBand_28MHz);                                
}