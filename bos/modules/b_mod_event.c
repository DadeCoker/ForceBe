/**
 *!
 * \file        b_mod_event.c
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
#include "modules/inc/b_mod_event.h"

#include "core/inc/b_section.h"

#if _EVENT_MANAGE_ENABLE
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup EVENT
 * \{
 */

/**
 * \defgroup EVENT_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup EVENT_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup EVENT_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup EVENT_Private_Variables
 * \{
 */
static bEventInstance_t *pEventInstanceHead = NULL;
/**
 * \}
 */

/**
 * \defgroup EVENT_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup EVENT_Private_Functions
 * \{
 */

static void _bEventCore()
{
    bEventInfo_t *ptmp = pEventInstanceHead;
    while (ptmp)
    {
        if (ptmp->trigger)
        {
            ptmp->trigger = 0;
            ptmp->phandler();
        }
        ptmp = ptmp->pnext;
    }
}

BOS_REG_POLLING_FUNC(_bEventCore);

/**
 * \}
 */

/**
 * \addtogroup EVENT_Exported_Functions
 * \{
 */

/**
 * \brief Regist event instance
 * \param pInstance Pointer to the event instance
 * \param handler The Callback function
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bEventRegist(bEventInstance_t *pInstance, pEventHandler_t handler)
{
    if (handler == NULL || pInstance == NULL)
    {
        return -1;
    }
    pInstance->pnext = NULL;
    if (pEventInstanceHead == NULL)
    {
        pEventInstanceHead = pInstance;
    }
    else
    {
        pInstance->pnext          = pEventInstanceHead->pnext;
        pEventInstanceHead->pnext = pInstance;
    }
    pInstance->trigger  = 0;
    pInstance->phandler = handler;
    return 0;
}

/**
 * \brief Call this function after the event is generated
 * \param pInstance Pointer to the event instance
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bEventTrigger(bEventInstance_t *pInstance)
{
    if (pInstance == NULL)
    {
        return -1;
    }
    pInstance->trigger = 1;
    return 0;
}

int bEventIsIdle()
{
    bEventInfo_t *ptmp = pEventInstanceHead;
    while (ptmp)
    {
        if (ptmp->trigger)
        {
            return -1;
        }
        ptmp = ptmp->pnext;
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
