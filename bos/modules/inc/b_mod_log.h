/**
 *!
 * \file        b_mod_utc.h
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
#ifndef __B_MOD_LOG_H__
#define __B_MOD_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"
#if _DEBUG_ENABLE
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
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

#define B_LOG_BUF_SIZE 256

#define B_LOG_I_PARAM (B_LOG_PARAM_NULL)
#define B_LOG_W_PARAM (B_LOG_PARAM_FUNC | B_LOG_PARAM_LINE)
#define B_LOG_E_PARAM (B_LOG_PARAM_FILE | B_LOG_PARAM_FUNC | B_LOG_PARAM_LINE)

#define B_LOG_PARAM_DEFAULT __FILE__, __func__, __LINE__

#ifdef _DEBUG_ENABLE
#if (_DEBUG_ENABLE <= 1)
#define b_log_i(...) bLogOut(0, B_LOG_PARAM_DEFAULT, __VA_ARGS__)
#define b_log(...) bLogOut(3, B_LOG_PARAM_DEFAULT, __VA_ARGS__)
#else
#define b_log_i(...)
#define b_log(...)
#endif

#if (_DEBUG_ENABLE <= 2)
#define b_log_w(...) bLogOut(1, B_LOG_PARAM_DEFAULT, __VA_ARGS__)
#else
#define b_log_w(...)
#endif

#if (_DEBUG_ENABLE <= 3)
#define b_log_e(...) bLogOut(2, B_LOG_PARAM_DEFAULT, __VA_ARGS__)
#else
#define b_log_e(...)
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

void bLogOut(uint8_t type, const char *ptr_file, const char *ptr_func, uint32_t line,
             const char *fmt, ...);
void bLogOutput(void *p);
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

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
