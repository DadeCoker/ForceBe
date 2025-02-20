/**
 *!
 * \file        b_tx.h
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
#ifndef __B_TX_H__
#define __B_TX_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"  
#if _TX_ENABLE
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup TX
 * \{
 */

/** 
 * \defgroup TX_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
    uint8_t *ptxBUF;
    uint16_t buf_size;
    uint8_t state;
    uint16_t tx_len;
    uint8_t td_mode;
    volatile uint8_t td_flag;
    uint8_t dev_no;
    uint32_t timeout;
    uint8_t timeout_f;
    int fd;
}bTX_Info_t;
/**
 * \}
 */
   
/** 
 * \defgroup TX_Exported_Defines
 * \{
 */
#define BTX_NULL		    0
#define BTX_REQ			    1
#define BTX_TXING		    2
#define BTX_WAIT		    3
#define BTX_END			    4 

#define BTX_F_SYN           0
#define BTX_F_ASYN          1

#define BTX_REQ_LEVEL0      0
#define BTX_REQ_LEVEL1      1

/**
 * \}
 */
   
/** 
 * \defgroup TX_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup TX_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup TX_Exported_Functions
 * \{
 */
int bTX_CplCallback(int no);
int bTX_Regist(uint8_t *pbuf, uint32_t size, uint8_t td_mode, uint8_t dev_no); 
int bTX_Request(int no, uint8_t *pbuf, uint16_t size, uint8_t flag);
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

