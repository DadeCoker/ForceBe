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

#include "utils/inc/b_utils.h"

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
 * \defgroup ESP12F_Private_TypesDefinitions
 * \{
 */
typedef struct
{
    const uint8_t *plist;
    uint8_t        index;
    uint8_t        list_len;
    uint8_t        at_id;
    uint8_t        cmd;
} bEspOptInfo_t;

typedef uint8_t (*pOptFunc_t)(void *param);

/**
 * \}
 */

/**
 * \defgroup ESP12F_Private_Defines
 * \{
 */

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
static int _bEspUartIdleCb(uint8_t *pbuf, uint16_t len);

static uint8_t _bEspEnterAt(void *param);
static uint8_t _bEspAt(void *param);
static uint8_t _bEspReset(void *param);
static uint8_t _bEspSetMux(void *param);
static uint8_t _bEspSetAutoConn(void *param);
static uint8_t _bEspStaMode(void *param);
static uint8_t _bEspJoinAp(void *param);
static uint8_t _bEspSetAp(void *param);
static uint8_t _bEspApMode(void *param);
static uint8_t _bEspStaApMode(void *param);
static uint8_t _bEspSetupTcpServer(void *param);
static uint8_t _bEspMqttClose(void *param);
static uint8_t _bEspCfgMqttUser(void *param);
static uint8_t _bEspMqttConn(void *param);
static uint8_t _bEspMqttConnCheck(void *param);
static uint8_t _bEspMqttSub(void *param);
static uint8_t _bEspMqttPub(void *param);
static uint8_t _bEspConnTcpServer(void *param);
static uint8_t _bEspConnUdpServer(void *param);
static uint8_t _bEspTcpUdpSend1(void *param);
static uint8_t _bEspTcpUdpSend2(void *param);
static uint8_t _bEspTcpUdpClose(void *param);
static uint8_t _bEspDisableMux(void *param);
static uint8_t _bEspPing(void *param);
static uint8_t _bEspEchoOff(void *param);
/**
 * \}
 */

/**
 * \defgroup ESP12F_Private_Variables
 * \{
 */
HALIF_KEYWORD bESP12F_HalIf_t bESP12F_HalIf = HAL_ESP12F_IF;
bESP12F_Driver_t              bESP12F_Driver;

bUTIL_UART_INSTANCE(bEspUart, 1536, 20, _bEspUartIdleCb);

static bEspOptInfo_t bEspOptInfo;

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
static void       *bParam       = NULL;
static bWiFiData_t bWiFiRecData = {
    .tcp.pstr  = NULL,
    .mqtt.pstr = NULL,
};

static volatile uint8_t bWiFiRecDataLock = 0;
static bMempList_t      bTcpDataList;
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

static int _bEspRecHandler(uint8_t *pbuf, uint16_t len)
{
    char *retp   = NULL;
    char *p      = NULL;
    int   retval = 0, id = 0, rlen = 0;
    char  tmp[64];
    char *pcheckend = NULL;
    if (bWiFiRecDataLock)
    {
        return -1;
    }

    bWiFiRecDataLock = 1;
    retp             = strstr((const char *)pbuf, "+MQTTSUBRECV");
    if (retp != NULL && bWiFiRecData.mqtt.pstr == NULL)
    {
        memset(tmp, 0, sizeof(tmp));
        p = (char *)bMalloc(len);
        if (p == NULL)
        {
            bWiFiRecDataLock = 0;
            return -2;
        }
        retval = sscanf((const char *)retp, "+MQTTSUBRECV:%d,%[^,],%d,%s", &id, tmp, &rlen, p);
        if (rlen <= 0 || rlen >= len || retval != 4)
        {
            bFree(p);
            bWiFiRecDataLock = 0;
            return -2;
        }
        bWiFiRecData.mqtt.pstr = p;
        memset(bWiFiRecData.mqtt.topic.topic, 0, sizeof(bWiFiRecData.mqtt.topic.topic));
        memcpy(bWiFiRecData.mqtt.topic.topic, tmp, strlen(tmp));
        bWiFiRecDataLock = 0;
        return 0;
    }
    retp = strstr((const char *)pbuf, "+IPD");
    while (retp)
    {
        retval = sscanf((const char *)retp, "+IPD,%d:%*s", &rlen);
        if (retval == 1 && rlen > 0 && rlen < len)
        {
            retp = retp + 5 + _bEspNumLen(rlen) + 1;
            bMempListAdd(&bTcpDataList, (uint8_t *)retp, rlen);
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
                bWiFiRecDataLock = 0;
                return -3;  //没有接收完整，继续接收
            }
        }
        retval = sscanf((const char *)retp, "+HTTPCLIENT:%d,%*s", &rlen);
        if (retval == 1 && rlen > 0 && rlen < len)
        {
            retp = retp + strlen("+HTTPCLIENT:") + _bEspNumLen(rlen) + 1;
            bMempListAdd(&bTcpDataList, (uint8_t *)retp, rlen);
            retp += rlen;
        }
        else
        {
            break;
        }
        retp = strstr((const char *)retp, "+HTTPCLIENT:");
    }
    bWiFiRecDataLock = 0;
    return 0;
}

