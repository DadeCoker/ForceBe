/**
 *!
 * \file        b_util_stat.h
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
#ifndef __B_UTIL_STAT_H__
#define __B_UTIL_STAT_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

/**
 * \addtogroup B_UTILS
 * \{
 */

/**
 * \addtogroup STAT
 * \{
 */

/**
 * \defgroup STAT_Exported_TypesDefinitions
 * \{
 */

typedef enum
{
    RETVAL_NULL,
    RETVAL_NEXT,
    RETVAL_BACK
} bStatReturn_t;

typedef enum
{
    PARAM_NULL,
    PARAM_ENTER,
    PARAM_EXIT
} bStatParam_t;

typedef bStatReturn_t (*pStatFunc_t)(bStatParam_t param, uint8_t prev_stat, uint32_t count);

typedef struct
{
    uint8_t     stat;       //当前状态
    uint8_t     next_stat;  //下一级状态
    uint8_t     prev_stat;  //上一级状态
    pStatFunc_t f;          //执行函数
    uint32_t    cycle_ms;   //每隔cycle_ms执行一次函数
} bStatList_t;

typedef struct bUtilStat
{
    const bStatList_t *plist;
    uint8_t            list_num;
    uint8_t            stat;
    uint8_t            prev_stat;
    uint32_t           count;
    uint32_t           tick;
    struct bUtilStat  *next;
} bUtilStat_t;

typedef bUtilStat_t bUtilStatInstance_t;

/**
 * \}
 */

/**
 * \defgroup STAT_Exported_Defines
 * \{
 */

#define B_UTIL_STAT_INVALID (0xFF)

#define bUTIL_STAT_INSTANCE(name, stat_list, list_number, default_stat) \
    bUtilStatInstance_t name = {                                        \
        .plist     = stat_list,                                         \
        .list_num  = list_number,                                       \
        .count     = 0,                                                 \
        .stat      = default_stat,                                      \
        .prev_stat = B_UTIL_STAT_INVALID,                               \
        .tick      = 0,                                                 \
        .next      = NULL,                                              \
    }
/**
 * \}
 */

/**
 * \defgroup STAT_Exported_Functions
 * \{
 */

int bUtilStatRegist(bUtilStatInstance_t *pinstance);

/**
 * \}
 */

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
