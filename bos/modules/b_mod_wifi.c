/**
 *!
 * \file        b_mod_wifi.c
 * \version     v0.0.1
 * \date        2020/05/16
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
#include "modules/inc/b_mod_wifi.h"

#include "b_section.h"
#include "core/inc/b_core.h"
#include "drivers/inc/b_driver.h"
#include "thirdparty/pt/pt.h"
#include "utils/inc/b_utils.h"

#if (defined(_WIFI_ENABLE) && (_WIFI_ENABLE == 1))
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup WIFI
 * \{
 */

/**
 * \defgroup WIFI_Private_TypesDefinitions
 * \{
 */
typedef struct
{
    char     ip[64];
    uint16_t port;
} bWifiServerInfo_t;

typedef struct
{
    uint8_t          type;
    char             ip[64];
    uint16_t         port;
    uint16_t         server_port;
    bFIFO_Instance_t sfifo;
    bFIFO_Instance_t rfifo;
} bWifiNetCtx_t;

typedef struct
{
    void            *pdev;
    bWifiNetCtx_t   *pctx;
    struct list_head list;
} bWifiNetCtxList_t;

typedef struct
{
    int                result;
    uint32_t           dev;
    int                fd;
    bWifiEventHandle_t cb;
    bWifiNetCtxList_t  ctx;
} bWifiDevice_t;

typedef struct
{
    bWifiDevice_t *handle;
    uint8_t        type;
    void          *param;
    void (*release)(void *);
} bWifiQItem_t;

/**
 * \}
 */

/**
 * \defgroup WIFI_Private_Defines
 * \{
 */
#define WIFI_CONFIG_STA (0)
#define WIFI_CONFIG_AP (1)
#define WIFI_CONFIG_AP_STA (2)
#define WIFI_JOINAP_START (3)
#define WIFI_START_PING (4)
#define WIFI_CREATE_TCP_SERVER (5)
#define WIFI_CREATE_UDP_SERVER (6)
#define WIFI_CONNECT_TCP_SERVER (7)
#define WIFI_CONNECT_UDP_SERVER (8)
#define WIFI_CONNECT_CLOSE (9)
#define WIFI_CONNECT_SEND (10)
#define WIFI_CONNECT_MQTT (11)
#define WIFI_CONNECT_MQTT_SUB (12)
#define WIFI_CONNECT_MQTT_PUB (13)

#define WIFI_CONN_TYPE_NULL (0)
#define WIFI_CONN_TYPE_TCP_SERVER (1)
#define WIFI_CONN_TYPE_UDP_SERVER (2)
#define WIFI_CONN_TYPE_TCP_CLIENT (3)
#define WIFI_CONN_TYPE_UDP_CLIENT (4)

/**
 * \}
 */

/**
 * \defgroup WIFI_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup WIFI_Private_Variables
 * \{
 */

bQUEUE_INSTANCE(bModWifiQueue, sizeof(bWifiQItem_t), WIFI_Q_LEN);
PT_INSTANCE(bModWifiPT);
static bWifiDevice_t bWifiDeviceTable[WIFI_DEVICE_NUMBER];
/**
 * \}
 */

/**
 * \defgroup WIFI_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup WIFI_Private_Functions
 * \{
 */

static void _bWifiDeleteNetCtx(bWifiNetCtxList_t *p)
{
    bWifiNetCtx_t *pctx = p->pctx;

    __list_del(p->list.prev, p->list.next);

    bFIFO_DynDelete(&pctx->rfifo);
    bFIFO_DynDelete(&pctx->sfifo);

    bFree(pctx);
    bFree(p);
}

static void _bWifiClearNetCtx(bWifiNetCtxList_t *ctx_head)
{
    bWifiNetCtxList_t *ctx_list = NULL;
    struct list_head  *pnode    = NULL;
    list_for_each(pnode, &ctx_head->list)
    {
        ctx_list = list_entry(pnode, bWifiNetCtxList_t, list);
        if (ctx_list)
        {
            _bWifiDeleteNetCtx(ctx_list);
        }
    }
}

static uint8_t _bWifiIpIsZero(char ip[64])
{
    int i = 0;
    for (i = 0; i < 64; i++)
    {
        if (ip[i] != 0)
        {
            return 0;
        }
    }
    return 1;
}

