/**
 *!
 * \file        b_modbus.h
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
#ifndef __B_MODBUS_H__
#define __B_MODBUS_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"
#if _MODBUS_ENABLE

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup MODBUS
 * \{
 */

/** 
 * \defgroup MODBUS_Exported_TypesDefinitions
 * \{
 */

#pragma pack(1)
 
typedef struct
{
    uint8_t addr;
    uint8_t func;
    uint16_t reg;       //Big endian
    uint16_t num;       //Big endian
    uint16_t crc;       //Little endian
}bMB_RTUS_W_t;


typedef struct
{
    uint8_t addr;
    uint8_t func;
    uint8_t len;
    uint8_t buf[1];
}bMB_RTUS_Ack_t;



#pragma pack()
 
typedef struct 
{
    uint8_t dev_no;
}bMB_Info_t;
   
/**
 * \}
 */
   
/** 
 * \defgroup MODBUS_Exported_Defines
 * \{
 */
#ifndef NULL
#define NULL ((void *)0)   
#endif

/**
 * \}
 */
   
/** 
 * \defgroup MODBUS_Exported_Macros
 * \{
 */
#define L2B_B2L_16b(n)  (((n) << 8) | (n >> 8))   
/**
 * \}
 */
   
/** 
 * \defgroup MODBUS_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup MODBUS_Exported_Functions
 * \{
 */
int bMB_Regist(uint8_t dev_no);
int bMB_WriteCmd(uint8_t no, uint8_t addr, uint8_t func, uint16_t reg, uint16_t num);
int bMB_CheckRTUS_ACK(uint8_t *psrc, uint16_t len);
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

