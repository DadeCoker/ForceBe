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
#include "modules/inc/b_mod_button.h"

#include <string.h>

#include "b_section.h"


#if _FLEXIBLEBUTTON_ENABLE
#include "hal/inc/b_hal.h"
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

#if _MATRIXKEY_ENABLE
typedef struct
{
    bHalGPIOInstance_t Rows[MATRIXKEY_ROWS];
    bHalGPIOInstance_t Columns[MATRIXKEY_COLUMNS];
}bMatrixKeyInfo_t;
#endif


typedef struct
{
    uint16_t           event;
    pBtnEventHandler_t handler;
} bButtonEventInfo_t;

/**
 * \}
 */

/**
 * \defgroup BUTTON_Private_Defines
 * \{
 */
#if _MATRIXKEY_ENABLE

#if (MATRIXKEY_ROWS == 0 || MATRIXKEY_COLUMNS == 0)
#error ROWS and COLUMNS Must be greater than zero
#endif

#define BUTTON_NUM (FLEX_BTN_NUMBER + ((MATRIXKEY_ROWS) * (MATRIXKEY_COLUMNS)))
#else

#if (FLEX_BTN_NUMBER == 0)
#error FLEX_BTN_NUMBER Must be greater than zero
#endif

#define BUTTON_NUM (FLEX_BTN_NUMBER)
#endif
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
static flex_button_t       bButtonList[BUTTON_NUM];
static bButtonEventInfo_t  bButtonEventInfo[BUTTON_NUM];

#if (FLEX_BTN_NUMBER > 0)
const static bButtonInfo_t bButtonInfo[FLEX_BTN_NUMBER] = HAL_B_BUTTON_GPIO;
#endif

#if _MATRIXKEY_ENABLE
const static bMatrixKeyInfo_t bMatrixKeyInfo = HAL_B_MATRIXKEY_GPIO;
#endif
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
#if (FLEX_BTN_NUMBER > 0) 
static uint8_t _bButtonRead(void *p)
{ 
    flex_button_t *btn = (flex_button_t *)p;
    return bHalGpioReadPin(bButtonInfo[btn->id].port, bButtonInfo[btn->id].pin);
}
#endif

#if _MATRIXKEY_ENABLE
static uint8_t _bMatrixKeyRead(void *p)
{
    uint8_t tmp = 1;
    flex_button_t *btn = (flex_button_t *)p;
    uint8_t id = btn->id - FLEX_BTN_NUMBER;
    uint8_t row = 0, col = 0;
    row = id / MATRIXKEY_COLUMNS;
    col = id % MATRIXKEY_COLUMNS;
    bHalGpioConfig(bMatrixKeyInfo.Rows[row].port, bMatrixKeyInfo.Rows[row].pin, B_HAL_GPIO_OUTPUT, B_HAL_GPIO_NOPULL);
    bHalGpioWritePin(bMatrixKeyInfo.Rows[row].port, bMatrixKeyInfo.Rows[row].pin, 0);
    tmp = bHalGpioReadPin(bMatrixKeyInfo.Columns[col].port, bMatrixKeyInfo.Columns[col].pin);
    bHalGpioConfig(bMatrixKeyInfo.Rows[row].port, bMatrixKeyInfo.Rows[row].pin, B_HAL_GPIO_INPUT, B_HAL_GPIO_PULLUP);
    return tmp;
}
#endif

static void _bButtonCore()
{
    static uint32_t tick = 0;
    if (bHalGetSysTick() - tick > MS2TICKS(1000 / FLEX_BTN_SCAN_FREQ_HZ))
    {
        tick = bHalGetSysTick();
        flex_button_scan();
    }
}

BOS_REG_POLLING_FUNC(_bButtonCore);

static void _bButtonCallback(void *p)
{
    flex_button_t *btn = (flex_button_t *)p;
    if (btn->id >= BUTTON_NUM)
    {
        return;
    }
    if (bButtonEventInfo[btn->id].event & (0x0001 << btn->event))
    {
        if (bButtonEventInfo[btn->id].handler)
        {
            bButtonEventInfo[btn->id].handler(btn->id, (0x0001 << btn->event), btn->click_cnt);
        }
    }
}

/**
 * \}
 */

/**
 * \addtogroup BUTTON_Exported_Functions
 * \{
 */

int bButtonInit(uint16_t short_xms, uint16_t long_xms, uint16_t llong_xms)
{
    int i;
#if (FLEX_BTN_NUMBER > 0)     
    for (i = 0; i < FLEX_BTN_NUMBER; i++)
    {
        bButtonList[i].id                     = i;
        bButtonList[i].pressed_logic_level    = bButtonInfo[i].logic_level;
        bButtonList[i].usr_button_read        = _bButtonRead;
        bButtonList[i].cb                     = _bButtonCallback;
        bButtonList[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(short_xms);
        bButtonList[i].long_press_start_tick  = FLEX_MS_TO_SCAN_CNT(long_xms);
        bButtonList[i].long_hold_start_tick   = FLEX_MS_TO_SCAN_CNT(llong_xms);
        flex_button_register(&bButtonList[i]);
        bButtonEventInfo[i].handler = NULL;
        bButtonEventInfo[i].event   = 0;
    }
#endif    
#if _MATRIXKEY_ENABLE  
    for(;i < BUTTON_NUM;i++)
    {
        bButtonList[i].id                     = i;
        bButtonList[i].pressed_logic_level    = 0;
        bButtonList[i].usr_button_read        = _bMatrixKeyRead;
        bButtonList[i].cb                     = _bButtonCallback;
        bButtonList[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(short_xms);
        bButtonList[i].long_press_start_tick  = FLEX_MS_TO_SCAN_CNT(long_xms);
        bButtonList[i].long_hold_start_tick   = FLEX_MS_TO_SCAN_CNT(llong_xms);
        flex_button_register(&bButtonList[i]);
        bButtonEventInfo[i].handler = NULL;
        bButtonEventInfo[i].event   = 0;
    }
#endif    
    return 0;
}

void bButtonRegEvent(uint8_t id, uint16_t event, pBtnEventHandler_t handler)
{
    if (id >= BUTTON_NUM || handler == NULL)
    {
        return;
    }
    bButtonEventInfo[id].event   = event;
    bButtonEventInfo[id].handler = handler;
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
