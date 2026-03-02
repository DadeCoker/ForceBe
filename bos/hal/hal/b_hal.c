/**
 *!
 * \file        b_hal.c
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
#include "b_hal.h" 
#include "b_driver.h"
#include <string.h>
/** 
 * \addtogroup B_HAL
 * \{
 */

/** 
 * \addtogroup HAL
 * \{
 */

/** 
 * \defgroup HAL_Private_TypesDefinitions
 * \{
 */
  
/**
 * \}
 */
   
/** 
 * \defgroup HAL_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup HAL_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup HAL_Private_Variables
 * \{
 */
volatile uint32_t bSysTick = 0;
/**
 * \}
 */
   
/** 
 * \defgroup HAL_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup HAL_Private_Functions
 * \{
 */


/**
 * \}
 */
   
/** 
 * \addtogroup HAL_Exported_Functions
 * \{
 */
void bHalEnterCritical()
{

}    

void bHalExitCritical()
{

}


/**
 * \brief Call this function _TICK_FRQ_HZ times per second \ref _TICK_FRQ_HZ
 */
void bHalIncSysTick()
{
    bSysTick += 1;
}


void bHalInit()
{
    // Add code ...gpio init or some other functions 
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


