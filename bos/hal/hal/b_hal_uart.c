/**
 *!
 * \file        b_hal_uart.c
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
   
/*Includes ----------------------------------------------*/
#include "b_hal.h" 
#include "b_utils.h"
#include <string.h>
/** 
 * \addtogroup B_HAL
 * \{
 */

/** 
 * \addtogroup UART
 * \{
 */

/** 
 * \defgroup UART_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup UART_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup UART_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup UART_Private_Variables
 * \{
 */
static uint8_t  bHalUart1Buffer[UART_1_RXBUF_LEN];
static uint8_t  bHalUart2Buffer[UART_2_RXBUF_LEN];
 
static bHalUartRxInfo_t  bHalUartRxInfo[B_HAL_UART_NUMBER] = {
    [B_HAL_UART_1] = {
        .pbuf = bHalUart1Buffer,
        .l_tick = 0,
        .index = 0,
        .max_len = UART_1_RXBUF_LEN,
    },
    [B_HAL_UART_2] = {
        .pbuf = bHalUart2Buffer,
        .l_tick = 0,
        .index = 0,
        .max_len = UART_2_RXBUF_LEN,
    },    
    // add more ...
};

/**
 * \}
 */
   
/** 
 * \defgroup UART_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup UART_Private_Functions
 * \{
 */


/**
 * \}
 */
   
/** 
 * \addtogroup UART_Exported_Functions
 * \{
 */

void bHalUartSend(uint8_t no, uint8_t *pbuf, uint16_t len)
{
    switch(no)
    {
        case B_HAL_UART_1:
            HAL_UART_Transmit(&huart1, pbuf, len, 0xfff);
            break;
        case B_HAL_UART_2:
    
            break;        
        default:
            break;
    }
}

/**
 * \brief Uart idle detection callbacks
 * \param no Uart number \ref bHalUartNumber_t
 * \param pbuf Pointer to the data buffer
 * \param len Amount of data in the buffer
 */
__weak void bHalUartIdleCallback(uint8_t no, uint8_t *pbuf, uint16_t len)
{
    ;
}

/**
 * \brief Determine idle events. Called in bExec()
 */
void bHalUartDetectIdle()
{
    uint32_t c_tick = bUtilGetTick();
    uint8_t i = 0;
    for(i = 0;i < B_HAL_UART_NUMBER;i++)
    {
        if(c_tick - bHalUartRxInfo[i].l_tick >= 3 && bHalUartRxInfo[i].index > 0)
        {
            bHalUartIdleCallback(i, bHalUartRxInfo[i].pbuf, bHalUartRxInfo[i].index);
            bHalUartRxInfo[i].index = 0;
        }
    }
}



/**
 * \brief This function handles Uart rx 1Byte interrupts.
 * \param no Uart number \ref bHalUartNumber_t
 * \param dat The received data
 */ 
void bHalUartRxIRQ_Handler(uint8_t no, uint8_t dat)
{
    if(no >= B_HAL_UART_NUMBER)
    {
        return;
    }
    if(bHalUartRxInfo[no].index < bHalUartRxInfo[no].max_len)
    {
        bHalUartRxInfo[no].pbuf[bHalUartRxInfo[no].index] = dat;
        bHalUartRxInfo[no].index += 1;
    }
    bHalUartRxInfo[no].l_tick = bUtilGetTick(); 
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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/


