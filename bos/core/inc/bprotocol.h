/**
 *!
 * \file       bprotocol.h
 * \brief      Common protocol
 * \version    v0.0.1
 * \date       2019/05/08
 * \author     notrynohigh
 * Last modified by notrynohigh 2019/05/08
 * Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
/*Includes ----------------------------------------------*/
#include <stdint.h> 

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_PROTOCOL
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

#ifndef NULL
#define NULL    ((void *)0)
#endif

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
/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/

