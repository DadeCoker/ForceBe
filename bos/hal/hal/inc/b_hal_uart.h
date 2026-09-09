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
 
typedef enum
{
    B_HAL_UART_1,
    B_HAL_UART_2,
    B_HAL_UART_NUMBER
}bHalUartNumber_t; 



typedef void (*pbUartRxIdleHandler_t)(uint8_t *pbuf, uint16_t len);

typedef struct bHalUartRxStruct
{
    bHalUartNumber_t uart;
    uint8_t *pbuf;
    uint16_t buf_len;
    uint32_t idle_threshold;
    pbUartRxIdleHandler_t handler;
    volatile uint32_t l_tick;
    volatile uint32_t l_index;
    volatile uint16_t index;
}bHalUartRxInfo_t;



typedef bHalUartRxInfo_t        bHalUartRxInstance_t;


/**
 * \}
 */
   
/** 
 * \defgroup UART_Exported_Defines
 * \{
 */
#define bHAL_REG_UART_RX(_uart, _buf_len, _idle_ms, _idle_handler)      static uint8_t _uart##buf[_buf_len];\
                                                                    bSECTION_ITEM_REGISTER_RAM(b_hal_uart, bHalUartRxInfo_t, CONCAT_2(rx, _uart))={\
                                                                    .uart = _uart,\
                                                                    .pbuf = _uart##buf,\
                                                                    .buf_len = _buf_len,\
                                                                    .idle_threshold = _idle_ms,\
                                                                    .handler = _idle_handler,\
                                                                    .index = 0,\
                                                                    .l_tick = 0,\
                                                                    .l_index = 0}
/**
 * \}
 */
   
/** 
 * \defgroup UART_Exported_Functions
 * \{
 */
void bHalUartSend(bHalUartNumber_t uart, uint8_t *pbuf, uint16_t len);
void bHalUartRxIRQ_Handler(bHalUartNumber_t uart, uint8_t dat);
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


