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

#if (defined(_ADCHUB_ENABLE) && (_ADCHUB_ENABLE == 1))
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

static void _AdcListAdd(bAdcInstance_t *pinstance)
{
    bAdcInstance_t *phead = &AdcListHead;
    for (;;)
    {
        if (phead->next == NULL)
        {
            phead->next      = pinstance;
            pinstance->prev  = phead;
            pinstance->next  = NULL;
            pinstance->flag  = 0;
            pinstance->index = 0;
            break;
        }
        else
        {
            phead = phead->next;
        }
    }
}

static bAdcInstance_t *_AdcFindInstance(uint8_t seq)
{
    bAdcInstance_t *phead = &AdcListHead;
    for (;;)
    {
        if (phead->next != NULL)
        {
            if (phead->next->seq == seq)
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

static uint32_t _AdcFilterFeedVal(bAdcInstance_t *pinstance, uint32_t val)
{
    int      i   = 0;
    uint32_t max = 0, min = 0xffffffff, sum = 0;
    if (pinstance->flag == 0)
    {
        for (i = 0; i < FILTER_BUF_SIZE; i++)
        {
            pinstance->buf[i] = val;
        }
        pinstance->flag = 1;
    }
    else
    {
        pinstance->buf[pinstance->index] = val;
        pinstance->index                 = (pinstance->index + 1) % FILTER_BUF_SIZE;
    }

    for (i = 0; i < FILTER_BUF_SIZE; i++)
    {
        if (pinstance->buf[i] >= max)
        {
            max = pinstance->buf[i];
        }

        if (pinstance->buf[i] <= min)
        {
            min = pinstance->buf[i];
        }

        sum += pinstance->buf[i];
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

int bAdchubRegist(bAdcInstance_t *pinstance)
{
    if (pinstance == NULL)
    {
        return -1;
    }
    _AdcListAdd(pinstance);
    return 0;
}

int bAdchubFeedValue(uint8_t adc_seq, uint32_t ad_val)
{
    bAdcInstance_t *pinstance = _AdcFindInstance(adc_seq);
    uint32_t        value     = ad_val;
    if (pinstance == NULL)
    {
        return -1;
    }

    if (pinstance->filter == 0)
    {
        value = ad_val;
    }
    else
    {
        value = _AdcFilterFeedVal(pinstance, ad_val);
    }
    pinstance->callback(value, pinstance->arg);
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
