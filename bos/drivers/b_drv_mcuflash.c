/**
 *!
 * \file        b_drv_mcuflash.c
 * \version     v0.0.1
 * \date        2021/06/13
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 Bean
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
#include "drivers/inc/b_drv_mcuflash.h"

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup MCUFLASH
 * \{
 */

/**
 * \defgroup MCUFLASH_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup MCUFLASH_Private_Defines
 * \{
 */
#define DRIVER_NAME MCUFLASH
/**
 * \}
 */

/**
 * \defgroup MCUFLASH_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup MCUFLASH_Private_Variables
 * \{
 */
bDRIVER_HALIF_TABLE(bMCUFLASH_HalIf_t, DRIVER_NAME);
/**
 * \}
 */

/**
 * \defgroup MCUFLASH_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup MCUFLASH_Private_Functions
 * \{
 */

static int _bMCUFLASHOpen(bDriverInterface_t *pdrv)
{
    return bHalFlashUnlock();
}

static int _bMCUFLASHClose(bDriverInterface_t *pdrv)
{
    return bHalFlashLock();
}

static int _bMCUFLASHWrite(bDriverInterface_t *pdrv, uint32_t off, uint8_t *pbuf, uint32_t len)
{
    return bHalFlashWrite(off, pbuf, len);
}

static int _bMCUFLASHRead(bDriverInterface_t *pdrv, uint32_t off, uint8_t *pbuf, uint32_t len)
{
    return bHalFlashRead(off, pbuf, len);
}

static int _bMCUFLASHCtl(bDriverInterface_t *pdrv, uint8_t cmd, void *param)
{
    int retval = -1;
    switch (cmd)
    {
        case bCMD_ERASE_SECTOR:
        {
            if (param)
            {
                bFlashErase_t *perase_param = (bFlashErase_t *)param;
                bHalFlashErase(perase_param->addr, perase_param->num);
                retval = 0;
            }
        }
        break;
        case bCMD_GET_SECTOR_SIZE:
        {
            if (param)
            {
                ((uint32_t *)param)[0] = bHalFlashSectorSize();
                retval                 = 0;
            }
        }
        break;
        case bCMD_GET_SECTOR_COUNT:
        {
            if (param)
            {
                ((uint32_t *)param)[0] = bHalFlashChipSize() / bHalFlashSectorSize();
                retval                 = 0;
            }
        }
        break;
    }
    return retval;
}

/**
 * \}
 */

/**
 * \addtogroup MCUFLASH_Exported_Functions
 * \{
 */
int bMCUFLASH_Init(bDriverInterface_t *pdrv)
{
    bDRIVER_STRUCT_INIT(pdrv, DRIVER_NAME, bMCUFLASH_Init);
    pdrv->close = _bMCUFLASHClose;
    pdrv->read  = _bMCUFLASHRead;
    pdrv->ctl   = _bMCUFLASHCtl;
    pdrv->open  = _bMCUFLASHOpen;
    pdrv->write = _bMCUFLASHWrite;

    if (bHalFlashInit() < 0)
    {
        return -1;
    }
    return 0;
}

bDRIVER_REG_INIT(B_DRIVER_MCUFLASH, bMCUFLASH_Init);

/**
 * \}
 */

/**
 * \}
 */

/**
 * \}
 */

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
