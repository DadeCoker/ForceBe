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
#define CONCAT_2(s1, s2)    s1##s2
#define __stringify(x...)	#x
#define STRINGIFY(s1)       __stringify(s1)



/** 
 * \defgroup SECTION
 * \{
 */
#define bSECTION_START(section_name) CONCAT_2(__bos_start_, section_name)
#define bSECTION_END(section_name)   CONCAT_2(__bos_stop_, section_name)
 
#define bSECTION_ITEM_REGISTER_FLASH(section_name, data_type, var_name) const data_type var_name \
                        __attribute__ ((section(".bos_flash." STRINGIFY(CONCAT_2(section_name, __1))))) __attribute__((used))
 
#define bSECTION_ITEM_REGISTER_RAM(section_name, data_type, var_name)   volatile data_type var_name \
                        __attribute__ ((section(".bos_ram." STRINGIFY(CONCAT_2(section_name, __1))))) __attribute__((used))
 
 
#define bSECTION_DEF_FLASH(section_name, data_type)   \
     const data_type bSECTION_START(section_name) __attribute__ ((section(".bos_flash." STRINGIFY(CONCAT_2(section_name, __0))))) __attribute__((used)); \
     const data_type bSECTION_END(section_name)   __attribute__ ((section(".bos_flash." STRINGIFY(CONCAT_2(section_name, __2))))) __attribute__((used))
 
#define bSECTION_DEF_RAM(section_name, data_type)   \
     volatile data_type bSECTION_START(section_name) __attribute__ ((section(".bos_ram." STRINGIFY(CONCAT_2(section_name, __0))))) __attribute__((used)); \
     volatile data_type bSECTION_END(section_name)   __attribute__ ((section(".bos_ram." STRINGIFY(CONCAT_2(section_name, __2))))) __attribute__((used))
 
 
#define bSECTION_LENGTH(section_name)           \
     ((size_t)&bSECTION_END(section_name) -       \
      (size_t)&bSECTION_START(section_name))
 
#define bSECTION_ITEM_GET(section_name, data_type, i)                                      \
     (data_type *) (bSECTION_START(section_name) + i)
 
#define bSECTION_ITEM_COUNT(section_name, data_type) \
     bSECTION_LENGTH(section_name) / sizeof(data_type)

#define bSECTION_FOR_EACH(section_name, data_type, variable)                               \
     for (data_type * variable = ((data_type *)&bSECTION_START(section_name)) + 1;                \
          (intptr_t) variable != (intptr_t)&bSECTION_END(section_name);                         \
          variable++)     

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
#define BOS_REG_POLLING_FUNC(func)      bSECTION_ITEM_REGISTER_FLASH(bos_polling, pbPoling_t, CONCAT_2(polling, func))=func
/**
 * \}
 */
   
/** 
 * \defgroup COMMON_Exported_Variables
 * \{
 */


/**
 * \}
 */


/** 
 * \defgroup DEVICE_Exported_Functions
 * \{
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

/**
 * \}
 */

#ifdef __cplusplus
	}
#endif


#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/

