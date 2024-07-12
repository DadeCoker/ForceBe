
/**
 *!
 * \file       bsum.h
 * \brief      SUM & Xor
 * \version    v0.0.1
 * \date       2019/07/02
 * \author     notrynohigh
 * Last modified by notrynohigh 2019/07/02
 * Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
#ifndef __BSUM_H__
#define __BSUM_H__
/*Includes ----------------------------------------------*/
#include <stdint.h>  

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_SUM
 * \{
 */

/** 
 * \defgroup SUM_Exported_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SUM_Exported_Defines
 * \{
 */
#ifndef NULL
#define NULL ((void *)0)   
#endif

/**
 * \}
 */
   
/** 
 * \defgroup SUM_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SUM_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SUM_Exported_Functions
 * \{
 */
uint8_t bSUM(uint8_t *pbuf, uint8_t len); 
uint8_t bSUM2(uint8_t *pbuf, uint8_t len); 
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

