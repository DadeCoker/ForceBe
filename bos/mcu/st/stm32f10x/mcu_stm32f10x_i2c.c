/**
 *!
 * \file        mcu_stm32f10x_i2c.c
 * \version     v0.0.1
 * \date        2020/03/25
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SI2CL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */

/*Includes ----------------------------------------------*/
#include "b_config.h"
#include "hal/inc/b_hal_i2c.h"
#include "utils/inc/b_util_i2c.h"

#if (_MCU_PLATFORM == 1001 || _MCU_PLATFORM == 1002 || _MCU_PLATFORM == 1003 || \
     _MCU_PLATFORM == 1004)

static void _I2CSendByte(bHalI2CIf_t *pi2c_if, uint8_t dat)
{
}

static uint8_t _I2CReadByte(bHalI2CIf_t *pi2c_if)
{
    uint8_t tmp;
    return tmp;
}

static int _I2CMemWrite(bHalI2CIf_t *i2c_if, uint16_t mem_addr, const uint8_t *pbuf, uint16_t len)
{
    int        retval = 0;
    bUtilI2C_t simulating_iic;
    if (IS_NULL(i2c_if))
    {
        return -1;
    }
    if (i2c_if->is_simulation == 1)
    {
        simulating_iic.sda = i2c_if->_if.simulating_i2c.sda;
        simulating_iic.clk = i2c_if->_if.simulating_i2c.clk;
        bUtilI2C_WriteBuff(simulating_iic, i2c_if->dev_addr, mem_addr, pbuf, len);
    }
    else
    {
        switch (i2c_if->_if.i2c)
        {
            case B_HAL_I2C_1:

                break;
            case B_HAL_I2C_2:

                break;

            default:
                break;
        }
    }
    return retval;
}

static int _I2CMemRead(bHalI2CIf_t *i2c_if, uint16_t mem_addr, uint8_t *pbuf, uint16_t len)
{
    int        retval = 0;
    bUtilI2C_t simulating_iic;
    if (IS_NULL(i2c_if))
    {
        return -1;
    }
    if (i2c_if->is_simulation == 1)
    {
        simulating_iic.sda = i2c_if->_if.simulating_i2c.sda;
        simulating_iic.clk = i2c_if->_if.simulating_i2c.clk;
        bUtilI2C_ReadBuff(simulating_iic, i2c_if->dev_addr, mem_addr, pbuf, len);
    }
    else
    {
        switch (i2c_if->_if.i2c)
        {
            case B_HAL_I2C_1:

                break;
            case B_HAL_I2C_2:

                break;

            default:
                break;
        }
    }
    return retval;
}

bHalI2CDriver_t bHalI2CDriver = {
    .pWriteByte = _I2CSendByte,
    .pReadByte  = _I2CReadByte,
    .pMemWrite  = _I2CMemWrite,
    .pMemRead   = _I2CMemRead,
};

#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
