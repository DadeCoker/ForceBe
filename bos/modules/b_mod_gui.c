/**
 *!
 * \file        b_mod_gui.c
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO GUI SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */

/*Includes ----------------------------------------------*/
#include "b_mod_gui.h"
#if _UGUI_ENABLE
#include "b_core.h"
#include "b_driver.h"
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup GUI
 * \{
 */

/**
 * \defgroup GUI_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup GUI_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup GUI_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup GUI_Private_Variables
 * \{
 */

static bGUI_Info_t GUI_Info = {
    .lcd_id   = -1,
    .touch_id = -1,
};

static UG_GUI bGUI_Handle;

#if _GUI_FONT == 3
UG_FONT bGUI_XBF_Font;
#endif
/**
 * \}
 */

/**
 * \defgroup GUI_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup GUI_Private_Functions
 * \{
 */
static void _LCD_SetColorPixel(UG_S16 x, UG_S16 y, UG_COLOR c)
{
    int      fd = -1;
    uint32_t off;
#if (_LCD_DISP_MODE == 0)
    int tmp_y = x;
    x         = _LCD_X_SIZE - 1 - y;
    y         = tmp_y;
#endif
    off = y;
    if (x >= _LCD_X_SIZE || y >= _LCD_Y_SIZE)
    {
        return;
    }
    fd = bOpen(GUI_Info.lcd_id, BCORE_FLAG_W);
    if (fd < 0)
    {
        return;
    }
    bLseek(fd, off * _LCD_X_SIZE + x);
    bWrite(fd, (uint8_t *)&c, sizeof(UG_COLOR));
    bClose(fd);
}

static void _bGUI_TouchExec()
{
    int                   fd = -1;
    bTouchAD_ReadStruct_t bTouchAD_ReadStruct;
#if (_LCD_DISP_MODE == 0)
    uint16_t tmp;
#endif
    if (GUI_Info.lcd_id < 0 || GUI_Info.touch_id < 0)
    {
        return;
    }
    fd = bOpen(GUI_Info.touch_id, BCORE_FLAG_R);
    if (fd < 0)
    {
        return;
    }
    bRead(fd, (uint8_t *)&bTouchAD_ReadStruct, sizeof(bTouchAD_ReadStruct_t));
    bClose(fd);
    if (bTouchAD_ReadStruct.x_ad < _X_TOUCH_AD_MIN || bTouchAD_ReadStruct.x_ad >= _X_TOUCH_AD_MAX ||
        bTouchAD_ReadStruct.y_ad < _Y_TOUCH_AD_MIN || bTouchAD_ReadStruct.y_ad >= _Y_TOUCH_AD_MAX)
    {
        UG_TouchUpdate(_LCD_X_SIZE, _LCD_Y_SIZE, TOUCH_STATE_RELEASED);
    }
    else
    {
        bTouchAD_ReadStruct.x_ad = (bTouchAD_ReadStruct.x_ad - _X_TOUCH_AD_MIN) * _LCD_X_SIZE /
                                   (_X_TOUCH_AD_MAX - _X_TOUCH_AD_MIN);
        bTouchAD_ReadStruct.y_ad = (bTouchAD_ReadStruct.y_ad - _Y_TOUCH_AD_MIN) * _LCD_Y_SIZE /
                                   (_Y_TOUCH_AD_MAX - _Y_TOUCH_AD_MIN);
#if (_LCD_DISP_MODE == 0)
        tmp                      = bTouchAD_ReadStruct.x_ad;
        bTouchAD_ReadStruct.x_ad = bTouchAD_ReadStruct.y_ad;
        bTouchAD_ReadStruct.y_ad = _LCD_X_SIZE - 1 - tmp;
#endif
        UG_TouchUpdate(bTouchAD_ReadStruct.x_ad, bTouchAD_ReadStruct.y_ad, TOUCH_STATE_PRESSED);
        b_log("::%d %d\r\n", bTouchAD_ReadStruct.x_ad, bTouchAD_ReadStruct.y_ad);
    }
}

static void _bGUI_Core()
{
    static uint32_t tick = 0;
    if (bUtilGetTick() - tick > MS2TICKS(10))
    {
        tick = bUtilGetTick();
        _bGUI_TouchExec();
    }
    UG_Update();
}

BOS_REG_POLLING_FUNC(_bGUI_Core);

/**
 * \}
 */

/**
 * \addtogroup GUI_Exported_Functions
 * \{
 */

int bGUI_Init(int lcd, int touch)
{
    if (lcd < 0)
    {
        return -1;
    }
    GUI_Info.lcd_id   = lcd;
    GUI_Info.touch_id = touch;
#if (_LCD_DISP_MODE == 0)
    UG_Init(&bGUI_Handle, _LCD_SetColorPixel, _LCD_Y_SIZE, _LCD_X_SIZE);
#else
    UG_Init(&bGUI_Handle, _LCD_SetColorPixel, _LCD_X_SIZE, _LCD_Y_SIZE);
#endif
    UG_SelectGUI(&bGUI_Handle);
    UG_SetForecolor(C_WHITE);
    UG_SetBackcolor(C_BLACK);
#if _GUI_FONT == 0
    UG_FontSelect(&FONT_6X8);
#elif _GUI_FONT == 1
    UG_FontSelect(&FONT_8X12);
#elif _GUI_FONT == 2
    UG_FontSelect(&FONT_12X16);
#elif _GUI_FONT == 3
    uint8_t xbf_info[18];
    if (0 > UG_ReadXBF(_XBF_FILE_ADDR, xbf_info, 18))
    {
        b_log_e("read err\r\n");
        return -1;
    }
    bGUI_XBF_Font.char_height = xbf_info[4];
    bGUI_XBF_Font.char_width  = xbf_info[4] / 2;
    bGUI_XBF_Font.start_char  = 0x00;
    bGUI_XBF_Font.end_char    = 0xff;
    bGUI_XBF_Font.font_type   = FONT_TYPE_1BPP;
    bGUI_XBF_Font.p           = NULL;
    bGUI_XBF_Font.widths      = NULL;
    UG_FontSelect(&bGUI_XBF_Font);
#endif
    return 0;
}

#if _GUI_FONT == 3

#include "b_device.h"

#if __GNUC__
int __attribute__((weak)) UG_ReadXBF(uint32_t off, uint8_t *pbuf, uint16_t len)
#else
__weak int UG_ReadXBF(uint32_t off, uint8_t *pbuf, uint16_t len)
#endif
{
    int fd = -1;
    fd     = bOpen(SPIFLASH, BCORE_FLAG_RW);
    if (fd < 0)
    {
        b_log_e("open err\r\n");
        return -1;
    }
    bLseek(fd, off);
    bRead(fd, pbuf, len);
    bClose(fd);
    return 0;
}
#endif
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
#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
