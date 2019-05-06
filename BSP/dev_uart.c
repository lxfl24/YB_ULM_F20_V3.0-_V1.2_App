#include "dev_uart.h"
#include "dev_wdog.h"
#include "em_usart.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "dev_systick.h"


UartBuffer Rxbuffer={0};
UartBuffer Txbuffer={0};

bool UartSendEnd=false;   //������ɱ�־
/*********************************************************************************************************
** Function name:      uartInit
** Descriptions:       ʵ��LEUART�ĳ�ʼ��
** input parameters:   none
** output parameters:  none
** Returned value:     none
** Created by:         chenhong
** Created Date:       2018-05-16
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void Initialize_Uart (u32 Baudrate)
{

  if(Baudrate==0x01)    Baudrate=9600;
  else if(Baudrate==0x02) Baudrate=19200;
  else if(Baudrate==0x03) Baudrate=115200;
  else   Baudrate=128000;
  USART_InitAsync_TypeDef uartInit;
  /* Prepare struct for initializing UART in asynchronous mode*/
  uartInit.enable       = usartDisable;   /* Don't enable UART upon intialization */
  uartInit.refFreq      = 0;              /* Provide information on reference frequency. When set to 0, the reference frequency is */
  uartInit.baudrate     = Baudrate;         /* Baud rate */
  uartInit.oversampling = usartOVS16;     /* Oversampling. Range is 4x, 6x, 8x or 16x */
  uartInit.databits     = usartDatabits8; /* Number of data bits. Range is 4 to 10 */
  uartInit.parity       = usartNoParity;  /* Parity mode */
  uartInit.stopbits     = usartStopbits1; /* Number of stop bits. Range is 0 to 2 */
  //uartInit.mvdis        = false;          /* Disable majority voting */
  //uartInit.prsRxEnable  = false;          /* Enable USART Rx via Peripheral Reflex System */
  //uartInit.prsRxCh      = usartPrsRxCh0;  /* Select PRS channel if enabled */

  /* Initialize USART with uartInit struct */
  USART_InitAsync(USART1, &uartInit);

  /* Prepare UART Rx and Tx interrupts */
  USART_IntClear(USART1, _USART_IF_MASK);
  USART_IntEnable(USART1, USART_IF_RXDATAV);
  NVIC_ClearPendingIRQ(USART1_RX_IRQn);
  NVIC_ClearPendingIRQ(USART1_TX_IRQn);
  NVIC_EnableIRQ(USART1_RX_IRQn);
  NVIC_EnableIRQ(USART1_TX_IRQn);

  /* Enable signals TX, RX *//* Enable I/O pins at UART0 location #0 */
  USART1->ROUTE |= USART_ROUTE_TXPEN | USART_ROUTE_RXPEN |USART_ROUTE_LOCATION_LOC0;

  /* Enable UART */
  USART_Enable(USART1, usartEnable);
  
  Txbuffer.head=0;
  Txbuffer.tail=0;

  Rxbuffer.head=0;
  Rxbuffer.tail=0;
}

/*********************************************************************************************************
** Function name:      uartTxByte
** Descriptions:       ���͵����ֽ�
** input parameters:   ucData����Ҫ���͵�����
** output parameters:  none
** Returned value:     none
** Created by:         chenhong
** Created Date:       2018-05-16
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void uartTxByte (u8 ucData)
{
    Enable_Tx_Disable_Rx();
    while (!(USART1->STATUS & USART_STATUS_TXBL));                  /* ��鷢�ͻ������Ƿ�Ϊ��       */

    USART1->TXDATA = (unsigned int)ucData;
    Service_Dog();
    Delay_ms(10);
    Enable_Rx_Disable_Tx();
}

/*********************************************************************************************************
** Function name:      uartRxByte
** Descriptions:       ���յ����ֽ�
** input parameters:   none
** output parameters:  none
** Returned value:     ���յ��ĵ����ֽ�
** Created by:         chenhong
** Created Date:       2018-05-16
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
u8 uartRxByte (void)
{
    while (!(USART1->STATUS & USART_STATUS_RXDATAV));               /* ��ѯ�Ƿ���յ�����           */

    return (u8)(USART1->RXDATA);
}

