/**
 *!
 * \file        b_mod_fifo.c
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
#include "modules/inc/b_mod_fifo.h"
#if _FIFO_ENABLE
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
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

#ifndef NULL
#define NULL ((void *)0)
#endif

/**
 * \}
 */

/**
 * \defgroup FIFO_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup FIFO_Private_Variables
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup FIFO_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup FIFO_Private_Functions
 * \{
 */

/**
 * \}
 */

/**
 * \addtogroup FIFO_Exported_Functions
 * \{
 */

int bFIFO_Length(bFIFO_Instance_t *pFIFO_Instance, uint16_t *plen)
{
    if (pFIFO_Instance == NULL || plen == NULL)
    {
        return -1;
    }
    *plen = FIFO_LEN(pFIFO_Instance->w_index, pFIFO_Instance->r_index, pFIFO_Instance->size);
    return 0;
}

int bFIFO_Flush(bFIFO_Instance_t *pFIFO_Instance)
{
    if (pFIFO_Instance == NULL)
    {
        return -1;
    }
    pFIFO_Instance->r_index = pFIFO_Instance->w_index = 0;
    return 0;
}

int bFIFO_Write(bFIFO_Instance_t *pFIFO_Instance, uint8_t *pbuf, uint16_t size)
{
    uint16_t fifo_len, valid_len, index;
    if (pFIFO_Instance == NULL || pbuf == NULL)
    {
        return -1;
    }
    fifo_len  = FIFO_LEN(pFIFO_Instance->w_index, pFIFO_Instance->r_index, pFIFO_Instance->size);
    valid_len = pFIFO_Instance->size - fifo_len;
    size      = (size <= valid_len) ? size : valid_len;
    index     = 0;
    while (index < size)
    {
        pFIFO_Instance->pbuf[pFIFO_Instance->w_index] = pbuf[index];
        pFIFO_Instance->w_index = (pFIFO_Instance->w_index + 1) % pFIFO_Instance->size;
        index += 1;
    }
    return size;
}

int bFIFO_Read(bFIFO_Instance_t *pFIFO_Instance, uint8_t *pbuf, uint16_t size)
{
    uint16_t fifo_len, index;
    if (pFIFO_Instance == NULL || pbuf == NULL)
    {
        return -1;
    }
    fifo_len = FIFO_LEN(pFIFO_Instance->w_index, pFIFO_Instance->r_index, pFIFO_Instance->size);
    size     = (size <= fifo_len) ? size : fifo_len;
    index    = 0;
    while (index < size)
    {
        pbuf[index]             = pFIFO_Instance->pbuf[pFIFO_Instance->r_index];
        pFIFO_Instance->r_index = (pFIFO_Instance->r_index + 1) % pFIFO_Instance->size;
        index += 1;
    }
    return size;
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

/**
 * \}
 */
#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
