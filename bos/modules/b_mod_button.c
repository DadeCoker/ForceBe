/**
 *!
 * \file        b_mod_button.c
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

/*Includes ----------------------------------------------*/
#include "b_mod_button.h"

#include <string.h>
#if _FLEXIBLEBUTTON_ENABLE
#include "b_hal.h"
#include "b_utils.h"
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
 * \defgroup BUTTON_Private_TypesDefinitions
 * \{
 */
typedef struct
{
    bHalGPIOPort_t port;
    bHalGPIOPin_t  pin;
    uint8_t        logic_level;
} bButtonInfo_t;
/**
 * \}
 */

/**
 * \defgroup BUTTON_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup BUTTON_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup BUTTON_Private_Variables
 * \{
 */
static flex_button_t       bButtonList[FLEX_BTN_NUMBER];
const static bButtonInfo_t bButtonInfo[FLEX_BTN_NUMBER] = HAL_B_BUTTON_GPIO;
/**
 * \}
 */

/**
 * \defgroup BUTTON_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup BUTTON_Private_Functions
 * \{
 */

static uint8_t _bButtonRead(void *p)
{
    flex_button_t *btn = (flex_button_t *)p;
    return bHalGPIO_ReadPin(bButtonInfo[btn->id].port, bButtonInfo[btn->id].pin);
}

static void _bButtonCore()
{
    static uint32_t tick = 0;
    if (bUtilGetTick() - tick > MS2TICKS(1000 / FLEX_BTN_SCAN_FREQ_HZ))
    {
        tick = bUtilGetTick();
        flex_button_scan();
    }
}

BOS_REG_POLLING_FUNC(_bButtonCore);

/**
 * \}
 */

/**
 * \addtogroup BUTTON_Exported_Functions
 * \{
 */

int bButtonInit()
{
    int i;
    for (i = 0; i < FLEX_BTN_NUMBER; i++)
    {
        bButtonList[i].id                     = i;
        bButtonList[i].pressed_logic_level    = bButtonInfo[i].logic_level;
        bButtonList[i].usr_button_read        = _bButtonRead;
        bButtonList[i].cb                     = bButtonCallback;
        bButtonList[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(1500);
        bButtonList[i].long_press_start_tick  = FLEX_MS_TO_SCAN_CNT(3000);
        bButtonList[i].long_hold_start_tick   = FLEX_MS_TO_SCAN_CNT(4500);
        flex_button_register(&bButtonList[i]);
    }
    return 0;
}

#if __GNUC__
void __attribute__((weak)) bButtonCallback(void *p)
#else
__weak void bButtonCallback(void *p)
#endif
{
    flex_button_t *btn = (flex_button_t *)p;
    b_log("id: [%d]  event: [%d]  repeat: %d\n", btn->id, btn->event, btn->click_cnt);
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
