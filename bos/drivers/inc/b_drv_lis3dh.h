/**
 *!
 * \file        b_drv_lis3dh.h
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
#ifndef __B_DRV_LIS3DH_H__
#define __B_DRV_LIS3DH_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "drivers/inc/b_driver.h"

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup LIS3DH
 * \{
 */

/**
 * \defgroup LIS3DH_Exported_TypesDefinitions
 * \{
 */
//<HALIF 2 SPI_I2C
typedef struct
{
    union
    {
        bHalI2CIf_t _i2c;
        bHalSPIIf_t _spi;
    } _if;
    uint8_t is_spi;
} bLIS3DH_HalIf_t;

typedef enum
{
    LIS3DH_POWER_DOWN                   = 0x00,
    LIS3DH_ODR_1Hz                      = 0x01,
    LIS3DH_ODR_10Hz                     = 0x02,
    LIS3DH_ODR_25Hz                     = 0x03,
    LIS3DH_ODR_50Hz                     = 0x04,
    LIS3DH_ODR_100Hz                    = 0x05,
    LIS3DH_ODR_200Hz                    = 0x06,
    LIS3DH_ODR_400Hz                    = 0x07,
    LIS3DH_ODR_1kHz620_LP               = 0x08,
    LIS3DH_ODR_5kHz376_LP_1kHz344_NM_HP = 0x09,
} bLis3dhODR_t;

typedef enum
{
    LIS3DH_FS_2G  = 0,
    LIS3DH_FS_4G  = 1,
    LIS3DH_FS_8G  = 2,
    LIS3DH_FS_16G = 3,
} bLis3dhFS_t;

typedef enum
{
    LIS3DH_HR_12BIT = 0,
    LIS3DH_NM_10BIT = 1,
    LIS3DH_LP_8BIT  = 2,
} bLis3dhOptMode_t;

typedef struct
{
    bLis3dhODR_t     odr;
    bLis3dhFS_t      fs;
    bLis3dhOptMode_t opmode;
} bList3dhPrivate_t;

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
