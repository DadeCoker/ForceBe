/**
 *!
 * \file        b_drv_ili9341.c
 * \version     v0.0.1
 * \date        2020/03/02
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
#include "drivers/inc/b_drv_ili9341.h"

/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup ILI9341
 * \{
 */

/**
 * \defgroup ILI9341_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup ILI9341_Private_Defines
 * \{
 */
#ifndef LCD_X_SIZE
#define LCD_X_SIZE 240
#endif

#ifndef LCD_Y_SIZE
#define LCD_Y_SIZE 320
#endif
/**
 * \}
 */

/**
 * \defgroup ILI9341_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup ILI9341_Private_Variables
 * \{
 */
HALIF_KEYWORD bILI9341_HalIf_t bILI9341_HalIf = HAL_ILI9341_IF;
bILI9341_Driver_t              bILI9341_Driver;
/**
 * \}
 */

/**
 * \defgroup ILI9341_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup ILI9341_Private_Functions
 * \{
 */

static void _bLcdWriteData(uint16_t dat)
{
    if (bILI9341_HalIf.if_type == LCD_IF_TYPE_RWADDR)
    {
        ((bLcdRWAddress_t *)bILI9341_HalIf._if.rw_addr)->dat = dat;
    }
    else if (bILI9341_HalIf.if_type == LCD_IF_TYPE_IO)
    {
        bHalGpioWritePin(bILI9341_HalIf._if._io.rs.port, bILI9341_HalIf._if._io.rs.pin, 1);
        bHalGpioWritePin(bILI9341_HalIf._if._io.rd.port, bILI9341_HalIf._if._io.rd.pin, 1);
        bHalGpioWritePin(bILI9341_HalIf._if._io.cs.port, bILI9341_HalIf._if._io.cs.pin, 0);
        bHalGpioWritePort(bILI9341_HalIf._if._io.data.port, dat);
        bHalGpioWritePin(bILI9341_HalIf._if._io.wr.port, bILI9341_HalIf._if._io.wr.pin, 0);
        bHalGpioWritePin(bILI9341_HalIf._if._io.wr.port, bILI9341_HalIf._if._io.wr.pin, 1);
        bHalGpioWritePin(bILI9341_HalIf._if._io.cs.port, bILI9341_HalIf._if._io.cs.pin, 1);
    }
    else if (bILI9341_HalIf.if_type == LCD_IF_TYPE_SPI)
    {
        bHalGpioWritePin(bILI9341_HalIf._if._spi.rs.port, bILI9341_HalIf._if._spi.rs.pin, 1);
        bHalGpioWritePin(bILI9341_HalIf._if._spi._spi.cs.port, bILI9341_HalIf._if._spi._spi.cs.pin,
                         0);
        bHalSpiSend(&bILI9341_HalIf._if._spi._spi, (uint8_t *)&dat, 1);
        bHalGpioWritePin(bILI9341_HalIf._if._spi._spi.cs.port, bILI9341_HalIf._if._spi._spi.cs.pin,
                         1);
    }
}

static void _bLcdWriteGRam(uint16_t dat)
{
    if (bILI9341_HalIf.if_type == LCD_IF_TYPE_RWADDR || bILI9341_HalIf.if_type == LCD_IF_TYPE_IO)
    {
        _bLcdWriteData(dat);
    }
    else if (bILI9341_HalIf.if_type == LCD_IF_TYPE_SPI)
    {
        bHalGpioWritePin(bILI9341_HalIf._if._spi.rs.port, bILI9341_HalIf._if._spi.rs.pin, 1);
        bHalGpioWritePin(bILI9341_HalIf._if._spi._spi.cs.port, bILI9341_HalIf._if._spi._spi.cs.pin,
                         0);
        bHalSpiSend(&bILI9341_HalIf._if._spi._spi, ((uint8_t *)&dat) + 1, 1);
        bHalSpiSend(&bILI9341_HalIf._if._spi._spi, ((uint8_t *)&dat), 1);
        bHalGpioWritePin(bILI9341_HalIf._if._spi._spi.cs.port, bILI9341_HalIf._if._spi._spi.cs.pin,
                         1);
    }
}

