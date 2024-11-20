/**
 *!
 * \file        b_kv.h
 * \version     v0.0.1
 * \date        2020/01/08
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 * 
 * Copyright (c) 2020 Bean
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO KV SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
#ifndef __B_KV_H__
#define __B_KV_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"
#if _KV_ENABLE

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup KV
 * \{
 */

/** 
 * \defgroup KV_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
    uint32_t id;
    uint32_t address;
    uint32_t len;
}bKV_Index_t;    
 
 
 
typedef struct
{
    uint8_t status;
    uint8_t index;
    int dev_no;
    uint32_t s_address;
    uint32_t e_size;
    uint32_t tc_address;
    uint32_t dc_address;
    uint32_t t_address[2];
    uint32_t d_address[2];
}bKV_Info_t;

/**
 * \}
 */
   
/** 
 * \defgroup KV_Exported_Defines
 * \{
 */
#define bKV_IDLE        0
#define bKV_BUSY        1
#define bKV_ERROR       2


#define bKV_SECTOR_T1   0X01
#define bKV_SECTOR_T2   0X02 
#define bKV_SECTOR_D1   0X04
#define bKV_SECTOR_D2   0X08 
#define bKV_SECTOR_ALL  0X0F 
/**
 * \}
 */
   
/** 
 * \defgroup KV_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup KV_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup KV_Exported_Functions
 * \{
 */

int bKV_Init(int dev_no, uint32_t s_addr, uint32_t size, uint32_t e_size);
int bKV_Set(uint8_t *key, uint8_t *pvalue, uint16_t len);
int bKV_Get(uint8_t *key, uint8_t *pvalue);
int bKV_Delete(uint8_t *key);
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


