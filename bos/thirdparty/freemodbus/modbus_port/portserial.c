/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );

//static usart_type* port_uart_list[] = {USART2, USART3};
//static usart_type* port_uart;

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
    if(xRxEnable == TRUE)
    {
			Uart_EnableIrq(M0P_UART1,UartRxIrq); 
    }
    else
    {
			Uart_DisableIrq(M0P_UART1,UartRxIrq); 
    }
  
    if(xTxEnable == TRUE)
    {
			Uart_EnableIrq(M0P_UART1,UartTxEIrq);
    }
    else
    {
			Uart_DisableIrq(M0P_UART1,UartTxEIrq);
    }
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
	stc_gpio_cfg_t stcGpioCfg;
	DDL_ZERO_STRUCT(stcGpioCfg);
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); //

	///<TX
	stcGpioCfg.enDir = GpioDirOut;
	Gpio_Init(GpioPortA, GpioPin2, &stcGpioCfg);
	Gpio_SetAfMode(GpioPortA, GpioPin2, GpioAf1);          //

	///<RX
	stcGpioCfg.enDir = GpioDirIn;
	Gpio_Init(GpioPortA, GpioPin3, &stcGpioCfg);
	Gpio_SetAfMode(GpioPortA, GpioPin3, GpioAf1);          //
	
	
  BOOL            bStatus = FALSE;
	bStatus = TRUE;
	
	stc_uart_cfg_t    stcCfg;
	DDL_ZERO_STRUCT(stcCfg);
	Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,TRUE);
	///<UART Init
	stcCfg.enRunMode        = UartMskMode1;          	
	stcCfg.enStopBit        = UartMsk1bit;           	
	stcCfg.enMmdorCk        = UartMskEven;          	
	stcCfg.stcBaud.u32Baud  = ulBaudRate;             
	stcCfg.stcBaud.enClkDiv = UartMsk8Or16Div;       
	stcCfg.stcBaud.u32Pclk  = Sysctrl_GetPClkFreq(); 	
	Uart_Init(M0P_UART1, &stcCfg);                   	
	///<UART
	Uart_ClrStatus(M0P_UART1,UartRC);                	
	Uart_ClrStatus(M0P_UART1,UartTxe);           
//	Uart_EnableIrq(M0P_UART1,UartRxIrq);             	
//	Uart_EnableIrq(M0P_UART1,UartTxIrq);        
	EnableNvic(UART1_3_IRQn, IrqLevel3, TRUE);      
  
  return bStatus;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
		Uart_SendDataPoll(M0P_UART1,ucByte);
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
//    *pucByte = usart_data_receive(port_uart);
		*pucByte = Uart_ReceiveData(M0P_UART1);
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}


void Uart1_IRQHandler(void)
{
    if(Uart_GetStatus(M0P_UART1, UartRC))         
    {
        Uart_ClrStatus(M0P_UART1, UartRC);      
				prvvUARTRxISR();
		}
		if(Uart_GetStatus(M0P_UART1, UartTxe))         
    {
        Uart_ClrStatus(M0P_UART1, UartTxe); 
        prvvUARTTxReadyISR();
    }

}

//void vUSARTHandler( void )
//{
//    if(port_uart->ctrl1_bit.rdbfien == SET)
//    {
//      if(usart_flag_get(port_uart, USART_RDBF_FLAG) == SET)
//      {
//        prvvUARTRxISR();
//      }
//    }
//    
//    if(port_uart->ctrl1_bit.tdbeien == SET)
//    {
//      if(usart_flag_get(port_uart, USART_TDBE_FLAG) == SET)
//      {
//        prvvUARTTxReadyISR();
//      }
//    }
//}

//void USART2_IRQHandler(void)
//{
//  vUSARTHandler(  );
//}

//void USART3_IRQHandler(void)
//{
//  vUSARTHandler(  );
//}
