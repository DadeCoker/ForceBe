/**
 *!
 * \file        b_mod_kv.h
 * \version     v0.0.2
 * \date        2020/04/09
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

#if _KV_ENABLE

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
 * \defgroup KV_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
    uint32_t id;
    uint32_t address;
    uint32_t len;
    uint32_t real_len;
    uint32_t statu;
} bKV_Index_t;

typedef struct
{
    uint8_t  status;
    uint32_t index;
    int      dev_no;
    uint32_t str_address;
    uint32_t ts_address;
    uint32_t te_address;
    uint32_t t_index;
    uint32_t t_max;
    uint32_t e_size;
    uint32_t ds_address;
    uint32_t de_address;
    uint32_t d_size;
    uint32_t d_index;
} bKV_Info_t;

/**
 * \}
 */

/**
 * \defgroup KV_Exported_Defines
 * \{
 */
#define bKV_IDLE 0
#define bKV_BUSY 1
#define bKV_ERROR 2

#define bKV_SECTOR_T1 0X01
#define bKV_SECTOR_T2 0X02
#define bKV_SECTOR_D1 0X04
#define bKV_SECTOR_D2 0X08
#define bKV_SECTOR_ALL 0X0F

#define bKV_HEAD_STR "B_KV"

#define bKV_ALIGN_4BYTES(n) (((n) + 3) / 4 * 4)

/**
 * \}
 */

/**
 * \defgroup KV_Exported_Functions
 * \{
 */

int bKV_Init(int dev_no, uint32_t s_addr, uint32_t size, uint32_t e_size);
int bKV_Set(const char *key, uint8_t *pvalue, uint16_t len);
int bKV_Get(const char *key, uint8_t *pvalue);
int bKV_Delete(const char *key);
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
