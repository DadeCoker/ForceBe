/**
 *!
 * \file        b_sum.c
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
#include "b_sum.h"  
#if (_CHECK_ENABLE && _CHECK_XOR_SUM_ENABLE)
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup XOR_SUM
 * \{
 */

/** 
 * \defgroup XOR_SUM_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup XOR_SUM_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup XOR_SUM_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup XOR_SUM_Private_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup XOR_SUM_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup XOR_SUM_Private_Functions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \addtogroup XOR_SUM_Exported_Functions
 * \{
 */
 

uint8_t bXOR(uint8_t *pbuf, uint8_t len)
{
    uint8_t tmp, i;
    if(pbuf == NULL || len == 0)
    {
        return 0;
    }
    tmp = pbuf[0];
    
    for(i = 1;i < len;i++)
    {
        tmp ^= pbuf[i];
    }
    return tmp;
} 


uint8_t bSUM(uint8_t *pbuf, uint8_t len)
{
    uint8_t tmp, i;
    if(pbuf == NULL || len == 0)
    {
        return 0;
    }
    tmp = pbuf[0];
    
    for(i = 1;i < len;i++)
    {
        tmp += pbuf[i];
    }
    return tmp;
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
#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/

