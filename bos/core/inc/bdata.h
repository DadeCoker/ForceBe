/**
 *!
 * \file       bdata.h
 * \brief      W/R data
 * \version    v0.0.1
 * \date       2019/05/08
 * \author     notrynohigh
 * Last modified by notrynohigh 2019/05/08
 * Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
#ifndef __BDATA_H__
#define __BDATA_H__
/*Includes ----------------------------------------------*/
#include <stdint.h>   

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_DATA
 * \{
 */

/** 
 * \defgroup BDATA_Exported_TypesDefinitions
 * \{
 */
 
///< |head(1byte)|content|crc(1byte)| <<< crc(content) exclude head

typedef struct 
{
    uint8_t flag;
    uint32_t address;
    uint32_t usize;
    uint8_t dev_no;
}bDataInfo_t;


/**
 * \}
 */
   
/** 
 * \defgroup BDATA_Exported_Defines
 * \{
 */






/**
 * \}
 */
   
/** 
 * \defgroup BDATA_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BDATA_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BDATA_Exported_Functions
 * \{
 */
int bDataRegistInfo(uint32_t address, uint32_t usize, uint8_t dev_no);
int bDataWrite(int no, uint8_t *pbuf);
int bDataRead(int no, uint8_t *pbuf);
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

