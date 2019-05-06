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
		 CMU_OscillatorEnable(cmuOsc_HFXO, true, false);//ʹ�ܸ��پ���
		 CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO); //

		 break;                    /* ʹ���ⲿ��Ƶ����             */
	 case LClOCK_32K:
		 //CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
		 //CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO); /* LFAʹ��LFXO��Ϊʱ��Դ        */
		 //CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);  /* LFBʹ��LFXO��Ϊʱ��Դ        */
         //CMU_ClockEnable(cmuClock_LFA, true);
         //CMU_ClockEnable(cmuClock_LFB, true); 
		 break;                    /* ʹ���ⲿ��Ƶ����             */
	 case 3: break;                    /* ʹ���ⲿ��Ƶ����             */
	 case 4: break;                    /* ʹ���ⲿ��Ƶ����             */
	 case 5: break;                    /* ʹ���ⲿ��Ƶ����             */
	 default:break;
	}

   /* Enable clock for GPIO*/
  CMU_ClockEnable(cmuClock_GPIO, true);                               /* ʹ��GPIOģ���ʱ��Դ         */
  
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

  /*����HFRCOƵ��Ϊ28MHz,��������MSC */
  CMU_HFRCOBandSet(cmuHFRCOBand_28MHz);                                
}