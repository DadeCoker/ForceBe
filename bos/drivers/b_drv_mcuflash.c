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

bMCUFLASH_Driver_t bMCUFLASH_Driver;
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

static int _bMCUFLASHOpen(bMCUFLASH_Driver_t *pdrv)
{
    return bHalFlashUnlock();
}

static int _bMCUFLASHClose(bMCUFLASH_Driver_t *pdrv)
{
    return bHalFlashLock();
}

static int _bMCUFLASHWrite(bMCUFLASH_Driver_t *pdrv, uint32_t off, uint8_t *pbuf, uint16_t len)
{
    return bHalFlashWrite(off, pbuf, len);
}

static int _bMCUFLASHRead(bMCUFLASH_Driver_t *pdrv, uint32_t off, uint8_t *pbuf, uint16_t len)
{
    return bHalFlashRead(off, pbuf, len);
}

static int _bMCUFLASHCtl(bMCUFLASH_Driver_t *pdrv, uint8_t cmd, void *param)
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
int bMCUFLASH_Init()
{
    int retval               = 0;
    bMCUFLASH_Driver.status  = 0;
    bMCUFLASH_Driver.init    = bMCUFLASH_Init;
    bMCUFLASH_Driver.close   = _bMCUFLASHClose;
    bMCUFLASH_Driver.read    = _bMCUFLASHRead;
    bMCUFLASH_Driver.ctl     = _bMCUFLASHCtl;
    bMCUFLASH_Driver.open    = _bMCUFLASHOpen;
    bMCUFLASH_Driver.write   = _bMCUFLASHWrite;
    bMCUFLASH_Driver._hal_if = NULL;

    if (bHalFlashInit() < 0)
    {
        bMCUFLASH_Driver.status = -1;
        retval                  = -1;
    }
    return retval;
}

bDRIVER_REG_INIT(bMCUFLASH_Init);

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
