/**
 *!
 * \file        b_drv_f8l10d.c
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
   
/*Includes ----------------------------------------------*/
#include "b_drv_f8l10d.h"
#if (_ASYN_TX_ENABLE == 1 && _AT_ENABLE == 1)
#include "b_at.h"
#include "b_asyntx.h"
#include "b_at.h"
#include <string.h>
#include <stdio.h>
#include "b_utils.h"
#include "b_hal.h"
/** 
 * \addtogroup B_DRIVER
 * \{
 */

/** 
 * \addtogroup F8L10D
 * \{
 */

/** 
 * \defgroup F8L10D_Private_TypesDefinitions
 * \{
 */

typedef struct
{
    uint16_t frq;
    uint16_t nid;
    uint16_t pid;
    uint16_t gwid;
    uint8_t mode;
    uint8_t speed;
    uint8_t dbi;
}_F8L10D_Config_t;

typedef enum
{
    I_FRQ,
    I_NID,
    I_PID,
    I_GWID,
    I_MODE,
    I_SPEED,
    I_DBI,
    I_NUMBER
}_F8L10D_Item_t;

/**
 * \}
 */
   
/** 
 * \defgroup F8L10D_Private_Defines
 * \{
 */
 
/**
 * \}
 */
   
/** 
 * \defgroup F8L10D_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup F8L10D_Private_Variables
 * \{
 */
 
bF8L10D_Driver_t bF8L10D_Driver = {
    .init = bF8L10D_Init,
}; 
 
 
const char *pATTable[I_NUMBER] = {
    "AT+LFR?\r\n",
    "AT+NID?\r\n",
    "AT+PID?\r\n",
    "AT+TID?\r\n",
    "AT+SLE?\r\n",
    "AT+LRS?\r\n", 
    "AT+TPR?\r\n",    
};

const char *pATSetTable[I_NUMBER] = {
    "AT+LFR=%d\r\n",
    "AT+NID=%d\r\n",
    "AT+PID=%d\r\n",
    "AT+TID=%d\r\n",
    "AT+SLE=%d\r\n",
    "AT+LRS=%d\r\n", 
    "AT+TPR=%d\r\n",    
};

static int AT_No = -1, ATX_No = -1;
static uint8_t F8L10D_Buf[200];

/**
 * \}
 */
   
/** 
 * \defgroup F8L10D_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup F8L10D_Private_Functions
 * \{
 */
static void _F8L10D_UartSend(uint8_t *pbuf, uint16_t len)
{
    bHalUartSend(HAL_F8L10D_UART, pbuf, len);
}

static int _F8L10D_EnterATMode(int at_no)
{
    int i = 0;
    bAT_ExpectedResp_t bAT_ExpectedResp;
    bAT_ExpectedResp.timeout = 1000;
    for(i = 0;i < 6;i++)
    {
        if(0 == bAT_Write(at_no, &bAT_ExpectedResp, "+++"))
        {
            if(0 == strncmp((char *)bAT_ExpectedResp.pResp, "\r\nOK\r\n", strlen("\r\nOK\r\n")))
            {
                return 0;
            }
        }
    }
    return -1;
}

static int _F8L10D_ExitATMode(int at_no)
{
    int i = 0;
    bAT_ExpectedResp_t bAT_ExpectedResp;
    bAT_ExpectedResp.timeout = 1000;

    for(i = 0;i < 3;i++)
    {
        if(0 == bAT_Write(at_no, &bAT_ExpectedResp, "AT+ESC\r\n"))
        {
            if(0 == strncmp((char *)bAT_ExpectedResp.pResp, "\r\nOK\r\n", strlen("\r\nOK\r\n")))
            {
                return 0;
            }
        }
    }
    return -1;
}

