/**
 *!
 * \file       bdata2.h
 * \brief      Save data to a contiguous area
 * \version    v0.0.1
 * \date       2019/05/08
 * \author     notrynohigh
 * Last modified by notrynohigh 2019/05/08
 * Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
#ifndef __BDATA2_H__
#define __BDATA2_H__
/*Includes ----------------------------------------------*/
#include <stdint.h>   

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_DATA2
 * \{
 */

/** 
 * \defgroup BDATA2_Exported_TypesDefinitions
 * \{
 */
 
///< |head(1byte)|content|...
typedef struct
{
    uint32_t address;
    uint32_t size;
    uint32_t esize;
    uint32_t usize;  
}bData2Struct_t;




typedef struct 
{
    uint8_t flag;
    uint32_t index;
    uint32_t max_num;
    bData2Struct_t st;
    uint8_t dev_no;
}bData2Info_t;


/**
 * \}
 */
   
/** 
 * \defgroup BDATA2_Exported_Defines
 * \{
 */






/**
 * \}
 */
   
/** 
 * \defgroup BDATA2_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BDATA2_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BDATA2_Exported_Functions
 * \{
 */
int bData2RegistInfo(bData2Struct_t, uint8_t dev_no);
int bData2Write(int no, uint8_t *pbuf);
int bData2Read(int no, uint32_t index, uint8_t *pbuf);
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

