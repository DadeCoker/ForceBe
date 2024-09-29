/**
 *!
 * \file        b_lunnar.h
 * \version     v0.0.1
 * \date        2019/12/31
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
#ifndef __B_LUNAR_H__
#define __B_LUNAR_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"  
#if _LUNAR_ENABLE

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup LUNAR
 * \{
 */

/** 
 * \defgroup LUNAR_Exported_TypesDefinitions
 * \{
 */

typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
}bLunarInfo_t;

/**
 * \}
 */
   
/** 
 * \defgroup LUNAR_Exported_Defines
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup LUNAR_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup LUNAR_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup LUNAR_Exported_Functions
 * \{
 */
int bSolar2Lunar(uint16_t syear, uint8_t smonth, uint8_t sday, bLunarInfo_t *plunar);
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