static int _bEspUartIdleCb(uint8_t *pbuf, uint16_t len)
{
    int retval = 0;
    bAtFeedRespData(pbuf, len);
    if (_bEspRecHandler(pbuf, len) == -3)
    {
        retval = -1;
    }
    return retval;
}

static void _bEspCmdFinished(uint8_t result)
{
    bESP12F_Driver._private.v = result;

    if (bParam != NULL)
    {
        bFree(bParam);
        bParam = NULL;
    }

    bEspOptInfo.index    = 0;
    bEspOptInfo.list_len = 0;
    bEspOptInfo.plist    = NULL;
    bEspOptInfo.at_id    = AT_INVALID_ID;
}

static void _bEspAtCb(uint8_t id, uint8_t result)
{
    static uint8_t retry = 0;
    if (bEspOptInfo.at_id == id && id != AT_INVALID_ID)
    {
        bEspOptInfo.at_id = AT_INVALID_ID;
        if (result == AT_STA_OK)
        {
            bEspOptInfo.index += 1;
            if (bEspOptInfo.index < bEspOptInfo.list_len)
            {
                if (bEspOptInfo.plist[bEspOptInfo.index] == OPT_NULL)
                {
                    _bEspCmdFinished(ESP_CMD_RESULT_OK);
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
                _bEspCmdFinished(ESP_CMD_RESULT_ERR);
            }
        }
    }
}

static void _bEspCmdStart(uint8_t cmd, const uint8_t *plist, uint8_t list_len, void *param,
                          uint16_t param_len)
{
    if (plist == NULL || list_len == 0)
    {
        return;
    }

    if (bParam != NULL)
    {
        _bEspCmdFinished(ESP_CMD_RESULT_ERR);
    }

    if (param != NULL && param_len > 0)
    {
        bParam = bMalloc(param_len);
        if (bParam == NULL)
        {
            _bEspCmdFinished(ESP_CMD_RESULT_ERR);
            return;
        }
        memcpy(bParam, param, param_len);
    }
    bEspOptInfo.index    = 0;
    bEspOptInfo.plist    = plist;
    bEspOptInfo.list_len = list_len;
    bEspOptInfo.at_id    = AT_INVALID_ID;
    bEspOptInfo.cmd      = cmd;

    bESP12F_Driver._private.v = ESP_CMD_RESULT_NULL;
}

static uint8_t _bEspStaMode(void *param)
{
    uint8_t retval = AT_INVALID_ID;
    retval         = bAtCmdSend("AT+CWMODE=1\r\n", strlen("AT+CWMODE=1\r\n"), "OK", strlen("OK"),
                                bESP12F_HalIf, 500);
    return retval;
}

static uint8_t _bEspApMode(void *param)
{
    uint8_t retval = AT_INVALID_ID;
    retval         = bAtCmdSend("AT+CWMODE=2\r\n", strlen("AT+CWMODE=2\r\n"), "OK", strlen("OK"),
                                bESP12F_HalIf, 500);
    return retval;
}

static uint8_t _bEspStaApMode(void *param)
{
    uint8_t retval = AT_INVALID_ID;
    retval         = bAtCmdSend("AT+CWMODE=3\r\n", strlen("AT+CWMODE=3\r\n"), "OK", strlen("OK"),
                                bESP12F_HalIf, 500);
    return retval;
}

static uint8_t _bEspSetAp(void *param)
{
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
        retval = bAtCmdSend(buf, len, "OK", strlen("OK"), bESP12F_HalIf, 500);
    }
    return retval;
}

