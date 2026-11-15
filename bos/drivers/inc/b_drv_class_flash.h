/**
 *!
 * \file        b_drv_class_flash.h
 * \version     v0.0.1
 * \date        2020/05/24
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
#ifndef __B_DRV_CLASS_FLASH_H__
#define __B_DRV_CLASS_FLASH_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_hal.h"
#include "b_utils.h"
#include "b_driver.h"
/** 
 * \addtogroup B_DRIVER
 * \{
 */

/** 
 * \defgroup FLASH_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
	uint32_t addr;
	uint32_t num;
}bCMD_Erase_t;
/**
 * \}
 */
   
/** 
 * \defgroup FLASH_Exported_Defines
 * \{
 */
#define bCMD_ERASE_SECTOR               0      // <==> bCMD_Erase_t
#define bCMD_GET_SECTOR_SIZE            1      // <==> uint32_t
#define bCMD_GET_SECTOR_COUNT           2      // <==> uint32_t
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