static void _bLcdWriteCmd(uint16_t cmd)
{
    if (bILI9341_HalIf.if_type == LCD_IF_TYPE_RWADDR)
    {
        ((bLcdRWAddress_t *)bILI9341_HalIf._if.rw_addr)->reg = cmd;
    }
    else if (bILI9341_HalIf.if_type == LCD_IF_TYPE_IO)
    {
        bHalGpioWritePin(bILI9341_HalIf._if._io.rs.port, bILI9341_HalIf._if._io.rs.pin, 0);
        bHalGpioWritePin(bILI9341_HalIf._if._io.rd.port, bILI9341_HalIf._if._io.rd.pin, 1);
        bHalGpioWritePin(bILI9341_HalIf._if._io.cs.port, bILI9341_HalIf._if._io.cs.pin, 0);
        bHalGpioWritePort(bILI9341_HalIf._if._io.data.port, cmd);
        bHalGpioWritePin(bILI9341_HalIf._if._io.wr.port, bILI9341_HalIf._if._io.wr.pin, 0);
        bHalGpioWritePin(bILI9341_HalIf._if._io.wr.port, bILI9341_HalIf._if._io.wr.pin, 1);
        bHalGpioWritePin(bILI9341_HalIf._if._io.cs.port, bILI9341_HalIf._if._io.cs.pin, 1);
    }
    else if (bILI9341_HalIf.if_type == LCD_IF_TYPE_SPI)
    {
        bHalGpioWritePin(bILI9341_HalIf._if._spi.rs.port, bILI9341_HalIf._if._spi.rs.pin, 0);
        bHalGpioWritePin(bILI9341_HalIf._if._spi._spi.cs.port, bILI9341_HalIf._if._spi._spi.cs.pin,
                         0);
        bHalSpiSend(&bILI9341_HalIf._if._spi._spi, (uint8_t *)&cmd, 1);
        bHalGpioWritePin(bILI9341_HalIf._if._spi._spi.cs.port, bILI9341_HalIf._if._spi._spi.cs.pin,
                         1);
    }
}

static uint16_t _bLcdReadData()
{
    uint16_t dat;

    if (bILI9341_HalIf.if_type == LCD_IF_TYPE_RWADDR)
    {
        dat = ((bLcdRWAddress_t *)bILI9341_HalIf._if.rw_addr)->dat;
    }
    else if (bILI9341_HalIf.if_type == LCD_IF_TYPE_IO)
    {
        bHalGpioConfig(bILI9341_HalIf._if._io.data.port, bILI9341_HalIf._if._io.data.pin,
                       B_HAL_GPIO_INPUT, B_HAL_GPIO_NOPULL);
        bHalGpioWritePin(bILI9341_HalIf._if._io.rs.port, bILI9341_HalIf._if._io.rs.pin, 1);
        bHalGpioWritePin(bILI9341_HalIf._if._io.rd.port, bILI9341_HalIf._if._io.rd.pin, 0);
        bHalGpioWritePin(bILI9341_HalIf._if._io.cs.port, bILI9341_HalIf._if._io.cs.pin, 0);
        bHalGpioWritePin(bILI9341_HalIf._if._io.rd.port, bILI9341_HalIf._if._io.rd.pin, 1);
        dat = bHalGpioReadPort(bILI9341_HalIf._if._io.data.port);
        bHalGpioWritePin(bILI9341_HalIf._if._io.cs.port, bILI9341_HalIf._if._io.cs.pin, 1);
        bHalGpioConfig(bILI9341_HalIf._if._io.data.port, bILI9341_HalIf._if._io.data.pin,
                       B_HAL_GPIO_OUTPUT, B_HAL_GPIO_NOPULL);
    }
    return dat;
}

