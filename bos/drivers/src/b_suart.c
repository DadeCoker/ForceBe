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

static void _S_UartStatusReset(SUART_Driver_t *pdrv)
{
    ((bSUART_Private_t *)pdrv->_private)->info.count = 0;
    ((bSUART_Private_t *)pdrv->_private)->info.idle_flag = 0;
}

static void _S_UartTxDelay(SUART_Driver_t *pdrv)
{  
    ((bSUART_Private_t *)pdrv->_private)->info.tx_delay_flag = 0;
    while(((bSUART_Private_t *)pdrv->_private)->info.tx_delay_flag == 0);
}

static int _S_UartSendByte(SUART_Driver_t *pdrv, uint8_t byte)
{
    uint8_t i = 0;
    
    bSUART_Private_t *_private;
    _private = pdrv->_private;
    _S_UartTxDelay(pdrv);
    _private->pTxPIN_Control(0);
    _S_UartTxDelay(pdrv);
    for(i = 0;i < 8;i++)
    {
        if(byte & 0x01)
        {
            _private->pTxPIN_Control(1);
        }
        else
        {
            _private->pTxPIN_Control(0);
        }
        _S_UartTxDelay(pdrv);
        byte >>= 1;
    }
    _private->pTxPIN_Control(1);
    return 0;
}


/*******************************************************************************************************driver interface*****/

static int _S_UartSendBuf(uint32_t off, uint8_t *pbuf,uint16_t len)
{
    uint16_t i = 0;
    
    SUART_Driver_t *pdrv;
    if(0 > bDeviceGetCurrentDrv(&pdrv) || pbuf == NULL)
    {
        return -1;
    }        
    
    for(i = 0;i < len;i++)
    {
        _S_UartSendByte(pdrv, pbuf[i]);
    }
    return 0;
}

static int _S_UartRead(uint32_t off, uint8_t *pbuf, uint16_t len)
{
    SUART_Driver_t *pdrv;
    if(0 > bDeviceGetCurrentDrv(&pdrv) || pbuf == NULL)
    {
        return -1;
    }
    
    if(((bSUART_Private_t *)pdrv->_private)->info.idle_flag)
    {
        if(len >= ((bSUART_Private_t *)pdrv->_private)->info.count)
        {
            len = ((bSUART_Private_t *)pdrv->_private)->info.count;
        }
        memcpy(pbuf, ((bSUART_Private_t *)pdrv->_private)->info.buf, len);
        _S_UartStatusReset(pdrv);
        return len;
    }
    return -1;
}


/**
 * \}
 */
   
/** 
 * \addtogroup SUART_Exported_Functions
 * \{
 */

/**
 * \brief RX Start
 */
void S_UartRXStart(SUART_Driver_t *pdrv)
{
    bSUART_Private_t *iface = (bSUART_Private_t *)pdrv->_private;
    if(iface->info.status == S_RX_S_NULL)
    {
        iface->info.status = S_RX_S_START;
        iface->info.c_bits = 0;
        iface->info.byte = 0; 
        iface->info.idle_count = 0;
        iface->info.idle_flag = 0;
    }
}

/**
 * \brief TX Timer handler
 */
void S_UartTxTimerHandler(SUART_Driver_t *pdrv)
{
    ((bSUART_Private_t *)pdrv->_private)->info.tx_delay_flag = 1;
} 

/**
 * \brief RX Timer handler
 */
void S_UartRxTimerHandler(SUART_Driver_t *pdrv)
{
    bSUART_Private_t *iface = (bSUART_Private_t *)pdrv->_private;
    if(iface->info.status == S_RX_S_START)
    {
        iface->info.status = S_RX_S_ING;
    }
    else if(iface->info.status == S_RX_S_ING)
    {
        iface->info.byte >>= 1;
        if(iface->RxPIN_Read())
        {
            iface->info.byte |= 0x80;
        }
        iface->info.c_bits += 1;
        if(iface->info.c_bits >= 8)
        {
            if(iface->info.idle_flag)
            {
                _S_UartStatusReset(pdrv);
            }
            if(iface->info.count < 128)
            {
                iface->info.buf[iface->info.count] = iface->info.byte;
                iface->info.count += 1;
            }
            iface->info.revf = 1;
            iface->info.status = S_RX_S_NULL;
        }
    }   
    else if(iface->info.status == S_RX_S_NULL)
    {
        if(iface->info.revf == 1 && iface->info.idle_flag == 0)
        {
            iface->info.idle_count += 1;
            if(iface->info.idle_count > 3)
            {
                iface->info.revf = 0;
                iface->info.idle_flag = 1;
            }
        }
    }
}


int SUART_Init(SUART_Driver_t *pdrv)
{
    bSUART_Private_t *iface = (bSUART_Private_t *)pdrv->_private;
    iface->info.status = S_RX_S_NULL;
    iface->info.c_bits = 0;
    iface->info.count = 0;
    iface->info.revf = 0;
    iface->info.idle_flag = 0;
    iface->info.tx_delay_flag = 0;
    
    pdrv->close = NULL;
    pdrv->open = NULL;
    pdrv->ctl = NULL;
    pdrv->read = _S_UartRead;
    pdrv->write = _S_UartSendBuf;
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