/*********************************************************************************************************
** Function name:      uartPrintString
** Descriptions:       �����ַ���
** input parameters:   none
** output parameters:  none
** Returned value:     none
** Created by:         chenhong
** Created Date:       2018-05-16
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void Uart_TxString(u8 *pcString,u8 len)
{
	  u8 i=0;
	  UartSendEnd=false;
	   Enable_Tx_Disable_Rx();
	  //�������
	  for(i=0;i<len;i++)
	  {
		 Txbuffer.Buffer[Txbuffer.head++]=*(pcString+i);
		 if(Txbuffer.head>=BUFFER_MAXSIZE)
		 {
			Txbuffer.head=0;
		 }
	  }
	  USART_IntEnable(USART1, USART_IF_TXBL);
}

void Uart_SendString(u8 *pcString,u8 len)
{
	u8 i=0;
    Enable_Tx_Disable_Rx();
	for(i=0;i<len;i++)
		{
	     while (!(USART1->STATUS & USART_STATUS_TXBL));                  /* ��鷢�ͻ������Ƿ�Ϊ��       */
	     USART1->TXDATA = (unsigned int)(*(pcString+i));
		}
    Service_Dog();
    Delay_ms(10);
    Enable_Rx_Disable_Tx();
}

/*********************************************************************************************************
** Function name:      USART1_RX_IRQHandler
** Descriptions:       USART1�����жϷ�����
** input parameters:   none
** output parameters:  none
** Returned value:     none
** Created by:         chenhong
** Created Date:       2018-05-16
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void USART1_RX_IRQHandler (void)
{

      //���ڽ���������Ч�ж���λ����Ҫ�ֶ������ֻ��Ҫ���������ݱ���ȡ����Զ���Ӳ�����
	  /* Check for RX data valid interrupt */
	  if (USART1->STATUS & USART_STATUS_RXDATAV)
	  {
		if(Rxbuffer.head>=BUFFER_MAXSIZE)
		{
			Rxbuffer.head=0;
		}
	    /* Copy data into RX Buffer */
		Rxbuffer.Buffer[Rxbuffer.head++]=USART_Rx(USART1);
	    /* Clear RXDATAV interrupt */
	    USART_IntClear(USART1, USART_IF_RXDATAV);
	    USART_IntClear(USART1, _USART_IF_MASK);
	  }
}


Bool Get_One_Char(u8 *Chr)
{
  if(Rxbuffer.tail==Rxbuffer.head) return 0;
  if(Rxbuffer.tail>=BUFFER_MAXSIZE)
   {
 	  Rxbuffer.tail=0;
   }
  *Chr=(u8)Rxbuffer.Buffer[Rxbuffer.tail];
  Rxbuffer.Buffer[Rxbuffer.tail++]=0x00;

  return 1;
}

Bool Get_OneChar_Delay(u8 *Chr)
{
  u16 cnt=0;
  while(cnt++<10)
  {
	  if(Get_One_Char(Chr)) break;
	  Delay_ms(1);
  }
  if(cnt>=10)
	  return 0;
  return 1;
}


/*********************************************************************************************************
** Function name:      USART1_TX_IRQHandler
** Descriptions:         USART1�����жϷ�����
** input parameters:   none
** output parameters:  none
** Returned value:     none
** Created by:         chenhong
** Created Date:       2018-05-16
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void USART1_TX_IRQHandler(void)
{

  //USART_IntGet(USART1);

  /* Check TX buffer level status */
  if (USART1->STATUS & USART_STATUS_TXBL)
  {
      /* Transmit pending character */
      if (Txbuffer.tail>=Txbuffer.head||UartSendEnd==true)//�жϹ��˻��ܽ������ٹ�
      {
    	  Txbuffer.head=0;
	      Txbuffer.tail=0;
		   Service_Dog();
		      Delay_ms(20);
		  USART_IntDisable(USART1, USART_IF_TXBL);
          UartSendEnd=true;
		 Enable_Rx_Disable_Tx();
		
      }
      else if(UartSendEnd==false&&Txbuffer.tail<Txbuffer.head)
      {
          USART_Tx(USART1,Txbuffer.Buffer[Txbuffer.tail]);
          Txbuffer.tail++;
		  if(Txbuffer.tail>=BUFFER_MAXSIZE)
		  {
				Txbuffer.tail=0;
		  }
		
      }
  }
  USART_IntClear(USART1, _USART_IF_MASK);
  USART_IntClear(USART1, USART_IF_TXBL);
}

