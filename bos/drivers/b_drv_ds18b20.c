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

#include "utils/inc/b_util_sBus.h"


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

bDS18B20_Driver_t           bDS18B20_Driver;
static bUtilSbus_t          SBUS_IO            = HAL_DS18B20_IF;
static bDS18B20ReadStruct_t bDS18B20ReadStruct = {0};
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
static uint16_t _bDS18B20ReadTemp()
{
    uint16_t temp      = 0;
    uint32_t tempvalue = 0;
    uint8_t  tmh, tml;
    uint16_t cp   = 0;
    uint16_t wait = 2000;
    if (bUtilSbus_Ready(SBUS_IO))
    {
        SbusWriteByte(SBUS_IO, 0xcc);
        //跳过ROM操作命令,只有当总线上只存在一个 DS18B20 的时候才可以使用此指令
        SbusWriteByte(SBUS_IO, 0x44);  //温度转换命令
    }
    else
    {
        return 0;
    }

    bHalGPIO_Config(SBUS_IO.sBusIo.port, SBUS_IO.sBusIo.pin, B_HAL_GPIO_INPUT, B_HAL_GPIO_NOPULL);
    while (((bHalGPIO_ReadPin(SBUS_IO.sBusIo.port, SBUS_IO.sBusIo.pin)) == 0) && cp++ < wait)
        ;  //等待转换成功
    if (cp >= wait)
    {
        return 0;
    }

    if (bUtilSbus_Ready(SBUS_IO))
    {
        SbusWriteByte(SBUS_IO, 0xcc);  //跳过ROM操作命令
        SbusWriteByte(SBUS_IO, 0xbe);  //发送读取温度命令
    }
    else
    {
        return 0;
    }

    tml  = SbusReadByte(SBUS_IO);  //读取温度值共16位，先读低字节
    tmh  = SbusReadByte(SBUS_IO);  //再读高字节
    temp = tmh;
    temp <<= 8;
    temp |= tml;
    tempvalue = temp * 625;
    if (tempvalue > 1000000)
    {
        tempvalue = 1000000;
    }
    temp = (uint16_t)(tempvalue / 1000);
    return temp;
}

static int _bDS18B20Read(bDS18B20_Driver_t *pdrv, uint32_t off, uint8_t *pbuf, uint16_t len)
{
    if (len < sizeof(bDS18B20ReadStruct_t))
    {
        return 0;
    }
    memcpy(pbuf, &bDS18B20ReadStruct.TempX10, sizeof(bDS18B20ReadStruct.TempX10));
}

static void _bDS18B20Polling()
{
    static uint8_t  flag     = 0;
    static uint16_t data[10] = {0};
    uint8_t         i        = 0;
    uint16_t        temp     = 0;
    uint16_t        temp_sum = 0;
    temp                     = _bDS18B20ReadTemp();
    if (temp == 0)
        return;
    if (flag)
    {
        for (i = 0; i < 9; i++)
        {
            data[i] = data[i + 1];
        }
        data[9] = temp;
    }
    else
    {
        flag = 1;
        for (i = 0; i < 10; i++)
        {
            data[i] = temp;
        }
    }
    for (i = 0; i < 10; i++)
    {
        temp_sum += data[i];
    }
    bDS18B20ReadStruct.TempX10 = temp_sum / 10;
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
    if (!bUtilSbus_Ready(SBUS_IO))
    {
        b_log_e("18b20 init error\n");
        bDS18B20_Driver.status = -1;
        return -1;
    }
    bDS18B20_Driver.status = 0;
    bDS18B20_Driver.read   = _bDS18B20Read;
    bDS18B20_Driver.write  = NULL;
    bDS18B20_Driver.open   = NULL;
    bDS18B20_Driver.close  = NULL;
    bDS18B20_Driver.ctl    = NULL;
    return 0;
}

BOS_REG_POLLING_FUNC(_bDS18B20Polling);
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
