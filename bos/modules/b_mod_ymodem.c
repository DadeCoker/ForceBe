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
#include "modules/inc/b_mod_ymodem.h"

#include "b_section.h"

#if (defined(_YMODEM_ENABLE) && (_YMODEM_ENABLE == 1))
#include <string.h>

#include "hal/inc/b_hal.h"

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
typedef struct
{
    uint8_t soh;
    uint8_t number;
    uint8_t xnumber;
    uint8_t dat[128];
    uint8_t crc_h;
    uint8_t crc_l;
} bYmodem128Struct_t;

typedef struct
{
    uint8_t stx;
    uint8_t number;
    uint8_t xnumber;
    uint8_t dat[1024];
    uint8_t crc_h;
    uint8_t crc_l;
} bYmodem1kStruct_t;

typedef struct
{
    pymcb_t  cb;
    pymsend  send_f;
    uint8_t  statu;
    uint8_t  tt_count;
    uint8_t  next_number;
    uint32_t tick;
} bYmodemInfo_t;
/**
 * \}
 */

/**
 * \defgroup YMODEM_Private_Defines
 * \{
 */
#define YM_S_NULL 0
#define YM_S_WAIT_NAME 1
#define YM_S_WAIT_START 2
#define YM_S_WAIT_DATA 3
#define YM_S_WAIT_END 4
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
static bYmodemInfo_t bYmodemInfo = {
    .cb          = NULL,
    .send_f      = NULL,
    .statu       = YM_S_NULL,
    .tt_count    = 0,
    .next_number = 0,
    .tick        = 0,
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

    for (i = 0; i < len; i++)
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
    bYmodem128Struct_t *ptmp = (bYmodem128Struct_t *)pbuf;
    static uint16_t     check;

    if (pbuf == NULL || (len != sizeof(bYmodem128Struct_t) && len != sizeof(bYmodem1kStruct_t)))
    {
        return -1;
    }

    if ((ptmp->soh != YMODEM_SOH && ptmp->soh != YMODEM_STX) ||
        (ptmp->number + ptmp->xnumber) != 0xff)
    {
        return -1;
    }

    check = _bYmodemCalCheck(pbuf + 3, len - 5);
    if (((check & 0xff00) >> 8) != pbuf[len - 2] || (check & 0xff) != pbuf[len - 1])
    {
        return -1;
    }
    return ptmp->soh;
}

static void _bYmodemTimeout()
{
    if (bYmodemInfo.statu == YM_S_NULL)
    {
        return;
    }
    if (bHalGetSysTick() - bYmodemInfo.tick >= MS2TICKS(2000))
    {
        bYmodemInfo.tick = bHalGetSysTick();

        if (bYmodemInfo.tt_count >= 10)
        {
            bYmodemInfo.tt_count = 0;
            if (bYmodemInfo.cb)
            {
                bYmodemInfo.cb(0, NULL, 0);
            }
            bYmodemInfo.statu = YM_S_NULL;
            return;
        }
        bYmodemInfo.tt_count++;

        switch (bYmodemInfo.statu)
        {
            case YM_S_WAIT_NAME:
            case YM_S_WAIT_START:
                bYmodemInfo.send_f(YMODEM_C);
                break;
            case YM_S_WAIT_DATA:
            case YM_S_WAIT_END:
                bYmodemInfo.send_f(YMODEM_NAK);
                break;
            default:
                break;
        }
    }
}

BOS_REG_POLLING_FUNC(_bYmodemTimeout);
/**
 * \}
 */

/**
 * \addtogroup YMODEM_Exported_Functions
 * \{
 */

int bYmodemInit(pymcb_t fcb, pymsend fs)
{
    if (fcb == NULL || fs == NULL)
    {
        return -1;
    }
    bYmodemInfo.cb     = fcb;
    bYmodemInfo.send_f = fs;
    bYmodemInfo.statu  = YM_S_NULL;
    return 0;
}

