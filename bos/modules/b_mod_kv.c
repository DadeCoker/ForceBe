/**
 *!
 * \file        b_mod_kv.c
 * \version     v0.0.3
 * \date        2022/11/13
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO KV SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */

/*Includes ----------------------------------------------*/
#include "modules/inc/b_mod_kv.h"
#if (defined(_KV_ENABLE) && (_KV_ENABLE == 1))
#include <string.h>

#include "algorithm/inc/algorithm.h"
#include "core/inc/b_core.h"
#include "drivers/inc/b_driver.h"
#include "utils/inc/b_util_log.h"

/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup KV
 * \{
 */

/**
 * \defgroup KV_Private_TypesDefinitions
 * \{
 */
typedef enum
{
    KV_SECTOR_STA_WRITABLE,
    KV_SECTOR_STA_FULL,
    KV_SECTOR_STA_NUM
} bKVSectorStat_t;

typedef struct
{
    uint32_t flag;
    uint32_t state[KV_SECTOR_STA_NUM];
} bKVSectorHead_t;

typedef struct
{
    uint32_t state;
    uint8_t  md5[8];
    uint32_t crc;
    uint32_t len;
    uint8_t  data[1];
} bKVDataHead_t;

/**
 * \}
 */

/**
 * \defgroup KV_Private_Defines
 * \{
 */
#define B_KV_FLAG (0x625F6B76)

#define B_KV_STA_WRITABLE (0x12345678)
#define B_KV_STA_FULL (0x12345679)
#define B_KV_STA_MESSY_FULL (0x12345670)
#define B_KV_STA_JOINT (0x12345671)
#define B_KV_STA_IS_FULL(s) \
    (((s) == B_KV_STA_FULL) || ((s) == B_KV_STA_MESSY_FULL) || ((s) == B_KV_STA_JOINT))

#define B_KV_MODIFIED_FLAG (0xAA)
#define B_KV_DELETED_FLAG (0x55)

/**
 * \}
 */

/**
 * \defgroup KV_Private_Macros
 * \{
 */

#define B_KV_IS_MODIFIED(s) ((((s)&0xff000000) >> 24) == B_KV_MODIFIED_FLAG)
#define B_KV_IS_DELETED(s) ((((s)&0xff000000) >> 24) == B_KV_DELETED_FLAG)
#define B_KV_NEW_ADDRESS(s) ((s)&0x00ffffff)

#define B_KV_MODIFIED(addr) ((B_KV_MODIFIED_FLAG << 24) | (addr))
#define B_KV_DELETED (B_KV_MODIFIED_FLAG << 24)

#define B_KV_KEY_LENGTH(len) ((uint8_t)(((len) >> 24) & 0xff))
#define B_KV_VALUE_LENGTH(len) ((len)&0x00ffffff)

/**
 * \}
 */

/**
 * \defgroup KV_Private_Variables
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup KV_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup KV_Private_Functions
 * \{
 */

/**
 * \}
 */

/**
 * \addtogroup KV_Exported_Functions
 * \{
 */
int bKVInit(bKVInstance_t *pinstance)
{
    int             fd     = -1;
    int             retval = -1, i = 0, windex = -1;
    uint32_t        erase_size   = 0;
    uint32_t        sector_count = 0;
    uint8_t         erase_flag   = 0;
    uint32_t        empty_count  = 0;
    bFlashErase_t   param;
    bKVSectorHead_t sector_head;

    if (pinstance == NULL)
    {
        return -1;
    }
    fd = bOpen(pinstance->dev, BCORE_FLAG_RW);
    if (fd < 0)
    {
        return -2;
    }
    retval = bCtl(fd, bCMD_GET_SECTOR_SIZE, &erase_size);
    if (retval == 0)
    {
        pinstance->erase_size = erase_size;
    }
    else
    {
        pinstance->erase_size = 0;
    }

    if (pinstance->erase_size > 0)
    {
        sector_count = pinstance->total_size / pinstance->erase_size;
    }
    else
    {
        pinstance->erase_size = pinstance->total_size / 2;
        sector_count          = 2;
    }

    for (i = 0; i < sector_count; i++)
    {
        bLseek(fd, pinstance->address + i * pinstance->erase_size);
        bRead(fd, (uint8_t *)&sector_head, sizeof(sector_head));
        if (sector_head.flag != B_KV_FLAG)
        {
            param.addr = pinstance->address + i * pinstance->erase_size;
            param.num  = 1;
            bCtl(fd, bCMD_ERASE_SECTOR, &param);
            sector_head.flag = B_KV_FLAG;
            bLseek(fd, pinstance->address + i * pinstance->erase_size);
            bWrite(fd, &sector_head.flag, sizeof(sector_head.flag));
            empty_count += 1;
            continue;
        }

        if (B_KV_STA_IS_FULL(sector_head.state[KV_SECTOR_STA_FULL]))
        {
            continue;
        }

        if (sector_head.state[KV_SECTOR_STA_WRITABLE] == B_KV_STA_WRITABLE)
        {
            windex = i;
            continue;
        }
        empty_count += 1;
    }
    if (windex >= 0)
    {
        pinstance->write_index = windex;
        return 0;
    }
    if (empty_count == 1)
    {
        ;  // 增加整理区块
    }
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
