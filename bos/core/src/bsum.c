
/**
 *!
 * \file       bsum.c
 * \brief      SUM & Xor
 * \version    v0.0.1
 * \date       2019/07/02
 * \author     notrynohigh
 *Last modified by notrynohigh 2019/07/02
 *Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "bsum.h" 

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_SUM
 * \brief 数据和计算
 * \{
 */

/** 
 * \defgroup SUM_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SUM_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SUM_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SUM_Private_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SUM_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SUM_Private_Functions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \addtogroup SUM_Exported_Functions
 * \{
 */
 
/**
 * \brief 计算数据和 
 */ 
uint8_t bSUM(uint8_t *pbuf, uint8_t len)
{
    uint8_t tmp, i;
    if(pbuf == NULL || len == 0)
    {
        return 0;
    }
    tmp = pbuf[0];
    
    for(i = 1;i < len;i++)
    {
        tmp ^= pbuf[i];
    }
    return tmp;
} 

uint8_t bSUM2(uint8_t *pbuf, uint8_t len)
{
    uint8_t tmp, i;
    if(pbuf == NULL || len == 0)
    {
        return 0;
    }
    tmp = pbuf[0];
    
    for(i = 1;i < len;i++)
    {
        tmp += pbuf[i];
    }
    return tmp;
} 

/**
 * \}
 */

/**
 * \}
 */


/**
 * \}
 */

/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/

