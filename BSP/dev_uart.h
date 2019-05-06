#ifndef  __DEV_UART_H
#define  __DEV_UART_H

#include "system.h"


#define BUFFER_MAXSIZE          200             //发送接收数据量最大数量

#define Enable_Rx_Disable_Tx()        GPIO->P[gpioPortC].DOUTCLR = 1 << 2;
#define Enable_Tx_Disable_Rx()        GPIO->P[gpioPortC].DOUTSET = 1 << 2;


typedef struct structBuffer
{
  u8  Buffer[BUFFER_MAXSIZE];    //数据
  u8  head;
  u8  tail;
  u8  Chr;
}UartBuffer;

extern  UartBuffer Rxbuffer;
extern  UartBuffer Txbuffer;

void Initialize_Uart(u32 Baudrate);

void uartTxByte(u8 ucData);

void Uart_SendString (u8 *pcString,u8 len);

Bool Get_One_Char(u8 *Chr);

Bool Get_OneChar_Delay(u8 *Chr);


#endif
