/**
 *!
 * \file       berror.h
 * \brief      Error management
 * \version    v0.0.1
 * \date       2019/06/05
 * \author     notrynohigh
 * Last modified by notrynohigh 2019/06/05
 * Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
#ifndef __BERROR_H__
#define __BERROR_H__
/*Includes ----------------------------------------------*/
#include <stdint.h>  

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_ERROR
 * \{
 */


/** 
 * \defgroup BERROR_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
    uint8_t err;
    uint32_t utc;
    uint32_t s_tick;
    uint32_t d_tick;
}bErrorInfo_t;    
/**
 * \}
 */
   
/** 
 * \defgroup BERROR_Exported_Defines
 * \{
 */
#define BERROR_RCD_NUMBER       5  
#define INVALID_ERR             ((uint8_t)0)


#define BERROR_RESTART          0X01
#define BERROR_SENSOR           0X02
#define BERROR_LORA             0X04
#define BERROR_FLASH            0X08
#define BERROR_BATTERY          0X10
#define BERROR_FW               0X20
#define BERROR_TEAR				0X40


#define BERROR_LEVEL_0          0X00
#define BERROR_LEVEL_1          0X01

#define IS_LEVEL1_ERR(n)		((n == BERROR_TEAR))

#ifndef NULL
#define NULL ((void *)0)
#endif
/**
 * \}
 */
   
/** 
 * \defgroup BERROR_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BERROR_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BERROR_Exported_Functions
 * \{
 */

int bErrorInfoRegist(uint8_t err, uint32_t utc, uint32_t interval, uint32_t level);
int bErrorGetInfo(bErrorInfo_t **pinfo);
int bErrorClear(bErrorInfo_t *perr);
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

