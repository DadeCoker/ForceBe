/**
 *!
 * \file        b_drv_st7789.c
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
#include "drivers/inc/b_drv_st7789.h"

/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup ST7789
 * \{
 */

/**
 * \defgroup ST7789_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup ST7789_Private_Defines
 * \{
 */
#define DRIVER_NAME ST7789

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
 * \defgroup ST7789_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup ST7789_Private_Variables
 * \{
 */
bDRIVER_HALIF_TABLE(bST7789_HalIf_t, DRIVER_NAME);
/**
 * \}
 */

/**
 * \defgroup ST7789_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup ST7789_Private_Functions
 * \{
 */

static void _bLcdWriteData(bDriverInterface_t *pdrv, uint16_t dat)
{
    bDRIVER_GET_HALIF(_if, bST7789_HalIf_t, pdrv);
    if (_if->if_type == LCD_IF_TYPE_RWADDR)
    {
        ((bLcdRWAddress_t *)_if->_if.rw_addr)->dat = dat;
    }
    else if (_if->if_type == LCD_IF_TYPE_IO)
    {
        bHalGpioWritePin(_if->_if._io.rs.port, _if->_if._io.rs.pin, 1);
        bHalGpioWritePin(_if->_if._io.rd.port, _if->_if._io.rd.pin, 1);
        bHalGpioWritePin(_if->_if._io.cs.port, _if->_if._io.cs.pin, 0);
        bHalGpioWritePort(_if->_if._io.data.port, dat);
        bHalGpioWritePin(_if->_if._io.wr.port, _if->_if._io.wr.pin, 0);
        bHalGpioWritePin(_if->_if._io.wr.port, _if->_if._io.wr.pin, 1);
        bHalGpioWritePin(_if->_if._io.cs.port, _if->_if._io.cs.pin, 1);
    }
    else if (_if->if_type == LCD_IF_TYPE_SPI)
    {
        bHalGpioWritePin(_if->_if._spi.rs.port, _if->_if._spi.rs.pin, 1);
        bHalGpioWritePin(_if->_if._spi._spi.cs.port, _if->_if._spi._spi.cs.pin, 0);
        bHalSpiSend(&_if->_if._spi._spi, (uint8_t *)&dat, 1);
        bHalGpioWritePin(_if->_if._spi._spi.cs.port, _if->_if._spi._spi.cs.pin, 1);
    }
}

static void _bLcdWriteGRam(bDriverInterface_t *pdrv, uint16_t dat)
{
    bDRIVER_GET_HALIF(_if, bST7789_HalIf_t, pdrv);
    if (_if->if_type == LCD_IF_TYPE_RWADDR || _if->if_type == LCD_IF_TYPE_IO)
    {
        _bLcdWriteData(pdrv, dat);
    }
    else if (_if->if_type == LCD_IF_TYPE_SPI)
    {
        bHalGpioWritePin(_if->_if._spi.rs.port, _if->_if._spi.rs.pin, 1);
        bHalGpioWritePin(_if->_if._spi._spi.cs.port, _if->_if._spi._spi.cs.pin, 0);
        bHalSpiSend(&_if->_if._spi._spi, ((uint8_t *)&dat) + 1, 1);
        bHalSpiSend(&_if->_if._spi._spi, ((uint8_t *)&dat), 1);
        bHalGpioWritePin(_if->_if._spi._spi.cs.port, _if->_if._spi._spi.cs.pin, 1);
    }
}

static void _bLcdWriteCmd(bDriverInterface_t *pdrv, uint16_t cmd)
{
    bDRIVER_GET_HALIF(_if, bST7789_HalIf_t, pdrv);
    if (_if->if_type == LCD_IF_TYPE_RWADDR)
    {
        ((bLcdRWAddress_t *)_if->_if.rw_addr)->reg = cmd;
    }
    else if (_if->if_type == LCD_IF_TYPE_IO)
    {
        bHalGpioWritePin(_if->_if._io.rs.port, _if->_if._io.rs.pin, 0);
        bHalGpioWritePin(_if->_if._io.rd.port, _if->_if._io.rd.pin, 1);
        bHalGpioWritePin(_if->_if._io.cs.port, _if->_if._io.cs.pin, 0);
        bHalGpioWritePort(_if->_if._io.data.port, cmd);
        bHalGpioWritePin(_if->_if._io.wr.port, _if->_if._io.wr.pin, 0);
        bHalGpioWritePin(_if->_if._io.wr.port, _if->_if._io.wr.pin, 1);
        bHalGpioWritePin(_if->_if._io.cs.port, _if->_if._io.cs.pin, 1);
    }
    else if (_if->if_type == LCD_IF_TYPE_SPI)
    {
        bHalGpioWritePin(_if->_if._spi.rs.port, _if->_if._spi.rs.pin, 0);
        bHalGpioWritePin(_if->_if._spi._spi.cs.port, _if->_if._spi._spi.cs.pin, 0);
        bHalSpiSend(&_if->_if._spi._spi, (uint8_t *)&cmd, 1);
        bHalGpioWritePin(_if->_if._spi._spi.cs.port, _if->_if._spi._spi.cs.pin, 1);
    }
}

static uint16_t _bLcdReadData(bDriverInterface_t *pdrv)
{
    uint16_t dat = 0;
    bDRIVER_GET_HALIF(_if, bST7789_HalIf_t, pdrv);
    if (_if->if_type == LCD_IF_TYPE_RWADDR)
    {
        dat = ((bLcdRWAddress_t *)_if->_if.rw_addr)->dat;
    }
    else if (_if->if_type == LCD_IF_TYPE_IO)
    {
        bHalGpioConfig(_if->_if._io.data.port, _if->_if._io.data.pin, B_HAL_GPIO_INPUT,
                       B_HAL_GPIO_NOPULL);
        bHalGpioWritePin(_if->_if._io.rs.port, _if->_if._io.rs.pin, 1);
        bHalGpioWritePin(_if->_if._io.rd.port, _if->_if._io.rd.pin, 0);
        bHalGpioWritePin(_if->_if._io.cs.port, _if->_if._io.cs.pin, 0);
        bHalGpioWritePin(_if->_if._io.rd.port, _if->_if._io.rd.pin, 1);
        dat = bHalGpioReadPort(_if->_if._io.data.port);
        bHalGpioWritePin(_if->_if._io.cs.port, _if->_if._io.cs.pin, 1);
        bHalGpioConfig(_if->_if._io.data.port, _if->_if._io.data.pin, B_HAL_GPIO_OUTPUT,
                       B_HAL_GPIO_NOPULL);
    }
    return dat;
}

static int _bST7789CheckId(bDriverInterface_t *pdrv)
{
    _bLcdReadData(pdrv);
    return 0;
}

static void _bST7789SetCursor(bDriverInterface_t *pdrv, uint16_t Xpos, uint16_t Ypos)
{
    _bLcdWriteCmd(pdrv, 0X2A);
    _bLcdWriteData(pdrv, Xpos >> 8);
    _bLcdWriteData(pdrv, Xpos & 0XFF);
    _bLcdWriteCmd(pdrv, 0X2B);
    _bLcdWriteData(pdrv, Ypos >> 8);
    _bLcdWriteData(pdrv, Ypos & 0XFF);
    _bLcdWriteCmd(pdrv, 0X2C);
}

static int _bST7789FillRect(bDriverInterface_t *pdrv, uint16_t x1, uint16_t y1, uint16_t x2,
                            uint16_t y2, uint16_t color)
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

    _bLcdWriteCmd(pdrv, 0x2a);
    _bLcdWriteData(pdrv, x1 >> 8);
    _bLcdWriteData(pdrv, x1);
    _bLcdWriteData(pdrv, x2 >> 8);
    _bLcdWriteData(pdrv, x2);
    _bLcdWriteCmd(pdrv, 0x2b);
    _bLcdWriteData(pdrv, y1 >> 8);
    _bLcdWriteData(pdrv, y1);
    _bLcdWriteData(pdrv, y2 >> 8);
    _bLcdWriteData(pdrv, y2);
    _bLcdWriteCmd(pdrv, 0x2C);

    for (i = 0; i < ((x2 - x1 + 1) * (y2 - y1 + 1)); i++)
    {
        _bLcdWriteGRam(pdrv, color);
    }
    return 0;
}

