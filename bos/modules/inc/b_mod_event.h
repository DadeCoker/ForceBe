/**
 *!
 * \file        b_mod_event.h
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
#ifndef __B_MOD_EVENT_H__
#define __B_MOD_EVENT_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"
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
 * \defgroup EVENT_Exported_TypesDefinitions
 * \{
 */
typedef void (*pEventHandler_t)(void);  

typedef struct bEventInfo
{
    volatile uint8_t trigger;
    pEventHandler_t phandler;
    struct bEventInfo *pnext;
}bEventInfo_t;


typedef bEventInfo_t  bEventInstance_t;   

/**
 * \}
 */
   
/** 
 * \defgroup EVENT_Exported_Defines
 * \{
 */
#define bEVENT_INSTANCE(name)       bEventInstance_t name;
/**
 * \}
 */
/** 
 * \defgroup EVENT_Exported_Functions
 * \{
 */

///< pInstance \ref bEVENT_INSTANCE
int bEventRegist(bEventInstance_t *pInstance, pEventHandler_t handler);
int bEventTrigger(bEventInstance_t *pInstance);
int bEventIsIdle(void); 
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


