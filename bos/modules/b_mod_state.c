/**
 *!
 * \file        b_mod_state.c
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

/*Includes ----------------------------------------------*/
#include "modules/inc/b_mod_state.h"

#include "b_section.h"
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
 * \defgroup STATE_Private_Defines
 * \{
 */
#define STATE_SAFE_INVOKE(f) \
    do                       \
    {                        \
        if (f != NULL)       \
        {                    \
            f();             \
        }                    \
    } while (0)

#define STATE_SAFE_INVOKE_ARG(f, arg) \
    do                                \
    {                                 \
        if (f != NULL)                \
        {                             \
            f(arg);                   \
        }                             \
    } while (0)
/**
 * \}
 */

/**
 * \defgroup STATE_Private_Variables
 * \{
 */
static bStateInfo_t *pbStateInfo = NULL;

bSECTION_DEF_FLASH(b_mod_state, bStateInstance_t);
/**
 * \}
 */

/**
 * \defgroup STATE_Private_Functions
 * \{
 */
static void _bStatePolling()
{
    if (pbStateInfo == NULL)
    {
        return;
    }
    STATE_SAFE_INVOKE(pbStateInfo->handler);
}

BOS_REG_POLLING_FUNC(_bStatePolling);

/**
 * \}
 */

/**
 * \addtogroup STATE_Exported_Functions
 * \{
 */

int bStateTransfer(int state)
{
    if (pbStateInfo != NULL)
    {
        if (pbStateInfo->state == state)
        {
            return 0;
        }
    }
    bSECTION_FOR_EACH(b_mod_state, bStateInstance_t, ptmp)
    {
        if (ptmp == NULL)
        {
            continue;
        }

        if (ptmp->state == state)
        {
            if (pbStateInfo != NULL)
            {
                STATE_SAFE_INVOKE(pbStateInfo->exit);
                STATE_SAFE_INVOKE_ARG(ptmp->enter, pbStateInfo->state);
            }
            else
            {
                STATE_SAFE_INVOKE_ARG(ptmp->enter, ptmp->state);
            }
            pbStateInfo = ptmp;
            return 0;
        }
    }
    return -1;
}

int bStateInvokeEvent(int event, void *arg)
{
    int i = 0;
    if (pbStateInfo == NULL)
    {
        return -1;
    }
    if (pbStateInfo->event_table.p_event_table == NULL || pbStateInfo->event_table.number == 0)
    {
        return -2;
    }
    for (i = 0; i < pbStateInfo->event_table.number; i++)
    {
        if (pbStateInfo->event_table.p_event_table[i].event == event)
        {
            STATE_SAFE_INVOKE_ARG(pbStateInfo->event_table.p_event_table[i].handler, arg);
            break;
        }
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
