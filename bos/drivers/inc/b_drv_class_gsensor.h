/**
 *!
 * \file        b_drv_class_gsensor.h
 * \version     v0.0.1
 * \date        2020/06/08
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
#ifndef __B_DRV_CLASS_GSENSOR_H__
#define __B_DRV_CLASS_GSENSOR_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_driver.h"
#include "b_hal.h"
#include "b_utils.h"
/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \defgroup GSENSOR_Exported_TypesDefinitions
 * \{
 */

typedef struct
{
    int16_t x_mg;
    int16_t y_mg;
    int16_t z_mg;
} bGsensor3Axis_t;

typedef struct
{
    uint8_t fifo_mode;
    uint8_t fifo_length;
} bGSensorCfgFIFO_t;

/**
 * \}
 */

/**
 * \defgroup GSENSOR_Exported_Defines
 * \{
 */
#define bCMD_CFG_ODR 0        // uint16_t   (x)Hz
#define bCMD_CFG_FS 1         // uint8_t   (x)g
#define bCMD_CFG_POWERDOWN 2  // no param
#define bCMD_CFG_FIFO 3       // bGSensorCfgFIFO_t
/**
 * \}
 */

/**
 * \defgroup GSENSOR_Exported_Functions
 * \{
 */
void bGsensor3AxisCallback(bGsensor3Axis_t *xyz, uint8_t number);

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
