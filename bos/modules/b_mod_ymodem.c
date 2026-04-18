/**
 *!
 * \file        b_mod_ymodem.c
 * \version     v0.0.1
 * \date        2020/02/12
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
#include "b_mod_ymodem.h"  
#if _YMODEM_ENABLE
#include "b_utils.h"
#include "string.h"
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup MODULES
 * \{
 */

/** 
 * \addtogroup YMODEM
 * \{
 */

/** 
 * \defgroup YMODEM_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup YMODEM_Private_Defines
 * \{
 */
#define YM_S_NULL           0
#define YM_S_WAIT_NAME      1
#define YM_S_WAIT_START     2
#define YM_S_WAIT_DATA      3
#define YM_S_WAIT_END       4
/**
 * \}
 */
   
/** 
 * \defgroup YMODEM_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup YMODEM_Private_Variables
 * \{
 */

static pymcb_t pCallback = NULL;
static pymsend pSendByte = NULL;

static uint8_t YmodemStat = YM_S_NULL;
static uint32_t YmTick = 0;

static bPollingFunc_t YmodemPollFunc = {
    .pPollingFunction = NULL,
};
/**
 * \}
 */
   
/** 
 * \defgroup YMODEM_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup YMODEM_Private_Functions
 * \{
 */
static uint16_t _bYmodemCalCheck(uint8_t *pbuf, uint16_t len)
{
    uint16_t crc = 0;
    uint16_t i, j;

    for (i = 0 ; i < len; i++) 
    {
        crc = crc ^ pbuf[i] << 8;
        for (j = 0; j < 8; j++) 
        {
            if (crc & 0x8000)
            {
                crc = crc << 1 ^ 0x1021;
            } 
            else 
            {
                crc = crc << 1;
            }
        }
    }
    return crc;
}   


static int _bYmodemISValid(uint8_t *pbuf, uint16_t len)
{
    bYmodem128Info_t *ptmp = (bYmodem128Info_t *)pbuf;
    static uint16_t check;
    
    if(pbuf == NULL || (len != sizeof(bYmodem128Info_t) && len != sizeof(bYmodem1kInfo_t)))
    {
        return -1;
    }
    
    if((ptmp->soh != YMODEM_SOH && ptmp->soh != YMODEM_STX)|| (ptmp->number + ptmp->xnumber) != 0xff)
    {
        return -1;
    }
    
    check = _bYmodemCalCheck(pbuf + 3, len - 5);
    if(((check & 0xff00) >> 8) != pbuf[len - 2] || (check & 0xff) != pbuf[len - 1])
    {
        return -1;
    }
    return ptmp->soh;
}


static int _bYmodemStart()
{
    if(pSendByte == NULL)
    {
        return -1;
    }
    pSendByte(YMODEM_C);
    YmTick = bUtilGetTick();
    YmodemStat = YM_S_WAIT_START;
    return 0;
}

static int _bYmodemEnd()
{
    if(pSendByte == NULL)
    {
        return -1;
    }
    pSendByte(YMODEM_C);
    YmTick = bUtilGetTick();
    YmodemStat = YM_S_WAIT_END;
    return 0;
}

static void _bYmodemTimeout()
{
    static uint8_t n_count = 0;
    static uint8_t s_count = 0;
    static uint8_t e_count = 0;
    if(YmodemStat == YM_S_NULL)
    {
        return;
    }
    if(bUtilGetTick() - YmTick > MS2TICKS(2000))
    {
        YmTick = bUtilGetTick();
        if(YmodemStat == YM_S_WAIT_NAME && n_count < 3)
        {
            YmodemStat = YM_S_NULL;
            bYmodemStart();
            n_count += 1;
        }
        else if(YmodemStat == YM_S_WAIT_START && s_count < 3)
        {
            _bYmodemStart();
            s_count += 1;
        }
        else if(YmodemStat == YM_S_WAIT_END && e_count < 3)
        {
            _bYmodemEnd();
            e_count += 1;
        }
        else
        {
            if(pCallback != NULL)
            {
                pCallback(0, 0, NULL, 0);
            }
            s_count = 0;
            n_count = 0;
            e_count = 0;
            YmodemStat = YM_S_NULL;
        }
    }
}


