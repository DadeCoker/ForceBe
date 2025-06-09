/**
 *!
 * \file        b_f8l10d.h
 * \version     v0.0.1
 * \date        2020/03/18
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 * 
 * Copyright (c) 2020 Bean
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
#ifndef __B_F8L10D_H__
#define __B_F8L10D_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h" 
#include "b_device.h"
/** 
 * \addtogroup B_DRIVER
 * \{
 */

/** 
 * \addtogroup F8L10D
 * \{
 */

/** 
 * \defgroup F8L10D_Exported_TypesDefinitions
 * \{
 */
 
typedef struct
{
    /**************depend on the platform**************************/
    void (*pUartSend)(uint8_t *, uint16_t);
    void (*pSleepPin_Control)(uint8_t);
    void (*pResetPin_Control)(uint8_t);
    
    /**********************private variables**********************************/
    int at_no;
    int atx_no;
    uint8_t buf[200];
}bF8L10D_Private_t;    
 
 
typedef bDriverInterface_t bF8L10D_Driver_t;  

/**
 * \}
 */
   
/** 
 * \defgroup F8L10D_Exported_Defines
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup F8L10D_Exported_Macros
 * \{
 */



/**
 * \}
 */
   
/** 
 * \defgroup F8L10D_Exported_Variables
 * \{
 */
  
/**
 * \}
 */
   
/** 
 * \defgroup F8L10D_Exported_Functions
 * \{
 */
int bF8L10D_Init(bF8L10D_Driver_t *);
void bF8L10D_TXDoneIrqHandler(bF8L10D_Driver_t *pdrv);
void bF8L10D_RXCplHandler(bF8L10D_Driver_t *pdrv, uint8_t *pbuf, uint16_t len);
/**
 * \}
 */
 

/**
 * \}
 */

/**
 * \}
 */

#ifdef __cplusplus
	}
#endif
 
#endif


/************************ Copyright (c) 2019 Bean *****END OF FILE****/



