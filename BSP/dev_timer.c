#include "dev_timer.h"
#include "em_timer.h"
#include "em_cmu.h"
#include "em_prs.h"
#include "dev_uart.h"
#include "dev_ADC.h"
#include "dev_gpio.h"
#include "YB_Connect.h"


//�ҵĵ�·���ⲿ����Ϊ32M����ʱ������ʱѡ��8��Ƶ�����Զ�ʱ����ʱ��Ƶ��Ϊ32M/8=4MHz;
//TIMER0_TOP_VAL(4)*��1/4M)=1us������ʱ����Ϊ1us����ʾ�������ԣ�����������Ϊ1us��

#define  INTERVAL_PER_SEC0   1		       //��ʱ��0ÿ1s�ж�һ��		               
#define  TIMER0_TOP_VAL  	 ((CMU_ClockFreqGet(cmuClock_HFPER)/1024)/INTERVAL_PER_SEC0)	 //��ʱ����ʼֵ,2msһ���ж�	��CPUƵ�ʸı�Ҳ��Ӱ��

#define  INTERVAL_PER_SEC2   400000	                 //��ʱ��0ÿ2.5us�ж�һ�� //5000002us		               
#define  TIMER2_TOP_VAL  	 ((CMU_ClockFreqGet(cmuClock_HFPER))/INTERVAL_PER_SEC2)	 //��ʱ����ʼֵ,2msһ���ж�	��CPUƵ�ʸı�Ҳ��Ӱ��

u16 PulseWidthRatio=15;    //30v��ʼֵ=>15---(8K)/40v��ʼֵ=>20---(8K)


//u16 PulseWidthRatio=21;   //30v=>21 ��40v=>31


//#define  WAVE_1M
//#define  WAVE_2M      
#define  WAVE_100K

extern u8 Finish_OneMission_falg;


extern Bool Start_Collect_Flag;

extern Bool Send_Data_Host_Flag;


void Init_Timer0()
{
  // Select TIMER0 parameters   
	TIMER_Init_TypeDef timerInit =
	{
		.enable     = true, 
		.debugRun   = true, 
		.prescale   = timerPrescale1024, 
		.clkSel     = timerClkSelHFPerClk, 
		.fallAction = timerInputActionNone, 
		.riseAction = timerInputActionNone, 
		.mode       = timerModeDown, 		   //����ģʽ
		.dmaClrAct  = false,
		.quadModeX4 = false, 
		.oneShot    = false, 
		.sync       = false, 
	};
	
	// Enable overflow interrupt 
	TIMER_IntEnable(TIMER0, TIMER_IF_UF);	  //ʹ����������ж�
	
	// Enable TIMER0 interrupt vector in NVIC 
	NVIC_EnableIRQ(TIMER0_IRQn);
	
	// Set TIMER Top value 
	TIMER_TopSet(TIMER0, TIMER0_TOP_VAL);
	
	// Configure TIMER
	TIMER_Init(TIMER0, &timerInit);


   /* TIMER_Init_TypeDef tTimerInit =
    {
        .enable     = false,                                            // ��ʼ��������ʹ��             
        .debugRun   = false,                                            // ����Haltʱֹͣ����          
        .prescale   = timerPrescale1,                                   // ��Ƶϵ��Ϊ1                 
        .clkSel     = timerClkSelHFPerClk,                              // ʱ��Դѡ��HFPERCLK          
        .fallAction = timerInputActionNone,
        .riseAction = timerInputActionNone,
        .mode       = timerModeUp,                                      // ���ϼ���ģʽ                
        .dmaClrAct  = false,                                            
        .quadModeX4 = false,
        .oneShot    = false,                                           
        .sync       = false,                                            // �����������ʱ��ͬ��        
    };
    
      //TIMER0�Ƚ�ͨ��0��ʼ���ṹ�壬�������벶�񲿷�ģ�鹦��
   
    TIMER_InitCC_TypeDef tTimerCCInit = 
    {
        .eventCtrl  = timerEventEveryEdge,                              // �����¼���Ӧ����             
        .edge       = timerEdgeRising,                                  // ���ñ��ؼ���������ı���   
        .prsSel     = timerPRSSELCh2,                                   // PRSͨ��ѡ��                  
        .cufoa      = timerOutputActionNone,                            // ���ö�ʱ������ʱ�������Ϊ   
        .cofoa      = timerOutputActionNone,                            // ���ö�ʱ������ʱ�������Ϊ   
        .cmoa       = timerOutputActionNone,                            // ����ƥ��ʱ�������Ϊ         
        .mode       = timerCCModePWM,                                   // ����PWM���ģʽ              
        .filter     = true,                                             // ���������˲�                 
        .prsInput   = false,                                            // ѡ�����������               
        .coist      = false,                                           
        .outInvert  = false,                                            // �������ת                  
    };
    
    TIMER_Init(TIMER0, &tTimerInit);                                    // ��ʼ����ʱ��                
    TIMER_InitCC(TIMER0, 0, &tTimerCCInit);                             // ��ʼ������ͨ��2 ,�ڶ��������ǣ�CC0ͨ��
                                                                         //    {4,5,6,7,8,9,10,11,11,12,13,14,15,16,17,18,19,20};
	TIMER_TopSet(TIMER0, 1600);           //20K                             //    {1600,1516,1426,1334,1233,1128,1010,862,549,511,472,430,387,340,280,194,102,20};
	TIMER_CompareBufSet(TIMER0, 0,1600);                                //�ڶ��������ǣ�CC0ͨ��

    TIMER0->ROUTE |= TIMER_ROUTE_CC0PEN | TIMER_ROUTE_LOCATION_LOC1;   //����PWM CCO���ʹ�� 
    TIMER_Enable(TIMER0,true);	                                       //�رն�ʱ��0    
        */
}