static int _F8L10D_GetInfo(int at_no, _F8L10D_Item_t it)
{
    int i = 0;
    int retval = -1, tmp;
    bAT_ExpectedResp_t bAT_ExpectedResp;
    bAT_ExpectedResp.timeout = 1000;
    for(i = 0;i < 3;i++)
    {
        if(0 == bAT_Write(at_no, &bAT_ExpectedResp, pATTable[it]))
        {
            if(it == I_FRQ)
            {
                retval = sscanf((char *)bAT_ExpectedResp.pResp, "\r\n+LFR:%d\r\nOK\r\n", &tmp);
            }
            else if(it == I_NID)
            {
                retval = sscanf((char *)bAT_ExpectedResp.pResp, "\r\n+NID:%d\r\nOK\r\n", &tmp);
            }
            else if(it == I_PID)
            {
                retval = sscanf((char *)bAT_ExpectedResp.pResp, "\r\n+PID:%d\r\nOK\r\n", &tmp);
            }
            else if(it == I_GWID)
            {
                retval = sscanf((char *)bAT_ExpectedResp.pResp, "\r\n+TID:%d\r\nOK\r\n", &tmp);
            }
            else if(it == I_MODE)
            {
                retval = sscanf((char *)bAT_ExpectedResp.pResp, "\r\n+SLE:%d\r\nOK\r\n", &tmp);
            }
            else if(it == I_SPEED)
            {
                retval = sscanf((char *)bAT_ExpectedResp.pResp, "\r\n+LRS:%d\r\nOK\r\n", &tmp);
            }
            else if(it == I_DBI)
            {
                retval = sscanf((char *)bAT_ExpectedResp.pResp, "\r\n+TPR:%d\r\nOK\r\n", &tmp);
            }            
        }
        if(retval > 0)
        {
            b_log("%s %d \r\n", pATTable[it], tmp);
            return tmp;
        }
    }
    return -1;
}


static int _F8L10D_SetInfo(int at_no, _F8L10D_Item_t it, int dat)
{
    int i = 0;

    bAT_ExpectedResp_t bAT_ExpectedResp;
    bAT_ExpectedResp.timeout = 1000;
    for(i = 0;i < 3;i++)
    {
        if(0 == bAT_Write(at_no, &bAT_ExpectedResp, pATSetTable[it], dat))
        {
            if(0 == strncmp((char *)bAT_ExpectedResp.pResp, "\r\nOK\r\n", strlen("\r\nOK\r\n")))
            {
                return 0;
            }
        }
    }
    return -1;
}

int tmptable[I_NUMBER];

static int _F8L10D_Init(int at_no, _F8L10D_Config_t config)
{
    int retval = -1;
    int i = 0, tmp;
    retval = _F8L10D_EnterATMode(at_no);       //enter AT mode
    if(retval < 0)
    {
        return -1;
    }
    /**********************************************************/  //config LoRa module
    tmp = _F8L10D_GetInfo(at_no, I_FRQ);
    if(tmp >= 0 && tmp != config.frq)
    {
        _F8L10D_SetInfo(at_no, I_FRQ, config.frq);
    }

    tmp = _F8L10D_GetInfo(at_no, I_PID);
    if(tmp >= 0 && tmp != config.pid)
    {
        _F8L10D_SetInfo(at_no, I_PID, config.pid);
    }

    tmp = _F8L10D_GetInfo(at_no, I_GWID);
    if(tmp >= 0 && tmp != config.gwid)
    {
        _F8L10D_SetInfo(at_no, I_GWID, config.gwid);
    }

    tmp = _F8L10D_GetInfo(at_no, I_MODE);
    if(tmp >= 0 && tmp != config.mode)
    {
        _F8L10D_SetInfo(at_no, I_MODE, config.mode);
    }

    tmp = _F8L10D_GetInfo(at_no, I_SPEED);
    if(tmp >= 0 && tmp != config.speed)
    {
        _F8L10D_SetInfo(at_no, I_SPEED, config.speed);
    }

    tmp = _F8L10D_GetInfo(at_no, I_DBI);
    if(tmp >= 0 && tmp != config.dbi)
    {
        _F8L10D_SetInfo(at_no, I_DBI, config.dbi);
    }    
    /**********************************************************/
    
    for(i = 0;i < I_NUMBER;i++)                                //Read back
    {
        tmp = _F8L10D_GetInfo(at_no, (_F8L10D_Item_t)i);
        if(tmp >= 0)
        {
            tmptable[i] = tmp;
        }
    }
    
    _F8L10D_ExitATMode(at_no);
    
    return 0;
}


