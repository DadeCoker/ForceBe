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

#define ESP_CMD_RETRY (2)

/**
 * \}
 */

/**
 * \defgroup ESP12F_Private_TypesDefinitions
 * \{
 */

typedef uint8_t (*pOptFunc_t)(bDriverInterface_t *pdrv, void *param);

/**
 * \}
 */

/**
 * \defgroup ESP12F_Private_Macros
 * \{
 */
#define OPT_AT (0)
#define OPT_ENTER_AT (1)
#define OPT_RESET (2)
#define OPT_SET_MUX (3)
#define OPT_SET_AUTOCONN (4)
#define OPT_STA_MODE (5)
#define OPT_JOIN_AP (6)
#define OPT_SET_AP (7)
#define OPT_AP_MODE (8)
#define OPT_STA_AP_MODE (9)
#define OPT_SETUP_TCP_SERVER (10)
#define OPT_MQTT_CLOSE (11)
#define OPT_CFG_MQTTUSER (12)
#define OPT_MQTT_CONN (13)
#define OPT_MQTT_CONN_CHECK (14)
#define OPT_MQTT_SUB (15)
#define OPT_MQTT_PUB (16)
#define OPT_CONN_TCP_SERVER (17)
#define OPT_CONN_UDP_SERVER (18)
#define OPT_TCP_UDP_SEND_1 (19)
#define OPT_TCP_UDP_SEND_2 (20)
#define OPT_TCP_UDP_CLOSE (21)
#define OPT_DISABLE_MUX (22)
#define OPT_PING (23)
#define OPT_ECHO_OFF (24)
#define OPT_NUMBER (25)
#define OPT_NULL (OPT_NUMBER + 1)
#define IS_ESP_OPT(n)                                                                             \
    ((n == OPT_AT) || (n == OPT_ENTER_AT) || (n == OPT_RESET) || (n == OPT_SET_MUX) ||            \
     (n == OPT_SET_AUTOCONN) || (n == OPT_STA_MODE) || (n == OPT_JOIN_AP) || (n == OPT_SET_AP) || \
     (n == OPT_AP_MODE) || (n == OPT_STA_AP_MODE) || (n == OPT_SETUP_TCP_SERVER) ||               \
     (n == OPT_MQTT_CLOSE) || (n == OPT_CFG_MQTTUSER) || (n == OPT_MQTT_CONN) ||                  \
     (n == OPT_MQTT_CONN_CHECK) || (n == OPT_MQTT_SUB) || (n == OPT_MQTT_PUB) ||                  \
     (n == OPT_CONN_TCP_SERVER) || (n == OPT_CONN_UDP_SERVER) || (n == OPT_TCP_UDP_SEND_1) ||     \
     (n == OPT_TCP_UDP_SEND_2) || (n == OPT_TCP_UDP_CLOSE) || (n == OPT_DISABLE_MUX) ||           \
     (n == OPT_PING) || (n == OPT_ECHO_OFF))
/**
 * \}
 */

/**
 * \defgroup ESP12F_Private_FunctionPrototypes
 * \{
 */
