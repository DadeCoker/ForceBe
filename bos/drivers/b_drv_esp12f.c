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

#define ESP_CONN_ID_MAX (5)
/**
 * \}
 */

/**
 * \defgroup ESP12F_Private_TypesDefinitions
 * \{
 */

typedef uint16_t (*bEsp12fAtCmd_t)(uint8_t index, void *param);

typedef struct
{
    char    resp[AT_CMD_RESPON_LEN + 1];
    uint8_t ack;
} bEsp12fAtCmdAck_t;

typedef union
{
    bApInfo_t       ap;
    bTcpUdpInfo_t   tcpudp;
    bTcpUdpData_t   tcpudp_data;
    bMqttConnInfo_t mqtt_client;
    bMqttTopic_t    mqtt_topic;
    bMqttData_t     mqtt_data;
} bEsp12fCmdParam_t;

typedef struct
{
    uint8_t               mux_enable;
    uint8_t               retry;
    uint8_t               ctl_cmd;
    bWifiModuleCallback_t cb;
    bUitlUartInstance_t   uart;
    bEsp12fAtCmdAck_t     at_ack;
    bEsp12fAtCmd_t       *pcmd;
    bEsp12fCmdParam_t     param;
    bTcpUdpInfo_t         conn[ESP_CONN_ID_MAX];
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

/**
 * \}
 */

/**
 * \defgroup ESP12F_Private_Variables
 * \{
 */

bDRIVER_HALIF_TABLE(bESP12F_HalIf_t, DRIVER_NAME);

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

static void _bAtCmdSetResp(uint8_t index, char *p)
{
    memset(&bEspRunInfo[index].at_ack, 0, sizeof(bEsp12fAtCmdAck_t));
    memcpy(bEspRunInfo[index].at_ack.resp, p,
           (strlen(p) > AT_CMD_RESPON_LEN) ? AT_CMD_RESPON_LEN : strlen(p));
    bEspRunInfo[index].at_ack.ack = AT_CMD_ACK_WAIT;
}

//==================================================================================AT Command==

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
    char       buf[200];
    int        len;
    bApInfo_t *pinfo = (bApInfo_t *)param;
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
    char       buf[200];
    int        len   = 0;
    bApInfo_t *pinfo = (bApInfo_t *)param;
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
    char          buf[128];
    int           len   = 0;
    bMqttTopic_t *pinfo = (bMqttTopic_t *)param;
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

static uint16_t _bEspMqttPub(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    bMqttData_t *pdata = (bMqttData_t *)param;
    if (param == NULL || pdata->pbuf == NULL)
    {
        return 0;
    }
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (const uint8_t *)"AT+MQTTPUB=0,\"", strlen("AT+MQTTPUB=0,\""));
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (const uint8_t *)pdata->topic.topic, strlen(pdata->topic.topic));
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (const uint8_t *)"\",\"", strlen("\",\""));
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))), pdata->pbuf,
                 pdata->len);
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (const uint8_t *)"\",", strlen("\","));
    uint8_t qos = pdata->topic.qos + 0x30;
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))), &qos, 1);
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (const uint8_t *)",0\r\n", strlen(",0\r\n"));
    if (pdata->release)
    {
        pdata->release(pdata->pbuf);
        pdata->pbuf = NULL;
    }
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
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (uint8_t *)"AT+PING=\"www.baidu.com\"\r\n",
                 strlen("AT+PING=\"www.baidu.com\"\r\n"));
    return AT_CMD_ACK_MS_MAX;
}

static uint16_t _bEspEchoOff(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (uint8_t *)"ATE0\r\n", strlen("ATE0\r\n"));
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspIPPortOn(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (uint8_t *)"AT+CIPDINFO=1\r\n", strlen("AT+CIPDINFO=1\r\n"));
    return AT_CMD_ACK_MS;
}

static uint16_t _bEspIPPortOff(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                 (uint8_t *)"AT+CIPDINFO=0\r\n", strlen("AT+CIPDINFO=0\r\n"));
    return AT_CMD_ACK_MS;
}

static int _bEspFindConnid(uint8_t index, char ip[64], uint16_t port)
{
    int i = 0;
    for (i = 0; i < ESP_CONN_ID_MAX; i++)
    {
        if (bEspRunInfo[index].conn[i].port == port &&
            (0 ==
             strncmp(bEspRunInfo[index].conn[i].ip, (const char *)ip, strlen((const char *)ip))))
        {
            return i;
        }
    }
    return -1;
}

