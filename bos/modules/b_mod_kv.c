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
    uint32_t flag;
    uint32_t addr : 24;
    uint32_t state : 8;
    uint8_t  md5[8];
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
        head.state[sta] = B_KV_STA_MESSY;
    }
    bLseek(fd, pinstance->address + sector_index * pinstance->erase_size +
                   B_OFFSET_OF(bKVSectorHead_t, state) + sta * sizeof(uint32_t));
    bWrite(fd, (uint8_t *)&head.state[sta], sizeof(uint32_t));
    bClose(fd);
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
            copy_len = sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(head) + B_KV_JVALUE_RLEN(i, head);
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
    return 0;
}

static int _bKVSectorDefrag(bKVInstance_t *pinstance)
{
    int             i      = 0;
    int             mindex = -1, eindex = -1;
    int             retval = -1;
    bKVSectorHead_t sector_head;
    if (pinstance->erase_size == 0)
    {
        return 0;
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
            retval = 0;
        }
    }
    else if (pinstance->empty_count == 1)
    {
        if (mindex != -1 && eindex != -1)
        {
            if (0 == (retval = _bKVMoveData(pinstance, mindex, eindex)))
            {
                pinstance->write_index = eindex;
            }
        }
    }

    b_log("ret:%d windex:%d\r\n", retval, pinstance->write_index);

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
        if (strncmp(tmp, key + rlen,
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
    int             i       = 0;
    uint32_t        r_index = 0;
    uint32_t        f_range = 0;
    bKVSectorHead_t sector_head;
    bKVDataHead_t   data_head;
    union
    {
        uint8_t  md5[8];
        uint64_t key_md5;
    } key_md5_value;
    md5_hex_8(key, strlen(key), key_md5_value.md5);
    f_range = (pinstance->erase_size == 0) ? pinstance->total_size : pinstance->erase_size;
    for (i = 0; i < B_KV_SECTOR_NUM(pinstance); i++)
    {
        if (0 > _bKVReadSectorHead(pinstance, i, &sector_head))
        {
            return -1;
        }
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
            if (data_head.md5 != key_md5_value.key_md5 || data_head.state == B_KV_DELETED_FLAG)
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
            *paddr = pinstance->address + i * pinstance->erase_size + r_index;
            if (_bKVReconfirmKey(pinstance, i, key, *paddr + sizeof(bKVDataHead_t),
                                 data_head.key_len) == 0)
            {
                return 0;
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
            }
        }
    }
    return -1;
}

static int _bKVReadData(bKVInstance_t *pinstance, uint32_t addr, bKVDataHead_t head, uint8_t *pbuf)
{
    int      fd     = -1;
    int      rindex = 0, i = 0;
    uint32_t r_addr = 0;
    uint32_t r_len  = 0;
    uint32_t len    = 0;
    fd              = bOpen(pinstance->dev, BCORE_FLAG_R);
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
            bLseek(fd, addr + sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(head.key_len) +
                           i * sizeof(uint32_t));
            bRead(fd, &r_addr, sizeof(r_addr));
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
        bLseek(fd, addr + sizeof(bKVDataHead_t) + B_KV_KEY_RLEN(head.key_len));
        bRead(fd, pbuf, head.value_len);
    }
    bClose(fd);
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
    if (pinstance->write_index >= 0)
    {
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
    if (key == NULL || pbuf == NULL || len == 0)
    {
        return -1;
    }
    if (strlen(key) > 0xFF)
    {
        return -1;
    }
    retval = _bKVFindKey(pinstance, key, &address);
    if (retval < 0)
    {
        return retval;
    }
    _bKVReadDataHead(pinstance, address, &data_head);
    if (len < data_head.value_len)
    {
        return -1;
    }
    _bKVReadData(pinstance, address, data_head, pbuf);
    if (prlen != NULL)
    {
        *prlen = data_head.value_len;
    }
    if (head.crc != crc_calculate(ALGO_CRC32, pbuf, data_head.value_len))
    {
        return -2;
    }
    return 0;
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
