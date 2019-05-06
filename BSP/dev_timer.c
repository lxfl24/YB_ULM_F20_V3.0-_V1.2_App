#include "dev_timer.h"
#include "em_timer.h"
#include "em_cmu.h"
#include "em_prs.h"
#include "dev_uart.h"
#include "dev_ADC.h"
#include "dev_gpio.h"
#include "YB_Connect.h"


//我的电路中外部晶振为32M，定时器设置时选择8分频，所以定时器的时钟频率为32M/8=4MHz;
//TIMER0_TOP_VAL(4)*（1/4M)=1us，即定时周期为1us，用示波器测试，驱动脉冲宽度为1us。

#define  INTERVAL_PER_SEC0   1		       //定时器0每1s中断一次		               
#define  TIMER0_TOP_VAL  	 ((CMU_ClockFreqGet(cmuClock_HFPER)/1024)/INTERVAL_PER_SEC0)	 //定时器初始值,2ms一次中断	，CPU频率改变也不影响

#define  INTERVAL_PER_SEC2   400000	                 //定时器0每2.5us中断一次 //5000002us		               
#define  TIMER2_TOP_VAL  	 ((CMU_ClockFreqGet(cmuClock_HFPER))/INTERVAL_PER_SEC2)	 //定时器初始值,2ms一次中断	，CPU频率改变也不影响

u16 PulseWidthRatio=15;    //30v初始值=>15---(8K)/40v初始值=>20---(8K)


//u16 PulseWidthRatio=21;   //30v=>21 、40v=>31


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
		.mode       = timerModeDown, 		   //下溢模式
		.dmaClrAct  = false,
		.quadModeX4 = false, 
		.oneShot    = false, 
		.sync       = false, 
	};
	
	// Enable overflow interrupt 
	TIMER_IntEnable(TIMER0, TIMER_IF_UF);	  //使能向下溢出中断
	
	// Enable TIMER0 interrupt vector in NVIC 
	NVIC_EnableIRQ(TIMER0_IRQn);
	
	// Set TIMER Top value 
	TIMER_TopSet(TIMER0, TIMER0_TOP_VAL);
	
	// Configure TIMER
	TIMER_Init(TIMER0, &timerInit);


   /* TIMER_Init_TypeDef tTimerInit =
    {
        .enable     = false,                                            // 初始化结束不使能             
        .debugRun   = false,                                            // 调试Halt时停止计数          
        .prescale   = timerPrescale1,                                   // 分频系数为1                 
        .clkSel     = timerClkSelHFPerClk,                              // 时钟源选择HFPERCLK          
        .fallAction = timerInputActionNone,
        .riseAction = timerInputActionNone,
        .mode       = timerModeUp,                                      // 向上计数模式                
        .dmaClrAct  = false,                                            
        .quadModeX4 = false,
        .oneShot    = false,                                           
        .sync       = false,                                            // 不开启多个定时器同步        
    };
    
      //TIMER0比较通道0初始化结构体，配置输入捕获部分模块功能
   
    TIMER_InitCC_TypeDef tTimerCCInit = 
    {
        .eventCtrl  = timerEventEveryEdge,                              // 配置事件响应边沿             
        .edge       = timerEdgeRising,                                  // 配置边沿检测器所检测的边沿   
        .prsSel     = timerPRSSELCh2,                                   // PRS通道选择                  
        .cufoa      = timerOutputActionNone,                            // 配置定时器下溢时的输出行为   
        .cofoa      = timerOutputActionNone,                            // 配置定时器上溢时的输出行为   
        .cmoa       = timerOutputActionNone,                            // 配置匹配时的输出行为         
        .mode       = timerCCModePWM,                                   // 配置PWM输出模式              
        .filter     = true,                                             // 禁能输入滤波                 
        .prsInput   = false,                                            // 选择从引脚输入               
        .coist      = false,                                           
        .outInvert  = false,                                            // 输出不翻转                  
    };
    
    TIMER_Init(TIMER0, &tTimerInit);                                    // 初始化定时器                
    TIMER_InitCC(TIMER0, 0, &tTimerCCInit);                             // 初始化捕获通道2 ,第二个参数是：CC0通道
                                                                         //    {4,5,6,7,8,9,10,11,11,12,13,14,15,16,17,18,19,20};
	TIMER_TopSet(TIMER0, 1600);           //20K                             //    {1600,1516,1426,1334,1233,1128,1010,862,549,511,472,430,387,340,280,194,102,20};
	TIMER_CompareBufSet(TIMER0, 0,1600);                                //第二个参数是：CC0通道

    TIMER0->ROUTE |= TIMER_ROUTE_CC0PEN | TIMER_ROUTE_LOCATION_LOC1;   //配置PWM CCO输出使能 
    TIMER_Enable(TIMER0,true);	                                       //关闭定时器0    
        */
}


