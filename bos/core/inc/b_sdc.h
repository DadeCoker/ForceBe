/**
 *!
 * \file        b_sdc.h
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
#ifndef __B_SDC_H__
#define __B_SDC_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"
#if (_SAVE_DATA_ENABLE && _SAVE_DATA_C_ENABLE)
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup SAVE_DATA
 * \{
 */

/** 
 * \defgroup SDC_Exported_TypesDefinitions
 * \{
 */
 
///< |head(1byte)|content|...
typedef struct
{
    uint32_t address;
    uint32_t size;
    uint32_t esize;
    uint32_t usize;  
}bSDC_Struct_t;




typedef struct 
{
    uint8_t flag;
    uint32_t index;
    uint32_t max_num;
    bSDC_Struct_t st;
    uint8_t dev_no;
}bSDC_Info_t;


/**
 * \}
 */
   
/** 
 * \defgroup SDC_Exported_Defines
 * \{
 */


#define bSDC_HEAD       0XCC



/**
 * \}
 */
   
/** 
 * \defgroup SDC_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SDC_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SDC_Exported_Functions
 * \{
 */
int bSDC_Regist(bSDC_Struct_t, uint8_t dev_no);
int bSDC_Write(int no, uint8_t *pbuf);
int bSDC_Read(int no, uint32_t index, uint8_t *pbuf);
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

