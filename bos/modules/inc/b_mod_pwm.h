/**
 *!
 * \file        b_mod_pwm.h
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
#ifndef __B_MOD_PWM_H__
#define __B_MOD_PWM_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

#include "b_config.h"

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
 * \defgroup PWM_Exported_TypesDefinitions
 * \{
 */

typedef void (*pPwmHandler)(void);

typedef struct bSoftPwmStruct
{
    uint8_t                  repeat;		//0 一直重复			
    uint32_t                 tick;
    uint32_t                 period;
	uint32_t                 ccr;
    pPwmHandler            	handler1;
	pPwmHandler            	handler2;
    struct bSoftPwmStruct *next;
} bSoftPwmStruct_t;

typedef bSoftPwmStruct_t bSoftPwmInstance_t;

/**
 * \}
 */

/**
 * \defgroup PWM_Exported_Defines
 * \{
 */

#define bPWM_INSTANCE(name, _period, _ccr, _repeat) \
    bSoftPwmInstance_t name = {.period = _period,.ccr = _ccr, .repeat = _repeat};

/**
 * \}
 */

/**
 * \defgroup PWM_Exported_Functions
 * \{
 */
///< pPWMInstance \ref bPWM_INSTANCE
int bSoftPwmStart(bSoftPwmInstance_t *pPwmInstance, pPwmHandler handler1, pPwmHandler handler2);
int bSoftPwmStop(bSoftPwmInstance_t *pPwmInstance);
int bSoftPwmReset(bSoftPwmInstance_t *pPwmInstance);
int bSoftPwmSetPeriod(bSoftPwmInstance_t *pPwmInstance, uint32_t ms);
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
