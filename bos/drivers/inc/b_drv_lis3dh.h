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
#include "b_drv_class_gsensor.h"
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

typedef bDriverInterface_t bLIS3DH_Driver_t; 

typedef struct {
  uint8_t not_used_01       : 7;
  uint8_t sdo_pu_disc       : 1;
}bLis3dhCTRL_REG0_t;

typedef struct {
  uint8_t xen               : 1;
  uint8_t yen               : 1;
  uint8_t zen               : 1;
  uint8_t lpen              : 1;
  uint8_t odr               : 4;
}bLis3dhCTRL_REG1_t;


typedef struct {
  uint8_t hp                : 3; /* HPCLICK + HP_IA2 + HP_IA1 -> HP */
  uint8_t fds               : 1;
  uint8_t hpcf              : 2;
  uint8_t hpm               : 2;
}bLis3dhCTRL_REG2_t;


typedef struct {
  uint8_t not_used_01       : 1;
  uint8_t i1_overrun        : 1;
  uint8_t i1_wtm            : 1;
  uint8_t not_used_02       : 1;
  uint8_t i1_zyxda          : 1;
  uint8_t i1_ia2            : 1;
  uint8_t i1_ia1            : 1;
  uint8_t i1_click          : 1;
}bLis3dhCTRL_REG3_t;

typedef enum
{
    LIS3DH_INT_CLICK   = 0x80,
    LIS3DH_INT_IA1     = 0x40,
    LIS3DH_INT_IA2     = 0x20,
    LIS3DH_INT_ZYXDA   = 0x10,
    LIS3DH_INT_321DA   = 0x08,
    LIS3DH_INT_WTM     = 0x04,
    LIS3DH_INT_OVERRUN = 0x02
}bLis3dhIntCfg_t;


typedef struct {
  uint8_t sim               : 1;
  uint8_t st                : 2;
  uint8_t hr                : 1;
  uint8_t fs                : 2;
  uint8_t ble               : 1;
  uint8_t bdu               : 1;
}bLis3dhCTRL_REG4_t;


typedef struct {
  uint8_t d4d_int2          : 1;
  uint8_t lir_int2          : 1;
  uint8_t d4d_int1          : 1;
  uint8_t lir_int1          : 1;
  uint8_t not_used_01       : 2;
  uint8_t fifo_en           : 1;
  uint8_t boot              : 1;
}bLis3dhCTRL_REG5_t;


typedef struct {
  uint8_t not_used_01       : 1;
  uint8_t int_polarity      : 1;
  uint8_t not_used_02       : 1;
  uint8_t i2_act            : 1;
  uint8_t i2_boot           : 1;
  uint8_t i2_ia2            : 1;
  uint8_t i2_ia1            : 1;
  uint8_t i2_click          : 1;
}bLis3dhCTRL_REG6_t;

typedef struct {
  uint8_t fth               : 5;
  uint8_t tr                : 1;
  uint8_t fm                : 2;
}bLis3dhFIFO_CtrlReg_t;

typedef struct {
  uint8_t fss               : 5;
  uint8_t empty             : 1;
  uint8_t ovrn_fifo         : 1;
  uint8_t wtm               : 1;
}bLis3dhFIFO_SrcReg_t;


typedef enum {
  LIS3DH_BYPASS_MODE           = 0,
  LIS3DH_FIFO_MODE             = 1,
  LIS3DH_DYNAMIC_STREAM_MODE   = 2,
  LIS3DH_STREAM_TO_FIFO_MODE   = 3,
}bLis3dhFIFO_Mode_t;


typedef enum {
  LIS3DH_POWER_DOWN                      = 0x00,
  LIS3DH_ODR_1Hz                         = 0x01,
  LIS3DH_ODR_10Hz                        = 0x02,
  LIS3DH_ODR_25Hz                        = 0x03,
  LIS3DH_ODR_50Hz                        = 0x04,
  LIS3DH_ODR_100Hz                       = 0x05,
  LIS3DH_ODR_200Hz                       = 0x06,
  LIS3DH_ODR_400Hz                       = 0x07,
  LIS3DH_ODR_1kHz620_LP                  = 0x08,
  LIS3DH_ODR_5kHz376_LP_1kHz344_NM_HP    = 0x09,
}bLis3dhODR_t;

#define HZ2ODR(hz)          ((hz == 1) ? 1 : ((hz == 10) ? 2 : ((hz <= 400) ? (2 + (hz / 25)) : ((hz == 1000) ? 8 : 9))))

