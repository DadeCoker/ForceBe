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
static bGUI_Info_t GUI_Info = {
    .lcd_id = -1,
    .touch_id = -1,
};  

static UG_GUI bGUI_Handle;

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
    int fd = -1;
    uint32_t off;
#if (_LCD_DISP_MODE == 0)
    int tmp_y = x;
    x = _LCD_X_SIZE - 1 - y;
    y = tmp_y;
#endif
    off = y;
    if(x >= _LCD_X_SIZE || y >= _LCD_Y_SIZE)
    {
        return;
    }
    fd = bOpen(GUI_Info.lcd_id, BCORE_FLAG_W);
    if(fd < 0)
    {
        return;
    }
    bLseek(fd, off * _LCD_X_SIZE + x);
    bWrite(fd, (uint8_t *)&c, sizeof(UG_COLOR));
    bClose(fd);
}
/**
 * \}
 */
   
/** 
 * \addtogroup GUI_Exported_Functions
 * \{
 */


int bGUI_Init(int lcd, int touch)
{
    if(lcd < 0)
    {
        return -1;
    }
    GUI_Info.lcd_id = lcd;
    GUI_Info.touch_id = touch;
#if (_LCD_DISP_MODE == 0)
    UG_Init( &bGUI_Handle, _LCD_SetColorPixel, _LCD_Y_SIZE, _LCD_X_SIZE);
#else    
    UG_Init( &bGUI_Handle, _LCD_SetColorPixel, _LCD_X_SIZE, _LCD_Y_SIZE);
#endif
    UG_SelectGUI(&bGUI_Handle);
    UG_SetForecolor(C_WHITE);
    UG_SetBackcolor(C_BLACK);
    UG_FontSelect(&FONT_8X12);
    return 0;
}


void bGUI_TouchExec()
{
    int fd = -1;
    uint16_t xy[2];
#if (_LCD_DISP_MODE == 0)
     uint16_t tmp;
#endif       
    if(GUI_Info.lcd_id < 0 || GUI_Info.touch_id < 0)
    {
        return;
    }
    fd = bOpen(GUI_Info.touch_id, BCORE_FLAG_R);
    if(fd < 0)
    {
        return;
    }
    bRead(fd, (uint8_t *)xy, sizeof(xy));
    bClose(fd);
    if(xy[0] < _X_TOUCH_AD_MIN || xy[0] >= _X_TOUCH_AD_MAX
        || xy[1] < _Y_TOUCH_AD_MIN || xy[1] >= _Y_TOUCH_AD_MAX)
    {
        UG_TouchUpdate( _LCD_X_SIZE, _LCD_Y_SIZE, TOUCH_STATE_RELEASED);
    }
    else
    {
        xy[0] = (xy[0] - _X_TOUCH_AD_MIN) * _LCD_X_SIZE / (_X_TOUCH_AD_MAX - _X_TOUCH_AD_MIN);
        xy[1] = (xy[1] - _Y_TOUCH_AD_MIN) * _LCD_Y_SIZE / (_Y_TOUCH_AD_MAX - _Y_TOUCH_AD_MIN);
#if (_LCD_DISP_MODE == 0)
        tmp = xy[0];
        xy[0] = xy[1];
        xy[1] = _LCD_X_SIZE - 1 - tmp;
#endif        
        UG_TouchUpdate( xy[0], xy[1], TOUCH_STATE_PRESSED);
    }
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

