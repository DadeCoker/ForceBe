/**
 *!
 * \file        b_mod_pwm.c
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
#include "modules/inc/b_mod_pwm.h"

#include "b_section.h"
#include "hal/inc/b_hal.h"

#if _PWM_ENABLE
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup PWM
 * \{
 */

/**
 * \defgroup PWM_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup PWM_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup PWM_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup PWM_Private_Variables
 * \{
 */
static bSoftPwmInstance_t *pSoftPwm = NULL;
/**
 * \}
 */

/**
 * \defgroup PWM_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup PWM_Private_Functions
 * \{
 */
static int _bSoftPwmDelete(bSoftPwmInstance_t *pPwmInstance)
{
    bSoftPwmStruct_t *ptmp = pSoftPwm;
    if (ptmp == NULL)
    {
        return -1;
    }
    if (pSoftPwm == pPwmInstance)
    {
        pSoftPwm = pSoftPwm->next;
        return 0;
    }
    while (ptmp)
    {
        if (ptmp->next == pPwmInstance)
        {
            ptmp->next = pPwmInstance->next;
            return 0;
        }
        ptmp = ptmp->next;
    }
    return -1;
}

static void _bSoftPwmCore()
{
    bSoftPwmStruct_t *ptmp = pSoftPwm;
    while (ptmp)
    {
        if (ptmp->flag == 0)
        {
            if (bHalGetSysTick() - ptmp->tick >= MS2TICKS(ptmp->ccr))
            {
                ptmp->handler(PWM_HANDLER_CCR);
                ptmp->flag = 1;
            }
        }

        if (bHalGetSysTick() - ptmp->tick >= MS2TICKS(ptmp->period))
        {
            ptmp->handler(PWM_HANDLER_PERIOD);
            ptmp->flag = 0;
            if (ptmp->repeat)
            {
                if (ptmp->repeat > 1)
                {
                    ptmp->repeat -= 1;
                    ptmp->tick = bHalGetSysTick();
                }
                else
                {
                    _bSoftPwmDelete(ptmp);
                }
            }
            else
            {
                ptmp->tick = bHalGetSysTick();
            }
        }
        ptmp = ptmp->next;
    }
}

BOS_REG_POLLING_FUNC(_bSoftPwmCore);

/**
 * \}
 */

/**
 * \addtogroup PWM_Exported_Functions
 * \{
 */

int bSoftPwmStart(bSoftPwmInstance_t *pPwmInstance, pPwmHandler handler)
{
    if (pPwmInstance == NULL || handler == NULL)
    {
        return -1;
    }
    if (pSoftPwm == NULL)
    {
        pSoftPwm           = pPwmInstance;
        pPwmInstance->next = NULL;
    }
    else
    {
        pPwmInstance->next = pSoftPwm->next;
        pSoftPwm->next     = pPwmInstance;
    }
    pPwmInstance->handler = handler;
    pPwmInstance->tick    = bHalGetSysTick();
    pPwmInstance->flag    = 0;
    return 0;
}

int bSoftPwmStop(bSoftPwmInstance_t *pPwmInstance)
{
    if (pPwmInstance == NULL)
    {
        return -1;
    }
    _bSoftPwmDelete(pPwmInstance);
    return 0;
}

int bSoftPwmReset(bSoftPwmInstance_t *pPwmInstance)
{
    if (pPwmInstance == NULL)
    {
        return -1;
    }
    pPwmInstance->tick = bHalGetSysTick();
    return 0;
}

int bSoftPwmSetPeriod(bSoftPwmInstance_t *pPwmInstance, uint32_t ms)
{
    if (pPwmInstance == NULL)
    {
        return -1;
    }
    pPwmInstance->period = ms;
    return 0;
}

int bSoftPwmSetCcr(bSoftPwmInstance_t *pPwmInstance, uint32_t ms)
{
    if (pPwmInstance == NULL)
    {
        return -1;
    }
    pPwmInstance->ccr = ms;
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
