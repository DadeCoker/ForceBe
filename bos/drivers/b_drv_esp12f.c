/**
 *!
 * \file        b_drv_esp12f.c
 * \version     v0.0.1
 * \date        2020/02/05
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
#include "drivers/inc/b_drv_esp12f.h"

#include <stdio.h>

#include "thirdparty/pt/pt.h"
#include "utils/inc/b_util_list.h"
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup ESP12F
 * \{
 */

/**
 * \defgroup ESP12F_Private_Defines
 * \{
 */
#define DRIVER_NAME ESP12F

#define AT_CMD_ACK_NULL (0)
#define AT_CMD_ACK_WAIT (1)
#define AT_CMD_ACK_DONE (2)

#define AT_CMD_ACK_MS (1000)
#define AT_CMD_ACK_MS_MAX (5000)

#define AT_CMD_RESPON_LEN (64)

#define AT_RETRY_MAX (3)

#define ESP_CONN_TYPE_TCP (0)
#define ESP_CONN_TYPE_UDP (1)
#define ESP_CONN_TYPE_CLIENT (2)

/**
 * \}
 */

/**
 * \defgroup ESP12F_Private_TypesDefinitions
 * \{
 */

typedef uint16_t (*bEsp12fAtCmd_t)(uint8_t index, void *param);

typedef struct ConnectCtx
{
    uint8_t            conn_num;
    uint8_t            conn_type;
    bFIFO_Instance_t   send_fifo;
    bFIFO_Instance_t   recv_fifo;
    struct ConnectCtx *next;
    struct ConnectCtx *prev;
} bEsp12fConnectCtx_t;

typedef struct
{
    char    resp[AT_CMD_RESPON_LEN + 1];
    uint8_t ack;
} bEsp12fAtCmdAck_t;

typedef struct
{
    uint8_t              wifi_connect;
    uint8_t              mux_enable;
    uint8_t              retry;
    uint8_t              result;
    uint8_t              ctl_cmd;
    bWifiCallback_t      cb;
    bEsp12fConnectCtx_t *pconn;
    bUitlUartInstance_t  uart;
    bEsp12fAtCmdAck_t    at_ack;
    bEsp12fAtCmd_t      *pcmd;
    void                *param;
} bEsp12fPrivate_t;

/**
 * \}
 */

/**
 * \defgroup ESP12F_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup ESP12F_Private_FunctionPrototypes
 * \{
 */
static uint16_t _bEspEnterAt(uint8_t index, void *param);
static uint16_t _bEspAt(uint8_t index, void *param);
static uint16_t _bEspReset(uint8_t index, void *param);
static uint16_t _bEspSetMux(uint8_t index, void *param);
static uint16_t _bEspSetAutoConn(uint8_t index, void *param);
static uint16_t _bEspStaMode(uint8_t index, void *param);
static uint16_t _bEspJoinAp(uint8_t index, void *param);
static uint16_t _bEspSetAp(uint8_t index, void *param);
static uint16_t _bEspApMode(uint8_t index, void *param);
static uint16_t _bEspStaApMode(uint8_t index, void *param);
static uint16_t _bEspSetupTcpServer(uint8_t index, void *param);
static uint16_t _bEspMqttClose(uint8_t index, void *param);
static uint16_t _bEspCfgMqttUser(uint8_t index, void *param);
static uint16_t _bEspMqttConn(uint8_t index, void *param);
static uint16_t _bEspMqttConnCheck(uint8_t index, void *param);
static uint16_t _bEspMqttSub(uint8_t index, void *param);
static uint16_t _bEspMqttPub(uint8_t index, void *param);
static uint16_t _bEspConnTcpServer(uint8_t index, void *param);
static uint16_t _bEspConnUdpServer(uint8_t index, void *param);
static uint16_t _bEspTcpUdpSend1(uint8_t index, void *param);
static uint16_t _bEspTcpUdpSend2(uint8_t index, void *param);
static uint16_t _bEspTcpUdpClose(uint8_t index, void *param);
static uint16_t _bEspDisableMux(uint8_t index, void *param);
static uint16_t _bEspPing(uint8_t index, void *param);
static uint16_t _bEspEchoOff(uint8_t index, void *param);
/**
 * \}
 */

/**
 * \defgroup ESP12F_Private_Variables
 * \{
 */

