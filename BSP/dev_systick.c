#include "em_chip.h"
#include "em_cmu.h"
#include "dev_systick.h"

//volatile u32 msTicks=0; /* counts 1ms timeTicks */
static u16  fac_us=0;   //us��ʱ������
static u16 fac_ms=0;   //ms��ʱ������

/******************************************************************************
* ������  : SysTick_Handler
* ����    : ϵͳʱ���жϷ�����
* ����    : ��
* ���    : -msTicks
* ����    : ��
* ����    : �ⲿ����
******************************************************************************/
/*void SysTick_Handler(void)
{
    msTicks++;       // increment counter necessary in Delay()
}*/

/*******************************************************************************
 * �� �� ��         : Delay_us
 * ��������           : ��ʱ��������ʱus
 * ��    ��         : i
 * ��    ��         : ��
 *******************************************************************************/
 void Delay_us(u32 nus)
  {
     SysTick->LOAD=fac_us*nus;                    //������װ��ֵ, //��ʱ10us�Ļ�����  10*nus,װ��load�Ĵ����� 
     SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;     //ʹ��
     SysTick->VAL=0;                //���������//��������0,��Ϊcurrrent�ֶα��ֶ�����ʱ,load���Զ���װ��VAL��  
     while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));   //�ȴ�ʱ�䵽��
     SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;   //�رռ�����
     SysTick->VAL=0;        //��ռ�����
  }


/******************************************************************************
* ������  : Delay_ms
* ����    : �ӳٺ���
* ����    :nms �ӳٵĽ�����
* ���    : ��
* ����    : ��
* ����    : �ⲿ����
******************************************************************************/
void Delay_ms(u32 nms)
{
  #if 0
	u32 curTicks;
    curTicks = msTicks;
    while ((msTicks - curTicks) < nms) ;
 #else 
	SysTick->LOAD=fac_ms*nms;      //������װ��ֵ,        
	SysTick->CTRL|= SysTick_CTRL_ENABLE_Msk;       //ʹ��                                                      
	SysTick->VAL=0;                                           //���������
	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));    //�ȴ�ʱ�䵽��
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;               //�رռ�����
	SysTick->VAL=0;                                          //��ռ�����
 #endif
}

/******************************************************************************
* ������  : Delay_Cycle
* ����    : �ӳٺ���
* ����    : -dlyTicks �ӳٵĽ�����
* ���    : -msTicks  ֵ�����޸�
* ����    : ��
* ����    : �ⲿ����
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
* ������  : Delay
* ����    : �ӳٺ���
* ����    : -dlyTicks �ӳٵĽ�����
* ���    : -msTicks  ֵ�����޸�
* ����    : ��
* ����    : �ⲿ����
******************************************************************************/
void Init_SysTick(void)
{
  #if 0
	/* Setup SysTick Timer for 1 msec interrupts  */
	if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1) ;
 #else 
   SystemCoreClockUpdate();
   fac_us=SystemCoreClock/1000000;
//������SysTickʱ��Ƶ��==HCLK/8==SystemCoreClock/8����ˣ�SysTickһ������ʱ��
//(1/(SystemCoreClock/8))��1us��Ҫ��������Ϊ
//10^-6��[1/(SystemCoreClock/8)]=SystemCoreClock/8000000��
  fac_ms=SystemCoreClock/1000;
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
  SysTick->CTRL &= ~(SysTick_CTRL_COUNTFLAG_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);

 #endif
}

