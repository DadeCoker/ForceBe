/**
 *!
 * \file        b_mod_adchub.c
 * \version     v0.0.1
 * \date        2020/03/26
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
#include "modules/inc/b_mod_adchub.h"

#include "b_section.h"
#include "hal/inc/b_hal.h"

#if _ADCHUB_ENABLE
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup ADCHUB
 * \{
 */

/**
 * \defgroup ADCHUB_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup ADCHUB_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup ADCHUB_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup ADCHUB_Private_Variables
 * \{
 */

static bAdcInstance_t AdcListHead = {
    .next = NULL,
    .prev = NULL,
};

/**
 * \}
 */

/**
 * \defgroup ADCHUB_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup ADCHUB_Private_Functions
 * \{
 */

static void _AdcListAdd(bAdcInstance_t *pInstance)
{
    bAdcInstance_t *phead = &AdcListHead;
    for (;;)
    {
        if (phead->next == NULL)
        {
            phead->next      = pInstance;
            pInstance->prev  = phead;
            pInstance->next  = NULL;
            pInstance->flag  = 0;
            pInstance->index = 0;
            break;
        }
        else
        {
            phead = phead->next;
        }
    }
}

static bAdcInstance_t *_AdcFindInstance(uint8_t srq)
{
    bAdcInstance_t *phead = &AdcListHead;
    for (;;)
    {
        if (phead->next != NULL)
        {
            if (phead->next->srq == srq)
            {
                return phead->next;
            }
            else
            {
                phead = phead->next;
            }
        }
        else
        {
            break;
        }
    }
    return NULL;
}

static uint32_t _AdcFilterFeedVal(bAdcInstance_t *pInstance, uint32_t val)
{
    int      i   = 0;
    uint32_t max = 0, min = 0xffffffff, sum = 0;
    if (pInstance->flag == 0)
    {
        for (i = 0; i < FILTER_BUF_SIZE; i++)
        {
            pInstance->buf[i] = val;
        }
        pInstance->flag = 1;
    }
    else
    {
        pInstance->buf[pInstance->index] = val;
        pInstance->index                 = (pInstance->index + 1) % FILTER_BUF_SIZE;
    }

    for (i = 0; i < FILTER_BUF_SIZE; i++)
    {
        if (pInstance->buf[i] >= max)
        {
            max = pInstance->buf[i];
        }

        if (pInstance->buf[i] <= min)
        {
            min = pInstance->buf[i];
        }

        sum += pInstance->buf[i];
    }

    return ((sum - max - min) / (FILTER_BUF_SIZE - 2));
}

/**
 * \}
 */

/**
 * \addtogroup ADCHUB_Exported_Functions
 * \{
 */

int bAdchubRegist(bAdcInstance_t *pInstance)
{
    if (pInstance == NULL)
    {
        return -1;
    }
    _AdcListAdd(pInstance);
    return 0;
}

int bAdchubFeedValue(uint8_t adc_srq, uint32_t ad_val)
{
    bAdcInstance_t *pInstance = _AdcFindInstance(adc_srq);
    uint32_t        value     = ad_val;
    if (pInstance == NULL)
    {
        return -1;
    }

    if (pInstance->filter == 0)
    {
        value = ad_val;
    }
    else
    {
        value = _AdcFilterFeedVal(pInstance, ad_val);
    }
    pInstance->callback(value, pInstance->arg);
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

/**
 * \}
 */
#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
