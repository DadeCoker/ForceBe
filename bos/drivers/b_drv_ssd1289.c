/**
 *!
 * \file        b_drv_ssd1289.c
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
#include "drivers/inc/b_drv_ssd1289.h"

/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup SSD1289
 * \{
 */

/**
 * \defgroup SSD1289_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SSD1289_Private_Defines
 * \{
 */
#define DRIVER_NAME SSD1289

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
 * \defgroup SSD1289_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SSD1289_Private_Variables
 * \{
 */
bDRIVER_HALIF_TABLE(bSSD1289_HalIf_t, DRIVER_NAME);
/**
 * \}
 */

/**
 * \defgroup SSD1289_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SSD1289_Private_Functions
 * \{
 */

static void _bLcdWriteData(bDriverInterface_t *pdrv, uint16_t dat)
{
    bDRIVER_GET_HALIF(_if, bSSD1289_HalIf_t, pdrv);
    if (_if->if_type == LCD_IF_TYPE_RWADDR)
    {
        ((bLcdRWAddress_t *)_if->_if.rw_addr)->dat = dat;
    }
    if (_if->if_type == LCD_IF_TYPE_IO)
    {
        bHalGpioWritePin(_if->_if._io.rs.port, _if->_if._io.rs.pin, 1);
        bHalGpioWritePin(_if->_if._io.rd.port, _if->_if._io.rd.pin, 1);
        bHalGpioWritePin(_if->_if._io.cs.port, _if->_if._io.cs.pin, 0);
        bHalGpioWritePort(_if->_if._io.data.port, dat);
        bHalGpioWritePin(_if->_if._io.wr.port, _if->_if._io.wr.pin, 0);
        bHalGpioWritePin(_if->_if._io.wr.port, _if->_if._io.wr.pin, 1);
        bHalGpioWritePin(_if->_if._io.cs.port, _if->_if._io.cs.pin, 1);
    }
}

static void _bLcdWriteCmd(bDriverInterface_t *pdrv, uint16_t cmd)
{
    bDRIVER_GET_HALIF(_if, bSSD1289_HalIf_t, pdrv);
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
}

static void _SSD1289WriteReg(bDriverInterface_t *pdrv, uint16_t cmd, uint16_t dat)
{
    _bLcdWriteCmd(pdrv, cmd);
    _bLcdWriteData(pdrv, dat);
}

static void _bSSD1289SetWindow(bDriverInterface_t *pdrv, uint16_t x1, uint16_t y1, uint16_t x2,
                               uint16_t y2)
{
    _SSD1289WriteReg(pdrv, 0x0044, ((x2 << 8) | x1));
    _SSD1289WriteReg(pdrv, 0x0045, y1);
    _SSD1289WriteReg(pdrv, 0x0046, y2);
    _SSD1289WriteReg(pdrv, 0x004E, x1);
    _SSD1289WriteReg(pdrv, 0x004F, y1);
    _bLcdWriteCmd(pdrv, 0x0022);
}

/***********************************************************************driver interface*******/

static int _bSSD1289FillRect(bDriverInterface_t *pdrv, uint16_t x1, uint16_t y1, uint16_t x2,
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

    _bSSD1289SetWindow(pdrv, x1, y1, x2, y2);

    for (i = 0; i < ((x2 - x1 + 1) * (y2 - y1 + 1)); i++)
    {
        _bLcdWriteData(pdrv, color);
    }
    return 0;
}

static int _bSSD1289Ctl(bDriverInterface_t *pdrv, uint8_t cmd, void *param)
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
                _bSSD1289FillRect(pdrv, pinfo->x1, pinfo->y1, pinfo->x2, pinfo->y2, pinfo->color);
            break;
        default:
            break;
    }
    return retval;
}

static int _bSSD1289Write(bDriverInterface_t *pdrv, uint32_t addr, uint8_t *pbuf, uint32_t len)
{
    uint16_t     x      = addr % LCD_X_SIZE;
    uint16_t     y      = addr / LCD_X_SIZE;
    bLcdWrite_t *pcolor = (bLcdWrite_t *)pbuf;
    if (y >= LCD_Y_SIZE || pbuf == NULL || len < sizeof(bLcdWrite_t))
    {
        return -1;
    }
    _bSSD1289SetWindow(pdrv, x, y, x, y);
    _bLcdWriteData(pdrv, pcolor->color);
    return 2;
}

/**
 * \}
 */

/**
 * \addtogroup SSD1289_Exported_Functions
 * \{
 */
int bSSD1289_Init(bDriverInterface_t *pdrv)
{

    bDRIVER_STRUCT_INIT(pdrv, DRIVER_NAME, bSSD1289_Init);
    pdrv->ctl   = _bSSD1289Ctl;
    pdrv->write = _bSSD1289Write;

    _SSD1289WriteReg(pdrv, 0x0007, 0x0021);
    _SSD1289WriteReg(pdrv, 0x0000, 0x0001);
    _SSD1289WriteReg(pdrv, 0x0007, 0x0023);
    _SSD1289WriteReg(pdrv, 0x0010, 0x0000);
    _SSD1289WriteReg(pdrv, 0x0007, 0x0033);
    _SSD1289WriteReg(pdrv, 0x0011, 0x6838);
    _SSD1289WriteReg(pdrv, 0x0002, 0x0600);
    _SSD1289WriteReg(pdrv, 0x0001, 0x2B3F);
    return 0;
}

bDRIVER_REG_INIT(B_DRIVER_SSD1289, bSSD1289_Init);

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
