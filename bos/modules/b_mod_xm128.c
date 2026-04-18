/**
 *!
 * \file        b_mod_xm128.c
 * \version     v0.0.1
 * \date        2020/02/03
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
#include "b_mod_xm128.h"  
#if _XMODEM128_ENABLE
#include "b_utils.h"
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup MODULES
 * \{
 */

/** 
 * \addtogroup XMODEM128
 * \{
 */

/** 
 * \defgroup XMODEM128_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup XMODEM128_Private_Defines
 * \{
 */
#define XM_S_NULL           0
#define XM_S_WAIT_START     1
#define XM_S_WAIT_DATA      2

/**
 * \}
 */
   
/** 
 * \defgroup XMODEM128_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup XMODEM128_Private_Variables
 * \{
 */

static pcb_t pCallback = NULL;
static psend pSendByte = NULL;

static uint8_t Xmodem128Stat = XM_S_NULL;
static uint32_t XmTick = 0;

static bPollingFunc_t XmodemPollFunc = {
    .pPollingFunction = NULL,
};


/**
 * \}
 */
   
/** 
 * \defgroup XMODEM128_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup XMODEM128_Private_Functions
 * \{
 */
static uint8_t _bXmodem128CalCheck(uint8_t *pbuf, uint8_t len)
{
    uint8_t tmp = 0;
    while(len--)
    {
        tmp += *pbuf++;
    }
    return tmp;
}   

static int _bXmodem128ISValid(uint8_t *pbuf, uint8_t len)
{
    bXmodem128Info_t *ptmp = (bXmodem128Info_t *)pbuf;
    uint8_t check;
    if(pbuf == NULL || len != sizeof(bXmodem128Info_t))
    {
        return -1;
    }
    
    if(ptmp->soh != XMODEM128_SOH || (ptmp->number + ptmp->xnumber) != 0xff)
    {
        return -1;
    }
    
    check = _bXmodem128CalCheck(pbuf, len - 1);
    if(check != ptmp->check)
    {
        return -1;
    }
    return 0;
}

static void _bXmodem128Timeout()
{
    static uint8_t s_count = 0;
    if(Xmodem128Stat == XM_S_NULL)
    {
        return;
    }
    if(bUtilGetTick() - XmTick > MS2TICKS(3000))
    {
        XmTick = bUtilGetTick();
        if(Xmodem128Stat == XM_S_WAIT_START && s_count < 3)
        {
            Xmodem128Stat = XM_S_NULL;
            bXmodem128Start();
            s_count += 1;
        }
        else
        {
            if(pCallback != NULL)
            {
                pCallback(0, NULL);
            }
            s_count = 0;
            Xmodem128Stat = XM_S_NULL;
        }
    }
}


/**
 * \}
 */
   
/** 
 * \addtogroup XMODEM128_Exported_Functions
 * \{
 */

int bXmodem128Init(pcb_t fcb, psend fs)
{
    if(fcb == NULL || fs == NULL)
    {
        return -1;
    }
    pCallback = fcb;
    pSendByte = fs;
    Xmodem128Stat = XM_S_NULL;
    if(XmodemPollFunc.pPollingFunction == NULL)
    {
        XmodemPollFunc.pPollingFunction = _bXmodem128Timeout;
        bRegistPollingFunc(&XmodemPollFunc);
    }
    return 0;
}


int bXmodem128Start()
{
    if(pSendByte == NULL)
    {
        return -1;
    }
    if(Xmodem128Stat == XM_S_NULL)
    {
        pSendByte(XMODEM128_NAK);
        XmTick = bUtilGetTick();
        Xmodem128Stat = XM_S_WAIT_START;
    }
    return 0;
}

int bXmodem128Stop()
{
    if(pSendByte == NULL)
    {
        return -1;
    }    
    if(Xmodem128Stat != XM_S_NULL)
    {
        pSendByte(XMODEM128_CAN);
        Xmodem128Stat = XM_S_NULL;
    }
    return 0; 
}



int bXmodem128Parse(uint8_t *pbuf, uint8_t len)
{
    static uint8_t num = 0;
    static uint16_t f_num = 0;
    bXmodem128Info_t *pxm = (bXmodem128Info_t *)pbuf;
    if(pbuf == NULL || pCallback == NULL || pSendByte == NULL || Xmodem128Stat == XM_S_NULL)
    {
        return -1;
    }
    XmTick = bUtilGetTick();
    
    if(Xmodem128Stat == XM_S_WAIT_START)
    {
        if(_bXmodem128ISValid(pbuf, len) == 0)
        {
            if(pxm->number == 1)
            {
                Xmodem128Stat = XM_S_WAIT_DATA;
                num = 1;
                f_num = 0;
            }
        }
    }
    
    if(Xmodem128Stat == XM_S_WAIT_DATA)
    {
        if(_bXmodem128ISValid(pbuf, len) == 0)
        {
            if(pxm->number == num)
            {
                pCallback(f_num, pxm->dat);
                pSendByte(XMODEM128_ACK);
                num += 1;
                f_num += 1;
            }
            else
            {
                pCallback(0, NULL);
                pSendByte(XMODEM128_CAN);
                Xmodem128Stat = XM_S_NULL;
                return -1;
            }
        }
        else if(len == 1 && *pbuf == XMODEM128_EOT)
        {
            pCallback(f_num, NULL);
            pSendByte(XMODEM128_ACK);
            Xmodem128Stat = XM_S_NULL;
        }
        else
        {
            pSendByte(XMODEM128_NAK);
        }
    }
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


/**
 * \}
 */
#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/