static uint8_t _bEspJoinAp(void *param)
{
    uint8_t        retval = AT_INVALID_ID;
    char           buf[200];
    int            len;
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
        retval = bAtCmdSend(buf, len, "OK", strlen("OK"), bESP12F_HalIf, 8000);
    }
    return retval;
}

static uint8_t _bEspSetMux(void *param)
{
    uint8_t retval = AT_INVALID_ID;
    retval         = bAtCmdSend("AT+CIPMUX=1\r\n", strlen("AT+CIPMUX=1\r\n"), "OK", strlen("OK"),
                                bESP12F_HalIf, 500);
    return retval;
}

static uint8_t _bEspDisableMux(void *param)
{
    uint8_t retval = AT_INVALID_ID;
    retval         = bAtCmdSend("AT+CIPMUX=0\r\n", strlen("AT+CIPMUX=0\r\n"), "OK", strlen("OK"),
                                bESP12F_HalIf, 500);
    return retval;
}

static uint8_t _bEspSetAutoConn(void *param)
{
    uint8_t retval = AT_INVALID_ID;
    retval = bAtCmdSend("AT+CWAUTOCONN=1\r\n", strlen("AT+CWAUTOCONN=1\r\n"), "OK", strlen("OK"),
                        bESP12F_HalIf, 500);
    return retval;
}

static uint8_t _bEspEnterAt(void *param)
{
    return bAtCmdSend("+++", strlen("+++"), NULL, 0, bESP12F_HalIf, 500);
}

static uint8_t _bEspAt(void *param)
{
    return bAtCmdSend("AT\r\n", strlen("AT\r\n"), "OK\r\n", strlen("OK\r\n"), bESP12F_HalIf, 500);
}

static uint8_t _bEspReset(void *param)
{
    uint8_t retval = AT_INVALID_ID;
    retval = bAtCmdSend("AT+RST\r\n", strlen("AT+RST\r\n"), "OK", strlen("OK"), bESP12F_HalIf, 500);
    return retval;
}

static uint8_t _bEspSetupTcpServer(void *param)
{
    uint8_t        retval = AT_INVALID_ID;
    char           buf[32];
    uint8_t        len;
    bTcpUdpInfo_t *pinfo = (bTcpUdpInfo_t *)param;
    if (param == NULL)
    {
        return retval;
    }
    len = snprintf(buf, 200, "AT+CIPSERVER=%d,%d\r\n", 1, pinfo->port);
    if (len > 0)
    {
        retval = bAtCmdSend(buf, len, "OK", strlen("OK"), bESP12F_HalIf, 500);
    }
    return retval;
}

static uint8_t _bEspMqttClose(void *param)
{
    uint8_t retval = AT_INVALID_ID;
    retval =
        bAtCmdSend("AT+MQTTCLEAN=0\r\n", strlen("AT+MQTTCLEAN=0\r\n"), NULL, 0, bESP12F_HalIf, 500);
    return retval;
}

static uint8_t _bEspCfgMqttUser(void *param)
{
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
        retval = bAtCmdSend(buf, len, "OK", strlen("OK"), bESP12F_HalIf, 1000);
    }
    return retval;
}

static uint8_t _bEspMqttConn(void *param)
{
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
        retval = bAtCmdSend(buf, len, "ERROR", strlen("ERROR"), bESP12F_HalIf, 5000);
    }
    return retval;
}

static uint8_t _bEspMqttConnCheck(void *param)
{
    uint8_t retval = AT_INVALID_ID;
    retval         = bAtCmdSend("AT+MQTTCONN?\r\n", strlen("AT+MQTTCONN?\r\n"), "+MQTTCONN:0,4",
                                strlen("+MQTTCONN:0,4"), bESP12F_HalIf, 500);
    return retval;
}

static uint8_t _bEspMqttSub(void *param)
{
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
        retval = bAtCmdSend(buf, len, "OK", strlen("OK"), bESP12F_HalIf, 1000);
    }
    return retval;
}

static uint8_t _bEspMqttPub(void *param)
{
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
        retval = bAtCmdSend(pbuf, len, "OK", strlen("OK"), bESP12F_HalIf, 1000);
    }
    bFree(pbuf);
    return retval;
}

