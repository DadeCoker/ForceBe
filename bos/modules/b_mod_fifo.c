/**
 *!
 * \file        b_mod_fifo.c
 * \version     v0.0.1
 * \date        2019/12/23
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
#include "b_mod_fifo.h"  
#if _FIFO_ENABLE
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup MODULES
 * \{
 */

/** 
 * \addtogroup FIFO
 * \{
 */

/** 
 * \defgroup FIFO_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup FIFO_Private_Defines
 * \{
 */
 #define FIFO_LEN(w,r,s)       (((w) >= (r)) ? ((w) - (r)) : ((w) + (s) - (r)))    
/**
 * \}
 */
   
/** 
 * \defgroup FIFO_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup FIFO_Private_Variables
 * \{
 */
static bFIFO_Info_t  bFIFO_Info[_FIFO_I_NUMBER];
static uint8_t bFIFO_InfoIndex = 0;   
/**
 * \}
 */
   
/** 
 * \defgroup FIFO_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup FIFO_Private_Functions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \addtogroup FIFO_Exported_Functions
 * \{
 */
 
int bFIFO_Regist(uint8_t *pbuf, uint16_t size)
{
    if(pbuf == NULL || size == 0 || bFIFO_InfoIndex >= _FIFO_I_NUMBER)
    {
        return -1;
    }
    bFIFO_Info[bFIFO_InfoIndex].pbuf = pbuf;
    bFIFO_Info[bFIFO_InfoIndex].r_index = 0;
    bFIFO_Info[bFIFO_InfoIndex].w_index = 0;
    bFIFO_Info[bFIFO_InfoIndex].size = size;
    bFIFO_InfoIndex += 1;
    return (bFIFO_InfoIndex - 1);
}    



int bFIFO_Length(int no, uint16_t *plen)
{
    if(no < 0 || no >= bFIFO_InfoIndex || plen == NULL)
    {
        return -1;
    } 
    *plen = FIFO_LEN(bFIFO_Info[no].w_index, bFIFO_Info[no].r_index, bFIFO_Info[no].size);
    return 0;
}


int bFIFO_Flush(int no)
{
    if(no < 0 || no >= bFIFO_InfoIndex)
    {
        return -1;
    } 
    bFIFO_Info[no].r_index = bFIFO_Info[no].w_index;
    return 0;
}

int bFIFO_Write(int no, uint8_t *pbuf, uint16_t size)
{
    uint16_t fifo_len, valid_len, index;
    if(no < 0 || no >= bFIFO_InfoIndex || pbuf == NULL)
    {
        return -1;
    }
    fifo_len = FIFO_LEN(bFIFO_Info[no].w_index, bFIFO_Info[no].r_index, bFIFO_Info[no].size);
    valid_len = bFIFO_Info[no].size - fifo_len;
    size = (size <= valid_len) ? size : valid_len;
    index = 0;
    while(index < size)
    {
        bFIFO_Info[no].pbuf[bFIFO_Info[no].w_index] = pbuf[index];
        bFIFO_Info[no].w_index = (bFIFO_Info[no].w_index + 1) % bFIFO_Info[no].size;
        index += 1;
    }
    return size;
}


int bFIFO_Read(int no, uint8_t *pbuf, uint16_t size)
{
    uint16_t fifo_len, index;
    if(no < 0 || no >= bFIFO_InfoIndex || pbuf == NULL)
    {
        return -1;
    }
    fifo_len = FIFO_LEN(bFIFO_Info[no].w_index, bFIFO_Info[no].r_index, bFIFO_Info[no].size);
    size = (size <= fifo_len) ? size : fifo_len;
    index = 0;
    while(index < size)
    {
        pbuf[index] = bFIFO_Info[no].pbuf[bFIFO_Info[no].r_index];
        bFIFO_Info[no].r_index = (bFIFO_Info[no].r_index + 1) % bFIFO_Info[no].size;
        index += 1;
    }
    return size;
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

