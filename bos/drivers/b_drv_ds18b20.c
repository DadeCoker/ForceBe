/**
 *!
 * \file        b_drv_ds18b20.c
 * \version     v0.0.1
 * \date        2021/04/29
 * \author      polyGithub(baoli.chen@outlook.com)
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 polyGithub
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
#include "drivers/inc/b_drv_ds18b20.h"

#include <string.h>

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup DS18B20
 * \{
 */

/**
 * \defgroup DS18B20_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup DS18B20_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup DS18B20_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup DS18B20_Private_Variables
 * \{
 */
const static bDS18B20_HalIf_t bDS18B20_HalIf = HAL_DS18B20_IF;
bDS18B20_Driver_t             bDS18B20_Driver;
/**
 * \}
 */

/**
 * \defgroup DS18B20_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup DS18B20_Private_Functions
 * \{
 */
static uint8_t _bSbusReady()
{
    uint16_t cp     = 0;
    uint8_t  retval = 1;
    bHalGPIODriver.pGpioWritePin(bDS18B20_HalIf.sBusIo.port, bDS18B20_HalIf.sBusIo.pin, 0);
    bHalDelayUs(600);  // 480~960us
    bHalGPIODriver.pGpioWritePin(bDS18B20_HalIf.sBusIo.port, bDS18B20_HalIf.sBusIo.pin, 1);

    bHalGPIODriver.pGpioConfig(bDS18B20_HalIf.sBusIo.port, bDS18B20_HalIf.sBusIo.pin, B_HAL_GPIO_INPUT,
                    B_HAL_GPIO_NOPULL);
    cp = 0;
    while (((bHalGPIODriver.pGpioReadPin(bDS18B20_HalIf.sBusIo.port, bDS18B20_HalIf.sBusIo.pin)) == 1) &&
           (cp++ < 100))
    {
        bHalDelayUs(1);
    }
    if (cp >= 100)
    {
        retval = 0;
    }
    bHalDelayUs(250);  // 60~240us
    bHalGPIODriver.pGpioConfig(bDS18B20_HalIf.sBusIo.port, bDS18B20_HalIf.sBusIo.pin, B_HAL_GPIO_OUTPUT,
                    B_HAL_GPIO_NOPULL);
    bHalGPIODriver.pGpioWritePin(bDS18B20_HalIf.sBusIo.port, bDS18B20_HalIf.sBusIo.pin, 1);
    return retval;
}

static uint8_t _bSbusReadByte()
{
    uint8_t byte = 0;
    uint8_t bit  = 0;
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        bHalGPIODriver.pGpioWritePin(bDS18B20_HalIf.sBusIo.port, bDS18B20_HalIf.sBusIo.pin, 0);
        bHalDelayUs(2);
        bHalGPIODriver.pGpioWritePin(bDS18B20_HalIf.sBusIo.port, bDS18B20_HalIf.sBusIo.pin, 1);

        bHalGPIODriver.pGpioConfig(bDS18B20_HalIf.sBusIo.port, bDS18B20_HalIf.sBusIo.pin, B_HAL_GPIO_INPUT,
                        B_HAL_GPIO_NOPULL);
        bHalDelayUs(2);  // < 15us
        bit  = bHalGPIODriver.pGpioReadPin(bDS18B20_HalIf.sBusIo.port, bDS18B20_HalIf.sBusIo.pin);
        byte = (byte >> 1) | (bit << 7);
        bHalDelayUs(100);  // > 60us

        bHalGPIODriver.pGpioConfig(bDS18B20_HalIf.sBusIo.port, bDS18B20_HalIf.sBusIo.pin, B_HAL_GPIO_OUTPUT,
                        B_HAL_GPIO_NOPULL);
        bHalGPIODriver.pGpioWritePin(bDS18B20_HalIf.sBusIo.port, bDS18B20_HalIf.sBusIo.pin, 1);
    }
    return byte;
}

static void _bSbusWriteByte(uint8_t dat)
{
    uint8_t j, wbit = 0;
    for (j = 0; j < 8; j++)
    {
        wbit = dat & 0x1;
        dat >>= 1;
        bHalGPIODriver.pGpioWritePin(bDS18B20_HalIf.sBusIo.port, bDS18B20_HalIf.sBusIo.pin, 0);
        bHalDelayUs(2);
        bHalGPIODriver.pGpioWritePin(bDS18B20_HalIf.sBusIo.port, bDS18B20_HalIf.sBusIo.pin, wbit);
        bHalDelayUs(100);  // 60~120us
        bHalGPIODriver.pGpioWritePin(bDS18B20_HalIf.sBusIo.port, bDS18B20_HalIf.sBusIo.pin, 1);
        bHalDelayUs(2);
    }
}

static float _bDS18B20ReadTemp()
{
    int16_t temp       = 0;
    float   temp_value = 0;
    uint8_t tmh, tml;

    if (_bSbusReady())
    {
        _bSbusWriteByte(0xCC);
        _bSbusWriteByte(0x44);
    }
    else
    {
        return 0;
    }

    bHalDelayMs(500);

    if (_bSbusReady())
    {
        _bSbusWriteByte(0xCC);
        _bSbusWriteByte(0xBE);
    }
    else
    {
        return 0;
    }

    tml  = _bSbusReadByte();
    tmh  = _bSbusReadByte();
    temp = ((int16_t)tmh) << 8;
    temp |= tml;

    if (temp < 0)
    {
        temp_value = (~temp + 1) * 0.0625;
    }
    else
    {
        temp_value = temp * 0.0625;
    }
    return temp_value;
}

static int _bDS18B20Read(bDS18B20_Driver_t *pdrv, uint32_t off, uint8_t *pbuf, uint16_t len)
{
    bTempVal_t temp;
    temp.tempx100 = (int16_t)(_bDS18B20ReadTemp() * 100);
    if (len < sizeof(bTempVal_t))
    {
        return 0;
    }
    memcpy(pbuf, &temp, sizeof(bTempVal_t));
    return sizeof(bTempVal_t);
}

/**
 * \}
 */

/**
 * \addtogroup DS18B20_Exported_Functions
 * \{
 */
int bDS18B20_Init()
{
    bDS18B20_Driver.status = 0;
    bDS18B20_Driver.read   = _bDS18B20Read;
    bDS18B20_Driver.write  = NULL;
    bDS18B20_Driver.open   = NULL;
    bDS18B20_Driver.close  = NULL;
    bDS18B20_Driver.ctl    = NULL;
    return 0;
}

bDRIVER_REG_INIT(bDS18B20_Init);

/**
 * \}
 */

/**
 * \}
 */

/**
 * \}
 */

/************************ Copyright (c) 2021 polyGithub *****END OF FILE****/
