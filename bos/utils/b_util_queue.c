/**
 *!
 * \file        b_util_queue.c
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

/*Includes ----------------------------------------------*/
#include "utils/inc/b_util_queue.h"

/**
 * \addtogroup B_UTILS
 * \{
 */

/**
 * \addtogroup QUEUE
 * \{
 */

/**
 * \defgroup QUEUE_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup QUEUE_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \addtogroup QUEUE_Exported_Functions
 * \{
 */
int bQueuePush(bQueueInstance_t *pinstance, void *data)
{
    if (pinstance == NULL || data == NULL)
    {
        return -1;
    }
    if (pinstance->pbuf == NULL)
    {
        return -1;
    }
    if (pinstance->number == pinstance->item_size)
    {
        return -2;
    }
    memcpy(&pinstance->pbuf[pinstance->w_index * pinstance->item_size], data, pinstance->item_size);
    pinstance->w_index = (pinstance->w_index + 1) % pinstance->item_num;
    pinstance->number += 1;
    return 0;
}

int bQueuePop(bQueueInstance_t *pinstance, void *data)
{
    if (pinstance == NULL || data == NULL)
    {
        return -1;
    }
    if (pinstance->pbuf == NULL)
    {
        return -1;
    }
    if (pinstance->number == 0)
    {
        return -3;
    }
    memcpy(data, &pinstance->pbuf[pinstance->r_index * pinstance->item_size], pinstance->item_size);
    pinstance->r_index = (pinstance->r_index + 1) % pinstance->item_num;
    pinstance->number -= 1;
    return 0;
}

int bQueuePeek(bQueueInstance_t *pinstance, void *data)
{
    if (pinstance == NULL || data == NULL)
    {
        return -1;
    }
    if (pinstance->pbuf == NULL)
    {
        return -1;
    }
    if (pinstance->number == 0)
    {
        return -3;
    }
    memcpy(data, &pinstance->pbuf[pinstance->r_index * pinstance->item_size], pinstance->item_size);
    return 0;
}

uint8_t bQueueIsFull(bQueueInstance_t *pinstance)
{
    if (pinstance == NULL)
    {
        return 0;
    }
    return (pinstance->number >= pinstance->item_size);
}

int bQueueDynCreate(bQueueInstance_t *pinstance, uint16_t item_size, uint16_t item_num)
{
#if defined(_MEMP_ENABLE) && _MEMP_ENABLE == 1
    if (pinstance == NULL || item_size == 0 || item_num == 0)
    {
        return -1;
    }
    memset(pinstance, 0, sizeof(bQueueInstance_t));
    pinstance->pbuf = bMalloc(item_size * item_num);
    if (pinstance->pbuf == NULL)
    {
        return -1;
    }
    pinstance->item_size = item_size;
    pinstance->item_num  = item_num;
    return 0;
#else
    return -1;
#endif
}

int bQueueDynDelete(bQueueInstance_t *pinstance)
{
#if defined(_MEMP_ENABLE) && _MEMP_ENABLE == 1
    if (pinstance == NULL || item_size == 0 || item_num == 0)
    {
        return -1;
    }
    if (pinstance->pbuf)
    {
        bFree(pinstance->pbuf);
        pinstance->pbuf = NULL;
    }
    memset(pinstance, 0, sizeof(bQueueInstance_t));
    return 0;
#else
    return -1;
#endif
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
