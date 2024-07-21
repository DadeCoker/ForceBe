/**
 *!
 * \file        b_sdb.h
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
#ifndef __B_SDB_H__
#define __B_SDB_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"
#if (_SAVE_DATA_ENABLE && _SAVE_DATA_B_ENABLE)
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup SAVE_DATA
 * \{
 */

/** 
 * \defgroup SDB_Exported_TypesDefinitions
 * \{
 */
 
///< |head(1byte)|content|crc(1byte)| <<< crc(content) exclude head

typedef struct 
{
    uint8_t flag;
    uint32_t address;
    uint32_t usize;
    uint8_t dev_no;
}bSDB_Info_t;


/**
 * \}
 */
   
/** 
 * \defgroup SDB_Exported_Defines
 * \{
 */


#define bSDB_HEAD       0XBB



/**
 * \}
 */
   
/** 
 * \defgroup SDB_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SDB_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SDB_Exported_Functions
 * \{
 */
int bSDB_Regist(uint32_t address, uint32_t usize, uint8_t dev_no);
int bSDB_Write(int no, uint8_t *pbuf);
int bSDB_Read(int no, uint8_t *pbuf);
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