static int _bILI9341CheckId()
{
    uint16_t id = 0x9341;
    if (bILI9341_HalIf.if_type == LCD_IF_TYPE_IO || bILI9341_HalIf.if_type == LCD_IF_TYPE_RWADDR)
    {
        _bLcdWriteCmd(0XD3);
        id = _bLcdReadData();
        id = _bLcdReadData();
        id = _bLcdReadData();
        id <<= 8;
        id |= _bLcdReadData();
    }
    else if (bILI9341_HalIf.if_type == LCD_IF_TYPE_SPI)
    {
    }
    return ((id == 0x9341) ? 0 : -1);
}

static void _bILI9341SetCursor(uint16_t Xpos, uint16_t Ypos)
{
    _bLcdWriteCmd(0X2A);
    _bLcdWriteData(Xpos >> 8);
    _bLcdWriteData(Xpos & 0XFF);
    _bLcdWriteCmd(0X2B);
    _bLcdWriteData(Ypos >> 8);
    _bLcdWriteData(Ypos & 0XFF);
    _bLcdWriteCmd(0X2C);
}

static int _bILI9341FillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    int      i   = 0;
    uint16_t tmp = 0;
    if (x1 > x2)
    {
        tmp = x1;
        x1  = x2;
        x2  = tmp;
    }
    if (y1 > y2)
    {
        tmp = y1;
        y1  = y2;
        y2  = tmp;
    }

    if (x2 >= LCD_X_SIZE || y2 >= LCD_Y_SIZE)
    {
        return -1;
    }

    _bLcdWriteCmd(0x2a);
    _bLcdWriteData(x1 >> 8);
    _bLcdWriteData(x1);
    _bLcdWriteData(x2 >> 8);
    _bLcdWriteData(x2);
    _bLcdWriteCmd(0x2b);
    _bLcdWriteData(y1 >> 8);
    _bLcdWriteData(y1);
    _bLcdWriteData(y2 >> 8);
    _bLcdWriteData(y2);
    _bLcdWriteCmd(0x2C);

    for (i = 0; i < ((x2 - x1 + 1) * (y2 - y1 + 1)); i++)
    {
        _bLcdWriteGRam(color);
    }
    return 0;
}

static int _bILI9341Ctl(bILI9341_Driver_t *pdrv, uint8_t cmd, void *param)
{
    int             retval = -1;
    bLcdRectInfo_t *pinfo  = (bLcdRectInfo_t *)param;
    switch (cmd)
    {
        case bCMD_FILL_RECT:
            if (param == NULL)
            {
                return -1;
            }
            retval = _bILI9341FillRect(pinfo->x1, pinfo->y1, pinfo->x2, pinfo->y2, pinfo->color);
            break;
        default:
            break;
    }
    return retval;
}

static int _bILI9341Write(bILI9341_Driver_t *pdrv, uint32_t addr, uint8_t *pbuf, uint16_t len)
{
    uint16_t     x      = addr % LCD_X_SIZE;
    uint16_t     y      = addr / LCD_X_SIZE;
    bLcdWrite_t *pcolor = (bLcdWrite_t *)pbuf;
    if (y >= LCD_Y_SIZE || pbuf == NULL || len < sizeof(bLcdWrite_t))
    {
        return -1;
    }
    _bILI9341SetCursor(x, y);
    _bLcdWriteGRam(pcolor->color);
    return 2;
}

/**
 * \}
 */

/**
 * \addtogroup ILI9341_Exported_Functions
 * \{
 */

