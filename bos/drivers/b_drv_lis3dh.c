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

static bList3dhPrivate_t bLis3dhRunInfo[bDRIVER_HALIF_NUM(bLIS3DH_HalIf_t, DRIVER_NAME)];
static const int         Digit2mgTable[4][3] = {{1, 4, 16}, {2, 8, 32}, {4, 16, 64}, {12, 48, 192}};
static const int         DataShiftTable[3]   = {4, 6, 8};
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
static int _bLis3dhReadRegs(bDriverInterface_t *pdrv, uint8_t reg, uint8_t *data, uint16_t len)
{
    bDRIVER_GET_HALIF(_if, bLIS3DH_HalIf_t, pdrv);
    if (_if->is_spi)
    {
        reg |= 0xC0;
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 0);
        bHalSpiSend(&_if->_if._spi, &reg, 1);
        bHalSpiReceive(&_if->_if._spi, data, len);
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
    }
    else
    {
        reg = reg | 0x80;
        bHalI2CMemRead(&_if->_if._i2c, reg, data, len);
    }
    return 0;
}

static int _bLis3dhWriteRegs(bDriverInterface_t *pdrv, uint8_t reg, uint8_t *data, uint16_t len)
{
    bDRIVER_GET_HALIF(_if, bLIS3DH_HalIf_t, pdrv);
    if (_if->is_spi)
    {
        reg |= 0x40;
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 0);
        bHalSpiSend(&_if->_if._spi, &reg, 1);
        bHalSpiSend(&_if->_if._spi, data, len);
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
    }
    else
    {
        bHalI2CMemWrite(&_if->_if._i2c, reg, data, len);
    }
    return 0;
}

static uint8_t _bLis3dhGetID(bDriverInterface_t *pdrv)
{
    uint8_t id = 0;
    _bLis3dhReadRegs(pdrv, LIS3DH_WHO_AM_I, &id, 1);
    b_log("lis3dh id:%x\n", id);
    return id;
}

static void bLis3dhBlockDataUpdateSet(bDriverInterface_t *pdrv, uint8_t val)
{
    uint8_t reg = 0;
    _bLis3dhReadRegs(pdrv, LIS3DH_CTRL_REG4, &reg, 1);
    reg &= ~(0x1 << 7);
    reg |= (((val == 1) ? 0x1 : 0x0) << 7);
    _bLis3dhWriteRegs(pdrv, LIS3DH_CTRL_REG4, &reg, 1);
}

static void bLis3dhDataRateSet(bDriverInterface_t *pdrv, bLis3dhODR_t val)
{
    uint8_t reg = 0;
    bDRIVER_GET_PRIVATE(_private, bList3dhPrivate_t, pdrv);
    _bLis3dhReadRegs(pdrv, LIS3DH_CTRL_REG1, &reg, 1);
    reg &= 0x0f;
    reg |= (val) << 4;
    _bLis3dhWriteRegs(pdrv, LIS3DH_CTRL_REG1, &reg, 1);
    _private->odr = val;
}

static void bLis3dhFullScaleSet(bDriverInterface_t *pdrv, bLis3dhFS_t val)
{
    uint8_t reg = 0;
    bDRIVER_GET_PRIVATE(_private, bList3dhPrivate_t, pdrv);
    _bLis3dhReadRegs(pdrv, LIS3DH_CTRL_REG4, &reg, 1);
    reg &= ~((0x3) << 4);
    reg |= (val) << 4;
    _bLis3dhWriteRegs(pdrv, LIS3DH_CTRL_REG4, &reg, 1);
    _private->fs = val;
}

void bLis3dhOptModeSet(bDriverInterface_t *pdrv, bLis3dhOptMode_t val)
{
    uint8_t reg1 = 0, reg4 = 0;
    bDRIVER_GET_PRIVATE(_private, bList3dhPrivate_t, pdrv);
    _bLis3dhReadRegs(pdrv, LIS3DH_CTRL_REG1, &reg1, 1);
    _bLis3dhReadRegs(pdrv, LIS3DH_CTRL_REG4, &reg4, 1);

    reg1 &= ~(0x1 << 3);
    reg4 &= ~(0x1 << 3);

    if (val == LIS3DH_HR_12BIT)
    {
        reg4 |= (0x1 << 3);
    }

    if (val == LIS3DH_NM_10BIT)
    {
        ;
    }

    if (val == LIS3DH_LP_8BIT)
    {
        reg1 |= (0x1 << 3);
    }

    _bLis3dhWriteRegs(pdrv, LIS3DH_CTRL_REG1, &reg1, 1);
    _bLis3dhWriteRegs(pdrv, LIS3DH_CTRL_REG4, &reg4, 1);
    _private->opmode = val;
}

static void _bLis3dhDefaultCfg(bDriverInterface_t *pdrv)
{
    bLis3dhBlockDataUpdateSet(pdrv, 0);
    bLis3dhDataRateSet(pdrv, LIS3DH_ODR_10Hz);
    bLis3dhFullScaleSet(pdrv, LIS3DH_FS_4G);
    bLis3dhOptModeSet(pdrv, LIS3DH_HR_12BIT);
}

static void _bLis3dhGetRawData(bDriverInterface_t *pdrv, bGsensor3Axis_t *pval)
{
    uint8_t buff[6];
    _bLis3dhReadRegs(pdrv, LIS3DH_OUT_X_L, buff, 6);
    pval->x_mg = (int16_t)buff[1];
    pval->x_mg = (pval->x_mg * 256) + (int16_t)buff[0];
    pval->y_mg = (int16_t)buff[3];
    pval->y_mg = (pval->y_mg * 256) + (int16_t)buff[2];
    pval->z_mg = (int16_t)buff[5];
    pval->z_mg = (pval->z_mg * 256) + (int16_t)buff[4];
}

static int _bLis3dhRead(bDriverInterface_t *pdrv, uint32_t off, uint8_t *pbuf, uint32_t len)
{
    bDRIVER_GET_PRIVATE(_private, bList3dhPrivate_t, pdrv);
    bGsensor3Axis_t *pTemp = (bGsensor3Axis_t *)pbuf;
    int              i     = 0;
    uint8_t          c     = len / sizeof(bGsensor3Axis_t);

    for (i = 0; i < c; i++)
    {
        _bLis3dhGetRawData(pdrv, &pTemp[i]);
        pTemp[i].x_mg = (pTemp[i].x_mg >> DataShiftTable[_private->opmode]) *
                        Digit2mgTable[_private->fs][_private->opmode];
        pTemp[i].y_mg = (pTemp[i].y_mg >> DataShiftTable[_private->opmode]) *
                        Digit2mgTable[_private->fs][_private->opmode];
        pTemp[i].z_mg = (pTemp[i].z_mg >> DataShiftTable[_private->opmode]) *
                        Digit2mgTable[_private->fs][_private->opmode];
    }
    return (c * sizeof(bGsensor3Axis_t));
}

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
    pdrv->read        = _bLis3dhRead;
    pdrv->_private._p = &bLis3dhRunInfo[pdrv->drv_no];

    if ((_bLis3dhGetID(pdrv)) != LIS3DH_ID)
    {
        return -1;
    }
    _bLis3dhDefaultCfg(pdrv);
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