static bWifiNetCtxList_t *_bWifiFindNetCtx(bWifiNetCtxList_t *ctx_head, const char *ip,
                                           uint16_t port)
{
    bWifiNetCtxList_t *ctx_list    = NULL;
    bWifiNetCtxList_t *pctx_list_z = NULL;
    bWifiNetCtx_t     *pctx        = NULL;
    struct list_head  *pnode       = NULL;
    list_for_each(pnode, &ctx_head->list)
    {
        ctx_list = list_entry(pnode, bWifiNetCtxList_t, list);
        if (ctx_list == NULL)
        {
            return NULL;
        }
        pctx = ctx_list->pctx;

        if (ip == NULL || _bWifiIpIsZero((char *)ip))
        {
            if (port == pctx->server_port)
            {
                return ctx_list;
            }
            continue;
        }

        if (_bWifiIpIsZero(pctx->ip))
        {
            pctx_list_z = ctx_list;
        }

        if (pctx->port == port && memcmp(pctx->ip, ip, sizeof(pctx->ip)) == 0)
        {
            return ctx_list;
        }
    }
    if (pctx_list_z != NULL)
    {
        memcpy(pctx_list_z->pctx->ip, ip, sizeof(pctx_list_z->pctx->ip));
        pctx_list_z->pctx->port = port;
        return pctx_list_z;
    }
    return NULL;
}

// id | ip | port 不必全部都有
static bWifiNetCtxList_t *_bWifiAddNetCtx(bWifiNetCtxList_t *ctx_head, uint8_t type, const char *ip,
                                          uint16_t port)
{
    bWifiNetCtx_t     *pctx     = NULL;
    bWifiNetCtxList_t *pctxlist = NULL;

    if ((pctxlist = _bWifiFindNetCtx(ctx_head, ip, port)) != NULL)
    {
        return pctxlist;
    }

    pctx = bMalloc(sizeof(bWifiNetCtx_t));
    if (pctx == NULL)
    {
        return NULL;
    }
    b_log("pctx %p \r\n", pctx);
    memset(pctx, 0, sizeof(bWifiNetCtx_t));

    pctxlist = bMalloc(sizeof(bWifiNetCtxList_t));
    if (pctxlist == NULL)
    {
        bFree(pctx);
        return NULL;
    }
    pctxlist->pctx = pctx;

    if (bFIFO_DynCreate(&pctx->rfifo, WIFI_RECV_BUF_LEN) < 0)
    {
        bFree(pctx);
        bFree(pctxlist);
        return NULL;
    }

    if (bFIFO_DynCreate(&pctx->sfifo, WIFI_SEND_BUF_LEN) < 0)
    {
        bFree(pctx);
        bFree(pctxlist);
        bFIFO_DynDelete(&pctx->rfifo);
        return NULL;
    }

    pctx->type = type;
    if (ip != NULL)
    {
        pctx->port = port;
        memcpy(pctx->ip, ip, sizeof(pctx->ip));
    }
    else
    {
        pctx->server_port = port;
    }

    list_add(&pctxlist->list, &ctx_head->list);
    return pctxlist;
}

