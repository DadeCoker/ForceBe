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
#include "modules/inc/b_mod_gui.h"
#if _UGUI_ENABLE
#include "core/inc/b_core.h"
#include "drivers/inc/b_driver.h"
#include "utils/inc/b_util_log.h"
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
typedef struct
{
    int lcd_id;
    int touch_id;
} bGUI_Info_t;
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

#if GUI_FONT == 3
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
#if (LCD_DISP_MODE == 0)
    int tmp_y = x;
    x         = LCD_X_SIZE - 1 - y;
    y         = tmp_y;
#endif
    off = y;
    if (x >= LCD_X_SIZE || y >= LCD_Y_SIZE)
    {
        return;
    }
    fd = bOpen(GUI_Info.lcd_id, BCORE_FLAG_W);
    if (fd < 0)
    {
        return;
    }
    bLseek(fd, off * LCD_X_SIZE + x);
    bWrite(fd, (uint8_t *)&c, sizeof(UG_COLOR));
    bClose(fd);
}

static void _LCD_FillRect(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c)
{
    int            fd     = -1;
    int            retval = 0;
    bLcdRectInfo_t info;
    fd = bOpen(GUI_Info.lcd_id, BCORE_FLAG_W);
    if (fd < 0)
    {
        return;
    }
    info.x1    = (x1 > x2) ? x2 : x1;
    info.x2    = (x1 > x2) ? x1 : x2;
    info.y1    = (y1 > y2) ? y2 : y1;
    info.y2    = (y1 > y2) ? y1 : y2;
    info.color = c;
    retval     = bCtl(fd, bCMD_FILL_RECT, &info);
    bClose(fd);
    if (retval >= 0)
    {
        return;
    }
    for (; info.x1 <= info.x2; info.x1++)
    {
        for (; info.y1 <= info.y2; info.y1++)
        {
            _LCD_SetColorPixel(info.x1, info.y1, c);
        }
    }
}

static void _bGUI_TouchExec()
{
    int           fd = -1;
    bTouchAdVal_t AdVal;
#if (LCD_DISP_MODE == 0)
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
    bRead(fd, (uint8_t *)&AdVal, sizeof(bTouchAdVal_t));
    bClose(fd);
    if (AdVal.x_ad < X_TOUCH_AD_MIN || AdVal.x_ad >= X_TOUCH_AD_MAX ||
        AdVal.y_ad < Y_TOUCH_AD_MIN || AdVal.y_ad >= Y_TOUCH_AD_MAX)
    {
        UG_TouchUpdate(LCD_X_SIZE, LCD_Y_SIZE, TOUCH_STATE_RELEASED);
    }
    else
    {
        AdVal.x_ad = (AdVal.x_ad - X_TOUCH_AD_MIN) * LCD_X_SIZE / (X_TOUCH_AD_MAX - X_TOUCH_AD_MIN);
        AdVal.y_ad = (AdVal.y_ad - Y_TOUCH_AD_MIN) * LCD_Y_SIZE / (Y_TOUCH_AD_MAX - Y_TOUCH_AD_MIN);
#if (LCD_DISP_MODE == 0)
        tmp        = AdVal.x_ad;
        AdVal.x_ad = AdVal.y_ad;
        AdVal.y_ad = LCD_X_SIZE - 1 - tmp;
#endif
        UG_TouchUpdate(AdVal.x_ad, AdVal.y_ad, TOUCH_STATE_PRESSED);
    }
}

static void _bGUI_Core()
{
    static uint32_t tick = 0;
    if (bHalGetSysTick() - tick > MS2TICKS(10))
    {
        tick = bHalGetSysTick();
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
    GUI_Info.lcd_id          = lcd;
    GUI_Info.touch_id        = touch;
    bGUI_Handle.char_h_space = 0;
    bGUI_Handle.char_v_space = 0;
#if (LCD_DISP_MODE == 0)
    UG_Init(&bGUI_Handle, _LCD_SetColorPixel, LCD_Y_SIZE, LCD_X_SIZE);
#else
    UG_Init(&bGUI_Handle, _LCD_SetColorPixel, LCD_X_SIZE, LCD_Y_SIZE);
#endif
    UG_SelectGUI(&bGUI_Handle);
    UG_SetForecolor(C_WHITE);
    UG_SetBackcolor(C_BLACK);
    UG_DriverRegister(DRIVER_FILL_FRAME, _LCD_FillRect);
#if GUI_FONT == 0
    UG_FontSelect(&FONT_6X8);
#elif GUI_FONT == 1
    UG_FontSelect(&FONT_8X12);
#elif GUI_FONT == 2
    UG_FontSelect(&FONT_12X16);
#elif GUI_FONT == 3
    uint8_t xbf_info[18];
    if (0 > UG_ReadXBF(XBF_FILE_ADDR, xbf_info, 18))
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

#if GUI_FONT == 3

#include "b_device.h"

__WEAKDEF int UG_ReadXBF(uint32_t off, uint8_t *pbuf, uint16_t len)
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
