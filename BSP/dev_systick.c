#include "em_chip.h"
#include "em_cmu.h"
#include "dev_systick.h"

//volatile u32 msTicks=0; /* counts 1ms timeTicks */
static u16  fac_us=0;   //us延时倍乘数
static u16 fac_ms=0;   //ms延时倍乘数

/******************************************************************************
* 函数名  : SysTick_Handler
* 描述    : 系统时钟中断服务函数
* 输入    : 无
* 输出    : -msTicks
* 返回    : 无
* 调用    : 外部调用
******************************************************************************/
/*void SysTick_Handler(void)
{
    msTicks++;       // increment counter necessary in Delay()
}*/

/*******************************************************************************
 * 函 数 名         : Delay_us
 * 函数功能           : 延时函数，延时us
 * 输    入         : i
 * 输    出         : 无
 *******************************************************************************/
 void Delay_us(u32 nus)
  {
     SysTick->LOAD=fac_us*nus;                    //设置重装数值, //延时10us的话就是  10*nus,装到load寄存器中 
     SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;     //使能
     SysTick->VAL=0;                //清零计数器//计数器清0,因为currrent字段被手动清零时,load将自动重装到VAL中  
     while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));   //等待时间到达
     SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;   //关闭计数器
     SysTick->VAL=0;        //清空计数器
  }


/******************************************************************************
* 函数名  : Delay_ms
* 描述    : 延迟函数
* 输入    :nms 延迟的节拍数
* 输出    : 无
* 返回    : 无
* 调用    : 外部调用
******************************************************************************/
void Delay_ms(u32 nms)
{
  #if 0
	u32 curTicks;
    curTicks = msTicks;
    while ((msTicks - curTicks) < nms) ;
 #else 
	SysTick->LOAD=fac_ms*nms;      //设置重装数值,        
	SysTick->CTRL|= SysTick_CTRL_ENABLE_Msk;       //使能                                                      
	SysTick->VAL=0;                                           //清零计数器
	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));    //等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;               //关闭计数器
	SysTick->VAL=0;                                          //清空计数器
 #endif
}

/******************************************************************************
* 函数名  : Delay_Cycle
* 描述    : 延迟函数
* 输入    : -dlyTicks 延迟的节拍数
* 输出    : -msTicks  值将被修改
* 返回    : 无
* 调用    : 外部调用
******************************************************************************/

void Delay_Cycle(unsigned long ulCycle)
{
    SysTick->LOAD = ulCycle;
    SysTick->VAL = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    
    while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}


/******************************************************************************
* 函数名  : Delay
* 描述    : 延迟函数
* 输入    : -dlyTicks 延迟的节拍数
* 输出    : -msTicks  值将被修改
* 返回    : 无
* 调用    : 外部调用
******************************************************************************/
void Init_SysTick(void)
{
  #if 0
	/* Setup SysTick Timer for 1 msec interrupts  */
	if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1) ;
 #else 
   SystemCoreClockUpdate();
   fac_us=SystemCoreClock/1000000;
//（，即SysTick时钟频率==HCLK/8==SystemCoreClock/8，因此，SysTick一个计数时长
//(1/(SystemCoreClock/8))，1us需要计数次数为
//10^-6÷[1/(SystemCoreClock/8)]=SystemCoreClock/8000000）
  fac_ms=SystemCoreClock/1000;
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
  SysTick->CTRL &= ~(SysTick_CTRL_COUNTFLAG_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);

 #endif
}

