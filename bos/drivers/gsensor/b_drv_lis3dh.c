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
#include "b_drv_lis3dh.h"
#include <string.h>
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

bLIS3DH_Driver_t bLIS3DH_Driver;

static bLis3dhConfig_t bLis3dhConfig = LIS3DH_DEFAULT_CONFIG;
static bGsensor3Axis_t bLis3dhFifoValue[32];
static const int Digit2mgTable[4][3] = {
    {1, 4, 16},
    {2, 8, 32},
    {4, 16, 64},
    {12, 48, 192}
};

static const int DataShiftTable[3] = {4, 6, 8};
static volatile uint8_t IntFlag = 1;

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
static int _bLis3dhReadRegs(uint8_t reg, uint8_t* data, uint16_t len)
{
#if HAL_LIS3DH_I2C_ENABLE
    reg = reg | 0x80;
    bHalI2C_MemRead(HAL_LIS3DH_I2C, HAL_LIS3DH_I2C_ADDR, reg, data, len);
#else    
    reg |= 0xC0;
    bHalGPIO_WritePin(HAL_LIS3DH_CS_PORT, HAL_LIS3DH_CS_PIN, 0);
    bHalSPI_Send(HAL_LIS3DH_SPI, &reg, 1);
    bHalSPI_Receive(HAL_LIS3DH_SPI, data, len);
    bHalGPIO_WritePin(HAL_LIS3DH_CS_PORT, HAL_LIS3DH_CS_PIN, 1);
#endif    
    return 0;
}

static int _bLis3dhWriteRegs(uint8_t reg, uint8_t* data, uint16_t len)
{
#if HAL_LIS3DH_I2C_ENABLE
    bHalI2C_MemWrite(HAL_LIS3DH_I2C, HAL_LIS3DH_I2C_ADDR, reg, data, len);
#else    
    reg |= 0x40;
    bHalGPIO_WritePin(HAL_LIS3DH_CS_PORT, HAL_LIS3DH_CS_PIN, 0);
    bHalSPI_Send(HAL_LIS3DH_SPI, &reg, 1);
    bHalSPI_Send(HAL_LIS3DH_SPI, data, len);
    bHalGPIO_WritePin(HAL_LIS3DH_CS_PORT, HAL_LIS3DH_CS_PIN, 1);
#endif    
    return 0;
}

static uint8_t _bLis3dhGetID()
{
    uint8_t id = 0;
    _bLis3dhReadRegs(LIS3DH_WHO_AM_I, &id, 1);
    return id;
}

//change the values of bdu in reg CTRL_REG4
static int _bLis3dhBlockDataUpdateSet(uint8_t val)
{
    int retval = 0;
    bLis3dhCTRL_REG4_t ctrl_reg4;
    retval = _bLis3dhReadRegs(LIS3DH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);
    if(retval == 0)
    {
        ctrl_reg4.bdu = val;
        retval = _bLis3dhWriteRegs(LIS3DH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);
    }
    return retval;
}

//Output data rate selection
static int _bLis3dhODR_Set(bLis3dhODR_t odr)
{
    int retval = 0;
    bLis3dhCTRL_REG1_t ctrl_reg1;
    retval = _bLis3dhReadRegs(LIS3DH_CTRL_REG1, (uint8_t*)&ctrl_reg1, 1);
    if(retval == 0)
    {
        ctrl_reg1.odr = odr;
        retval = _bLis3dhWriteRegs(LIS3DH_CTRL_REG1, (uint8_t*)&ctrl_reg1, 1);
    }
    return retval;
}

//Full-scale configuration.
static int _bLis3dhFullScaleSet(bLis3dhFS_t val)
{
    int retval = 0;
    bLis3dhCTRL_REG4_t ctrl_reg4;

    retval = _bLis3dhReadRegs(LIS3DH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);
    if(retval == 0)
    {
        ctrl_reg4.fs = val;
        retval = _bLis3dhWriteRegs(LIS3DH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);
    }
    return retval;    
}

