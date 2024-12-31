/**
 *!
 * \file        b_xpt2046.h
 * \version     v0.0.1
 * \date        2020/02/05
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
#ifndef __B_XPT2046_H__
#define __B_XPT2046_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h" 
#include "b_device.h"
/** 
 * \addtogroup B_DRIVER
 * \{
 */

/** 
 * \addtogroup XPT2046
 * \{
 */

/** 
 * \defgroup XPT2046_Exported_TypesDefinitions
 * \{
 */
typedef bDriverInterface_t bXPT2046_Driver_t;  

/**
 * \}
 */
   
/** 
 * \defgroup XPT2046_Exported_Defines
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup XPT2046_Exported_Macros
 * \{
 */



/**
 * \}
 */
   
/** 
 * \defgroup XPT2046_Exported_Variables
 * \{
 */
extern bXPT2046_Driver_t bXPT2046_Driver;   
/**
 * \}
 */
   
/** 
 * \defgroup XPT2046_Exported_Functions
 * \{
 */
int bXPT2046_Init(void);

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