bDRIVER_HALIF_TABLE(bESP12F_HalIf_t, DRIVER_NAME);

const static bEsp12fAtCmd_t bWifiStaOptList[] = {
    _bEspEnterAt, _bEspEnterAt,    _bEspAt,          _bEspReset,   _bEspEnterAt, _bEspEnterAt,
    _bEspAt,      _bEspDisableMux, _bEspSetAutoConn, _bEspStaMode, _bEspEchoOff, NULL};
const static bEsp12fAtCmd_t bWifiApOptList[] = {
    _bEspEnterAt, _bEspEnterAt,     _bEspAt,     _bEspReset, _bEspEnterAt, _bEspEnterAt, _bEspAt,
    _bEspSetMux,  _bEspSetAutoConn, _bEspApMode, _bEspSetAp, _bEspEchoOff, NULL};
const static bEsp12fAtCmd_t bWifiStaApOptList[] = {
    _bEspEnterAt, _bEspEnterAt,     _bEspAt,        _bEspReset, _bEspEnterAt, _bEspEnterAt, _bEspAt,
    _bEspSetMux,  _bEspSetAutoConn, _bEspStaApMode, _bEspSetAp, _bEspEchoOff, NULL};
const static bEsp12fAtCmd_t bWifiJoinApOptList[]         = {_bEspJoinAp, NULL};
const static bEsp12fAtCmd_t bWifiSetupTcpServerOptList[] = {_bEspSetupTcpServer, NULL};
const static bEsp12fAtCmd_t bWifiConnTcpServerOptList[]  = {_bEspConnTcpServer, NULL};
const static bEsp12fAtCmd_t bWifiConnUdpServerOptList[]  = {_bEspConnUdpServer, NULL};
const static bEsp12fAtCmd_t bWifiMqttConnOptList[]       = {
    _bEspMqttClose, _bEspMqttClose, _bEspCfgMqttUser, _bEspMqttConn, _bEspMqttConnCheck, NULL};
const static bEsp12fAtCmd_t bWifiMqttSubOptList[] = {_bEspMqttSub, NULL};
const static bEsp12fAtCmd_t bWifiPingOptList[]    = {_bEspPing, NULL};

static bEsp12fPrivate_t bEspRunInfo[bDRIVER_HALIF_NUM(bESP12F_HalIf_t, DRIVER_NAME)];

PT_INSTANCE(bEsp12fPT);

/**
 * \}
 */

/**
 * \defgroup ESP12F_Private_Functions
 * \{
 */

static uint8_t _bEspNumLen(uint32_t n)
{
    uint32_t t   = 1;
    uint8_t  len = 0;
    if (n == 0)
    {
        return 1;
    }
    while (n / t)
    {
        t *= 10;
        len++;
    }
    return len;
}

static char *_bEspStrStr(uint8_t *pbuf, uint16_t len, const char *str)
{
    if (pbuf == NULL || len == 0 || str == NULL)
    {
        return NULL;
    }
    uint16_t str_len = strlen(str);
    uint16_t i, j;
    for (i = 0; i < len; i++)
    {
        if (pbuf[i] == str[0])
        {
            for (j = 0; j < str_len; j++)
            {
                if (pbuf[i + j] != str[j])
                {
                    break;
                }
            }
            if (j >= str_len)
            {
                return ((char *)&pbuf[i]);
            }
        }
    }
    return NULL;
}

static int _bEspCreateConn(bEsp12fPrivate_t *pinfo, uint8_t type, uint8_t num)
{
    bEsp12fConnectCtx_t *pconn = NULL;
    pconn                      = (bEsp12fConnectCtx_t *)bMalloc(sizeof(bEsp12fConnectCtx_t));
    if (pconn == NULL)
    {
        return -1;
    }
    if (0 > bFIFO_DynCreate(&pconn->send_fifo))
    {
        bFree(pconn);
        return -1;
    }
    if (0 > bFIFO_DynCreate(&pconn->recv_fifo))
    {
        bFIFO_DynDelete(&pconn->send_fifo);
        bFree(pconn);
        return -1;
    }
    pconn->conn_type = type;
    pconn->conn_num  = num;
    if (pinfo->pconn == NULL)
    {
        pinfo->pconn = pconn;
        pconn->next  = NULL;
        pconn->prev  = NULL;
    }
    else
    {
        pconn->next = pinfo->pconn->next if (pinfo->pconn->next != NULL)
        {
        }
        pconn->next  = pinfo->pconn->next;
        pinfo->pconn = pconn;
    }
    return 0;
}

