/**
 *!
 * \file        mcu_hc32l13x_uart.c
 * \version     v0.0.1
 * \date        2020/03/25
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2020 Bean
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SUARTL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */

/*Includes ----------------------------------------------*/
#include "b_config.h"
#include "hal/inc/b_hal_uart.h"

#if (_MCU_PLATFORM == 4001)

#ifndef NULL
#define NULL ((void *)0)
#endif

//      Register Address

#define UART1_BASE_ADDR (0x40000000)
#define UART2_BASE_ADDR (0x40000100)

typedef struct
{
    volatile uint32_t SBUF;
    volatile uint32_t SCON;
    volatile uint32_t SADDR;
    volatile uint32_t SADEN;
    volatile uint32_t ISR;
    volatile uint32_t ICR;    
    volatile uint32_t SCNT;  
} McuUartReg_t;

#define MCU_UART1 ((McuUartReg_t *)UART1_BASE_ADDR)
#define MCU_UART2 ((McuUartReg_t *)UART2_BASE_ADDR)

static McuUartReg_t *UartTable[2] = {MCU_UART1, MCU_UART2};

static int _UartSend(bHalUartNumber_t uart, const uint8_t *pbuf, uint16_t len)
{
    int           i       = 0;
    int           timeout = 0x000B0000;
    McuUartReg_t *pUart   = NULL;
    if (uart > B_HAL_UART_2 || pbuf == NULL)
    {
        return -1;
    }
    
    pUart = UartTable[uart];

    for (i = 0; i < len; i++)
    {
        timeout = 0x000B0000;
        while (timeout > 0 && ((pUart->ISR & (0x1 << 3)) == 0))
        {
            timeout--;
        }
        if (timeout <= 0)
        {
            return -2;
        }
        pUart->SBUF = pbuf[i];
        
        timeout = 0x000B0000;
        while (timeout > 0 && ((pUart->ISR & (0x1 << 1)) == 0))
        {
            timeout--;
        }
        if (timeout <= 0)
        {
            return -2;
        }
        pUart->ISR &= ~(0x1 << 1);
    }
    return len;
}

static int _UartReceive(bHalUartNumber_t uart, uint8_t *pbuf, uint16_t len)
{
    int           i       = 0;
    McuUartReg_t *pUart   = NULL;
    if (uart > B_HAL_UART_2 || pbuf == NULL)
    {
        return -1;
    }
    pUart = UartTable[uart];
    for (i = 0; i < len; i++)
    {
        pbuf[i] = pUart->SBUF;
    }
    return len;
}

bHalUartDriver_t bHalUartDriver = {
    .pSend    = _UartSend,
    .pReceive = _UartReceive,
};

#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
