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
#include "b_mod_event.h"
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
static bEventInfo_t bEventInfoTable[_EVENT_Q_LENGTH];   
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
   
/**
 * \}
 */
   
/** 
 * \addtogroup EVENT_Exported_Functions
 * \{
 */
 
/**
 * \brief Register an event
 * \param number Event number same as priority
 * \param phandler The Callback function
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */ 
int bEventRegist(uint8_t number, pEventHandler_t phandler)
{
    static uint8_t init = 0;
    uint8_t i = 0;
    
    if(number >= _EVENT_Q_LENGTH || phandler == NULL)
    {
        return -1;
    }
    
    if(init == 0)
    {
        for(i = 0;i < _EVENT_Q_LENGTH;i++)
        {
            bEventInfoTable[i].enable = 0;
        }
        init = 1;
    }
    
    if(bEventInfoTable[number].enable == 1)
    {
        return -1;
    }
    
    bEventInfoTable[number].enable = 1;
    bEventInfoTable[number].trigger = 0;
    bEventInfoTable[number].phandler = phandler;
    return 0;
} 


/**
 * \brief Call this function after the event is generated
 * \param number Event number
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bEventTrigger(uint8_t number)
{
    if(number >= _EVENT_Q_LENGTH)
    {
        return -1;
    }  
    
    if(bEventInfoTable[number].enable == 0)
    {
        return -1;
    }
    
    bEventInfoTable[number].trigger = 1;
    return 0;
}

/**
 * \brief check and run callback
 */
void bEventCore()
{
    uint8_t i = 0;
    for(i = 0;i < _EVENT_Q_LENGTH;i++)
    {
        if(bEventInfoTable[i].trigger == 1 && bEventInfoTable[i].enable == 1)
        {
            bEventInfoTable[i].phandler();
            bEventInfoTable[i].trigger = 0;
        }
    }
}


int bEventIsIdle()
{
    uint8_t i = 0;
    for(i = 0;i < _EVENT_Q_LENGTH;i++)
    {
        if(bEventInfoTable[i].trigger == 1 && bEventInfoTable[i].enable == 1)
        {
            return -1;
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

