#include "dev_gpio.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "dev_systick.h"

void Init_All_Gpio(void)
{

  GPIO_DriveModeSet(gpioPortC, gpioDriveModeHigh);                    /* 设置驱动电流为默认大小       */
  
  GPIO_PinModeSet(gpioPortC, 14, gpioModePushPullDrive, 0);           /* 设置LED管脚为推挽输出        */
                                                                    

  GPIO_PinModeSet(gpioPortA, 0, gpioModePushPull, 0);                 //PWM 4_20MA
  
  GPIO_PinModeSet(gpioPortC, 0, gpioModePushPull, 1);                 //TXD
  GPIO_PinModeSet(gpioPortC, 1, gpioModeInput, 0);                    //RXD
  GPIO_PinModeSet(gpioPortC, 2, gpioModePushPull, 0);                 //EN

  GPIO_PinModeSet(gpioPortC, 3, gpioModePushPull, 1);               //-3V0EN
  
  GPIO_PinModeSet(gpioPortC, 13, gpioModePushPull, 0);              //PWM_Power输出
  GPIO_PinModeSet(gpioPortD, 4, gpioModePushPull, 0);               //驱动波形输出

  
  GPIO_PinModeSet(gpioPortD, 7, gpioModeInput,0);                   //温度输入
  GPIO_PinModeSet(gpioPortD, 6, gpioModeInputPull, 0);              //电压反馈输入    
  GPIO_PinModeSet(gpioPortD, 5, gpioModeInput,0);                  //回波信号输入
  
  
}


void Test_Gpio()
{
   GPIO_PinModeSet(gpioPortC, 15, gpioModePushPullDrive, 0);
   Delay_ms(1000);
   GPIO_PinModeSet(gpioPortC, 15, gpioModePushPullDrive, 1);
   Delay_ms(1000);
}