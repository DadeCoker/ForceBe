/**
 *!
 * \file        b_mod_fs.c
 * \version     v0.0.1
 * \date        2020/06/02
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */

/*Includes ----------------------------------------------*/
#include "modules/inc/b_mod_fs.h"
#if _FS_ENABLE
#include <stdio.h>
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup FS
 * \{
 */

/**
 * \defgroup FS_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup FS_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup FS_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup FS_Private_Variables
 * \{
 */
#if _FS_SELECT == 0
static FATFS bFATFS_Table[E_DEV_NUMBER];
#endif

#if _FS_SELECT == 1
lfs_t bLittleFS;
#endif

/**
 * \}
 */

/**
 * \defgroup FS_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup FS_Private_Functions
 * \{
 */
#if _FS_SELECT == 1
#include "core/inc/b_core.h"

static int _bFS_DeviceRead(const struct lfs_config *c, lfs_block_t block, lfs_off_t off,
                           void *buffer, lfs_size_t size)
{
    int      fd     = -1;
    int      retval = LFS_ERR_OK;
    uint32_t e_size = 4096;
    fd              = bOpen(SPIFLASH, BCORE_FLAG_RW);
    if (fd >= 0)
    {
        if (bCtl(fd, bCMD_GET_SECTOR_SIZE, &e_size) == 0)
        {
            bLseek(fd, block * e_size + off);
            bRead(fd, buffer, size);
        }
        else
        {
            retval = LFS_ERR_CORRUPT;
        }
        bClose(fd);
    }
    else
    {
        retval = LFS_ERR_CORRUPT;
    }
    return retval;
}

int _bFS_DeviceWrite(const struct lfs_config *c, lfs_block_t block, lfs_off_t off,
                     const void *buffer, lfs_size_t size)
{
    int      fd     = -1;
    uint32_t e_size = 4096;
    int      retval = LFS_ERR_OK;
    fd              = bOpen(SPIFLASH, BCORE_FLAG_RW);
    if (fd >= 0)
    {
        if (bCtl(fd, bCMD_GET_SECTOR_SIZE, &e_size) == 0)
        {
            bLseek(fd, block * e_size + off);
            bWrite(fd, (uint8_t *)buffer, size);
        }
        else
        {
            retval = LFS_ERR_CORRUPT;
        }
        bClose(fd);
    }
    else
    {
        return LFS_ERR_CORRUPT;
    }
    return retval;
}

int _bFS_DeviceErase(const struct lfs_config *c, lfs_block_t block)
{
    int           fd     = -1;
    int           retval = LFS_ERR_OK;
    uint32_t      e_size = 4096;
    bFlashErase_t cmd;
    fd = bOpen(SPIFLASH, BCORE_FLAG_RW);
    if (fd >= 0)
    {
        if (bCtl(fd, bCMD_GET_SECTOR_SIZE, &e_size) == 0)
        {
            cmd.addr = block * e_size;
            cmd.num  = 1;
            bCtl(fd, bCMD_ERASE_SECTOR, &cmd);
        }
        else
        {
            retval = LFS_ERR_CORRUPT;
        }
        bClose(fd);
    }
    else
    {
        return LFS_ERR_CORRUPT;
    }
    return retval;
}

int _bFS_DeviceSync(const struct lfs_config *c)
{
    return LFS_ERR_OK;
}

#endif
/**
 * \}
 */

/**
 * \addtogroup FS_Exported_Functions
 * \{
 */
#if _FS_SELECT == 0
uint8_t bFileSystemWorkBuf[FF_MAX_SS];
int     bFS_Init()
{
    FRESULT result = FR_OK;
    uint8_t disk_str[8];
    FATFS * fs;
    DWORD   fre_clust, fre_sect, tot_sect;
#if _SPIFLASH_ENABLE
    sprintf((char *)disk_str, "%d:", DEV_SPIFLASH);
    result = f_mount(&bFATFS_Table[E_DEV_SPIFLASH], (const char *)disk_str, 1);
    if (result == FR_NO_FILESYSTEM)
    {
        b_log_w("no filesystem\r\n");
        if (f_mkfs((const char *)disk_str, NULL, bFileSystemWorkBuf, FF_MAX_SS) != FR_OK)
        {
            b_log_e("mkfs err..\r\n");
            return -1;
        }
    }
    else if (result != FR_OK)
    {
        b_log_e("sd mount err..%d\r\n", result);
        return -1;
    }
    /* Get volume information and free clusters of drive 1 */
    f_getfree((const char *)disk_str, &fre_clust, &fs);
    tot_sect = (fs->n_fatent - 2) * fs->csize;
    fre_sect = fre_clust * fs->csize;

    /* Print the free space (assuming 512 bytes/sector) */
    b_log("%10lu KiB total drive space.\n%10lu KiB available.\r\n", tot_sect * 4, fre_sect * 4);

#endif

#if _SD_ENABLE
    sprintf((char *)disk_str, "%d:", DEV_SDCARD);
    result = f_mount(&bFATFS_Table[E_DEV_SDCARD], (const char *)disk_str, 1);
    if (result != FR_OK)
    {
        b_log_e("sd mount err..%d\r\n", result);
        return -1;
    }
    f_getfree((const char *)disk_str, &fre_clust, &fs);
    tot_sect = (fs->n_fatent - 2) * fs->csize;
    fre_sect = fre_clust * fs->csize;

    /* Print the free space (assuming 512 bytes/sector) */
    b_log("%10lu KiB total drive space.\r\n%10lu KiB available.\r\n", tot_sect / 2, fre_sect / 2);
#endif
    return 0;
}

#if _FS_TEST_ENABLE
FIL fil;
int bFS_Test()
{
    // read current count
    uint32_t boot_count = 0;
    UINT     brw        = 0;
    FRESULT  fr; /* FatFs return code */
#if _SD_ENABLE
    fr = f_open(&fil, "1:test.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
#else
    fr = f_open(&fil, "0:test.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
#endif
    if (fr)
    {
        b_log_e("open %d\r\n", fr);
        f_close(&fil);
        return -1;
    }
    f_lseek(&fil, 0);
    fr = f_read(&fil, &boot_count, sizeof(boot_count), &brw);
    if (fr)
    {
        b_log_e("read %d\r\n", fr);
        f_close(&fil);
        return -1;
    }
    if (brw != sizeof(boot_count))
    {
        b_log_e("read %d %d\r\n", brw, boot_count);
        boot_count = 0;
    }
    else
    {
        b_log("boot_count %d\r\n", boot_count);
        boot_count++;
    }
    f_lseek(&fil, 0);
    fr = f_write(&fil, &boot_count, sizeof(boot_count), &brw);
    if (fr)
    {
        b_log_e("write %d\r\n", fr);
        f_close(&fil);
        return -1;
    }
    f_close(&fil);
    return 0;
}
#else
int bFS_Test()
{
    return 0;
}
#endif

#endif

#if (_FS_SELECT == 1)
// configuration of the filesystem is provided by this struct
const struct lfs_config cfg = {
    // block device operations
    .read  = _bFS_DeviceRead,
    .prog  = _bFS_DeviceWrite,
    .erase = _bFS_DeviceErase,
    .sync  = _bFS_DeviceSync,

    // block device configuration
    .read_size      = 16,
    .prog_size      = 16,
    .block_size     = 4096,
    .block_count    = _SPIFLASH_SIZE * 1024 * 1024 / 4096,
    .cache_size     = 16,
    .lookahead_size = 16,
    .block_cycles   = 500,
};

int bFS_Init()
{
    // mount the filesystem
    int err = lfs_mount(&bLittleFS, &cfg);
    b_log("mount:%d\r\n", err);
    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err)
    {
        lfs_format(&bLittleFS, &cfg);
        lfs_mount(&bLittleFS, &cfg);
    }
    return 0;
}

#if _FS_TEST_ENABLE
lfs_file_t file;
int        bFS_Test()
{
    // read current count
    lfs_t *  plsf       = &bLittleFS;
    uint32_t boot_count = 0;
    lfs_file_open(plsf, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(plsf, &file, &boot_count, sizeof(boot_count));

    // update boot count
    boot_count += 1;
    lfs_file_rewind(plsf, &file);
    lfs_file_write(plsf, &file, &boot_count, sizeof(boot_count));

    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(plsf, &file);

    // release any resources we were using
    lfs_unmount(plsf);

    // print the boot count
    b_log("boot_count: %d\n", boot_count);
    return 0;
}
#else
int bFS_Test()
{
    return 0;
}
#endif

#endif

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
