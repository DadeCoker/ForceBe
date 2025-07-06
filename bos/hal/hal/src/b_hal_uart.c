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
extern UART_HandleTypeDef huart1;

static bHalUartRxInfo_t  bHalUartRxInfo[B_HAL_UART_NUMBER];
static volatile uint8_t bHalUartInitFlag = 0;
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
        default:
            break;
    }
}

void bHalUartRegIdleCallback(uint8_t no, pUartIdleCallback f)
{
    if(no >= B_HAL_UART_NUMBER)
    {
        return;
    }
    bHalUartRxInfo[no].fCallback = f;
}

void bHalUartDetectIdle()
{
    uint32_t c_tick = bHalGetTick();
    uint8_t i = 0;
    for(i = 0;i < B_HAL_UART_NUMBER;i++)
    {
        if(bHalUartRxInfo[i].fCallback == NULL)
        {
            continue;
        }
        if(c_tick - bHalUartRxInfo[i].l_tick >= 3 && bHalUartRxInfo[i].index > 0)
        {
            bHalUartRxInfo[i].fCallback((uint8_t *)bHalUartRxInfo[i].buf, bHalUartRxInfo[i].index);
            bHalUartRxInfo[i].index = 0;
        }
    }
}



/**
 * \brief Rx Transfer completed callbacks.
 * \param no Uart number \ref bHalUartNumber_t
 */ 
void bHalUartRxIrqCallback(uint8_t no, uint8_t dat)
{
    if(bHalUartInitFlag == 0)
    {
        memset(bHalUartRxInfo, 0, sizeof(bHalUartRxInfo));
        bHalUartInitFlag = 1;
    }
    if(no >= B_HAL_UART_NUMBER)
    {
        return;
    }
    if(bHalUartRxInfo[no].index < UART_BUF_LEN)
    {
        bHalUartRxInfo[no].buf[bHalUartRxInfo[no].index] = dat;
        bHalUartRxInfo[no].index += 1;
    }
    bHalUartRxInfo[no].l_tick = bHalGetTick();
    
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