static uint8_t _bEspConnTcpServer(void *param)
{
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
        retval = bAtCmdSend(buf, len, "OK", strlen("OK"), bESP12F_HalIf, 1000);
    }
    return retval;
}

static uint8_t _bEspConnUdpServer(void *param)
{
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
        retval = bAtCmdSend(buf, len, "OK", strlen("OK"), bESP12F_HalIf, 1000);
    }
    return retval;
}

static uint8_t _bEspTcpUdpSend1(void *param)
{
    uint8_t        retval = AT_INVALID_ID;
    bTcpUdpData_t *pdata  = (bTcpUdpData_t *)param;
    char           buf[32];
    uint8_t        len;
    if (param == NULL)
    {
        return retval;
    }
    len = sprintf(buf, "AT+CIPSEND=%d\r\n", pdata->len);
    if (len > 0)
    {
        retval = bAtCmdSend(buf, len, ">", strlen(">"), bESP12F_HalIf, 500);
    }
    return retval;
}

static uint8_t _bEspTcpUdpSend2(void *param)
{
    uint8_t        retval = AT_INVALID_ID;
    bTcpUdpData_t *pdata  = (bTcpUdpData_t *)param;
    if (param == NULL)
    {
        return retval;
    }
    retval = bAtCmdSend(pdata->pstr, pdata->len, "OK", strlen("OK"), bESP12F_HalIf, 1000);
    return retval;
}

static uint8_t _bEspTcpUdpClose(void *param)
{
    uint8_t retval = AT_INVALID_ID;
    retval = bAtCmdSend("AT+CIPCLOSE\r\n", strlen("AT+CIPCLOSE\r\n"), "ERROR", strlen("ERROR"),
                        bESP12F_HalIf, 500);
    return retval;
}

static uint8_t _bEspPing(void *param)
{
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
        retval = bAtCmdSend(buf, len, "OK", strlen("OK"), bESP12F_HalIf, 2000);
    }
    return retval;
}

static uint8_t _bEspEchoOff(void *param)
{
    uint8_t retval = AT_INVALID_ID;
    retval = bAtCmdSend("ATE0\r\n", strlen("ATE0\r\n"), "OK", strlen("OK"), bESP12F_HalIf, 500);
    return retval;
}

static void _bEspPolling()
{
    if (bEspOptInfo.plist == NULL || bEspOptInfo.at_id != AT_INVALID_ID)
    {
        return;
    }
    if (!IS_ESP_OPT(bEspOptInfo.plist[bEspOptInfo.index]))
    {
        return;
    }
    bEspOptInfo.at_id = bOptFuncTable[(bEspOptInfo.plist[bEspOptInfo.index])](bParam);
}

BOS_REG_POLLING_FUNC(_bEspPolling);