typedef enum {
  LIS3DH_2g   = 0,
  LIS3DH_4g   = 1,
  LIS3DH_8g   = 2,
  LIS3DH_16g  = 3,
}bLis3dhFS_t;


typedef enum {
  LIS3DH_HR_12bit   = 0,            // High-resolution 
  LIS3DH_NM_10bit   = 1,            // Normal
  LIS3DH_LP_8bit    = 2,            // Low power
}bLis3dhOpMode_t;

typedef union
{
    int16_t i16bit;
    uint8_t u8bit[2];
}bLis3dhAxis_t;

typedef struct
{
    bLis3dhAxis_t x;
    bLis3dhAxis_t y;
    bLis3dhAxis_t z;
}bLis3dh3Axis_t;



typedef struct
{
    uint8_t fifo_enable;            // 0:disable   1:enable
    uint8_t fth;                    // Set FIFO watermark 0 <= fth <= 31
    bLis3dhODR_t    odr;            // Output data rates \ref bLis3dhODR_t
    bLis3dhFS_t     fs;             // Full scale \ref bLis3dhFS_t
    bLis3dhOpMode_t op_mode;        // Operating mode \ref bLis3dhOpMode_t
    bLis3dhFIFO_Mode_t fifo_mode;   // FIFO Mode \ref bLis3dhFIFO_Mode_t
}bLis3dhConfig_t;

/**
 * \}
 */
   

/** 
 * \defgroup LIS3DH_Exported_Defines
 * \{
 */
/** Device Identification (Who am I) **/
#define LIS3DH_ID                   0x33U

#define LIS3DH_OUT_ADC1_L           0x08U
#define LIS3DH_OUT_ADC1_H           0x09U
#define LIS3DH_OUT_ADC2_L           0x0AU
#define LIS3DH_OUT_ADC2_H           0x0BU
#define LIS3DH_OUT_ADC3_L           0x0CU
#define LIS3DH_OUT_ADC3_H           0x0DU
#define LIS3DH_WHO_AM_I             0x0FU
#define LIS3DH_CTRL_REG0            0x1EU
#define LIS3DH_TEMP_CFG_REG         0x1FU
#define LIS3DH_CTRL_REG1            0x20U
#define LIS3DH_CTRL_REG2            0x21U
#define LIS3DH_CTRL_REG3            0x22U
#define LIS3DH_CTRL_REG4            0x23U
#define LIS3DH_CTRL_REG5            0x24U
#define LIS3DH_CTRL_REG6            0x25U
#define LIS3DH_REFERENCE            0x26U
#define LIS3DH_STATUS_REG           0x27U
#define LIS3DH_OUT_X_L              0x28U
#define LIS3DH_OUT_X_H              0x29U
#define LIS3DH_OUT_Y_L              0x2AU
#define LIS3DH_OUT_Y_H              0x2BU
#define LIS3DH_OUT_Z_L              0x2CU
#define LIS3DH_OUT_Z_H              0x2DU
#define LIS3DH_FIFO_CTRL_REG        0x2EU
#define LIS3DH_FIFO_SRC_REG         0x2FU
#define LIS3DH_INT1_CFG             0x30U
#define LIS3DH_INT1_SRC             0x31U
#define LIS3DH_INT1_THS             0x32U
#define LIS3DH_INT1_DURATION        0x33U
#define LIS3DH_INT2_CFG             0x34U
#define LIS3DH_INT2_SRC             0x35U
#define LIS3DH_INT2_THS             0x36U 
#define LIS3DH_INT2_DURATION        0x37U
#define LIS3DH_CLICK_CFG            0x38U
#define LIS3DH_CLICK_SRC            0x39U
#define LIS3DH_CLICK_THS            0x3AU
#define LIS3DH_TIME_LIMIT           0x3BU
#define LIS3DH_TIME_LATENCY         0x3CU
#define LIS3DH_TIME_WINDOW          0x3DU
#define LIS3DH_ACT_THS              0x3EU
#define LIS3DH_ACT_DUR              0x3FU






#define LIS3DH_DEFAULT_CONFIG       {.fifo_enable = 1, \
                                    .fth = 24, \
                                    .odr = LIS3DH_ODR_25Hz, \
                                    .fs = LIS3DH_2g, \
                                    .op_mode = LIS3DH_LP_8bit,\
                                    .fifo_mode = LIS3DH_DYNAMIC_STREAM_MODE}


                                    
/**
 * \}
 */

/** 
 * \defgroup LIS3DH_Exported_Functions
 * \{
 */
void bLis3dhDataReadyCallback(bLis3dh3Axis_t *xyz, uint8_t number);

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



