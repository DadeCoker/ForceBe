/**
 *!
 * \file       b_mod_ADCHUB.h
 * \version    v0.0.1
 * \date       2020/03/26
 * \author     Bean(notrynohigh@outlook.com)
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
#ifndef __B_MOD_ADCHUB_H__
#define __B_MOD_ADCHUB_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

#include "b_config.h"

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
 * \defgroup ADCHUB_Exported_TypesDefinitions
 * \{
 */

typedef void (*pAdchubCb_t)(uint32_t ad_val, uint32_t arg);

typedef struct _AdcInfo
{
    uint8_t          seq;
    uint8_t          filter;
    uint8_t          flag;
    uint8_t          index;
    pAdchubCb_t      callback;
    uint32_t         arg;
    uint32_t         buf[FILTER_BUF_SIZE];
    struct _AdcInfo *next;
    struct _AdcInfo *prev;
} bAdcInfo_t;

typedef bAdcInfo_t bAdcInstance_t;

/**
 * \}
 */

/**
 * \defgroup ADCHUB_Exported_Defines
 * \{
 */

#define bADC_INSTANCE(name, ad_seq, filter_en, cb, cb_arg) \
    bAdcInstance_t name = {                                \
        .seq      = ad_seq,                                \
        .filter   = filter_en,                             \
        .callback = cb,                                    \
        .arg      = cb_arg,                                \
    }

/**
 * \}
 */

/**
 * \defgroup ADCHUB_Exported_Functions
 * \{
 */

int bAdchubRegist(bAdcInstance_t *pinstance);
int bAdchubFeedValue(uint8_t ad_seq, uint32_t ad_val);

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
/************************ Copyright (c) 2020 Bean *****END OF FILE****/
