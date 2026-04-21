/**
 *!
 * \file        b_mod_button.h
 * \version     v0.0.1
 * \date        2020/02/21
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO BUTTON SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
#ifndef __B_MOD_BUTTON_H__
#define __B_MOD_BUTTON_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"
#if _FLEXIBLEBUTTON_ENABLE
#include "flexible_button.h"
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup MODULES
 * \{
 */

/** 
 * \addtogroup BUTTON
 * \{
 */

/** 
 * \defgroup BUTTON_Exported_TypesDefinitions
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup BUTTON_Exported_Defines
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup BUTTON_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BUTTON_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BUTTON_Exported_Functions
 * \{
 */
int bButtonInit(void);

void bButtonCallback(void *p);
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