static void _bWifiEventHandler(bWifiModuleEvent_t event, void *arg, void (*release)(void *),
                               void *user_data)
{
    bWifiDevice_t *pdev = (bWifiDevice_t *)user_data;
    uint8_t        cmd  = 0;
    if (event == WIFI_DRV_EVT_CMD_OK || event == WIFI_DRV_EVT_CMD_ERR)
    {
        pdev->result = event;
        cmd          = *((uint8_t *)arg);
    }

    if (event == WIFI_DRV_EVT_CMD_OK)
    {
        if (cmd == bCMD_WIFI_MODE_STA)
        {
            pdev->cb(user_data, WIFI_EVT_STA_MODE, NULL);
        }
        else if (cmd == bCMD_WIFI_MODE_AP)
        {
            pdev->cb(user_data, WIFI_EVT_AP_MODE, NULL);
        }
        else if (cmd == bCMD_WIFI_MODE_STA_AP)
        {
            pdev->cb(user_data, WIFI_EVT_AP_STA_MODE, NULL);
        }
        else if (cmd == bCMD_WIFI_JOIN_AP)
        {
            pdev->cb(user_data, WIFI_EVT_JOINAP_OK, NULL);
        }
        else if (cmd == bCMD_WIFI_PING)
        {
            pdev->cb(user_data, WIFI_EVT_PING_OK, NULL);
        }
        else if (cmd == bCMD_WIFI_LOCAL_TCP_SERVER)
        {
            pdev->cb(user_data, WIFI_EVT_SETUP_TCPSERVER_OK, NULL);
        }
        else if (cmd == bCMD_WIFI_MQTT_CONN)
        {
            pdev->cb(user_data, WIFI_EVT_MQTT_CONN_OK, NULL);
        }
        else if (cmd == bCMD_WIFI_MQTT_SUB)
        {
            pdev->cb(user_data, WIFI_EVT_MQTT_SUB_OK, NULL);
        }
        else if (cmd == bCMD_WIFI_MQTT_PUB)
        {
            pdev->cb(user_data, WIFI_EVT_MQTT_PUB_OK, NULL);
        }
    }
    else if (event == WIFI_DRV_EVT_CMD_ERR)
    {
        if (cmd == bCMD_WIFI_MODE_STA)
        {
            pdev->cb(user_data, WIFI_EVT_STA_ERR, NULL);
        }
        else if (cmd == bCMD_WIFI_MODE_AP)
        {
            pdev->cb(user_data, WIFI_EVT_AP_ERR, NULL);
        }
        else if (cmd == bCMD_WIFI_MODE_STA_AP)
        {
            pdev->cb(user_data, WIFI_EVT_AP_STA_ERR, NULL);
        }
        else if (cmd == bCMD_WIFI_JOIN_AP)
        {
            pdev->cb(user_data, WIFI_EVT_JOINAP_ERR, NULL);
        }
        else if (cmd == bCMD_WIFI_PING)
        {
            pdev->cb(user_data, WIFI_EVT_PING_ERR, NULL);
        }
        else if (cmd == bCMD_WIFI_LOCAL_TCP_SERVER)
        {
            pdev->cb(user_data, WIFI_EVT_SETUP_TCPSERVER_ERR, NULL);
        }
        else if (cmd == bCMD_WIFI_MQTT_CONN)
        {
            pdev->cb(user_data, WIFI_EVT_MQTT_CONN_ERR, NULL);
        }
        else if (cmd == bCMD_WIFI_MQTT_SUB)
        {
            pdev->cb(user_data, WIFI_EVT_MQTT_SUB_ERR, NULL);
        }
        else if (cmd == bCMD_WIFI_MQTT_PUB)
        {
            pdev->cb(user_data, WIFI_EVT_MQTT_PUB_ERR, NULL);
        }
    }
    else if (event == WIFI_DRV_EVT_DATA)
    {
        bWifiNetCtxList_t *pctx_list = NULL;
        bTcpUdpData_t     *pdata     = (bTcpUdpData_t *)arg;
        pctx_list = _bWifiFindNetCtx(&pdev->ctx, pdata->conn.ip, pdata->conn.port);
        if (pctx_list != NULL)
        {
            bFIFO_Write(&pctx_list->pctx->rfifo, pdata->pbuf, pdata->len);
            if (pdata->release)
            {
                pdata->release(pdata->pbuf);
            }
            pdev->cb(user_data, WIFI_EVT_DATA_READY, NULL);
        }
    }
    else if (event == WIFI_DRV_EVT_MQTT_DATA)
    {
        bWifiMqttData_t mqttdata;
        bMqttData_t    *pmqttdata = (bMqttData_t *)arg;
        memset(&mqttdata, 0, sizeof(bWifiMqttData_t));
        mqttdata.len     = pmqttdata->len;
        mqttdata.release = pmqttdata->release;
        mqttdata.pbuf    = pmqttdata->pbuf;
        memcpy(mqttdata.topic, pmqttdata->topic.topic, sizeof(mqttdata.topic));
        pdev->cb(user_data, WIFI_EVT_MQTT_DATA, &mqttdata);
    }
    if (release && arg)
    {
        release(arg);
    }
}

