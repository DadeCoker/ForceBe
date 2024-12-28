/**
 *!
 * \file        b_xm128.c
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
#include "b_xm128.h"  
#if _XMODEM128_ENABLE
/** 
 * \addtogroup BABYOS
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
    return 0;
}

int bXmodem128Start()
{
    if(pSendByte == NULL)
    {
        return -1;
    }
    pSendByte(XMODEM128_NAK);
    return 0;
}

int bXmodem128Stop()
{
    if(pSendByte == NULL)
    {
        return -1;
    }    
    pSendByte(XMODEM128_CAN);
    return 0; 
}



int bXmodem128Parse(uint8_t *pbuf, uint8_t len)
{
    uint8_t check;
    static uint8_t num = 0;
    bXmodem128Info_t *pxm = (bXmodem128Info_t *)pbuf;
    if(pbuf == NULL || pCallback == NULL || pSendByte == NULL)
    {
        return -1;
    }
    if(len == 1 && *pbuf == XMODEM128_EOT)
    {
        pCallback(0, NULL);
        pSendByte(XMODEM128_ACK);
        return 0;
    }
    
    if(len != sizeof(bXmodem128Info_t))
    {
        goto xm_error;
    }
    
    if(pxm->soh != XMODEM128_SOH || (pxm->number + pxm->xnumber) != 0xff)
    {
        goto xm_error;
    }

    check = _bXmodem128CalCheck(pbuf, len - 1);
    if(check != pxm->check)
    {
        goto xm_error;
    }
    
    if(pxm->number == 0x1 && num != 0x1)
    {
        num = 1;
    }
    
    if(num == pxm->number)
    {
        pCallback(num, pxm->dat);
        pSendByte(XMODEM128_ACK);
        num += 1;
        return 0;
    }
    else
    {
        pSendByte(XMODEM128_CAN);
        return -1;
    }

xm_error:
    pSendByte(XMODEM128_NAK);
    return -1;
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

