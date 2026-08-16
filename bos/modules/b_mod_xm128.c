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
static bXmodem128Info_t bXmodem128Info = {
    .cb = NULL,
    .send_f = NULL,
    .statu = XM_S_NULL,
    .tt_count = 0,
    .next_number = 0,
    .frame_number = 0,
    .tick = 0
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
    bXmodem128Struct_t *ptmp = (bXmodem128Struct_t *)pbuf;
    uint8_t check;
    if(pbuf == NULL || len != sizeof(bXmodem128Struct_t))
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
    if(bXmodem128Info.statu == XM_S_NULL)
    {
        return;
    }
    if(bUtilGetTick() - bXmodem128Info.tick >= MS2TICKS(1000))
    {
        bXmodem128Info.tick = bUtilGetTick();
        if(bXmodem128Info.tt_count >= 10)
        {
            bXmodem128Info.tt_count = 0;
            bXmodem128Info.statu = XM_S_NULL;
            if(bXmodem128Info.cb)
            {
                bXmodem128Info.cb(0, NULL);
            }
        }
        else
        {
            bXmodem128Info.send_f(XMODEM128_NAK);
            bXmodem128Info.tt_count += 1;
        }
    }
}

BOS_REG_POLLING_FUNC(_bXmodem128Timeout);

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
    bXmodem128Info.cb = fcb;
    bXmodem128Info.send_f = fs;
    bXmodem128Info.statu = XM_S_NULL;
    return 0;
}


int bXmodem128Start()
{
    if(bXmodem128Info.send_f == NULL)
    {
        return -1;
    }
    if(bXmodem128Info.statu == XM_S_NULL)
    {
        bXmodem128Info.send_f(XMODEM128_NAK);
        bXmodem128Info.tick = bUtilGetTick();
        bXmodem128Info.next_number = 0;
        bXmodem128Info.frame_number = 0;
        bXmodem128Info.statu = XM_S_WAIT_START;
    }
    return 0;
}

int bXmodem128Stop()
{
    if(bXmodem128Info.send_f == NULL)
    {
        return -1;
    }    
    if(bXmodem128Info.statu != XM_S_NULL)
    {
        bXmodem128Info.send_f(XMODEM128_CAN);
        bXmodem128Info.statu = XM_S_NULL;
    }
    return 0; 
}



int bXmodem128Parse(uint8_t *pbuf, uint8_t len)
{
    bXmodem128Struct_t *pxm = (bXmodem128Struct_t *)pbuf;
    if(pbuf == NULL || bXmodem128Info.cb == NULL || bXmodem128Info.send_f == NULL || bXmodem128Info.statu == XM_S_NULL)
    {
        return -1;
    }
    bXmodem128Info.tick = bUtilGetTick();
    bXmodem128Info.tt_count = 0;
    
    if(bXmodem128Info.statu == XM_S_WAIT_START)
    {
        if(_bXmodem128ISValid(pbuf, len) == 0)
        {
            if(pxm->number == 1)
            {
                bXmodem128Info.statu = XM_S_WAIT_DATA;
                bXmodem128Info.next_number = 1;
                bXmodem128Info.frame_number = 0;
            }
        }
    }
    
    if(bXmodem128Info.statu == XM_S_WAIT_DATA)
    {
        if(_bXmodem128ISValid(pbuf, len) == 0)
        {
            if(pxm->number == bXmodem128Info.next_number)
            {
                bXmodem128Info.cb(bXmodem128Info.frame_number, pxm->dat);
                bXmodem128Info.next_number += 1;
                bXmodem128Info.frame_number += 1;
                bXmodem128Info.send_f(XMODEM128_ACK);
            }
            else if(pxm->number > bXmodem128Info.next_number)
            {
                bXmodem128Info.cb(0, NULL);
                bXmodem128Info.statu = XM_S_NULL;
                bXmodem128Info.send_f(XMODEM128_CAN);
            }
            else
            {
                bXmodem128Info.send_f(XMODEM128_ACK);
            }
        }
        else if(len == 1 && *pbuf == XMODEM128_EOT)
        {
            bXmodem128Info.cb(bXmodem128Info.frame_number, NULL);
            bXmodem128Info.statu = XM_S_NULL;
            bXmodem128Info.send_f(XMODEM128_ACK);
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