static int _bEspCloseConn(bEsp12fPrivate_t *pinfo, uint8_t type, uint8_t num)
{
    bEsp12fConnectCtx_t *pconn = pinfo->pconn;
    while (pconn != NULL)
    {
        if (pconn->conn_type == type && pconn->conn_num == num)
        {
        }
    }
}

static int _bEspRecHandler(bEsp12fPrivate_t *pinfo, uint8_t *pbuf, uint16_t len)
{
    int retval  = -1;
    int conn_id = 0;

    if (strstr((char *)pbuf, "WIFI CONNECTED") != NULL)
    {
        pinfo->wifi_connect = 1;
    }

    if (strstr((char *)pbuf, "WIFI	DISCONNECT") != NULL)
    {
        pinfo->wifi_connect = 0;
    }

    retval = sscanf((const char *)pbuf, "%d,CONNECT", &conn_id);
    if (retval == 1)
    {
        _bEspCreateConn(pinfo, ESP_CONN_TYPE_CLIENT, conn_id);
    }

    retval = sscanf((const char *)pbuf, "%d,CLOSED", &conn_id);
    if (retval == 1)
    {
        _bEspCloseConn(pinfo, ESP_CONN_TYPE_CLIENT, conn_id);
    }
    // retp = strstr((const char *)pbuf, "+MQTTSUBRECV");
    // if (retp != NULL && pinfo->wifi_rec_data.mqtt.pstr == NULL)
    // {
    //     memset(tmp, 0, sizeof(tmp));
    //     p = (char *)bMalloc(len);
    //     if (p == NULL)
    //     {
    //         pinfo->lock = 0;
    //         return -2;
    //     }
    //     retval = sscanf((const char *)retp, "+MQTTSUBRECV:%d,%[^,],%d,%s", &id, tmp, &rlen,
    //     p); if (rlen <= 0 || rlen >= len || retval != 4)
    //     {
    //         bFree(p);
    //         pinfo->lock = 0;
    //         return -2;
    //     }
    //     memcpy(
    //         p,
    //         retp + strlen("+MQTTSUBRECV:") + _bEspNumLen(id) + strlen(tmp) +
    //         _bEspNumLen(rlen) + 3, rlen);
    //     pinfo->wifi_rec_data.mqtt.pstr = p;
    //     memset(pinfo->wifi_rec_data.mqtt.topic.topic, 0,
    //            sizeof(pinfo->wifi_rec_data.mqtt.topic.topic));
    //     memcpy(pinfo->wifi_rec_data.mqtt.topic.topic, tmp, strlen(tmp));
    //     pinfo->lock = 0;
    //     return 0;
    // }
    // retp = strstr((const char *)pbuf, "+IPD");
    // while (retp)
    // {
    //     if (pinfo->cfg.mux_enable)
    //     {
    //         retval = sscanf((const char *)retp, "+IPD,%d,%d:%*s", &id, &rlen);
    //         if (retval != 2)
    //         {
    //             retval = sscanf((const char *)retp, "+IPD,%d:%*s", &rlen);
    //             if (retval == 1)
    //             {
    //                 pinfo->cfg.mux_enable = 0;
    //             }
    //         }
    //     }

    //     if (pinfo->cfg.mux_enable == 0)
    //     {
    //         retval = sscanf((const char *)retp, "+IPD,%d:%*s", &rlen);
    //     }

    //     if (((retval == 1 && pinfo->cfg.mux_enable == 0) ||
    //          (retval == 2 && pinfo->cfg.mux_enable == 1)) &&
    //         rlen > 0 && rlen < len)
    //     {
    //         if (pinfo->cfg.mux_enable)
    //         {
    //             pinfo->cfg.conn_id = id;
    //             retp               = retp + _bEspNumLen(id) + 1;
    //         }
    //         retp = retp + 5 + _bEspNumLen(rlen) + 1;
    //         bMempListAdd(&pinfo->tcp_data_list, (uint8_t *)retp, rlen);
    //         retp += rlen;
    //     }
    //     else
    //     {
    //         break;
    //     }
    //     retp = strstr((const char *)retp, "+IPD");
    // }
    // retp = _bEspStrStr(pbuf, len, "+HTTPCLIENT:");
    // while (retp)
    // {
    //     pcheckend = _bEspStrStr(pbuf, len, "\r\n\r\nOK");
    //     if (pcheckend == NULL)
    //     {
    //         pcheckend = _bEspStrStr(pbuf, len, "\r\n\r\nERR");
    //         if (pcheckend == NULL)
    //         {
    //             pinfo->lock = 0;
    //             return -3;  // 没有接收完整，继续接收
    //         }
    //     }
    //     retval = sscanf((const char *)retp, "+HTTPCLIENT:%d,%*s", &rlen);
    //     if (retval == 1 && rlen > 0 && rlen < len)
    //     {
    //         retp = retp + strlen("+HTTPCLIENT:") + _bEspNumLen(rlen) + 1;
    //         bMempListAdd(&pinfo->tcp_data_list, (uint8_t *)retp, rlen);
    //         retp += rlen;
    //     }
    //     else
    //     {
    //         break;
    //     }
    //     retp = strstr((const char *)retp, "+HTTPCLIENT:");
    // }
    return 0;
}

