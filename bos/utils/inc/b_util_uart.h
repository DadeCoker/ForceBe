/**
 *!
 * \file        b_util_uart.h
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
#ifndef __B_UTIL_UART_H__
#define __B_UTIL_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

/**
 * \addtogroup B_UTILS
 * \{
 */

/**
 * \addtogroup UART
 * \{
 */

/**
 * \defgroup UART_Exported_TypesDefinitions
 * \{
 */

typedef int (*pbUartIdleCallback_t)(uint8_t *pbuf, uint16_t len, void *arg);

typedef struct UtilUart
{
    uint8_t             *pbuf;
    uint16_t             buf_size;
    volatile uint16_t    index;
    uint32_t             idle_thd_ms;
    pbUartIdleCallback_t callback;
    void                *cb_arg;
    uint32_t             l_tick;
    uint32_t             l_index;
    struct UtilUart     *next;
    struct UtilUart     *prev;
} bUitlUart_t;

typedef bUitlUart_t bUitlUartInstance_t;

/**
 * \}
 */

/**
 * \defgroup UART_Exported_Defines
 * \{
 */
#define bUTIL_UART_INSTANCE(name, buf_len, idle_ms, cb, arg) \
    static uint8_t      Buf##name[buf_len];                  \
    bUitlUartInstance_t name = {                             \
        .pbuf        = Buf##name,                            \
        .buf_size    = buf_len,                              \
        .idle_thd_ms = idle_ms,                              \
        .callback    = cb,                                   \
        .cb_arg      = arg,                                  \
        .index       = 0,                                    \
        .l_tick      = 0,                                    \
        .l_index     = 0,                                    \
        .prev        = NULL,                                 \
        .next        = NULL,                                 \
    }

#define bUTIL_UART_INIT_INSTANCE(pinstance, buf_len, idle_ms, cb, arg) \
    static uint8_t Buf##cb[buf_len];                                   \
    do                                                                 \
    {                                                                  \
        memset((pinstance), 0, sizeof(bUitlUartInstance_t));           \
        (pinstance)->pbuf        = Buf##cb;                            \
        (pinstance)->buf_size    = buf_len;                            \
        (pinstance)->idle_thd_ms = idle_ms;                            \
        (pinstance)->callback    = cb;                                 \
        (pinstance)->cb_arg      = arg;                                \
    } while (0)

/**
 * \}
 */

/**
 * \defgroup UART_Exported_Functions
 * \{
 */
//    初始化用户定义的bUitlUartInstance_t（没有使用bUTIL_UART_INSTANCE去定义实例的情况）
void bUtilUartInitStruct(bUitlUartInstance_t *pinstance, uint8_t *pbuf, uint16_t size,
                         uint32_t idle_ms, pbUartIdleCallback_t cb, void *arg);

//    将串口号绑定到已有实例，绑定后可以用 bUtilUartRxHandler2和bUtilUartReceivedSize2
void bUtilUartBind(uint8_t uart_no, bUitlUartInstance_t *pinstance);

//    bUtilUartRxHandler 和 bUtilUartRxHandler2 效果是一样
//    但是，只有通过bUtilUartBind绑定串口号，才能调用bUtilUartRxHandler2
void bUtilUartRxHandler(bUitlUartInstance_t *pinstance, uint8_t dat);
void bUtilUartRxHandler2(uint8_t uart_no, uint8_t dat);

//    获取当前BUF中已经收到的数据长度
uint16_t bUtilUartReceivedSize(bUitlUartInstance_t *pinstance);
uint16_t bUtilUartReceivedSize2(uint8_t uart_no);
/**
 * \}
 */

/**
 * \}
 */

/**
 * \}
 */

#ifdef __cplusplus
}
#endif

#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
