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
HALIF_KEYWORD bLIS3DH_HalIf_t bLIS3DH_HalIf = HAL_LIS3DH_IF;
bLIS3DH_Driver_t              bLIS3DH_Driver;

static bLis3dhConfig_t bLis3dhConfig       = LIS3DH_DEFAULT_CONFIG;
static const int       Digit2mgTable[4][3] = {{1, 4, 16}, {2, 8, 32}, {4, 16, 64}, {12, 48, 192}};
static const int       DataShiftTable[3]   = {4, 6, 8};

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
static int _bLis3dhReadRegs(uint8_t reg, uint8_t *data, uint16_t len)
{
    if (bLIS3DH_HalIf.is_spi)
    {
        reg |= 0xC0;
        bHalGpioWritePin(bLIS3DH_HalIf._if._spi.cs.port, bLIS3DH_HalIf._if._spi.cs.pin, 0);
        bHalSpiSend(&bLIS3DH_HalIf._if._spi, &reg, 1);
        bHalSpiReceive(&bLIS3DH_HalIf._if._spi, data, len);
        bHalGpioWritePin(bLIS3DH_HalIf._if._spi.cs.port, bLIS3DH_HalIf._if._spi.cs.pin, 1);
    }
    else
    {
        reg = reg | 0x80;
        bHalI2CMemRead(&bLIS3DH_HalIf._if._i2c, reg, data, len);
    }
    return 0;
}

static int _bLis3dhWriteRegs(uint8_t reg, uint8_t *data, uint16_t len)
{
    if (bLIS3DH_HalIf.is_spi)
    {
        reg |= 0x40;
        bHalGpioWritePin(bLIS3DH_HalIf._if._spi.cs.port, bLIS3DH_HalIf._if._spi.cs.pin, 0);
        bHalSpiSend(&bLIS3DH_HalIf._if._spi, &reg, 1);
        bHalSpiSend(&bLIS3DH_HalIf._if._spi, data, len);
        bHalGpioWritePin(bLIS3DH_HalIf._if._spi.cs.port, bLIS3DH_HalIf._if._spi.cs.pin, 1);
    }
    else
    {
        bHalI2CMemWrite(&bLIS3DH_HalIf._if._i2c, reg, data, len);
    }
    return 0;
}

static uint8_t _bLis3dhGetID()
{
    uint8_t id = 0;
    _bLis3dhReadRegs(LIS3DH_WHO_AM_I, &id, 1);
    return id;
}

