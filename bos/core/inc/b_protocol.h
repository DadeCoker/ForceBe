/**
 *!
 * \file        b_protocol.h
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
#ifndef __B_PROTOCOL_H__
#define __B_PROTOCOL_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"
#if _PROTO_ENABLE
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup PROTOCOL
 * \{
 */

/** 
 * \defgroup PROTOCOL_Exported_TypesDefinitions
 * \{
 */

////|bProtocolHead_t | data | sum|

#pragma pack(1) 

typedef struct
{
    uint8_t head;
    uint32_t device_id;
    uint8_t len;
    uint8_t cmd;
}bProtocolHead_t;   

#pragma pack()

typedef int (*pdispatch)(uint8_t *pbuf, uint32_t len);

typedef struct
{
    uint32_t id;
    uint8_t tx_no;
    pdispatch f;
}bProtocolInfo_t;


/**
 * \}
 */
   
/** 
 * \defgroup PROTOCOL_Exported_Defines
 * \{
 */


#define PROTOCOL_HEAD       0xFE

/**
 * \}
 */
   
/** 
 * \defgroup PROTOCOL_Exported_Macros
 * \{
 */
      
/**
 * \}
 */
   
/** 
 * \defgroup PROTOCOL_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup PROTOCOL_Exported_Functions
 * \{
 */
int bProtocolRegist(uint32_t id, uint8_t tx_no, pdispatch f);
int bProtocolSetID(uint8_t no, uint32_t id);
int bProtocolParseCmd(uint8_t no, uint8_t *pbuf, uint8_t len);
int bProtocolPack(uint8_t no, uint8_t cmd, uint8_t *psrc, uint8_t *pdes, uint8_t size);
int bProtocolUnpack(uint8_t *pbuf, uint8_t len, uint8_t **pdata, uint8_t size);
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

