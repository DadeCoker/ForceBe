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
#include "b_drv_l3gd20.h"

#include <string.h>
/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup L3GD20
 * \{
 */

/**
 * \defgroup L3GD20_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup L3GD20_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup L3GD20_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup L3GD20_Private_Variables
 * \{
 */

bL3GD20_Driver_t bL3GD20_Driver;

static bL3gd20Config_t bL3gd20Config = L3GD20_DEFAULT_CONFIG;
static bGsensor3Axis_t bL3gd20FifoValue[32];
static const int       Digit2mgTable[4][3] = {{1, 4, 16}, {2, 8, 32}, {4, 16, 64}, {12, 48, 192}};

static const int        DataShiftTable[3] = {4, 6, 8};
static volatile uint8_t IntFlag           = 1;

/**
 * \}
 */

/**
 * \defgroup L3GD20_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup L3GD20_Private_Functions
 * \{
 */
static int _bL3gd20ReadRegs(uint8_t reg, uint8_t *data, uint16_t len)
{
#if HAL_L3GD20_I2C_ENABLE
    reg = reg | 0x80;
    bHalI2C_MemRead(HAL_L3GD20_I2C, HAL_L3GD20_I2C_ADDR, reg, data, len);
#else
    reg |= 0xC0;
    bHalGPIO_WritePin(HAL_L3GD20_CS_PORT, HAL_L3GD20_CS_PIN, 0);
    bHalSPI_Send(B_HAL_SPI_2, &reg, 1);
    bHalSPI_Receive(B_HAL_SPI_2, data, len);
    bHalGPIO_WritePin(HAL_L3GD20_CS_PORT, HAL_L3GD20_CS_PIN, 1);
#endif
    return 0;
}

static int _bL3gd20WriteRegs(uint8_t reg, uint8_t *data, uint16_t len)
{
#if HAL_L3GD20_I2C_ENABLE
    bHalI2C_MemWrite(HAL_L3GD20_I2C, HAL_L3GD20_I2C_ADDR, reg, data, len);
#else
    reg |= 0x40;
    bHalGPIO_WritePin(HAL_L3GD20_CS_PORT, HAL_L3GD20_CS_PIN, 0);
    bHalSPI_Send(B_HAL_SPI_2, &reg, 1);
    bHalSPI_Send(B_HAL_SPI_2, data, len);
    bHalGPIO_WritePin(HAL_L3GD20_CS_PORT, HAL_L3GD20_CS_PIN, 1);
#endif
    return 0;
}

static uint8_t _bL3gd20GetID()
{
    uint8_t id = 0;
    _bL3gd20ReadRegs(L3GD20_WHO_AM_I, &id, 1);
    return id;
}

// change the values of bdu in reg CTRL_REG4
static int _bL3gd20BlockDataUpdateSet(uint8_t val)
{
    int                retval = 0;
    bL3gd20CTRL_REG4_t ctrl_reg4;
    retval = _bL3gd20ReadRegs(L3GD20_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
    if (retval == 0)
    {
        ctrl_reg4.bdu = val;
        retval        = _bL3gd20WriteRegs(L3GD20_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
    }
    return retval;
}

// Output data rate selection
static int _bL3gd20ODR_Set(bL3gd20ODR_t odr)
{
    int                retval = 0;
    bL3gd20CTRL_REG1_t ctrl_reg1;
    retval = _bL3gd20ReadRegs(L3GD20_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
    if (retval == 0)
    {
        ctrl_reg1.odr = odr;
        retval        = _bL3gd20WriteRegs(L3GD20_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
    }
    return retval;
}

// Full-scale configuration.
static int _bL3gd20FullScaleSet(bL3gd20FS_t val)
{
    int                retval = 0;
    bL3gd20CTRL_REG4_t ctrl_reg4;

    retval = _bL3gd20ReadRegs(L3GD20_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
    if (retval == 0)
    {
        ctrl_reg4.fs = val;
        retval       = _bL3gd20WriteRegs(L3GD20_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
    }
    return retval;
}

// Operating mode selection.
static int _bL3gd20OpModeSet(bL3gd20OpMode_t val)
{
    bL3gd20CTRL_REG1_t ctrl_reg1;
    bL3gd20CTRL_REG4_t ctrl_reg4;
    int                retval = 0;

    retval = _bL3gd20ReadRegs(L3GD20_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
    if (retval < 0)
    {
        return retval;
    }

    retval = _bL3gd20ReadRegs(L3GD20_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
    if (retval < 0)
    {
        return retval;
    }

    if (val == L3GD20_HR_12bit)
    {
        ctrl_reg1.lpen = 0;
        ctrl_reg4.hr   = 1;
    }

    if (val == L3GD20_NM_10bit)
    {
        ctrl_reg1.lpen = 0;
        ctrl_reg4.hr   = 0;
    }

    if (val == L3GD20_LP_8bit)
    {
        ctrl_reg1.lpen = 1;
        ctrl_reg4.hr   = 0;
    }

    retval = _bL3gd20WriteRegs(L3GD20_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
    if (retval < 0)
    {
        return retval;
    }

    retval = _bL3gd20WriteRegs(L3GD20_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
    return retval;
}

// FIFO configuration.
static int _bL3gd20FIFO_Set(uint8_t fth, bL3gd20FIFO_Mode_t mode, uint8_t enable)
{
    bL3gd20FIFO_CtrlReg_t fifo_ctrl_reg;
    bL3gd20CTRL_REG5_t    ctrl_reg5;
    int                   retval = 0;
    retval = _bL3gd20ReadRegs(L3GD20_FIFO_CTRL_REG, (uint8_t *)&fifo_ctrl_reg, 1);
    if (retval < 0)
    {
        return retval;
    }
    fifo_ctrl_reg.fth = fth;
    fifo_ctrl_reg.fm  = mode;
    retval            = _bL3gd20WriteRegs(L3GD20_FIFO_CTRL_REG, (uint8_t *)&fifo_ctrl_reg, 1);
    if (retval < 0)
    {
        return retval;
    }

    retval = _bL3gd20ReadRegs(L3GD20_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);
    if (retval < 0)
    {
        return retval;
    }
    ctrl_reg5.fifo_en = enable;
    retval            = _bL3gd20WriteRegs(L3GD20_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);
    return retval;
}

// Int configuration.
static int _bL3gd20IntSet(bL3gd20IntCfg_t val)
{
    int     retval = 0;
    uint8_t ctrl_reg3;

    retval = _bL3gd20ReadRegs(L3GD20_CTRL_REG3, &ctrl_reg3, 1);
    if (retval == 0)
    {
        ctrl_reg3 = val;
        retval    = _bL3gd20WriteRegs(L3GD20_CTRL_REG3, &ctrl_reg3, 1);
    }
    return retval;
}

//--------------------------------------------------------------------------------
// len has to be a multiple of sizeof(bGsensor3Axis_t)
static int _bL3gd20Read(bL3GD20_Driver_t *pdrv, uint32_t off, uint8_t *pbuf, uint16_t len)
{
    uint8_t c     = len / sizeof(bGsensor3Axis_t);
    uint8_t c_off = off / sizeof(bGsensor3Axis_t);
    if (c == 0 || c_off > bL3gd20Config.fth)
    {
        return 0;
    }
    if ((c_off + c) > (bL3gd20Config.fth + 1))
    {
        c = (bL3gd20Config.fth + 1) - c_off;
    }
    memcpy(pbuf, (uint8_t *)&bL3gd20FifoValue[c_off], c * sizeof(bGsensor3Axis_t));
    return (c * sizeof(bGsensor3Axis_t));
}

static int _bL3gd20Ctl(bL3GD20_Driver_t *pdrv, uint8_t cmd, void *param)
{
    int retval = -1;
    switch (cmd)
    {
        case bCMD_CFG_ODR:
        {
            uint16_t     hz  = ((uint16_t *)param)[0];
            bL3gd20ODR_t odr = (bL3gd20ODR_t)HZ2ODR(hz);
            _bL3gd20ODR_Set(odr);
            bL3gd20Config.odr = odr;
            retval            = 0;
        }
        break;
        case bCMD_CFG_FS:
        {
            uint16_t    g  = ((uint16_t *)param)[0];
            bL3gd20FS_t fs = (bL3gd20FS_t)G2FS(g);
            _bL3gd20FullScaleSet(fs);
            bL3gd20Config.fs = fs;
            retval           = 0;
        }
        break;
    }
    return retval;
}

static void _bL3gd20Polling()
{
    bL3gd20FIFO_SrcReg_t fifo_src_reg;
    int                  i   = 0;
    uint8_t              fss = 0;
    if (IntFlag)
    {
        IntFlag = 0;
        _bL3gd20ReadRegs(L3GD20_FIFO_SRC_REG, (uint8_t *)&fifo_src_reg, 1);
        fss = (uint8_t)fifo_src_reg.fss;
        for (i = 0; i <= fss; i++)
        {
            _bL3gd20ReadRegs(L3GD20_OUT_X_L, (uint8_t *)&bL3gd20FifoValue[i], 6);
            bL3gd20FifoValue[i].x_mg =
                (bL3gd20FifoValue[i].x_mg >> DataShiftTable[bL3gd20Config.op_mode]) *
                Digit2mgTable[bL3gd20Config.fs][bL3gd20Config.op_mode];
            bL3gd20FifoValue[i].y_mg =
                (bL3gd20FifoValue[i].y_mg >> DataShiftTable[bL3gd20Config.op_mode]) *
                Digit2mgTable[bL3gd20Config.fs][bL3gd20Config.op_mode];
            bL3gd20FifoValue[i].z_mg =
                (bL3gd20FifoValue[i].z_mg >> DataShiftTable[bL3gd20Config.op_mode]) *
                Digit2mgTable[bL3gd20Config.fs][bL3gd20Config.op_mode];
        }
        bGsensorL3gd20Callback(bL3gd20FifoValue, fss + 1);
    }
}
/**
 * \}
 */

/**
 * \addtogroup L3GD20_Exported_Functions
 * \{
 */

#if __GNUC__
void __attribute__((weak)) bGsensorL3gd20Callback(bGsensor3Axis_t *xyz, uint8_t number)
#else
__weak void bGsensorL3gd20Callback(bGsensor3Axis_t *xyz, uint8_t number)
#endif
{
    int i = 0;
    for (i = 0; i < number; i++)
    {
        b_log("x:%d y:%d z:%d\r\n", xyz[i].x_mg, xyz[i].y_mg, xyz[i].z_mg);
    }
		IntFlag = 1;
}

int bL3GD20_Init()
{
    uint8_t id = 0;
    if ((id = _bL3gd20GetID()) != L3GD20_ID)
    {
        b_log_e("->id:%x\r\n", id);
        bL3GD20_Driver.status = -1;
        return -1;
    }
    _bL3gd20BlockDataUpdateSet(0);
    _bL3gd20ODR_Set(bL3gd20Config.odr);
    _bL3gd20FullScaleSet(bL3gd20Config.fs);
    _bL3gd20OpModeSet(bL3gd20Config.op_mode);
    _bL3gd20FIFO_Set(bL3gd20Config.fth, bL3gd20Config.fifo_mode, bL3gd20Config.fifo_enable);
    _bL3gd20IntSet(L3GD20_INT_WTM);
    bL3GD20_Driver.status = 0;
    bL3GD20_Driver.read   = _bL3gd20Read;
    bL3GD20_Driver.write  = NULL;
    bL3GD20_Driver.open   = NULL;
    bL3GD20_Driver.close  = NULL;
    bL3GD20_Driver.ctl    = _bL3gd20Ctl;
    return 0;
}

BOS_REG_POLLING_FUNC(_bL3gd20Polling);
bDRIVER_REG_INIT(bL3GD20_Init);

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