/**
 * \}
 */
   
/** 
 * \addtogroup YMODEM_Exported_Functions
 * \{
 */

int bYmodemInit(pymcb_t fcb, pymsend fs)
{
    if(fcb == NULL || fs == NULL)
    {
        return -1;
    }
    pCallback = fcb;
    pSendByte = fs;
    YmodemStat = YM_S_NULL;
    if(YmodemPollFunc.pPollingFunction == NULL)
    {
        YmodemPollFunc.pPollingFunction = _bYmodemTimeout;
        bRegistPollingFunc(&YmodemPollFunc);
    }
    return 0;
}


int bYmodemStart()
{
    if(pSendByte == NULL)
    {
        return -1;
    }
    if(YmodemStat == YM_S_NULL)
    {
        pSendByte(YMODEM_C);
        YmTick = bUtilGetTick();
        YmodemStat = YM_S_WAIT_NAME;
    }
    return 0;
}

int bYmodemStop()
{
    if(pSendByte == NULL)
    {
        return -1;
    }    
    if(YmodemStat != YM_S_NULL)
    {
        pSendByte(YMODEM_CAN);
        YmodemStat = YM_S_NULL;
    }
    return 0; 
}



int bYmodemParse(uint8_t *pbuf, uint16_t len)
{
    static uint8_t num = 0;
    static uint16_t f_num = 0;
    int t;
    bYmodem128Info_t *pxm = (bYmodem128Info_t *)pbuf;
    
    if(pbuf == NULL || pCallback == NULL || pSendByte == NULL || YmodemStat == YM_S_NULL)
    {
        return -1;
    }
    YmTick = bUtilGetTick();
    
    
    if(YmodemStat == YM_S_WAIT_NAME)
    {
        if(_bYmodemISValid(pbuf, len) == YMODEM_SOH)
        {
            if(pxm->number == 0)
            {
                pCallback(YMODEM_FILENAME, 0, pxm->dat, strlen((const char *)pxm->dat));
                pSendByte(YMODEM_ACK);
                YmodemStat = YM_S_WAIT_START;
            }
        }
    }
    else if(YmodemStat == YM_S_WAIT_START)
    {
        t = _bYmodemISValid(pbuf, len);
        if(t > 0 && pxm->number == 1)
        {
            YmodemStat = YM_S_WAIT_DATA;
            num = 1;
            f_num = 0;
        }
    }
    
    if(YmodemStat == YM_S_WAIT_DATA)
    {
        t = _bYmodemISValid(pbuf, len);
        if(t > 0)
        {
            if(pxm->number == num)
            {
                if(t == YMODEM_SOH)
                {
                    pCallback(YMODEM_FILEDATA, f_num, pxm->dat, 128);
                }
                else if(t == YMODEM_STX)
                {
                    pCallback(YMODEM_FILEDATA, f_num, pxm->dat, 1024);
                }
                num += 1;
                f_num += 1;
                pSendByte(YMODEM_ACK);
            }
            else
            {
                pCallback(0, 0, NULL, 0);
                pSendByte(YMODEM_CAN);
                YmodemStat = YM_S_NULL;
                return -1;
            }
        }
        else if(len == 1 && *pbuf == YMODEM_EOT)
        {
            pSendByte(YMODEM_ACK);
            YmodemStat = YM_S_WAIT_END;
        }
        else
        {
            pSendByte(YMODEM_NAK);
        }
    }
    else if(YmodemStat == YM_S_WAIT_END)
    {
        pCallback(YMODEM_FILEDATA, f_num, NULL, 0);
        pSendByte(YMODEM_ACK);
        YmodemStat = YM_S_NULL;
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

