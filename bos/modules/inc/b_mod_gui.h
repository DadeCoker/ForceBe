/**
 *!
 * \file        b_mod_gui.h
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
#ifndef __B_MOD_GUI_H__
#define __B_MOD_GUI_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

#include "b_config.h"

#if _UGUI_ENABLE
#include "thirdparty/ugui/ugui.h"
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
 * \defgroup GUI_Exported_Typedefines
 * \{
 */

typedef struct bGUIStruct
{
    const uint32_t     lcd_dev_no;
    const uint32_t     touch_dev_no;
    const int          touch_type;
    const uint16_t     lcd_x_size;
    const uint16_t     lcd_y_size;
    uint8_t            lcd_disp_dir;
    uint16_t           touch_ad_x[2];
    uint16_t           touch_ad_y[2];
    UG_GUI             gui_handle;
    struct bGUIStruct *pnext;
} bGUIStruct_t;

typedef bGUIStruct_t bGUIInstance_t;

/**
 * \}
 */

/**
 * \defgroup GUI_Exported_Defines
 * \{
 */
#define TOUCH_TYPE_RES (0)
#define TOUCH_TYPE_CAP (1)

#define LCD_DISP_H (0)
#define LCD_DISP_V (1)

#define bGUI_ADD_DEVICE(_lcd_dev_no, _touch_dev_no, _x_size, _y_size, _touch_type) \
    static bGUIInstance_t gui_##_lcd_dev_no = {                                    \
        .lcd_dev_no   = _lcd_dev_no,                                               \
        .touch_dev_no = _touch_dev_no,                                             \
        .touch_type   = _touch_type,                                               \
        .lcd_x_size   = _x_size,                                                   \
        .lcd_y_size   = _y_size,                                                   \
        .pnext        = NULL,                                                      \
    };                                                                             \
    bGUIRegist(&gui_##_lcd_dev_no);

/**
 * \}
 */

/**
 * \defgroup GUI_Exported_Functions
 * \{
 */
// 建议不要直接调用此接口，使用 bGUI_ADD_DEVICE 代替
int bGUIRegist(bGUIInstance_t *pInstance);

// 选择当前操作的目标
int bGUISelect(uint32_t lcd_dev_no);

// 设置电阻屏触摸的AD值范围
int bGUITouchRange(uint32_t lcd_dev_no, uint16_t x_ad_min, uint16_t x_ad_max, uint16_t y_ad_min,
                   uint16_t y_ad_max);

// 设置屏幕显示方向，默认是LCD_DISP_V
int bGUIDispDir(uint32_t lcd_dev_no, uint8_t dir);

// 如果将汉字字库存放在FLASH，通过此接口传入dev_no
int bGUISetFontDevice(uint32_t dev_no);

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

#ifdef __cplusplus
}
#endif

#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
