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
#include "utils/inc/b_util_memp.h"
#include "utils/inc/b_util_queue.h"

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
    uint8_t type;
    void   *param;
    void (*release)(void *);
} bWifiQItem_t;

typedef struct
{
    uint32_t           dev;
    int                fd;
    bWifiEventHandle_t cb;
} bWifiDevice_t;
/**
 * \}
 */

/**
 * \defgroup WIFI_Private_Defines
 * \{
 */

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

static void _bWifiCallback_t(uint8_t cmd, void *arg, void (*release)(void *), void *user_data)
{
    if (release && arg)
    {
        release(arg);
    }
}

/**
 * \}
 */

/**
 * \addtogroup WIFI_Exported_Functions
 * \{
 */
bWifiHandle_t bWifiUp(uint32_t dev_no, bWifiEventHandle_t cb)
{
    bWifiDevice_t  *pdev = (bWifiDevice_t *)bMalloc(sizeof(bWifiDevice_t));
    bWifiCallback_t drv_cb;
    if (pdev == NULL)
    {
        return NULL;
    }
    pdev->fd = bOpen(dev_no, BCORE_FLAG_WR);
    if (pdev->fd <= 0)
    {
        bFree(pdev);
        return NULL;
    }
    pdev->cb         = cb;
    drv_cb.cb        = _bWifiCallback_t;
    drv_cb.user_data = pdev;
    bCtl(pdev->fd, bCMD_WIFI_REG_CALLBACK, &drv_cb);
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
    bFree(pdev);
    return 0;
}

int bWifiSetStaMode(bWifiHandle_t handle)
{
    bWifiDevice_t *pdev = (bWifiDevice_t *)handle;
    if (handle == NULL)
    {
        return -1;
    }
}

int bWifiSetApMode(bWifiHandle_t handle, bApInfo_t *pinfo);
int bWifiSetApStaMode(bWifiHandle_t handle, bApInfo_t *pinfo);

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
