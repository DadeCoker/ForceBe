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

#if (defined(_FLEXIBLEBUTTON_ENABLE) && (_FLEXIBLEBUTTON_ENABLE == 1))
#include "core/inc/b_core.h"
#include "core/inc/b_device.h"
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

static bButtonInstance_t *pButtonHead     = NULL;
static uint8_t            bButtonInitFlag = 0;
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
static bButtonInstance_t *_bButtonFindInstance(void *pflex, uint8_t id)
{
    uint32_t           addr_val = 0;
    bDeviceMsg_t       msg;
    bButtonInstance_t *p = pButtonHead;
    addr_val             = (((uint32_t)pflex) - id * sizeof(flex_button_t));
    while (p != NULL)
    {
        bDeviceReadMessage(p->dev_no, &msg);
        if (msg.v == addr_val)
        {
            break;
        }
        p = p->next;
    }
    return p;
}

static uint8_t _bButtonRead(void *p)
{
    int                fd        = -1;
    uint8_t            tmp       = 0;
    flex_button_t     *btn       = (flex_button_t *)p;
    bButtonInstance_t *pinstance = _bButtonFindInstance(p, btn->id);
    if (pinstance == NULL)
    {
        return 0;
    }
    fd = bOpen(pinstance->dev_no, BCORE_FLAG_R);
    if (fd < 0)
    {
        return 0;
    }
    bLseek(fd, btn->id);
    bRead(fd, &tmp, 1);
    bClose(fd);
    return tmp;
}

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
    flex_button_t     *btn       = (flex_button_t *)p;
    bButtonInstance_t *pinstance = _bButtonFindInstance(p, btn->id);
    if (pinstance == NULL)
    {
        return;
    }
    if (pinstance->event & (0x0001 << btn->event))
    {
        if (pinstance->handler)
        {
            pinstance->handler(pinstance->dev_no, btn->id, (0x0001 << btn->event), btn->click_cnt);
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
int bButtonAddKey(bButtonInstance_t *pbutton, flex_button_t *pflex)
{
    bDeviceMsg_t msg;
    if (pbutton == NULL || pflex == NULL)
    {
        return -1;
    }
    if (bButtonInitFlag == 0)
    {
        flex_button_reg_function(_bButtonCallback, _bButtonRead);
        bButtonInitFlag = 1;
    }
    if (pButtonHead == NULL)
    {
        pButtonHead       = pbutton;
        pButtonHead->next = NULL;
    }
    else
    {
        pbutton->next     = pButtonHead->next;
        pButtonHead->next = pbutton;
    }
    msg._p = pflex;
    bDeviceWriteMessage(pbutton->dev_no, &msg);
    pflex->id                  = 0;
    pflex->pressed_logic_level = 1;
    flex_button_register(pflex);
    return 0;
}

int bButtonAddMatrixKeys(bButtonInstance_t *pbutton, flex_button_t *pflex)
{
    bDeviceMsg_t msg;
    int          i = 0;
    if (pbutton == NULL || pflex == NULL)
    {
        return -1;
    }
    if (bButtonInitFlag == 0)
    {
        flex_button_reg_function(_bButtonCallback, _bButtonRead);
        bButtonInitFlag = 1;
    }
    if (pButtonHead == NULL)
    {
        pButtonHead       = pbutton;
        pButtonHead->next = NULL;
    }
    else
    {
        pbutton->next     = pButtonHead->next;
        pButtonHead->next = pbutton;
    }
    msg._p = pflex;
    bDeviceWriteMessage(pbutton->dev_no, &msg);
    for (i = 0; i < (MATRIX_KEYS_ROWS * MATRIX_KEYS_COLUMNS); i++)
    {
        pflex[i].id                  = i;
        pflex[i].pressed_logic_level = 1;
        flex_button_register(&pflex[i]);
    }
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
