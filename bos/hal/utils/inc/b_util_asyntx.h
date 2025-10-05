/**
 *!
 * \file        b_util_asyntx.h
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
#ifndef __B_UTIL_ASYNTX_H__
#define __B_UTIL_ASYNTX_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"  
/** 
 * \addtogroup B_UTILS
 * \{
 */

/** 
 * \addtogroup ASYN_TX
 * \{
 */

/** 
 * \defgroup ASYN_TX_Exported_TypesDefinitions
 * \{
 */

#define _ASYN_TX_I_NUMBER               1

typedef void (*pSendBytes)(uint8_t *pbuf, uint16_t len); 

typedef struct
{
    uint8_t state;
    volatile uint8_t td_flag;
    pSendBytes f;
    uint32_t s_tick;
    uint32_t timeout;
    uint8_t timeout_f;
}bAsyntxInfo_t;
/**
 * \}
 */
   
/** 
 * \defgroup ASYN_TX_Exported_Defines
 * \{
 */
#define BASYN_TX_NULL		    0
#define BASYN_TX_WAIT		    1



#define BASYN_TX_REQ_L0         0
#define BASYN_TX_REQ_L1         1

/**
 * \}
 */
   
/** 
 * \defgroup ASYN_TX_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup ASYN_TX_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup ASYN_TX_Exported_Functions
 * \{
 */
int bAsyntxRegist(pSendBytes f, uint32_t timeout_ms);
int bAsyntxRequest(int no, uint8_t *pbuf, uint16_t size, uint8_t flag);
int bAsyntxCplCallback(int no);

///<Called in bExec()
void bAsyntxCore(void);

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

/************************ Copyright (c) 2019 Bean *****END OF FILE****/

