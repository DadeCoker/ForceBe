/**
 *!
 * \file        b_hal_uart.h
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SUARTL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
#ifndef __B_HAL_UART_H__
#define __B_HAL_UART_H__

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
 * \addtogroup UART
 * \{
 */

/** 
 * \defgroup UART_Exported_TypesDefinitions
 * \{
 */

#define UART_BUF_LEN            200
 
typedef enum
{
    B_HAL_UART_1,
    B_HAL_UART_2,
    B_HAL_UART_NUMBER
}bHalUartNumber_t; 



typedef struct
{
    uint8_t *pbuf;
    volatile uint32_t l_tick;
    volatile uint16_t index;
    uint16_t max_len;
}bHalUartRxInfo_t;



/**
 * \}
 */
   
/** 
 * \defgroup UART_Exported_Defines
 * \{
 */
#define UART_1_RXBUF_LEN        200        
#define UART_2_RXBUF_LEN        200  
/**
 * \}
 */
   
/** 
 * \defgroup UART_Exported_Macros
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup UART_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup UART_Exported_Functions
 * \{
 */

void bHalUartSend(uint8_t no, uint8_t *pbuf, uint16_t len);

void bHalUartRxIRQ_Handler(uint8_t no, uint8_t dat);
void bHalUartIdleCallback(uint8_t no, uint8_t *pbuf, uint16_t len);



///<Called in bHalCore()
void bHalUartDetectIdle(void);


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


