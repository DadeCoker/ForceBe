#include "b_os.h"
#include "ff.h" /* Obtains integer types */
/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include "diskio.h" /* Declarations of disk functions */
/* Definitions of physical drive number for each drive */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
#if (defined(_FS_ENABLE) && (_FS_ENABLE == 1)) && (defined(FS_FATFS))
    DSTATUS stat = RES_OK;
    switch (pdrv)
    {
        case DEV_SPIFLASH:
            return stat;

        case DEV_SDCARD:
            return stat;
    }
#endif
    return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
    return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(BYTE  pdrv,   /* Physical drive nmuber to identify the drive */
                  BYTE *buff,   /* Data buffer to store read data */
                  LBA_t sector, /* Start sector in LBA */
                  UINT  count   /* Number of sectors to read */
)
{
#if (defined(_FS_ENABLE) && (_FS_ENABLE == 1)) && (defined(FS_FATFS))
    DRESULT res = RES_OK;
    int     fd  = -1;
    int     len = 0;
    switch (pdrv)
    {
#if _SPIFLASH_ENABLE
        case DEV_SPIFLASH:
        {
            // translate the arguments here
            (void)len;
            uint32_t e_size = 0;
            fd              = bOpen(bSPIFLASH, BCORE_FLAG_RW);
            if (fd >= 0)
            {
                if (bCtl(fd, bCMD_GET_SECTOR_SIZE, &e_size) == 0)
                {
                    bLseek(fd, sector * e_size);
                    bRead(fd, buff, count * e_size);
                }
                else
                {
                    res = RES_ERROR;
                }
                bClose(fd);
            }
            else
            {
                res = RES_ERROR;
            }

            // translate the reslut code here

            return res;
        }
#endif

#if _SD_ENABLE
        case DEV_SDCARD:
        {
            // translate the arguments here

            fd = bOpen(bSD, BCORE_FLAG_RW);
            if (fd >= 0)
            {
                bLseek(fd, sector);
                len = bRead(fd, buff, count);
                bClose(fd);
                if (len < 0)
                {
                    res = RES_ERROR;
                }
            }
            else
            {
                res = RES_ERROR;
            }

            // translate the reslut code here

            return res;
        }
#endif
    }
#endif
    return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(BYTE        pdrv,   /* Physical drive nmuber to identify the drive */
                   const BYTE *buff,   /* Data to be written */
                   LBA_t       sector, /* Start sector in LBA */
                   UINT        count   /* Number of sectors to write */
)
{
#if (defined(_FS_ENABLE) && (_FS_ENABLE == 1)) && (defined(FS_FATFS))
    DRESULT res = RES_OK;
    int     fd  = -1;
    int     len = 0;
    switch (pdrv)
    {
#if _SPIFLASH_ENABLE
        case DEV_SPIFLASH:
        {
            // translate the arguments here
            (void)len;
            bFlashErase_t cmd_erase;
            uint32_t      e_size = 0;
            fd                   = bOpen(bSPIFLASH, BCORE_FLAG_RW);
            if (fd >= 0)
            {
                if (bCtl(fd, bCMD_GET_SECTOR_SIZE, &e_size) == 0)
                {
                    cmd_erase.addr = sector * e_size;
                    cmd_erase.num  = count;
                    bCtl(fd, bCMD_ERASE_SECTOR, &cmd_erase);
                    bLseek(fd, sector * e_size);
                    bWrite(fd, (uint8_t *)buff, count * e_size);
                }
                else
                {
                    res = RES_ERROR;
                }
                bClose(fd);
            }
            else
            {
                res = RES_ERROR;
            }

            // translate the reslut code here

            return res;
        }
#endif

#if _SD_ENABLE
        case DEV_SDCARD:
        {
            // translate the arguments here

            fd = bOpen(bSD, BCORE_FLAG_RW);
            if (fd >= 0)
            {
                bLseek(fd, sector);
                len = bWrite(fd, (uint8_t *)buff, count);
                bClose(fd);
                if (len < 0)
                {
                    res = RES_ERROR;
                }
            }
            else
            {
                res = RES_ERROR;
            }

            // translate the reslut code here

            return res;
        }
#endif
    }
#endif
    return RES_PARERR;
}

#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE  pdrv, /* Physical drive nmuber (0..) */
                   BYTE  cmd,  /* Control code */
                   void *buff  /* Buffer to send/receive control data */
)
{
#if (defined(_FS_ENABLE) && (_FS_ENABLE == 1))    
    DRESULT res = RES_OK;    
    switch (pdrv)
    {
#if _SPIFLASH_ENABLE
        case DEV_SPIFLASH:
        {
            int fd = -1;
            fd     = bOpen(bSPIFLASH, BCORE_FLAG_RW);
            if (fd >= 0)
            {
                switch (cmd)
                {
                    case GET_SECTOR_COUNT:
                        if (bCtl(fd, bCMD_GET_SECTOR_COUNT, buff) < 0)
                        {
                            res = RES_ERROR;
                        }
                        break;
                    case GET_SECTOR_SIZE:
                        if (bCtl(fd, bCMD_GET_SECTOR_SIZE, buff) < 0)
                        {
                            res = RES_ERROR;
                        }
                        break;
                    case GET_BLOCK_SIZE:
                        ((WORD *)buff)[0] = 1;
                        break;
                }
                bClose(fd);
            }
            else
            {
                res = RES_ERROR;
            }
            // Process of the command for the RAM drive
            return res;
        }
#endif
#if _SD_ENABLE        
        case DEV_SDCARD:
            switch (cmd)
            {
                case GET_SECTOR_COUNT:
                    ((LBA_t *)buff)[0] = SD_SIZE_XG * 1024 * 1024 / 512 * 1024;
                    break;
                case GET_SECTOR_SIZE:
                    ((WORD *)buff)[0] = 512;
                    break;
                case GET_BLOCK_SIZE:
                    ((WORD *)buff)[0] = 8;
                    break;
            }
            // Process of the command for the MMC/SD card
            return res;   
#endif         
    }       
#endif
    return RES_PARERR;
}
