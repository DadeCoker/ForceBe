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
#ifndef __B_DRV_L3GD20_H__
#define __B_DRV_L3GD20_H__

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
 * \addtogroup L3GD20
 * \{
 */

/**
 * \defgroup L3GD20_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
    union
    {
        struct
        {
            bHalI2CNumber_t iic;
            uint8_t         addr;
        } _iic;
        struct
        {
            bHalSPINumber_t    spi;
            bHalGPIOInstance_t cs;
        } _spi;
    } _if;
    uint8_t is_spi;
    uint8_t exti_line;
} bL3GD20_HalIf_t;

typedef bDriverInterface_t bL3GD20_Driver_t;

typedef struct
{
    uint8_t not_used_01 : 7;
    uint8_t sdo_pu_disc : 1;
} bL3gd20CTRL_REG0_t;

typedef struct
{
    uint8_t xen : 1;
    uint8_t yen : 1;
    uint8_t zen : 1;
    uint8_t lpen : 1;
    uint8_t odr : 4;
} bL3gd20CTRL_REG1_t;

typedef struct
{
    uint8_t hp : 3; /* HPCLICK + HP_IA2 + HP_IA1 -> HP */
    uint8_t fds : 1;
    uint8_t hpcf : 2;
    uint8_t hpm : 2;
} bL3gd20CTRL_REG2_t;

typedef struct
{
    uint8_t not_used_01 : 1;
    uint8_t i1_overrun : 1;
    uint8_t i1_wtm : 1;
    uint8_t not_used_02 : 1;
    uint8_t i1_zyxda : 1;
    uint8_t i1_ia2 : 1;
    uint8_t i1_ia1 : 1;
    uint8_t i1_click : 1;
} bL3gd20CTRL_REG3_t;

typedef enum
{
    L3GD20_INT_CLICK   = 0x80,
    L3GD20_INT_IA1     = 0x40,
    L3GD20_INT_IA2     = 0x20,
    L3GD20_INT_ZYXDA   = 0x10,
    L3GD20_INT_321DA   = 0x08,
    L3GD20_INT_WTM     = 0x04,
    L3GD20_INT_OVERRUN = 0x02
} bL3gd20IntCfg_t;

typedef struct
{
    uint8_t sim : 1;
    uint8_t st : 2;
    uint8_t hr : 1;
    uint8_t fs : 2;
    uint8_t ble : 1;
    uint8_t bdu : 1;
} bL3gd20CTRL_REG4_t;

typedef struct
{
    uint8_t d4d_int2 : 1;
    uint8_t lir_int2 : 1;
    uint8_t d4d_int1 : 1;
    uint8_t lir_int1 : 1;
    uint8_t not_used_01 : 2;
    uint8_t fifo_en : 1;
    uint8_t boot : 1;
} bL3gd20CTRL_REG5_t;

typedef struct
{
    uint8_t not_used_01 : 1;
    uint8_t int_polarity : 1;
    uint8_t not_used_02 : 1;
    uint8_t i2_act : 1;
    uint8_t i2_boot : 1;
    uint8_t i2_ia2 : 1;
    uint8_t i2_ia1 : 1;
    uint8_t i2_click : 1;
} bL3gd20CTRL_REG6_t;

typedef struct
{
    uint8_t fth : 5;
    uint8_t tr : 1;
    uint8_t fm : 2;
} bL3gd20FIFO_CtrlReg_t;

typedef struct
{
    uint8_t fss : 5;
    uint8_t empty : 1;
    uint8_t ovrn_fifo : 1;
    uint8_t wtm : 1;
} bL3gd20FIFO_SrcReg_t;

typedef enum
{
    L3GD20_BYPASS_MODE         = 0,
    L3GD20_FIFO_MODE           = 1,
    L3GD20_DYNAMIC_STREAM_MODE = 2,
    L3GD20_STREAM_TO_FIFO_MODE = 3,
} bL3gd20FIFO_Mode_t;

typedef enum
{
    L3GD20_POWER_DOWN                   = 0x00,
    L3GD20_ODR_1Hz                      = 0x01,
    L3GD20_ODR_10Hz                     = 0x02,
    L3GD20_ODR_25Hz                     = 0x03,
    L3GD20_ODR_50Hz                     = 0x04,
    L3GD20_ODR_100Hz                    = 0x05,
    L3GD20_ODR_200Hz                    = 0x06,
    L3GD20_ODR_400Hz                    = 0x07,
    L3GD20_ODR_1kHz620_LP               = 0x08,
    L3GD20_ODR_5kHz376_LP_1kHz344_NM_HP = 0x09,
} bL3gd20ODR_t;

