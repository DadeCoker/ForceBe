/**
 *!
 * \file        b_utils.c
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
#include "b_utils.h"   

/** 
 * \addtogroup B_HAL
 * \{
 */

/** 
 * \addtogroup UTILS
 * \{
 */

/** 
 * \defgroup UTILS_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup UTILS_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup UTILS_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup UTILS_Private_Variables
 * \{
 */
extern volatile uint32_t bSysTick;

/**
 * \}
 */
   
/** 
 * \defgroup UTILS_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup UTILS_Private_Functions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \addtogroup UTILS_Exported_Functions
 * \{
 */


void bHalDelayMS(uint32_t xms)
{
  uint32_t tickstart = bSysTick;
  uint32_t wait = xms * _TICK_FRQ_HZ / 1000;

  while ((bSysTick - tickstart) < wait)
  {
    ;
  }
}

void bHalDelayUS(uint32_t xus)
{
    while(xus--)
    {
        __nop();
    }
}

uint32_t bHalGetTick()
{
    return bSysTick;
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

/************************ Copyright (c) 2019 Bean *****END OF FILE****/


