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

typedef enum
{
    KV_DATA_STA_MODIFY,
    KV_DATA_STA_DELETE,
    KV_DATA_STA_NUM
} bKVDataStat_t;

typedef struct
{
    uint32_t flag;
    uint32_t state[KV_SECTOR_STA_NUM];
} bKVSectorHead_t;

typedef struct
{
    uint32_t flag;
    uint32_t addr : 24;
    uint32_t state : 8;
    union
    {
        uint8_t  md5[8];
        uint64_t md5_value;
    } key_md5;
    uint32_t crc;
    uint32_t value_len : 23;
    uint32_t joint_f : 1;
    uint32_t key_len : 8;
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
#define B_KV_STA_MESSY (0x12345670)
#define B_KV_STA_JOINT (0x12345671)

#define B_KV_MODIFIED_FLAG (0xAA)
#define B_KV_DELETED_FLAG (0x55)

#define B_KV_INVALID_ADDR (0xFFFFFF)
/**
 * \}
 */

/**
 * \defgroup KV_Private_Macros
 * \{
 */
#define B_KV_STA_IS_FULL(s) (((s) == B_KV_STA_FULL) || ((s) == B_KV_STA_JOINT))
#define B_KV_SECTOR_DATA_SIZE(i)                                      \
    ((i->erase_size == 0) ? (i->total_size - sizeof(bKVSectorHead_t)) \
                          : (i->erase_size - sizeof(bKVSectorHead_t)))
#define B_KV_SECTOR_NUM(i) ((i->erase_size == 0) ? 1 : (i->total_size / i->erase_size))
#define B_KV_KEY_RLEN(head) B_SIZE_ALIGNMENT(head.key_len, 4)
#define B_KV_VALUE_RLEN(head) B_SIZE_ALIGNMENT(head.value_len, 4)
#define B_KV_JVALUE_SECOTR_NUM(i, head) \
    ((head.value_len + B_KV_SECTOR_DATA_SIZE(i) - 1) / B_KV_SECTOR_DATA_SIZE(i))
#define B_KV_JVALUE_RLEN(i, head) \
    B_SIZE_ALIGNMENT(B_KV_JVALUE_SECOTR_NUM(i, head) * sizeof(uint32_t), 4)
#define B_KV_ADDR_IS_VALID(i, addr) ((addr) >= i->address && (addr) < (i->address + i->total_size))
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

    if (sector_index >= (B_KV_SECTOR_NUM(pinstance)))
    {
        return -1;
    }
    fd = bOpen(pinstance->dev, BCORE_FLAG_R);
    if (fd < 0)
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
    if (sector_index >= (B_KV_SECTOR_NUM(pinstance)))
    {
        return -1;
    }
    fd = bOpen(pinstance->dev, BCORE_FLAG_RW);
    if (fd < 0)
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

    if (sector_index >= (B_KV_SECTOR_NUM(pinstance)))
    {
        return -1;
    }

    fd = bOpen(pinstance->dev, BCORE_FLAG_RW);
    if (fd < 0)
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
    if (sector_index >= (B_KV_SECTOR_NUM(pinstance)) || sta >= KV_SECTOR_STA_NUM)
    {
        return -1;
    }
    fd = bOpen(pinstance->dev, BCORE_FLAG_RW);
    if (fd < 0)
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
        head.state[sta] = B_KV_STA_MESSY;
    }
    bLseek(fd, pinstance->address + sector_index * pinstance->erase_size + sizeof(uint32_t) +
                   sta * sizeof(uint32_t));
    bWrite(fd, (uint8_t *)&head.state[sta], sizeof(uint32_t));
    bClose(fd);
    return 0;
}

static int _bKVSetDataState(bKVInstance_t *pinstance, uint32_t addr, bKVDataStat_t sta,
                            uint32_t param)
{
    uint32_t wdata = 0;
    int      fd = -1, retval = -1;
    addr = addr + sizeof(uint32_t);
    if (sta == KV_DATA_STA_MODIFY)
    {
        wdata = B_KV_MODIFIED_FLAG;
        wdata = (wdata << 24) | (param & 0xffffff);
    }
    else if (sta == KV_DATA_STA_DELETE)
    {
        wdata = B_KV_DELETED_FLAG;
        wdata <<= 24;
    }
    b_log("wdata:0x%x addr:0x%x\r\n", wdata, addr);
    fd = bOpen(pinstance->dev, BCORE_FLAG_RW);
    if (fd < 0)
    {
        b_log("open fail %d \r\n", fd);
        return -1;
    }
    bLseek(fd, addr);
    retval = bWrite(fd, (uint8_t *)&wdata, sizeof(uint32_t));
    bClose(fd);
    b_log("write retval %d \r\n", retval);
    if (retval < 0)
    {
        return -1;
    }
    return 0;
}

