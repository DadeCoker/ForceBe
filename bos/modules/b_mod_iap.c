/**
 *!
 * \file        b_mod_iap.c
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

/*Includes ----------------------------------------------*/
#include "modules/inc/b_mod_iap.h"
#if _IAP_ENABLE
#include <string.h>

#include "core/inc/b_core.h"
#include "drivers/inc/b_driver.h"
#include "hal/inc/b_hal.h"

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
 * \defgroup IAP_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup IAP_Private_Defines
 * \{
 */
#if ((RECEIVE_FIRMWARE_MODE == 1) && (IAP_FILE_CACHE == 0))
#error IAP configuration is invalid
#endif
/**
 * \}
 */

/**
 * \defgroup IAP_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup IAP_Private_Variables
 * \{
 */
static bIapFlag_t bIapFlag = {
    .stat = B_IAP_STA_NULL,
};
/**
 * \}
 */

/**
 * \defgroup IAP_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup IAP_Private_Functions
 * \{
 */
/**
 * \brief 计算CRC32
 * \param init_crc 第一次计算，初始CRC传入0
 */
static uint32_t _bIapCalCRC32(uint32_t init_crc, uint8_t *pbuf, uint32_t len)
{
    int      i, j;
    uint32_t crc, mask;
    crc = ~init_crc;
    for (i = 0; i < len; i++)
    {
        crc = crc ^ (uint32_t)pbuf[i];
        for (j = 7; j >= 0; j--)
        {
            mask = -(crc & 1);
            crc  = (crc >> 1) ^ (0xEDB88320 & mask);
        }
    }
    return ~crc;
}

static int _bIapEraseSpace()
{
    int           fd          = -1;
    uint32_t      sector_size = 0;
    bFlashErase_t param;

#if (IAP_FILE_CACHE == 0 || IAP_FILE_CACHE == 1)
    if (IAP_FILE_CACHE == 0)
    {
        addr = APP_START_ADDR - MCUFLASH_BASE_ADDR;
    }
    else
    {
        addr = IAP_FW_SAVE_ADDR - MCUFLASH_BASE_ADDR;
    }
    bHalFlashUnlock();
    bHalFlashErase(addr, (bIapFlag.info.len + bHalFlashSectorSize() - 1) / bHalFlashSectorSize());
    bHalFlashLock();
#elif (IAP_FILE_CACHE == 2)
    fd = bOpen(bIapFlag.info.dev_no, BCORE_FLAG_RW);
    if (fd == -1)
    {
        return -1;
    }
    bCtl(fd, bCMD_GET_SECTOR_SIZE, &sector_size);
    param.addr = IAP_FW_SAVE_ADDR;
    param.num  = (bIapFlag.info.len + sector_size - 1) / sector_size;
    //根据固件的大小，擦除区域等待数据写入
    bCtl(fd, bCMD_ERASE_SECTOR, &param);
    bClose(fd);
#endif
    return 0;
}

static int _bIapSetStat(uint8_t s)
{
    if (IS_IAP_STA(s) == 0)
    {
        return -1;
    }
    switch (s)
    {
        case B_IAP_STA_NULL:
        {
            ;
        }
        break;
        case B_IAP_STA_START:
        {
            if ((((uint32_t)_bIapEraseSpace) > APP_START_ADDR) && RECEIVE_FIRMWARE_MODE == 0)
            {
                ;
            }
            else
            {
                _bIapEraseSpace();
            }
        }
        break;
        case B_IAP_STA_READY:
        {
            ;
        }
        break;
        case B_IAP_STA_FINISHED:
        {
            bIapFlag.fail_count = 0;
        }
        break;
    }
    bIapFlag.stat = s;
    bHalFlashUnlock();
    bHalFlashErase(IAP_FLAG_ADDR - MCUFLASH_BASE_ADDR, 1);
    bIapFlag.fcrc =
        _bIapCalCRC32(0, (uint8_t *)&bIapFlag, sizeof(bIapFlag_t) - sizeof(bIapFlag.fcrc));
    bHalFlashWrite(IAP_FLAG_ADDR - MCUFLASH_BASE_ADDR, (uint8_t *)&bIapFlag, sizeof(bIapFlag_t));
    bHalFlashLock();
    return 0;
}

static int _IapCheckFwData()
{
    int      fd      = -1;
    uint32_t tmp_len = 0, r_len = 0;
    uint8_t  tmp[64];
    uint32_t tmp_crc = 0;

#if (IAP_FILE_CACHE == 2)
    fd = bOpen(bIapFlag.info.dev_no, BCORE_FLAG_RW);
    if (fd == -1)
    {
        return -1;
    }
    bLseek(fd, IAP_FW_SAVE_ADDR);
    while (tmp_len < bIapFlag.info.len)
    {
        r_len = ((bIapFlag.info.len - tmp_len) > 64) ? 64 : (bIapFlag.info.len - tmp_len);
        bRead(fd, tmp, r_len);
        tmp_crc = _bIapCalCRC32(tmp_crc, tmp, r_len);
        tmp_len += r_len;
    }
    bClose(fd);
    if (tmp_crc != bIapFlag.info.c_crc32)
    {
        return -1;
    }
#elif ((IAP_FILE_CACHE == 0) || (IAP_FILE_CACHE == 1))
    if (IAP_FILE_CACHE == 0)
    {
        addr = APP_START_ADDR;
    }
    else
    {
        addr = IAP_FW_SAVE_ADDR;
    }
    tmp_crc = _bIapCalCRC32(0, (uint8_t *)addr, bIapFlag.info.len);
    if (tmp_crc != bIapFlag.info.c_crc32)
    {
        return -1;
    }
#endif
    return 0;
}

static int _IapCopyFwData()
{
    int      retval  = 0;
    int      fd      = -1;
    uint32_t tmp_len = 0, r_len = 0;
    uint8_t  tmp[64];
    uint32_t tmp_crc = 0;
    uint8_t  retry   = 0;

#if (IAP_FILE_CACHE == 0)
    return 0;
#endif
    for (retry = 0; retry < B_IAP_FAIL_COUNT; retry++)
    {
        retval = 0;
#if (IAP_FILE_CACHE == 1)
        bHalFlashUnlock();
        bHalFlashErase((APP_START_ADDR - MCUFLASH_BASE_ADDR),
                       (bIapFlag.info.len + bHalFlashSectorSize() - 1) / bHalFlashSectorSize());
        bHalFlashWrite((APP_START_ADDR - MCUFLASH_BASE_ADDR), (uint8_t *)(IAP_FW_SAVE_ADDR),
                       bIapFlag.info.len);
        bHalFlashLock();
#endif

#if (IAP_FILE_CACHE == 2)
        fd = bOpen(bIapFlag.info.dev_no, BCORE_FLAG_RW);
        if (fd == -1)
        {
            return -1;
        }
        bHalFlashUnlock();
        bHalFlashErase((APP_START_ADDR - MCUFLASH_BASE_ADDR),
                       (bIapFlag.info.len + bHalFlashSectorSize() - 1) / bHalFlashSectorSize());
        bLseek(fd, IAP_FW_SAVE_ADDR);
        while (tmp_len < bIapFlag.info.len)
        {
            r_len = ((bIapFlag.info.len - tmp_len) > 64) ? 64 : (bIapFlag.info.len - tmp_len);
            bRead(fd, tmp, r_len);
            bHalFlashWrite((APP_START_ADDR - MCUFLASH_BASE_ADDR + tmp_len), tmp, r_len);
            tmp_len += r_len;
        }
        bClose(fd);
        bHalFlashLock();
#endif
        tmp_crc = _bIapCalCRC32(0, (uint8_t *)APP_START_ADDR, bIapFlag.info.len);
        if (tmp_crc != bIapFlag.info.c_crc32)
        {
            retval = -1;
        }
        else
        {
            break;
        }
    }
    return retval;
}

/**
 * \}
 */

/**
 * \addtogroup IAP_Exported_Functions
 * \{
 */

/**
 * \brief 默认使用重启，用户可以根据需求重写此函数
 */
__WEAKDEF void bIapJump2Boot()
{
    bHalIntDisable();
    ((pJumpFunc_t)(*((volatile uint32_t *)(BOOT_START_ADDR + 4))))();
}

/**
 * \brief 跳转到应用程序，用户可以根据需求重写此函数
 */
__WEAKDEF void bIapJump2App()
{
    bHalIntDisable();
    ((pJumpFunc_t)(*((volatile uint32_t *)(APP_START_ADDR + 4))))();
}

int bIapInit()
{
    uint32_t flag_check = 0;
    bHalFlashInit();
    bHalFlashRead(IAP_FLAG_ADDR - MCUFLASH_BASE_ADDR, (uint8_t *)&bIapFlag, sizeof(bIapFlag_t));
    flag_check = _bIapCalCRC32(0, (uint8_t *)&bIapFlag, sizeof(bIapFlag_t) - sizeof(bIapFlag.fcrc));
    if (flag_check != bIapFlag.fcrc)
    {
        memset(&bIapFlag, 0, sizeof(bIapFlag_t));
    }
    return 0;
}

int bIapStart(bIapFwInfo_t *pinfo)
{
    char *ptmp = NULL;
    if (pinfo == NULL)
    {
        return -1;
    }
    //文件名中必须包含当前的固件名，否则认为是非法文件
    ptmp = strstr(pinfo->name, FW_NAME);
    if (ptmp == NULL)
    {
        return -2;
    }
    memset(&bIapFlag, 0, sizeof(bIapFlag_t));
    //设置当前状态并复制固件信息
    bIapFlag.stat       = B_IAP_STA_START;
    bIapFlag.fail_count = 0;
    memcpy(&bIapFlag.info, pinfo, sizeof(bIapFwInfo_t));
    _bIapSetStat(B_IAP_STA_START);
#if (RECEIVE_FIRMWARE_MODE == 0)
    //跳转到执行BOOT代码
    bIapJump2Boot();
#endif
    return 0;
}

/**
 * \brief 传入新固件的数据用于写入存储区域
 * \param
 */

/**
 * \brief 传入新固件的数据用于写入存储区域
 * \param index 新固件数据的索引，即相对文件起始的偏移
 * \return int 0：正常存储  -1：存储异常   -2：校验失败，重新接收
 */
int bIapUpdateFwData(uint32_t index, uint8_t *pbuf, uint32_t len)
{
    int      retval = 0, fd = -1;
    uint32_t addr = index;
    if (pbuf == NULL || len == 0 || ((index) >= bIapFlag.info.len))
    {
        return -1;
    }
#if (IAP_FILE_CACHE == 2)
    fd = bOpen(bIapFlag.info.dev_no, BCORE_FLAG_RW);
    if (fd == -1)
    {
        return -1;
    }
    addr += IAP_FW_SAVE_ADDR;
    bLseek(fd, addr);
    bWrite(fd, pbuf, len);
    bClose(fd);
#elif ((IAP_FILE_CACHE == 0) || (IAP_FILE_CACHE == 1))
    if (IAP_FILE_CACHE == 0)
    {
        addr += APP_START_ADDR - BOOT_START_ADDR;
    }
    else
    {
        addr += IAP_FW_SAVE_ADDR - BOOT_START_ADDR;
    }
    bHalFlashUnlock();
    bHalFlashWrite(addr, pbuf, len);
    bHalFlashLock();
#endif
    /**
     * 数据接收完成后，计算校验，改变状态
     */
    if ((index + len) >= bIapFlag.info.len)
    {
        retval = _IapCheckFwData();
        /**
         * 接收完成，根据校验结果切换状态
         */
        if (retval < 0)
        {
#if (IAP_FILE_CACHE == 0)
            _bIapSetStat(B_IAP_STA_START);
            retval = -2;
#else
            _bIapSetStat(B_IAP_STA_NULL);
#endif
        }
        else
        {
#if (IAP_FILE_CACHE == 0)
            _bIapSetStat(B_IAP_STA_FINISHED);
#else
            _bIapSetStat(B_IAP_STA_READY);
#if (RECEIVE_FIRMWARE_MODE == 0)
            retval = _IapCopyFwData();
            if (retval == 0)
            {
                _bIapSetStat(B_IAP_STA_FINISHED);
            }
            else
            {
                _bIapSetStat(B_IAP_STA_START);
                retval = -2;
            }
#endif

#endif
        }
    }
    return retval;
}

/**
 * \brief 应用程序调用
 * \return int 0：正常跳转  1：升级完成跳转  -1：升级异常跳转
 */
int bIapAppCheckFlag()
{
    int retval = 0;
    if (bIapFlag.stat != B_IAP_STA_NULL)
    {
        if (bIapFlag.stat == B_IAP_STA_FINISHED)
        {
            retval = 1;
        }
        else
        {
            retval = -1;
        }
        _bIapSetStat(B_IAP_STA_NULL);
    }
    return retval;
}

/**
 * \brief 启动程序调用
 */
int bIapBootCheckFlag()
{
    int retval = 0;
#if (RECEIVE_FIRMWARE_MODE == 0)
    if (bIapFlag.stat == B_IAP_STA_START)
    {
        retval = 1;
        _bIapSetStat(B_IAP_STA_START);
    }
    else if (bIapFlag.stat == B_IAP_STA_READY)
    {
        retval = _IapCopyFwData();
        if (retval == 0)
        {
            _bIapSetStat(B_IAP_STA_FINISHED);
        }
        else
        {
            _bIapSetStat(B_IAP_STA_START);
            retval = 1;
        }
    }
    else if (bIapFlag.stat == B_IAP_STA_FINISHED)
    {
        if (bIapFlag.fail_count >= B_IAP_APP_FAIL_COUNT)
        {
            bIapFlag.fail_count = 0;
            retval              = 1;
            _bIapSetStat(B_IAP_STA_START);
        }
        else
        {
            retval = -1;
            bIapFlag.fail_count += 1;
        }
    }
#else
    if (bIapFlag.stat == B_IAP_STA_READY)
    {
        retval = _IapCopyFwData();
        if (retval == 0)
        {
            _bIapSetStat(B_IAP_STA_FINISHED);
        }
        else
        {
            /////拷贝备份程序
        }
    }
    else if (bIapFlag.stat == B_IAP_STA_FINISHED)
    {
        if (bIapFlag.fail_count >= B_IAP_FAIL_COUNT)
        {
            bIapFlag.fail_count = 0;
            /////拷贝备份程序
        }
        else
        {
            retval = -1;
            bIapFlag.fail_count += 1;
        }
    }
    else
    {
        retval = -1;
        _bIapSetStat(B_IAP_STA_NULL);
    }
#endif
    return retval;
}

/**
 * \brief 返回IAP当前状态
 */
uint8_t bIapGetStatus(void)
{
    return bIapFlag.stat;
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

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
