/**
 *!
 * \file        b_util_memp.h
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
#ifndef __B_UTIL_MEMP_H__
#define __B_UTIL_MEMP_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdlib.h>
#include <string.h>

#include "b_config.h"

/**
 * \addtogroup B_UTILS
 * \{
 */

/**
 * \addtogroup MEMP
 * \{
 */

/**
 * \defgroup MEMP_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
    uint16_t unused_unit;
} bMempMonitorInfo_t;

typedef struct bMempList
{
    uint8_t           *p;
    uint32_t          total_size;
    uint32_t          size;
    struct bMempList *next;
    struct bMempList *prev;
} bMempList_t;

/**
 * \}
 */
#if _MEMP_ENABLE
/**
 * \defgroup MEMP_Exported_Functions
 * \{
 */
void *bMalloc(uint32_t size);
void  bFree(void *paddr);
#if _MEMP_MONITOR_ENABLE
void bMempGetMonitorInfo(bMempMonitorInfo_t *pinfo);
#endif
int bMempListInit(bMempList_t *phead);
int bMempListAdd(bMempList_t *phead, uint8_t *p, uint32_t len);
int bMempListFree(bMempList_t *phead);

uint8_t * bMempList2Array(const bMempList_t *phead);
/**
 * \}
 */
#endif
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