//Operating mode selection.
static int _bLis3dhOpModeSet(bLis3dhOpMode_t val)
{
    bLis3dhCTRL_REG1_t ctrl_reg1;
    bLis3dhCTRL_REG4_t ctrl_reg4;
    int retval = 0;

    retval = _bLis3dhReadRegs(LIS3DH_CTRL_REG1, (uint8_t*)&ctrl_reg1, 1);
    if(retval < 0)
    {
        return retval;
    }
    
    retval = _bLis3dhReadRegs(LIS3DH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);
    if(retval < 0)
    {
        return retval;
    }
    
    if(val == LIS3DH_HR_12bit)
    {
        ctrl_reg1.lpen = 0;
        ctrl_reg4.hr   = 1;
    }
    
    if(val == LIS3DH_NM_10bit)
    {
        ctrl_reg1.lpen = 0;
        ctrl_reg4.hr   = 0;
    }

    if(val == LIS3DH_LP_8bit)
    {
        ctrl_reg1.lpen = 1;
        ctrl_reg4.hr   = 0;
    }
    
    retval = _bLis3dhWriteRegs(LIS3DH_CTRL_REG1, (uint8_t*)&ctrl_reg1, 1);
    if(retval < 0)
    {
        return retval;
    }
    
    retval = _bLis3dhWriteRegs(LIS3DH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);
    return retval;
}

//FIFO configuration.
static int _bLis3dhFIFO_Set(uint8_t fth, bLis3dhFIFO_Mode_t mode, uint8_t enable)
{
    bLis3dhFIFO_CtrlReg_t fifo_ctrl_reg;
    bLis3dhCTRL_REG5_t ctrl_reg5;
    int retval = 0;
    retval = _bLis3dhReadRegs(LIS3DH_FIFO_CTRL_REG, (uint8_t*)&fifo_ctrl_reg, 1);
    if(retval < 0)
    {
        return retval;
    } 
    fifo_ctrl_reg.fth = fth;
    fifo_ctrl_reg.fm = mode;
    retval = _bLis3dhWriteRegs(LIS3DH_FIFO_CTRL_REG, (uint8_t*)&fifo_ctrl_reg, 1);
    if(retval < 0)
    {
        return retval;
    } 

    retval = _bLis3dhReadRegs(LIS3DH_CTRL_REG5, (uint8_t*)&ctrl_reg5, 1);
    if(retval < 0)
    {
        return retval;
    } 
    ctrl_reg5.fifo_en = enable;
    retval = _bLis3dhWriteRegs(LIS3DH_CTRL_REG5, (uint8_t*)&ctrl_reg5, 1);
    return retval;  
}

//Int configuration.
static int _bLis3dhIntSet(bLis3dhIntCfg_t val)
{
    int retval = 0;
    uint8_t ctrl_reg3;

    retval = _bLis3dhReadRegs(LIS3DH_CTRL_REG3, &ctrl_reg3, 1);
    if(retval == 0)
    {
        ctrl_reg3 = val;
        retval = _bLis3dhWriteRegs(LIS3DH_CTRL_REG3, &ctrl_reg3, 1);
    }
    return retval;    
}

//--------------------------------------------------------------------------------
//len has to be a multiple of sizeof(bLis3dh3Axis_t)
static int _bLis3dhRead(bLIS3DH_Driver_t *pdrv, uint32_t off, uint8_t *pbuf, uint16_t len)
{
    uint8_t c = len / sizeof(bGsensor3Axis_t);
    uint8_t c_off = off / sizeof(bGsensor3Axis_t);
    if(c == 0 || c_off > bLis3dhConfig.fth)
    {
        return 0;
    }
    if((c_off + c) > (bLis3dhConfig.fth + 1))
    {
        c = (bLis3dhConfig.fth + 1) - c_off;
    }
    memcpy(pbuf, (uint8_t *)&bLis3dhFifoValue[c_off], c * sizeof(bGsensor3Axis_t));
    return (c * sizeof(bGsensor3Axis_t));
}

