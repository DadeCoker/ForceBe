/**
 *!
 * \file        b_os.h
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
#ifndef __B_OS_H__
#define __B_OS_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

#include "b_config.h"

#ifndef _BOS_ALGO_ENABLE
#define _BOS_ALGO_ENABLE (0)
#endif

#ifndef _BOS_MODULES_ENABLE
#define _BOS_MODULES_ENABLE (0)
#endif

/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup BOS
 * \{
 */
#define BOS_PERIODIC_TASK(pf, ms)                         \
    {                                                     \
        static uint32_t tick##pf = 0;                     \
        if (bHalGetSysTick() - tick##pf > (MS2TICKS(ms))) \
        {                                                 \
            tick##pf = bHalGetSysTick();                  \
            pf();                                         \
        }                                                 \
    }

#if _BOS_ALGO_ENABLE
#include "algorithm/inc/algorithm.h"
#endif

#include "core/inc/b_core.h"
#include "core/inc/b_device.h"
#include "drivers/inc/b_driver.h"

#if _BOS_MODULES_ENABLE
#include "modules/inc/b_modules.h"
#endif

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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