/***************************************************************************//**
 * @brief   TIMER0中断处理函数
							
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
         TIMER_Enable(TIMER0,false);	  //关闭定时器0
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
     *  TIMER2计数器初始化结构体
     */
    TIMER_Init_TypeDef tTimerInit =
    {
        .enable     = false,                                            /* 初始化结束不使能             */
        .debugRun   = false,                                            /* 调试Halt时停止计数           */
        .prescale   = timerPrescale1,                                   /* 分频系数为1                  */
        .clkSel     = timerClkSelHFPerClk,                              /* 时钟源选择HFPERCLK           */
        .fallAction = timerInputActionNone,
        .riseAction = timerInputActionNone,
        .mode       = timerModeUp,                                      /* 向上计数模式                 */
        .dmaClrAct  = false,                                            
        .quadModeX4 = false,
        .oneShot    = false,                                           
        .sync       = false,                                            /* 不开启多个定时器同步         */
    };
    /*
     *  TIMER0比较通道0初始化结构体，配置输入捕获部分模块功能
     */
    TIMER_InitCC_TypeDef tTimerCCInit = 
    {
        .eventCtrl  = timerEventEveryEdge,                              /* 配置事件响应边沿             */
        .edge       = timerEdgeRising,                                  /* 配置边沿检测器所检测的边沿   */
        .prsSel     = timerPRSSELCh0,                                   /* PRS通道选择                  */
        .cufoa      = timerOutputActionNone,                            /* 配置定时器下溢时的输出行为   */
        .cofoa      = timerOutputActionNone,                            /* 配置定时器上溢时的输出行为   */
        .cmoa       = timerOutputActionNone,                            /* 配置匹配时的输出行为         */
        .mode       = timerCCModePWM,                                   /* 配置PWM输出模式              */
        .filter     = true,                                             /* 禁能输入滤波                 */
        .prsInput   = false,                                            /* 选择从引脚输入               */
        /*
         *  该位配置初始输出电平，只用于输出匹配和PWM模式。在PWM模式不论该位置为何值，禁能时输出都为0，
         *  但当使能时该位决定输出初始电平。在输出匹配模式，该位若置位则禁能时输出高电平，若该位清零
         *  则相反
         */
        .coist      = false,                                           
        .outInvert  = false,                                            /* 输出不翻转                   */
    };
    
    TIMER_Init(TIMER1, &tTimerInit);                                    /* 初始化定时器                 */
    TIMER_InitCC(TIMER1, 0, &tTimerCCInit);                             /* 初始化捕获通道0 ,第二个参数是：CC0通道*/
     /* 上升PWM模式的PWM周期等于 TOP+1，配置PWM周期
        配置PWM输出高电平的持续时间为 CCV的值，配置PWM占空比为50%*/
#ifdef WAVE_2M
    TIMER_TopSet(TIMER1, 16-1); 
    TIMER_CompareBufSet(TIMER1, 0, 7);  //第二个参数是：CC0通道
#else
   #ifdef WAVE_1M
	 TIMER_TopSet(TIMER1, 31-1); 
	 TIMER_CompareBufSet(TIMER1,0, 16);  //第二个参数是：CC0通道
   #else
    //TIMER_TopSet(TIMER1, 320-1); 
	//TIMER_CompareBufSet(TIMER1, 0, 160);  //第二个参数是：CC0通道
	TIMER_TopSet(TIMER1, 4000-1);    // 160 为200k  32000为1K  6400为8K
	TIMER_CompareBufSet(TIMER1, 0, PulseWidthRatio);  //第二个参数是：CC0通道
   #endif
#endif
    TIMER1->ROUTE |= TIMER_ROUTE_CC0PEN | TIMER_ROUTE_LOCATION_LOC0;   /*配置PWM CCO输出使能   */  
    TIMER_Enable(TIMER1,true);	  //开启定时器0    
}


void Init_Timer2(void)
{
/* Select TIMER0 parameters */	
   TIMER_Init_TypeDef timer2Init =
   {
	   .enable	   = false, 
	   .debugRun   = true, 
	   .prescale   = timerPrescale1, 			//1分频
	   .clkSel	   = timerClkSelHFPerClk, 
	   .fallAction = timerInputActionNone, 
	   .riseAction = timerInputActionNone, 
	   .mode	   = timerModeUp, 		  //下溢模式
	   .dmaClrAct  = false,
	   .quadModeX4 = false, 
	   .oneShot    = false, 
	   .sync	   = false, 
   };
   
   /* Enable overflow interrupt */
   TIMER_IntEnable(TIMER2, TIMER_IF_UF);	 //使能向下溢出中断
   
   /* Enable TIMER0 interrupt vector in NVIC */
   //NVIC_EnableIRQ(TIMER2_IRQn);
   
   /* Set TIMER Top value */
   TIMER_TopSet(TIMER2, TIMER2_TOP_VAL);
   
   /* Configure TIMER */
   TIMER_Init(TIMER2, &timer2Init);

   PRS_SourceSignalSet(1, PRS_CH_CTRL_SOURCESEL_TIMER2, PRS_CH_CTRL_SIGSEL_TIMER2OF, prsEdgePos);

}

/***************************************************************************//**
 * @brief   TIMER0中断处理函数
							
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


