/**
 *!
 * \file        b_common.h
 * \version     v0.0.1
 * \date        2020/05/30
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
#ifndef __B_COMMON_H__
#define __B_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup CORE
 * \{
 */

/**
 * \addtogroup COMMON
 * \{
 */

/**
 * \defgroup COMMON_Exported_TypesDefinitions
 * \{
 */
typedef void (*pbPoling_t)(void);
/**
 * \}
 */

/**
 * \defgroup COMMON_Exported_Defines
 * \{
 */

#if defined(__ICCARM__)
// Enable IAR language extensions
#pragma language = extended
#endif

#define CONCAT_2(s1, s2) s1##s2
#define __stringify(x...) #x
#define STRINGIFY(s1) __stringify(s1)

/**
 * \defgroup SECTION
 * \{
 */

/**
 * \brief the beginning of a section
 */
#if defined(__CC_ARM)
#define BOS_SECTION_START_ADDR(section_name) &CONCAT_2(section_name, $$Base)

#elif defined(__GNUC__)
#define BOS_SECTION_START_ADDR(section_name) &CONCAT_2(__start_, section_name)

#elif defined(__ICCARM__)
#define BOS_SECTION_START_ADDR(section_name) __section_begin(STRINGIFY(section_name))
#endif

/**
 * \brief the end of a section.
 */
#if defined(__CC_ARM)
#define BOS_SECTION_END_ADDR(section_name) &CONCAT_2(section_name, $$Limit)

#elif defined(__GNUC__)
#define BOS_SECTION_END_ADDR(section_name) &CONCAT_2(__stop_, section_name)

#elif defined(__ICCARM__)
#define BOS_SECTION_END_ADDR(section_name) __section_end(STRINGIFY(section_name))
#endif

/**
 * \brief the length of a given section
 */
#define BOS_SECTION_LENGTH(section_name) \
    ((size_t)BOS_SECTION_END_ADDR(section_name) - (size_t)BOS_SECTION_START_ADDR(section_name))

/**
 * \brief creating a section
 */
#if defined(__CC_ARM)
#define BOS_SECTION_DEF(section_name, data_type)      \
    extern data_type *CONCAT_2(section_name, $$Base); \
    extern void *     CONCAT_2(section_name, $$Limit)

#elif defined(__GNUC__)
#define BOS_SECTION_DEF(section_name, data_type)        \
    extern data_type *CONCAT_2(__start_, section_name); \
    extern void *     CONCAT_2(__stop_, section_name)

#elif defined(__ICCARM__)
#define BOS_SECTION_DEF(section_name, data_type) \
    _Pragma(STRINGIFY(section = STRINGIFY(section_name)));

#endif

/**
 * \brief declaring a variable and registering it in a section.
 */
#if defined(__CC_ARM)
#define BOS_SECTION_ITEM_REGISTER(section_name, section_var) \
    section_var __attribute__((section(STRINGIFY(section_name)))) __attribute__((used))

#elif defined(__GNUC__)
#define BOS_SECTION_ITEM_REGISTER(section_name, section_var) \
    section_var __attribute__((section("." STRINGIFY(section_name)))) __attribute__((used))

#elif defined(__ICCARM__)
#define BOS_SECTION_ITEM_REGISTER(section_name, section_var) \
    __root section_var @STRINGIFY(section_name)
#endif

/**
 * \brief retrieving a variable from a section.
 */
#define BOS_SECTION_ITEM_GET(section_name, data_type, i) \
    ((data_type *)BOS_SECTION_START_ADDR(section_name) + (i))

/**
 * \brief getting the number of variables in a section.
 */
#define BOS_SECTION_ITEM_COUNT(section_name, data_type) \
    BOS_SECTION_LENGTH(section_name) / sizeof(data_type)

#define bSECTION_DEF_FLASH(section_name, data_type) BOS_SECTION_DEF(section_name, const data_type)
#define bSECTION_DEF_RAM(section_name, data_type) BOS_SECTION_DEF(section_name, data_type)

#define bSECTION_ITEM_REGISTER_FLASH(section_name, data_type, var_name) \
    BOS_SECTION_ITEM_REGISTER(section_name, const data_type var_name)

#define bSECTION_ITEM_REGISTER_RAM(section_name, data_type, var_name) \
    BOS_SECTION_ITEM_REGISTER(section_name, data_type var_name)

#define bSECTION_FOR_EACH(section_name, data_type, variable)                     \
    for (data_type *variable = BOS_SECTION_ITEM_GET(section_name, data_type, 0); \
         (intptr_t)variable != (intptr_t)BOS_SECTION_END_ADDR(section_name); variable++)

/**
 * \}
 */

/**
 * \}
 */

/**
 * \defgroup COMMON_Exported_Macros
 * \{
 */
#define BOS_REG_POLLING_FUNC(func) \
    bSECTION_ITEM_REGISTER_FLASH(bos_polling, pbPoling_t, CONCAT_2(polling, func)) = func
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

#ifdef __cplusplus
}
#endif

#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
