/**
 *!
 * \file       bint.h
 * \brief      Software interrupt
 * \version    v0.0.1
 * \date       2019/06/06
 * \author     notrynohigh
 * Last modified by notrynohigh 2019/06/06
 * Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
#ifndef __BINT_H__
#define __BINT_H__
/*Includes ----------------------------------------------*/
#include <stdint.h>

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_INT
 * \{
 */

/** 
 * \defgroup BINT_Exported_TypesDefinitions
 * \{
 */
typedef void (*pIntHandler_t)(void);  

typedef struct
{
    uint8_t enable;
    volatile uint8_t trigger;
    pIntHandler_t phandler;
}bIntInfo_t;


/**
 * \}
 */
   
/** 
 * \defgroup BINT_Exported_Defines
 * \{
 */
#ifndef NULL
#define NULL    ((void *)0)
#endif

/**
 * \}
 */
   
/** 
 * \defgroup BINT_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BINT_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BINT_Exported_Functions
 * \{
 */
int bIntBusy(void); 
int bIntCore(void);  
int bIntTrigger(uint8_t number);
int bIntRegist(uint8_t number, pIntHandler_t phandler);
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