static int _bModWifiTask(struct pt *pt)
{
    static bWifiQItem_t item;
    void               *param = NULL;
    int                 wait  = 0;
    PT_BEGIN(pt);
    while (1)
    {
        if (bQueuePop(&bModWifiQueue, &item) < 0)
        {
            return -1;
        }
        wait                = 1;
        item.handle->result = -1;
        if (item.type == WIFI_CONFIG_STA)
        {
            bCtl(item.handle->fd, bCMD_WIFI_MODE_STA, NULL);
        }
        else if (item.type == WIFI_CONFIG_AP || item.type == WIFI_CONFIG_AP_STA)
        {
            param = bMalloc(sizeof(bApInfo_t));
            if (param == NULL)
            {
                wait = 0;
            }
            else
            {
                memset(param, 0, sizeof(bApInfo_t));
                memcpy(((bApInfo_t *)param)->ssid, ((bWifiApInfo_t *)item.param)->ssid,
                       sizeof(((bApInfo_t *)param)->ssid));
                memcpy(((bApInfo_t *)param)->passwd, ((bWifiApInfo_t *)item.param)->passwd,
                       sizeof(((bApInfo_t *)param)->passwd));
                ((bApInfo_t *)param)->encryption = ((bWifiApInfo_t *)item.param)->encryption;
                if (WIFI_CONFIG_AP_STA == item.type)
                {
                    bCtl(item.handle->fd, bCMD_WIFI_MODE_STA_AP, param);
                }
                else
                {
                    bCtl(item.handle->fd, bCMD_WIFI_MODE_AP, param);
                }
                bFree(param);
                param = NULL;
            }
        }
        else if (item.type == WIFI_JOINAP_START)
        {
            param = bMalloc(sizeof(bApInfo_t));
            if (param == NULL)
            {
                wait = 0;
            }
            else
            {
                memset(param, 0, sizeof(bApInfo_t));
                memcpy(((bApInfo_t *)param)->ssid, ((bWifiApInfo_t *)item.param)->ssid,
                       sizeof(((bApInfo_t *)param)->ssid));
                memcpy(((bApInfo_t *)param)->passwd, ((bWifiApInfo_t *)item.param)->passwd,
                       sizeof(((bApInfo_t *)param)->passwd));
                ((bApInfo_t *)param)->encryption = ((bWifiApInfo_t *)item.param)->encryption;
                bCtl(item.handle->fd, bCMD_WIFI_JOIN_AP, param);
                bFree(param);
                param = NULL;
            }
        }
        else if (item.type == WIFI_START_PING)
        {
            bCtl(item.handle->fd, bCMD_WIFI_PING, NULL);
        }
        else if (item.type == WIFI_CREATE_TCP_SERVER || item.type == WIFI_CREATE_UDP_SERVER ||
                 item.type == WIFI_CONNECT_TCP_SERVER || item.type == WIFI_CONNECT_UDP_SERVER ||
                 item.type == WIFI_CONNECT_CLOSE)
        {
            param = bMalloc(sizeof(bTcpUdpInfo_t));
            if (param == NULL)
            {
                wait = 0;
            }
            else
            {
                memset(param, 0, sizeof(bTcpUdpInfo_t));
                ((bTcpUdpInfo_t *)param)->port = *((uint16_t *)item.param);
                if (WIFI_CREATE_TCP_SERVER == item.type)
                {
                    bCtl(item.handle->fd, bCMD_WIFI_LOCAL_TCP_SERVER, param);
                }
                else if (WIFI_CREATE_UDP_SERVER == item.type)
                {
                    bCtl(item.handle->fd, bCMD_WIFI_LOCAL_UDP_SERVER, param);
                }
                else if (WIFI_CONNECT_TCP_SERVER == item.type)
                {
                    memcpy(((bTcpUdpInfo_t *)param)->ip, ((bWifiServerInfo_t *)item.param)->ip,
                           sizeof(((bTcpUdpInfo_t *)param)->ip));
                    bCtl(item.handle->fd, bCMD_WIFI_REMOT_TCP_SERVER, param);
                }
                else if (WIFI_CONNECT_UDP_SERVER == item.type)
                {
                    memcpy(((bTcpUdpInfo_t *)param)->ip, ((bWifiServerInfo_t *)item.param)->ip,
                           sizeof(((bTcpUdpInfo_t *)param)->ip));
                    bCtl(item.handle->fd, bCMD_WIFI_REMOT_UDP_SERVER, param);
                }
                else if (WIFI_CONNECT_CLOSE == item.type)
                {
                    memcpy(((bTcpUdpInfo_t *)param)->ip, ((bWifiServerInfo_t *)item.param)->ip,
                           sizeof(((bTcpUdpInfo_t *)param)->ip));
                    bCtl(item.handle->fd, bCMD_WIFI_TCPUDP_CLOSE, param);
                }
                bFree(param);
                param = NULL;
            }
        }
        else if (item.type == WIFI_CONNECT_SEND)
        {
            bWifiNetCtx_t *pctx = (bWifiNetCtx_t *)item.param;
            param               = bMalloc(sizeof(bTcpUdpData_t));
            if (param == NULL)
            {
                wait = 0;
            }
            else
            {

                memset(param, 0, sizeof(bTcpUdpData_t));
                bFIFO_Length(&pctx->sfifo, &(((bTcpUdpData_t *)param)->len));
                if (((bTcpUdpData_t *)param)->len > 0)
                {
                    ((bTcpUdpData_t *)param)->pbuf = bMalloc(((bTcpUdpData_t *)param)->len);
                }
                if (((bTcpUdpData_t *)param)->pbuf != NULL)
                {
                    ((bTcpUdpData_t *)param)->release = bFree;
                    bFIFO_Read(&pctx->sfifo, ((bTcpUdpData_t *)param)->pbuf,
                               ((bTcpUdpData_t *)param)->len);
                    memcpy(((bTcpUdpData_t *)param)->conn.ip, pctx->ip,
                           sizeof(((bTcpUdpData_t *)param)->conn.ip));
                    ((bTcpUdpData_t *)param)->conn.port = pctx->port;
                    bCtl(item.handle->fd, bCMD_WIFI_TCPUDP_SEND, param);
                }
                bFree(param);
                param = NULL;
            }
        }
        else if (item.type == WIFI_CONNECT_MQTT)
        {
            bWifiMqtt_t *pmqtt = (bWifiMqtt_t *)item.param;
            param              = bMalloc(sizeof(bMqttConnInfo_t));
            if (param == NULL)
            {
                wait = 0;
            }
            else
            {
                memset(param, 0, sizeof(bMqttConnInfo_t));

                memcpy(((bMqttConnInfo_t *)param)->broker, pmqtt->broker,
                       sizeof(((bMqttConnInfo_t *)param)->broker));
                ((bMqttConnInfo_t *)param)->port = pmqtt->port;
                memcpy(((bMqttConnInfo_t *)param)->device_id, pmqtt->device_id,
                       sizeof(((bMqttConnInfo_t *)param)->device_id));
                memcpy(((bMqttConnInfo_t *)param)->user, pmqtt->user,
                       sizeof(((bMqttConnInfo_t *)param)->user));
                memcpy(((bMqttConnInfo_t *)param)->passwd, pmqtt->passwd,
                       sizeof(((bMqttConnInfo_t *)param)->passwd));
                bCtl(item.handle->fd, bCMD_WIFI_MQTT_CONN, param);
                bFree(param);
                param = NULL;
            }
        }
        else if (item.type == WIFI_CONNECT_MQTT_SUB)
        {
            uint8_t *p = (uint8_t *)item.param;
            param      = bMalloc(sizeof(bMqttTopic_t));
            if (param == NULL)
            {
                wait = 0;
            }
            else
            {
                memset(param, 0, sizeof(bMqttTopic_t));
                ((bMqttTopic_t *)param)->qos = p[0];
                p += 1;
                memcpy(((bMqttTopic_t *)param)->topic, p, strlen((const char *)p));
                bCtl(item.handle->fd, bCMD_WIFI_MQTT_SUB, param);
                bFree(param);
                param = NULL;
            }
        }
        else if (item.type == WIFI_CONNECT_MQTT_PUB)
        {
            uint8_t *p = (uint8_t *)item.param;
            param      = bMalloc(sizeof(bMqttData_t));
            if (param == NULL)
            {
                wait = 0;
            }
            else
            {
                memset(param, 0, sizeof(bMqttData_t));
                ((bMqttData_t *)param)->topic.qos = p[0];
                p += 1;
                memcpy(((bMqttData_t *)param)->topic.topic, p, strlen((const char *)p));
                p += strlen((const char *)p) + 1;
                ((bMqttData_t *)param)->len = p[1];
                ((bMqttData_t *)param)->len = ((bMqttData_t *)param)->len << 8;
                ((bMqttData_t *)param)->len |= p[0];
                p += 2;
                ((bMqttData_t *)param)->pbuf = bMalloc(((bMqttData_t *)param)->len);
                if (((bMqttData_t *)param)->pbuf != NULL)
                {
                    ((bMqttData_t *)param)->release = bFree;
                    memcpy(((bMqttData_t *)param)->pbuf, p, ((bMqttData_t *)param)->len);
                    bCtl(item.handle->fd, bCMD_WIFI_MQTT_PUB, param);
                }
                bFree(param);
                param = NULL;
            }
        }
        else
        {
            wait = 0;
        }
        if (wait == 1)
        {
            PT_WAIT_UNTIL_FOREVER(pt, item.handle->result >= 0);
        }
        if (item.release)
        {
            item.release(item.param);
        }
        memset(&item, 0, sizeof(bWifiQItem_t));
    }
    PT_END(pt);
}

