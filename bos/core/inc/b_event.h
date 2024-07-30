/**
 *!
 * \file        b_event.h
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
#ifndef __B_EVENT_H__
#define __B_EVENT_H__

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
 * \addtogroup EVENT
 * \{
 */

/** 
 * \defgroup EVENT_Exported_TypesDefinitions
 * \{
 */
typedef void (*pEventHandler_t)(void);  

typedef struct
{
    uint8_t enable;
    volatile uint8_t trigger;
    pEventHandler_t phandler;
}bEventInfo_t;


/**
 * \}
 */
   
/** 
 * \defgroup EVENT_Exported_Defines
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup EVENT_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup EVENT_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup EVENT_Exported_Functions
 * \{
 */
int bEventIsIdle(void); 
int bEventCore(void);  
int bEventTrigger(uint8_t number);
int bEventRegist(uint8_t number, pEventHandler_t phandler);
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