#define HZ2ODR(hz) \
    ((hz == 1) ? 1 : ((hz == 10) ? 2 : ((hz <= 400) ? (2 + (hz / 25)) : ((hz == 1000) ? 8 : 9))))
#define G2FS(g) ((g == 2) ? 0 : ((g == 4) ? 1 : ((g == 8) ? 2 : 3)))

typedef enum
{
    L3GD20_2g  = 0,
    L3GD20_4g  = 1,
    L3GD20_8g  = 2,
    L3GD20_16g = 3,
} bL3gd20FS_t;

typedef enum
{
    L3GD20_HR_12bit = 0,  // High-resolution
    L3GD20_NM_10bit = 1,  // Normal
    L3GD20_LP_8bit  = 2,  // Low power
} bL3gd20OpMode_t;

typedef struct
{
    uint8_t            fifo_enable;  // 0:disable   1:enable
    uint8_t            fth;          // Set FIFO watermark 0 <= fth <= 31
    bL3gd20ODR_t       odr;          // Output data rates \ref bL3gd20ODR_t
    bL3gd20FS_t        fs;           // Full scale \ref bL3gd20FS_t
    bL3gd20OpMode_t    op_mode;      // Operating mode \ref bL3gd20OpMode_t
    bL3gd20FIFO_Mode_t fifo_mode;    // FIFO Mode \ref bL3gd20FIFO_Mode_t
} bL3gd20Config_t;

/**
 * \}
 */

/**
 * \defgroup L3GD20_Exported_Defines
 * \{
 */
/** Device Identification (Who am I) **/
#define L3GD20_ID 0xD4U

#define L3GD20_OUT_ADC1_L 0x08U
#define L3GD20_OUT_ADC1_H 0x09U
#define L3GD20_OUT_ADC2_L 0x0AU
#define L3GD20_OUT_ADC2_H 0x0BU
#define L3GD20_OUT_ADC3_L 0x0CU
#define L3GD20_OUT_ADC3_H 0x0DU
#define L3GD20_WHO_AM_I 0x0FU
#define L3GD20_CTRL_REG0 0x1EU
#define L3GD20_TEMP_CFG_REG 0x1FU
#define L3GD20_CTRL_REG1 0x20U
#define L3GD20_CTRL_REG2 0x21U
#define L3GD20_CTRL_REG3 0x22U
#define L3GD20_CTRL_REG4 0x23U
#define L3GD20_CTRL_REG5 0x24U
#define L3GD20_CTRL_REG6 0x25U
#define L3GD20_REFERENCE 0x26U
#define L3GD20_STATUS_REG 0x27U
#define L3GD20_OUT_X_L 0x28U
#define L3GD20_OUT_X_H 0x29U
#define L3GD20_OUT_Y_L 0x2AU
#define L3GD20_OUT_Y_H 0x2BU
#define L3GD20_OUT_Z_L 0x2CU
#define L3GD20_OUT_Z_H 0x2DU
#define L3GD20_FIFO_CTRL_REG 0x2EU
#define L3GD20_FIFO_SRC_REG 0x2FU
#define L3GD20_INT1_CFG 0x30U
#define L3GD20_INT1_SRC 0x31U
#define L3GD20_INT1_THS 0x32U
#define L3GD20_INT1_DURATION 0x33U
#define L3GD20_INT2_CFG 0x34U
#define L3GD20_INT2_SRC 0x35U
#define L3GD20_INT2_THS 0x36U
#define L3GD20_INT2_DURATION 0x37U
#define L3GD20_CLICK_CFG 0x38U
#define L3GD20_CLICK_SRC 0x39U
#define L3GD20_CLICK_THS 0x3AU
#define L3GD20_TIME_LIMIT 0x3BU
#define L3GD20_TIME_LATENCY 0x3CU
#define L3GD20_TIME_WINDOW 0x3DU
#define L3GD20_ACT_THS 0x3EU
#define L3GD20_ACT_DUR 0x3FU

#define L3GD20_DEFAULT_CONFIG                                                 \
    {                                                                         \
        .fifo_enable = 1, .fth = 12, .odr = L3GD20_ODR_25Hz, .fs = L3GD20_2g, \
        .op_mode = L3GD20_LP_8bit, .fifo_mode = L3GD20_DYNAMIC_STREAM_MODE    \
    }

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
