/**
 *!
 * \file        b_drv_sd.c
 * \version     v0.0.1
 * \date        2020/06/01
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
#include "drivers/inc/b_drv_sd.h"

#include "utils/inc/b_util_log.h"

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup SD
 * \{
 */

/**
 * \defgroup SD_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SD_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SD_Private_Defines
 * \{
 */
#define DRIVER_NAME SD
/**
 * \}
 */

/**
 * \defgroup SD_Private_Variables
 * \{
 */
bDRIVER_HALIF_TABLE(bSD_HalIf_t, DRIVER_NAME);
/**
 * \}
 */

/**
 * \defgroup SD_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SD_Private_Functions
 * \{
 */

/*****************************driver interface***************************/
static int _bSD_WaitReady(bDriverInterface_t *pdrv)
{
    uint32_t tick = bHalGetSysTick();
    uint8_t  tmp  = 0;
    bDRIVER_GET_HALIF(_if, bSD_HalIf_t, pdrv);
    for (;;)
    {
        tmp = bHalSpiTransfer(&_if->_if._spi, 0xff);
        if (tmp == 0xff)
        {
            return 0;
        }
        if (bHalGetSysTick() - tick > 500)
        {
            break;
        }
    }
    return -1;
}

static void _bSD_SendDump(bDriverInterface_t *pdrv, uint8_t n)
{
    uint8_t tmp = 0xff;
    uint8_t i   = 0;
    bDRIVER_GET_HALIF(_if, bSD_HalIf_t, pdrv);
    for (i = 0; i < n; i++)
    {
        bHalSpiSend(&_if->_if._spi, &tmp, 1);
    }
}

static int _bSD_PowerON(bDriverInterface_t *pdrv)
{
    uint8_t  tmp    = 0xff;
    uint8_t  cmd[6] = {CMD0, 0, 0, 0, 0, 0X95};
    uint32_t cnt;
    bDRIVER_GET_HALIF(_if, bSD_HalIf_t, pdrv);
    bHalDelayMs(100);
    bHalSpiSetSpeed(&_if->_if._spi, B_HAL_SPI_SLOW);
    bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
    _bSD_SendDump(pdrv, 10);

    bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 0);

    bHalSpiSend(&_if->_if._spi, cmd, 6);
    cnt = 0;
    for (;;)
    {
        tmp = bHalSpiTransfer(&_if->_if._spi, 0xff);
        if (tmp == 0x1 || cnt >= 0x1fff)
        {
            break;
        }
        cnt++;
    }
    bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
    _bSD_SendDump(pdrv, 1);
    if (cnt >= 0x1fff)
    {
        return -1;
    }
    return 0;
}

static int _bSD_SendCmd(bDriverInterface_t *pdrv, uint8_t cmd, uint32_t param, uint8_t crc)
{
    uint8_t cmd_table[6];
    uint8_t tmp, cnt;
    bDRIVER_GET_HALIF(_if, bSD_HalIf_t, pdrv);
    if (_bSD_WaitReady(pdrv) < 0)
    {
        return -1;
    }
    cmd_table[0] = cmd;
    cmd_table[1] = (uint8_t)((param >> 24) & 0xff);
    cmd_table[2] = (uint8_t)((param >> 16) & 0xff);
    cmd_table[3] = (uint8_t)((param >> 8) & 0xff);
    cmd_table[4] = (uint8_t)((param >> 0) & 0xff);
    cmd_table[5] = crc;
    bHalSpiSend(&_if->_if._spi, cmd_table, 6);
    cnt = 0;
    for (;;)
    {
        tmp = bHalSpiTransfer(&_if->_if._spi, 0xff);
        if (tmp != 0xff || cnt > 200)
        {
            break;
        }
        cnt++;
    }
    if (cnt > 200)
    {
        b_log_e("cmd%d err..\r\n", cmd);
        return -1;
    }
    return tmp;
}

