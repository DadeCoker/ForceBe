/**
 *!
 * \file        b_ota.h
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
#ifndef __B_OTA_H__
#define __B_OTA_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"
#if (_PROTO_ENABLE && _PROTO_OTA_ENABLE) 

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup OTA
 * \{
 */

/** 
 * \defgroup OTA_Exported_TypesDefinitions
 * \{
 */

#pragma pack(1)


typedef struct
{
    uint32_t f_size;
    uint32_t f_crc32;
    uint32_t f_key;
}bOTA_FWInfo_t;

typedef struct
{
    uint16_t number;
    uint8_t buf[_OTA_BUFF_SIZE];
}bOTA_FWData_t;

typedef struct
{
    uint16_t number;
}bOTA_FWACK_t;

#pragma pack()


typedef struct
{
    uint8_t status;
    uint16_t number;
    int data_no;
    uint8_t dev_no;
    uint8_t pro_no;
}bOTA_Ctl_t;



/**
 * \}
 */
   
/** 
 * \defgroup OTA_Exported_Defines
 * \{
 */

#define BOTA_CMD_FW_START           0XE2
#define BOTA_CMD_FW_DATA            0XE3
#define BOTA_CMD_FW_ACK             0XE4


#define BOTA_S_NULL                 0
#define BOTA_S_ING                  1

#define BOTA_SECTOR_SIZE            0X1000

/**
 * \}
 */
   
/** 
 * \defgroup OTA_Exported_Macros
 * \{
 */
      
/**
 * \}
 */
   
/** 
 * \defgroup OTA_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup OTA_Exported_Functions
 * \{
 */
int bOTA_Init(uint8_t dev_no, uint8_t pro_no);
int bOTA_Parse(uint8_t *pbuf, uint16_t len);
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

