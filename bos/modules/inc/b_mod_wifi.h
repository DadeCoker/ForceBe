/**
 *!
 * \file        b_mod_wifi.h
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
#ifndef __B_MOD_WIFI_H__
#define __B_MOD_WIFI_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

#include "b_config.h"

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
 * \defgroup WIFI_Exported_TypesDefinitions
 * \{
 */
typedef void *bWifiHandle_t;
typedef void *bWifiConnHandle_t;

typedef struct
{
    char    ssid[64];
    char    passwd[64];
    uint8_t encryption;
} bWifiApInfo_t;

typedef struct
{
    char     broker[64];
    uint16_t port;
    char     device_id[64];
    char     user[64];
    char     passwd[64];
} bWifiMqtt_t;

typedef struct
{
    char     topic[64];
    uint8_t *pbuf;
    uint16_t len;
    void (*release)(void *);
} bWifiMqttData_t;

typedef enum
{
    WIFI_EVT_STA_MODE,
    WIFI_EVT_STA_ERR,
    WIFI_EVT_AP_MODE,
    WIFI_EVT_AP_ERR,
    WIFI_EVT_AP_STA_MODE,
    WIFI_EVT_AP_STA_ERR,
    WIFI_EVT_JOINAP_OK,
    WIFI_EVT_JOINAP_ERR,
    WIFI_EVT_PING_OK,
    WIFI_EVT_PING_ERR,
    WIFI_EVT_SETUP_TCPSERVER_OK,
    WIFI_EVT_SETUP_TCPSERVER_ERR,
    WIFI_EVT_DATA_READY,
    WIFI_EVT_MQTT_CONN_OK,
    WIFI_EVT_MQTT_CONN_ERR,
    WIFI_EVT_MQTT_SUB_OK,
    WIFI_EVT_MQTT_SUB_ERR,
    WIFI_EVT_MQTT_PUB_OK,
    WIFI_EVT_MQTT_PUB_ERR,
    WIFI_EVT_MQTT_DATA,
} bWifiEvent_t;

typedef void (*bWifiEventHandle_t)(bWifiHandle_t handle, bWifiEvent_t evt, void *arg);
/**
 * \}
 */

/**
 * \defgroup WIFI_Exported_Defines
 * \{
 */
#define WIFI_KEY_OPEN (0)
#define WIFI_KEY_WPA_PSK (1)
#define WIFI_KEY_WPA2_PSK (2)
#define WIFI_KEY_WPA_WPA2_PSK (3)
#define IS_WIFI_KEY(k)                                                              \
    ((k == WIFI_KEY_OPEN) || (k == WIFI_KEY_WPA_PSK) || (k == WIFI_KEY_WPA2_PSK) || \
     (k == WIFI_KEY_WPA_WPA2_PSK))

#define WIFI_MODE_INVALID (0)
#define WIFI_MODE_STA (1)
#define WIFI_MODE_AP (2)
#define WIFI_MODE_AP_STA (3)
#define IS_WIFI_MODE(m) ((m == WIFI_MODE_STA) || (m == WIFI_MODE_AP) || (m == WIFI_MODE_AP_STA))

#define WIFI_SERVER_TYPE_TCP (0)
#define WIFI_SERVER_TYPE_UDP (1)
#define IS_WIFI_SERVER_TYPE(t) ((t == WIFI_SERVER_TYPE_TCP) || (t == WIFI_SERVER_TYPE_UDP))

#define WIFI_NET_CTX_NEW(name, dev_no) static bWifiNetCtx_t name
/**
 * \}
 */

/**
 * \defgroup WIFI_Exported_Functions
 * \{
 */
int bWifiInit(void);

bWifiHandle_t bWifiUp(uint32_t dev_no, bWifiEventHandle_t cb);
int           bWifiDown(bWifiHandle_t handle);
/**
 * \brief 设置WIFI的工作模式
 * \param handle \ref bWifiUp
 * \param mode WIFI_MODE_STA、WIFI_MODE_AP、WIFI_MODE_AP_STA
 * \param pinfo STA模式可以为空，其他模式需要设定AP信息
 */
int bWifiSetMode(bWifiHandle_t handle, uint8_t mode, const bWifiApInfo_t *pinfo);
int bWifiJoinAp(bWifiHandle_t handle, const bWifiApInfo_t *pinfo);
int bWifiPing(bWifiHandle_t handle);

bWifiConnHandle_t bWifiSetupServer(bWifiHandle_t handle, uint8_t type, uint16_t port);
bWifiConnHandle_t bWifiConnectServer(bWifiHandle_t handle, uint8_t type, const char *ip,
                                     uint16_t port);

int bWifiConnClose(bWifiConnHandle_t conn);
int bWifiConnSend(bWifiConnHandle_t conn, uint8_t *pbuf, uint16_t len);
int bWifiConnRead(bWifiConnHandle_t conn, uint8_t *pbuf, uint16_t len);

int bWifiMqttConnect(bWifiHandle_t handle, bWifiMqtt_t *mqtt);
int bWifiMqttSub(bWifiHandle_t handle, const char *topic, uint8_t qos);
int bWifiMqttPub(bWifiHandle_t handle, const char *topic, uint8_t qos, uint8_t *pbuf, uint16_t len);

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

#ifdef __cplusplus
}
#endif

#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
