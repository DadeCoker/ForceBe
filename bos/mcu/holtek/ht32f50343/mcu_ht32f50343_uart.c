/**
 *!
 * \file        mcu_ht32f50343_uart.c
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

#if (defined(TH32F50343))

//      Register Address
#define UART1_BASE_ADDR (0x40001000)
#define UART2_BASE_ADDR (0x40041000)

typedef struct
{
    volatile uint32_t DR;  /*!< 0x000         Data Register  */
    volatile uint32_t CR;  /*!< 0x004         Control Register  */
    volatile uint32_t FCR; /*!< 0x008         FIFO Control Register */
    volatile uint32_t IER; /*!< 0x00C         Interrupt Enable Register */
    volatile uint32_t SR;  /*!< 0x010         Status Register  */
    volatile uint32_t TPR; /*!< 0x014         Timing Parameter Register */
    volatile uint32_t ICR; /*!< 0x018         IrDA COntrol Register */
    volatile uint32_t RCR; /*!< 0x01C         RS485 Control Register */
    volatile uint32_t SCR; /*!< 0x020         Synchronous Control Register */
    volatile uint32_t DLR; /*!< 0x024         Divisor Latch Register */
    volatile uint32_t DTR; /*!< 0x028         Debug/Test Register */
} McuUartReg_t;

#define MCU_UART1 ((McuUartReg_t *)UART1_BASE_ADDR)
#define MCU_UART2 ((McuUartReg_t *)UART2_BASE_ADDR)

static McuUartReg_t *UartTable[2] = {MCU_UART1, MCU_UART2};

int bMcuUartSend(bHalUartNumber_t uart, const uint8_t *pbuf, uint16_t len)
{
    int           i       = 0;
    int           timeout = 0x000B0000;
    McuUartReg_t *pUart   = NULL;
    if ((uart > B_HAL_UART_2) || pbuf == NULL)
    {
        return -1;
    }

    pUart = UartTable[uart];

    for (i = 0; i < len; i++)
    {
        timeout = 0x000B0000;
        while (timeout > 0 && ((pUart->SR & (0x1 << 8)) == 0))
        {
            timeout--;
        }
        if (timeout <= 0)
        {
            return -2;
        }
        pUart->DR = pbuf[i];
    }
    return len;
}

int bMcuReceive(bHalUartNumber_t uart, uint8_t *pbuf, uint16_t len)
{
    int           i       = 0;
    int           timeout = 0x000B0000;
    McuUartReg_t *pUart   = NULL;
    if ((uart > B_HAL_UART_2) || pbuf == NULL)
    {
        return -1;
    }

    pUart = UartTable[uart];

    for (i = 0; i < len; i++)
    {
        timeout = 0x000B0000;
        while (timeout > 0 && ((pUart->SR & (0x1 << 5)) == 0))
        {
            timeout--;
        }
        if (timeout <= 0)
        {
            return -2;
        }
        pbuf[i] = pUart->DR;
    }
    return len;
}

#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
