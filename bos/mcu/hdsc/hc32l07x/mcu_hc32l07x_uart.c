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

#if (defined(HC32L07X))

//      Register Address
// 第一个串口UART0; 对应 B_HAL_UART_1
#define UART0_BASE_ADDR (0x40000000)
#define UART1_BASE_ADDR (0x40000100)
#define UART2_BASE_ADDR (0x40006000)
#define UART3_BASE_ADDR (0x40006400)

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

typedef enum McuUartIrqSel
{
    MCU_UartRxIrq  = 0u,  ///< 接收中断使能
    MCU_UartTxIrq  = 1u,  ///< 发送中断使能
    MCU_UartTxEIrq = 8u,  ///< TX空中断使能
} McuUartIrqSel_t;

#define MCU_UART0 ((McuUartReg_t *)UART0_BASE_ADDR)
#define MCU_UART1 ((McuUartReg_t *)UART1_BASE_ADDR)
#define MCU_UART2 ((McuUartReg_t *)UART2_BASE_ADDR)
#define MCU_UART3 ((McuUartReg_t *)UART3_BASE_ADDR)

static McuUartReg_t *UartTable[4] = {MCU_UART0, MCU_UART1, MCU_UART2, MCU_UART3};

int bMcuUartSend(bHalUartNumber_t uart, const uint8_t *pbuf, uint16_t len)
{
    int           i       = 0;
    int           timeout = 0x000B0000;
    McuUartReg_t *pUart   = NULL;
    if (uart > B_HAL_UART_3 || pbuf == NULL)
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

int bMcuReceive(bHalUartNumber_t uart, uint8_t *pbuf, uint16_t len)
{
    int           i     = 0;
    McuUartReg_t *pUart = NULL;
    if (uart > B_HAL_UART_3 || pbuf == NULL)
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

int bMcuUartEnableRXIrq(bHalUartNumber_t uart)
{
    McuUartReg_t *pUart = NULL;
    if (uart > B_HAL_UART_3)
    {
        return -1;
    }
    pUart = UartTable[uart];
    pUart->SCON |= ((1UL) << (MCU_UartRxIrq));
    return 0;
}

int bMcuUartDisableRXIrq(bHalUartNumber_t uart)
{
    McuUartReg_t *pUart = NULL;
    if (uart > B_HAL_UART_3)
    {
        return -1;
    }
    pUart = UartTable[uart];
    pUart->SCON &= (~(1UL << (MCU_UartRxIrq)));
    return 0;
}

int bMcuUartEnableTXEIrq(bHalUartNumber_t uart)
{
    McuUartReg_t *pUart = NULL;
    if (uart > B_HAL_UART_3)
    {
        return -1;
    }
    pUart = UartTable[uart];
    pUart->SCON |= ((1UL) << (MCU_UartTxEIrq));
    return 0;
}

int bMcuUartDisableTXEIrq(bHalUartNumber_t uart)
{
    McuUartReg_t *pUart = NULL;
    if (uart > B_HAL_UART_3)
    {
        return -1;
    }
    pUart = UartTable[uart];
    pUart->SCON &= (~(1UL << (MCU_UartTxEIrq)));
    return 0;
}

#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
