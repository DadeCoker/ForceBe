/**
 *!
 * \file        b_util_uart.c
 * \version     v0.0.1
 * \date        2020/04/01
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */

/*Includes ----------------------------------------------*/
#include "utils/inc/b_util_uart.h"

#include <string.h>

#include "b_section.h"
#include "hal/inc/b_hal.h"

/**
 * \addtogroup B_UTILS
 * \{
 */

/**
 * \addtogroup UART
 * \{
 */

/**
 * \defgroup UART_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup UART_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup UART_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup UART_Private_Variables
 * \{
 */
static bUitlUartInstance_t  bUitlUartHead = {.next = NULL, .prev = NULL};
static bUitlUartInstance_t *pUtilUartBindTable[B_HAL_UART_NUMBER];
/**
 * \}
 */

/**
 * \defgroup UART_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup UART_Private_Functions
 * \{
 */

static void _UtilUartListAdd(bUitlUartInstance_t *pinstance)
{
    bUitlUartInstance_t *phead = &bUitlUartHead;
    for (;;)
    {
        if (phead->next == NULL)
        {
            phead->next     = pinstance;
            pinstance->prev = phead;
            pinstance->next = NULL;
            break;
        }
        else
        {
            phead = phead->next;
        }
    }
}

static void _bUtilUartDetectIdle()
{
    bUitlUartInstance_t *phead = &bUitlUartHead;
    for (;;)
    {
        if (phead->next != NULL)
        {
            if (phead->next->index > 0)
            {
                if (phead->next->l_index == 0)
                {
                    phead->next->l_index = phead->next->index;
                    phead->next->l_tick  = bHalGetSysTick();
                }
                else
                {
                    if (phead->next->index != phead->next->l_index)
                    {
                        phead->next->l_index = phead->next->index;
                        phead->next->l_tick  = bHalGetSysTick();
                    }
                    else if (bHalGetSysTick() - phead->next->l_tick > phead->next->idle_thd_ms)
                    {
                        if (phead->next->callback != NULL)
                        {
                            if ((phead->next->callback(phead->next->pbuf, phead->next->index,
                                                       phead->next->cb_arg) >= 0) ||
                                (phead->next->index == phead->next->buf_size))
                            {
                                memset(phead->next->pbuf, 0, phead->next->buf_size);
                                phead->next->index = 0;
                            }
                        }
                        phead->next->l_tick = bHalGetSysTick();
                    }
                }
            }
        }
        else
        {
            break;
        }
        phead = phead->next;
    }
}

BOS_REG_POLLING_FUNC(_bUtilUartDetectIdle);

/**
 * \}
 */

/**
 * \addtogroup UART_Exported_Functions
 * \{
 */

void bUtilUartBind(uint8_t uart_no, bUitlUartInstance_t *pinstance)
{
    static uint8_t bind_init_f = 0;
    if (bind_init_f == 0)
    {
        memset(pUtilUartBindTable, 0, sizeof(pUtilUartBindTable));
        bind_init_f = 1;
    }

    if (uart_no < B_HAL_UART_NUMBER && pinstance != NULL)
    {
        pUtilUartBindTable[uart_no] = pinstance;
        if (pinstance->prev == NULL)
        {
            _UtilUartListAdd(pinstance);
        }
    }
}

void bUtilUartRxHandler(bUitlUartInstance_t *pinstance, uint8_t dat)
{
    if (pinstance == NULL)
    {
        return;
    }
    if (pinstance->pbuf == NULL)
    {
        return;
    }
    if (pinstance->prev == NULL)
    {
        _UtilUartListAdd(pinstance);
    }
    if (pinstance->index < pinstance->buf_size)
    {
        pinstance->pbuf[pinstance->index] = dat;
        pinstance->index += 1;
    }
}

void bUtilUartRxHandler2(uint8_t uart_no, uint8_t dat)
{
    if (uart_no >= B_HAL_UART_NUMBER)
    {
        return;
    }
    if (pUtilUartBindTable[uart_no] == NULL)
    {
        return;
    }
    if (pUtilUartBindTable[uart_no]->pbuf == NULL)
    {
        return;
    }
    if (pUtilUartBindTable[uart_no]->index < pUtilUartBindTable[uart_no]->buf_size)
    {
        pUtilUartBindTable[uart_no]->pbuf[pUtilUartBindTable[uart_no]->index] = dat;
        pUtilUartBindTable[uart_no]->index += 1;
    }
}

uint16_t bUtilUartReceivedSize(bUitlUartInstance_t *pinstance)
{
    if (pinstance == NULL)
    {
        return 0;
    }
    return pinstance->index;
}

uint16_t bUtilUartReceivedSize2(uint8_t uart_no)
{
    if (uart_no >= B_HAL_UART_NUMBER)
    {
        return 0;
    }
    if (pUtilUartBindTable[uart_no] == NULL)
    {
        return 0;
    }
    return pUtilUartBindTable[uart_no]->index;
}

void bUtilUartInitStruct(bUitlUartInstance_t *pinstance, uint8_t *pbuf, uint16_t size,
                         uint32_t idle_ms, pbUartIdleCallback_t cb, void *arg)
{
    if (pinstance == NULL || pbuf == NULL || cb == NULL)
    {
        return;
    }
    pinstance->pbuf        = pbuf;
    pinstance->buf_size    = size;
    pinstance->idle_thd_ms = idle_ms;
    pinstance->callback    = cb;
    pinstance->cb_arg      = arg;
    pinstance->index       = 0;
    pinstance->l_tick      = 0;
    pinstance->l_index     = 0;
    pinstance->prev        = NULL;
    pinstance->next        = NULL;
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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
