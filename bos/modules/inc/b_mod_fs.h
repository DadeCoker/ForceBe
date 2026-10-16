/**
 *!
 * \file        b_mod_fs.h
 * \version     v0.0.1
 * \date        2020/06/02
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
#ifndef __B_MOD_FS_H__
#define __B_MOD_FS_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"  
#if _FS_ENABLE
#include "ff.h"
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup MODULES
 * \{
 */

/** 
 * \addtogroup FS
 * \{
 */

/** 
 * \defgroup FS_Exported_TypesDefinitions
 * \{
 */
typedef enum
{
#if _SPIFLASH_ENABLE    
    E_DEV_SPIFLASH,           /* Map SPIFLASH to physical drive*/
#endif
#if _SD_ENABLE    
    E_DEV_SDCARD,             /* Map MMC/SD card to physical drive*/
#endif
    E_DEV_NUMBER,
}FS_DEV_Enum_t;
/**
 * \}
 */
   
/** 
 * \defgroup FS_Exported_Defines
 * \{
 */
#define DEV_SPIFLASH        0   /* Map SPIFLASH to physical drive 0*/
#define DEV_SDCARD          1   /* Map SPIFLASH to physical drive 1*/

/**
 * \}
 */
   
   
/** 
 * \defgroup FS_Exported_Functions
 * \{
 */
int bFS_Init(void);
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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/

