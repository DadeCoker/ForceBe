/**
 *!
 * \file        b_util_fifo.c
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
#include "utils/inc/b_util_fifo.h"

#include "b_config.h"
#if defined(_MEMP_ENABLE) && _MEMP_ENABLE == 1
#include "utils/inc/b_util_memp.h"
#endif
/**
 * \addtogroup B_UTILS
 * \{
 */

/**
 * \addtogroup FIFO
 * \{
 */

/**
 * \defgroup FIFO_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup FIFO_Private_Defines
 * \{
 */
#define FIFO_LEN(w, r, s) (((w) >= (r)) ? ((w) - (r)) : ((w) + (s) - (r)))

/**
 * \}
 */

/**
 * \addtogroup FIFO_Exported_Functions
 * \{
 */

int bFIFO_Length(bFIFO_Instance_t *pinstance, uint16_t *plen)
{
    if (pinstance == NULL || plen == NULL)
    {
        return -1;
    }
    *plen = FIFO_LEN(pinstance->w_index, pinstance->r_index, pinstance->size);
    return 0;
}

int bFIFO_Flush(bFIFO_Instance_t *pinstance)
{
    if (pinstance == NULL)
    {
        return -1;
    }
    pinstance->r_index = pinstance->w_index = 0;
    return 0;
}

int bFIFO_Write(bFIFO_Instance_t *pinstance, uint8_t *pbuf, uint16_t size)
{
    uint16_t fifo_len, valid_len, index;
    if (pinstance == NULL || pbuf == NULL)
    {
        return -1;
    }
    if (pinstance->pbuf == NULL)
    {
        return -1;
    }
    fifo_len  = FIFO_LEN(pinstance->w_index, pinstance->r_index, pinstance->size);
    valid_len = pinstance->size - fifo_len;
    size      = (size <= valid_len) ? size : valid_len;
    index     = 0;
    while (index < size)
    {
        pinstance->pbuf[pinstance->w_index] = pbuf[index];
        pinstance->w_index                  = (pinstance->w_index + 1) % pinstance->size;
        index += 1;
    }
    return size;
}

int bFIFO_Read(bFIFO_Instance_t *pinstance, uint8_t *pbuf, uint16_t size)
{
    uint16_t fifo_len, index;
    if (pinstance == NULL || pbuf == NULL)
    {
        return -1;
    }
    if (pinstance->pbuf == NULL)
    {
        return -1;
    }
    fifo_len = FIFO_LEN(pinstance->w_index, pinstance->r_index, pinstance->size);
    size     = (size <= fifo_len) ? size : fifo_len;
    index    = 0;
    while (index < size)
    {
        pbuf[index]        = pinstance->pbuf[pinstance->r_index];
        pinstance->r_index = (pinstance->r_index + 1) % pinstance->size;
        index += 1;
    }
    return size;
}

int bFIFO_DynCreate(bFIFO_Instance_t *pinstance, uint16_t size)
{
#if defined(_MEMP_ENABLE) && _MEMP_ENABLE == 1
    if (pinstance == NULL)
    {
        return -1;
    }
    pinstance->pbuf = (uint8_t *)bMalloc(size);
    if (pinstance->pbuf == NULL)
    {
        return -1;
    }
    pinstance->size    = size;
    pinstance->w_index = 0;
    pinstance->r_index = 0;
    return 0;
#else
    return -1;
#endif
}

int bFIFO_DynDelete(bFIFO_Instance_t *pinstance)
{
#if defined(_MEMP_ENABLE) && _MEMP_ENABLE == 1
    if (pinstance == NULL)
    {
        return -1;
    }
    if (pinstance->pbuf)
    {
        bFree(pinstance->pbuf);
        pinstance->pbuf = NULL;
    }
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
