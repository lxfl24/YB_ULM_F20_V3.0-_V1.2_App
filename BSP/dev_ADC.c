#include "dev_ADC.h"
#include "em_adc.h"
#include "em_cmu.h"
#include "em_timer.h"
#include "system.h"

extern u8 Channel_number;

extern Bool Temper_Rev_Flag;
extern Bool Voltage_Rev_Flag;
extern Bool Collect_Distance_OK;

u16 convered_count=0;
u32 ADC_Temp[POINT_NUM]={0};

/*********************************************************************************************************
** Function name:      adcInit
** Descriptions:       ADC��ͨ����ʼ��
** input parameters:   none
** output parameters:  none
** Returned value:     none
** Created by:         chenhong
** Created Date:       2018-05-16
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void Init_SingleADC (void)
{
  #if 1  
 ADC_Init_TypeDef tAdcInit = {
        .ovsRateSel = adcOvsRateSel2,                                   /* ADC����������                */
        .lpfMode    = adcLPFilterBypass,                                /* ��·�����˲�RC��·           */
        .warmUpMode = adcWarmupNormal,                                  /* ����Ԥ��ģʽ                 */
        .timebase   = ADC_TimebaseCalc(0),                              /* ��ʱ������                   */
        .prescale   = ADC_PrescaleCalc(13000000, 0),                     /* ADCʱ�ӷ�Ƶ����              *///7MHZ
        .tailgate   = false                                             /* ��ʹ��Tailgate               */
    };
 ADC_InitSingle_TypeDef tSingleInit = {
        .prsSel     = adcPRSSELCh1,                                     /* ѡ��PRSͨ��0 */
        .acqTime    = adcAcqTime16,                                     /* ���òɼ�ʱ��Ϊ16����         */
        .reference  = adcRefVDD,                                        /* ʹ��VDD�ο���ѹ                */
        .resolution = adcRes12Bit,                                      /* ʹ��12λ�ֱ���                 */
        .input      = adcSingleInpCh6,                                  /* ����ѡ��VDD/3                */
        .diff       = false,                                            /* �����ò�ֲɼ�ģʽ           */
        .prsEnable  = true,                                            /* ����PRS����                  */
        .leftAdjust = false,                                            /* ʹ���Ҷ�׼                   */
        .rep        = false                                             /* ��ʹ������ת��               */
    };
 
    ADC_Init(ADC0, &tAdcInit);

    ADC_InitSingle(ADC0, &tSingleInit);                                 /* ��ʼ��ADC����ת��            */
    /*
     *  ʹ�ܵ��βɼ�����ж�
     */
    ADC_IntEnable(ADC0, ADC_IF_SINGLE);
    NVIC_EnableIRQ(ADC0_IRQn);
  #else
  
    ADC_Init_TypeDef tAdcInit = {
		   .ovsRateSel = adcOvsRateSel2,								   /* ADC���������� 			   */
		   .lpfMode    = adcLPFilterBypass, 							   /* ��·�����˲�RC��·		   */
		   .warmUpMode = adcWarmupNormal,								   /* ����Ԥ��ģʽ				   */
		   .timebase   = ADC_TimebaseCalc(0),							   /* ��ʱ������				   */
		   .prescale   = ADC_PrescaleCalc(7000000, 0),					   /* ADCʱ�ӷ�Ƶ����			   *///7MHZ
  		   .tailgate   = false											   /* ��ʹ��Tailgate			   */
	   };
	   ADC_InitSingle_TypeDef tSingleInit = {
		   .prsSel	   = adcPRSSELCh0,									   /* ѡ��PRSͨ��0 */
		   .acqTime    = adcAcqTime16,									   /* ���òɼ�ʱ��Ϊ16����		   */
		   .reference  = adcRef1V25,										/* ʹ��VDD�ο���ѹ				  */
		   .resolution = adcRes12Bit,									   /* ʹ��12λ�ֱ���				 */
		   .input	   = adcSingleInpTemp,								/* ����ѡ��VDD/3				*/
		   .diff	   = false, 										   /* �����ò�ֲɼ�ģʽ		   */
		   .prsEnable  = false, 										   /* ����PRS����				   */
		   .leftAdjust = false, 										   /* ʹ���Ҷ�׼				   */
		   .rep 	   = false											   /* ��ʹ������ת��			   */
	   };
	   ADC_Init(ADC0, &tAdcInit);
   
	   ADC_InitSingle(ADC0, &tSingleInit);								   /* ��ʼ��ADC����ת�� 		   */
	   /*
		*  ʹ�ܵ��βɼ�����ж�
		*/
	   ADC_IntEnable(ADC0, ADC_IF_SINGLE);
	   NVIC_EnableIRQ(ADC0_IRQn);
  #endif
}

/*********************************************************************************************************
** Function name:      Init_ScanADC
** Descriptions:       ��ʼ����ͨ��ADC
** input parameters:   none
** output parameters:  none
** Returned value:     none
** Created by:         chenhong
** Created Date:       2018-05-16
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/

void Init_ScanADC ()
{
        ADC_Init_TypeDef     init     = ADC_INIT_DEFAULT;
	ADC_InitScan_TypeDef scanInit = ADC_INITSCAN_DEFAULT;

	/* Init common issues for both single conversion and scan mode */
	init.timebase = ADC_TimebaseCalc(0);
	init.prescale = ADC_PrescaleCalc(7000000, 0);
	ADC_Init(ADC0, &init);

	/* Init for scan sequence use ( for dvk: accelerometer X, Y and Z axis). */
	scanInit.reference = adcRefVDD;
	scanInit.input     = ADC_SCANCTRL_INPUTMASK_CH2 | ADC_SCANCTRL_INPUTMASK_CH3 | ADC_SCANCTRL_INPUTMASK_CH4;
	scanInit.prsEnable = true;
	ADC_InitScan(ADC0, &scanInit);

	/* Enable ADC Interrupt when scan Conversion Complete */
	ADC_IntEnable(ADC0, ADC_IEN_SCANOF);

	/* Enable ADC interrupt vector in NVIC*/
	NVIC_EnableIRQ(ADC0_IRQn);
}

