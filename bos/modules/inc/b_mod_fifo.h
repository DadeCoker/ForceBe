/**
 *!
 * \file        b_mod_fifo.h
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
#ifndef __B_MOD_FIFO_H__
#define __B_MOD_FIFO_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"
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
 * \defgroup FIFO_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
    uint8_t *         pbuf;
    uint16_t          size;
    volatile uint16_t r_index;
    volatile uint16_t w_index;
} bFIFO_Info_t;

typedef bFIFO_Info_t bFIFO_Instance_t;
/**
 * \}
 */

/**
 * \defgroup FIFO_Exported_Defines
 * \{
 */
#define bFIFO_INSTANCE(name, _fifo_size)     \
    static uint8_t   fifo##name[_fifo_size]; \
    bFIFO_Instance_t name = {.pbuf = fifo##name, .size = _fifo_size, .r_index = 0, .w_index = 0};

/**
 * \}
 */

/**
 * \defgroup FIFO_Exported_Functions
 * \{
 */
///< pFIFO_Instance \ref bFIFO_INSTANCE
int bFIFO_Length(bFIFO_Instance_t *pFIFO_Instance, uint16_t *plen);
int bFIFO_Flush(bFIFO_Instance_t *pFIFO_Instance);
int bFIFO_Write(bFIFO_Instance_t *pFIFO_Instance, uint8_t *pbuf, uint16_t size);
int bFIFO_Read(bFIFO_Instance_t *pFIFO_Instance, uint8_t *pbuf, uint16_t size);
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

#ifdef __cplusplus
}
#endif

#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
