/**
 *!
 * \file        b_fifo.h
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
#ifndef __B_FIFO_H__
#define __B_FIFO_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"  
#if _FIFO_ENABLE

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup FIFO
 * \{
 */

/** 
 * \defgroup FIFO_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
    uint8_t *pbuf;
    uint16_t size;
    volatile uint16_t r_index;
    volatile uint16_t w_index;
}bFIFO_Info_t;
/**
 * \}
 */
   
/** 
 * \defgroup FIFO_Exported_Defines
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup FIFO_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup FIFO_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup FIFO_Exported_Functions
 * \{
 */
int bFIFO_Regist(uint8_t *pbuf, uint16_t size);
int bFIFO_Length(int no, uint16_t *plen);
int bFIFO_Flush(int no);
int bFIFO_Write(int no, uint8_t *pbuf, uint16_t size);
int bFIFO_Read(int no, uint8_t *pbuf, uint16_t size);
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

