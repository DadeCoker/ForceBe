/**
 *!
 * \file        b_hal_sccb.h
 * \version     v0.0.1
 * \date        2020/03/25
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SSCCBL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
#ifndef __B_HAL_SCCB_H__
#define __B_HAL_SCCB_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h" 


/** 
 * \addtogroup B_HAL
 * \{
 */

/** 
 * \addtogroup SCCB
 * \{
 */

/** 
 * \defgroup SCCB_Exported_TypesDefinitions
 * \{
 */
typedef enum
{
    B_HAL_SCCB_1,
    B_HAL_SCCB_2,
    B_HAL_SCCB_NUMBER
}bHalSCCBNumber_t; 
/**
 * \}
 */
   
/** 
 * \defgroup SCCB_Exported_Defines
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup SCCB_Exported_Macros
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup SCCB_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SCCB_Exported_Functions
 * \{
 */

void SCCB_Start(bHalSCCBNumber_t sccb);
void SCCB_Stop(bHalSCCBNumber_t sccb);
void SCCB_NoAck(bHalSCCBNumber_t sccb);
void SCCB_SendByte(bHalSCCBNumber_t sccb, uint8_t dat);
uint8_t SCCB_ReceiveByte(bHalSCCBNumber_t sccb);
    
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


/************************ Copyright (c) 2020 Bean *****END OF FILE****/


