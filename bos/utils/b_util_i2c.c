/**
 *!
 * \file        b_util_i2c.c
 * \version     v0.0.1
 * \date        2020/04/01
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
#include "utils/inc/b_util_i2c.h"

/**
 * \addtogroup B_UTILS
 * \{
 */

/**
 * \addtogroup I2C
 * \{
 */

/**
 * \defgroup I2C_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup I2C_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup I2C_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup I2C_Private_Variables
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup I2C_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup I2C_Private_Functions
 * \{
 */

/**
 * \}
 */

/**
 * \addtogroup I2C_Exported_Functions
 * \{
 */

void bUtilI2C_Start(bUtilI2C_t i2c)
{
    bHalGpioWritePin(i2c.sda.port, i2c.sda.pin, 1);
    bHalGpioWritePin(i2c.clk.port, i2c.clk.pin, 1);
    bHalDelayUs(1);
    bHalGpioWritePin(i2c.sda.port, i2c.sda.pin, 0);
    bHalDelayUs(1);
    bHalGpioWritePin(i2c.clk.port, i2c.clk.pin, 0);
}

void bUtilI2C_Stop(bUtilI2C_t i2c)
{
    bHalGpioWritePin(i2c.sda.port, i2c.sda.pin, 0);
    bHalGpioWritePin(i2c.clk.port, i2c.clk.pin, 1);
    bHalDelayUs(1);
    bHalGpioWritePin(i2c.sda.port, i2c.sda.pin, 1);
    bHalDelayUs(1);
    bHalGpioWritePin(i2c.clk.port, i2c.clk.pin, 0);
    bHalGpioWritePin(i2c.sda.port, i2c.sda.pin, 0);
}

int bUtilI2C_ACK(bUtilI2C_t i2c)
{
    uint8_t tmp = 0xff;
    bHalGpioConfig(i2c.sda.port, i2c.sda.pin, B_HAL_GPIO_INPUT, B_HAL_GPIO_NOPULL);
    bHalGpioWritePin(i2c.clk.port, i2c.clk.pin, 0);
    bHalDelayUs(1);
    bHalGpioWritePin(i2c.clk.port, i2c.clk.pin, 1);
    do
    {
        tmp--;
        bHalDelayUs(10);
    } while ((bHalGpioReadPin(i2c.sda.port, i2c.sda.pin) != 0x0) && (tmp > 0));
    if (tmp == 0)
    {
        bUtilI2C_Stop(i2c);
        bHalGpioConfig(i2c.sda.port, i2c.sda.pin, B_HAL_GPIO_OUTPUT, B_HAL_GPIO_NOPULL);
        return -1;
    }
    bHalGpioWritePin(i2c.clk.port, i2c.clk.pin, 0);
    bHalGpioConfig(i2c.sda.port, i2c.sda.pin, B_HAL_GPIO_OUTPUT, B_HAL_GPIO_NOPULL);
    return 0;
}

void bUtilI2C_mACK(bUtilI2C_t i2c)
{
    bHalGpioWritePin(i2c.sda.port, i2c.sda.pin, 0);
    bHalGpioWritePin(i2c.clk.port, i2c.clk.pin, 1);
    bHalDelayUs(1);
    bHalGpioWritePin(i2c.clk.port, i2c.clk.pin, 0);
    bHalDelayUs(1);
}

void bUtilI2C_WriteByte(bUtilI2C_t i2c, uint8_t dat)
{
    uint8_t i = 0;
    bHalGpioWritePin(i2c.clk.port, i2c.clk.pin, 0);
    for (i = 0; i < 8; i++)
    {
        bHalDelayUs(1);
        if (dat & 0x80)
        {
            bHalGpioWritePin(i2c.sda.port, i2c.sda.pin, 1);
        }
        else
        {
            bHalGpioWritePin(i2c.sda.port, i2c.sda.pin, 0);
        }
        bHalGpioWritePin(i2c.clk.port, i2c.clk.pin, 1);
        bHalDelayUs(1);
        bHalGpioWritePin(i2c.clk.port, i2c.clk.pin, 0);
        dat <<= 1;
    }
}