static void _bModWifiPolling()
{
    _bModWifiTask(&bModWifiPT);
}

BOS_REG_POLLING_FUNC(_bModWifiPolling);

/**
 * \}
 */

/**
 * \addtogroup WIFI_Exported_Functions
 * \{
 */

int bWifiInit()
{
    memset(bWifiDeviceTable, 0, sizeof(bWifiDeviceTable));
    PT_INIT(&bModWifiPT);
    return 0;
}

bWifiHandle_t bWifiUp(uint32_t dev_no, bWifiEventHandle_t cb)
{
    bWifiModuleCallback_t drv_cb;
    bWifiDevice_t        *pdev = NULL;

    if (cb == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < WIFI_DEVICE_NUMBER; i++)
    {
        if (bWifiDeviceTable[i].dev == 0)
        {
            pdev = &bWifiDeviceTable[i];
        }
    }
    if (pdev == NULL)
    {
        return NULL;
    }
    pdev->dev = dev_no;
    pdev->cb  = cb;
    pdev->fd  = bOpen(dev_no, BCORE_FLAG_RW);
    if (pdev->fd < 0)
    {
        return NULL;
    }
    drv_cb.cb        = _bWifiEventHandler;
    drv_cb.user_data = pdev;
    bCtl(pdev->fd, bCMD_WIFI_REG_CALLBACK, &drv_cb);
    INIT_LIST_HEAD(&pdev->ctx.list);
    return pdev;
}

