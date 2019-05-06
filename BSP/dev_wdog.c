#include "dev_wdog.h"
#include "em_wdog.h"

void Init_wdog (void)
{
    /*
     * ���Ź���ʱ�����ýṹ�壬��ʹ�ÿ��Ź�ʱ�ýṹ������
     */
    WDOG_Init_TypeDef tWdogInit = {
        .enable     = true,                                             /* ʹ�ܿ��Ź�                   */
        .debugRun   = false,                                            /* ���Ź��ڵ���ʱ������         */
        .em2Run     = true,                                             /* ��EM2ģʽ���Ź���������      */
        .em3Run     = false,                                             /* ��EM3ģʽ���Ź���������      */
        .em4Block   = false,                                            /* ������������EM4����          */
        .swoscBlock = false,                                            /* ��������CMU�ر�WDOG��ѡ����*/
        .lock       = false,                                            /* ���������Ź�����             */
        .clkSel     = wdogClkSelULFRCO,                                 /* ѡ��1kHZ��Ϊ���Ź�ʱ��       */
        .perSel     = wdogPeriod_2k,                                 	/* ���ÿ��Ź�����Ϊ2049ʱ������ */
    };
    WDOG_Init(&tWdogInit);     
}

void Service_Dog()
{
     WDOG_Feed();                                                      /* ι�����忴�Ź���ʱ��������   */
}