static uint16_t _bEspTcpUdpSend1(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    char           buf[100];
    uint8_t        len;
    bTcpUdpData_t *pdata = (bTcpUdpData_t *)param;
    if (param == NULL || pdata->pbuf == NULL)
    {
        return 0;
    }
    int conn_id = _bEspFindConnid(index, pdata->conn.ip, pdata->conn.port);
    if (conn_id < 0 || conn_id >= ESP_CONN_ID_MAX)
    {
        return 0;
    }
    if (bEspRunInfo[index].mux_enable)
    {
        len = sprintf(buf, "AT+CIPSEND=%d,%d\r\n", conn_id, pdata->len);
    }
    else
    {
        len = sprintf(buf, "AT+CIPSEND=%d\r\n", pdata->len);
    }
    if (len > 0)
    {
        _bAtCmdSetResp(index, ">");
        bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))),
                     (uint8_t *)buf, len);
        return AT_CMD_ACK_MS;
    }
    return 0;
}

static uint16_t _bEspTcpUdpSend2(uint8_t index, void *param)
{
    b_log("[%d]%s\r\n", index, __func__);
    bTcpUdpData_t *pdata = (bTcpUdpData_t *)param;
    if (param == NULL || pdata->pbuf == NULL)
    {
        return 0;
    }
    _bAtCmdSetResp(index, "OK");
    bHalUartSend(*((bESP12F_HalIf_t *)(bDRIVER_HALIF_INSTANCE(DRIVER_NAME, index))), pdata->pbuf,
                 pdata->len);
    return AT_CMD_ACK_MS;
}

//===================================================================================AT End====

const static bEsp12fAtCmd_t bWifiStaOptList[] = {
    _bEspEnterAt, _bEspEnterAt,  _bEspAt,     _bEspReset,       _bEspEnterAt,
    _bEspEnterAt, _bEspAt,       _bEspSetMux, _bEspSetAutoConn, _bEspStaMode,
    _bEspEchoOff, _bEspIPPortOn, NULL};
const static bEsp12fAtCmd_t bWifiApOptList[] = {
    _bEspEnterAt, _bEspEnterAt,     _bEspAt,     _bEspReset, _bEspEnterAt, _bEspEnterAt,  _bEspAt,
    _bEspSetMux,  _bEspSetAutoConn, _bEspApMode, _bEspSetAp, _bEspEchoOff, _bEspIPPortOn, NULL};
const static bEsp12fAtCmd_t bWifiStaApOptList[] = {
    _bEspEnterAt,  _bEspEnterAt, _bEspAt,          _bEspReset,     _bEspEnterAt, _bEspEnterAt,
    _bEspAt,       _bEspSetMux,  _bEspSetAutoConn, _bEspStaApMode, _bEspSetAp,   _bEspEchoOff,
    _bEspIPPortOn, NULL};
const static bEsp12fAtCmd_t bWifiJoinApOptList[]         = {_bEspJoinAp, NULL};
const static bEsp12fAtCmd_t bWifiSetupTcpServerOptList[] = {_bEspSetupTcpServer, NULL};
const static bEsp12fAtCmd_t bWifiConnTcpServerOptList[]  = {_bEspConnTcpServer, NULL};
const static bEsp12fAtCmd_t bWifiConnUdpServerOptList[]  = {_bEspConnUdpServer, NULL};
const static bEsp12fAtCmd_t bWifiMqttConnOptList[]       = {
    _bEspMqttClose, _bEspMqttClose, _bEspCfgMqttUser, _bEspMqttConn, _bEspMqttConnCheck, NULL};
const static bEsp12fAtCmd_t bWifiMqttSubOptList[] = {_bEspMqttSub, NULL};
const static bEsp12fAtCmd_t bWifiMqttPubOptList[] = {_bEspMqttPub, NULL};
const static bEsp12fAtCmd_t bWifiPingOptList[]    = {_bEspPing, NULL};
const static bEsp12fAtCmd_t bWifiTcpSendOptList[] = {_bEspTcpUdpSend1, _bEspTcpUdpSend2, NULL};
//-----------------------------------------------------------------------------------------

