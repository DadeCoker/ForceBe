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

#include "../../algorithm/inc/algo_crc.h"
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

/**
 * IAP状态，IAP介绍文档中有状态的切换路径
 */
#define B_IAP_STA_NULL (0)
#define B_IAP_STA_START (1)
#define B_IAP_STA_READY (2)
#define B_IAP_STA_FINISHED (3)
#define IS_IAP_STA(s)                                                                   \
    (((s) == B_IAP_STA_NULL) || ((s) == B_IAP_STA_START) || ((s) == B_IAP_STA_READY) || \
     ((s) == B_IAP_STA_FINISHED))

#define B_IAP_FILENAME_LEN (64)    // 固件名的长度限制
#define B_IAP_FAIL_COUNT (3)       // 固件失败的次数限制
#define B_IAP_BACKUP_EN (0x55)     // 备份固件功能启用的标志
#define B_IAP_BACKUP_VALID (0xAA)  // 存在有效备份固件的标志

/**
 * \}
 */

/**
 * \defgroup IAP_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
    uint8_t *pbuf;
    uint32_t len;
    void (*release)(void *);  // 如果pbuf不需要释放，则赋值NULL
} bIapFwData_t;

#pragma pack(1)

typedef struct
{
    char     name[B_IAP_FILENAME_LEN];  // 固件名，限制在64个字符
    uint32_t version;                   // 固件版本号
    uint32_t len;                       // 固件长度
    uint32_t crc;                       // 固件数据的校验值
    uint32_t crc_type;                  // CRC类型，参考algorithm/inc/algo_crc.h
} bIapFwInfo_t;

typedef struct
{
    uint8_t  flag;    // 备份标志，0xAA表示存在有效备份
    uint32_t fcrc;    // 备份区固件的crc32校验值
    uint32_t second;  // 运行多少秒后进行备份
} bIapBackupInof_t;

typedef struct
{
    int              stat;
    int              fail_count;
    uint32_t         cache_dev;
    uint32_t         backup_dev;
    bIapFwInfo_t     info;
    bIapBackupInof_t backup;
    uint32_t         percentage;
    uint32_t         fcrc;       //crc
} bIapFlag_t;

#pragma pack()

typedef void (*pJumpFunc_t)(void);

typedef enum
{
    B_IAP_EVENT_START,  // \ref bIapFwInfo_t
    B_IAP_EVENT_DATA    // \ref bIapFwData_t
} bIapEvent_t;

/**
 * \}
 */

/**
 * \defgroup IAP_Exported_Functions
 * \{
 */
/**
 * 跳转是弱函数，用户可自己实现
 */
void bIapJump2Boot(void);
void bIapJump2App(void);

/**
 * \brief 初始化函数
 * \param cache_dev_no   缓存固件的设备号，若不需要缓存则传入0
 * \param backup_dev_no  备份固件的设备号，若不需要备份固件则传入0
 * \param backup_time_s  运行多少s后备份固件，若不需要备份固件则忽略
 */
int bIapInit(uint32_t cache_dev_no, uint32_t backup_dev_no, uint32_t backup_time_s);

/**
 * \brief 事件处理函数
 * \param event \ref bIapEvent_t
 * \param arg
 */
int bIapEventHandler(bIapEvent_t event, void *arg);

/**
 * 查询当前IAP的状态
 * 应用程序，查询到是B_IAP_STA_READY状态，则跳转至启动程序
 */
uint8_t bIapGetStatus(void);

/**
 * 查询备份固件是否有效
 */
uint8_t bIapBackupIsValid(void);

/**
 * 查询升级进度
 */
uint8_t bIapPercentage(void);

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
