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
HALIF_KEYWORD bST7789_HalIf_t bST7789_HalIf = HAL_ST7789_IF;
bST7789_Driver_t              bST7789_Driver;
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

static void _bLcdWriteData(uint16_t dat)
{
    if (bST7789_HalIf.if_type == LCD_IF_TYPE_RWADDR)
    {
        ((bLcdRWAddress_t *)bST7789_HalIf._if.rw_addr)->dat = dat;
    }
    else if (bST7789_HalIf.if_type == LCD_IF_TYPE_IO)
    {
        bHalGpioWritePin(bST7789_HalIf._if._io.rs.port, bST7789_HalIf._if._io.rs.pin, 1);
        bHalGpioWritePin(bST7789_HalIf._if._io.rd.port, bST7789_HalIf._if._io.rd.pin, 1);
        bHalGpioWritePin(bST7789_HalIf._if._io.cs.port, bST7789_HalIf._if._io.cs.pin, 0);
        bHalGpioWritePort(bST7789_HalIf._if._io.data.port, dat);
        bHalGpioWritePin(bST7789_HalIf._if._io.wr.port, bST7789_HalIf._if._io.wr.pin, 0);
        bHalGpioWritePin(bST7789_HalIf._if._io.wr.port, bST7789_HalIf._if._io.wr.pin, 1);
        bHalGpioWritePin(bST7789_HalIf._if._io.cs.port, bST7789_HalIf._if._io.cs.pin, 1);
    }
    else if (bST7789_HalIf.if_type == LCD_IF_TYPE_SPI)
    {
        bHalGpioWritePin(bST7789_HalIf._if._spi.rs.port, bST7789_HalIf._if._spi.rs.pin, 1);
        bHalGpioWritePin(bST7789_HalIf._if._spi._spi.cs.port, bST7789_HalIf._if._spi._spi.cs.pin,
                         0);
        bHalSpiSend(&bST7789_HalIf._if._spi._spi, (uint8_t *)&dat, 1);
        bHalGpioWritePin(bST7789_HalIf._if._spi._spi.cs.port, bST7789_HalIf._if._spi._spi.cs.pin,
                         1);
    }
}

static void _bLcdWriteGRam(uint16_t dat)
{
    if (bST7789_HalIf.if_type == LCD_IF_TYPE_RWADDR || bST7789_HalIf.if_type == LCD_IF_TYPE_IO)
    {
        _bLcdWriteData(dat);
    }
    else if (bST7789_HalIf.if_type == LCD_IF_TYPE_SPI)
    {
        bHalGpioWritePin(bST7789_HalIf._if._spi.rs.port, bST7789_HalIf._if._spi.rs.pin, 1);
        bHalGpioWritePin(bST7789_HalIf._if._spi._spi.cs.port, bST7789_HalIf._if._spi._spi.cs.pin,
                         0);
        bHalSpiSend(&bST7789_HalIf._if._spi._spi, ((uint8_t *)&dat) + 1, 1);
        bHalSpiSend(&bST7789_HalIf._if._spi._spi, ((uint8_t *)&dat), 1);
        bHalGpioWritePin(bST7789_HalIf._if._spi._spi.cs.port, bST7789_HalIf._if._spi._spi.cs.pin,
                         1);
    }
}