/***************************************************************************//**
 * @brief   TIMER0�жϴ�����
							
 * @param[in] 
 * @return   
 ******************************************************************************/
void TIMER0_IRQHandler(void)
{ 
	// Clear flag for TIMER0 overflow interrupt 
	static u32 count=0;
	TIMER_IntClear(TIMER0, TIMER_IF_UF);
	/*if(Finish_OneMission_falg)
	{
	  if(count++>=300)
 	  {
	     Finish_OneMission_falg=0;
	     Start_Collect_Flag=1;
	     count=0;
		 Open_Led();
         TIMER_Enable(TIMER0,false);	  //�رն�ʱ��0
	  }
	}*/
	 if((++count)>=system_param.SendTime)
     	{
     	  count=0;
     	  Send_Data_Host_Flag=0x01;
     	}
    //   uartTxByte(0x34);
}


void   Init_Timer1()
{
     /*
     *  TIMER2��������ʼ���ṹ��
     */
    TIMER_Init_TypeDef tTimerInit =
    {
        .enable     = false,                                            /* ��ʼ��������ʹ��             */
        .debugRun   = false,                                            /* ����Haltʱֹͣ����           */
        .prescale   = timerPrescale1,                                   /* ��Ƶϵ��Ϊ1                  */
        .clkSel     = timerClkSelHFPerClk,                              /* ʱ��Դѡ��HFPERCLK           */
        .fallAction = timerInputActionNone,
        .riseAction = timerInputActionNone,
        .mode       = timerModeUp,                                      /* ���ϼ���ģʽ                 */
        .dmaClrAct  = false,                                            
        .quadModeX4 = false,
        .oneShot    = false,                                           
        .sync       = false,                                            /* �����������ʱ��ͬ��         */
    };
    /*
     *  TIMER0�Ƚ�ͨ��0��ʼ���ṹ�壬�������벶�񲿷�ģ�鹦��
     */
    TIMER_InitCC_TypeDef tTimerCCInit = 
    {
        .eventCtrl  = timerEventEveryEdge,                              /* �����¼���Ӧ����             */
        .edge       = timerEdgeRising,                                  /* ���ñ��ؼ���������ı���   */
        .prsSel     = timerPRSSELCh0,                                   /* PRSͨ��ѡ��                  */
        .cufoa      = timerOutputActionNone,                            /* ���ö�ʱ������ʱ�������Ϊ   */
        .cofoa      = timerOutputActionNone,                            /* ���ö�ʱ������ʱ�������Ϊ   */
        .cmoa       = timerOutputActionNone,                            /* ����ƥ��ʱ�������Ϊ         */
        .mode       = timerCCModePWM,                                   /* ����PWM���ģʽ              */
        .filter     = true,                                             /* ���������˲�                 */
        .prsInput   = false,                                            /* ѡ�����������               */
        /*
         *  ��λ���ó�ʼ�����ƽ��ֻ�������ƥ���PWMģʽ����PWMģʽ���۸�λ��Ϊ��ֵ������ʱ�����Ϊ0��
         *  ����ʹ��ʱ��λ���������ʼ��ƽ�������ƥ��ģʽ����λ����λ�����ʱ����ߵ�ƽ������λ����
         *  ���෴
         */
        .coist      = false,                                           
        .outInvert  = false,                                            /* �������ת                   */
    };
    
    TIMER_Init(TIMER1, &tTimerInit);                                    /* ��ʼ����ʱ��                 */
    TIMER_InitCC(TIMER1, 0, &tTimerCCInit);                             /* ��ʼ������ͨ��0 ,�ڶ��������ǣ�CC0ͨ��*/
     /* ����PWMģʽ��PWM���ڵ��� TOP+1������PWM����
        ����PWM����ߵ�ƽ�ĳ���ʱ��Ϊ CCV��ֵ������PWMռ�ձ�Ϊ50%*/
#ifdef WAVE_2M
    TIMER_TopSet(TIMER1, 16-1); 
    TIMER_CompareBufSet(TIMER1, 0, 7);  //�ڶ��������ǣ�CC0ͨ��
#else
   #ifdef WAVE_1M
	 TIMER_TopSet(TIMER1, 31-1); 
	 TIMER_CompareBufSet(TIMER1,0, 16);  //�ڶ��������ǣ�CC0ͨ��
   #else
    //TIMER_TopSet(TIMER1, 320-1); 
	//TIMER_CompareBufSet(TIMER1, 0, 160);  //�ڶ��������ǣ�CC0ͨ��
	TIMER_TopSet(TIMER1, 4000-1);    // 160 Ϊ200k  32000Ϊ1K  6400Ϊ8K
	TIMER_CompareBufSet(TIMER1, 0, PulseWidthRatio);  //�ڶ��������ǣ�CC0ͨ��
   #endif
#endif
    TIMER1->ROUTE |= TIMER_ROUTE_CC0PEN | TIMER_ROUTE_LOCATION_LOC0;   /*����PWM CCO���ʹ��   */  
    TIMER_Enable(TIMER1,true);	  //������ʱ��0    
}


