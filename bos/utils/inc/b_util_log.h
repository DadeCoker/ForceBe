/**
 *!
 * \file        b_util_log.h
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
#ifndef __B_UTIL_LOG_H__
#define __B_UTIL_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

#include "b_config.h"
/**
 * \addtogroup B_UTILS
 * \{
 */

/**
 * \addtogroup LOG
 * \{
 */

/**
 * \defgroup LOG_Exported_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup LOG_Exported_Defines
 * \{
 */
#define B_LOG_PARAM_NULL 0X0
#define B_LOG_PARAM_LINE 0X1
#define B_LOG_PARAM_FUNC 0X2
#define B_LOG_PARAM_FILE 0X4

#define B_LOG_BUF_SIZE LOG_BUF_SIZE

#define B_LOG_I_PARAM (B_LOG_PARAM_NULL)
#define B_LOG_W_PARAM (B_LOG_PARAM_FUNC | B_LOG_PARAM_LINE)
#define B_LOG_E_PARAM (B_LOG_PARAM_FILE | B_LOG_PARAM_FUNC | B_LOG_PARAM_LINE)

#define B_LOG_PARAM_DEFAULT __FILE__, __func__, __LINE__

#if ((defined(_DEBUG_ENABLE)) && (_DEBUG_ENABLE == 1))

#if ((defined(LOG_LEVEL_INFO)) && (LOG_LEVEL_INFO == 1))

#define b_log_i(...) bLogOut(0, B_LOG_PARAM_DEFAULT, __VA_ARGS__)
#define b_log_w(...) bLogOut(1, B_LOG_PARAM_DEFAULT, __VA_ARGS__)
#define b_log_e(...) bLogOut(2, B_LOG_PARAM_DEFAULT, __VA_ARGS__)
#define b_log(...) bLogOut(3, B_LOG_PARAM_DEFAULT, __VA_ARGS__)

#elif ((defined(LOG_LEVEL_WARNING)) && (LOG_LEVEL_WARNING == 1))

#define b_log_i(...)
#define b_log_w(...) bLogOut(1, B_LOG_PARAM_DEFAULT, __VA_ARGS__)
#define b_log_e(...) bLogOut(2, B_LOG_PARAM_DEFAULT, __VA_ARGS__)
#define b_log(...)

#elif ((defined(LOG_LEVEL_ERROR)) && (LOG_LEVEL_ERROR == 1))

#define b_log_i(...)
#define b_log_w(...)
#define b_log_e(...) bLogOut(2, B_LOG_PARAM_DEFAULT, __VA_ARGS__)
#define b_log(...)

#else

#define b_log_i(...)
#define b_log_w(...)
#define b_log_e(...)
#define b_log(...)

#endif

#else

#define b_log_i(...)
#define b_log_w(...)
#define b_log_e(...)
#define b_log(...)

#endif

/**
 * \}
 */

/**
 * \defgroup LOG_Exported_Functions
 * \{
 */
#if ((defined(_DEBUG_ENABLE)) && (_DEBUG_ENABLE == 1))
void bLogOut(uint8_t type, const char *ptr_file, const char *ptr_func, uint32_t line,
             const char *fmt, ...);
void bLogOutput(void *p);
#endif
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
