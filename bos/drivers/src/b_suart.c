/**
 *!
 * \file        b_suart.c
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
   
/*Includes ----------------------------------------------*/
#include "b_suart.h" 
#include "b_hal.h"
#include <string.h>  
/** 
 * \addtogroup B_DRIVER
 * \{
 */

/** 
 * \addtogroup SUART
 * \{
 */

/** 
 * \defgroup SUART_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SUART_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SUART_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SUART_Private_Variables
 * \{
 */

SUART_Driver_t SUART_Driver = 
{
    .init = SUART_Init,
};

static S_RXInfo_t S_RXInfo = {
    .status = S_RX_S_NULL,
    .c_bits = 0,
    .count = 0,
    .revf = 0,
    .idle_flag = 0,
}; 


static volatile uint8_t TxDelayFlag = 0; 
/**
 * \}
 */
   
/** 
 * \defgroup SUART_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SUART_Private_Functions
 * \{
 */

static void _S_UartStatusReset()
{
    S_RXInfo.count = 0;
    S_RXInfo.idle_flag = 0;
}

static void _S_UartTxDelay()
{
    TxDelayFlag = 0;
    while(TxDelayFlag == 0);
}

static int _S_UartSendByte(uint8_t byte)
{
    uint8_t i = 0;
    _S_UartTxDelay();
    S_TX_PIN_RESET();
    _S_UartTxDelay();
    for(i = 0;i < 8;i++)
    {
        if(byte & 0x01)
        {
            S_TX_PIN_SET();
        }
        else
        {
            S_TX_PIN_RESET();
        }
        _S_UartTxDelay();
        byte >>= 1;
    }
    S_TX_PIN_SET();
    return 0;
}

static int _S_UartSendBuf(uint32_t off, uint8_t *pbuf,uint16_t len)
{
    if(pbuf == NULL)
    {
        return -1;
    }
    uint16_t i = 0;
    for(i = 0;i < len;i++)
    {
        _S_UartSendByte(pbuf[i]);
    }
    return 0;
}

static int _S_UartRead(uint32_t off, uint8_t *pbuf, uint16_t len)
{
    if(pbuf == NULL)
    {
        return -1;
    }
    if(S_RXInfo.idle_flag)
    {
        if(len >= S_RXInfo.count)
        {
            len = S_RXInfo.count;
        }
        memcpy(pbuf, S_RXInfo.buf, len);
        _S_UartStatusReset();
        return len;
    }
    return -1;
}

static int _SUartNullF()
{
    return 0;
}

static int _SUartCtl(uint8_t cmd, void * param)
{
    return 0;
}

/**
 * \}
 */
   
/** 
 * \addtogroup SUART_Exported_Functions
 * \{
 */

/**
 * \brief TX Timer handler
 */
void S_UartTxTimerHandler()
{
    TxDelayFlag = 1;
} 

/**
 * \brief RX Start
 */
void S_UartRXStart()
{
    if(S_RXInfo.status == S_RX_S_NULL)
    {
        S_RXInfo.status = S_RX_S_START;
        S_RXInfo.c_bits = 0;
        S_RXInfo.byte = 0; 
        S_RXInfo.idle_count = 0;
        S_RXInfo.idle_flag = 0;
    }
}

/**
 * \brief RX Timer handler
 */
void S_UartRxTimerHandler()
{
    if(S_RXInfo.status == S_RX_S_START)
    {
        S_RXInfo.status = S_RX_S_ING;
    }
    else if(S_RXInfo.status == S_RX_S_ING)
    {
        S_RXInfo.byte >>= 1;
        if(S_RX_PIN_READ() == GPIO_PIN_SET)
        {
            S_RXInfo.byte |= 0x80;
        }
        S_RXInfo.c_bits += 1;
        if(S_RXInfo.c_bits >= 8)
        {
            if(S_RXInfo.idle_flag)
            {
                _S_UartStatusReset();
            }
            if(S_RXInfo.count < 128)
            {
                S_RXInfo.buf[S_RXInfo.count] = S_RXInfo.byte;
                S_RXInfo.count += 1;
            }
            S_RXInfo.revf = 1;
            S_RXInfo.status = S_RX_S_NULL;
        }
    }   
    else if(S_RXInfo.status == S_RX_S_NULL)
    {
        if(S_RXInfo.revf == 1 && S_RXInfo.idle_flag == 0)
        {
            S_RXInfo.idle_count += 1;
            if(S_RXInfo.idle_count > 3)
            {
                S_RXInfo.revf = 0;
                S_RXInfo.idle_flag = 1;
            }
        }
    }
}

int SUART_Init()
{
    SUART_Driver.close = _SUartNullF;
    SUART_Driver.open = _SUartNullF;
    SUART_Driver.ctl = _SUartCtl;
    SUART_Driver.read = _S_UartRead;
    SUART_Driver.write = _S_UartSendBuf;
    return 0;
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


/************************ Copyright (c) 2019 Bean *****END OF FILE****/


