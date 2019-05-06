#include "dev_DMA.h"
#include "em_DMA.h"
#include "system.h"
#include "dev_ADC.h"


/** DMA control block, requires proper alignment. */
#if defined (__ICCARM__)
#pragma data_alignment=256
DMA_DESCRIPTOR_TypeDef dmaControlBlock[DMA_CHAN_COUNT * 2];
#elif defined (__CC_ARM)
DMA_DESCRIPTOR_TypeDef dmaControlBlock[DMA_CHAN_COUNT * 2] __attribute__ ((aligned(256)));
#elif defined (__GNUC__)
DMA_DESCRIPTOR_TypeDef dmaControlBlock[DMA_CHAN_COUNT * 2] __attribute__ ((aligned(256)));
#else
#error Undefined toolkit, need to define alignment
#endif


/* DMA callback structure */
DMA_CB_TypeDef cb;

/** DMA channel used for scan sequence sampling adc channel 2, 3 and 4. */
#define DMA_CHANNEL    0
#define NUM_SAMPLES    1

u8 DMA_TransferSUM=0;

uint32_t samples[1];

extern uint32_t convered_count;

extern Bool Collect_Distance_OK;


/**************************************************************************//**
 * @brief  Call-back called when transfer is complete
 *****************************************************************************/
void transferComplete(unsigned int channel, bool primary, void *user)
{

	   if(DMA_TransferSUM==0)
	   	{
		    DMA_ActivateBasic(DMA_CHANNEL,
		                      true,
		                      false,
		                      ADC_Temp+1000,
		                      (void *)((uint32_t) &(ADC0->SINGLEDATA)),
		                     POINT_NUM-1000);
			DMA_TransferSUM++;
	     }  
	   else
	      {
	       //ADC_Temp[convered_count]=samples[0];
		   //convered_count++; 
	       //if(convered_count >POINT_NUM-1)
	       //{    
         TIMER2->CMD = TIMER_CMD_STOP;//关闭定时器2
         Collect_Distance_OK=1;
		 DMA_TransferSUM=0;
         //convered_count = 0;
		//}
      }
   // printf("%d, %d, %d \r\n", samples[0], samples[1], samples[2]);//打印log信息
    
   // GPIO_PinOutToggle(gpioPortD, 7); //用来测试ADC扫描是否正常工作
}


/***************************************************************************//**
* @brief
*   Configure DMA usage for this application.
*******************************************************************************/
void DMAConfig(void)
{
    DMA_Init_TypeDef       dmaInit;
    DMA_CfgDescr_TypeDef   descrCfg;
    DMA_CfgChannel_TypeDef chnlCfg;
    
    /* Configure general DMA issues */
    dmaInit.hprot        = 0;
    dmaInit.controlBlock = dmaControlBlock;
    DMA_Init(&dmaInit);
    
    /* Setting up call-back function */  
    cb.cbFunc  = transferComplete;
    cb.userPtr = NULL;
    
    /*Configure DMA channel used */
    chnlCfg.highPri   = false;
    chnlCfg.enableInt = true;
    chnlCfg.select    = DMAREQ_ADC0_SINGLE;  //DMAREQ_ADC0_SCAN;
    chnlCfg.cb        = &cb;
    DMA_CfgChannel(DMA_CHANNEL, &chnlCfg);
    
    descrCfg.dstInc  = dmaDataInc4;
    descrCfg.srcInc  = dmaDataIncNone;
    descrCfg.size    = dmaDataSize4;
    descrCfg.arbRate = dmaArbitrate1;
    descrCfg.hprot   = 0;
    DMA_CfgDescr(DMA_CHANNEL, true, &descrCfg);

}