static void _bLcdWriteCmd(uint16_t cmd)
{
    if (bST7789_HalIf.if_type == LCD_IF_TYPE_RWADDR)
    {
        ((bLcdRWAddress_t *)bST7789_HalIf._if.rw_addr)->reg = cmd;
    }
    else if (bST7789_HalIf.if_type == LCD_IF_TYPE_IO)
    {
        bHalGpioWritePin(bST7789_HalIf._if._io.rs.port, bST7789_HalIf._if._io.rs.pin, 0);
        bHalGpioWritePin(bST7789_HalIf._if._io.rd.port, bST7789_HalIf._if._io.rd.pin, 1);
        bHalGpioWritePin(bST7789_HalIf._if._io.cs.port, bST7789_HalIf._if._io.cs.pin, 0);
        bHalGpioWritePort(bST7789_HalIf._if._io.data.port, cmd);
        bHalGpioWritePin(bST7789_HalIf._if._io.wr.port, bST7789_HalIf._if._io.wr.pin, 0);
        bHalGpioWritePin(bST7789_HalIf._if._io.wr.port, bST7789_HalIf._if._io.wr.pin, 1);
        bHalGpioWritePin(bST7789_HalIf._if._io.cs.port, bST7789_HalIf._if._io.cs.pin, 1);
    }
    else if (bST7789_HalIf.if_type == LCD_IF_TYPE_SPI)
    {
        bHalGpioWritePin(bST7789_HalIf._if._spi.rs.port, bST7789_HalIf._if._spi.rs.pin, 0);
        bHalGpioWritePin(bST7789_HalIf._if._spi._spi.cs.port, bST7789_HalIf._if._spi._spi.cs.pin,
                         0);
        bHalSpiSend(&bST7789_HalIf._if._spi._spi, (uint8_t *)&cmd, 1);
        bHalGpioWritePin(bST7789_HalIf._if._spi._spi.cs.port, bST7789_HalIf._if._spi._spi.cs.pin,
                         1);
    }
}

static uint16_t _bLcdReadData()
{
    uint16_t dat;

    if (bST7789_HalIf.if_type == LCD_IF_TYPE_RWADDR)
    {
        dat = ((bLcdRWAddress_t *)bST7789_HalIf._if.rw_addr)->dat;
    }
    else if (bST7789_HalIf.if_type == LCD_IF_TYPE_IO)
    {
        bHalGpioConfig(bST7789_HalIf._if._io.data.port, bST7789_HalIf._if._io.data.pin,
                       B_HAL_GPIO_INPUT, B_HAL_GPIO_NOPULL);
        bHalGpioWritePin(bST7789_HalIf._if._io.rs.port, bST7789_HalIf._if._io.rs.pin, 1);
        bHalGpioWritePin(bST7789_HalIf._if._io.rd.port, bST7789_HalIf._if._io.rd.pin, 0);
        bHalGpioWritePin(bST7789_HalIf._if._io.cs.port, bST7789_HalIf._if._io.cs.pin, 0);
        bHalGpioWritePin(bST7789_HalIf._if._io.rd.port, bST7789_HalIf._if._io.rd.pin, 1);
        dat = bHalGpioReadPort(bST7789_HalIf._if._io.data.port);
        bHalGpioWritePin(bST7789_HalIf._if._io.cs.port, bST7789_HalIf._if._io.cs.pin, 1);
        bHalGpioConfig(bST7789_HalIf._if._io.data.port, bST7789_HalIf._if._io.data.pin,
                       B_HAL_GPIO_OUTPUT, B_HAL_GPIO_NOPULL);
    }
    return dat;
}

static int _bST7789CheckId()
{
    _bLcdReadData();
    return 0;
}

static void _bST7789SetCursor(uint16_t Xpos, uint16_t Ypos)
{
    _bLcdWriteCmd(0X2A);
    _bLcdWriteData(Xpos >> 8);
    _bLcdWriteData(Xpos & 0XFF);
    _bLcdWriteCmd(0X2B);
    _bLcdWriteData(Ypos >> 8);
    _bLcdWriteData(Ypos & 0XFF);
    _bLcdWriteCmd(0X2C);
}

static int _bST7789Write(bST7789_Driver_t *pdrv, uint32_t addr, uint8_t *pbuf, uint16_t len)
{
    uint16_t     x      = addr % LCD_X_SIZE;
    uint16_t     y      = addr / LCD_X_SIZE;
    bLcdWrite_t *pcolor = (bLcdWrite_t *)pbuf;
    if (y >= LCD_Y_SIZE || pbuf == NULL || len < sizeof(bLcdWrite_t))
    {
        return -1;
    }
    _bST7789SetCursor(x, y);
    _bLcdWriteGRam(pcolor->color);
    return 2;
}

