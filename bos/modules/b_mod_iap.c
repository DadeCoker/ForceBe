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
static uint8_t    bIapMcuDevNo = 0;
static bIapFlag_t bIapFlag     = {
        .stat = B_IAP_STA_NULL,
};
static bIapFwInfo_t bIapFwInfo = {
    .len = 0,
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
static int _bIapSaveFlag()
{
    bFlashErase_t param;
    int           fd = -1;
    param.addr       = IAP_FLAG_ADDR - BOOT_START_ADDR;
    param.num        = 1;
    fd               = bOpen(bIapMcuDevNo, BCORE_FLAG_RW);
    if (fd == -1)
    {
        return -1;
    }
    bCtl(fd, bCMD_ERASE_SECTOR, &param);
    bLseek(fd, IAP_FLAG_ADDR - BOOT_START_ADDR);
    bWrite(fd, (uint8_t *)&bIapFlag, sizeof(bIapFlag_t));
    bClose(fd);
    return 0;
}

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

int bIapInit(uint8_t dev_no)
{
    int fd       = -1;
    bIapMcuDevNo = dev_no;
    fd           = bOpen(dev_no, BCORE_FLAG_R);
    if (fd == -1)
    {
        return -1;
    }
    bLseek(fd, IAP_FLAG_ADDR - BOOT_START_ADDR);
    bRead(fd, (uint8_t *)&bIapFlag, sizeof(bIapFlag_t));
    bClose(fd);
    return 0;
}

int bIapStart(const char *pfname)
{
    char *ptmp = NULL;
    if (pfname == NULL)
    {
        return -1;
    }
    if (strlen(pfname) > B_IAP_FILENAME_MAXLEN)
    {
        return -2;
    }
    //文件名中必须包含当前的固件名，否则认为是非法文件
    ptmp = strstr(pfname, FW_NAME);
    if (ptmp == NULL)
    {
        return -3;
    }
    memset(&bIapFlag, 0, sizeof(bIapFlag_t));
    //设置当前状态并复制文件名
    bIapFlag.stat           = B_IAP_STA_START;
    bIapFlag.app_invalid    = 0;
    bIapFlag.app_fail_count = 0;
    memcpy(bIapFlag.file_name, pfname, strlen(pfname));
    //标志写入到FLASH
    _bIapSaveFlag();
    //跳转到执行BOOT代码
    bIapJump2Boot();
    return 0;
}

/**
 * \brief 应用程序调用，清除标致
 */
int bIapAppCheckFlag()
{
    if (bIapFlag.stat != B_IAP_STA_NULL)
    {
        bIapFlag.stat = B_IAP_STA_NULL;
        _bIapSaveFlag();
    }
    return 0;
}

/**
 * \brief Boot调用，查询接下来做什么操作
 * \return int B_BOOT_JUMP2APP：跳转应用程序  B_BOOT_WAIT_FW：等待接收新固件
 */
int bIapBootCheckFlag()
{
    int retval = B_BOOT_JUMP2APP;
    if (bIapFlag.stat == B_IAP_STA_READY)
    {
        bIapFlag.app_fail_count += 1;
        if (bIapFlag.app_fail_count >= B_IAP_APP_FAIL_MAXCOUNT)
        {
            bIapFlag.stat        = B_IAP_STA_START;
            bIapFlag.app_invalid = 1;
            retval               = B_BOOT_WAIT_FW;
        }
        _bIapSaveFlag();
    }
    else if (bIapFlag.stat == B_IAP_STA_START)
    {
        retval = B_BOOT_WAIT_FW;
    }
    return retval;
}

/**
 * \brief 反馈更新固件的结果
 * \param stat B_UPDATE_FW_ERR 、B_UPDATE_FW_OK
 * \return int B_BOOT_JUMP2APP：跳转应用程序  B_BOOT_WAIT_FW：等待接收新固件
 */
int bIapUpdateFwResult(int result)
{
    int retval = B_BOOT_JUMP2APP;

    if (result == B_UPDATE_FW_OK)
    {
        bIapFlag.stat           = B_IAP_STA_READY;
        bIapFlag.app_fail_count = 0;
        bIapFlag.app_invalid    = 0;
        retval                  = B_BOOT_JUMP2APP;
    }
    else
    {
        if (bIapFlag.app_invalid == 0)
        {
            bIapFlag.stat = B_IAP_STA_NULL;
            retval        = B_BOOT_JUMP2APP;
        }
        else
        {
            retval = B_BOOT_WAIT_FW;
        }
    }
    if (retval == B_BOOT_JUMP2APP)
    {
        _bIapSaveFlag();
    }
    return retval;
}

/**
 * \brief 传入固件信息，如果需要设备暂存固件，则根据固件大小，擦除区域
 * \param pinfo 固件暂存设备的设备号、固件大小以及固件校验值
 */
int bIapSetFwInfo(bIapFwInfo_t *pinfo)
{
    bFlashErase_t param;
    uint32_t      sector_size = 0;
    int           fd          = -1;
    if (pinfo == NULL)
    {
        return -1;
    }
    memcpy(&bIapFwInfo, pinfo, sizeof(bIapFwInfo_t));
#if IAP_FILE_CACHE == 0
    bIapFlag.app_invalid = 1;
    _bIapSaveFlag();
#endif
#if ((IAP_FILE_CACHE == 0) || (IAP_FILE_CACHE == 1) || (IAP_FILE_CACHE == 2))
    fd = bOpen(bIapFwInfo.dev_no, BCORE_FLAG_RW);
    if (fd == -1)
    {
        return -1;
    }
    bCtl(fd, bCMD_GET_SECTOR_SIZE, &sector_size);
#if IAP_FILE_CACHE == 0
    param.addr = APP_START_ADDR - BOOT_START_ADDR;
#elif IAP_FILE_CACHE == 1
    param.addr = IAP_FW_SAVE_ADDR - BOOT_START_ADDR;
#else
    param.addr = IAP_FW_SAVE_ADDR;
#endif
    param.num = (bIapFwInfo.len + sector_size - 1) / sector_size;
    //根据固件的大小，擦除区域等待数据写入
    bCtl(fd, bCMD_ERASE_SECTOR, &param);
    bClose(fd);
#endif
    return 0;
}

/**
 * \brief 传入新固件的数据用于写入存储区域
 * \param index 当前数据的起始序号，从0开始
 */
int bIapUpdateFwData(uint32_t index, uint8_t *pbuf, uint32_t len)
{
    int      fd   = -1;
    uint32_t addr = index;
    if (pbuf == NULL || len == 0 || ((index + len) > bIapFwInfo.len))
    {
        return -1;
    }
#if ((IAP_FILE_CACHE == 0) || (IAP_FILE_CACHE == 1) || (IAP_FILE_CACHE == 2))
    fd = bOpen(bIapFwInfo.dev_no, BCORE_FLAG_RW);
    if (fd == -1)
    {
        return -1;
    }
#if IAP_FILE_CACHE == 0
    addr += APP_START_ADDR - BOOT_START_ADDR;
#elif IAP_FILE_CACHE == 1
    addr += IAP_FW_SAVE_ADDR - BOOT_START_ADDR;
#else
    addr += IAP_FW_SAVE_ADDR;
#endif
    bLseek(fd, addr);
    bWrite(fd, pbuf, len);
    bClose(fd);
#endif
    return 0;
}

int bIapVerifyFwData()
{
    int fd_mcu = -1;
#if ((IAP_FILE_CACHE) == 2)
    int      fd_flash = -1;
    uint32_t rlen     = 0;
    uint32_t i        = 0;
    uint8_t  tmp[128];
#endif

#if ((IAP_FILE_CACHE) != 0)
    bFlashErase_t param;
    uint32_t      sector_size = 0;
    uint32_t      addr        = APP_START_ADDR - BOOT_START_ADDR;
#endif
    fd_mcu = bOpen(bIapMcuDevNo, BCORE_FLAG_RW);
    if (fd_mcu == -1)
    {
        return -1;
    }
    //搬运固件
#if ((IAP_FILE_CACHE) != 0)
    bCtl(fd_mcu, bCMD_GET_SECTOR_SIZE, &sector_size);
    param.addr = APP_START_ADDR - BOOT_START_ADDR;
    param.num  = (bIapFwInfo.len + sector_size - 1) / sector_size;
    //根据固件的大小，擦除区域等待数据写入
    bCtl(fd_mcu, bCMD_ERASE_SECTOR, &param);
    //擦除FLASH后，标记当前应用程序为无效
    bIapFlag.app_invalid = 1;
    param.addr           = IAP_FLAG_ADDR - BOOT_START_ADDR;
    param.num            = 1;
    bCtl(fd_mcu, bCMD_ERASE_SECTOR, &param);
    bLseek(fd_mcu, IAP_FLAG_ADDR - BOOT_START_ADDR);
    bWrite(fd_mcu, (uint8_t *)&bIapFlag, sizeof(bIapFlag_t));
#endif

#if (IAP_FILE_CACHE == 1)
    bLseek(fd_mcu, addr);
    bWrite(fd_mcu, (uint8_t *)IAP_FW_SAVE_ADDR, bIapFwInfo.len);
#elif (IAP_FILE_CACHE == 2)
    fd_flash = bOpen(bIapFwInfo.dev_no, BCORE_FLAG_RW);
    if (fd_flash == -1)
    {
        bClose(fd_mcu);
        return -1;
    }
    bLseek(fd_flash, IAP_FW_SAVE_ADDR);
    bLseek(fd_mcu, addr);
    for (i = 0; i < bIapFwInfo.len;)
    {
        rlen = ((bIapFwInfo.len - i) > 128) ? 128 : (bIapFwInfo.len - i);
        bRead(fd_flash, tmp, rlen);
        bWrite(fd_mcu, tmp, rlen);
        i += rlen;
    }
    bClose(fd_flash);
#endif
    bClose(fd_mcu);
#if _IAP_CHECKSUM_ENABLE
    if (_bIapCalCRC32(0, (uint8_t *)APP_START_ADDR, bIapFwInfo.len) != bIapFwInfo.c_crc32)
    {
        return -1;
    }
#endif
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

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