static int _bEspUartIdleCb(uint8_t *pbuf, uint16_t len, void *arg)
{
    int retval = 0;
    b_log("%s\r\n", pbuf);
    bEsp12fAtCmdAck_t *pack = &(((bEsp12fPrivate_t *)arg)->at_ack);
    if (pack->ack == AT_CMD_ACK_WAIT)
    {
        if (strstr((char *)pbuf, pack->resp) != NULL)
        {
            pack->ack = AT_CMD_ACK_DONE;
        }
    }
    return _bEspRecHandler(arg, pbuf, len);
}

static void _bAtCmdSetResp(uint8_t index, char *p)
{
    memset(&bEspRunInfo[index].at_ack, 0, sizeof(bEsp12fAtCmdAck_t));
    memcpy(bEspRunInfo[index].at_ack.resp, p,
           (strlen(p) > AT_CMD_RESPON_LEN) ? AT_CMD_RESPON_LEN : strlen(p));
    bEspRunInfo[index].at_ack.ack = AT_CMD_ACK_WAIT;
}

static uint16_t _bEspStaMode(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (uint8_t *)"AT+CWMODE=1\r\n", strlen("AT+CWMODE=1\r\n"));
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspApMode(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (uint8_t *)"AT+CWMODE=2\r\n", strlen("AT+CWMODE=2\r\n"));
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspStaApMode(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (uint8_t *)"AT+CWMODE=3\r\n", strlen("AT+CWMODE=3\r\n"));
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspSetAp(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    char           buf[200];
    int            len;
    bWifiApInfo_t *pinfo = (bWifiApInfo_t *)param;
    if (param == NULL)
    {
        return 0;
    }
    if (strlen(pinfo->passwd) == 0 || pinfo->encryption == 0)
    {
        len = snprintf(buf, 200, "AT+CWSAP=\"%s\",\"\",%d,%d\r\n", pinfo->ssid, 5, 0);
    }
    else
    {
        len = snprintf(buf, 200, "AT+CWSAP=\"%s\",\"%s\",%d,%d\r\n", pinfo->ssid, pinfo->passwd, 5,
                       pinfo->encryption + 1);
    }
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))), (uint8_t *)buf,
                 len);
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspJoinAp(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    char           buf[200];
    int            len   = 0;
    bWifiApInfo_t *pinfo = (bWifiApInfo_t *)param;
    if (param == NULL)
    {
        return 0;
    }
    if (pinfo->encryption == 0 || strlen(pinfo->passwd) == 0)
    {
        len = snprintf(buf, 200, "AT+CWJAP=\"%s\",\"\"\r\n", pinfo->ssid);
    }
    else if (strlen(pinfo->passwd) > 0)
    {
        len = snprintf(buf, 200, "AT+CWJAP=\"%s\",\"%s\"\r\n", pinfo->ssid, pinfo->passwd);
    }
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))), (uint8_t *)buf,
                 len);
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspSetMux(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (uint8_t *)"AT+CIPMUX=1\r\n", strlen("AT+CIPMUX=1\r\n"));
    bEspRunInfo[index].mux_enable = 1;
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspDisableMux(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (uint8_t *)"AT+CIPMUX=0\r\n", strlen("AT+CIPMUX=0\r\n"));
    bEspRunInfo[index].mux_enable = 0;
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspSetAutoConn(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (uint8_t *)"AT+CWAUTOCONN=1\r\n", strlen("AT+CWAUTOCONN=1\r\n"));
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspEnterAt(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (uint8_t *)"+++", strlen("+++"));
    return 0;
}