static int _bEspCtl(bESP12F_Driver_t *pdrv, uint8_t cmd, void *param)
{
    int retval = -1;
    switch (cmd)
    {
        case bCMD_WIFI_MODE_STA:
            _bEspCmdStart(cmd, bWifiStaOptList, sizeof(bWifiStaOptList), NULL, 0);
            break;
        case bCMD_WIFI_MODE_AP:
            if (param != NULL)
            {
                _bEspCmdStart(cmd, bWifiApOptList, sizeof(bWifiApOptList), param,
                              sizeof(bWifiApInfo_t));
            }
            break;
        case bCMD_WIFI_MODE_STA_AP:
            if (param != NULL)
            {
                _bEspCmdStart(cmd, bWifiStaApOptList, sizeof(bWifiStaApOptList), param,
                              sizeof(bWifiApInfo_t));
            }
            break;
        case bCMD_WIFI_JOIN_AP:
            if (param != NULL)
            {
                _bEspCmdStart(cmd, bWifiJoinApOptList, sizeof(bWifiJoinApOptList), param,
                              sizeof(bWifiApInfo_t));
            }
            break;
        case bCMD_WIFI_MQTT_CONN:
            if (param != NULL)
            {
                _bEspCmdStart(cmd, bWifiMqttConnOptList, sizeof(bWifiMqttConnOptList), param,
                              sizeof(bMqttConnInfo_t));
            }
            break;
        case bCMD_WIFI_MQTT_SUB:
            if (param != NULL)
            {
                _bEspCmdStart(cmd, bWifiMqttSubOptList, sizeof(bWifiMqttSubOptList), param,
                              sizeof(bMqttTopicInfo_t));
            }
            break;
        case bCMD_WIFI_MQTT_PUB:
            if (param != NULL)
            {
                _bEspCmdStart(cmd, bWifiMqttPubOptList, sizeof(bWifiMqttPubOptList), param,
                              sizeof(bMqttTopicData_t));
            }
            break;
        case bCMD_WIFI_LOCAL_TCP_SERVER:
            if (param != NULL)
            {
                _bEspCmdStart(cmd, bWifiSetupTcpServerOptList, sizeof(bWifiSetupTcpServerOptList),
                              param, sizeof(bTcpUdpInfo_t));
            }
            break;
        case bCMD_WIFI_REMOT_TCP_SERVER:
            if (param != NULL)
            {
                _bEspCmdStart(cmd, bWifiConnTcpServerOptList, sizeof(bWifiConnTcpServerOptList),
                              param, sizeof(bTcpUdpInfo_t));
            }
            break;
        case bCMD_WIFI_REMOT_UDP_SERVER:
            if (param != NULL)
            {
                _bEspCmdStart(cmd, bWifiConnUdpServerOptList, sizeof(bWifiConnUdpServerOptList),
                              param, sizeof(bTcpUdpInfo_t));
            }
            break;
        case bCMD_WIFI_TCP_SEND:
            if (param != NULL)
            {
                _bEspCmdStart(cmd, bWifiTcpSendOptList, sizeof(bWifiTcpSendOptList), param,
                              sizeof(bTcpUdpData_t));
            }
            break;
        case bCMD_WIFI_PING:
            if (param != NULL)
            {
                _bEspCmdStart(cmd, bWifiPingOptList, sizeof(bWifiPingOptList), param,
                              strlen(param));
            }
            break;
        default:
            break;
    }
    return retval;
}

static int _bEspRead(bESP12F_Driver_t *pdrv, uint32_t offset, uint8_t *pbuf, uint16_t len)
{
    int retval = 0;
    if (len < sizeof(bWiFiData_t) || pbuf == NULL || bWiFiRecDataLock)
    {
        return -1;
    }
    if (bWiFiRecData.mqtt.pstr == NULL && bTcpDataList.total_size == 0)
    {
        return 0;
    }
    bWiFiRecDataLock = 1;
    if (bTcpDataList.total_size > 0)
    {
        bWiFiRecData.tcp.pstr = (char *)bMempList2Array(&bTcpDataList);
        bWiFiRecData.tcp.len  = bTcpDataList.total_size;
        bMempListFree(&bTcpDataList);
    }
    memcpy(pbuf, &bWiFiRecData, sizeof(bWiFiData_t));
    //读取并使用数据后，需要释放 bWiFiRecData.mqtt.pstr 和 bWiFiRecData.tcp.pstr 指向的内存
    if (bWiFiRecData.mqtt.pstr != NULL)
    {
        retval += strlen(bWiFiRecData.mqtt.pstr);
        bWiFiRecData.mqtt.pstr = NULL;
    }
    if (bWiFiRecData.tcp.pstr != NULL)
    {
        retval += bWiFiRecData.tcp.len;
        bWiFiRecData.tcp.pstr = NULL;
    }
    bWiFiRecDataLock = 0;
    return retval;
}

/**
 * \}
 */

/**
 * \addtogroup ESP12F_Exported_Functions
 * \{
 */
int bESP12F_Init()
{
    bEspOptInfo.plist    = NULL;
    bEspOptInfo.index    = 0;
    bEspOptInfo.list_len = 0;
    bEspOptInfo.at_id    = AT_INVALID_ID;
    bAtRegistCallback(_bEspAtCb);

    bMempListInit(&bTcpDataList);

    bESP12F_Driver.status  = 0;
    bESP12F_Driver.init    = bESP12F_Init;
    bESP12F_Driver.close   = NULL;
    bESP12F_Driver.read    = _bEspRead;
    bESP12F_Driver.ctl     = _bEspCtl;
    bESP12F_Driver.open    = NULL;
    bESP12F_Driver.write   = NULL;
    bESP12F_Driver._hal_if = (void *)&bESP12F_HalIf;

    bUtilUartBind(bESP12F_HalIf, &bEspUart);
    return 0;
}

bDRIVER_REG_INIT(bESP12F_Init);

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