static int _bSD_Init(bDriverInterface_t *pdrv)
{
    int      retval = -1;
    uint32_t cnt    = 0;
    uint8_t  ocr[4];
    bDRIVER_GET_HALIF(_if, bSD_HalIf_t, pdrv);
    if (_if->is_spi == 0)
    {
        return 0;
    }
    if (_bSD_PowerON(pdrv) < 0)
    {
        b_log_e("power on err\r\n");
        return -1;
    }
    bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 0);
    retval = _bSD_SendCmd(pdrv, CMD0, 0, 0x95);
    if (retval < 0)
    {
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
        return -1;
    }

    if (retval == 1)
    {
        retval = _bSD_SendCmd(pdrv, CMD8, 0x1AA, 0x87);
        if (retval < 0)
        {
            bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
            return -1;
        }
    }
    else
    {
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
        return -1;
    }

    if (retval == 0x5)
    {
        pdrv->_private.v = CT_SD1;
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
        _bSD_SendDump(pdrv, 1);
        cnt = 0;
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 0);
        do
        {
            retval = _bSD_SendCmd(pdrv, CMD55, 0, 0);
            if (retval < 0)
            {
                bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
                return -1;
            }
            retval = _bSD_SendCmd(pdrv, CMD41, 0, 0);
            if (retval < 0)
            {
                bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
                return -1;
            }
            cnt++;
        } while ((retval != 0) && (cnt < 400));
        if (cnt >= 400)
        {
            bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
            return -1;
        }
        bHalSpiSetSpeed(&_if->_if._spi, B_HAL_SPI_FAST);
        _bSD_SendDump(pdrv, 1);
        retval = _bSD_SendCmd(pdrv, CMD59, 0, 0x95);
        if (retval < 0)
        {
            bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
            return -1;
        }
        retval = _bSD_SendCmd(pdrv, CMD16, 512, 0x95);
        if (retval < 0)
        {
            bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
            return -1;
        }
    }
    else if (retval == 0x1)
    {
        ocr[0] = bHalSpiTransfer(&_if->_if._spi, 0xff);
        ocr[1] = bHalSpiTransfer(&_if->_if._spi, 0xff);
        ocr[2] = bHalSpiTransfer(&_if->_if._spi, 0xff);
        ocr[3] = bHalSpiTransfer(&_if->_if._spi, 0xff);
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
        _bSD_SendDump(pdrv, 1);
        cnt = 0;
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 0);
        do
        {
            retval = _bSD_SendCmd(pdrv, CMD55, 0, 0);
            if (retval < 0)
            {
                bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
                return -1;
            }
            retval = _bSD_SendCmd(pdrv, CMD41, 0x40000000, 0);
            if (retval < 0)
            {
                bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
                return -1;
            }
            cnt++;
        } while ((retval != 0) && (cnt < 400));
        if (cnt >= 400)
        {
            bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
            return -1;
        }
        retval = _bSD_SendCmd(pdrv, CMD58, 0, 0);
        if (retval != 0)
        {
            bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
            return -1;
        }
        ocr[0] = bHalSpiTransfer(&_if->_if._spi, 0xff);
        ocr[1] = bHalSpiTransfer(&_if->_if._spi, 0xff);
        ocr[2] = bHalSpiTransfer(&_if->_if._spi, 0xff);
        ocr[3] = bHalSpiTransfer(&_if->_if._spi, 0xff);
        if (ocr[0] & 0x40)
        {
            pdrv->_private.v = CT_SDHC;
        }
        else
        {
            pdrv->_private.v = CT_SD2;
        }
    }
    bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
    _bSD_SendDump(pdrv, 1);
    bHalSpiSetSpeed(&_if->_if._spi, B_HAL_SPI_FAST);
    b_log("sd type:%d\r\n", pdrv->_private.v);
    return 0;
}

static int _bSD_WaitResponse(bDriverInterface_t *pdrv, uint8_t exp)
{
    uint16_t cnt = 0;
    uint8_t  tmp;
    bDRIVER_GET_HALIF(_if, bSD_HalIf_t, pdrv);
    do
    {
        tmp = bHalSpiTransfer(&_if->_if._spi, 0xff);
        cnt++;
    } while (tmp != exp && cnt <= 0xfff);
    if (tmp == exp)
    {
        return 0;
    }
    return -1;
}

static int _bSD_ReceiveData(bDriverInterface_t *pdrv, uint8_t *buff, uint16_t len)
{
    bDRIVER_GET_HALIF(_if, bSD_HalIf_t, pdrv);
    if (_bSD_WaitResponse(pdrv, 0xfe) < 0)
    {
        return -1;
    }
    while (len--)
    {
        *buff = bHalSpiTransfer(&_if->_if._spi, 0xff);
        buff++;
    }
    bHalSpiTransfer(&_if->_if._spi, 0xff);
    bHalSpiTransfer(&_if->_if._spi, 0xff);
    return len;
}

