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
#define DRIVER_NAME DS18B20
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
bDRIVER_HALIF_TABLE(bDS18B20_HalIf_t, DRIVER_NAME);
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
static uint8_t _bSbusReady(bDriverInterface_t *pdrv)
{
    uint16_t cp     = 0;
    uint8_t  retval = 1;

    bDRIVER_GET_HALIF(_if, bDS18B20_HalIf_t, pdrv);

    bHalGpioWritePin(_if->port, _if->pin, 0);
    bHalDelayUs(600);  // 480~960us
    bHalGpioWritePin(_if->port, _if->pin, 1);

    bHalGpioConfig(_if->port, _if->pin, B_HAL_GPIO_INPUT, B_HAL_GPIO_NOPULL);
    cp = 0;
    while (((bHalGpioReadPin(_if->port, _if->pin)) == 1) && (cp++ < 100))
    {
        bHalDelayUs(1);
    }
    if (cp >= 100)
    {
        retval = 0;
    }
    bHalDelayUs(250);  // 60~240us
    bHalGpioConfig(_if->port, _if->pin, B_HAL_GPIO_OUTPUT, B_HAL_GPIO_NOPULL);
    bHalGpioWritePin(_if->port, _if->pin, 1);
    return retval;
}

static uint8_t _bSbusReadByte(bDriverInterface_t *pdrv)
{
    uint8_t byte = 0;
    uint8_t bit  = 0;
    uint8_t i;

    bDRIVER_GET_HALIF(_if, bDS18B20_HalIf_t, pdrv);

    for (i = 0; i < 8; i++)
    {
        bHalGpioWritePin(_if->port, _if->pin, 0);
        bHalDelayUs(2);
        bHalGpioWritePin(_if->port, _if->pin, 1);

        bHalGpioConfig(_if->port, _if->pin, B_HAL_GPIO_INPUT, B_HAL_GPIO_NOPULL);
        bHalDelayUs(2);  // < 15us
        bit  = bHalGpioReadPin(_if->port, _if->pin);
        byte = (byte >> 1) | (bit << 7);
        bHalDelayUs(100);  // > 60us

        bHalGpioConfig(_if->port, _if->pin, B_HAL_GPIO_OUTPUT, B_HAL_GPIO_NOPULL);
        bHalGpioWritePin(_if->port, _if->pin, 1);
    }
    return byte;
}

static void _bSbusWriteByte(bDriverInterface_t *pdrv, uint8_t dat)
{
    uint8_t j, wbit = 0;
    bDRIVER_GET_HALIF(_if, bDS18B20_HalIf_t, pdrv);
    for (j = 0; j < 8; j++)
    {
        wbit = dat & 0x1;
        dat >>= 1;
        bHalGpioWritePin(_if->port, _if->pin, 0);
        bHalDelayUs(2);
        bHalGpioWritePin(_if->port, _if->pin, wbit);
        bHalDelayUs(100);  // 60~120us
        bHalGpioWritePin(_if->port, _if->pin, 1);
        bHalDelayUs(2);
    }
}

static int _bDS18B20Start(bDriverInterface_t *pdrv)
{
    int retval = -1;
    if (_bSbusReady(pdrv))
    {
        _bSbusWriteByte(pdrv, 0xCC);
        _bSbusWriteByte(pdrv, 0x44);
        retval = 0;
    }
    return retval;
}

static int _bDS18B20Ctl(bDriverInterface_t *pdrv, uint8_t cmd, void *param)
{
    int retval = -1;
    switch (cmd)
    {
        case bCMD_SENSOR_START:
            retval = _bDS18B20Start(pdrv);
            break;
        default:
            break;
    }
    return retval;
}

static int _bDS18B20Read(bDriverInterface_t *pdrv, uint32_t off, uint8_t *pbuf, uint32_t len)
{
    int16_t    temp_dig = 0;
    float      temp     = 0;
    uint8_t    tmh, tml;
    bTempVal_t temp_val;
    if (len < sizeof(bTempVal_t))
    {
        return -1;
    }

    if (_bSbusReady(pdrv))
    {
        _bSbusWriteByte(pdrv, 0xCC);
        _bSbusWriteByte(pdrv, 0xBE);
    }
    else
    {
        return -1;
    }
    tml      = _bSbusReadByte(pdrv);
    tmh      = _bSbusReadByte(pdrv);
    temp_dig = ((int16_t)tmh) << 8;
    temp_dig |= tml;
    if (temp_dig < 0)
    {
        temp = (~temp_dig + 1) * 0.0625;
    }
    else
    {
        temp = temp_dig * 0.0625;
    }

    temp_val.tempx100 = (int16_t)(temp * 100);
    memcpy(pbuf, &temp_val, sizeof(bTempVal_t));
    return sizeof(bTempVal_t);
}

/**
 * \}
 */

/**
 * \addtogroup DS18B20_Exported_Functions
 * \{
 */
int bDS18B20_Init(bDriverInterface_t *pdrv)
{
    bDRIVER_STRUCT_INIT(pdrv, DRIVER_NAME, bDS18B20_Init);
    pdrv->read = _bDS18B20Read;
    pdrv->ctl  = _bDS18B20Ctl;
    return 0;
}

bDRIVER_REG_INIT(B_DRIVER_DS18B20, bDS18B20_Init);
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
