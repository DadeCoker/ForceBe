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
#if (defined(_FLEXIBLEBUTTON_ENABLE) && (_FLEXIBLEBUTTON_ENABLE == 1))
#include "thirdparty/flexiblebutton/flexible_button.h"
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

typedef void (*pBtnEventHandler_t)(uint32_t dev_no, uint8_t sub_id, uint16_t event, uint8_t param);

typedef struct bButtonInstance
{
    uint32_t                dev_no;
    uint16_t                event;
    pBtnEventHandler_t      handler;
    struct bButtonInstance *next;
} bButtonInstance_t;

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

#define bBUTTON_ADD_KEY(dev, e, e_handler)           \
    static flex_button_t     CONCAT_2(button_, dev); \
    static bButtonInstance_t CONCAT_2(key_, dev) = { \
        .dev_no  = dev,                              \
        .event   = e,                                \
        .handler = e_handler,                        \
        .next    = NULL,                             \
    };                                               \
    bButtonAddKey(&CONCAT_2(key_, dev), &CONCAT_2(button_, dev))

#define bBUTTON_ADD_MATRIXKEYS(dev, e, e_handler)                                            \
    static flex_button_t     CONCAT_2(button_, dev)[MATRIX_KEYS_ROWS * MATRIX_KEYS_COLUMNS]; \
    static bButtonInstance_t CONCAT_2(key_, dev) = {                                         \
        .dev_no  = dev,                                                                      \
        .event   = e,                                                                        \
        .handler = e_handler,                                                                \
        .next    = NULL,                                                                     \
    };                                                                                       \
    bButtonAddMatrixKeys(&CONCAT_2(key_, dev), CONCAT_2(button_, dev))

/**
 * \}
 */

/**
 * \defgroup BUTTON_Exported_Functions
 * \{
 */
// 请不要直接调用下面这两个函数。使用bBUTTON_ADD_KEY和bBUTTON_ADD_MATRIXKEYS代替
int bButtonAddKey(bButtonInstance_t *pbutton, flex_button_t *pflex);
int bButtonAddMatrixKeys(bButtonInstance_t *pbutton, flex_button_t *pflex);
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
