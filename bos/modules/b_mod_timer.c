/**
 *!
 * \file        b_mod_timer.c
 * \version     v0.0.1
 * \date        2020/05/16
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
   
/*Includes ----------------------------------------------*/
#include "b_mod_timer.h"  
#include "b_utils.h"
#if _TIMER_ENABLE
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup MODULES
 * \{
 */

/** 
 * \addtogroup TIMER
 * \{
 */

/** 
 * \defgroup TIMER_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup TIMER_Private_Defines
 * \{
 */
  
/**
 * \}
 */
   
/** 
 * \defgroup TIMER_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup TIMER_Private_Variables
 * \{
 */
static bSoftTimerInstance_t *pSoftTimer = NULL;   
/**
 * \}
 */
   
/** 
 * \defgroup TIMER_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup TIMER_Private_Functions
 * \{
 */
static int _bSoftTimerDelete(bSoftTimerInstance_t *pTimerInstance)   
{
    bSoftTimerStruct_t *ptmp = pSoftTimer;
    if(ptmp == NULL)
    {
        return -1;
    }
    if(pSoftTimer == pTimerInstance)
    {
        pSoftTimer = pSoftTimer->next;
        return 0;
    }
    while(ptmp)
    {
        if(ptmp->next == pTimerInstance)
        {
            ptmp->next = pTimerInstance->next;
            return 0;
        }
        ptmp = ptmp->next;
    }
    return -1;
}

static void _bSoftTimerCore()
{
    bSoftTimerStruct_t *ptmp = pSoftTimer;
    while(ptmp)
    {
        if(bUtilGetTick() - ptmp->tick >= MS2TICKS(ptmp->period))
        {
            ptmp->handler();
            if(ptmp->repeat)
            {
                ptmp->tick = bUtilGetTick();
            }
            else
            {
                _bSoftTimerDelete(ptmp);
            }
        }
        ptmp = ptmp->next;
    }
}

BOS_REG_POLLING_FUNC(_bSoftTimerCore);

/**
 * \}
 */
   
/** 
 * \addtogroup TIMER_Exported_Functions
 * \{
 */
 
int bSoftTimerStart(bSoftTimerInstance_t *pTimerInstance, pTimerHandler handler)
{
    if(pTimerInstance == NULL || handler == NULL)
    {
        return -1;
    }
    if(pSoftTimer == NULL)
    {
        pSoftTimer = pTimerInstance;
        pTimerInstance->next = NULL;
    }
    else
    {
        pTimerInstance->next = pSoftTimer->next;
        pSoftTimer->next = pTimerInstance;
    }
    pTimerInstance->handler = handler;
    pTimerInstance->tick = bUtilGetTick();
    return 0;
}



int bSoftTimerStop(bSoftTimerInstance_t *pTimerInstance)
{
    if(pTimerInstance == NULL)
    {
        return -1;
    }
    _bSoftTimerDelete(pTimerInstance);
    return 0;
}


int bSoftTimerReset(bSoftTimerInstance_t *pTimerInstance)
{
    if(pTimerInstance == NULL)
    {
        return -1;
    }
    pTimerInstance->tick = bUtilGetTick();
    return 0;
}

int bSoftTimerSetPeriod(bSoftTimerInstance_t *pTimerInstance, uint32_t ms)
{
    if(pTimerInstance == NULL)
    {
        return -1;
    }
    pTimerInstance->period = ms;
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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/

