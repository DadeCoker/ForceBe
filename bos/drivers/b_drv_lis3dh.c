/**
 *!
 * \file        b_drv_lis3dh.c
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

/*Includes ----------------------------------------------*/
#include "drivers/inc/b_drv_lis3dh.h"

#include <string.h>

#include "utils/inc/b_util_log.h"

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup LIS3DH
 * \{
 */

/**
 * \defgroup LIS3DH_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup LIS3DH_Private_Defines
 * \{
 */

#define DRIVER_NAME LIS3DH

/** Device Identification (Who am I) **/
#define LIS3DH_ID 0x33U
#define LIS3DH_OUT_ADC1_L 0x08U
#define LIS3DH_OUT_ADC1_H 0x09U
#define LIS3DH_OUT_ADC2_L 0x0AU
#define LIS3DH_OUT_ADC2_H 0x0BU
#define LIS3DH_OUT_ADC3_L 0x0CU
#define LIS3DH_OUT_ADC3_H 0x0DU
#define LIS3DH_WHO_AM_I 0x0FU
#define LIS3DH_CTRL_REG0 0x1EU
#define LIS3DH_TEMP_CFG_REG 0x1FU
#define LIS3DH_CTRL_REG1 0x20U
#define LIS3DH_CTRL_REG2 0x21U
#define LIS3DH_CTRL_REG3 0x22U
#define LIS3DH_CTRL_REG4 0x23U
#define LIS3DH_CTRL_REG5 0x24U
#define LIS3DH_CTRL_REG6 0x25U
#define LIS3DH_REFERENCE 0x26U
#define LIS3DH_STATUS_REG 0x27U
#define LIS3DH_OUT_X_L 0x28U
#define LIS3DH_OUT_X_H 0x29U
#define LIS3DH_OUT_Y_L 0x2AU
#define LIS3DH_OUT_Y_H 0x2BU
#define LIS3DH_OUT_Z_L 0x2CU
#define LIS3DH_OUT_Z_H 0x2DU
#define LIS3DH_FIFO_CTRL_REG 0x2EU
#define LIS3DH_FIFO_SRC_REG 0x2FU
#define LIS3DH_INT1_CFG 0x30U
#define LIS3DH_INT1_SRC 0x31U
#define LIS3DH_INT1_THS 0x32U
#define LIS3DH_INT1_DURATION 0x33U
#define LIS3DH_INT2_CFG 0x34U
#define LIS3DH_INT2_SRC 0x35U
#define LIS3DH_INT2_THS 0x36U
#define LIS3DH_INT2_DURATION 0x37U
#define LIS3DH_CLICK_CFG 0x38U
#define LIS3DH_CLICK_SRC 0x39U
#define LIS3DH_CLICK_THS 0x3AU
#define LIS3DH_TIME_LIMIT 0x3BU
#define LIS3DH_TIME_LATENCY 0x3CU
#define LIS3DH_TIME_WINDOW 0x3DU
#define LIS3DH_ACT_THS 0x3EU
#define LIS3DH_ACT_DUR 0x3FU
/**
 * \}
 */

/**
 * \defgroup LIS3DH_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup LIS3DH_Private_Variables
 * \{
 */
bDRIVER_HALIF_TABLE(bLIS3DH_HalIf_t, DRIVER_NAME);

/**
 * \}
 */

/**
 * \defgroup LIS3DH_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup LIS3DH_Private_Functions
 * \{
 */

/**
 * \}
 */

/**
 * \addtogroup LIS3DH_Exported_Functions
 * \{
 */

int bLIS3DH_Init(bDriverInterface_t *pdrv)
{
    bDRIVER_STRUCT_INIT(pdrv, DRIVER_NAME, bLIS3DH_Init);

    if ((_bLis3dhGetID(pdrv)) != LIS3DH_ID)
    {
        return -1;
    }
    return 0;
}

bDRIVER_REG_INIT(B_DRIVER_LIS3DH, bLIS3DH_Init);

/**
 * \}
 */

/**
 * \}
 */

/**
 * \}
 */

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
