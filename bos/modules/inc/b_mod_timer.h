/**
 *!
 * \file        b_mod_timer.h
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
#ifndef __B_MOD_TIMER_H__
#define __B_MOD_TIMER_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"  
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
 * \defgroup TIMER_Exported_TypesDefinitions
 * \{
 */
 
typedef void (*pTimerHandler)(void); 

typedef struct bSoftTimerStruct
{
    uint8_t repeat;
    uint32_t tick;
    uint32_t period;
    pTimerHandler handler;
    struct bSoftTimerStruct *next;
}bSoftTimerStruct_t;

typedef bSoftTimerStruct_t      bSoftTimerInstance_t;

/**
 * \}
 */
   
/** 
 * \defgroup TIMER_Exported_Defines
 * \{
 */

#define bTIMER_INSTANCE(name, _period, _repeat)     bSoftTimerInstance_t name = {\
                                                                .period = _period,\
                                                                .repeat = _repeat};

/**
 * \}
 */
   
/** 
 * \defgroup TIMER_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup TIMER_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup TIMER_Exported_Functions
 * \{
 */
///< pTimerInstance \ref bTIMER_INSTANCE
int bSoftTimerStart(bSoftTimerInstance_t *pTimerInstance, pTimerHandler handler);
int bSoftTimerStop(bSoftTimerInstance_t *pTimerInstance);
int bSoftTimerReset(bSoftTimerInstance_t *pTimerInstance);
int bSoftTimerSetPeriod(bSoftTimerInstance_t *pTimerInstance, uint32_t ms);
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