static void _bKVFindMessyEmptySector(bKVInstance_t *pinstance, int *messy, int *empty, int sindex)
{
    int             i = 0;
    bKVSectorHead_t sector_head;
    *messy = -1;
    *empty = -1;

    if (sindex < 0 || sindex >= B_KV_SECTOR_NUM(pinstance))
    {
        sindex = 0;
    }

    for (i = 0; i < B_KV_SECTOR_NUM(pinstance); i++)
    {
        _bKVReadSectorHead(pinstance, (i + sindex + 1) % B_KV_SECTOR_NUM(pinstance), &sector_head);
        if (sector_head.state[KV_SECTOR_STA_MESSY] == B_KV_STA_MESSY &&
            sector_head.state[KV_SECTOR_STA_FULL] == B_KV_STA_FULL)
        {
            if (*messy == -1)
            {
                *messy = (i + sindex + 1) % B_KV_SECTOR_NUM(pinstance);
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
            *empty = (i + sindex + 1) % B_KV_SECTOR_NUM(pinstance);
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
    bRead(fd, (uint8_t *)phead, sizeof(bKVDataHead_t));
    bClose(fd);
    return 0;
}

static int _bKVCopyData(bKVInstance_t *pinstance, int src, int des, uint32_t len)
{
    int      fd = -1;
    uint8_t  tmp[32];
    uint32_t rlen = 0;

    fd = bOpen(pinstance->dev, BCORE_FLAG_RW);
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
    int           retval   = 0;
    uint32_t      copy_len = 0;
    bKVDataHead_t head;
    uint32_t r_addr = pinstance->address + src * pinstance->erase_size + sizeof(bKVSectorHead_t);
    uint32_t w_addr = pinstance->address + des * pinstance->erase_size + sizeof(bKVSectorHead_t);
    retval          = _bKVSetSectorState(pinstance, des, KV_SECTOR_STA_WRITABLE, 0);
    if (retval < 0)
    {
        return -1;
    }
    while ((pinstance->address + (src + 1) * pinstance->erase_size - sizeof(bKVDataHead_t)) >
           r_addr)
    {
        _bKVReadDataHead(pinstance, r_addr, &head);
        if (head.flag != B_KV_FLAG)
        {
            break;
        }
        if (head.joint_f)
        {
            copy_len =
                sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(head) + B_KV_JVALUE_RLEN(pinstance, head);
        }
        else
        {
            copy_len = sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(head) + B_KV_VALUE_RLEN(head);
        }
        if (head.state != B_KV_MODIFIED_FLAG && head.state != B_KV_DELETED_FLAG)
        {
            retval = _bKVCopyData(pinstance, r_addr, w_addr, copy_len);
            if (retval >= 0)
            {
                w_addr += retval;
            }
        }
        r_addr += copy_len;
    }
    _bKVEraseSector(pinstance, src);
    _bKVSetSectorFlag(pinstance, src);
    return (w_addr - (pinstance->address + des * pinstance->erase_size));
}

static int _bKVSectorDefragNoErase(bKVInstance_t *pinstance)
{
    int           retval = -1;
    bKVDataHead_t head;
    uint32_t      r_addr = pinstance->address + sizeof(bKVSectorHead_t);
    uint32_t      w_addr = pinstance->address + sizeof(bKVSectorHead_t);
    if (pinstance->write_index < 0 && pinstance->empty_count > 0)
    {
        pinstance->write_index = 0;
        pinstance->empty_count = 0;
        retval = _bKVSetSectorState(pinstance, pinstance->write_index, KV_SECTOR_STA_WRITABLE, 0);
        pinstance->write_offset = sizeof(bKVSectorHead_t);
        return retval;
    }
    while (r_addr < (pinstance->address + pinstance->total_size))
    {
        retval = _bKVReadDataHead(pinstance, r_addr, &head);
        if (retval < 0)
        {
            return retval;
        }
        if (head.flag != B_KV_FLAG)
        {
            break;
        }
        if (head.state != B_KV_MODIFIED_FLAG && head.state != B_KV_DELETED_FLAG)
        {
            if (r_addr != w_addr)
            {
                _bKVCopyData(pinstance, r_addr, w_addr,
                             sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(head) + B_KV_VALUE_RLEN(head));
                _bKVSetDataState(pinstance, r_addr, KV_DATA_STA_MODIFY, w_addr);
            }
            w_addr += sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(head) + B_KV_VALUE_RLEN(head);
        }
        r_addr += sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(head) + B_KV_VALUE_RLEN(head);
    }
    pinstance->write_offset = w_addr - pinstance->address;
    return 0;
}

static int _bKVSectorDefrag(bKVInstance_t *pinstance)
{
    int mindex = -1, eindex = -1;
    int retval = -1;
    if (pinstance->erase_size == 0)
    {
        return _bKVSectorDefragNoErase(pinstance);
    }
    if (pinstance->write_index != -1)
    {
        _bKVSetSectorState(pinstance, pinstance->write_index, KV_SECTOR_STA_FULL, 0);
    }
    _bKVFindMessyEmptySector(pinstance, &mindex, &eindex, pinstance->write_index);
    pinstance->write_index = -1;  // 置write_index无效，准备寻找新的区块

    b_log("find: e %d m %d\r\n", eindex, mindex);

    if (pinstance->empty_count > 1)
    {
        if (eindex != -1)
        {
            pinstance->write_index = eindex;
            pinstance->empty_count -= 1;
            pinstance->write_offset = sizeof(bKVSectorHead_t);
            retval = _bKVSetSectorState(pinstance, eindex, KV_SECTOR_STA_WRITABLE, 0);
        }
    }
    else if (pinstance->empty_count == 1)
    {
        if (mindex != -1 && eindex != -1)
        {
            if (0 < (retval = _bKVMoveData(pinstance, mindex, eindex)))
            {
                pinstance->write_index  = eindex;
                pinstance->write_offset = retval;
                retval                  = 0;
            }
        }
    }

    b_log("ret:%d windex:%d off:%d\r\n", retval, pinstance->write_index, pinstance->write_offset);

    return retval;
}

static int _bKVClearAll(bKVInstance_t *pinstance)
{
    int i      = 0;
    int retval = 0;

    pinstance->empty_count = 0;
    pinstance->write_index = 0;
    for (i = 0; i < B_KV_SECTOR_NUM(pinstance); i++)
    {
        retval = _bKVEraseSector(pinstance, i);
        if (retval < 0)
        {
            break;
        }
        retval = _bKVSetSectorFlag(pinstance, i);
        if (retval < 0)
        {
            break;
        }
        pinstance->empty_count += 1;
    }
    return retval;
}

static int _bKVGetJointSectorAddr(bKVInstance_t *pinstance, uint32_t addr, uint32_t index)
{
    int           fd = -1, retval = -1;
    bKVDataHead_t head;
    retval = _bKVReadDataHead(pinstance, addr, &head);
    if (retval < 0)
    {
        return retval;
    }
    if (index >= B_KV_JVALUE_SECOTR_NUM(pinstance, head))
    {
        return -1;
    }
    fd = bOpen(pinstance->dev, BCORE_FLAG_RW);
    if (fd < 0)
    {
        return -1;
    }
    bLseek(fd, addr + sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(head) + index * sizeof(uint32_t));
    bRead(fd, (uint8_t *)&retval, sizeof(uint32_t));
    bClose(fd);
    return retval;
}

static int _bKVReconfirmKey(bKVInstance_t *pinstance, uint32_t sector, const char *key,
                            uint32_t key_addr, uint8_t len)
{
    uint8_t key_len = strlen(key);
    uint8_t tmp[32];
    int     fd = -1, rlen = 0;
    fd = bOpen(pinstance->dev, BCORE_FLAG_R);
    if (fd < 0)
    {
        return -1;
    }
    bLseek(fd, key_addr);
    while (rlen < key_len)
    {
        bRead(fd, tmp, sizeof(tmp));
        if (strncmp((const char *)tmp, key + rlen,
                    ((key_len - rlen) > sizeof(tmp)) ? sizeof(tmp) : (key_len - rlen)) == 0)
        {
            rlen += sizeof(tmp);
        }
        else
        {
            bClose(fd);
            return -1;
        }
    }
    bClose(fd);
    return 0;
}

static int _bKVFindKey(bKVInstance_t *pinstance, const char *key, uint32_t *paddr)
{
    int             i = 0, retval = -1;
    uint32_t        r_index = 0;
    uint32_t        f_range = 0;
    bKVSectorHead_t sector_head;
    bKVDataHead_t   data_head;
    union
    {
        uint8_t  md5[8];
        uint64_t key_md5;
    } key_md5_value;
    md5_hex_8((uint8_t *)key, strlen(key), key_md5_value.md5);
    f_range = (pinstance->erase_size == 0) ? pinstance->total_size : pinstance->erase_size;
    b_log("frange:%d md5:0x%x\r\n", f_range, key_md5_value.key_md5);
    for (i = 0; i < B_KV_SECTOR_NUM(pinstance); i++)
    {
        if (0 > _bKVReadSectorHead(pinstance, i, &sector_head))
        {
            return -1;
        }
        b_log("%d state:0x%x 0x%x 0x%x\r\n", i, sector_head.state[0], sector_head.state[1],
              sector_head.state[2]);
        if (sector_head.state[KV_SECTOR_STA_FULL] == B_KV_STA_JOINT)
        {
            continue;
        }
        if (sector_head.state[KV_SECTOR_STA_FULL] != B_KV_STA_FULL &&
            sector_head.state[KV_SECTOR_STA_WRITABLE] != B_KV_STA_WRITABLE)
        {
            continue;
        }
        r_index = sizeof(bKVSectorHead_t);
        while (r_index < f_range)
        {
            retval = _bKVReadDataHead(
                pinstance, pinstance->address + i * pinstance->erase_size + r_index, &data_head);
            if (retval < 0)
            {
                return -1;
            }
            if (data_head.flag != B_KV_FLAG)
            {
                break;
            }
            if (data_head.key_md5.md5_value != key_md5_value.key_md5 ||
                data_head.state == B_KV_DELETED_FLAG)
            {
                if (data_head.joint_f)
                {
                    r_index += sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(data_head) +
                               B_KV_JVALUE_RLEN(pinstance, data_head);
                }
                else
                {
                    r_index += sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(data_head) +
                               B_KV_VALUE_RLEN(data_head);
                }
                continue;
            }

            *paddr = pinstance->address + i * pinstance->erase_size + r_index;
            if (_bKVReconfirmKey(pinstance, i, key, *paddr + sizeof(bKVDataHead_t),
                                 data_head.key_len) == 0)
            {
                ;
            }
            else
            {
                if (data_head.joint_f)
                {
                    r_index += sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(data_head) +
                               B_KV_JVALUE_RLEN(pinstance, data_head);
                }
                else
                {
                    r_index += sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(data_head) +
                               B_KV_VALUE_RLEN(data_head);
                }
                continue;
            }
            if (data_head.state == B_KV_MODIFIED_FLAG)
            {
                if (data_head.addr == B_KV_INVALID_ADDR)
                {
                    break;
                }
                else
                {
                    r_index = data_head.addr;
                    continue;
                }
            }
            return 0;
        }
    }
    return -1;
}

static int _bKVReadData(bKVInstance_t *pinstance, uint32_t addr, bKVDataHead_t head, uint8_t *pbuf)
{
    int      fd     = -1;
    int      i      = 0;
    uint32_t r_addr = 0;
    uint32_t r_len  = 0;
    uint32_t len    = 0;

    fd = bOpen(pinstance->dev, BCORE_FLAG_R);
    if (fd < 0)
    {
        return -1;
    }
    if (head.joint_f)
    {
        for (i = 0; i < B_KV_JVALUE_SECOTR_NUM(pinstance, head); i++)
        {
            if (r_len > head.value_len)
            {
                bClose(fd);
                return -1;
            }
            bLseek(fd, addr + sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(head) + i * sizeof(uint32_t));
            bRead(fd, (uint8_t *)&r_addr, sizeof(r_addr));
            len = ((head.value_len - r_len) > B_KV_SECTOR_DATA_SIZE(pinstance))
                      ? B_KV_SECTOR_DATA_SIZE(pinstance)
                      : (head.value_len - r_len);
            if (B_KV_ADDR_IS_VALID(pinstance, r_addr))
            {
                bLseek(fd, r_addr + sizeof(bKVSectorHead_t));
                bRead(fd, pbuf + r_len, len);
                r_len += len;
            }
            else
            {
                bClose(fd);
                return -1;
            }
        }
    }
    else
    {
        bLseek(fd, addr + sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(head));
        bRead(fd, pbuf, head.value_len);
    }
    bClose(fd);
    return 0;
}

static int _bKVFindWriteOffset(bKVInstance_t *pinstance)
{
    bKVDataHead_t head;
    uint32_t      raddr = 0, roffset = 0;
    uint32_t      range = 0;
    roffset             = sizeof(bKVSectorHead_t);
    raddr = pinstance->address + pinstance->write_index * pinstance->erase_size + roffset;
    range = (pinstance->erase_size > 0) ? pinstance->erase_size : pinstance->total_size;
    while (roffset < range)
    {
        if (0 == _bKVReadDataHead(pinstance, raddr, &head))
        {
            if (head.flag != B_KV_FLAG)
            {
                pinstance->write_offset = roffset;
                return 0;
            }
            else
            {
                if (head.joint_f)
                {
                    roffset += sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(head) +
                               B_KV_JVALUE_RLEN(pinstance, head);
                }
                else
                {
                    roffset += sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(head) + B_KV_VALUE_RLEN(head);
                }
            }
        }
        else
        {
            return -1;
        }
    }
    pinstance->write_offset = range;
    return 0;
}

static int _bKVCompValue(bKVInstance_t *pinstance, uint32_t addr, uint8_t *pbuf, uint32_t len)
{
    uint8_t       tmp[32], rtmp_len = 0;
    int           i = 0, j = 0, k = 0, fd = -1;
    int           retval  = -1;
    uint32_t      r_index = 0;
    uint32_t      r_addr = 0, cmp_len = 0;
    bKVDataHead_t head;
    retval = _bKVReadDataHead(pinstance, addr, &head);
    if (retval < 0)
    {
        return retval;
    }
    if (head.value_len != len)
    {
        return -1;
    }
    fd = bOpen(pinstance->dev, BCORE_FLAG_R);
    if (fd < 0)
    {
        return -1;
    }
    if (head.joint_f)
    {
        for (i = 0; i < B_KV_JVALUE_SECOTR_NUM(pinstance, head); i++)
        {
            bLseek(fd, addr + sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(head) + i * sizeof(uint32_t));
            bRead(fd, (uint8_t *)&r_addr, sizeof(uint32_t));
            cmp_len = ((len - r_index) > B_KV_SECTOR_DATA_SIZE(pinstance))
                          ? (B_KV_SECTOR_DATA_SIZE(pinstance))
                          : (len - r_index);
            j       = 0;
            bLseek(fd, r_addr + sizeof(bKVSectorHead_t));
            while (j < cmp_len)
            {
                rtmp_len = ((cmp_len - j) > sizeof(tmp)) ? sizeof(tmp) : (cmp_len - j);
                bRead(fd, tmp, rtmp_len);
                for (k = 0; k < rtmp_len; k++)
                {
                    if (tmp[k] != pbuf[r_index + k])
                    {
                        bClose(fd);
                        return -1;
                    }
                }
                j += rtmp_len;
                r_index += rtmp_len;
            }
        }
    }
    else
    {
        bLseek(fd, addr + sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(head));
        j = 0;
        while (j < len)
        {
            rtmp_len = ((len - j) > sizeof(tmp)) ? sizeof(tmp) : (len - j);
            bRead(fd, tmp, rtmp_len);
            for (k = 0; k < rtmp_len; k++)
            {
                if (tmp[k] != pbuf[j + k])
                {
                    bClose(fd);
                    return -1;
                }
            }
            j += rtmp_len;
        }
    }
    bClose(fd);
    return 0;
}

static int _bKVWriteJointData(bKVInstance_t *pinstance, uint32_t addr, const char *key,
                              uint8_t *pbuf, uint32_t len)
{
    int           fd          = -1;
    int           messy_index = -1, empty_index = -1, empty_addr = 0;
    uint32_t      w_index = 0;
    bKVDataHead_t head;
    head.flag    = B_KV_FLAG;
    head.joint_f = 1;
    md5_hex_8((uint8_t *)key, strlen(key), head.key_md5.md5);
    head.crc       = crc_calculate(ALGO_CRC32, pbuf, len);
    head.key_len   = strlen(key);
    head.value_len = len;

    fd = bOpen(pinstance->dev, BCORE_FLAG_RW);
    if (fd < 0)
    {
        return -1;
    }
    bLseek(fd, addr);
    bWrite(fd, (uint8_t *)&head, sizeof(head.flag));
    bLseek(fd, addr + 8);
    bWrite(fd, ((uint8_t *)&head) + 8, sizeof(bKVDataHead_t) - 8);
    bWrite(fd, (uint8_t *)key, strlen(key));
    bClose(fd);
    while (w_index < len)
    {
        _bKVFindMessyEmptySector(pinstance, &messy_index, &empty_index, 0);
        _bKVSetSectorState(pinstance, empty_index, KV_SECTOR_STA_FULL, 1);
        empty_addr = pinstance->address + empty_index * pinstance->erase_size;
        pinstance->empty_count -= 1;
        fd = bOpen(pinstance->dev, BCORE_FLAG_RW);
        if (fd < 0)
        {
            return -1;
        }
        bLseek(fd, addr + sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(head) +
                       (w_index / B_KV_SECTOR_DATA_SIZE(pinstance)) * sizeof(uint32_t));
        bWrite(fd, (uint8_t *)&empty_addr, sizeof(uint32_t));
        bLseek(fd, empty_addr + sizeof(bKVSectorHead_t));
        bWrite(fd, pbuf + w_index,
               ((len - w_index) > B_KV_SECTOR_DATA_SIZE(pinstance))
                   ? B_KV_SECTOR_DATA_SIZE(pinstance)
                   : (len - w_index));
        bClose(fd);
        w_index += B_KV_SECTOR_DATA_SIZE(pinstance);
    }
    return 0;
}

static int _bKVClearJointData(bKVInstance_t *pinstance, uint32_t addr)
{
    uint32_t      sector_num = 0;
    int           i = 0, retval = -1;
    bKVDataHead_t head;
    _bKVReadDataHead(pinstance, addr, &head);
    if (head.joint_f == 0)
    {
        return -1;
    }
    sector_num = B_KV_JVALUE_SECOTR_NUM(pinstance, head);
    for (i = 0; i < sector_num; i++)
    {
        retval = _bKVGetJointSectorAddr(pinstance, addr, i);
        if (retval < 0)
        {
            return -1;
        }
        _bKVEraseSector(pinstance, (retval - pinstance->address) / pinstance->erase_size);
        _bKVSetSectorFlag(pinstance, (retval - pinstance->address) / pinstance->erase_size);
        pinstance->empty_count += 1;
    }
    return 0;
}

static int _bKVAddValue(bKVInstance_t *pinstance, const char *key, uint8_t *pbuf, uint32_t len)
{
    int retval = -1, fd = -1;
    int valid = (pinstance->erase_size == 0) ? (pinstance->total_size - pinstance->write_offset)
                                             : (pinstance->erase_size - pinstance->write_offset);
    uint32_t kv_len =
        sizeof(bKVDataHead_t) + B_SIZE_ALIGNMENT(strlen(key), 4) + B_SIZE_ALIGNMENT(len, 4);
    uint32_t      w_addr    = 0;
    int32_t       old_addr  = -1;
    uint32_t      empty_num = 0;
    bKVDataHead_t head;

    // 寻找是否已存在，存在并且值相同，则直接返回
    retval = _bKVFindKey(pinstance, key, (uint32_t *)&old_addr);
    b_log("find key %s retval:%d\r\n", key, retval);
    if (retval == 0)
    {
        if ((old_addr > pinstance->address &&
             old_addr < (pinstance->address + pinstance->total_size)))
        {
            if (0 == _bKVCompValue(pinstance, (uint32_t)old_addr, pbuf, len))
            {
                b_log("%s same as the old value \r\n", key);
                return 0;
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        old_addr = -1;
    }
    // 已存在的值没有相同，或者没有已存在
    // 新增一项
    // 判断是否需要联合区块存储
    if (pinstance->erase_size > 0 && len > (B_KV_SECTOR_DATA_SIZE(pinstance)))
    {
        empty_num =
            ((len + B_KV_SECTOR_DATA_SIZE(pinstance) - 1) / B_KV_SECTOR_DATA_SIZE(pinstance));
        kv_len = sizeof(bKVDataHead_t) + B_SIZE_ALIGNMENT(strlen(key), 4) + empty_num * 4;
    }
    b_log("kv_len:%d valid:%d empty_num:%d old_addr:0x%x\r\n", kv_len, valid, empty_num, old_addr);
    // 判断可用区块是否满足
    if (kv_len > valid)
    {
        retval = _bKVSectorDefrag(pinstance);
        if (retval < 0)
        {
            return retval;
        }
        valid = (pinstance->erase_size == 0) ? (pinstance->total_size - pinstance->write_offset)
                                             : (pinstance->erase_size - pinstance->write_offset);
        if (kv_len > valid)
        {
            return -2;
        }
    }
    // 如果用到联合区块，判断可用空块是否足够
    if (empty_num > 0)
    {
        b_log("joint data\r\n");
        if (old_addr != -1)
        {
            _bKVReadDataHead(pinstance, old_addr, &head);
            if ((B_KV_JVALUE_SECOTR_NUM(pinstance, head) + pinstance->empty_count - 1) < empty_num)
            {
                return -2;
            }
        }
    }
    // 如果已存在，则将之前的数据设置为已修改
    if (old_addr != -1)
    {
        if (pinstance->erase_size == 0)
        {
            retval =
                _bKVSetDataState(pinstance, old_addr, KV_DATA_STA_MODIFY, pinstance->write_offset);
        }
        else
        {
            if (((old_addr - pinstance->address) / pinstance->erase_size) != pinstance->write_index)
            {
                retval = _bKVSetDataState(pinstance, old_addr, KV_DATA_STA_MODIFY, 0xffffff);
            }
            else
            {
                retval = _bKVSetDataState(pinstance, old_addr, KV_DATA_STA_MODIFY,
                                          pinstance->write_offset);
                _bKVSetSectorState(pinstance, pinstance->write_index, KV_SECTOR_STA_MESSY, 0);
            }
        }
        b_log("retval:%d\r\n", retval);
        if (retval < 0)
        {
            return retval;
        }
    }
    b_log("write index:%d offset:%d\r\n", pinstance->write_index, pinstance->write_offset);
    // 写入数据
    w_addr = pinstance->address + pinstance->write_index * pinstance->erase_size +
             pinstance->write_offset;
    b_log("write addr:0x%x\r\n", w_addr);
    // 联合区块写入
    if (empty_num > 0)
    {
        if (old_addr != -1)
        {
            _bKVClearJointData(pinstance, old_addr);
        }
        _bKVWriteJointData(pinstance, w_addr, key, pbuf, len);
    }
    else
    {
        head.flag    = B_KV_FLAG;
        head.joint_f = 0;
        md5_hex_8((uint8_t *)key, strlen(key), head.key_md5.md5);
        head.crc       = crc_calculate(ALGO_CRC32, pbuf, len);
        head.key_len   = strlen(key);
        head.value_len = len;

        fd = bOpen(pinstance->dev, BCORE_FLAG_RW);
        if (fd < 0)
        {
            return -1;
        }
        bLseek(fd, w_addr);
        bWrite(fd, (uint8_t *)&head, sizeof(head.flag));
        bLseek(fd, w_addr + 8);
        bWrite(fd, ((uint8_t *)&head) + 8, sizeof(bKVDataHead_t) - 8);
        bWrite(fd, (uint8_t *)key, strlen(key));
        bLseek(fd, w_addr + sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(head));
        bWrite(fd, pbuf, len);
        bClose(fd);
    }
    pinstance->write_offset += kv_len;
    return 0;
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

    b_log("=====>BabyOS KV V0.3==========\r\n");

    if (pinstance == NULL)
    {
        return -1;
    }
    if (pinstance->total_size == 0 || pinstance->total_size <= pinstance->erase_size)
    {
        return -1;
    }

    pinstance->write_index  = -1;
    pinstance->write_offset = sizeof(bKVSectorHead_t);
    pinstance->empty_count  = 0;

    b_log("b_kv:0x%x %dB e_size %dB sector %d\r\n", pinstance->address, pinstance->total_size,
          pinstance->erase_size, B_KV_SECTOR_NUM(pinstance));
    /**
     * 遍历区块，确定可写入的区块以及空区块的个数
     * 弱区块未初始化，则进行擦除并写入区块头部信息
     */
    for (i = 0; i < B_KV_SECTOR_NUM(pinstance); i++)
    {
        if (0 > _bKVReadSectorHead(pinstance, i, &sector_head))
        {
            return -1;
        }
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

    // 最小擦除单元大于0字节时，必定会保留1个空区块用于数据整理
    // 如果空区块数量为0，则进行异常处理

    if (pinstance->erase_size > 0)
    {
        if (pinstance->empty_count == 0 && pinstance->write_index != -1)
        {
            _bKVEraseSector(pinstance, pinstance->write_index);
            _bKVSetSectorFlag(pinstance, pinstance->write_index);
            pinstance->empty_count = 1;
            pinstance->write_index = -1;
        }
        else if (pinstance->empty_count == 0 && pinstance->write_index == -1)
        {
            if (0 > _bKVClearAll(pinstance))
            {
                return -1;
            }
        }
    }
    pinstance->init_f = 1;
    b_log("b_kv:%d %d\r\n", pinstance->write_index, pinstance->empty_count);

    // 确定当前可写入区块，空闲区域偏移量
    if (pinstance->write_index >= 0)
    {
        _bKVFindWriteOffset(pinstance);
        b_log(":::%d %d\r\n", pinstance->write_index, pinstance->write_offset);
        return 0;
    }
    return _bKVSectorDefrag(pinstance);
}

int bKVGetValue(bKVInstance_t *pinstance, const char *key, uint8_t *pbuf, uint32_t len,
                uint32_t *prlen)
{
    int           retval  = -1;
    uint32_t      address = 0;
    bKVDataHead_t data_head;
    if (key == NULL || pbuf == NULL || len == 0 || pinstance == NULL)
    {
        b_log("param error \r\n");
        return -1;
    }
    if (pinstance->init_f != 1)
    {
        b_log("no init... \r\n");
        return -2;
    }

    if (strlen(key) > 0xFF)
    {
        return -3;
    }
    retval = _bKVFindKey(pinstance, key, &address);
    b_log("find %s retval %d addr:0x%x\r\n", key, retval, address);
    if (retval < 0)
    {
        return retval;
    }
    _bKVReadDataHead(pinstance, address, &data_head);
    b_log("len:%d value_len:%d\r\n", len, data_head.value_len);
    if (len < data_head.value_len)
    {
        return -4;
    }
    _bKVReadData(pinstance, address, data_head, pbuf);
    if (prlen != NULL)
    {
        *prlen = data_head.value_len;
    }
    if (data_head.crc != crc_calculate(ALGO_CRC32, pbuf, data_head.value_len))
    {
        b_log("check crc error \r\n");
        return -5;
    }
    return 0;
}

int bKVSetValue(bKVInstance_t *pinstance, const char *key, uint8_t *pbuf, uint32_t len)
{
    if (pinstance == NULL || key == NULL || pbuf == NULL || len == 0)
    {
        return -1;
    }
    if (pinstance->init_f != 1)
    {
        return -2;
    }
    return _bKVAddValue(pinstance, key, pbuf, len);
}

int bKVDeleteValue(bKVInstance_t *pinstance, const char *key)
{
    int      retval  = -1;
    uint32_t address = 0;
    if (pinstance == NULL || key == NULL)
    {
        return -1;
    }
    retval = _bKVFindKey(pinstance, key, &address);
    if (retval < 0)
    {
        return retval;
    }
    _bKVSetDataState(pinstance, address, KV_DATA_STA_DELETE, 0);
    if (pinstance->erase_size > 0)
    {
        _bKVSetSectorState(pinstance, (address - pinstance->address) / pinstance->erase_size,
                           KV_SECTOR_STA_MESSY, 0);
    }
    return 0;
}

int bKVGetValueLength(bKVInstance_t *pinstance, const char *key)
{
    int           retval  = -1;
    uint32_t      address = 0;
    bKVDataHead_t head;
    if (pinstance == NULL || key == NULL)
    {
        return -1;
    }
    retval = _bKVFindKey(pinstance, key, &address);
    if (retval < 0)
    {
        return retval;
    }
    _bKVReadDataHead(pinstance, address, &head);
    return head.value_len;
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
