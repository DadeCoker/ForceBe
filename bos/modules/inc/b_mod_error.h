/**
 *!
 * \file        b_mod_error.h
 * \version     v1.0.1
 * \date        2020/02/24
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
#ifndef __B_MOD_ERROR_H__
#define __B_MOD_ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

#include "b_config.h"

#if _ERROR_MANAGE_ENABLE
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup ERROR
 * \{
 */

/**
 * \defgroup ERROR_Exported_TypesDefinitions
 * \{
 */

typedef void (*pecb)(uint8_t err);

/**
 * \}
 */

/**
 * \defgroup ERROR_Exported_Defines
 * \{
 */
#define INVALID_ERR ((uint8_t)0xFF)

#define BERROR_LEVEL_0 0X00
#define BERROR_LEVEL_1 0X01

/**
 * \}
 */

/**
 * \defgroup ERROR_Exported_Functions
 * \{
 */
int bErrorInit(pecb cb);
int bErrorRegist(uint8_t err, uint32_t interval_ms, uint32_t level);
int bErrorClear(uint8_t e_no);
int bErrorIsExist(uint8_t e_no);
int bErrorIsEmpty(void);
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
