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
static bHalUartRxInfo_t bHalUartRxInfoHead = {
    .pnext = NULL,
};

static bPollingFunc_t bHalUartRxPollingFunc;
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
 * \brief Determine idle events. Called in bExec()
 */
static void _bHalUartDetectIdle()
{
    uint32_t c_tick = bUtilGetTick();
    bHalUartRxInfo_t *ptmp = bHalUartRxInfoHead.pnext;
    
    while(ptmp)
    {
        if(ptmp->index > 0 && (c_tick - ptmp->l_tick >= ptmp->idle_threshold) && ptmp->cb != NULL)
        {
            ptmp->cb(ptmp->pbuf, ptmp->index);
            ptmp->index = 0;
        }
    }
}

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
 * \brief Register UART_RX information
 * \param puart_rx Pointer to UART_RX information \ref bHalUartRxInfo_t
 */
int bHalUartRxRegist(bHalUartRxInfo_t *puart_rx)
{
    if(puart_rx == NULL)
    {
        return -1;
    }
    
    if(bHalUartRxInfoHead.pnext == NULL)
    {
        bHalUartRxPollingFunc.pnext = NULL;
        bHalUartRxPollingFunc.pPollingFunction = _bHalUartDetectIdle;
        bRegistPollingFunc(&bHalUartRxPollingFunc);
    }
    
    puart_rx->index = 0;
    puart_rx->pnext = bHalUartRxInfoHead.pnext;
    bHalUartRxInfoHead.pnext = puart_rx;
    return 0;
}



/**
 * \brief This function handles Uart rx 1Byte interrupts.
 * \param no Uart number \ref bHalUartNumber_t
 * \param dat The received data
 */ 
void bHalUartRxIRQ_Handler(uint8_t no, uint8_t dat)
{
    bHalUartRxInfo_t *ptmp = bHalUartRxInfoHead.pnext;
    if(no >= B_HAL_UART_NUMBER)
    {
        return;
    }
    while(ptmp)
    {
        if(ptmp->uart == no)
        {
            ptmp->pbuf[ptmp->index++] = dat;
            ptmp->l_tick = bUtilGetTick();
            break;
        }
        ptmp = ptmp->pnext;
    }
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


