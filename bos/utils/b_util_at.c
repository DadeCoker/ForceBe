/**
 *!
 * \file        b_util_at.c
 * \version     v0.0.1
 * \date        2020/04/01
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
#include "utils/inc/b_util_at.h"

#include "b_section.h"
#include "hal/inc/b_hal.h"
#include "utils/inc/b_util_log.h"
#include "utils/inc/b_util_memp.h"

#if (defined(_AT_ENABLE) && (_AT_ENABLE == 1))

/**
 * \addtogroup B_UTILS
 * \{
 */

/**
 * \addtogroup AT
 * \{
 */

/**
 * \defgroup AT_Private_TypesDefinitions
 * \{
 */

typedef struct
{
    uint8_t id;
    uint8_t status;
} bAtStat_t;

typedef struct
{
    uint8_t  id;
    uint8_t  uart;
    uint8_t *pcmd;
    uint8_t *presp;
    uint16_t cmd_len;
    uint16_t resp_len;
    uint32_t timeout;
} bAtQueue_t;

typedef struct
{
    uint8_t           head_index;
    uint8_t           tail_index;
    uint8_t           stat_index;
    uint8_t           id;
    volatile uint8_t  retry_count;
    volatile uint8_t  flag;
    volatile uint32_t tick;
} bAtInfo_t;

typedef struct CbInfo
{
    bAtCallback_t  cb;
    void          *arg;
    struct CbInfo *next;
} bAtCbInfo_t;

/**
 * \}
 */

/**
 * \defgroup AT_Private_Defines
 * \{
 */
#define AT_STAT_LEN (2 * (AT_Q_LEN))
#define AT_INDEX_INVALID (0XFF)

#define AT_FLAG_NULL (0)
#define AT_FLAG_WAIT (1)
/**
 * \}
 */

/**
 * \defgroup AT_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup AT_Private_Variables
 * \{
 */
static bAtQueue_t  bAtQueue[AT_Q_LEN];
static bAtStat_t   bAtStat[AT_STAT_LEN];
static bAtInfo_t   bAtInfo;
static uint8_t     bAtInitFlag   = 0;
static bAtCbInfo_t bAtCbInfoHead = {.next = NULL};
/**
 * \}
 */

/**
 * \defgroup AT_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup AT_Private_Functions
 * \{
 */
static void _bAtInit()
{
    memset(&bAtStat[0], 0xff, sizeof(bAtStat));
    memset(&bAtQueue[0], 0, sizeof(bAtQueue));
    bAtInfo.head_index  = 0;
    bAtInfo.tail_index  = AT_INDEX_INVALID;
    bAtInfo.stat_index  = 0;
    bAtInfo.id          = 0;
    bAtInfo.retry_count = 0;
    bAtInfo.flag        = AT_FLAG_NULL;
}

static void _bAtRetry()
{
    bAtInfo.flag = AT_FLAG_NULL;
    bAtInfo.retry_count += 1;
}

static void _bAtNextCommand(uint8_t id, uint8_t sta)
{

    int          i     = 0;
    bAtCbInfo_t *pinfo = bAtCbInfoHead.next;
    for (i = 0; i < AT_STAT_LEN; i++)
    {
        if (bAtStat[i].id == id)
        {
            bAtStat[i].status = sta;
            break;
        }
    }

    bAtInfo.retry_count = 0;
    bAtInfo.flag        = AT_FLAG_NULL;
    bFree(bAtQueue[bAtInfo.tail_index].pcmd);
    bFree(bAtQueue[bAtInfo.tail_index].presp);
    bAtInfo.tail_index = (bAtInfo.tail_index + 1) % AT_Q_LEN;
    if (bAtInfo.tail_index == bAtInfo.head_index)
    {
        bAtInfo.tail_index = AT_INDEX_INVALID;
    }

    while (pinfo != NULL)
    {
        pinfo->cb(id, sta, pinfo->arg);
        pinfo = pinfo->next;
    }
}

/**
 * \}
 */

/**
 * \addtogroup AT_Exported_Functions
 * \{
 */