static int _bSD_ReadSingleBlock(bDriverInterface_t *pdrv, uint32_t sector, uint8_t *pbuf)
{
    int retval = 0;
    bDRIVER_GET_HALIF(_if, bSD_HalIf_t, pdrv);
    if (_if->is_spi)
    {
        if (pdrv->_private.v != CT_SDHC)
        {
            sector = sector << 9;
        }
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 0);
        retval = _bSD_SendCmd(pdrv, CMD17, sector, 0);
        if (retval != 0)
        {
            bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
            return -1;
        }
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
        _bSD_SendDump(pdrv, 1);

        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 0);
        if (_bSD_ReceiveData(pdrv, pbuf, 512) < 0)
        {
            bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
            return -1;
        }
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
        _bSD_SendDump(pdrv, 1);
    }
    return 512;
}

static int _bSD_WriteSingleBlock(bDriverInterface_t *pdrv, uint32_t sector, uint8_t *pbuf)
{
    int      retval = 0;
    uint16_t cnt;
    bDRIVER_GET_HALIF(_if, bSD_HalIf_t, pdrv);
    if (_if->is_spi)
    {
        if (pdrv->_private.v != CT_SDHC)
        {
            sector = sector << 9;
        }
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 0);
        retval = _bSD_SendCmd(pdrv, CMD24, sector, 0);
        if (retval != 0)
        {
            bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
            return -1;
        }
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
        _bSD_SendDump(pdrv, 1);

        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 0);
        _bSD_SendDump(pdrv, 3);
        bHalSpiTransfer(&_if->_if._spi, 0xfe);
        bHalSpiSend(&_if->_if._spi, pbuf, 512);
        _bSD_SendDump(pdrv, 2);
        retval = bHalSpiTransfer(&_if->_if._spi, 0xff);
        if ((retval & 0x1f) != 0x05)
        {
            bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
            return -1;
        }
        cnt = 0;
        while (!bHalSpiTransfer(&_if->_if._spi, 0xff))
        {
            cnt++;
            if (cnt >= 0xfffe)
            {
                bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
                return -1;
            }
        }
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
        _bSD_SendDump(pdrv, 1);
    }
    return 0;
}

// sector:  Sector number to write from
// count: Number of sectors to write
static int _bSD_Write(bDriverInterface_t *pdrv, uint32_t sector, uint8_t *pbuf, uint32_t count)
{
    int i      = 0;
    int retval = -1;
    bDRIVER_GET_HALIF(_if, bSD_HalIf_t, pdrv);
    if (_if->is_spi)
    {
        for (i = 0; i < count; i++)
        {
            retval = _bSD_WriteSingleBlock(pdrv, sector + i, pbuf + i * 512);
            if (retval < 0)
            {
                break;
            }
        }
    }
    else
    {
        retval = bHalSDIOWriteBlocks(_if->_if._sdio, pbuf, sector, count);
    }
    if (retval < 0)
    {
        return -1;
    }
    return count;
}

// sector:  Sector number to write from
// count: Number of sectors to write
static int _bSD_Read(bDriverInterface_t *pdrv, uint32_t sector, uint8_t *pbuf, uint32_t count)
{
    int i      = 0;
    int retval = -1;
    bDRIVER_GET_HALIF(_if, bSD_HalIf_t, pdrv);
    if (_if->is_spi)
    {
        for (i = 0; i < count; i++)
        {
            retval = _bSD_ReadSingleBlock(pdrv, sector + i, pbuf + i * 512);
            if (retval < 0)
            {
                break;
            }
        }
    }
    else
    {
        retval = bHalSDIOReadBlocks(_if->_if._sdio, pbuf, sector, count);
    }
    if (retval < 0)
    {
        return -1;
    }
    return count;
}

/**
 * \}
 */

/**
 * \addtogroup SD_Exported_Functions
 * \{
 */
int bSD_Init(bDriverInterface_t *pdrv)
{
    bDRIVER_STRUCT_INIT(pdrv, DRIVER_NAME, bSD_Init);
    pdrv->read  = _bSD_Read;
    pdrv->write = _bSD_Write;
    if (_bSD_Init(pdrv) < 0)
    {
        return -1;
    }
    return 0;
}

bDRIVER_REG_INIT(B_DRIVER_SD, bSD_Init);

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
