/**
 *!
 * \file        b_hal_it.h
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
#ifndef __B_HAL_IT_H__
#define __B_HAL_IT_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

#include "utils/inc/b_util_list.h"

/**
 * \addtogroup B_HAL
 * \{
 */

/**
 * \addtogroup IT
 * \{
 */

/**
 * \defgroup IT_Exported_TypesDefinitions
 * \{
 */

typedef enum
{
    B_EXTI_H,
    B_EXTI_L,
    B_EXTI_RISE,
    B_EXTI_FALL
} bHalItExtiType_t;

typedef union
{
    struct
    {
        bHalItExtiType_t type;
    } _exti;
    struct
    {
        uint8_t *pbuf;
        uint16_t len;
    } _uart;
} bHalItParam_t;

typedef enum
{
    B_HAL_IT_EXTI = 0x4954,
    B_HAL_IT_UART_RX,
    B_HAL_IT_UART_TX,
    B_HAL_IT_UART_RX_DMA_CPL,
    B_HAL_IT_UART_TX_DMA_CPL,
} bHalItNumber_t;

typedef struct b_hal_it
{
    bHalItNumber_t it;
    uint8_t        index;
} bHalItIf_t;

typedef void (*bHalItHandler_t)(bHalItNumber_t it, uint8_t index, bHalItParam_t *param,
                                void *user_data);

typedef struct
{
    bHalItNumber_t   it;
    uint8_t          index;
    bHalItHandler_t  handler;
    void            *user_data;
    struct list_head head;
} bHalIt_t;

#define bHAL_IT_REGISTER(name, _it, _index, _handler, _user_data) \
    static bHalIt_t name;                                         \
    do                                                            \
    {                                                             \
        name.it        = _it;                                     \
        name.index     = _index;                                  \
        name.handler   = _handler;                                \
        name.user_data = _user_data;                              \
        bHalItRegister(&name);                                    \
    } while (0)

/**
 * \}
 */

/**
 * \defgroup IT_Exported_Functions
 * \{
 */

void bMcuIntEnable(void);
void bMcuIntDisable(void);
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
void bHalIntEnable(void);
void bHalIntDisable(void);

/**
 * 注册中断处理函数。
 * index : 例如，当int 为 B_HAL_IT_EXTI， index可以表示外部中断号
 *         例如  当int 为 B_HAL_IT_UART_RX，index可以表示串口号
 * 推荐使用：bHAL_IT_REGISTER(name, _interrupt, _index, _handler)
 */
int bHalItRegister(bHalIt_t *pit);

/**
 * MCU触发中断后，通过此函数告诉BOS有中断发生，并将数据传给BOS
 */
int bHalItInvoke(bHalItNumber_t it, uint8_t index, bHalItParam_t *param);

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
