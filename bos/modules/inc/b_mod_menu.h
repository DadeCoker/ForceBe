/**
 *!
 * \file        b_mod_menu.h
 * \version     v0.0.1
 * \date        2020/03/04
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
#ifndef __B_MOD_MENU_H__
#define __B_MOD_MENU_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"
#if _MENU_ENABLE
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup BOS_MENU
 * \{
 */

/**
 * \defgroup MENU_Exported_TypesDefinitions
 * \{
 */

typedef void (*pCreateUI)(uint32_t pre_id);

typedef struct bMenuItem
{
    uint32_t          id;
    struct bMenuItem *prev;
    struct bMenuItem *next;
    struct bMenuItem *parent;
    struct bMenuItem *child;
    pCreateUI         create_ui;
    uint8_t           visible;
} bMenuItem_t;

/**
 * \}
 */

/**
 * \defgroup MENU_Exported_Defines
 * \{
 */
#define MENU_UP 1
#define MENU_DOWN 2
#define MENU_BACK 3
#define MENU_ENTER 4
/**
 * \}
 */

/**
 * \defgroup MENU_Exported_Functions
 * \{
 */
int      bMenuAddSibling(uint32_t ref_id, uint32_t id, pCreateUI f);
int      bMenuAddChild(uint32_t ref_id, uint32_t id, pCreateUI f);
void     bMenuAction(uint8_t cmd);
void     bMenuJump(uint32_t id);
uint32_t bMenuCurrentID(void);
int      bMenuSetVisible(uint32_t id, uint8_t s);
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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