/*********************************************************************************************************
** Function name:      ADC_Switch_Channel
** Descriptions:       ADCͨ���л�����
** input parameters:   input�������ͨ��
** output parameters:  none
** Returned value:     none
** Created by:         chenhong
** Created Date:       2018-05-16
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void ADC_Switch_Channel(ADC_SingleInput_TypeDef input)
{
    /* Make sure single conversion is not in progress */
    ADC0->CMD = ADC_CMD_SINGLESTOP;
    /* Clear the input channel */
    ADC0->SINGLECTRL &= ~_ADC_SINGLECTRL_INPUTSEL_MASK;
    /* Update the input channel */
    ADC0->SINGLECTRL |= input << _ADC_SINGLECTRL_INPUTSEL_SHIFT;
}


void ADC_Adjust_AcqTime(ADC_AcqTime_TypeDef acqtime)
{
    /* Make sure single conversion is not in progress */
    ADC0->CMD = ADC_CMD_SINGLESTOP;
  
    ADC0->SINGLECTRL &= ~_ADC_SINGLECTRL_AT_MASK;
    
    ADC0->SINGLECTRL |= acqtime << _ADC_SINGLECTRL_AT_SHIFT;
}


void ADC_Adjust_Resolution(ADC_Res_TypeDef Resolution)
{
    /* Make sure single conversion is not in progress */
    ADC0->CMD = ADC_CMD_SINGLESTOP;
  
    ADC0->SINGLECTRL &= ~_ADC_SINGLECTRL_RES_MASK;
    
    ADC0->SINGLECTRL |= Resolution << _ADC_SINGLECTRL_RES_SHIFT;
}


/*********************************************************************************************************
** Function name:      ADC0_IRQHandler
** Descriptions:       ADC�жϷ�����
** input parameters:   none
** output parameters:  none
** Returned value:     none
** Created by:         chenhong
** Created Date:       2018-05-16
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void ADC0_IRQHandler (void)
{                      
    /*if(Channel_number==0x05)
	{
	   ADC_Temp[convered_count]=(uint32_t)(ADC0->SINGLEDATA);//ADC_DataSingleGet(ADC0);
	   convered_count++; 
           if(convered_count > POINT_NUM-1)
           {
             TIMER2->CMD = TIMER_CMD_STOP;//�رն�ʱ��2
             convered_count = 0;
             Collect_Distance_OK=1;
           }
	}
	else
	{*/
	  if(Channel_number==0x07)
		{
		   Temper_Rev_Flag=1;
		}
		else if(Channel_number==0x06)
		{
		   Voltage_Rev_Flag=1;
		}
	//}
	ADC0->IFC = ADC_IF_SINGLE;     /* ��ADC��ͨ���жϱ�־λ              */
}


void Test_ADC()
{
	  uint32_t sample;
	  double   voltage=0;

	  ADC_Switch_Channel(adcSingleInpCh6);

	  ADC_Start(ADC0, adcStartSingle);

	  /* Wait while conversion is active */
	  while (ADC0->STATUS & ADC_STATUS_SINGLEACT) ;

	  /* Get ADC result */
	  sample = ADC_DataSingleGet(ADC0);

	  /* Calculate supply voltage */
	  voltage = (sample * 3.2) / 4096; //����Vdd=3.2V
            
}

void Test_ADC_Temp()
{
	   u32 Temper=0;
	   float temp=0;
	   u32 calTemp0;
	   u32 calValue0;
	   u32  adcSample=0;


	   
	   ADC_Start(ADC0, adcStartSingle);

	   /* Wait while conversion is active */
	    while (ADC0->STATUS & ADC_STATUS_SINGLEACT) ;
	   
			 /* Get ADC result */
	   adcSample =ADC_DataSingleGet(ADC0);

       calTemp0 = ((DEVINFO->CAL & _DEVINFO_CAL_TEMP_MASK) >> _DEVINFO_CAL_TEMP_SHIFT);  //У׼���¶�ֵ
      calValue0 = ((DEVINFO->ADC0CAL2&_DEVINFO_ADC0CAL2_TEMP1V25_MASK)>>_DEVINFO_ADC0CAL2_TEMP1V25_SHIFT);      //У׼�¶��µ�ADCֵ
       //����ʵ�ʻ���ADC���õ�ֵ����У׼ֵ֮��������㲹�����ó������¶�ֵ
       //���㹫ʽΪ��
       //TCELSIUS = CAL_TEMP-��ADC0CAL3_TEMPREAD1V25 - ADC_result����VFS /��4096�� V_TS_SLOPE�
	  s32 readDiff =calValue0 - adcSample;
	  temp=((float)readDiff * 1250);
      temp/=(4096 * -6.3);
	/* Calculate offset from calibration temperature */
	  temp =(float)calTemp0-temp;
      Temper=temp;
	 __NOP();
                
       

}
