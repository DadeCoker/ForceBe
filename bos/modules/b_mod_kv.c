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

#define B_KV_SECTOR_NUM(i) (i->total_size / i->erase_size)

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
static int _bKVReadSectorHead(bKVInstance_t *pinstance, uint32_t sector_index,
                              bKVSectorHead_t *phead)
{
    int fd = -1;
    fd     = bOpen(pinstance->dev, BCORE_FLAG_R);
    if (fd < 0 || sector_index >= (B_KV_SECTOR_NUM(pinstance)))
    {
        return -1;
    }
    bLseek(fd, pinstance->address + sector_index * pinstance->erase_size);
    bRead(fd, (uint8_t *)phead, sizeof(bKVSectorHead_t));
    bClose(fd);
    return 0;
}

static int _bKVEraseSector(bKVInstance_t *pinstance, uint32_t sector_index)
{
    int           fd = -1;
    bFlashErase_t param;
    fd = bOpen(pinstance->dev, BCORE_FLAG_RW);
    if (fd < 0 || sector_index >= (B_KV_SECTOR_NUM(pinstance)))
    {
        return -1;
    }
    param.addr = pinstance->address + sector_index * pinstance->erase_size;
    param.num  = 1;
    bCtl(fd, bCMD_ERASE_SECTOR, &param);
    bClose(fd);
    return 0;
}

static int _bKVSetSectorFlag(bKVInstance_t *pinstance, uint32_t sector_index)
{
    int             fd = -1;
    bKVSectorHead_t head;
    fd = bOpen(pinstance->dev, BCORE_FLAG_RW);
    if (fd < 0 || sector_index >= (B_KV_SECTOR_NUM(pinstance)))
    {
        return -1;
    }
    head.flag = B_KV_FLAG;
    bLseek(fd, pinstance->address + sector_index * pinstance->erase_size);
    bWrite(fd, (uint8_t *)&head.flag, sizeof(head.flag));
    bClose(fd);
    return 0;
}

static int _bKVSetSectorState(bKVInstance_t *pinstance, uint32_t sector_index, bKVSectorStat_t sta,
                              uint8_t messy, uint8_t joint)
{
    int             fd = -1;
    bKVSectorHead_t head;
    fd = bOpen(pinstance->dev, BCORE_FLAG_RW);
    if (fd < 0 || sector_index >= (B_KV_SECTOR_NUM(pinstance)) || sta >= KV_SECTOR_STA_NUM)
    {
        return -1;
    }
    if (sta == KV_SECTOR_STA_WRITABLE)
    {
        head.state[sta] = B_KV_STA_WRITABLE;
    }
    else if (sta == KV_SECTOR_STA_FULL)
    {
        if (messy == 0 && joint == 0)
        {
            head.state[sta] = B_KV_STA_FULL;
        }
        else if (messy == 0 && joint != 0)
        {
            head.state[sta] = B_KV_STA_JOINT;
        }
        else
        {
            head.state[sta] = B_KV_STA_MESSY_FULL;
        }
    }
    bLseek(fd, pinstance->address + sector_index * pinstance->erase_size +
                   B_OFFSET_OF(bKVSectorHead_t, state) + sta * sizeof(uint32_t));
    bWrite(fd, (uint8_t *)&head.state[sta], sizeof(uint32_t));
    bClose(fd);
    return 0;
}

static int _bKVSectorDefrag()
{
}

/**
 * \}
 */

/**
 * \addtogroup KV_Exported_Functions
 * \{
 */
int bKVInit(bKVInstance_t *pinstance)
{
    int             retval = -1, i = 0;
    uint32_t        sector_count = 0;
    uint32_t        empty_count  = 0;
    bKVSectorHead_t sector_head;

    if (pinstance == NULL)
    {
        return -1;
    }
    if (pinstance->size == 0 || pinstance->size <= pinstance->erase_size)
    {
        return -1;
    }
    pinstance->write_index = -1;
    if (pinstance->erase_size == 0)
    {
        pinstance->erase_size = pinstance->total_size / 2
    }
    sector_count = pinstance->total_size / pinstance->erase_size;
    for (i = 0; i < sector_count; i++)
    {
        _bKVReadSectorHead(pinstance, i, &sector_head);
        if (sector_head.flag != B_KV_FLAG)
        {
            _bKVEraseSector(pinstance, i);
            _bKVSetSectorFlag(pinstance, i);
            empty_count += 1;
            continue;
        }
        if (B_KV_STA_IS_FULL(sector_head.state[KV_SECTOR_STA_FULL]))
        {
            continue;
        }
        if (sector_head.state[KV_SECTOR_STA_WRITABLE] == B_KV_STA_WRITABLE)
        {
            pinstance->write_index = i;
            continue;
        }
        empty_count += 1;
    }

    if (pinstance->write_index >= 0)
    {
        return 0;
    }

    if (empty_count <= 1)
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