static uint8_t _bEspEnterAt(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspAt(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspReset(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspSetMux(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspSetAutoConn(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspStaMode(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspJoinAp(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspSetAp(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspApMode(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspStaApMode(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspSetupTcpServer(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspMqttClose(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspCfgMqttUser(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspMqttConn(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspMqttConnCheck(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspMqttSub(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspMqttPub(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspConnTcpServer(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspConnUdpServer(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspTcpUdpSend1(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspTcpUdpSend2(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspTcpUdpClose(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspDisableMux(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspPing(bDriverInterface_t *pdrv, void *param);
static uint8_t _bEspEchoOff(bDriverInterface_t *pdrv, void *param);
/**
 * \}
 */

/**
 * \defgroup ESP12F_Private_Variables
 * \{
 */

bDRIVER_HALIF_TABLE(bESP12F_HalIf_t, DRIVER_NAME);

const static uint8_t bWifiStaOptList[] = {
    OPT_ENTER_AT, OPT_ENTER_AT,    OPT_AT,           OPT_RESET,    OPT_ENTER_AT, OPT_ENTER_AT,
    OPT_AT,       OPT_DISABLE_MUX, OPT_SET_AUTOCONN, OPT_STA_MODE, OPT_ECHO_OFF, OPT_NULL};
const static uint8_t bWifiApOptList[] = {
    OPT_ENTER_AT, OPT_ENTER_AT,     OPT_AT,      OPT_RESET,  OPT_ENTER_AT, OPT_ENTER_AT, OPT_AT,
    OPT_SET_MUX,  OPT_SET_AUTOCONN, OPT_AP_MODE, OPT_SET_AP, OPT_ECHO_OFF, OPT_NULL};
const static uint8_t bWifiStaApOptList[] = {
    OPT_ENTER_AT, OPT_ENTER_AT,     OPT_AT,          OPT_RESET,  OPT_ENTER_AT, OPT_ENTER_AT, OPT_AT,
    OPT_SET_MUX,  OPT_SET_AUTOCONN, OPT_STA_AP_MODE, OPT_SET_AP, OPT_ECHO_OFF, OPT_NULL};
const static uint8_t bWifiJoinApOptList[]         = {OPT_JOIN_AP, OPT_NULL};
const static uint8_t bWifiSetupTcpServerOptList[] = {OPT_SETUP_TCP_SERVER, OPT_NULL};
const static uint8_t bWifiConnTcpServerOptList[]  = {OPT_TCP_UDP_CLOSE, OPT_CONN_TCP_SERVER,
                                                     OPT_NULL};
const static uint8_t bWifiConnUdpServerOptList[]  = {OPT_TCP_UDP_CLOSE, OPT_CONN_UDP_SERVER,
                                                     OPT_NULL};
const static uint8_t bWifiMqttConnOptList[]       = {
          OPT_MQTT_CLOSE, OPT_MQTT_CLOSE, OPT_CFG_MQTTUSER, OPT_MQTT_CONN, OPT_MQTT_CONN_CHECK, OPT_NULL};
const static uint8_t bWifiMqttSubOptList[] = {OPT_MQTT_SUB, OPT_NULL};
const static uint8_t bWifiMqttPubOptList[] = {OPT_MQTT_PUB, OPT_NULL};
const static uint8_t bWifiTcpSendOptList[] = {OPT_TCP_UDP_SEND_1, OPT_TCP_UDP_SEND_2, OPT_NULL};
const static uint8_t bWifiPingOptList[]    = {OPT_PING, OPT_NULL};

const static pOptFunc_t bOptFuncTable[OPT_NUMBER] = {
    _bEspAt,          _bEspEnterAt,       _bEspReset,          _bEspSetMux,
    _bEspSetAutoConn, _bEspStaMode,       _bEspJoinAp,         _bEspSetAp,
    _bEspApMode,      _bEspStaApMode,     _bEspSetupTcpServer, _bEspMqttClose,
    _bEspCfgMqttUser, _bEspMqttConn,      _bEspMqttConnCheck,  _bEspMqttSub,
    _bEspMqttPub,     _bEspConnTcpServer, _bEspConnUdpServer,  _bEspTcpUdpSend1,
    _bEspTcpUdpSend2, _bEspTcpUdpClose,   _bEspDisableMux,     _bEspPing,
    _bEspEchoOff,
};

static bEsp12fPrivate_t    bEspRunInfo[bDRIVER_HALIF_NUM(bESP12F_HalIf_t, DRIVER_NAME)];
static bDriverInterface_t *bEspDrvTable[bDRIVER_HALIF_NUM(bESP12F_HalIf_t, DRIVER_NAME)];
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

static int _bEspRecHandler(bDriverInterface_t *pdrv, uint8_t *pbuf, uint16_t len)
{
    char             *retp   = NULL;
    char             *p      = NULL;
    int               retval = 0, id = 0, rlen = 0;
    char              tmp[64];
    char             *pcheckend = NULL;
    bEsp12fPrivate_t *pinfo     = (bEsp12fPrivate_t *)pdrv->_private._p;
    if (pinfo->lock)
    {
        return -1;
    }

    pinfo->lock = 1;
    retp        = strstr((const char *)pbuf, "+MQTTSUBRECV");
    if (retp != NULL && pinfo->wifi_rec_data.mqtt.pstr == NULL)
    {
        memset(tmp, 0, sizeof(tmp));
        p = (char *)bMalloc(len);
        if (p == NULL)
        {
            pinfo->lock = 0;
            return -2;
        }
        retval = sscanf((const char *)retp, "+MQTTSUBRECV:%d,%[^,],%d,%s", &id, tmp, &rlen, p);
        if (rlen <= 0 || rlen >= len || retval != 4)
        {
            bFree(p);
            pinfo->lock = 0;
            return -2;
        }
        memcpy(p, retp + strlen("+MQTTSUBRECV:") + _bEspNumLen(id) + strlen(tmp) + _bEspNumLen(rlen) + 3, rlen);
        pinfo->wifi_rec_data.mqtt.pstr = p;
        memset(pinfo->wifi_rec_data.mqtt.topic.topic, 0,
               sizeof(pinfo->wifi_rec_data.mqtt.topic.topic));
        memcpy(pinfo->wifi_rec_data.mqtt.topic.topic, tmp, strlen(tmp));
        pinfo->lock = 0;
        return 0;
    }
    retp = strstr((const char *)pbuf, "+IPD");
    while (retp)
    {
        if (pinfo->cfg.mux_enable)
        {
            retval = sscanf((const char *)retp, "+IPD,%d,%d:%*s", &id, &rlen);
            if (retval != 2)
            {
                retval = sscanf((const char *)retp, "+IPD,%d:%*s", &rlen);
                if (retval == 1)
                {
                    pinfo->cfg.mux_enable = 0;
                }
            }
        }

        if (pinfo->cfg.mux_enable == 0)
        {
            retval = sscanf((const char *)retp, "+IPD,%d:%*s", &rlen);
        }

        if (((retval == 1 && pinfo->cfg.mux_enable == 0) ||
             (retval == 2 && pinfo->cfg.mux_enable == 1)) &&
            rlen > 0 && rlen < len)
        {
            if (pinfo->cfg.mux_enable)
            {
                pinfo->cfg.conn_id = id;
                retp               = retp + _bEspNumLen(id) + 1;
            }
            retp = retp + 5 + _bEspNumLen(rlen) + 1;
            bMempListAdd(&pinfo->tcp_data_list, (uint8_t *)retp, rlen);
            retp += rlen;
        }
        else
        {
            break;
        }
        retp = strstr((const char *)retp, "+IPD");
    }
    retp = _bEspStrStr(pbuf, len, "+HTTPCLIENT:");
    while (retp)
    {
        pcheckend = _bEspStrStr(pbuf, len, "\r\n\r\nOK");
        if (pcheckend == NULL)
        {
            pcheckend = _bEspStrStr(pbuf, len, "\r\n\r\nERR");
            if (pcheckend == NULL)
            {
                pinfo->lock = 0;
                return -3;  //没有接收完整，继续接收
            }
        }
        retval = sscanf((const char *)retp, "+HTTPCLIENT:%d,%*s", &rlen);
        if (retval == 1 && rlen > 0 && rlen < len)
        {
            retp = retp + strlen("+HTTPCLIENT:") + _bEspNumLen(rlen) + 1;
            bMempListAdd(&pinfo->tcp_data_list, (uint8_t *)retp, rlen);
            retp += rlen;
        }
        else
        {
            break;
        }
        retp = strstr((const char *)retp, "+HTTPCLIENT:");
    }
    pinfo->lock = 0;
    return 0;
}

static int _bEspUartIdleCb(uint8_t *pbuf, uint16_t len, void *arg)
{
    int retval = 0;
    bAtFeedRespData(pbuf, len);
    if (_bEspRecHandler(arg, pbuf, len) == -3)
    {
        retval = -1;
    }
    return retval;
}

static void _bEspCmdFinished(bDriverInterface_t *pdrv, uint8_t result)
{
    bEsp12fPrivate_t *pinfo = (bEsp12fPrivate_t *)pdrv->_private._p;
    pinfo->result           = result;

    if (pinfo->opt_param != NULL)
    {
        bFree(pinfo->opt_param);
        pinfo->opt_param = NULL;
    }

    pinfo->opt.index    = 0;
    pinfo->opt.list_len = 0;
    pinfo->opt.plist    = NULL;
    pinfo->opt.at_id    = AT_INVALID_ID;
}

static void _bEspAtCb(uint8_t id, uint8_t result, void *arg)
{
    static uint8_t    retry = 0;
    bEsp12fPrivate_t *pinfo = (bEsp12fPrivate_t *)(((bDriverInterface_t *)arg)->_private._p);
    if (pinfo->opt.at_id == id && id != AT_INVALID_ID)
    {
        pinfo->opt.at_id = AT_INVALID_ID;
        if (result == AT_STA_OK)
        {
            pinfo->opt.index += 1;
            if (pinfo->opt.index < pinfo->opt.list_len)
            {
                if (pinfo->opt.plist[pinfo->opt.index] == OPT_NULL)
                {
                    _bEspCmdFinished(arg, ESP12F_CMD_RESULT_OK);
                }
            }
            retry = 0;
        }
        else
        {
            retry++;
            if (retry > ESP_CMD_RETRY)
            {
                retry = 0;
                _bEspCmdFinished(arg, ESP12F_CMD_RESULT_ERR);
            }
        }
    }
}

static void _bEspCmdStart(bDriverInterface_t *pdrv, uint8_t cmd, const uint8_t *plist,
                          uint8_t list_len, void *param, uint16_t param_len)
{
    bEsp12fPrivate_t *pinfo = (bEsp12fPrivate_t *)pdrv->_private._p;
    if (plist == NULL || list_len == 0)
    {
        return;
    }

    if (pinfo->opt_param != NULL)
    {
        _bEspCmdFinished(pdrv, ESP12F_CMD_RESULT_ERR);
    }

    if (param != NULL && param_len > 0)
    {
        pinfo->opt_param = bMalloc(param_len);
        if (pinfo->opt_param == NULL)
        {
            _bEspCmdFinished(pdrv, ESP12F_CMD_RESULT_ERR);
            return;
        }
        memcpy(pinfo->opt_param, param, param_len);
    }
    pinfo->opt.index    = 0;
    pinfo->opt.plist    = plist;
    pinfo->opt.list_len = list_len;
    pinfo->opt.at_id    = AT_INVALID_ID;
    pinfo->opt.cmd      = cmd;
    pinfo->result       = ESP12F_CMD_RESULT_NULL;
}

static uint8_t _bEspStaMode(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t retval = AT_INVALID_ID;
    retval =
        bAtCmdSend("AT+CWMODE=1\r\n", strlen("AT+CWMODE=1\r\n"), "OK", strlen("OK"), *_if, 500);
    return retval;
}

static uint8_t _bEspApMode(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t retval = AT_INVALID_ID;
    retval =
        bAtCmdSend("AT+CWMODE=2\r\n", strlen("AT+CWMODE=2\r\n"), "OK", strlen("OK"), *_if, 500);
    return retval;
}

static uint8_t _bEspStaApMode(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t retval = AT_INVALID_ID;
    retval =
        bAtCmdSend("AT+CWMODE=3\r\n", strlen("AT+CWMODE=3\r\n"), "OK", strlen("OK"), *_if, 500);
    return retval;
}

static uint8_t _bEspSetAp(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t        retval = AT_INVALID_ID;
    char           buf[200];
    int            len;
    bWifiApInfo_t *pinfo = (bWifiApInfo_t *)param;
    if (param == NULL)
    {
        return retval;
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
    if (len > 0)
    {
        retval = bAtCmdSend(buf, len, "OK", strlen("OK"), *_if, 500);
    }
    return retval;
}

static uint8_t _bEspJoinAp(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t        retval = AT_INVALID_ID;
    char           buf[200];
    int            len   = 0;
    bWifiApInfo_t *pinfo = (bWifiApInfo_t *)param;
    if (param == NULL)
    {
        return retval;
    }
    if (pinfo->encryption == 0 || strlen(pinfo->passwd) == 0)
    {
        len = snprintf(buf, 200, "AT+CWJAP=\"%s\",\"\"\r\n", pinfo->ssid);
    }
    else if (strlen(pinfo->passwd) > 0)
    {
        len = snprintf(buf, 200, "AT+CWJAP=\"%s\",\"%s\"\r\n", pinfo->ssid, pinfo->passwd);
    }
    if (len > 0)
    {
        retval = bAtCmdSend(buf, len, "OK", strlen("OK"), *_if, 8000);
    }
    return retval;
}

static uint8_t _bEspSetMux(bDriverInterface_t *pdrv, void *param)
{
    bEsp12fPrivate_t *pinfo = (bEsp12fPrivate_t *)pdrv->_private._p;
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t retval = AT_INVALID_ID;
    retval =
        bAtCmdSend("AT+CIPMUX=1\r\n", strlen("AT+CIPMUX=1\r\n"), "OK", strlen("OK"), *_if, 500);
    pinfo->cfg.mux_enable = 1;
    return retval;
}

static uint8_t _bEspDisableMux(bDriverInterface_t *pdrv, void *param)
{
    bEsp12fPrivate_t *pinfo = (bEsp12fPrivate_t *)pdrv->_private._p;
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t retval = AT_INVALID_ID;
    retval =
        bAtCmdSend("AT+CIPMUX=0\r\n", strlen("AT+CIPMUX=0\r\n"), "OK", strlen("OK"), *_if, 500);
    pinfo->cfg.mux_enable = 0;
    return retval;
}

static uint8_t _bEspSetAutoConn(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t retval = AT_INVALID_ID;
    retval = bAtCmdSend("AT+CWAUTOCONN=1\r\n", strlen("AT+CWAUTOCONN=1\r\n"), "OK", strlen("OK"),
                        *_if, 500);
    return retval;
}

static uint8_t _bEspEnterAt(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    return bAtCmdSend("+++", strlen("+++"), NULL, 0, *_if, 500);
}

static uint8_t _bEspAt(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    return bAtCmdSend("AT\r\n", strlen("AT\r\n"), "OK\r\n", strlen("OK\r\n"), *_if, 500);
}

static uint8_t _bEspReset(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t retval = AT_INVALID_ID;
    retval         = bAtCmdSend("AT+RST\r\n", strlen("AT+RST\r\n"), "OK", strlen("OK"), *_if, 500);
    return retval;
}

static uint8_t _bEspSetupTcpServer(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t        retval = AT_INVALID_ID;
    char           buf[32];
    uint8_t        len;
    bTcpUdpInfo_t *pinfo = (bTcpUdpInfo_t *)param;
    if (param == NULL)
    {
        return retval;
    }
    len = snprintf(buf, 32, "AT+CIPSERVER=%d,%d\r\n", 1, pinfo->port);
    if (len > 0)
    {
        retval = bAtCmdSend(buf, len, "OK", strlen("OK"), *_if, 500);
    }
    return retval;
}

static uint8_t _bEspMqttClose(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t retval = AT_INVALID_ID;
    retval = bAtCmdSend("AT+MQTTCLEAN=0\r\n", strlen("AT+MQTTCLEAN=0\r\n"), NULL, 0, *_if, 500);
    return retval;
}

static uint8_t _bEspCfgMqttUser(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t          retval = AT_INVALID_ID;
    char             buf[256];
    int              len   = 0;
    bMqttConnInfo_t *pinfo = (bMqttConnInfo_t *)param;
    if (param == NULL)
    {
        return retval;
    }
    len = snprintf(buf, 256, "AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n",
                   pinfo->device_id, pinfo->user, pinfo->passwd);
    if (len > 0)
    {
        retval = bAtCmdSend(buf, len, "OK", strlen("OK"), *_if, 1000);
    }
    return retval;
}

static uint8_t _bEspMqttConn(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t          retval = AT_INVALID_ID;
    char             buf[128];
    int              len   = 0;
    bMqttConnInfo_t *pinfo = (bMqttConnInfo_t *)param;
    if (param == NULL)
    {
        return retval;
    }
    len = snprintf(buf, 128, "AT+MQTTCONN=0,\"%s\",%d,1\r\n", pinfo->broker, pinfo->port);
    if (len > 0)
    {
        retval = bAtCmdSend(buf, len, "ERROR", strlen("ERROR"), *_if, 5000);
    }
    return retval;
}

static uint8_t _bEspMqttConnCheck(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t retval = AT_INVALID_ID;
    retval         = bAtCmdSend("AT+MQTTCONN?\r\n", strlen("AT+MQTTCONN?\r\n"), "+MQTTCONN:0,4",
                                strlen("+MQTTCONN:0,4"), *_if, 500);
    return retval;
}

static uint8_t _bEspMqttSub(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t           retval = AT_INVALID_ID;
    char              buf[128];
    int               len   = 0;
    bMqttTopicInfo_t *pinfo = (bMqttTopicInfo_t *)param;
    if (param == NULL)
    {
        return retval;
    }
    len = snprintf(buf, 128, "AT+MQTTSUB=0,\"%s\",%d\r\n", pinfo->topic, pinfo->qos);
    if (len > 0)
    {
        retval = bAtCmdSend(buf, len, "OK", strlen("OK"), *_if, 1000);
    }
    return retval;
}

static uint8_t _bEspMqttPub(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t           retval = AT_INVALID_ID;
    char             *pbuf   = NULL;
    int               len    = 0;
    bMqttTopicData_t *pinfo  = (bMqttTopicData_t *)param;
    if (param == NULL)
    {
        return retval;
    }
    if (pinfo->pstr == NULL)
    {
        return retval;
    }
    pbuf = (char *)bMalloc(64 + strlen(pinfo->topic.topic) + strlen(pinfo->pstr));
    if (pbuf == NULL)
    {
        return retval;
    }
    len = sprintf((char *)pbuf, "AT+MQTTPUB=0,\"%s\",\"%s\",%d,0\r\n", pinfo->topic.topic,
                  pinfo->pstr, pinfo->topic.qos);
    if (len > 0)
    {
        retval = bAtCmdSend(pbuf, len, "OK", strlen("OK"), *_if, 1000);
    }
    bFree(pbuf);
    return retval;
}

static uint8_t _bEspConnTcpServer(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t        retval = AT_INVALID_ID;
    char           buf[100];
    uint8_t        len;
    bTcpUdpInfo_t *pinfo = (bTcpUdpInfo_t *)param;
    if (param == NULL)
    {
        return retval;
    }
    len = sprintf(buf, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", pinfo->ip, pinfo->port);
    if (len > 0)
    {
        retval = bAtCmdSend(buf, len, "OK", strlen("OK"), *_if, 1000);
    }
    return retval;
}

static uint8_t _bEspConnUdpServer(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t        retval = AT_INVALID_ID;
    char           buf[100];
    uint8_t        len;
    bTcpUdpInfo_t *pinfo = (bTcpUdpInfo_t *)param;
    if (param == NULL)
    {
        return retval;
    }
    len = sprintf(buf, "AT+CIPSTART=\"UDP\",\"%s\",%d\r\n", pinfo->ip, pinfo->port);
    if (len > 0)
    {
        retval = bAtCmdSend(buf, len, "OK", strlen("OK"), *_if, 1000);
    }
    return retval;
}

static uint8_t _bEspTcpUdpSend1(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    bEsp12fPrivate_t *pinfo  = (bEsp12fPrivate_t *)pdrv->_private._p;
    uint8_t           retval = AT_INVALID_ID;
    bTcpUdpData_t    *pdata  = (bTcpUdpData_t *)param;
    char              buf[32];
    uint8_t           len;
    if (param == NULL)
    {
        return retval;
    }
    if (pinfo->cfg.mux_enable)
    {
        len = sprintf(buf, "AT+CIPSEND=%d,%d\r\n", pinfo->cfg.conn_id, pdata->len);
    }
    else
    {
        len = sprintf(buf, "AT+CIPSEND=%d\r\n", pdata->len);
    }

    if (len > 0)
    {
        retval = bAtCmdSend(buf, len, ">", strlen(">"), *_if, 500);
    }
    return retval;
}

static uint8_t _bEspTcpUdpSend2(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t        retval = AT_INVALID_ID;
    bTcpUdpData_t *pdata  = (bTcpUdpData_t *)param;
    if (param == NULL)
    {
        return retval;
    }
    retval = bAtCmdSend(pdata->pstr, pdata->len, "OK", strlen("OK"), *_if, 1000);
    return retval;
}

static uint8_t _bEspTcpUdpClose(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t retval = AT_INVALID_ID;
    retval = bAtCmdSend("AT+CIPCLOSE\r\n", strlen("AT+CIPCLOSE\r\n"), "ERROR", strlen("ERROR"),
                        *_if, 500);
    return retval;
}

static uint8_t _bEspPing(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t retval = AT_INVALID_ID;
    char    buf[100];
    uint8_t len;
    if (param == NULL)
    {
        return retval;
    }
    len = sprintf(buf, "AT+PING=\"%s\"\r\n", (char *)param);
    if (len > 0)
    {
        retval = bAtCmdSend(buf, len, "OK", strlen("OK"), *_if, 2000);
    }
    return retval;
}

static uint8_t _bEspEchoOff(bDriverInterface_t *pdrv, void *param)
{
    bDRIVER_GET_HALIF(_if, bESP12F_HalIf_t, pdrv);
    uint8_t retval = AT_INVALID_ID;
    retval         = bAtCmdSend("ATE0\r\n", strlen("ATE0\r\n"), "OK", strlen("OK"), *_if, 500);
    return retval;
}

static void _bEspPolling()
{
    int                 i     = 0;
    bDriverInterface_t *pdrv  = NULL;
    bEsp12fPrivate_t   *pinfo = NULL;
    for (i = 0; i < bDRIVER_HALIF_NUM(bESP12F_HalIf_t, DRIVER_NAME); i++)
    {
        pdrv  = bEspDrvTable[i];
        pinfo = (bEsp12fPrivate_t *)pdrv->_private._p;
        if (pdrv == NULL || pinfo == NULL)
        {
            continue;
        }
        if (pinfo->opt.plist == NULL || pinfo->opt.at_id != AT_INVALID_ID)
        {
            continue;
        }
        if (!IS_ESP_OPT(pinfo->opt.plist[pinfo->opt.index]))
        {
            continue;
        }
        pinfo->opt.at_id =
            bOptFuncTable[pinfo->opt.plist[pinfo->opt.index]](pdrv, pinfo->opt_param);
    }
}

BOS_REG_POLLING_FUNC(_bEspPolling);

static int _bEspCtl(bDriverInterface_t *pdrv, uint8_t cmd, void *param)
{
    int retval = -1;
    switch (cmd)
    {
        case bCMD_WIFI_MODE_STA:
            _bEspCmdStart(pdrv, cmd, bWifiStaOptList, sizeof(bWifiStaOptList), NULL, 0);
            break;
        case bCMD_WIFI_MODE_AP:
            if (param != NULL)
            {
                _bEspCmdStart(pdrv, cmd, bWifiApOptList, sizeof(bWifiApOptList), param,
                              sizeof(bWifiApInfo_t));
            }
            break;
        case bCMD_WIFI_MODE_STA_AP:
            if (param != NULL)
            {
                _bEspCmdStart(pdrv, cmd, bWifiStaApOptList, sizeof(bWifiStaApOptList), param,
                              sizeof(bWifiApInfo_t));
            }
            break;
        case bCMD_WIFI_JOIN_AP:
            if (param != NULL)
            {
                _bEspCmdStart(pdrv, cmd, bWifiJoinApOptList, sizeof(bWifiJoinApOptList), param,
                              sizeof(bWifiApInfo_t));
            }
            break;
        case bCMD_WIFI_MQTT_CONN:
            if (param != NULL)
            {
                _bEspCmdStart(pdrv, cmd, bWifiMqttConnOptList, sizeof(bWifiMqttConnOptList), param,
                              sizeof(bMqttConnInfo_t));
            }
            break;
        case bCMD_WIFI_MQTT_SUB:
            if (param != NULL)
            {
                _bEspCmdStart(pdrv, cmd, bWifiMqttSubOptList, sizeof(bWifiMqttSubOptList), param,
                              sizeof(bMqttTopicInfo_t));
            }
            break;
        case bCMD_WIFI_MQTT_PUB:
            if (param != NULL)
            {
                _bEspCmdStart(pdrv, cmd, bWifiMqttPubOptList, sizeof(bWifiMqttPubOptList), param,
                              sizeof(bMqttTopicData_t));
            }
            break;
        case bCMD_WIFI_LOCAL_TCP_SERVER:
            if (param != NULL)
            {
                _bEspCmdStart(pdrv, cmd, bWifiSetupTcpServerOptList,
                              sizeof(bWifiSetupTcpServerOptList), param, sizeof(bTcpUdpInfo_t));
            }
            break;
        case bCMD_WIFI_REMOT_TCP_SERVER:
            if (param != NULL)
            {
                _bEspCmdStart(pdrv, cmd, bWifiConnTcpServerOptList,
                              sizeof(bWifiConnTcpServerOptList), param, sizeof(bTcpUdpInfo_t));
            }
            break;
        case bCMD_WIFI_REMOT_UDP_SERVER:
            if (param != NULL)
            {
                _bEspCmdStart(pdrv, cmd, bWifiConnUdpServerOptList,
                              sizeof(bWifiConnUdpServerOptList), param, sizeof(bTcpUdpInfo_t));
            }
            break;
        case bCMD_WIFI_TCP_UDP_SEND:
            if (param != NULL)
            {
                _bEspCmdStart(pdrv, cmd, bWifiTcpSendOptList, sizeof(bWifiTcpSendOptList), param,
                              sizeof(bTcpUdpData_t));
            }
            break;
        case bCMD_WIFI_PING:
            if (param != NULL)
            {
                _bEspCmdStart(pdrv, cmd, bWifiPingOptList, sizeof(bWifiPingOptList), param,
                              strlen(param));
            }
            break;
        default:
            break;
    }
    return retval;
}

static int _bEspRead(bDriverInterface_t *pdrv, uint32_t offset, uint8_t *pbuf, uint32_t len)
{
    int               retval = 0;
    bEsp12fPrivate_t *pinfo  = (bEsp12fPrivate_t *)pdrv->_private._p;
    if (len < sizeof(bWiFiData_t) || pbuf == NULL || pinfo->lock)
    {
        return -1;
    }
    if (pinfo->wifi_rec_data.mqtt.pstr == NULL && pinfo->tcp_data_list.total_size == 0)
    {
        return 0;
    }
    pinfo->lock = 1;
    if (pinfo->tcp_data_list.total_size > 0)
    {
        pinfo->wifi_rec_data.tcp.pstr = (char *)bMempList2Array(&pinfo->tcp_data_list);
        pinfo->wifi_rec_data.tcp.len  = pinfo->tcp_data_list.total_size;
        bMempListFree(&pinfo->tcp_data_list);
    }
    memcpy(pbuf, &pinfo->wifi_rec_data, sizeof(bWiFiData_t));
    //读取并使用数据后，需要释放 bWiFiRecData.mqtt.pstr 和 bWiFiRecData.tcp.pstr 指向的内存
    if (pinfo->wifi_rec_data.mqtt.pstr != NULL)
    {
        retval += strlen(pinfo->wifi_rec_data.mqtt.pstr);
        pinfo->wifi_rec_data.mqtt.pstr = NULL;
    }
    if (pinfo->wifi_rec_data.tcp.pstr != NULL)
    {
        retval += pinfo->wifi_rec_data.tcp.len;
        pinfo->wifi_rec_data.tcp.pstr = NULL;
    }
    pinfo->lock = 0;
    return retval;
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

    bDRIVER_STRUCT_INIT(pdrv, DRIVER_NAME, bESP12F_Init);
    pdrv->read        = _bEspRead;
    pdrv->ctl         = _bEspCtl;
    pdrv->_private._p = &bEspRunInfo[index];

    pinfo = (bEsp12fPrivate_t *)pdrv->_private._p;
    memset(pinfo, 0, sizeof(bEsp12fPrivate_t));
    pinfo->opt.at_id    = AT_INVALID_ID;
    bEspDrvTable[index] = pdrv;
    bAtRegistCallback(_bEspAtCb, pdrv);
    bMempListInit(&(pinfo->tcp_data_list));
    bUtilUartInitStruct(&(pinfo->uart), pinfo->rec_buf, ESP12F_REC_BUF_LEN, 20, _bEspUartIdleCb,
                        pdrv);
    bUtilUartBind(*((bESP12F_HalIf_t *)pdrv->hal_if),
                  &(((bEsp12fPrivate_t *)pdrv->_private._p)->uart));
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
