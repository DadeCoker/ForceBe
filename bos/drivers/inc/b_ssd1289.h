/**
 *!
 * \file        b_ssd1289.h
 * \version     v0.0.1
 * \date        2020/03/02
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
#ifndef __B_SSD1289_H__
#define __B_SSD1289_H__

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
 * \addtogroup SSD1289
 * \{
 */

/** 
 * \defgroup SSD1289_Exported_TypesDefinitions
 * \{
 */
 
typedef struct
{
    void (*pWriteCmd)(uint16_t);
    void (*pWriteDat)(uint16_t);
}bSSD1289Private_t; 
 
 
typedef bDriverInterface_t bSSD1289_Driver_t;  
/**
 * \}
 */
   
/** 
 * \defgroup SSD1289_Exported_Defines
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup SSD1289_Exported_Macros
 * \{
 */



/**
 * \}
 */
   
/** 
 * \defgroup SSD1289_Exported_Variables
 * \{
 */
 
/**
 * \}
 */
   
/** 
 * \defgroup SSD1289_Exported_Functions
 * \{
 */
int bSSD1289_Init(bSSD1289_Driver_t *pdrv);

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



