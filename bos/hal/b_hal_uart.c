/**
 *!
 * \file        b_hal_uart.c
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
#include "hal/inc/b_hal.h"

/**
 * \addtogroup B_HAL
 * \{
 */

/**
 * \addtogroup UART
 * \{
 */

/**
 * \addtogroup UART_Exported_Functions
 * \{
 */

__WEAKDEF int bMcuUartSend(bHalUartNumber_t uart, const uint8_t *pbuf, uint16_t len)
{
    return -1;
}

__WEAKDEF int bMcuReceive(bHalUartNumber_t uart, uint8_t *pbuf, uint16_t len)
{
    return -1;
}

__WEAKDEF int bMcuUartEnableRXIrq(bHalUartNumber_t uart)
{
    return -1;
}
__WEAKDEF int bMcuUartDisableRXIrq(bHalUartNumber_t uart)
{
    return -1;
}
__WEAKDEF int bMcuUartEnableTXEIrq(bHalUartNumber_t uart)
{
    return -1;
}
__WEAKDEF int bMcuUartDisableTXEIrq(bHalUartNumber_t uart)
{
    return -1;
}


//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

int bHalUartSend(bHalUartNumber_t uart, const uint8_t *pbuf, uint16_t len)
{
    return bMcuUartSend(uart, pbuf, len);
}


int bHalReceive(bHalUartNumber_t uart, uint8_t *pbuf, uint16_t len)
{
    return bMcuReceive(uart, pbuf, len);
}


int bHalUartEnableRXIrq(bHalUartNumber_t uart)
{
	return 0;
}

int bHalUartDisableRXIrq(bHalUartNumber_t uart)
{
	return 0;
}

int bHalUartEnableTXEIrq(bHalUartNumber_t uart)
{
	return 0;
}

int bHalUartDisableTXEIrq(bHalUartNumber_t uart)
{
	return 0;
}



/**
 * \}
 */

/**
 * \}
 */

/**
 * \}
 */

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
