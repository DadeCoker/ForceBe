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
 * \defgroup SD_Private_Variables
 * \{
 */
HALIF_KEYWORD bSD_HalIf_t bSD_HalIf = HAL_SD_IF;
bSD_Driver_t              bSD_Driver;
/**
 * \}
 */

/**
 * \defgroup SD_Private_Defines
 * \{
 */
#define SD_CS_SET() bHalGpioWritePin(bSD_HalIf.cs.port, bSD_HalIf.cs.pin, 1)
#define SD_CS_RESET() bHalGpioWritePin(bSD_HalIf.cs.port, bSD_HalIf.cs.pin, 0)
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

/************************************************************************************************************driver
 * interface*******/
static int _bSD_WaitReady()
{
    uint32_t tick = bHalGetSysTick();
    uint8_t  tmp  = 0;

    for (;;)
    {
        tmp = bHalSpiTransfer(&bSD_HalIf, 0xff);
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

static void _bSD_SendDump(uint8_t n)
{
    uint8_t tmp = 0xff;
    uint8_t i   = 0;
    for (i = 0; i < n; i++)
    {
        bHalSpiSend(&bSD_HalIf, &tmp, 1);
    }
}

static int _bSD_PowerON()
{
    uint8_t  tmp    = 0xff;
    uint8_t  cmd[6] = {CMD0, 0, 0, 0, 0, 0X95};
    uint32_t cnt;
    bHalDelayMs(100);
    bHalSpiSetSpeed(&bSD_HalIf, B_HAL_SPI_SLOW);
    SD_CS_SET();
    _bSD_SendDump(10);

    SD_CS_RESET();

    bHalSpiSend(&bSD_HalIf, cmd, 6);
    cnt = 0;
    for (;;)
    {
        tmp = bHalSpiTransfer(&bSD_HalIf, 0xff);
        if (tmp == 0x1 || cnt >= 0x1fff)
        {
            break;
        }
        cnt++;
    }
    SD_CS_SET();
    _bSD_SendDump(1);
    if (cnt >= 0x1fff)
    {
        return -1;
    }
    return 0;
}

static int _bSD_SendCmd(uint8_t cmd, uint32_t param, uint8_t crc)
{
    uint8_t cmd_table[6];
    uint8_t tmp, cnt;
    if (_bSD_WaitReady() < 0)
    {
        return -1;
    }
    cmd_table[0] = cmd;
    cmd_table[1] = (uint8_t)((param >> 24) & 0xff);
    cmd_table[2] = (uint8_t)((param >> 16) & 0xff);
    cmd_table[3] = (uint8_t)((param >> 8) & 0xff);
    cmd_table[4] = (uint8_t)((param >> 0) & 0xff);
    cmd_table[5] = crc;
    bHalSpiSend(&bSD_HalIf, cmd_table, 6);
    cnt = 0;
    for (;;)
    {
        tmp = bHalSpiTransfer(&bSD_HalIf, 0xff);
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

static int _bSD_Init()
{
    int      retval = -1;
    uint32_t cnt    = 0;
    uint8_t  ocr[4];
    if (_bSD_PowerON() < 0)
    {
        b_log_e("power on err\r\n");
        return -1;
    }
    SD_CS_RESET();
    retval = _bSD_SendCmd(CMD0, 0, 0x95);
    if (retval < 0)
    {
        SD_CS_SET();
        return -1;
    }

    if (retval == 1)
    {
        retval = _bSD_SendCmd(CMD8, 0x1AA, 0x87);
        if (retval < 0)
        {
            SD_CS_SET();
            return -1;
        }
    }
    else
    {
        SD_CS_SET();
        return -1;
    }

    if (retval == 0x5)
    {
        bSD_Driver._private.v = CT_SD1;
        SD_CS_SET();
        _bSD_SendDump(1);
        cnt = 0;
        SD_CS_RESET();
        do
        {
            retval = _bSD_SendCmd(CMD55, 0, 0);
            if (retval < 0)
            {
                SD_CS_SET();
                return -1;
            }
            retval = _bSD_SendCmd(CMD41, 0, 0);
            if (retval < 0)
            {
                SD_CS_SET();
                return -1;
            }
            cnt++;
        } while ((retval != 0) && (cnt < 400));
        if (cnt >= 400)
        {
            SD_CS_SET();
            return -1;
        }
        bHalSpiSetSpeed(&bSD_HalIf, B_HAL_SPI_FAST);
        _bSD_SendDump(1);
        retval = _bSD_SendCmd(CMD59, 0, 0x95);
        if (retval < 0)
        {
            SD_CS_SET();
            return -1;
        }
        retval = _bSD_SendCmd(CMD16, 512, 0x95);
        if (retval < 0)
        {
            SD_CS_SET();
            return -1;
        }
    }
    else if (retval == 0x1)
    {
        ocr[0] = bHalSpiTransfer(&bSD_HalIf, 0xff);
        ocr[1] = bHalSpiTransfer(&bSD_HalIf, 0xff);
        ocr[2] = bHalSpiTransfer(&bSD_HalIf, 0xff);
        ocr[3] = bHalSpiTransfer(&bSD_HalIf, 0xff);
        SD_CS_SET();
        _bSD_SendDump(1);
        cnt = 0;
        SD_CS_RESET();
        do
        {
            retval = _bSD_SendCmd(CMD55, 0, 0);
            if (retval < 0)
            {
                SD_CS_SET();
                return -1;
            }
            retval = _bSD_SendCmd(CMD41, 0x40000000, 0);
            if (retval < 0)
            {
                SD_CS_SET();
                return -1;
            }
            cnt++;
        } while ((retval != 0) && (cnt < 400));
        if (cnt >= 400)
        {
            SD_CS_SET();
            return -1;
        }
        retval = _bSD_SendCmd(CMD58, 0, 0);
        if (retval != 0)
        {
            SD_CS_SET();
            return -1;
        }
        ocr[0] = bHalSpiTransfer(&bSD_HalIf, 0xff);
        ocr[1] = bHalSpiTransfer(&bSD_HalIf, 0xff);
        ocr[2] = bHalSpiTransfer(&bSD_HalIf, 0xff);
        ocr[3] = bHalSpiTransfer(&bSD_HalIf, 0xff);
        if (ocr[0] & 0x40)
        {
            bSD_Driver._private.v = CT_SDHC;
        }
        else
        {
            bSD_Driver._private.v = CT_SD2;
        }
    }
    SD_CS_SET();
    _bSD_SendDump(1);
    bHalSpiSetSpeed(&bSD_HalIf, B_HAL_SPI_FAST);
    b_log("sd type:%d\r\n", bSD_Driver._private.v);
    return 0;
}

static int _bSD_WaitResponse(uint8_t exp)
{
    uint16_t cnt = 0;
    uint8_t  tmp;
    do
    {
        tmp = bHalSpiTransfer(&bSD_HalIf, 0xff);
        cnt++;
    } while (tmp != exp && cnt <= 0xfff);
    if (tmp == exp)
    {
        return 0;
    }
    return -1;
}

static int _bSD_ReceiveData(uint8_t *buff, uint16_t len)
{
    if (_bSD_WaitResponse(0xfe) < 0)
    {
        return -1;
    }
    while (len--)
    {
        *buff = bHalSpiTransfer(&bSD_HalIf, 0xff);
        buff++;
    }
    bHalSpiTransfer(&bSD_HalIf, 0xff);
    bHalSpiTransfer(&bSD_HalIf, 0xff);
    return len;
}

static int _bSD_ReadSingleBlock(uint32_t sector, uint8_t *pbuf)
{
    int retval = 0;
    if (bSD_Driver._private.v != CT_SDHC)
    {
        sector = sector << 9;
    }
    SD_CS_RESET();
    retval = _bSD_SendCmd(CMD17, sector, 0);
    if (retval != 0)
    {
        SD_CS_SET();
        return -1;
    }
    SD_CS_SET();
    _bSD_SendDump(1);

    SD_CS_RESET();
    if (_bSD_ReceiveData(pbuf, 512) < 0)
    {
        SD_CS_SET();
        return -1;
    }
    SD_CS_SET();
    _bSD_SendDump(1);
    return 512;
}

static int _bSD_WriteSingleBlock(uint32_t sector, uint8_t *pbuf)
{
    int      retval = 0;
    uint16_t cnt;
    if (bSD_Driver._private.v != CT_SDHC)
    {
        sector = sector << 9;
    }
    SD_CS_RESET();
    retval = _bSD_SendCmd(CMD24, sector, 0);
    if (retval != 0)
    {
        SD_CS_SET();
        return -1;
    }
    SD_CS_SET();
    _bSD_SendDump(1);

    SD_CS_RESET();
    _bSD_SendDump(3);
    bHalSpiTransfer(&bSD_HalIf, 0xfe);
    bHalSpiSend(&bSD_HalIf, pbuf, 512);
    _bSD_SendDump(2);
    retval = bHalSpiTransfer(&bSD_HalIf, 0xff);
    if ((retval & 0x1f) != 0x05)
    {
        SD_CS_SET();
        return -1;
    }
    cnt = 0;
    while (!bHalSpiTransfer(&bSD_HalIf, 0xff))
    {
        cnt++;
        if (cnt >= 0xfffe)
        {
            SD_CS_SET();
            return -1;
        }
    }
    SD_CS_SET();
    _bSD_SendDump(1);
    return 0;
}

// sector:  Sector number to write from
// count: Number of sectors to write
static int _bSD_Write(bSD_Driver_t *pdrv, uint32_t sector, uint8_t *pbuf, uint16_t count)
{
    int i = 0;
    for (i = 0; i < count; i++)
    {
        _bSD_WriteSingleBlock(sector + i, pbuf + i * 512);
    }
    return count;
}

// sector:  Sector number to write from
// count: Number of sectors to write
static int _bSD_Read(bSD_Driver_t *pdrv, uint32_t sector, uint8_t *pbuf, uint16_t count)
{
    int i = 0;
    for (i = 0; i < count; i++)
    {
        _bSD_ReadSingleBlock(sector + i, pbuf + i * 512);
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
int bSD_Init()
{
    if (_bSD_Init() < 0)
    {
        b_log("sd_err\r\n");
        return -1;
    }
    bSD_Driver.status  = 0;
    bSD_Driver.init    = bSD_Init;
    bSD_Driver.close   = NULL;
    bSD_Driver.read    = _bSD_Read;
    bSD_Driver.ctl     = NULL;
    bSD_Driver.open    = NULL;
    bSD_Driver.write   = _bSD_Write;
    bSD_Driver._hal_if = (void *)&bSD_HalIf;
    return 0;
}

bDRIVER_REG_INIT(bSD_Init);

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
