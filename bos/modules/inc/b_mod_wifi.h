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

typedef struct
{
    uint32_t dev;
} bWifiNetCtx_t;

typedef struct
{
    char    ssid[64];
    char    passwd[64];
    uint8_t encryption;
} bApInfo_t;

typedef enum
{
    WIFI_EVT_STA_MODE,
    WIFI_EVT_STA_ERR,
    WIFI_EVT_AP_MODE,
    WIFI_EVT_AP_ERR,
    WIFI_EVT_AP_STA_MODE,
    WIFI_EVT_AP_STA_ERR,
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

#define WIFI_NET_CTX_NEW(name, dev_no) static bWifiNetCtx_t name
/**
 * \}
 */

/**
 * \defgroup WIFI_Exported_Functions
 * \{
 */

bWifiHandle_t bWifiUp(uint32_t dev_no, bWifiEventHandle_t cb);
int           bWifiDown(bWifiHandle_t handle);

int bWifiSetStaMode(bWifiHandle_t handle);
int bWifiSetApMode(bWifiHandle_t handle, bApInfo_t *pinfo);
int bWifiSetApStaMode(bWifiHandle_t handle, bApInfo_t *pinfo);

int bWifiSetupTcpServer(bWifiHandle_t handle, bWifiNetCtx_t *pctx, uint16_t port);

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
