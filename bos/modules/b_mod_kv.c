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
    KV_SECTOR_STA_MESSY,
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
#define B_KV_DATA_LENGTH(len) \
    (B_SIZE_ALIGNMENT(B_KV_KEY_LENGTH(len), 4) + B_SIZE_ALIGNMENT(B_KV_VALUE_LENGTH(len), 4))

#define B_KV_SECTOR_NUM(i) ((i->erase_size == 0) ? 1 : (i->total_size / i->erase_size))
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
                              uint8_t joint)
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
        if (joint == 0)
        {
            head.state[sta] = B_KV_STA_FULL;
        }
        else
        {
            head.state[sta] = B_KV_STA_JOINT;
        }
    }
    else if (sta == KV_SECTOR_STA_MESSY)
    {
        head.state[sta] = B_KV_STA_MESSY_FULL;
    }
    bLseek(fd, pinstance->address + sector_index * pinstance->erase_size +
                   B_OFFSET_OF(bKVSectorHead_t, state) + sta * sizeof(uint32_t));
    bWrite(fd, (uint8_t *)&head.state[sta], sizeof(uint32_t));
    bClose(fd);
    return 0;
}

static void _bKVFindMessyEmptySector(bKVInstance_t *pinstance, int *messy, int *empty)
{
    int             i = 0;
    bKVSectorHead_t sector_head;
    *messy = -1;
    *empty = -1;
    for (i = 0; i < B_KV_SECTOR_NUM(pinstance); i--)
    {
        _bKVReadSectorHead(pinstance, i, &sector_head);
        if (sector_head.state[KV_SECTOR_STA_MESSY] == B_KV_STA_MESSY_FULL)
        {
            if (*messy == -1)
            {
                *messy = i;
            }
            continue;
        }
        if (B_KV_STA_IS_FULL(sector_head.state[KV_SECTOR_STA_FULL]))
        {
            continue;
        }
        if (sector_head.state[KV_SECTOR_STA_WRITABLE] == B_KV_STA_WRITABLE)
        {
            continue;
        }
        if (*empty == -1)
        {
            *empty = i;
        }
        if (*empty != -1 && *messy != -1)
        {
            break;
        }
    }
}

static int _bKVReadDataHead(bKVInstance_t *pinstance, uint32_t addr, bKVDataHead_t *phead)
{
    int fd = -1;
    fd     = bOpen(pinstance->dev, BCORE_FLAG_RW);
    if (fd < 0)
    {
        return -1;
    }
    bLseek(fd, addr);
    bRead(fd, phead, sizeof(bKVDataHead_t));
    bClose(fd);
    return 0;
}

