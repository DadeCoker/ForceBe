/**
 *!
 * \file        b_mod_button.h
 * \version     v0.0.1
 * \date        2020/02/21
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO BUTTON SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
#ifndef __B_MOD_BUTTON_H__
#define __B_MOD_BUTTON_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"
#if _FLEXIBLEBUTTON_ENABLE
#include "thirdparty/FlexibleButton/flexible_button.h"
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup BUTTON
 * \{
 */

/**
 * \defgroup BUTTON_Exported_TypesDefinitions
 * \{
 */

typedef void (*pBtnEventHandler_t)(uint16_t event, uint8_t param);

/**
 * \}
 */

/**
 * \defgroup BUTTON_Exported_Defines
 * \{
 */

#define BTN_EVENT_DOWN (0x001)
#define BTN_EVENT_CLICK (0x002)
#define BTN_EVENT_DOUBLE_CLICK (0x004)
#define BTN_EVENT_REPEAT_CLICK (0x008)
#define BTN_EVENT_SHORT (0x010)
#define BTN_EVENT_SHORT_UP (0x020)
#define BTN_EVENT_LONG (0x040)
#define BTN_EVENT_LONG_UP (0x080)
#define BTN_EVENT_LONGLONG (0x100)
#define BTN_EVENT_LONGLONG_UP (0x200)

/**
 * \}
 */

/**
 * \defgroup BUTTON_Exported_Functions
 * \{
 */
int  bButtonInit(uint16_t short_xms, uint16_t long_xms, uint16_t llong_xms);
void bButtonRegEvent(uint8_t id, uint16_t event, pBtnEventHandler_t handler);
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