uint8_t bUtilI2C_ReadByte(bUtilI2C_t i2c)
{
    uint8_t i = 0, tmp = 0;
    bHalGpioConfig(i2c.sda.port, i2c.sda.pin, B_HAL_GPIO_INPUT, B_HAL_GPIO_NOPULL);
    bHalGpioWritePin(i2c.clk.port, i2c.clk.pin, 0);
    bHalDelayUs(1);
    for (i = 0; i < 8; i++)
    {
        bHalGpioWritePin(i2c.clk.port, i2c.clk.pin, 1);
        tmp <<= 1;
        if (bHalGpioReadPin(i2c.sda.port, i2c.sda.pin))
        {
            tmp++;
        }
        bHalDelayUs(1);
        bHalGpioWritePin(i2c.clk.port, i2c.clk.pin, 0);
        bHalDelayUs(1);
    }
    bHalGpioConfig(i2c.sda.port, i2c.sda.pin, B_HAL_GPIO_OUTPUT, B_HAL_GPIO_NOPULL);
    return tmp;
}

int bUtilI2C_WriteData(bUtilI2C_t i2c, uint8_t dev, uint8_t dat)
{
    bUtilI2C_Start(i2c);
    bUtilI2C_WriteByte(i2c, dev);
    if (bUtilI2C_ACK(i2c) < 0)
    {
        bUtilI2C_Stop(i2c);
        return -1;
    }
    bUtilI2C_WriteByte(i2c, dat);
    if (bUtilI2C_ACK(i2c) < 0)
    {
        bUtilI2C_Stop(i2c);
        return -1;
    }
    bUtilI2C_Stop(i2c);
    return 0;
}

uint8_t bUtilI2C_ReadData(bUtilI2C_t i2c, uint8_t dev)
{
    uint8_t tmp;
    bUtilI2C_Start(i2c);
    bUtilI2C_WriteByte(i2c, dev | 0x1);
    if (bUtilI2C_ACK(i2c) < 0)
    {
        bUtilI2C_Stop(i2c);
        return 0;
    }
    tmp = bUtilI2C_ReadByte(i2c);
    bUtilI2C_Stop(i2c);
    return tmp;
}

int bUtilI2C_ReadBuff(bUtilI2C_t i2c, uint8_t dev, uint8_t addr, uint8_t *pdat, uint8_t len)
{
    bUtilI2C_Start(i2c);
    bUtilI2C_WriteByte(i2c, dev);
    if (bUtilI2C_ACK(i2c) < 0)
    {
        bUtilI2C_Stop(i2c);
        return -1;
    }
    bUtilI2C_WriteByte(i2c, addr);
    if (bUtilI2C_ACK(i2c) < 0)
    {
        bUtilI2C_Stop(i2c);
        return -1;
    }
    bUtilI2C_Start(i2c);
    bUtilI2C_WriteByte(i2c, dev | 0x1);
    if (bUtilI2C_ACK(i2c) < 0)
    {
        bUtilI2C_Stop(i2c);
        return -1;
    }
    while (len-- > 1)
    {
        *pdat++ = bUtilI2C_ReadByte(i2c);
        bUtilI2C_mACK(i2c);
    }
    *pdat++ = bUtilI2C_ReadByte(i2c);

    bUtilI2C_Stop(i2c);
    return 0;
}

int bUtilI2C_WriteBuff(bUtilI2C_t i2c, uint8_t dev, uint8_t addr, const uint8_t *pdat, uint8_t len)
{
    uint32_t i = 0;
    bUtilI2C_Start(i2c);
    bUtilI2C_WriteByte(i2c, dev);
    if (bUtilI2C_ACK(i2c) < 0)
    {
        bUtilI2C_Stop(i2c);
        return -1;
    }
    bUtilI2C_WriteByte(i2c, addr);
    if (bUtilI2C_ACK(i2c) < 0)
    {
        bUtilI2C_Stop(i2c);
        return -1;
    }
    for (i = 0; i < len; i++)
    {
        bUtilI2C_WriteByte(i2c, pdat[i]);
        if (bUtilI2C_ACK(i2c) < 0)
        {
            bUtilI2C_Stop(i2c);
            return -1;
        }
    }
    bUtilI2C_Stop(i2c);
    return 0;
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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
