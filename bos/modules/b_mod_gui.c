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

static bGUIInstance_t *pGUIHead    = NULL;
static bGUIInstance_t *pGUICurrent = NULL;

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

static bGUIInstance_t *_GUIId2Instace(uint8_t id)
{
    bGUIInstance_t *p = pGUIHead;
    while (p != NULL)
    {
        if (p->gui_id == id)
        {
            return p;
        }
        p = p->pnext;
    }
    return NULL;
}

static void _LCD_SetColorPixel(UG_S16 x, UG_S16 y, UG_COLOR c)
{
    int      fd    = -1;
    int      tmp_y = x;
    uint32_t off;
    if (pGUICurrent == NULL)
    {
        return;
    }
    if (pGUICurrent->lcd_disp_dir == LCD_DISP_H)
    {
        x = pGUICurrent->lcd_x_size - 1 - y;
        y = tmp_y;
    }
    off = y;
    if (x >= pGUICurrent->lcd_x_size || y >= pGUICurrent->lcd_y_size)
    {
        return;
    }
    fd = bOpen(pGUICurrent->lcd_dev_no, BCORE_FLAG_W);
    if (fd < 0)
    {
        return;
    }
    bLseek(fd, off * pGUICurrent->lcd_x_size + x);
    bWrite(fd, (uint8_t *)&c, sizeof(UG_COLOR));
    bClose(fd);
}

static void _LCD_FillRect(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c)
{
    int            fd     = -1;
    int            retval = 0;
    bLcdRectInfo_t info;
    if (pGUICurrent == NULL)
    {
        return;
    }
    fd = bOpen(pGUICurrent->lcd_dev_no, BCORE_FLAG_W);
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
    uint16_t      tmp;
    bTouchAdVal_t ad_val;
    if (pGUICurrent == NULL)
    {
        return;
    }
    if (pGUICurrent->touch_dev_no == 0)
    {
        return;
    }
    fd = bOpen(pGUICurrent->touch_dev_no, BCORE_FLAG_R);
    if (fd < 0)
    {
        return;
    }
    if (pGUICurrent->touch_type == TOUCH_TYPE_RES)
    {
        bRead(fd, (uint8_t *)&ad_val, sizeof(bTouchAdVal_t));
    }
    bClose(fd);

    if (pGUICurrent->touch_type == TOUCH_TYPE_RES)
    {
        if (ad_val.x_ad < pGUICurrent->touch_ad_x[0] || ad_val.x_ad >= pGUICurrent->touch_ad_x[1] ||
            ad_val.y_ad < pGUICurrent->touch_ad_y[0] || ad_val.y_ad >= pGUICurrent->touch_ad_y[1])
        {
            UG_TouchUpdate(pGUICurrent->lcd_x_size, pGUICurrent->lcd_y_size, TOUCH_STATE_RELEASED);
        }
        else
        {
            ad_val.x_ad = (ad_val.x_ad - pGUICurrent->touch_ad_x[0]) * pGUICurrent->lcd_x_size /
                          (pGUICurrent->touch_ad_x[1] - pGUICurrent->touch_ad_x[0]);
            ad_val.y_ad = (ad_val.y_ad - pGUICurrent->touch_ad_y[0]) * pGUICurrent->lcd_y_size /
                          (pGUICurrent->touch_ad_y[1] - pGUICurrent->touch_ad_y[0]);
            if (pGUICurrent->lcd_disp_dir == LCD_DISP_H)
            {
                tmp         = ad_val.x_ad;
                ad_val.x_ad = ad_val.y_ad;
                ad_val.y_ad = ad_val.x_ad - 1 - tmp;
            }
            UG_TouchUpdate(ad_val.x_ad, ad_val.y_ad, TOUCH_STATE_PRESSED);
        }
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

int bGUIRegist(bGUIInstance_t *pInstance)
{
    if (pInstance == NULL)
    {
        return -1;
    }
    if (pInstance->touch_type != TOUCH_TYPE_RES && pInstance->touch_type != TOUCH_TYPE_CAP)
    {
        return -2;
    }
    if (pGUIHead == NULL)
    {
        pGUIHead         = pInstance;
        pGUIHead->gui_id = 0;
        pGUIHead->pnext  = NULL;
    }
    else
    {
        pInstance->pnext = pGUIHead->pnext;
        pGUIHead->pnext  = pInstance;
        if (pInstance->pnext == NULL)
        {
            pInstance->gui_id = 1;
        }
        else
        {
            pInstance->gui_id = pInstance->pnext->gui_id + 1;
        }
    }
    pInstance->lcd_disp_dir            = LCD_DISP_V;
    pInstance->gui_handle.char_h_space = 0;
    pInstance->gui_handle.char_v_space = 0;
    UG_Init(&pInstance->gui_handle, _LCD_SetColorPixel, pInstance->lcd_x_size,
            pInstance->lcd_y_size);
    UG_SelectGUI(&pInstance->gui_handle);
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
    pGUICurrent = pInstance;
    return pInstance->gui_id;
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

int bGUISelect(uint8_t id)
{
    bGUIInstance_t *p = _GUIId2Instace(id);
    if (p == NULL)
    {
        return -1;
    }
    pGUICurrent = p;
    UG_SelectGUI(&p->gui_handle);
    return 0;
}

int bGUITouchRange(uint8_t id, uint16_t x_ad_min, uint16_t x_ad_max, uint16_t y_ad_min,
                   uint16_t y_ad_max)
{

    bGUIInstance_t *p = _GUIId2Instace(id);
    if (p == NULL)
    {
        return -1;
    }
    if (p->touch_type == TOUCH_TYPE_RES)
    {
        p->touch_ad_x[0] = x_ad_min;
        p->touch_ad_x[1] = x_ad_max;
        p->touch_ad_y[0] = y_ad_min;
        p->touch_ad_y[1] = y_ad_max;
    }
    else
    {
        return -1;
    }
    return 0;
}

int bGUIDispDir(uint8_t id, uint8_t dir)
{
    bGUIInstance_t *p = _GUIId2Instace(id);
    if (p == NULL || (dir != LCD_DISP_H && dir != LCD_DISP_V))
    {
        return -1;
    }
    p->lcd_disp_dir = dir;
    return 0;
}

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