static uint16_t _bEspAt(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (uint8_t *)"AT\r\n", strlen("AT\r\n"));
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspReset(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (uint8_t *)"AT+RST\r\n", strlen("AT+RST\r\n"));
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspSetupTcpServer(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    char           buf[32];
    uint8_t        len;
    bTcpUdpInfo_t *pinfo = (bTcpUdpInfo_t *)param;
    if (param == NULL)
    {
        return 0;
    }
    len = snprintf(buf, 32, "AT+CIPSERVER=%d,%d\r\n", 1, pinfo->port);
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))), (uint8_t *)buf,
                 len);
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspMqttClose(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (uint8_t *)"AT+MQTTCLEAN=0\r\n", strlen("AT+MQTTCLEAN=0\r\n"));
    return 0;
}

static uint16_t _bEspCfgMqttUser(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    char             buf[256];
    int              len   = 0;
    bMqttConnInfo_t *pinfo = (bMqttConnInfo_t *)param;
    if (param == NULL)
    {
        return 0;
    }
    len = snprintf(buf, 256, "AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n",
                   pinfo->device_id, pinfo->user, pinfo->passwd);
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))), (uint8_t *)buf,
                 len);
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspMqttConn(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    char             buf[128];
    int              len   = 0;
    bMqttConnInfo_t *pinfo = (bMqttConnInfo_t *)param;
    if (param == NULL)
    {
        return 0;
    }
    len = snprintf(buf, 128, "AT+MQTTCONN=0,\"%s\",%d,1\r\n", pinfo->broker, pinfo->port);
    _bAtCmdSetResp(index, "ERROR");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))), (uint8_t *)buf,
                 len);
    return AT_CMD_ACK_MS_MAX;
}

static uint16_t _bEspMqttConnCheck(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    _bAtCmdSetResp(index, "+MQTTCONN:0,4");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (uint8_t *)"AT+MQTTCONN?\r\n", strlen("AT+MQTTCONN?\r\n"));
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspMqttSub(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    char              buf[128];
    int               len   = 0;
    bMqttTopicInfo_t *pinfo = (bMqttTopicInfo_t *)param;
    if (param == NULL)
    {
        return 0;
    }
    len = snprintf(buf, 128, "AT+MQTTSUB=0,\"%s\",%d\r\n", pinfo->topic, pinfo->qos);
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))), (uint8_t *)buf,
                 len);
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspConnTcpServer(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    char           buf[100];
    uint8_t        len;
    bTcpUdpInfo_t *pinfo = (bTcpUdpInfo_t *)param;
    if (param == NULL)
    {
        return 0;
    }
    len = sprintf(buf, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", pinfo->ip, pinfo->port);
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))), (uint8_t *)buf,
                 len);
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspConnUdpServer(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    char           buf[100];
    uint8_t        len;
    bTcpUdpInfo_t *pinfo = (bTcpUdpInfo_t *)param;
    if (param == NULL)
    {
        return 0;
    }
    len = sprintf(buf, "AT+CIPSTART=\"UDP\",\"%s\",%d\r\n", pinfo->ip, pinfo->port);
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))), (uint8_t *)buf,
                 len);
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspTcpUdpClose(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    _bAtCmdSetResp(index, "ERROR");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (uint8_t *)"AT+CIPCLOSE\r\n", strlen("AT+CIPCLOSE\r\n"));
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspPing(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    char    buf[100];
    uint8_t len;
    if (param == NULL)
    {
        return 0;
    }
    len = sprintf(buf, "AT+PING=\"%s\"\r\n", (char *)param);
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))), (uint8_t *)buf,
                 len);
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspEchoOff(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (uint8_t *)"ATE0\r\n", strlen("ATE0\r\n"));
    return AT_CMD_ACK_MS;
}