int bILI9341_Init()
{
    if (_bILI9341CheckId() < 0)
    {
        bILI9341_Driver.status = -1;
        return -1;
    }
    _bLcdWriteCmd(0xCF);
    _bLcdWriteData(0x00);
    _bLcdWriteData(0xC1);
    _bLcdWriteData(0X30);
    _bLcdWriteCmd(0xED);
    _bLcdWriteData(0x64);
    _bLcdWriteData(0x03);
    _bLcdWriteData(0X12);
    _bLcdWriteData(0X81);
    _bLcdWriteCmd(0xE8);
    _bLcdWriteData(0x85);
    _bLcdWriteData(0x10);
    _bLcdWriteData(0x7A);
    _bLcdWriteCmd(0xCB);
    _bLcdWriteData(0x39);
    _bLcdWriteData(0x2C);
    _bLcdWriteData(0x00);
    _bLcdWriteData(0x34);
    _bLcdWriteData(0x02);
    _bLcdWriteCmd(0xF7);
    _bLcdWriteData(0x20);
    _bLcdWriteCmd(0xEA);
    _bLcdWriteData(0x00);
    _bLcdWriteData(0x00);
    _bLcdWriteCmd(0xC0);
    _bLcdWriteData(0x1B);
    _bLcdWriteCmd(0xC1);
    _bLcdWriteData(0x01);
    _bLcdWriteCmd(0xC5);
    _bLcdWriteData(0x30);
    _bLcdWriteData(0x30);
    _bLcdWriteCmd(0xC7);
    _bLcdWriteData(0XB7);
    _bLcdWriteCmd(0x36);
    _bLcdWriteData(0x08);
    _bLcdWriteCmd(0x3A);
    _bLcdWriteData(0x55);
    _bLcdWriteCmd(0xB1);
    _bLcdWriteData(0x00);
    _bLcdWriteData(0x1A);
    _bLcdWriteCmd(0xB6);
    _bLcdWriteData(0x0A);
    _bLcdWriteData(0xA2);
    _bLcdWriteCmd(0xF2);
    _bLcdWriteData(0x00);
    _bLcdWriteCmd(0x26);
    _bLcdWriteData(0x01);
    _bLcdWriteCmd(0xE0);
    _bLcdWriteData(0x0F);
    _bLcdWriteData(0x2A);
    _bLcdWriteData(0x28);
    _bLcdWriteData(0x08);
    _bLcdWriteData(0x0E);
    _bLcdWriteData(0x08);
    _bLcdWriteData(0x54);
    _bLcdWriteData(0XA9);
    _bLcdWriteData(0x43);
    _bLcdWriteData(0x0A);
    _bLcdWriteData(0x0F);
    _bLcdWriteData(0x00);
    _bLcdWriteData(0x00);
    _bLcdWriteData(0x00);
    _bLcdWriteData(0x00);
    _bLcdWriteCmd(0XE1);
    _bLcdWriteData(0x00);
    _bLcdWriteData(0x15);
    _bLcdWriteData(0x17);
    _bLcdWriteData(0x07);
    _bLcdWriteData(0x11);
    _bLcdWriteData(0x06);
    _bLcdWriteData(0x2B);
    _bLcdWriteData(0x56);
    _bLcdWriteData(0x3C);
    _bLcdWriteData(0x05);
    _bLcdWriteData(0x10);
    _bLcdWriteData(0x0F);
    _bLcdWriteData(0x3F);
    _bLcdWriteData(0x3F);
    _bLcdWriteData(0x0F);
    _bLcdWriteCmd(0x2B);
    _bLcdWriteData(0x00);
    _bLcdWriteData(0x00);
    _bLcdWriteData(0x01);
    _bLcdWriteData(0x3f);
    _bLcdWriteCmd(0x2A);
    _bLcdWriteData(0x00);
    _bLcdWriteData(0x00);
    _bLcdWriteData(0x00);
    _bLcdWriteData(0xef);
    _bLcdWriteCmd(0x11);
    bHalDelayMs(120);
    _bLcdWriteCmd(0x29);

    bILI9341_Driver.status  = 0;
    bILI9341_Driver.init    = bILI9341_Init;
    bILI9341_Driver.close   = NULL;
    bILI9341_Driver.read    = NULL;
    bILI9341_Driver.ctl     = _bILI9341Ctl;
    bILI9341_Driver.open    = NULL;
    bILI9341_Driver.write   = _bILI9341Write;
    bILI9341_Driver._hal_if = (void *)&bILI9341_HalIf;
    return 0;
}

bDRIVER_REG_INIT(bILI9341_Init);

/**
 * \}
 */

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
