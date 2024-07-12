/**
 *!
 * \file        b_crc32.c
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
#if (_CHECK_ENABLE && _CHECK_CRC32_ENABLE)
#include "b_crc32.h"  
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup CRC32
 * \{
 */


/** 
 * \defgroup CRC32_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup CRC32_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup CRC32_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup CRC32_Private_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup CRC32_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup CRC32_Private_Functions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \addtogroup CRC32_Exported_Functions
 * \{
 */

uint32_t bCRC32(uint32_t crc32_in, uint8_t *pbuf, uint32_t len)
{
    uint32_t crc;
    uint32_t i = 0, j = 0;
    crc = crc32_in & 0xFFFFFFFF;
    for (i = 0; i < len; i++)
    {
        crc = crc ^ pbuf[i];
        for (j = 8; j > 0; j--)
        {
            crc = (crc >> 1) ^ (0xEDB88320U & ((crc & 1) ? 0xFFFFFFFF : 0));
        }
    }
    return crc;
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


