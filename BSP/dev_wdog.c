#include "dev_wdog.h"
#include "em_wdog.h"

void Init_wdog (void)
{
    /*
     * 看门狗定时器配置结构体，在使用看门狗时该结构体有用
     */
    WDOG_Init_TypeDef tWdogInit = {
        .enable     = true,                                             /* 使能看门狗                   */
        .debugRun   = false,                                            /* 看门狗在调试时不计数         */
        .em2Run     = true,                                             /* 在EM2模式看门狗继续计数      */
        .em3Run     = false,                                             /* 在EM3模式看门狗继续计数      */
        .em4Block   = false,                                            /* 禁能阻塞进入EM4功能          */
        .swoscBlock = false,                                            /* 禁能阻塞CMU关闭WDOG所选振荡器*/
        .lock       = false,                                            /* 不锁定看门狗配置             */
        .clkSel     = wdogClkSelULFRCO,                                 /* 选择1kHZ作为看门狗时钟       */
        .perSel     = wdogPeriod_2k,                                 	/* 设置看门狗周期为2049时钟周期 */
    };
    WDOG_Init(&tWdogInit);     
}

void Service_Dog()
{
     WDOG_Feed();                                                      /* 喂狗，清看门狗定时器计数器   */
}