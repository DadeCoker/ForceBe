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
    uint8_t        init_f;
    const uint32_t dev;
    const uint32_t address;
    const uint32_t total_size;
    const uint32_t erase_size;
    int32_t        write_index;
    uint32_t       write_offset;
    uint32_t       empty_count;
} bKVStruct_t;

typedef bKVStruct_t bKVInstance_t;

/**
 * \}
 */

/**
 * \defgroup KV_Exported_Defines
 * \{
 */

/**
 * \brief 定义实例，如果存储器不需要擦除，则e_size为0
 */
#define bKV_INSTANCE(name, dev_no, addr, size, e_size) \
    bKVInstance_t name = {                             \
        .init_f = 0, .dev = dev_no, .address = addr, .total_size = size, erase_size = e_size};

/**
 * \}
 */

/**
 * \defgroup KV_Exported_Functions
 * \{
 */
int bKVInit(bKVInstance_t *pinstance);
int bKVGetValue(bKVInstance_t *pinstance, const char *key, uint8_t *pbuf, uint32_t len,
                uint32_t *prlen);
int bKVSetValue(bKVInstance_t *pinstance, const char *key, uint8_t *pbuf, uint32_t len);
int bKVDeleteValue(bKVInstance_t *pinstance, const char *key);
int bKVGetValueLength(bKVInstance_t *pinstance, const char *key);
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
