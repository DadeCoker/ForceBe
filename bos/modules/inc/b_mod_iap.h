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

#define B_IAP_FILENAME_LEN (64)  // 固件名的长度限制
#define B_IAP_FAIL_COUNT (3)     // 固件失败的次数限制

/**
 * \}
 */

/**
 * \defgroup IAP_Exported_TypesDefinitions
 * \{
 */
#pragma pack(1)

typedef struct
{
    uint8_t  dev_no;                    //暂存新固件的设备号，不需要暂存可以忽略
    char     name[B_IAP_FILENAME_LEN];  //固件名，限制在64个字符
    uint32_t len;                       //固件长度
    uint32_t c_crc32;                   //固件数据CRC32校验值
} bIapFwInfo_t;

typedef struct
{
    int          stat;
    int          fail_count;
    bIapFwInfo_t info;
    uint32_t     fcrc;
} bIapFlag_t;

#pragma pack()

typedef void (*pJumpFunc_t)(void);

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
 * boot和app都先调用bIapInit
 * 紧接着，按照不同的代码，调用bIapXXXCheckFlag()
 * XXX: Boot or App
 * 主要用于判断，进入启动程序和进入应用程序时，当前状态是否合法
 */
/**
 * \param dev_no：固件暂存区的设备号
 *        注：暂存于内部FLASH 或 没有暂存区，dev_no = 0
 */
int bIapInit(uint8_t dev_no);
/**
 * \return int 0：没有升级流程  1：升级流程正常运行中  -1：升级流程异常
 */
int bIapAppCheckFlag(void);
int bIapBootCheckFlag(void);

/**
 * 应用程序调用，表示升级流程开始。传入新固件的信息。
 */
int bIapStart(bIapFwInfo_t *pinfo);

/**
 * \brief 传入新固件的数据用于写入存储区域
 * \param index 新固件数据的索引，即相对文件起始的偏移
 * \return int 0：正常存储  -1：存储异常   -2：校验失败，重新接收
 */
int bIapUpdateFwData(uint32_t index, uint8_t *pbuf, uint32_t len);

/**
 * 查询当前IAP的状态
 * 应用程序，查询到是B_IAP_STA_READY状态，则跳转至启动程序
 * 启动程序，查询到是B_IAP_STA_NULL或者B_IAP_STA_FINISHED状态，则跳转至应用程序
 */
uint8_t bIapGetStatus(void);
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
