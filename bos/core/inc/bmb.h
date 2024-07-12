/**
 *!
 * \file       bmb.h
 * \brief      Modbus RTU
 * \version    v0.0.1
 * \date       2019/07/02
 * \author     notrynohigh
 * Last modified by notrynohigh 2019/07/02
 * Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
#ifndef __BMB_H__
#define __BMB_H__
/*Includes ----------------------------------------------*/
#include <stdint.h>  

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_MB
 * \{
 */

/** 
 * \defgroup MB_Exported_TypesDefinitions
 * \{
 */

#pragma pack(1)
 
typedef struct
{
    uint8_t addr;
    uint8_t func;
    uint16_t reg;       //大端模式
    uint16_t num;       //大端模式
    uint16_t crc;       //小端模式
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
}bMBInfo_t;
   
/**
 * \}
 */
   
/** 
 * \defgroup MB_Exported_Defines
 * \{
 */
#ifndef NULL
#define NULL ((void *)0)   
#endif

/**
 * \}
 */
   
/** 
 * \defgroup MB_Exported_Macros
 * \{
 */
#define L2B_B2L_16b(n)  (((n) << 8) | (n >> 8))   
/**
 * \}
 */
   
/** 
 * \defgroup MB_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup MB_Exported_Functions
 * \{
 */
int bMB_Regist(uint8_t dev_no);
int bMB_WriteCmd(uint8_t no, uint8_t addr, uint8_t func, uint16_t reg, uint16_t num);
int bMB_CheckReadCmdAck(uint8_t *psrc, uint16_t len);
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