int bAtCmdSend(const char *pcmd, uint16_t cmd_len, const char *presp, uint16_t resp_len,
               uint8_t uart, uint32_t timeout)
{
    int retval = -1;

    if (bAtInitFlag == 0)
    {
        _bAtInit();
        bAtInitFlag = 1;
    }

    if (pcmd == NULL || cmd_len == 0 || (presp == NULL && resp_len > 0))
    {
        return -1;
    }

    if (bAtInfo.head_index == bAtInfo.tail_index)
    {
        return -2;
    }
    bAtQueue[bAtInfo.head_index].pcmd  = (uint8_t *)bMalloc(cmd_len);
    bAtQueue[bAtInfo.head_index].presp = (uint8_t *)bMalloc(resp_len);
    if (bAtQueue[bAtInfo.head_index].pcmd == NULL ||
        (bAtQueue[bAtInfo.head_index].presp == NULL && resp_len > 0))
    {
        bFree(bAtQueue[bAtInfo.head_index].pcmd);
        bFree(bAtQueue[bAtInfo.head_index].presp);
        b_log_e("malloc err... \n");
        return -3;
    }

    memcpy(bAtQueue[bAtInfo.head_index].pcmd, pcmd, cmd_len);
    if (resp_len > 0)
    {
        memcpy(bAtQueue[bAtInfo.head_index].presp, presp, resp_len);
    }

    bAtQueue[bAtInfo.head_index].cmd_len  = cmd_len;
    bAtQueue[bAtInfo.head_index].resp_len = resp_len;
    bAtQueue[bAtInfo.head_index].uart     = uart;
    bAtQueue[bAtInfo.head_index].id       = bAtInfo.id;
    bAtQueue[bAtInfo.head_index].timeout  = timeout;

    bAtStat[bAtInfo.stat_index].id     = bAtInfo.id;
    bAtStat[bAtInfo.stat_index].status = AT_STA_NULL;

    retval = bAtInfo.id;

    bAtInfo.id         = (bAtInfo.id + 1) % AT_INVALID_ID;
    bAtInfo.stat_index = (bAtInfo.stat_index + 1) % AT_STAT_LEN;
    if (bAtInfo.tail_index == AT_INDEX_INVALID)
    {
        bAtInfo.tail_index = bAtInfo.head_index;
    }
    bAtInfo.head_index = (bAtInfo.head_index + 1) % AT_Q_LEN;
    return retval;
}

int bAtGetStat(uint8_t id)
{
    int i = 0;
    for (i = 0; i < AT_STAT_LEN; i++)
    {
        if (bAtStat[i].id == id)
        {
            return bAtStat[i].status;
        }
    }
    return AT_STA_ID_INVALID;
}

int bAtFeedRespData(uint8_t *pbuf, uint16_t len)
{
    char       *pretval = NULL;
    bAtQueue_t *pAtCmd  = NULL;
    if (pbuf == NULL || bAtInfo.flag != AT_FLAG_WAIT)
    {
        return -1;
    }
    pAtCmd  = &bAtQueue[bAtInfo.tail_index];
    pretval = strstr((const char *)pbuf, (const char *)pAtCmd->presp);
    if (pretval != NULL)
    {
        _bAtNextCommand(pAtCmd->id, AT_STA_OK);
    }
    return 0;
}

int bAtRegistCallback(bAtCallback_t cb, void *arg)
{
    bAtCbInfo_t *pinfo = NULL;
    if (cb == NULL)
    {
        return -1;
    }

    pinfo = (bAtCbInfo_t *)bMalloc(sizeof(bAtCbInfo_t));
    if (pinfo == NULL)
    {
        return -2;
    }
    pinfo->cb          = cb;
    pinfo->arg         = arg;
    pinfo->next        = bAtCbInfoHead.next;
    bAtCbInfoHead.next = pinfo;
    return 0;
}

void bAtPolling()
{
    bAtQueue_t *pAtCmd = NULL;
    if (bAtInfo.tail_index == AT_INDEX_INVALID || bAtInitFlag == 0)
    {
        return;
    }
    pAtCmd = &bAtQueue[bAtInfo.tail_index];
    if (bAtInfo.flag == AT_FLAG_NULL)
    {
        if (bAtInfo.retry_count > AT_RETRY_COUNT)
        {
            _bAtNextCommand(pAtCmd->id, AT_STA_ERR);
        }
        else
        {
            bHalUartSend((bHalUartNumber_t)pAtCmd->uart, pAtCmd->pcmd, pAtCmd->cmd_len);
            if (pAtCmd->resp_len == 0)
            {
                _bAtNextCommand(pAtCmd->id, AT_STA_OK);
            }
            else
            {
                bAtInfo.flag = AT_FLAG_WAIT;
                bAtInfo.tick = bHalGetSysTick();
            }
        }
    }
    else if (bAtInfo.flag == AT_FLAG_WAIT)
    {
        if (bHalGetSysTick() - bAtInfo.tick >= MS2TICKS(pAtCmd->timeout))
        {
            _bAtRetry();
        }
    }
}

BOS_REG_POLLING_FUNC(bAtPolling);

#endif

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
