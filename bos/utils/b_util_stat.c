/**
 *!
 * \file        b_util_stat.c
 * \version     v0.0.1
 * \date        2019/06/05
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
#include "utils/inc/b_util_stat.h"

#include "b_section.h"
#include "hal/inc/b_hal.h"

/**
 * \addtogroup B_UTILS
 * \{
 */

/**
 * \addtogroup STAT
 * \{
 */

/**
 * \defgroup STAT_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup STAT_Private_Variables
 * \{
 */
static bUtilStatInstance_t *pStatHead = NULL;
/**
 * \}
 */

/**
 * \defgroup STAT_Private_Functions
 * \{
 */

static void _bUtilStatPolling()
{
    bStatReturn_t        retval = RETVAL_NULL;
    bUtilStatInstance_t *p      = pStatHead;
    const bStatList_t   *pstat  = NULL;
    while (p != NULL)
    {
        if (p->stat >= p->list_num)
        {
            p = p->next;
            continue;
        }
        pstat = &p->plist[p->stat];
        if (p->count == 0)
        {
            if (pstat->f != NULL)
            {
                retval = pstat->f(PARAM_ENTER, p->prev_stat, p->count);
                p->count += 1;
                p->tick = bHalGetSysTick();
            }
        }
        else
        {
            if (bHalGetSysTick() - p->tick > MS2TICKS(pstat->cycle_ms))
            {
                if (pstat->f != NULL)
                {
                    retval = pstat->f(PARAM_NULL, p->prev_stat, p->count);
                    p->count += 1;
                    p->tick = bHalGetSysTick();
                }
            }
        }
        if (retval != RETVAL_NULL)
        {
            if (pstat->f != NULL)
            {
                pstat->f(PARAM_EXIT, p->prev_stat, p->count);
            }
            p->prev_stat = p->stat;
            if (retval == RETVAL_NEXT)
            {
                p->stat = pstat->next_stat;
            }
            else
            {
                p->stat = pstat->prev_stat;
            }
            p->count = 0;
        }
        p = p->next;
    }
}

BOS_REG_POLLING_FUNC(_bUtilStatPolling);

/**
 * \}
 */

/**
 * \addtogroup STAT_Exported_Functions
 * \{
 */

int bUtilStatRegist(bUtilStatInstance_t *pinstance)
{
    if (pinstance == NULL)
    {
        return -1;
    }
    if (pStatHead == NULL)
    {
        pStatHead = pinstance;
        return 0;
    }
    pinstance->next = pStatHead->next;
    pStatHead->next = pinstance;
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

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
