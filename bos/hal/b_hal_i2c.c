/**
 *!
 * \file        b_hal_spi.c
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SUARTL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
/*Includes ----------------------------------------------*/
#include "hal/inc/b_hal.h"
#include "utils/inc/b_util_i2c.h"
/**
 * \addtogroup B_HAL
 * \{
 */

/**
 * \addtogroup I2C
 * \{
 */

/**
 * \addtogroup I2C_Exported_Functions
 * \{
 */

__WEAKDEF uint8_t bMcuI2CReadByte(const bHalI2CIf_t *i2c_if)
{
    return 0;
}

__WEAKDEF int bMcuI2CWriteByte(const bHalI2CIf_t *i2c_if, uint8_t dat)
{
    return -1;
}

__WEAKDEF int bMcuI2CMemWrite(const bHalI2CIf_t *i2c_if, uint16_t mem_addr, const uint8_t *pbuf,
                              uint16_t len)
{
    return -1;
}

__WEAKDEF int bMcuI2CMemRead(const bHalI2CIf_t *i2c_if, uint16_t mem_addr, uint8_t *pbuf,
                             uint16_t len)
{
    return -1;
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

uint8_t bHalI2CReadByte(const bHalI2CIf_t *i2c_if)
{
    uint8_t    tmp = 0;
    bUtilI2C_t simulating_iic;
    if (IS_NULL(i2c_if))
    {
        return 0;
    }
    if (i2c_if->is_simulation == 1)
    {
        simulating_iic.sda = i2c_if->_if.simulating_i2c.sda;
        simulating_iic.clk = i2c_if->_if.simulating_i2c.clk;
        tmp                = bUtilI2C_ReadData(simulating_iic, i2c_if->dev_addr);
    }
    else
    {
        tmp = bMcuI2CReadByte(i2c_if);
    }
    return tmp;
}

int bHalI2CWriteByte(const bHalI2CIf_t *i2c_if, uint8_t dat)
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
        bUtilI2C_WriteData(simulating_iic, i2c_if->dev_addr, dat);
    }
    else
    {
        retval = bMcuI2CWriteByte(i2c_if, dat);
    }
    return retval;
}

int bHalI2CMemWrite(const bHalI2CIf_t *i2c_if, uint16_t mem_addr, const uint8_t *pbuf, uint16_t len)
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
        retval = bMcuI2CMemWrite(i2c_if, mem_addr, pbuf, len);
    }
    return retval;
}

int bHalI2CMemRead(const bHalI2CIf_t *i2c_if, uint16_t mem_addr, uint8_t *pbuf, uint16_t len)
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
        retval = bMcuI2CMemRead(i2c_if, mem_addr, pbuf, len);
    }
    return retval;
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

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