/**
 * \}
 */

/**
 * \addtogroup ST7789_Exported_Functions
 * \{
 */

int bST7789_Init()
{
    if (_bST7789CheckId() < 0)
    {
        bST7789_Driver.status = -1;
        return -1;
    }
    bHalDelayMs(12);
    /* Sleep Out */
    _bLcdWriteCmd(0x11);
    /* wait for power stability */
    bHalDelayMs(12);

    /* Memory Data Access Control */
    _bLcdWriteCmd(0x36);
    _bLcdWriteData(0x00);

    /* RGB 5-6-5-bit  */
    _bLcdWriteCmd(0x3A);
    _bLcdWriteData(0x65);

    /* Porch Setting */
    _bLcdWriteCmd(0xB2);
    _bLcdWriteData(0x0C);
    _bLcdWriteData(0x0C);
    _bLcdWriteData(0x00);
    _bLcdWriteData(0x33);
    _bLcdWriteData(0x33);

    /*  Gate Control */
    _bLcdWriteCmd(0xB7);
    _bLcdWriteData(0x72);

    /* VCOM Setting */
    _bLcdWriteCmd(0xBB);
    _bLcdWriteData(0x3D);  // Vcom=1.625V

    /* LCM Control */
    _bLcdWriteCmd(0xC0);
    _bLcdWriteData(0x2C);

    /* VDV and VRH Command Enable */
    _bLcdWriteCmd(0xC2);
    _bLcdWriteData(0x01);

    /* VRH Set */
    _bLcdWriteCmd(0xC3);
    _bLcdWriteData(0x19);

    /* VDV Set */
    _bLcdWriteCmd(0xC4);
    _bLcdWriteData(0x20);

    /* Frame Rate Control in Normal Mode */
    _bLcdWriteCmd(0xC6);
    _bLcdWriteData(0x0F);  // 60MHZ

    /* Power Control 1 */
    _bLcdWriteCmd(0xD0);
    _bLcdWriteData(0xA4);
    _bLcdWriteData(0xA1);

    /* Positive Voltage Gamma Control */
    _bLcdWriteCmd(0xE0);
    _bLcdWriteData(0xD0);
    _bLcdWriteData(0x04);
    _bLcdWriteData(0x0D);
    _bLcdWriteData(0x11);
    _bLcdWriteData(0x13);
    _bLcdWriteData(0x2B);
    _bLcdWriteData(0x3F);
    _bLcdWriteData(0x54);
    _bLcdWriteData(0x4C);
    _bLcdWriteData(0x18);
    _bLcdWriteData(0x0D);
    _bLcdWriteData(0x0B);
    _bLcdWriteData(0x1F);
    _bLcdWriteData(0x23);

    /* Negative Voltage Gamma Control */
    _bLcdWriteCmd(0xE1);
    _bLcdWriteData(0xD0);
    _bLcdWriteData(0x04);
    _bLcdWriteData(0x0C);
    _bLcdWriteData(0x11);
    _bLcdWriteData(0x13);
    _bLcdWriteData(0x2C);
    _bLcdWriteData(0x3F);
    _bLcdWriteData(0x44);
    _bLcdWriteData(0x51);
    _bLcdWriteData(0x2F);
    _bLcdWriteData(0x1F);
    _bLcdWriteData(0x1F);
    _bLcdWriteData(0x20);
    _bLcdWriteData(0x23);

    /* Display Inversion On */
    _bLcdWriteCmd(0x21);
    _bLcdWriteCmd(0x29);

    bST7789_Driver.status  = 0;
    bST7789_Driver.init    = bST7789_Init;
    bST7789_Driver.close   = NULL;
    bST7789_Driver.read    = NULL;
    bST7789_Driver.ctl     = NULL;
    bST7789_Driver.open    = NULL;
    bST7789_Driver.write   = _bST7789Write;
    bST7789_Driver._hal_if = (void *)&bST7789_HalIf;
    return 0;
}

bDRIVER_REG_INIT(bST7789_Init);

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