static int _bST7789Ctl(bDriverInterface_t *pdrv, uint8_t cmd, void *param)
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
            retval =
                _bST7789FillRect(pdrv, pinfo->x1, pinfo->y1, pinfo->x2, pinfo->y2, pinfo->color);
            break;
        default:
            break;
    }
    return retval;
}

static int _bST7789Write(bDriverInterface_t *pdrv, uint32_t addr, uint8_t *pbuf, uint32_t len)
{
    uint16_t     x      = addr % LCD_X_SIZE;
    uint16_t     y      = addr / LCD_X_SIZE;
    bLcdWrite_t *pcolor = (bLcdWrite_t *)pbuf;
    if (y >= LCD_Y_SIZE || pbuf == NULL || len < sizeof(bLcdWrite_t))
    {
        return -1;
    }
    _bST7789SetCursor(pdrv, x, y);
    _bLcdWriteGRam(pdrv, pcolor->color);
    return 2;
}

/**
 * \}
 */

/**
 * \addtogroup ST7789_Exported_Functions
 * \{
 */

int bST7789_Init(bDriverInterface_t *pdrv)
{
    bDRIVER_STRUCT_INIT(pdrv, DRIVER_NAME, bST7789_Init);
    pdrv->write = _bST7789Write;
    pdrv->ctl   = _bST7789Ctl;

    if (_bST7789CheckId(pdrv) < 0)
    {
        return -1;
    }
    bHalDelayMs(12);
    /* Sleep Out */
    _bLcdWriteCmd(pdrv, 0x11);
    /* wait for power stability */
    bHalDelayMs(12);

    /* Memory Data Access Control */
    _bLcdWriteCmd(pdrv, 0x36);
    _bLcdWriteData(pdrv, 0x00);

    /* RGB 5-6-5-bit  */
    _bLcdWriteCmd(pdrv, 0x3A);
    _bLcdWriteData(pdrv, 0x65);

    /* Porch Setting */
    _bLcdWriteCmd(pdrv, 0xB2);
    _bLcdWriteData(pdrv, 0x0C);
    _bLcdWriteData(pdrv, 0x0C);
    _bLcdWriteData(pdrv, 0x00);
    _bLcdWriteData(pdrv, 0x33);
    _bLcdWriteData(pdrv, 0x33);

    /*  Gate Control */
    _bLcdWriteCmd(pdrv, 0xB7);
    _bLcdWriteData(pdrv, 0x72);

    /* VCOM Setting */
    _bLcdWriteCmd(pdrv, 0xBB);
    _bLcdWriteData(pdrv, 0x3D);  // Vcom=1.625V

    /* LCM Control */
    _bLcdWriteCmd(pdrv, 0xC0);
    _bLcdWriteData(pdrv, 0x2C);

    /* VDV and VRH Command Enable */
    _bLcdWriteCmd(pdrv, 0xC2);
    _bLcdWriteData(pdrv, 0x01);

    /* VRH Set */
    _bLcdWriteCmd(pdrv, 0xC3);
    _bLcdWriteData(pdrv, 0x19);

    /* VDV Set */
    _bLcdWriteCmd(pdrv, 0xC4);
    _bLcdWriteData(pdrv, 0x20);

    /* Frame Rate Control in Normal Mode */
    _bLcdWriteCmd(pdrv, 0xC6);
    _bLcdWriteData(pdrv, 0x0F);  // 60MHZ

    /* Power Control 1 */
    _bLcdWriteCmd(pdrv, 0xD0);
    _bLcdWriteData(pdrv, 0xA4);
    _bLcdWriteData(pdrv, 0xA1);

    /* Positive Voltage Gamma Control */
    _bLcdWriteCmd(pdrv, 0xE0);
    _bLcdWriteData(pdrv, 0xD0);
    _bLcdWriteData(pdrv, 0x04);
    _bLcdWriteData(pdrv, 0x0D);
    _bLcdWriteData(pdrv, 0x11);
    _bLcdWriteData(pdrv, 0x13);
    _bLcdWriteData(pdrv, 0x2B);
    _bLcdWriteData(pdrv, 0x3F);
    _bLcdWriteData(pdrv, 0x54);
    _bLcdWriteData(pdrv, 0x4C);
    _bLcdWriteData(pdrv, 0x18);
    _bLcdWriteData(pdrv, 0x0D);
    _bLcdWriteData(pdrv, 0x0B);
    _bLcdWriteData(pdrv, 0x1F);
    _bLcdWriteData(pdrv, 0x23);

    /* Negative Voltage Gamma Control */
    _bLcdWriteCmd(pdrv, 0xE1);
    _bLcdWriteData(pdrv, 0xD0);
    _bLcdWriteData(pdrv, 0x04);
    _bLcdWriteData(pdrv, 0x0C);
    _bLcdWriteData(pdrv, 0x11);
    _bLcdWriteData(pdrv, 0x13);
    _bLcdWriteData(pdrv, 0x2C);
    _bLcdWriteData(pdrv, 0x3F);
    _bLcdWriteData(pdrv, 0x44);
    _bLcdWriteData(pdrv, 0x51);
    _bLcdWriteData(pdrv, 0x2F);
    _bLcdWriteData(pdrv, 0x1F);
    _bLcdWriteData(pdrv, 0x1F);
    _bLcdWriteData(pdrv, 0x20);
    _bLcdWriteData(pdrv, 0x23);

    /* Display Inversion On */
    _bLcdWriteCmd(pdrv, 0x21);
    _bLcdWriteCmd(pdrv, 0x29);
    return 0;
}

bDRIVER_REG_INIT(B_DRIVER_ST7789, bST7789_Init);

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
