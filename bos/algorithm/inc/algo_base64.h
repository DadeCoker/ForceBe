/**
 *!
 * \file        algo_base64.h
 * \version     v0.0.1
 * \date        2020/04/27
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
#ifndef __B_ALGO_BASE64_H__
#define __B_ALGO_BASE64_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"

/**
 * \addtogroup ALGORITHM
 * \{
 */

/**
 * \addtogroup BASE64
 * \{
 */

/**
 * \defgroup BASE64_Exported_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup BASE64_Exported_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup BASE64_Exported_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup BASE64_Exported_Variables
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup BASE64_Exported_Functions
 * \{
 */
uint16_t base64_encode(const uint8_t *in, uint16_t inlen, char *out);
uint16_t base64_decode(const char *in, uint16_t inlen, uint8_t *out);

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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
