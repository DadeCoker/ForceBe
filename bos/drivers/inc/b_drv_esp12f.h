/**
 *!
 * \file        b_drv_esp12f.h
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
#ifndef __B_DRV_ESP12F_H__
#define __B_DRV_ESP12F_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "drivers/inc/b_driver.h"
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
 * \defgroup ESP12F_Exported_TypesDefinitions
 * \{
 */
//<HALIF 1 UART
typedef bHalUartNumber_t bESP12F_HalIf_t;

typedef struct
{
    const uint8_t *plist;
    uint8_t        index;
    uint8_t        list_len;
    uint8_t        at_id;
    uint8_t        cmd;
} bEsp12fOptInfo_t;

#define ESP12F_REC_BUF_LEN (1536)
typedef struct
{
    bUitlUartInstance_t uart;
    uint8_t             rec_buf[ESP12F_REC_BUF_LEN];
    bEsp12fOptInfo_t    opt;
    void               *opt_param;
    bWiFiData_t         wifi_rec_data;
    bMempList_t         tcp_data_list;
    struct
    {
        uint8_t mux_enable;
        uint8_t conn_id; /* data */
    } cfg;
    uint8_t lock;
    uint8_t result;
} bEsp12fPrivate_t;

/**
 * \}
 */

/**
 * \defgroup ESP12F_Exported_Definitions
 * \{
 */
#define ESP12F_CMD_RESULT_NULL (0)
#define ESP12F_CMD_RESULT_OK (1)
#define ESP12F_CMD_RESULT_ERR (2)
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

#ifdef __cplusplus
}
#endif

#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
