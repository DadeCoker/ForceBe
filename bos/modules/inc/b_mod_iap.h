/**
 *!
 * \file        b_mod_iap.h
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
#ifndef __B_MOD_IAP_H__
#define __B_MOD_IAP_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

#include "b_config.h"

#if _IAP_ENABLE
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup IAP
 * \{
 */

/**
 * \defgroup IAP_Exported_Defines
 * \{
 */
#define B_IAP_STA_NULL (0)
#define B_IAP_STA_START (1)
#define B_IAP_STA_READY (2)
#define IS_IAP_STA(s) \
    (((s) == B_IAP_STA_NULL) || ((s) == B_IAP_STA_START) || ((s) == B_IAP_STA_READY))

#define B_IAP_FILENAME_MAXLEN (64)
#define B_IAP_APP_FAIL_MAXCOUNT (3)

#define B_BOOT_JUMP2APP (0)
#define B_BOOT_WAIT_FW (1)

#define B_UPDATE_FW_ERR (0)
#define B_UPDATE_FW_OK (1)

/**
 * \}
 */

/**
 * \defgroup IAP_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
    int  stat;
    int  app_invalid;
    int  app_fail_count;
    char file_name[B_IAP_FILENAME_MAXLEN + (4 - (B_IAP_FILENAME_MAXLEN % 4))];
} bIapFlag_t;

typedef struct
{
    uint8_t  dev_no;   //暂存新固件的设备号，不需要暂存可以忽略
    uint32_t len;      //固件长度
    uint32_t c_crc32;  //固件数据CRC32校验值
} bIapFwInfo_t;

typedef void (*pJumpFunc_t)(void);

/**
 * \}
 */

/**
 * \defgroup IAP_Exported_Functions
 * \{
 */

void bIapJump2Boot(void);
void bIapJump2App(void);

int bIapInit(uint8_t dev_no);
int bIapStart(const char *pfname);
int bIapAppCheckFlag(void);
int bIapBootCheckFlag(void);
int bIapUpdateFwResult(int result);
int bIapSetFwInfo(bIapFwInfo_t *pinfo);
int bIapUpdateFwData(uint32_t index, uint8_t *pbuf, uint32_t len);
int bIapVerifyFwData(void);
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

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