int bWifiDown(bWifiHandle_t handle)
{
    bWifiDevice_t *pdev = (bWifiDevice_t *)handle;
    if (handle == NULL)
    {
        return -1;
    }
    bClose(pdev->fd);
    _bWifiClearNetCtx(&pdev->ctx);
    memset(pdev, 0, sizeof(bWifiDevice_t));
    return 0;
}

int bWifiSetMode(bWifiHandle_t handle, uint8_t mode, const bWifiApInfo_t *pinfo)
{
    bWifiQItem_t   item;
    bWifiDevice_t *pdev = (bWifiDevice_t *)handle;
    if (handle == NULL || !IS_WIFI_MODE(mode))
    {
        return -1;
    }
    if (mode != WIFI_MODE_STA && pinfo == NULL)
    {
        return -1;
    }
    _bWifiClearNetCtx(&pdev->ctx);
    memset(&item, 0, sizeof(item));
    if (mode == WIFI_MODE_STA)
    {
        item.type = WIFI_CONFIG_STA;
    }
    else if (mode == WIFI_MODE_AP)
    {
        item.type = WIFI_CONFIG_AP;
    }
    else if (mode == WIFI_MODE_AP_STA)
    {
        item.type = WIFI_CONFIG_AP_STA;
    }
    item.handle = handle;
    if (mode != WIFI_MODE_STA)
    {
        item.param = bMalloc(sizeof(bWifiApInfo_t));
        if (item.param == NULL)
        {
            return -1;
        }
        item.release = bFree;
        memcpy(item.param, pinfo, sizeof(bWifiApInfo_t));
    }
    if (0 > bQueuePush(&bModWifiQueue, &item))
    {
        if (item.param != NULL)
        {
            bFree(item.param);
        }
        return -1;
    }
    return 0;
}

int bWifiJoinAp(bWifiHandle_t handle, const bWifiApInfo_t *pinfo)
{
    bWifiQItem_t   item;
    bWifiDevice_t *pdev = (bWifiDevice_t *)handle;
    if (handle == NULL)
    {
        return -1;
    }

    item.param = bMalloc(sizeof(bWifiApInfo_t));
    if (item.param == NULL)
    {
        return -1;
    }
    item.release = bFree;
    memcpy(item.param, pinfo, sizeof(bWifiApInfo_t));

    item.type   = WIFI_JOINAP_START;
    item.handle = handle;
    if (0 > bQueuePush(&bModWifiQueue, &item))
    {
        if (item.param != NULL)
        {
            bFree(item.param);
        }
        return -1;
    }
    return 0;
}

int bWifiPing(bWifiHandle_t handle)
{
    bWifiQItem_t   item;
    bWifiDevice_t *pdev = (bWifiDevice_t *)handle;
    if (handle == NULL)
    {
        return -1;
    }
    item.type    = WIFI_START_PING;
    item.handle  = handle;
    item.param   = NULL;
    item.release = NULL;
    return bQueuePush(&bModWifiQueue, &item);
}

