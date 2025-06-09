/**
 *!
 * \file        b_at.h
 * \version     v0.0.1
 * \date        2019/12/26
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
#ifndef __B_AT_H__
#define __B_AT_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"  
#if _AT_ENABLE

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup AT
 * \{
 */

/** 
 * \defgroup AT_Exported_TypesDefinitions
 * \{
 */
 
typedef void (*pAT_TX)(uint8_t *, uint16_t len); 
 
typedef struct
{
    uint8_t stat;
    uint32_t ctick;
    pAT_TX AT_TXFunc; 
    uint8_t buf[_AT_BUF_LEN];
    volatile uint8_t r_flag; 
    volatile uint16_t r_len;
}bAT_Info_t;

typedef struct
{
    uint8_t *pResp;
    uint16_t len;
    uint32_t timeout;
}bAT_ExpectedResp_t;

/**
 * \}
 */
   
/** 
 * \defgroup AT_Exported_Defines
 * \{
 */
#define AT_STA_NULL         0
#define AT_STA_WAIT         1

/**
 * \}
 */
   
/** 
 * \defgroup AT_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup AT_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup AT_Exported_Functions
 * \{
 */
int bAT_Regist(pAT_TX ptx);
int bAT_Write(int no, bAT_ExpectedResp_t *pe_resp, const char *pcmd, ...);
int bAT_Read(int no, uint8_t *pbuf, uint16_t size);
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