static void _F8L10D_Reset()
{
    bHalGPIO_WritePin(HAL_F8L10D_SLEEP_PORT, HAL_F8L10D_SLEEP_PIN, 0);
    bHalGPIO_WritePin(HAL_F8L10D_RESET_PORT, HAL_F8L10D_SLEEP_PIN, 0);
    bHalDelayMS(250);
    bHalGPIO_WritePin(HAL_F8L10D_RESET_PORT, HAL_F8L10D_SLEEP_PIN, 1);
    bHalDelayMS(250);
    bHalGPIO_WritePin(HAL_F8L10D_SLEEP_PORT, HAL_F8L10D_SLEEP_PIN, 0);
    bHalDelayMS(1500);
}

static void _F8L10D_TXDoneCB()
{
    bAsyntxCplCallback(ATX_No);
}


static void _F8L10D_RXCplCB(uint8_t *pbuf, uint16_t len)
{
    bAT_Read(ATX_No, pbuf, len);
}

/**************************************************************************************************driver interface*****/

static int _F8L10D_Sleep()
{
    bHalGPIO_WritePin(HAL_F8L10D_SLEEP_PORT, HAL_F8L10D_SLEEP_PIN, 0);
    return 0;
}

static int _F8L10D_Wakeup()
{
    bHalGPIO_WritePin(HAL_F8L10D_SLEEP_PORT, HAL_F8L10D_SLEEP_PIN, 1);
    bHalDelayMS(90);
    return 0;
}

static int _F8L10D_Read(uint32_t off, uint8_t *pbuf, uint16_t len)
{
    if(off >= 200)
    {
        return -1;
    }
    len = (200 - off > len) ? len : (200 - off);
    memcpy(pbuf, &F8L10D_Buf[off], len);
    return len;
}

static int _F8L10D_Write(uint32_t off, uint8_t *pbuf, uint16_t len)
{
    len = (len <= 200) ? len : 200;
    if(0 > bAsyntxRequest(ATX_No, pbuf, len, BASYN_TX_REQ_L0))
    {
        return -1;
    }
    return len;
}

/**
 * \}
 */
   
/** 
 * \addtogroup F8L10D_Exported_Functions
 * \{
 */
int bF8L10D_Init()
{  
    _F8L10D_Config_t  _F8L10D_Config = 
    {
        .frq = 433,
        .nid = 1,
        .pid = 0,
        .gwid = 60000,
        .mode = 0,
        .speed = 4,
        .dbi = 20,
    };
   
    AT_No = bAT_Regist(_F8L10D_UartSend);
    if(AT_No < 0)
    {
        return -1;
    }
    
    ATX_No = bAsyntxRegist(_F8L10D_UartSend, 1000);
    if(ATX_No < 0)
    {
        return -1;
    }   
    
    _F8L10D_Reset();
    bHalGPIO_WritePin(HAL_F8L10D_SLEEP_PORT, HAL_F8L10D_SLEEP_PIN, 1);
    bHalDelayMS(90);
    if(0 > _F8L10D_Init(AT_No, _F8L10D_Config))
    {
        bHalGPIO_WritePin(HAL_F8L10D_SLEEP_PORT, HAL_F8L10D_SLEEP_PIN, 0);
        return -1;
    }
    bHalGPIO_WritePin(HAL_F8L10D_SLEEP_PORT, HAL_F8L10D_SLEEP_PIN, 0);
    bF8L10D_Driver.close = _F8L10D_Sleep;
    bF8L10D_Driver.open = _F8L10D_Wakeup;
    bF8L10D_Driver.read = _F8L10D_Read;
    bF8L10D_Driver.write = _F8L10D_Write;
    bF8L10D_Driver.ctl = NULL;
    
    bHalGPIO_RegEXTICallback(HAL_F8L10D_TXD_PIN, _F8L10D_TXDoneCB);
    bHalUartRegIdleCallback(HAL_F8L10D_UART, _F8L10D_RXCplCB);
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
#endif
/************************ Copyright (c) 2019 Bean *****END OF FILE****/

