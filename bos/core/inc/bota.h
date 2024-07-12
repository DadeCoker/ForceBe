/**
 *!
 * \file       bota.h
 * \brief      OTA
 * \version    v0.0.1
 * \date       2019/05/08
 * \author     notrynohigh
 * Last modified by notrynohigh 2019/05/08
 * Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
#ifndef __BOTA_H__
#define __BOTA_H__
/*Includes ----------------------------------------------*/
#include <stdint.h> 
#include "bconfig.h"

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_OTA
 * \{
 */

/** 
 * \defgroup BOTA_Exported_TypesDefinitions
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
    uint8_t buf[bCFG_FW_BUF_SIZE];
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
 * \defgroup BOTA_Exported_Defines
 * \{
 */

#define BOTA_CMD_FW_START           0XE2
#define BOTA_CMD_FW_DATA            0XE3
#define BOTA_CMD_FW_ACK             0XE4


#define BOTA_S_NULL                 0
#define BOTA_S_ING                  1

/**
 * \}
 */
   
/** 
 * \defgroup BOTA_Exported_Macros
 * \{
 */
      
/**
 * \}
 */
   
/** 
 * \defgroup BOTA_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BOTA_Exported_Functions
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
/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/

