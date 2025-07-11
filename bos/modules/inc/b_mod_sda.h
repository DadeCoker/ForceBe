/**
 *!
 * \file        b_mod_sda.h
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
#ifndef __B_MOD_SDA_H__
#define __B_MOD_SDA_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"
#if (_SAVE_DATA_ENABLE && _SAVE_DATA_A_ENABLE)
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup MODULES
 * \{
 */

/** 
 * \addtogroup SAVE_DATA
 * \{
 */

/** 
 * \defgroup SDA_Exported_TypesDefinitions
 * \{
 */

 
typedef struct
{
    uint8_t min_unit;       
    uint8_t min_number;
    uint32_t min_size;
    uint8_t total_unit;
    uint8_t total_number;
    
    uint32_t fbase_address;
    uint32_t fsize;
    uint32_t ferase_size;
}bSDA_Struct_t;


typedef struct
{
    uint8_t e_flag;
    uint16_t n_per_eu;
    uint16_t un_number;
    uint32_t total_size;
    uint32_t base_address;
    uint32_t data_address;
    bSDA_Struct_t st;
    uint8_t dev_no;
}bSDA_Info_t;


typedef struct
{
    uint32_t utc;
}bSDA_InitInfo_t;



/**
 * \}
 */
   
/** 
 * \defgroup SDA_Exported_Defines
 * \{
 */

#define UNIT_SECOND         0
#define UNIT_MINUTE         1
#define UNIT_HOUR           2
#define UNIT_DAY            3
#define UNIT_MONTH          4
#define UNIT_YEAR           5
#define UNIT_NUMBER         6

#ifndef NULL
#define NULL    ((void *)0)
#endif

/**
 * \}
 */
   
/** 
 * \defgroup SDA_Exported_Macros
 * \{
 */
#define IS_UNIT_VALID(n)        ((n) == UNIT_SECOND || (n) == UNIT_MINUTE || (n) == UNIT_HOUR  \
                                    || (n) == UNIT_DAY || (n) == UNIT_MONTH || (n) == UNIT_YEAR) 



/**
 * \}
 */
   
/** 
 * \defgroup SDA_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SDA_Exported_Functions
 * \{
 */

int bSDA_Regist(bSDA_Struct_t , uint8_t dev_no);
int bSDA_Write(int, uint32_t utc, uint8_t *pbuf);
int bSDA_Read(int, uint32_t utc, uint8_t *pbuf);
int bSDA_TimeChanged(int, uint32_t o_utc, uint32_t n_utc);

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

#ifdef __cplusplus
	}
#endif

#endif  

/************************ Copyright (c) 2019 Bean *****END OF FILE****/