int bYmodemStart()
{
    if (bYmodemInfo.send_f == NULL)
    {
        return -1;
    }
    if (bYmodemInfo.statu == YM_S_NULL)
    {
        bYmodemInfo.send_f(YMODEM_C);
        bYmodemInfo.tick        = bHalGetSysTick();
        bYmodemInfo.statu       = YM_S_WAIT_NAME;
        bYmodemInfo.next_number = 0;
        bYmodemInfo.tt_count    = 0;
    }
    return 0;
}

int bYmodemStop()
{
    if (bYmodemInfo.send_f == NULL)
    {
        return -1;
    }
    if (bYmodemInfo.statu != YM_S_NULL)
    {
        bYmodemInfo.send_f(YMODEM_CAN);
        bYmodemInfo.statu = YM_S_NULL;
    }
    return 0;
}

int bYmodemParse(uint8_t *pbuf, uint16_t len)
{
    int                 t;
    bYmodem128Struct_t *pxm = (bYmodem128Struct_t *)pbuf;
    if (pbuf == NULL || bYmodemInfo.cb == NULL || bYmodemInfo.send_f == NULL ||
        bYmodemInfo.statu == YM_S_NULL)
    {
        return -1;
    }
    bYmodemInfo.tick     = bHalGetSysTick();
    bYmodemInfo.tt_count = 0;

    if (bYmodemInfo.statu == YM_S_WAIT_NAME)
    {
        if (_bYmodemISValid(pbuf, len) == YMODEM_SOH)
        {
            if (pxm->number == 0)
            {
                if (pxm->dat[0] == 0)
                {
                    bYmodemInfo.send_f(YMODEM_ACK);
                    bYmodemInfo.cb(YMODEM_FILEDATA, NULL, 0);
                    bYmodemInfo.statu = YM_S_NULL;
                }
                else
                {
                    bYmodemInfo.cb(YMODEM_FILENAME, pxm->dat, 128);  // [%s %s] [name file_size]
                    bYmodemInfo.send_f(YMODEM_ACK);
                    bYmodemInfo.send_f(YMODEM_C);
                    bYmodemInfo.statu = YM_S_WAIT_START;
                }
            }
        }
        else
        {
            bYmodemInfo.send_f(YMODEM_ACK);
            bYmodemInfo.cb(YMODEM_FILEDATA, NULL, 0);
            bYmodemInfo.statu = YM_S_NULL;
        }
    }
    else if (bYmodemInfo.statu == YM_S_WAIT_START)
    {
        t = _bYmodemISValid(pbuf, len);
        if (t > 0 && pxm->number == 1)
        {
            bYmodemInfo.statu       = YM_S_WAIT_DATA;
            bYmodemInfo.next_number = 1;
        }
    }

    if (bYmodemInfo.statu == YM_S_WAIT_DATA)
    {
        t = _bYmodemISValid(pbuf, len);
        if (t > 0)
        {
            if (pxm->number == bYmodemInfo.next_number)
            {
                if (t == YMODEM_SOH)
                {
                    bYmodemInfo.cb(YMODEM_FILEDATA, pxm->dat, 128);
                }
                else if (t == YMODEM_STX)
                {
                    bYmodemInfo.cb(YMODEM_FILEDATA, pxm->dat, 1024);
                }
                bYmodemInfo.next_number += 1;
                bYmodemInfo.send_f(YMODEM_ACK);
            }
            else if (pxm->number > bYmodemInfo.next_number)
            {
                bYmodemInfo.cb(0, NULL, 0);
                bYmodemInfo.statu = YM_S_NULL;
                bYmodemInfo.send_f(YMODEM_CAN);
                return -1;
            }
        }
        else if (len == 1 && *pbuf == YMODEM_EOT)
        {
            bYmodemInfo.send_f(YMODEM_NAK);
            bYmodemInfo.statu = YM_S_WAIT_END;
        }
    }
    else if (bYmodemInfo.statu == YM_S_WAIT_END)
    {
        if (len == 1 && *pbuf == YMODEM_EOT)
        {
            bYmodemInfo.send_f(YMODEM_ACK);
            bYmodemInfo.send_f(YMODEM_C);
            bYmodemInfo.statu = YM_S_WAIT_NAME;
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
