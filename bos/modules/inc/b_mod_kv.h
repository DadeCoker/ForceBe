/**
 *!
 * \file        b_mod_kv.h
 * \version     v0.0.3
 * \date        2022/11/13
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO KV SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
#ifndef __B_MOD_KV_H__
#define __B_MOD_KV_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

#include "b_config.h"

#define _KV_ENABLE 1

#if (defined(_KV_ENABLE) && (_KV_ENABLE == 1))

/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup KV
 * \{
 */

/**
 * \defgroup KV_Exported_Typedefines
 * \{
 */
typedef struct
{
    const uint32_t dev;
    const uint32_t address;
    const uint32_t total_size;
    uint32_t       erase_size;
    uint32_t       write_index;
} bKVStruct_t;

typedef bKVStruct_t bKVInstance_t;

/**
 * \}
 */

/**
 * \defgroup KV_Exported_Defines
 * \{
 */

#define bKV_INSTANCE(name, dev_no, addr, size) \
    bKVInstance_t name = {.dev = dev_no, .address = addr, .total_size = size, erase_size = 0};

/**
 * \}
 */

/**
 * \defgroup KV_Exported_Functions
 * \{
 */
int bKVInit(bKVInstance_t *pinstance);
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
