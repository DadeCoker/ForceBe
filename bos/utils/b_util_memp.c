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

#if (defined(_MEMP_ENABLE) && (_MEMP_ENABLE == 1))

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
#pragma pack(1)
typedef struct head
{
    uint32_t     status : 8;
    uint32_t     size : 24;
    struct head *next;
} bMempUnitHead_t;
#pragma pack()

/**
 * \}
 */

/**
 * \defgroup MEMP_Private_Defines
 * \{
 */
#define MEMP_SIZE (((MEMP_MAX_SIZE) >> 2) << 2)

#define MEMP_UNIT_USED (0xAA)
#define MEMP_UNIT_FREE (0x55)

#if defined(__CC_ARM)
__attribute__((aligned(4))) static uint8_t bMempBuf[MEMP_SIZE];
#elif defined(__ICCARM__)
#pragma data_alignment = 4
static uint8_t bMempBuf[MEMP_SIZE];
#elif defined(__GNUC__)
__attribute__((aligned(4))) static uint8_t bMempBuf[MEMP_SIZE];
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
static uint8_t bMempInitFlag = 0;
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

static void _bMempInit()
{
    bMempUnitHead_t *phead = (bMempUnitHead_t *)bMempBuf;
    if (bMempInitFlag == 0)
    {
        memset(bMempBuf, 0, sizeof(bMempBuf));
        phead->status = MEMP_UNIT_FREE;
        phead->size   = MEMP_SIZE - sizeof(bMempUnitHead_t);
        phead->next   = NULL;
        bMempInitFlag = 1;
    }
}

static void *_bMempAlloc(uint32_t size)
{
    size = B_SIZE_ALIGNMENT(size, 4);

    bMempUnitHead_t *phead      = (bMempUnitHead_t *)bMempBuf;
    void            *pret       = NULL;
    uint32_t         real_size  = size + sizeof(bMempUnitHead_t);
    bMempUnitHead_t *p_new_head = NULL;
    while (phead)
    {
        if (phead->status == MEMP_UNIT_USED)
        {
            phead = phead->next;
        }
        else
        {
            if (phead->size == size)
            {
                phead->status = MEMP_UNIT_USED;
                pret          = (void *)(((char *)phead) + sizeof(bMempUnitHead_t));
            }
            else if (phead->size < real_size)
            {
                phead = phead->next;
            }
            else
            {
                phead->status = MEMP_UNIT_USED;
                pret          = (void *)(((char *)phead) + sizeof(bMempUnitHead_t));
                // create a new unit
                p_new_head         = (bMempUnitHead_t *)(((char *)pret) + size);
                p_new_head->status = MEMP_UNIT_FREE;
                p_new_head->size   = phead->size - size - sizeof(bMempUnitHead_t);
                p_new_head->next   = phead->next;
                phead->size        = size;
                phead->next        = p_new_head;
            }
        }
        if (pret != NULL)
        {
            break;
        }
    }
    return pret;
}

static void _bMempFree(uint32_t addr)
{
    bMempUnitHead_t *phead = (bMempUnitHead_t *)(addr - sizeof(bMempUnitHead_t));
    if (phead->status != MEMP_UNIT_USED ||
        addr < (((uint32_t)bMempBuf) + sizeof(bMempUnitHead_t)) ||
        addr > (((uint32_t)bMempBuf) + MEMP_SIZE - sizeof(bMempUnitHead_t)))
    {
        return;
    }
    phead->status = MEMP_UNIT_FREE;
    phead         = (bMempUnitHead_t *)bMempBuf;

    while (phead != NULL)
    {
        if (phead->status == MEMP_UNIT_FREE)
        {
            while (phead->next != NULL && phead->next->status == MEMP_UNIT_FREE)
            {
                phead->size += phead->next->size + sizeof(bMempUnitHead_t);
                phead->next = phead->next->next;
            }
        }
        phead = phead->next;
    }
}

static uint32_t _bGetFreeSize()
{
    bMempUnitHead_t *phead = (bMempUnitHead_t *)bMempBuf;
    uint32_t         ret   = 0;
    while (phead)
    {
        if (phead->status == MEMP_UNIT_FREE)
        {
            ret += phead->size;
        }
        phead = phead->next;
    }
    return ret;
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
    int              valid_index = -1;
    bMempUnitHead_t *phead       = (bMempUnitHead_t *)bMempBuf;
    _bMempInit();
    if (size == 0)
    {
        return NULL;
    }
    return _bMempAlloc(size);
}

void bFree(void *paddr)
{
    if (paddr == NULL || bMempInitFlag == 0)
    {
        return;
    }
    _bMempFree((uint32_t)paddr);
}

uint32_t bGetFreeSize()
{
    _bMempInit();
    return _bGetFreeSize();
}

__WEAKDEF void bMallocFailedHook()
{
    ;
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
