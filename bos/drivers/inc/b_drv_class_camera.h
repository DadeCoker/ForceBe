/**
 *!
 * \file        b_drv_class_camera.h
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
#ifndef __B_DRV_CLASS_CAMERA_H__
#define __B_DRV_CLASS_CAMERA_H__

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
 * \defgroup CAMERA_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
    uint8_t config_val;
}bCameraCfgStruct_t;


typedef struct
{
    uint16_t xoff;
    uint16_t yoff;
    uint16_t xsize;
    uint16_t ysize;
}bCameraCfgOutsize_t;

/**
 * \}
 */
   
/** 
 * \defgroup CAMERA_Exported_Defines
 * \{
 */
#define bCMD_CONF_LIGHTMODE         0               //bCameraCfgStruct_t
#define bCMD_CONF_COLOR_SAT         1               //bCameraCfgStruct_t
#define bCMD_CONF_BRIGHTNESS        2               //bCameraCfgStruct_t
#define bCMD_CONF_CONTRAST          3               //bCameraCfgStruct_t
#define bCMD_CONF_SHARPNESS         4               //bCameraCfgStruct_t
#define bCMD_CONF_FLASH_LED         5               //bCameraCfgStruct_t
#define bCMD_CONF_OUTSIZE           6
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