static void _bEspResult(uint8_t index, uint8_t result)
{
    bEspRunInfo[index].result = result;
    bEspRunInfo[index].pcmd   = NULL;
    bEspRunInfo[index].retry  = 0;
    if (bEspRunInfo[index].cb.cb)
    {
        bEspRunInfo[index].cb.cb(bEspRunInfo[index].ctl_cmd, &bEspRunInfo[index].result, NULL,
                                 bEspRunInfo[index].cb.user_data);
    }
}

static int _bEspMainTask(struct pt *pt)
{
    static uint8_t index  = 0;
    uint16_t       retval = 0;
    PT_BEGIN(pt);
    while (1)
    {
        if (bEspRunInfo[index].pcmd == NULL)
        {
            break;
        }

        if (*(bEspRunInfo[index].pcmd) == NULL)
        {
            _bEspResult(index, 1);
            index = (index + 1) % (bDRIVER_HALIF_NUM(bESP12F_HalIf_t, DRIVER_NAME));
            break;
        }
        retval = (*(bEspRunInfo[index].pcmd))(index, bEspRunInfo[index].param);
        if (retval > 0)
        {
            PT_WAIT_UNTIL(pt, bEspRunInfo[index].at_ack.ack == AT_CMD_ACK_DONE, retval);
            if (bEspRunInfo[index].at_ack.ack == AT_CMD_ACK_DONE)
            {
                retval = 0;
            }
            else
            {
                retval = 1;
            }
        }
        if (retval == 0)
        {
            bEspRunInfo[index].retry = 0;
            bEspRunInfo[index].pcmd += 1;
        }
        else
        {
            bEspRunInfo[index].retry += 1;
            if (bEspRunInfo[index].retry >= AT_RETRY_MAX)
            {
                bEspRunInfo[index].retry = 0;
                _bEspResult(index, 0);
            }
            index = (index + 1) % (bDRIVER_HALIF_NUM(bESP12F_HalIf_t, DRIVER_NAME));
        }
        PT_DELAY_MS(pt, 500);
    }
    PT_END(pt);
}

static void _bEspPolling()
{
    _bEspMainTask(&bEsp12fPT);
}

BOS_REG_POLLING_FUNC(_bEspPolling);

static int _bEspCtlStart(bEsp12fPrivate_t *pinfo, uint8_t cmd, bEsp12fAtCmd_t *pcmd, void *param)
{
    if (pinfo->pcmd != NULL)
    {
        return -2;
    }
    pinfo->retry   = 0;
    pinfo->result  = 0;
    pinfo->ctl_cmd = cmd;
    pinfo->pcmd    = pcmd;
    pinfo->param   = param;
    return 0;
}

