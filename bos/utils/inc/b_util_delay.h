/**
 *!
 * \file        b_util_delay.h
 * \version     v0.0.1
 * \date        2019/12/26
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
#ifndef __B_UTIL_DELAY_H__
#define __B_UTIL_DELAY_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h" 


/** 
 * \addtogroup B_UTILS
 * \{
 */

/** 
 * \addtogroup DELAY
 * \{
 */

/** 
 * \defgroup DELAY_Exported_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup DELAY_Exported_Defines
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup DELAY_Exported_Macros
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup DELAY_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup DELAY_Exported_Functions
 * \{
 */

void bUtilDelayMS(uint32_t xms);
void bUtilDelayUS(uint32_t xus);  
uint32_t bUtilGetTick(void);

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


