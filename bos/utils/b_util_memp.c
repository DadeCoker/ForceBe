/**
 *!
 * \file        b_util_memp.c
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
#include "utils/inc/b_util_memp.h"

#if _MEMP_ENABLE

/**
 * \addtogroup B_UTILS
 * \{
 */

/**
 * \addtogroup MEMP
 * \{
 */

/**
 * \defgroup MEMP_Private_TypesDefinitions
 * \{
 */
typedef struct
{
    uint16_t index;
    uint16_t maxlen;
} bMempUnitInfo_t;

/**
 * \}
 */

/**
 * \defgroup MEMP_Private_Defines
 * \{
 */
#define MEMP_SIZE (((MEMP_MAX_SIZE) >> 2) << 2)
#define MEMP_UNIT_NUM ((MEMP_SIZE) / (MEMP_BLOCK_SIZE))

#if defined(__CC_ARM)
__align(4) static uint8_t bMempBuf[MEMP_SIZE];
#elif defined(__ICCARM__)
#pragma data_alignment = 4
static uint8_t bMempBuf[MEMP_SIZE];
#elif defined(__GNUC__)
static uint8_t bMempBuf[MEMP_SIZE];
__attribute__((aligned(4)));
#endif

/**
 * \}
 */

/**
 * \defgroup MEMP_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup MEMP_Private_Variables
 * \{
 */
static bMempUnitInfo_t    bMempUnitInfo = {0, MEMP_UNIT_NUM};
static uint16_t           bMempUnit[MEMP_UNIT_NUM];
static uint8_t            bMempInitFlag = 0;
static bMempMonitorInfo_t bMempMonitorInfo;
/**
 * \}
 */

/**
 * \defgroup MEMP_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup MEMP_Private_Functions
 * \{
 */

static void _bMempMonitor()
{
    uint16_t num = 0, i = 0;
    for (i = 0; i < MEMP_UNIT_NUM; i++)
    {
        if (bMempUnit[i] == 0)
        {
            num += 1;
        }
    }
    if (num < bMempMonitorInfo.unused_unit)
    {
        bMempMonitorInfo.unused_unit = num;
    }
}

static void _bMempUpdateInfo()
{
    int      i         = 0;
    uint16_t max_len   = bMempUnitInfo.maxlen;
    uint16_t max_index = bMempUnitInfo.index;
    uint16_t count = 0, index = 0;

    for (i = 0; i < MEMP_UNIT_NUM; i++)
    {
        if (bMempUnit[i] == 0)
        {
            if (count == 0)
            {
                index = i;
            }
            count++;
        }
        else
        {
            if (count > max_len)
            {
                max_len   = count;
                max_index = index;
            }
            count = 0;
        }
    }
    if (count > max_len)
    {
        max_len   = count;
        max_index = index;
    }
    bMempUnitInfo.maxlen = max_len;
    bMempUnitInfo.index  = max_index;
}

static int _bMempAlloc(uint16_t num)
{
    int retval = -1, i = 0;
    if (num > bMempUnitInfo.maxlen)
    {
        _bMempUpdateInfo();
    }
    if (num <= bMempUnitInfo.maxlen)
    {
        for (i = 0; i < num; i++)
        {
            bMempUnit[i + bMempUnitInfo.index] = bMempUnitInfo.index + 1;
        }
        retval = bMempUnitInfo.index;
        bMempUnitInfo.maxlen -= num;
        bMempUnitInfo.index += num;
    }
    return retval;
}

static void _bMempFree(uint32_t index)
{
    uint16_t id = 0, i;
    if (index >= MEMP_UNIT_NUM)
    {
        return;
    }
    id = bMempUnit[index];
    if (id == 0)
    {
        return;
    }
    for (i = 0; i < MEMP_UNIT_NUM; i++)
    {
        if (bMempUnit[i] == id)
        {
            bMempUnit[i] = 0;
            memset(&bMempBuf[i * MEMP_BLOCK_SIZE], 0, MEMP_BLOCK_SIZE);
        }
    }
}
/**
 * \}
 */

/**
 * \addtogroup MEMP_Exported_Functions
 * \{
 */