void Init_Timer2(void)
{
/* Select TIMER0 parameters */	
   TIMER_Init_TypeDef timer2Init =
   {
	   .enable	   = false, 
	   .debugRun   = true, 
	   .prescale   = timerPrescale1, 			//1��Ƶ
	   .clkSel	   = timerClkSelHFPerClk, 
	   .fallAction = timerInputActionNone, 
	   .riseAction = timerInputActionNone, 
	   .mode	   = timerModeUp, 		  //����ģʽ
	   .dmaClrAct  = false,
	   .quadModeX4 = false, 
	   .oneShot    = false, 
	   .sync	   = false, 
   };
   
   /* Enable overflow interrupt */
   TIMER_IntEnable(TIMER2, TIMER_IF_UF);	 //ʹ����������ж�
   
   /* Enable TIMER0 interrupt vector in NVIC */
   //NVIC_EnableIRQ(TIMER2_IRQn);
   
   /* Set TIMER Top value */
   TIMER_TopSet(TIMER2, TIMER2_TOP_VAL);
   
   /* Configure TIMER */
   TIMER_Init(TIMER2, &timer2Init);

   PRS_SourceSignalSet(1, PRS_CH_CTRL_SOURCESEL_TIMER2, PRS_CH_CTRL_SIGSEL_TIMER2OF, prsEdgePos);

}

/***************************************************************************//**
 * @brief   TIMER0�жϴ�����
							
 * @param[in] 
 * @return   
 ******************************************************************************/
//void TIMER2_IRQHandler(void)
//{ 
	/* Clear flag for TIMER0 overflow interrupt */
	//TIMER_IntClear(TIMER2, TIMER_IF_UF);
    //ADC_Start(ADC0, adcStartSingle); 

	//TIMER2->IFC=TIMER_IF_UF;
	//ADC0->CMD=(uint32_t)adcStartSingle;
//}


