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
#include "b_mod_fs.h"  
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
static FATFS    bFATFS_Table[E_DEV_NUMBER];
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
   
/**
 * \}
 */
   
/** 
 * \addtogroup FS_Exported_Functions
 * \{
 */
uint8_t bFileSystemWorkBuf[FF_MAX_SS];
int bFS_Init()
{
    FRESULT result = FR_OK;
    uint8_t disk_str[8];
    FATFS *fs;
    DWORD fre_clust, fre_sect, tot_sect;
#if _SPIFLASH_ENABLE
    sprintf((char *)disk_str, "%d:", DEV_SPIFLASH);
    result = f_mount(&bFATFS_Table[E_DEV_SPIFLASH], (const char *)disk_str, 1);
    if(result == FR_NO_FILESYSTEM)
    {
        b_log_w("no filesystem\r\n");
        if(f_mkfs((const char *)disk_str, NULL, bFileSystemWorkBuf, FF_MAX_SS) != FR_OK)
        {
            b_log_e("mkfs err..\r\n");
            return -1;
        }
    }
    else if(result != FR_OK)
    {
        b_log_e("sd mount err..%d\r\n", result);
        return -1;
    }
    /* Get volume information and free clusters of drive 1 */
    f_getfree((const char *)disk_str, &fre_clust, &fs);
    tot_sect = (fs->n_fatent - 2) * fs->csize;
    fre_sect = fre_clust * fs->csize;

    /* Print the free space (assuming 512 bytes/sector) */
    b_log("%10lu KiB total drive space.\n%10lu KiB available.\n", tot_sect * 4, fre_sect * 4);    
    
    
#endif
    
#if _SD_ENABLE 
    sprintf((char *)disk_str, "%d:", DEV_SDCARD);
    result = f_mount(&bFATFS_Table[E_DEV_SDCARD], (const char *)disk_str, 1);
    if(result != FR_OK)
    {
        b_log_e("sd mount err..%d\r\n", result);
        return -1;
    }
    f_getfree((const char *)disk_str, &fre_clust, &fs);
    tot_sect = (fs->n_fatent - 2) * fs->csize;
    fre_sect = fre_clust * fs->csize;

    /* Print the free space (assuming 512 bytes/sector) */
    b_log("%10lu KiB total drive space.\n%10lu KiB available.\n", tot_sect / 2, fre_sect / 2);  
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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/