// change the values of bdu in reg CTRL_REG4
static int _bLis3dhBlockDataUpdateSet(uint8_t val)
{
    int               retval = 0;
    bLis3dhCtrlReg4_t ctrl_reg4;
    retval = _bLis3dhReadRegs(LIS3DH_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
    if (retval == 0)
    {
        ctrl_reg4.bdu = val;
        retval        = _bLis3dhWriteRegs(LIS3DH_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
    }
    return retval;
}

// Output data rate selection
static int _bLis3dhODR_Set(bLis3dhODR_t odr)
{
    int               retval = 0;
    bLis3dhCtrlReg1_t ctrl_reg1;
    retval = _bLis3dhReadRegs(LIS3DH_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
    if (retval == 0)
    {
        ctrl_reg1.odr = odr;
        retval        = _bLis3dhWriteRegs(LIS3DH_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
    }
    return retval;
}

// Full-scale configuration.
static int _bLis3dhFullScaleSet(bLis3dhFS_t val)
{
    int               retval = 0;
    bLis3dhCtrlReg4_t ctrl_reg4;

    retval = _bLis3dhReadRegs(LIS3DH_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
    if (retval == 0)
    {
        ctrl_reg4.fs = val;
        retval       = _bLis3dhWriteRegs(LIS3DH_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
    }
    return retval;
}

// Operating mode selection.
static int _bLis3dhOpModeSet(bLis3dhOpMode_t val)
{
    bLis3dhCtrlReg1_t ctrl_reg1;
    bLis3dhCtrlReg4_t ctrl_reg4;
    int               retval = 0;

    retval = _bLis3dhReadRegs(LIS3DH_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
    if (retval < 0)
    {
        return retval;
    }

    retval = _bLis3dhReadRegs(LIS3DH_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
    if (retval < 0)
    {
        return retval;
    }

    if (val == LIS3DH_HR_12bit)
    {
        ctrl_reg1.lpen = 0;
        ctrl_reg4.hr   = 1;
    }

    if (val == LIS3DH_NM_10bit)
    {
        ctrl_reg1.lpen = 0;
        ctrl_reg4.hr   = 0;
    }

    if (val == LIS3DH_LP_8bit)
    {
        ctrl_reg1.lpen = 1;
        ctrl_reg4.hr   = 0;
    }

    retval = _bLis3dhWriteRegs(LIS3DH_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
    if (retval < 0)
    {
        return retval;
    }

    retval = _bLis3dhWriteRegs(LIS3DH_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
    return retval;
}

// FIFO configuration.
static int _bLis3dhFIFO_Set(uint8_t fth, bLis3dhFifoMode_t mode, uint8_t enable)
{
    bLis3dhFifoCtrlReg_t fifo_ctrl_reg;
    bLis3dhCtrlReg5_t    ctrl_reg5;
    int                  retval = 0;
    retval = _bLis3dhReadRegs(LIS3DH_FIFO_CTRL_REG, (uint8_t *)&fifo_ctrl_reg, 1);
    if (retval < 0)
    {
        return retval;
    }
    fifo_ctrl_reg.fth = fth;
    fifo_ctrl_reg.fm  = mode;
    retval            = _bLis3dhWriteRegs(LIS3DH_FIFO_CTRL_REG, (uint8_t *)&fifo_ctrl_reg, 1);
    if (retval < 0)
    {
        return retval;
    }

    retval = _bLis3dhReadRegs(LIS3DH_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);
    if (retval < 0)
    {
        return retval;
    }
    ctrl_reg5.fifo_en = enable;
    retval            = _bLis3dhWriteRegs(LIS3DH_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);
    return retval;
}

// Int configuration.
static int _bLis3dhIntSet(bLis3dhIntCfg_t val)
{
    int     retval = 0;
    uint8_t ctrl_reg3;

    retval = _bLis3dhReadRegs(LIS3DH_CTRL_REG3, &ctrl_reg3, 1);
    if (retval == 0)
    {
        ctrl_reg3 = val;
        retval    = _bLis3dhWriteRegs(LIS3DH_CTRL_REG3, &ctrl_reg3, 1);
    }
    return retval;
}

static int _bLis3dhCtl(bLIS3DH_Driver_t *pdrv, uint8_t cmd, void *param)
{
    int retval = -1;
    switch (cmd)
    {
        case bCMD_CFG_ODR:
        {
            uint16_t     hz  = ((uint16_t *)param)[0];
            bLis3dhODR_t odr = (bLis3dhODR_t)HZ2ODR(hz);
            _bLis3dhODR_Set(odr);
            bLis3dhConfig.odr = odr;
            retval            = 0;
        }
        break;
        case bCMD_CFG_FS:
        {
            uint16_t    g  = ((uint16_t *)param)[0];
            bLis3dhFS_t fs = (bLis3dhFS_t)G2FS(g);
            _bLis3dhFullScaleSet(fs);
            bLis3dhConfig.fs = fs;
            retval           = 0;
        }
        break;
        case bCMD_CFG_FIFO:
        {
            bGSensorFifo_t *fifo_cfg = (bGSensorFifo_t *)param;
            if (fifo_cfg->fifo_length == 0 || fifo_cfg->fifo_length > 32 ||
                fifo_cfg->fifo_mode > LIS3DH_STREAM_TO_FIFO_MODE)
            {
                break;
            }
            _bLis3dhFIFO_Set(fifo_cfg->fifo_length - 1, (bLis3dhFifoMode_t)(fifo_cfg->fifo_mode),
                             fifo_cfg->fifo_en != 0);
            retval = 0;
        }
        break;
    }
    return retval;
}

static int _bLis3dhRead(bLIS3DH_Driver_t *pdrv, uint32_t off, uint8_t *pbuf, uint16_t len)
{
    bLis3dhFifoSrcReg_t fifo_src_reg;
    bGsensor3Axis_t *   pTemp = (bGsensor3Axis_t *)pbuf;
    int                 i     = 0;
    uint8_t             fss   = 0;
    uint8_t             c     = len / sizeof(bGsensor3Axis_t);

    _bLis3dhReadRegs(LIS3DH_FIFO_SRC_REG, (uint8_t *)&fifo_src_reg, 1);
    fss = (uint8_t)fifo_src_reg.fss;
    if (c > (fss + 1))
    {
        c = fss + 1;
    }
    for (i = 0; i < c; i++)
    {
        _bLis3dhReadRegs(LIS3DH_OUT_X_L, (uint8_t *)&pTemp[i], 6);
        pTemp[i].x_mg = (pTemp[i].x_mg >> DataShiftTable[bLis3dhConfig.op_mode]) *
                        Digit2mgTable[bLis3dhConfig.fs][bLis3dhConfig.op_mode];
        pTemp[i].y_mg = (pTemp[i].y_mg >> DataShiftTable[bLis3dhConfig.op_mode]) *
                        Digit2mgTable[bLis3dhConfig.fs][bLis3dhConfig.op_mode];
        pTemp[i].z_mg = (pTemp[i].z_mg >> DataShiftTable[bLis3dhConfig.op_mode]) *
                        Digit2mgTable[bLis3dhConfig.fs][bLis3dhConfig.op_mode];
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

int bLIS3DH_Init()
{
    uint8_t id = 0;

    bLIS3DH_Driver.status  = 0;
    bLIS3DH_Driver.init    = bLIS3DH_Init;
    bLIS3DH_Driver.read    = _bLis3dhRead;
    bLIS3DH_Driver.write   = NULL;
    bLIS3DH_Driver.open    = NULL;
    bLIS3DH_Driver.close   = NULL;
    bLIS3DH_Driver.ctl     = _bLis3dhCtl;
    bLIS3DH_Driver._hal_if = (void *)&bLIS3DH_HalIf;

    if ((id = _bLis3dhGetID()) != LIS3DH_ID)
    {
        b_log_e("id:%x\r\n", id);
        id                    = id;
        bLIS3DH_Driver.status = -1;
        return -1;
    }
    _bLis3dhBlockDataUpdateSet(0);
    _bLis3dhODR_Set(bLis3dhConfig.odr);
    _bLis3dhFullScaleSet(bLis3dhConfig.fs);
    _bLis3dhOpModeSet(bLis3dhConfig.op_mode);
    _bLis3dhFIFO_Set(bLis3dhConfig.fth, bLis3dhConfig.fifo_mode, bLis3dhConfig.fifo_enable);
    _bLis3dhIntSet(LIS3DH_INT_WTM);
    return 0;
}

bDRIVER_REG_INIT(bLIS3DH_Init);

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