static int _bEspRecHandler(bEsp12fPrivate_t *pinfo, uint8_t *pbuf, uint16_t len)
{
    int   retval    = -1;
    char *p         = NULL;
    p               = strstr((const char *)pbuf, ",CONNECT");
    int     conn_id = 0;
    int     rlen    = 0;
    int     port    = 0;
    uint8_t ip[64];

    if (p != NULL)
    {
        conn_id = *(p - 1) - 0x30;
        if (conn_id >= 0 && conn_id < ESP_CONN_ID_MAX)
        {
            pinfo->conn[conn_id].port = 0xffff;
        }
    }
    p = strstr((const char *)pbuf, ",CLOSED");
    if (p != NULL)
    {
        conn_id = *(p - 1) - 0x30;
        if (conn_id >= 0 && conn_id < ESP_CONN_ID_MAX)
        {
            memset(&pinfo->conn[conn_id], 0, sizeof(bTcpUdpInfo_t));
        }
    }
    //+IPD,0,3,"192.168.4.2",46596:123
    p = strstr((const char *)pbuf, "+IPD,");
    if (p != NULL)
    {
        memset(ip, 0, sizeof(ip));
        retval = sscanf(p, "+IPD,%d,%d,\"%[^\"]\",%d:%*s", &conn_id, &rlen, ip, &port);
        if (retval == 4)
        {
            if (conn_id >= 0 && conn_id < ESP_CONN_ID_MAX)
            {
                if (pinfo->conn[conn_id].port == port &&
                    (0 ==
                     strncmp(pinfo->conn[conn_id].ip, (const char *)ip, strlen((const char *)ip))))
                {
                    ;
                }
                else
                {
                    pinfo->conn[conn_id].port = port;
                    memcpy(pinfo->conn[conn_id].ip, ip, sizeof(pinfo->conn[conn_id].ip));
                }
            }
            p = strstr((const char *)p, ":");
            if (p != NULL)
            {
                bTcpUdpData_t *pdata = (bTcpUdpData_t *)bMalloc(sizeof(bTcpUdpData_t));
                if (pdata != NULL)
                {
                    memcpy(pdata->conn.ip, pinfo->conn[conn_id].ip, sizeof(pdata->conn.ip));
                    pdata->conn.port = pinfo->conn[conn_id].port;
                    pdata->len       = rlen;
                    pdata->pbuf      = (uint8_t *)(p + 1);
                    pdata->release   = NULL;
                    if (pinfo->cb.cb)
                    {
                        pinfo->cb.cb(WIFI_DRV_EVT_DATA, pdata, bFree, pinfo->cb.user_data);
                    }
                }
            }
        }
    }
    //+MQTTSUBRECV:0,"babyos",5,12345
    p = strstr((const char *)pbuf, "+MQTTSUBRECV:");
    if (p != NULL)
    {
        bMqttData_t *pmqttdata = bMalloc(sizeof(bMqttData_t));
        if (pmqttdata != NULL)
        {
            retval = sscanf(p, "+MQTTSUBRECV:0,\"%[^\"]\",%d,%*s", pmqttdata->topic.topic, &rlen);
            if (retval == 2)
            {
                pmqttdata->len     = rlen;
                pmqttdata->pbuf    = bMalloc(rlen);
                pmqttdata->release = bFree;
                if (pmqttdata->pbuf == NULL)
                {
                    bFree(pmqttdata);
                }
                else
                {
                    p = ((char *)p) + strlen("+MQTTSUBRECV:0,\"\",,") +
                        strlen(pmqttdata->topic.topic) + _bEspNumLen(rlen);
                    memcpy(pmqttdata->pbuf, p, rlen);
                    if (pinfo->cb.cb)
                    {
                        pinfo->cb.cb(WIFI_DRV_EVT_MQTT_DATA, pmqttdata, bFree, pinfo->cb.user_data);
                    }
                }
            }
            else
            {
                bFree(pmqttdata);
            }
        }
    }

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

//-----------------------------------------------------------------------------------------

static void _bEspResult(uint8_t index, uint8_t result)
{
    bEspRunInfo[index].pcmd  = NULL;
    bEspRunInfo[index].retry = 0;

    if (bEspRunInfo[index].ctl_cmd == bCMD_WIFI_TCPUDP_SEND)
    {
        if (bEspRunInfo[index].param.tcpudp_data.release)
        {
            bEspRunInfo[index].param.tcpudp_data.release(bEspRunInfo[index].param.tcpudp_data.pbuf);
            bEspRunInfo[index].param.tcpudp_data.pbuf = NULL;
        }
    }
    else if (bEspRunInfo[index].ctl_cmd == bCMD_WIFI_MQTT_PUB)
    {
        if (bEspRunInfo[index].param.mqtt_data.release)
        {
            bEspRunInfo[index].param.mqtt_data.release(bEspRunInfo[index].param.mqtt_data.pbuf);
            bEspRunInfo[index].param.mqtt_data.pbuf = NULL;
        }
    }

    if (bEspRunInfo[index].cb.cb)
    {
        bEspRunInfo[index].cb.cb((result == 0) ? WIFI_DRV_EVT_CMD_ERR : WIFI_DRV_EVT_CMD_OK,
                                 &bEspRunInfo[index].ctl_cmd, NULL,
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
        retval = (*(bEspRunInfo[index].pcmd))(index, &bEspRunInfo[index].param);
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

//----------------------------------------------------------------------------------------------

static int _bEspCtlStart(bEsp12fPrivate_t *pinfo, uint8_t cmd, bEsp12fAtCmd_t *pcmd, void *param,
                         uint16_t param_len)
{
    if (pinfo->pcmd != NULL)
    {
        return -2;
    }
    pinfo->retry   = 0;
    pinfo->ctl_cmd = cmd;
    pinfo->pcmd    = pcmd;
    if (param_len > 0)
    {
        memset(&pinfo->param, 0, sizeof(pinfo->param));
        memcpy(&pinfo->param, param, param_len);
    }
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
                                   param, 0);
            break;
        case bCMD_WIFI_MODE_AP:
            if (param != NULL)
            {
                retval =
                    _bEspCtlStart(pinfo, bCMD_WIFI_MODE_AP, (bEsp12fAtCmd_t *)&bWifiApOptList[0],
                                  param, sizeof(bApInfo_t));
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
                                       (bEsp12fAtCmd_t *)&bWifiStaApOptList[0], param,
                                       sizeof(bApInfo_t));
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
                                       (bEsp12fAtCmd_t *)&bWifiJoinApOptList[0], param,
                                       sizeof(bApInfo_t));
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
                                       (bEsp12fAtCmd_t *)&bWifiMqttConnOptList[0], param,
                                       sizeof(bMqttConnInfo_t));
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
                                       (bEsp12fAtCmd_t *)&bWifiMqttSubOptList[0], param,
                                       sizeof(bMqttTopic_t));
            }
            else
            {
                retval = -1;
            }
            break;
        case bCMD_WIFI_MQTT_PUB:
            if (param != NULL)
            {
                retval = _bEspCtlStart(pinfo, bCMD_WIFI_MQTT_PUB,
                                       (bEsp12fAtCmd_t *)&bWifiMqttPubOptList[0], param,
                                       sizeof(bMqttData_t));
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
                                       (bEsp12fAtCmd_t *)&bWifiSetupTcpServerOptList[0], param,
                                       sizeof(bTcpUdpInfo_t));
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
                                       (bEsp12fAtCmd_t *)&bWifiConnTcpServerOptList[0], param,
                                       sizeof(bTcpUdpInfo_t));
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
                                       (bEsp12fAtCmd_t *)&bWifiConnUdpServerOptList[0], param,
                                       sizeof(bTcpUdpInfo_t));
            }
            else
            {
                retval = -1;
            }
            break;
        case bCMD_WIFI_PING:
            retval = _bEspCtlStart(pinfo, bCMD_WIFI_PING, (bEsp12fAtCmd_t *)&bWifiPingOptList[0],
                                   param, 0);
            break;
        case bCMD_WIFI_TCPUDP_SEND:
            if (param != NULL)
            {
                retval = _bEspCtlStart(pinfo, bCMD_WIFI_TCPUDP_SEND,
                                       (bEsp12fAtCmd_t *)&bWifiTcpSendOptList[0], param,
                                       sizeof(bTcpUdpData_t));
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
    pdrv->ctl = _bEspCtl;
    memset(&bEspRunInfo[index], 0, sizeof(bEsp12fPrivate_t));
    pdrv->_private._p = &bEspRunInfo[index];

    bUTIL_UART_INIT_INSTANCE((&(bEspRunInfo[index].uart)), ESP12F_UART_RX_BUF_LEN, 200,
                             _bEspUartIdleCb, &(bEspRunInfo[index]));
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