static int _bKVCheckValue(bKVInstance_t *pinstance, uint32_t addr, bKVDataHead_t head)
{
    int      fd = -1;
    uint8_t  tmp[32];
    uint8_t  joint_f = 0;
    uint32_t crc     = 0;
    uint32_t rlen    = 0;
    CRC_REG_SBS_HANDLE(kv_crc, ALGO_CRC32);
    addr = addr + sizeof(bKVDataHead_t) + B_SIZE_ALIGNMENT(B_KV_KEY_LENGTH(head.len), 4);
    if (B_SIZE_ALIGNMENT(B_KV_KEY_LENGTH(head.len) > pinstance->erase_size - sizeof(bKVSectorHead_t))
    {
        joint_f = 1;
    }
    fd = bOpen(pinstance->dev, BCORE_FLAG_R);
    if (fd < 0)
    {
        return -1;
    }
    bLseek(fd, addr);
    if (joint_f == 1)
    {
        bRead(fd, (uint8_t *)&crc, sizeof(crc));
        while (rlen < ())  // 补充对联合体，所以信息的读取和CRC32校验
    }
    else
    {
        crc = head.crc;
        while (rlen < B_KV_VALUE_LENGTH(head.len))
        {
            bRead(fd, tmp, sizeof(tmp));
            crc_calculate_sbs(&kv_crc, tmp,
                              ((B_KV_VALUE_LENGTH(head.len) - rlen) > sizeof(tmp))
                                  ? sizeof(tmp)
                                  : (B_KV_VALUE_LENGTH(head.len) - rlen));
            rlen += sizeof(tmp);
        }
    }
    bClose(fd);
    if (kv_crc.crc != crc)
    {
        return -1;
    }
    return 0;
}

static int _bKVCopyData(bKVInstance_t *pinstance, int src, int des, uint32_t len)
{
    int      fd = -1;
    uint8_t  tmp[32];
    uint32_t rlen = 0;
    fd            = bOpen(pinstance->dev, BCORE_FLAG_RW);
    if (fd < 0)
    {
        return -1;
    }
    while (rlen < len)
    {
        bLseek(fd, src + rlen);
        bRead(fd, tmp, sizeof(tmp));
        bLseek(fd, des + rlen);
        bWrite(fd, tmp, ((len - rlen) > sizeof(tmp)) ? sizeof(tmp) : (len - rlen));
        rlen += sizeof(tmp);
    }
    bClose(fd);
    return len;
}

static int _bKVMoveData(bKVInstance_t *pinstance, int src, int des)
{
    int           retval = 0;
    bKVDataHead_t head;
    uint32_t r_addr = pinstance->address + src * pinstance->erase_size + sizeof(bKVSectorHead_t);
    uint32_t w_addr = pinstance->address + des * pinstance->erase_size + sizeof(bKVSectorHead_t);
    _bKVSetSectorState(pinstance, des, KV_SECTOR_STA_WRITABLE, 0);
    while ((pinstance->address + (src + 1) * pinstance->erase_size - sizeof(bKVDataHead_t)) >
           r_addr)
    {
        _bKVReadDataHead(pinstance, r_addr, &head);
        if (B_KV_IS_MODIFIED(head.state) || B_KV_IS_DELETED(head.state))
        {
            r_addr += B_KV_DATA_LENGTH(head.len) + sizeof(bKVDataHead_t);
        }
        else if (head.state == 0xFFFFFFFF)
        {
            // 校验正确才数据正确数据，才能拷贝，否则继续寻找有效数据
            if (0 == _bKVCheckValue(pinstance, r_addr, head))
            {
                retval = _bKVCopyData(pinstance, r_addr, w_addr,
                                      B_KV_DATA_LENGTH(head.len) + sizeof(bKVDataHead_t));
                if (retval >= 0)
                {
                    w_addr += retval;
                }
                r_addr += B_KV_DATA_LENGTH(head.len) + sizeof(bKVDataHead_t);
            }
        }
        else
        {
            ;
        }
    }
}

static int _bKVSectorDefrag(bKVInstance_t *pinstance)
{
    int             i      = 0;
    int             mindex = -1, eindex = -1;
    bKVSectorHead_t sector_head;
    pinstance->write_index = -1;
    _bKVFindMessyEmptySector(pinstance, &mindex, &eindex);
    if (pinstance->empty_count > 1)
    {
        if (eindex != -1)
        {
            pinstance->write_index = eindex;
            pinstance->empty_count -= 1;
        }
    }
    else if (pinstance->empty_count == 1)
    {
        if (mindex != -1 && eindex != -1)
        {
            _bKVMoveData(pinstance, mindex, eindex);
            pinstance->write_index = eindex;
        }
    }
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
    int             i = 0;
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
    pinstance->empty_count = 0;

    b_log("b_kv:0x%x %dB e_size %dB sector %d\r\n", pinstance->address, pinstance->total_size,
          pinstance->erase_size, B_KV_SECTOR_NUM(pinstance));
    for (i = 0; i < B_KV_SECTOR_NUM(pinstance); i++)
    {
        _bKVReadSectorHead(pinstance, i, &sector_head);
        if (sector_head.flag != B_KV_FLAG)
        {
            _bKVEraseSector(pinstance, i);
            _bKVSetSectorFlag(pinstance, i);
            pinstance->empty_count += 1;
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
        pinstance->empty_count += 1;
    }
    b_log("b_kv:%d %d\r\n", pinstance->write_index, pinstance->empty_count);
    if (pinstance->write_index >= 0)
    {
        return 0;
    }
    return _bKVSectorDefrag(pinstance);
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
