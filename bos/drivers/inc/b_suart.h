/**
 *!
 * \file        b_suart.h
 * \version     v0.0.1
 * \date        2019/06/05
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 * 
 * Copyright (c) 2019 Bean
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
#ifndef __B_SUART_H__
#define __B_SUART_H__

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
 * \addtogroup SUART
 * \{
 */

/** 
 * \defgroup SUART_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
    volatile uint8_t byte;
    volatile uint8_t status;
    volatile uint8_t c_bits;
    uint8_t buf[128];
    volatile uint8_t count;
    volatile uint8_t revf;
    volatile uint8_t idle_count;
    volatile uint8_t idle_flag;
    volatile uint8_t tx_delay_flag;
}bSUART_Param_t;    
 
 
typedef struct
{
    bSUART_Param_t info;
    void (*pTxPIN_Control)(uint8_t);
    uint8_t (*RxPIN_Read)(void);
}bSUART_Private_t;

typedef bDriverInterface_t   SUART_Driver_t;  

/**
 * \}
 */
   
/** 
 * \defgroup SUART_Exported_Defines
 * \{
 */

#define S_TX_S_NULL             0
#define S_TX_S_REQ              1
#define S_TX_S_ING              2
#define S_TX_S_STOP             3
#define S_TX_S_WAIT             4


#define S_RX_S_NULL             0
#define S_RX_S_START            1
#define S_RX_S_ING              2
#define S_RX_S_STOP             3
#define S_RX_S_WAIT             4
#define S_RX_S_IDLE             5


#ifndef NULL
#define NULL ((void *)0)   
#endif

/**
 * \}
 */
   
/** 
 * \defgroup SUART_Exported_Macros
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup SUART_Exported_Variables
 * \{
 */
  
/**
 * \}
 */
   
/** 
 * \defgroup SUART_Exported_Functions
 * \{
 */

int SUART_Init(SUART_Driver_t *pdrv);

void S_UartRXStart(SUART_Driver_t *);
void S_UartTxTimerHandler(SUART_Driver_t *);
void S_UartRxTimerHandler(SUART_Driver_t *);

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


