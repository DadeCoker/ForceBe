/**
 *!
 * \file        b_driver_cmd.h
 * \version     v0.0.1
 * \date        2019/06/05
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2019 Bean
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
#ifndef __B_DRIVER_CMD_H__
#define __B_DRIVER_CMD_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

///////////////////////////////////////////////////////////
// Flash Command & Data Structure
///////////////////////////////////////////////////////////
#define bCMD_ERASE_SECTOR 0      // bFlashErase_t
#define bCMD_GET_SECTOR_SIZE 1   // uint32_t
#define bCMD_GET_SECTOR_COUNT 2  // uint32_t

typedef struct
{
    uint32_t addr;
    uint32_t num;
} bFlashErase_t;

///////////////////////////////////////////////////////////
// GSensor Command & Data Structure
///////////////////////////////////////////////////////////
#define bCMD_CFG_ODR 0        // b_drv_xxx.h odr struct
#define bCMD_CFG_FS 1         // b_drv_xxx.h fs struct
#define bCMD_CFG_FIFO 2       // bGSensorFifo_t
#define bCMD_CFG_POWERDOWN 3  // none

typedef struct
{
    int16_t x_mg;
    int16_t y_mg;
    int16_t z_mg;
} bGsensor3Axis_t;

typedef struct
{
    uint8_t fifo_en;
    uint8_t fifo_mode;
    uint8_t fifo_length;
} bGSensorFifo_t;

///////////////////////////////////////////////////////////
// LCD  Command & Data Structure
///////////////////////////////////////////////////////////
#define bCMD_FILL_RECT 0  // bLcdRectInfo_t

typedef struct
{
    uint16_t x1;
    uint16_t y1;
    uint16_t x2;
    uint16_t y2;
    uint16_t color;
} bLcdRectInfo_t;

typedef struct
{
    uint16_t color;
} bLcdWrite_t;

typedef struct
{
    uint16_t reg;
    uint16_t dat;
} bLcdRWAddress_t;

///////////////////////////////////////////////////////////
// Touch Command & Data Structure
///////////////////////////////////////////////////////////
typedef struct
{
    uint16_t x_ad;
    uint16_t y_ad;
} bTouchAdVal_t;

///////////////////////////////////////////////////////////
// Temperature Command & Data Structure
///////////////////////////////////////////////////////////
typedef struct
{
    int16_t tempx100;
} bTempVal_t;

///////////////////////////////////////////////////////////
// Sensor Command & Data Structure
///////////////////////////////////////////////////////////
#define bCMD_SENSOR_START 0

///////////////////////////////////////////////////////////
// Wifi Module Command & Data Structure
///////////////////////////////////////////////////////////
#define bCMD_WIFI_MODE_STA 0           // none
#define bCMD_WIFI_MODE_AP 1            // bApInfo_t
#define bCMD_WIFI_MODE_STA_AP 2        // bApInfo_t
#define bCMD_WIFI_JOIN_AP 3            // bApInfo_t
#define bCMD_WIFI_MQTT_CONN 4          // bMqttConnInfo_t
#define bCMD_WIFI_MQTT_SUB 5           // bMqttTopic_t
#define bCMD_WIFI_MQTT_PUB 6           // bMqttData_t
#define bCMD_WIFI_LOCAL_TCP_SERVER 7   // bTcpUdpInfo_t
#define bCMD_WIFI_LOCAL_UDP_SERVER 8   // bTcpUdpInfo_t
#define bCMD_WIFI_REMOT_TCP_SERVER 9   // bTcpUdpInfo_t
#define bCMD_WIFI_REMOT_UDP_SERVER 10  // bTcpUdpInfo_t
#define bCMD_WIFI_PING 11              // default www.baidu.com
#define bCMD_WIFI_GET_CONN_STATUS 12   // bWfifiConnStat_t
#define bCMD_WIFI_REG_CALLBACK 13      // bWifiModuleCallback_t
#define bCMD_WIFI_TCPUDP_CLOSE 14      // bTcpUdpInfo_t
#define bCMD_WIFI_TCPUDP_SEND 15       // bTcpUdpData_t

typedef enum
{
    WIFI_DRV_EVT_CMD_OK,     // arg 为对应Wifi Module Command
    WIFI_DRV_EVT_CMD_ERR,    // arg 为对应Wifi Module Command
    WIFI_DRV_EVT_DATA,       // 提示收到数据 bTcpUdpData_t
    WIFI_DRV_EVT_MQTT_DATA,  // 提示收到数据 bMqttData_t
} bWifiModuleEvent_t;

typedef struct
{
    void (*cb)(bWifiModuleEvent_t event, void *arg, void (*release)(void *), void *user_data);
    void *user_data;
} bWifiModuleCallback_t;

typedef struct
{
    char    ssid[64];
    char    passwd[64];
    uint8_t encryption;
    // 0(open) 1(WPA_PSK) 2(WPA2_PSK) 3(WPA_WPA2_PSK)
} bApInfo_t;

typedef struct
{
    char     ip[64];
    uint16_t port;
} bTcpUdpInfo_t;

typedef struct
{
    bTcpUdpInfo_t conn;
    uint8_t      *pbuf;
    uint16_t      len;
    void (*release)(void *);
} bTcpUdpData_t;

typedef struct
{
    char     broker[64];
    uint16_t port;
    char     device_id[64];
    char     user[64];
    char     passwd[64];
} bMqttConnInfo_t;

typedef struct
{
    char    topic[64];
    uint8_t qos;
} bMqttTopic_t;

typedef struct
{
    bMqttTopic_t topic;
    uint8_t     *pbuf;
    uint16_t     len;
    void (*release)(void *);
} bMqttData_t;
///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
