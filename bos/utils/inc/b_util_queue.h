/**
 *!
 * \file        b_util_queue.h
 * \version     v0.0.1
 * \date        2019/12/23
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2019 Bean
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
#ifndef __B_UTIL_QUEUE_H__
#define __B_UTIL_QUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_type.h"
/**
 * \addtogroup B_UTILS
 * \{
 */

/**
 * \addtogroup QUEUE
 * \{
 */

/**
 * \defgroup QUEUE_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
    uint8_t *pbuf;
    uint16_t item_size;
    uint16_t item_num;
    uint16_t number;
    uint16_t r_index;
    uint16_t w_index;
} bQueueInfo_t;

typedef bQueueInfo_t bQueueInstance_t;
/**
 * \}
 */

/**
 * \defgroup QUEUE_Exported_Defines
 * \{
 */
#define bQUEUE_INSTANCE(name, _item_size, _item_num)      \
    static uint8_t   queue##name[_item_size * _item_num]; \
    bQueueInstance_t name = {.pbuf      = queue##name,    \
                             .item_size = _item_size,     \
                             .item_num  = _item_num,      \
                             .r_index   = 0,              \
                             .w_index   = 0,              \
                             .number    = 0};

/**
 * \}
 */

/**
 * \defgroup QUEUE_Exported_Functions
 * \{
 */
///< pinstance \ref bQUEUE_INSTANCE
int     bQueuePush(bQueueInstance_t *pinstance, void *data);
int     bQueuePop(bQueueInstance_t *pinstance, void *data);
int     bQueuePeek(bQueueInstance_t *pinstance, void *data);
uint8_t bQueueIsFull(bQueueInstance_t *pinstance);
int     bQueueDynCreate(bQueueInstance_t *pinstance, uint16_t item_size, uint16_t item_num);
int     bQueueDynDelete(bQueueInstance_t *pinstance);
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
/************************ Copyright (c) 2019 Bean *****END OF FILE****/
