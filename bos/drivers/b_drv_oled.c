/**
 *!
 * \file        b_drv_oled.c
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
#include "drivers/inc/b_drv_oled.h"

#include <string.h>

#include "utils/inc/b_util_log.h"

/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup OLED
 * \{
 */

/**
 * \defgroup OLED_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup OLED_Private_Defines
 * \{
 */
#ifndef LCD_X_SIZE
#define LCD_X_SIZE 128
#endif

#ifndef LCD_Y_SIZE
#define LCD_Y_SIZE 64
#endif
/**
 * \}
 */

/**
 * \defgroup OLED_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup OLED_Private_Variables
 * \{
 */
HALIF_KEYWORD bOLED_HalIf_t bOLED_HalIf = HAL_OLED_IF;
bOLED_Driver_t              bOLED_Driver;

static uint8_t bOLED_Buff[LCD_X_SIZE * LCD_Y_SIZE / 8];
/**
 * \}
 */

/**
 * \defgroup OLED_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup OLED_Private_Functions
 * \{
 */

static void _bOLED_WriteCmd(uint8_t cmd)
{
    int ret = 0;
    if (bOLED_HalIf.is_spi)
    {
        ret = -1;
        // add spi ...
    }
    else
    {
        ret = bHalI2CMemWrite(&bOLED_HalIf._if._i2c, 0x00, &cmd, 1);
    }

    if (ret < 0)
    {
        b_log_e("oled write command err\n");
    }
}

static void _bOLED_WriteData(uint8_t dat)
{
    int ret = 0;

    if (bOLED_HalIf.is_spi)
    {
        ret = -1;
        // add spi ...
    }
    else
    {
        ret = bHalI2CMemWrite(&bOLED_HalIf._if._i2c, 0x40, &dat, 1);
    }

    if (ret < 0)
    {
        b_log_e("oled write data err\n");
    }
}

static void _bOLEDSetCursor(uint8_t x, uint8_t y)
{
    _bOLED_WriteCmd(0xb0 + y);
    _bOLED_WriteCmd(x % 16);
    _bOLED_WriteCmd((x / 16) | 0x10);
}

static void _bOLEDDrawPixel(uint8_t x, uint8_t y, uint8_t t)
{
    uint16_t index;
    uint8_t  tmp = 0, off;

    index = (y / 8) * LCD_X_SIZE + x;
    tmp   = bOLED_Buff[index];
    off   = y % 8;
    if (t)
    {
        tmp |= 1 << off;
    }
    else
    {
        tmp &= ~(1 << off);
    }

    bOLED_Buff[index] = tmp;
    _bOLEDSetCursor(x, y / 8);
    _bOLED_WriteData(tmp);
}

static int _bOLEDFillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    int      i = 0, j = 0;
    uint16_t tmp = 0;
    uint16_t index;
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
    for (i = x1; i <= x2; i++)
    {
        for (j = y1; j <= y2; j++)
        {
            index = (j / 8) * LCD_X_SIZE + i;
            if (color)
            {
                bOLED_Buff[index] |= 1 << (j % 8);
            }
            else
            {
                bOLED_Buff[index] &= ~(1 << (j % 8));
            }
        }
    }
    for (i = 0; i < (LCD_Y_SIZE / 8); i++)
    {
        _bOLED_WriteCmd(0xb0 + i);  /// page0-page1
        _bOLED_WriteCmd(0x00);      /// low column start address
        _bOLED_WriteCmd(0x10);      /// high column start address
        for (j = 0; j < LCD_X_SIZE; j++)
        {
            _bOLED_WriteData(bOLED_Buff[i * LCD_X_SIZE + j]);
        }
    }
    return 0;
}

static int _bOLEDCtl(bOLED_Driver_t *pdrv, uint8_t cmd, void *param)
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
            retval = _bOLEDFillRect(pinfo->x1, pinfo->y1, pinfo->x2, pinfo->y2, pinfo->color);
            break;
        default:
            break;
    }
    return retval;
}

static int _bOLEDWrite(bOLED_Driver_t *pdrv, uint32_t addr, uint8_t *pbuf, uint32_t len)
{
    uint16_t     x      = addr % LCD_X_SIZE;
    uint16_t     y      = addr / LCD_X_SIZE;
    bLcdWrite_t *pcolor = (bLcdWrite_t *)pbuf;
    if (y >= LCD_Y_SIZE || pbuf == NULL || len < sizeof(bLcdWrite_t))
    {
        return -1;
    }
    _bOLEDDrawPixel(x, y, pcolor->color);
    return 2;
}
/**
 * \}
 */

/**
 * \addtogroup OLED_Exported_Functions
 * \{
 */
int bOLED_Init()
{
    bHalDelayMs(100);
    _bOLED_WriteCmd(0xAE);  /// display off
    _bOLED_WriteCmd(0x20);  /// Set Memory Addressing Mode
    _bOLED_WriteCmd(0x10);  /// 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page
                            /// Addressing Mode (RESET);11,Invalid
    _bOLED_WriteCmd(0xb0);  /// Set Page Start Address for Page Addressing Mode,0-7
    _bOLED_WriteCmd(0xc8);  /// Set COM Output Scan Direction
    _bOLED_WriteCmd(0x00);  ///---set low column address
    _bOLED_WriteCmd(0x10);  ///---set high column address
    _bOLED_WriteCmd(0x40);  ///--set start line address
    _bOLED_WriteCmd(0x81);  ///--set contrast control register
    _bOLED_WriteCmd(0xff);  /// 0x00~0xff **** Brightness control
    _bOLED_WriteCmd(0xa1);  ///--set segment re-map 0 to 127
    _bOLED_WriteCmd(0xa6);  ///--set normal display
    _bOLED_WriteCmd(0xa8);  ///--set multiplex ratio(1 to 64)
    _bOLED_WriteCmd(0x3F);  ///
    _bOLED_WriteCmd(0xa4);  /// 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    _bOLED_WriteCmd(0xd3);  ///-set display offset
    _bOLED_WriteCmd(0x00);  ///-not offset
    _bOLED_WriteCmd(0xd5);  ///--set display clock divide ratio/oscillator frequency
    _bOLED_WriteCmd(0xf0);  ///--set divide ratio
    _bOLED_WriteCmd(0xd9);  //--set pre-charge period
    _bOLED_WriteCmd(0x22);  ///
    _bOLED_WriteCmd(0xda);  ///--set com pins hardware configuration
    _bOLED_WriteCmd(0x12);
    _bOLED_WriteCmd(0xdb);  ///--set vcomh
    _bOLED_WriteCmd(0x20);  /// 0x20,0.77xVcc
    _bOLED_WriteCmd(0x8d);  ///--set DC-DC enable
    _bOLED_WriteCmd(0x14);  ///
    _bOLED_WriteCmd(0xaf);  ///--turn on oled panel

    memset(bOLED_Buff, 0, sizeof(bOLED_Buff));

    bOLED_Driver.init    = bOLED_Init;
    bOLED_Driver.close   = NULL;
    bOLED_Driver.read    = NULL;
    bOLED_Driver.ctl     = _bOLEDCtl;
    bOLED_Driver.open    = NULL;
    bOLED_Driver.write   = _bOLEDWrite;
    bOLED_Driver.status  = 0;
    bOLED_Driver._hal_if = (void *)&bOLED_HalIf;
    return 0;
}

bDRIVER_REG_INIT(bOLED_Init);

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