void *bMalloc(uint32_t size)
{
    int valid_index = -1;
    if (size > MEMP_SIZE || size == 0)
    {
        return NULL;
    }
    if (bMempInitFlag == 0)
    {
        memset(bMempBuf, 0, sizeof(bMempBuf));
        memset(bMempUnit, 0, sizeof(bMempUnit));

        bMempUnitInfo.index          = 0;
        bMempUnitInfo.maxlen         = MEMP_UNIT_NUM;
        bMempMonitorInfo.unused_unit = MEMP_UNIT_NUM;
        bMempInitFlag                = 1;
    }
    valid_index = _bMempAlloc((size + MEMP_BLOCK_SIZE - 1) / (MEMP_BLOCK_SIZE));
    if (valid_index < 0)
    {
        return NULL;
    }
#if _MEMP_MONITOR_ENABLE
    _bMempMonitor();
#endif
    return ((void *)(&bMempBuf[valid_index * MEMP_BLOCK_SIZE]));
}

void bFree(void *paddr)
{
    uint32_t index = 0;
    if (paddr == NULL || bMempInitFlag == 0)
    {
        return;
    }
    index = ((uint32_t)paddr - (uint32_t)(&bMempBuf[0])) / MEMP_BLOCK_SIZE;
    _bMempFree(index);
}

#if _MEMP_MONITOR_ENABLE
void bMempGetMonitorInfo(bMempMonitorInfo_t *pinfo)
{
    if (pinfo == NULL)
    {
        return;
    }
    pinfo->unused_unit = bMempMonitorInfo.unused_unit;
}
#endif

int bMempListInit(bMempList_t *phead)
{
    if (phead == NULL)
    {
        return -1;
    }
    phead->p          = NULL;
    phead->total_size = 0;
    phead->size       = 0;
    phead->next       = phead;
    phead->prev       = phead;
    return 0;
}

int bMempListAdd(bMempList_t *phead, uint8_t *p, uint32_t len)
{
    void        *ptmp  = NULL;
    bMempList_t *pnode = NULL;
    if (phead == NULL || p == NULL || len == 0)
    {
        return -1;
    }
    ptmp = bMalloc(len);
    if (ptmp == NULL)
    {
        return -2;
    }
    memcpy(ptmp, p, len);
    if (phead->p != NULL)
    {
        pnode = (bMempList_t *)bMalloc(sizeof(bMempList_t));
        if (pnode == NULL)
        {
            bFree(ptmp);
            return -2;
        }
        pnode->next       = phead;
        pnode->prev       = phead->prev;
        phead->prev->next = pnode;
        phead->prev       = pnode;
        pnode->size       = len;
        pnode->total_size = 0;
        pnode->p          = ptmp;
    }
    else
    {
        phead->p    = ptmp;
        phead->size = len;
    }
    phead->total_size += len;
    return 0;
}

uint8_t *bMempList2Array(const bMempList_t *phead)
{
    uint8_t           *p     = NULL;
    const bMempList_t *ptmp  = phead;
    uint32_t           index = 0;
    if (phead == NULL)
    {
        return NULL;
    }
    if (phead->total_size == 0)
    {
        return NULL;
    }
    p = bMalloc(phead->total_size);
    if (p == NULL)
    {
        return NULL;
    }
    memcpy(p + index, phead->p, phead->size);
    index += phead->size;
    while (ptmp->next != phead)
    {
        ptmp = ptmp->next;
        memcpy(&p[index], ptmp->p, ptmp->size);
        index += ptmp->size;
    }
    return p;
}

int bMempListFree(bMempList_t *phead)
{
    bMempList_t *ptmp = phead;
    if (phead == NULL)
    {
        return -1;
    }

    if (phead->p != NULL)
    {
        bFree(phead->p);
        phead->p          = NULL;
        phead->size       = 0;
        phead->total_size = 0;
    }

    while (phead->next != phead)
    {
        ptmp        = phead->next;
        phead->next = ptmp->next;
        bFree(ptmp->p);
        bFree(ptmp);
    }
    phead->prev = phead;
    return 0;
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

#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
