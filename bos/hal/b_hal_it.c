/**
 *!
 * \file        b_hal_it.c
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
 * \addtogroup IT
 * \{
 */

/**
 * \defgroup IT_Private_Variables
 * \{
 */

static LIST_HEAD(bHalItListHead);

/**
 * \}
 */

/**
 * \addtogroup IT_Exported_Functions
 * \{
 */

__WEAKDEF void bMcuIntEnable()
{
    ;
}

__WEAKDEF void bMcuIntDisable()
{
    ;
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

void bHalIntEnable()
{
    bMcuIntEnable();
}

void bHalIntDisable()
{
    bMcuIntDisable();
}

/**
 * 注册中断处理函数。
 * index : 例如，当int 为 B_HAL_IT_EXTI， index可以表示外部中断号
 *         例如  当int 为 B_HAL_IT_UART_RX，index可以表示串口号
 * 推荐使用：bHAL_IT_REGISTER(name, _interrupt, _index, _handler)
 */
int bHalItRegister(bHalIt_t *pit)
{
    if (pit == NULL)
    {
        return -1;
    }
    if (pit->handler == NULL)
    {
        return -1;
    }
    list_add(&pit->head, &bHalItListHead);
    return 0;
}

/**
 * MCU触发中断后，通过此函数告诉BOS有中断发生，并将数据传给BOS
 */
int bHalItInvoke(bHalItNumber_t it, uint8_t index, bHalItParam_t *param)
{
    bHalIt_t         *phalit = NULL;
    struct list_head *pos;
    list_for_each(pos, &bHalItListHead)
    {
        phalit = list_entry(pos, bHalIt_t, head);
        if (it == phalit->it && index == phalit->index)
        {
            if (phalit->handler)
            {
                phalit->handler(it, index, param, phalit->user_data);
                return 0;
            }
        }
    }
    return -1;
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
