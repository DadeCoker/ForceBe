/**
 *!
 * \file        b_mod_state.h
 * \version     v0.0.1
 * \date        2019/06/05
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2019 Bean
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
#ifndef __B_MOD_STATE_H__
#define __B_MOD_STATE_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

#include "b_config.h"

#if _STATE_ENABLE
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup STATE
 * \{
 */

/**
 * \defgroup STATE_Exported_TypesDefinitions
 * \{
 */

typedef void (*pStateEvenHandler_t)(void *arg);
typedef void (*pStateEnterHandler_t)(uint32_t pre_state);
typedef void (*pStateExitHandler_t)(void);
typedef void (*pStateHandler_t)(void);

typedef struct
{
    uint32_t            event;
    pStateEvenHandler_t handler;
} bStateEvent_t;

typedef struct
{
    bStateEvent_t *p_event_table;
    uint32_t       number;
} bStateEventTable_t;

typedef struct
{
    uint32_t             state;
    pStateEnterHandler_t enter;
    pStateExitHandler_t  exit;
    pStateHandler_t      handler;
    bStateEventTable_t   event_table;
} bStateInfo_t;

typedef bStateInfo_t *bStateInstance_t;

/**
 * \}
 */

/**
 * \defgroup STATE_Exported_Defines
 * \{
 */
#define bSTATE_REG_INSTANCE(state_info)                                                     \
    bSECTION_ITEM_REGISTER_FLASH(b_mod_state, bStateInstance_t, CONCAT_2(b_, state_info)) = \
        &state_info
/**
 * \}
 */

/**
 * \defgroup STATE_Exported_Functions
 * \{
 */
int bStateTransfer(uint32_t state);
int bStateInvokeEvent(uint32_t event, void *arg);
int bGetCurrentState(void);
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