bWifiConnHandle_t bWifiSetupServer(bWifiHandle_t handle, uint8_t type, uint16_t port)
{
    bWifiQItem_t   item;
    bWifiDevice_t *pdev = (bWifiDevice_t *)handle;
    if (handle == NULL || !IS_WIFI_SERVER_TYPE(type))
    {
        return NULL;
    }

    bWifiNetCtxList_t *plist = NULL;
    if (type == WIFI_SERVER_TYPE_TCP)
    {
        plist = _bWifiAddNetCtx(&pdev->ctx, WIFI_CONN_TYPE_TCP_SERVER, NULL, port);
    }
    else if (type == WIFI_SERVER_TYPE_UDP)
    {
        plist = _bWifiAddNetCtx(&pdev->ctx, WIFI_CONN_TYPE_UDP_SERVER, NULL, port);
    }

    if (plist == NULL)
    {
        return NULL;
    }
    plist->pdev = pdev;
    if (type == WIFI_SERVER_TYPE_TCP)
    {
        item.type = WIFI_CREATE_TCP_SERVER;
    }
    else if (type == WIFI_SERVER_TYPE_UDP)
    {
        item.type = WIFI_CREATE_UDP_SERVER;
    }
    item.handle = handle;
    item.param  = bMalloc(sizeof(uint16_t));
    if (item.param == NULL)
    {
        return NULL;
    }
    item.release              = bFree;
    *((uint16_t *)item.param) = port;
    if (0 > bQueuePush(&bModWifiQueue, &item))
    {
        bFree(item.param);
        _bWifiDeleteNetCtx(plist);
        return NULL;
    }
    return plist;
}

bWifiConnHandle_t bWifiConnectServer(bWifiHandle_t handle, uint8_t type, const char *ip,
                                     uint16_t port)
{
    bWifiQItem_t   item;
    bWifiDevice_t *pdev = (bWifiDevice_t *)handle;
    if (handle == NULL || !IS_WIFI_SERVER_TYPE(type) || ip == NULL)
    {
        return NULL;
    }

    bWifiNetCtxList_t *plist = NULL;
    if (type == WIFI_SERVER_TYPE_TCP)
    {
        plist = _bWifiAddNetCtx(&pdev->ctx, WIFI_CONN_TYPE_TCP_CLIENT, ip, port);
    }
    else if (type == WIFI_SERVER_TYPE_UDP)
    {
        plist = _bWifiAddNetCtx(&pdev->ctx, WIFI_CONN_TYPE_UDP_CLIENT, ip, port);
    }
    if (plist == NULL)
    {
        return NULL;
    }
    plist->pdev = pdev;
    if (type == WIFI_SERVER_TYPE_TCP)
    {
        item.type = WIFI_CONNECT_TCP_SERVER;
    }
    else if (type == WIFI_SERVER_TYPE_UDP)
    {
        item.type = WIFI_CONNECT_UDP_SERVER;
    }
    item.handle = handle;
    item.param  = bMalloc(sizeof(bWifiServerInfo_t));
    if (item.param == NULL)
    {
        return NULL;
    }
    memset(item.param, 0, sizeof(bWifiServerInfo_t));
    item.release                            = bFree;
    ((bWifiServerInfo_t *)item.param)->port = port;
    memcpy(((bWifiServerInfo_t *)item.param)->ip, ip,
           sizeof(((bWifiServerInfo_t *)item.param)->ip));
    if (0 > bQueuePush(&bModWifiQueue, &item))
    {
        bFree(item.param);
        _bWifiDeleteNetCtx(plist);
        return NULL;
    }
    return plist;
}

int bWifiConnectClose(bWifiConnHandle_t conn)
{
    if (conn == NULL)
    {
        return -1;
    }
    bWifiQItem_t       item;
    bWifiNetCtxList_t *pconn = (bWifiNetCtxList_t *)conn;
    bWifiDevice_t     *pdev  = pconn->pdev;

    item.handle = pdev;
    item.param  = bMalloc(sizeof(bWifiServerInfo_t));
    if (item.param == NULL)
    {
        return -1;
    }
    memset(item.param, 0, sizeof(bWifiServerInfo_t));
    item.release = bFree;
    item.type    = WIFI_CONNECT_CLOSE;

    ((bWifiServerInfo_t *)item.param)->port = pconn->pctx->port;
    memcpy(((bWifiServerInfo_t *)item.param)->ip, pconn->pctx->ip,
           sizeof(((bWifiServerInfo_t *)item.param)->ip));
    if (0 > bQueuePush(&bModWifiQueue, &item))
    {
        bFree(item.param);
        return -1;
    }
    _bWifiDeleteNetCtx(conn);
    return 0;
}