static int _bEspCtl(bDriverInterface_t *pdrv, uint8_t cmd, void *param)
{
    int               retval = 0;
    bEsp12fPrivate_t *pinfo  = (bEsp12fPrivate_t *)pdrv->_private._p;
    switch (cmd)
    {
        case bCMD_WIFI_MODE_STA:
            retval = _bEspCtlStart(pinfo, bCMD_WIFI_MODE_STA, (bEsp12fAtCmd_t *)&bWifiStaOptList[0],
                                   param);
            break;
        case bCMD_WIFI_MODE_AP:
            if (param != NULL)
            {
                retval = _bEspCtlStart(pinfo, bCMD_WIFI_MODE_AP,
                                       (bEsp12fAtCmd_t *)&bWifiApOptList[0], param);
            }
            else
            {
                retval = -1;
            }
            break;
        case bCMD_WIFI_MODE_STA_AP:
            if (param != NULL)
            {
                retval = _bEspCtlStart(pinfo, bCMD_WIFI_MODE_STA_AP,
                                       (bEsp12fAtCmd_t *)&bWifiStaApOptList[0], param);
            }
            else
            {
                retval = -1;
            }
            break;
        case bCMD_WIFI_JOIN_AP:
            if (param != NULL)
            {
                retval = _bEspCtlStart(pinfo, bCMD_WIFI_JOIN_AP,
                                       (bEsp12fAtCmd_t *)&bWifiJoinApOptList[0], param);
            }
            else
            {
                retval = -1;
            }
            break;
        case bCMD_WIFI_MQTT_CONN:
            if (param != NULL)
            {
                retval = _bEspCtlStart(pinfo, bCMD_WIFI_MQTT_CONN,
                                       (bEsp12fAtCmd_t *)&bWifiMqttConnOptList[0], param);
            }
            else
            {
                retval = -1;
            }
            break;
        case bCMD_WIFI_MQTT_SUB:
            if (param != NULL)
            {
                retval = _bEspCtlStart(pinfo, bCMD_WIFI_MQTT_SUB,
                                       (bEsp12fAtCmd_t *)&bWifiMqttSubOptList[0], param);
            }
            else
            {
                retval = -1;
            }
            break;
        case bCMD_WIFI_LOCAL_TCP_SERVER:
            if (param != NULL)
            {
                retval = _bEspCtlStart(pinfo, bCMD_WIFI_LOCAL_TCP_SERVER,
                                       (bEsp12fAtCmd_t *)&bWifiSetupTcpServerOptList[0], param);
            }
            else
            {
                retval = -1;
            }
            break;
        case bCMD_WIFI_REMOT_TCP_SERVER:
            if (param != NULL)
            {
                retval = _bEspCtlStart(pinfo, bCMD_WIFI_REMOT_TCP_SERVER,
                                       (bEsp12fAtCmd_t *)&bWifiConnTcpServerOptList[0], param);
            }
            else
            {
                retval = -1;
            }
            break;
        case bCMD_WIFI_REMOT_UDP_SERVER:
            if (param != NULL)
            {
                retval = _bEspCtlStart(pinfo, bCMD_WIFI_REMOT_UDP_SERVER,
                                       (bEsp12fAtCmd_t *)&bWifiConnUdpServerOptList[0], param);
            }
            else
            {
                retval = -1;
            }
            break;
        case bCMD_WIFI_PING:
            if (param != NULL)
            {
                retval = _bEspCtlStart(pinfo, bCMD_WIFI_PING,
                                       (bEsp12fAtCmd_t *)&bWifiPingOptList[0], param);
            }
            else
            {
                retval = -1;
            }
            break;
        case bCMD_WIFI_REG_CALLBACK:
            if (param != NULL)
            {
                memcpy(&pinfo->cb, param, sizeof(pinfo->cb));
            }
            else
            {
                retval = -1;
            }
            break;
        default:
            break;
    }
    return retval;
}

static int _bEspRead(bDriverInterface_t *pdrv, uint32_t offset, uint8_t *pbuf, uint32_t len)
{
    return 0;
}

/**
 * \}
 */

/**
 * \addtogroup ESP12F_Exported_Functions
 * \{
 */
int bESP12F_Init(bDriverInterface_t *pdrv)
{
    bEsp12fPrivate_t *pinfo = NULL;
    uint8_t           index = pdrv->drv_no;
    uint8_t          *p     = NULL;
    bDRIVER_STRUCT_INIT(pdrv, DRIVER_NAME, bESP12F_Init);
    //    pdrv->read        = _bEspRead;
    //    pdrv->write       = _bEspWrite;
    pdrv->ctl         = _bEspCtl;
    pdrv->_private._p = &bEspRunInfo[index];

    // 初始化 runinfo
    memset(&bEspRunInfo[index], 0, sizeof(bEsp12fPrivate_t));
    bEspRunInfo[index].wifi_connect = WIFI_STA_INVALID;

    p = (uint8_t *)bMalloc(ESP12F_UART_RX_BUF_LEN);
    if (p == NULL)
    {
        return -1;
    }
    bUtilUartInitStruct(&(bEspRunInfo[index].uart), p, ESP12F_UART_RX_BUF_LEN, 200, _bEspUartIdleCb,
                        &(bEspRunInfo[index]));
    bUtilUartBind(*((bESP12F_HalIf_t *)pdrv->hal_if), &(bEspRunInfo[index].uart));

    PT_INIT(&bEsp12fPT);
    return 0;
}

bDRIVER_REG_INIT(B_DRIVER_ESP12F, bESP12F_Init);

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
// https://docs.ai-thinker.com/固件汇总  MQTT透传AT固件 （固件号：1112）
/************************ Copyright (c) 2019 Bean *****END OF FILE********/