static int _bLis3dhCtl(bLIS3DH_Driver_t *pdrv, uint8_t cmd, void *param)
{
    int retval = -1;
    switch(cmd)
    {
        case bCMD_CFG_ODR:
            {
                uint16_t hz = ((uint16_t *)param)[0];
                bLis3dhODR_t odr = (bLis3dhODR_t)HZ2ODR(hz);
                _bLis3dhODR_Set(odr);
                bLis3dhConfig.odr = odr;
                retval = 0;
            }
            break;
        case bCMD_CFG_FS:
            {
                uint16_t g = ((uint16_t *)param)[0];
                bLis3dhFS_t fs = (bLis3dhFS_t)G2FS(g);
                _bLis3dhFullScaleSet(fs);
                bLis3dhConfig.fs = fs;
                retval = 0;
            }
            break;    
    }
    return retval;
}


static void _bLis3dhEXTI()
{
    IntFlag = 1;
}

static void _bLis3dhPolling()
{
    bLis3dhFIFO_SrcReg_t fifo_src_reg;
    int i = 0;
    uint8_t fss = 0;
    if(IntFlag)
    {
        IntFlag = 0;
        _bLis3dhReadRegs(LIS3DH_FIFO_SRC_REG, (uint8_t*)&fifo_src_reg, 1);
        fss = (uint8_t)fifo_src_reg.fss;
        for(i = 0;i <= fss;i++)
        {
            _bLis3dhReadRegs(LIS3DH_OUT_X_L, (uint8_t *)&bLis3dhFifoValue[i], 6);
            bLis3dhFifoValue[i].x.i16bit =  (bLis3dhFifoValue[i].x.i16bit >> DataShiftTable[bLis3dhConfig.op_mode]) * Digit2mgTable[bLis3dhConfig.fs][bLis3dhConfig.op_mode];
            bLis3dhFifoValue[i].y.i16bit =  (bLis3dhFifoValue[i].y.i16bit >> DataShiftTable[bLis3dhConfig.op_mode]) * Digit2mgTable[bLis3dhConfig.fs][bLis3dhConfig.op_mode];
            bLis3dhFifoValue[i].z.i16bit =  (bLis3dhFifoValue[i].z.i16bit >> DataShiftTable[bLis3dhConfig.op_mode]) * Digit2mgTable[bLis3dhConfig.fs][bLis3dhConfig.op_mode];
        }
        bGsensor3AxisCallback(bLis3dhFifoValue, fss + 1);
    }
}
/**
 * \}
 */
   
/** 
 * \addtogroup LIS3DH_Exported_Functions
 * \{
 */

#if __GNUC__ 
void __attribute__((weak)) bGsensor3AxisCallback(bLis3dh3Axis_t *xyz, uint8_t number)
#else
__weak void bGsensor3AxisCallback(bGsensor3Axis_t *xyz, uint8_t number)
#endif
{
    int i = 0;
    for(i = 0;i < number;i++)
    {
        b_log("x:%d y:%d z:%d\r\n", xyz[i].x.i16bit, xyz[i].y.i16bit, xyz[i].z.i16bit);
    }
}



int bLIS3DH_Init()
{      
    uint8_t id = 0;
    if((id = _bLis3dhGetID()) != LIS3DH_ID)
    {
        b_log_e("id:%x\r\n", id);
        bLIS3DH_Driver.status = -1;
        return -1;
    }
    _bLis3dhBlockDataUpdateSet(0);
    _bLis3dhODR_Set(bLis3dhConfig.odr);
    _bLis3dhFullScaleSet(bLis3dhConfig.fs);
    _bLis3dhOpModeSet(bLis3dhConfig.op_mode);
    _bLis3dhFIFO_Set(bLis3dhConfig.fth, bLis3dhConfig.fifo_mode, bLis3dhConfig.fifo_enable);
    _bLis3dhIntSet(LIS3DH_INT_WTM);
    bLIS3DH_Driver.status = 0;
    bLIS3DH_Driver.read = _bLis3dhRead;
    bLIS3DH_Driver.write = NULL;
    bLIS3DH_Driver.open = NULL;
    bLIS3DH_Driver.close = NULL;
    bLIS3DH_Driver.ctl = _bLis3dhCtl;
    return 0;
}




bHAL_REG_GPIO_EXTI(HAL_LIS3DH_INT_PIN, _bLis3dhEXTI);
BOS_REG_POLLING_FUNC(_bLis3dhPolling);
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