int bWifiConnSend(bWifiConnHandle_t conn, uint8_t *pbuf, uint16_t len)
{
    bWifiQItem_t       item;
    bWifiNetCtxList_t *pconn  = (bWifiNetCtxList_t *)conn;
    int                retval = -1;
    if (conn == NULL || pbuf == NULL)
    {
        return -1;
    }

    item.type    = WIFI_CONNECT_SEND;
    item.handle  = pconn->pdev;
    item.param   = pconn->pctx;
    item.release = NULL;

    retval = bFIFO_Write(&pconn->pctx->sfifo, pbuf, len);
    if (retval > 0)
    {
        bQueuePush(&bModWifiQueue, &item);
    }
    return retval;
}

int bWifiConnRead(bWifiConnHandle_t conn, uint8_t *pbuf, uint16_t len)
{
    bWifiNetCtxList_t *pconn = (bWifiNetCtxList_t *)conn;
    if (conn == NULL || pbuf == NULL)
    {
        return -1;
    }
    return bFIFO_Read(&pconn->pctx->rfifo, pbuf, len);
}

int bWifiMqttConnect(bWifiHandle_t handle, bWifiMqtt_t *mqtt)
{
    if (handle == NULL || mqtt == NULL)
    {
        return -1;
    }
    bWifiQItem_t   item;
    bWifiDevice_t *pdev = (bWifiDevice_t *)handle;
    item.type           = WIFI_CONNECT_MQTT;
    item.handle         = handle;
    item.param          = bMalloc(sizeof(bWifiMqtt_t));
    if (item.param == NULL)
    {
        return -1;
    }
    memset(item.param, 0, sizeof(bWifiMqtt_t));
    memcpy(item.param, mqtt, sizeof(bWifiMqtt_t));
    item.release = bFree;
    if (0 > bQueuePush(&bModWifiQueue, &item))
    {
        bFree(item.param);
        return -1;
    }
    return 0;
}

int bWifiMqttSub(bWifiHandle_t handle, const char *topic, uint8_t qos)
{
    if (handle == NULL || topic == NULL || qos > 2)
    {
        return -1;
    }
    uint8_t       *pch = NULL;
    bWifiQItem_t   item;
    bWifiDevice_t *pdev = (bWifiDevice_t *)handle;
    item.type           = WIFI_CONNECT_MQTT_SUB;
    item.handle         = handle;
    item.param          = bMalloc(strlen(topic) + 1 + 1);
    if (item.param == NULL)
    {
        return -1;
    }
    memset(item.param, 0, strlen(topic) + 1 + 1);
    pch    = item.param;
    pch[0] = qos;
    memcpy(pch + 1, topic, strlen(topic));
    item.release = bFree;
    if (0 > bQueuePush(&bModWifiQueue, &item))
    {
        bFree(item.param);
        return -1;
    }
    return 0;
}

int bWifiMqttPub(bWifiHandle_t handle, const char *topic, uint8_t qos, uint8_t *pbuf, uint16_t len)
{
    if (handle == NULL || topic == NULL || qos > 2 || pbuf == NULL || len == 0)
    {
        return -1;
    }
    uint8_t       *pch = NULL;
    bWifiQItem_t   item;
    bWifiDevice_t *pdev = (bWifiDevice_t *)handle;
    item.type           = WIFI_CONNECT_MQTT_PUB;
    item.handle         = handle;
    item.param          = bMalloc(strlen(topic) + 1 + 1 + sizeof(len) + len);
    if (item.param == NULL)
    {
        return -1;
    }
    memset(item.param, 0, (strlen(topic) + 1 + 1 + sizeof(len) + len));
    pch    = item.param;
    pch[0] = qos;
    memcpy(pch + 1, topic, strlen(topic));
    memcpy(pch + 1 + strlen(topic) + 1, &len, sizeof(len));
    memcpy(pch + 1 + strlen(topic) + 1 + sizeof(len), pbuf, len);
    item.release = bFree;
    if (0 > bQueuePush(&bModWifiQueue, &item))
    {
        bFree(item.param);
        return -1;
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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
