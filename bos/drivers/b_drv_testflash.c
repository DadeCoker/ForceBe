/**
 *!
 * \file        b_drv_testflash.c
 * \version     v0.0.1
 * \date        2022/10/29
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 Bean
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
#include "drivers/inc/b_drv_testflash.h"

#include <string.h>
/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup TESTFLASH
 * \{
 */

/**
 * \defgroup TESTFLASH_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup TESTFLASH_Private_Defines
 * \{
 */
#define TEST_FLASH_SIZE (4 * 1024 * 1024)
#define IS_VALID_W_SIZE(w) ((w) == 1 || (w) == 2 || (w) == 4 || (w) == 8)
/**
 * \}
 */

/**
 * \defgroup TESTFLASH_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup TESTFLASH_Private_Variables
 * \{
 */
const static bTESTFLASH_HalIf_t bTESTFLASH_HalIf = HAL_TESTFLASH_IF;
bTESTFLASH_Driver_t             bTESTFLASH_Driver;
volatile static uint8_t         bTestFlashMemory[TEST_FLASH_SIZE];
/**
 * \}
 */

/**
 * \defgroup TESTFLASH_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup TESTFLASH_Private_Functions
 * \{
 */

static int _bTESTFLASHOpen(bTESTFLASH_Driver_t *pdrv)
{
    return 0;
}

static int _bTESTFLASHClose(bTESTFLASH_Driver_t *pdrv)
{
    return 0;
}

static int _bTESTFLASHWrite(bTESTFLASH_Driver_t *pdrv, uint32_t off, uint8_t *pbuf, uint32_t len)
{
    uint32_t i = 0;
    if (off >= TEST_FLASH_SIZE)
    {
        return -1;
    }
    len = (len + bTESTFLASH_HalIf.w_size - 1) / bTESTFLASH_HalIf.w_size;
    for (i = 0; i < len; i++)
    {
        if ((off + (i * bTESTFLASH_HalIf.w_size)) >= TEST_FLASH_SIZE)
        {
            break;
        }
        if (bTESTFLASH_HalIf.w_size == 1)
        {
            if (((uint8_t *)bTestFlashMemory)[off + i] == 0xff)
            {
                ((uint8_t *)bTestFlashMemory)[off + i] = ((uint8_t *)pbuf)[i];
            }
        }
        if (bTESTFLASH_HalIf.w_size == 2)
        {
            if (((uint16_t *)bTestFlashMemory)[off + i] == 0xffff)
            {
                ((uint16_t *)bTestFlashMemory)[off + i] = ((uint16_t *)pbuf)[i];
            }
        }
        if (bTESTFLASH_HalIf.w_size == 4)
        {
            if (((uint32_t *)bTestFlashMemory)[off + i] == 0xffffffff)
            {
                ((uint32_t *)bTestFlashMemory)[off + i] = ((uint32_t *)pbuf)[i];
            }
        }
        if (bTESTFLASH_HalIf.w_size == 8)
        {
            if (((uint64_t *)bTestFlashMemory)[off + i] == 0xffffffffffffffff)
            {
                ((uint64_t *)bTestFlashMemory)[off + i] = ((uint64_t *)pbuf)[i];
            }
        }
    }
    return (i * bTESTFLASH_HalIf.w_size);
}

static int _bTESTFLASHRead(bTESTFLASH_Driver_t *pdrv, uint32_t off, uint8_t *pbuf, uint32_t len)
{
    if (off >= TEST_FLASH_SIZE)
    {
        return -1;
    }
    len = (len > (TEST_FLASH_SIZE - off)) ? (TEST_FLASH_SIZE - off) : len;
    memcpy(pbuf, (const void *)&bTestFlashMemory[off], len);
    return len;
}

static int _bTESTFLASHCtl(bTESTFLASH_Driver_t *pdrv, uint8_t cmd, void *param)
{
    int retval = -1;
    int i      = 0;
    switch (cmd)
    {
        case bCMD_ERASE_SECTOR:
        {
            if (param)
            {
                bFlashErase_t *perase_param = (bFlashErase_t *)param;
                bHalFlashErase(perase_param->addr, perase_param->num);
                if (perase_param->addr < TEST_FLASH_SIZE)
                {
                    perase_param->addr =
                        (perase_param->addr) / bTESTFLASH_HalIf.e_size * bTESTFLASH_HalIf.e_size;
                    for (i = 0; i < perase_param->num; i++)
                    {
                        if ((perase_param->addr + i * bTESTFLASH_HalIf.e_size) >= TEST_FLASH_SIZE)
                        {
                            break;
                        }
                        memset((void *)&bTestFlashMemory[perase_param->addr +
                                                         i * bTESTFLASH_HalIf.e_size],
                               0xff, bTESTFLASH_HalIf.e_size);
                    }
                }
                retval = 0;
            }
        }
        break;
        case bCMD_GET_SECTOR_SIZE:
        {
            if (param)
            {
                ((uint32_t *)param)[0] = bTESTFLASH_HalIf.e_size;
                retval                 = 0;
            }
        }
        break;
        case bCMD_GET_SECTOR_COUNT:
        {
            if (param)
            {
                ((uint32_t *)param)[0] = TEST_FLASH_SIZE / bTESTFLASH_HalIf.e_size;
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
 * \addtogroup TESTFLASH_Exported_Functions
 * \{
 */
int bTESTFLASH_Init()
{
    int retval = 0;

    if (!IS_VALID_W_SIZE(bTESTFLASH_HalIf.w_size))
    {
        bTESTFLASH_Driver.status = -1;
        return -1;
    }

    bTESTFLASH_Driver.status  = 0;
    bTESTFLASH_Driver.init    = bTESTFLASH_Init;
    bTESTFLASH_Driver.close   = _bTESTFLASHClose;
    bTESTFLASH_Driver.read    = _bTESTFLASHRead;
    bTESTFLASH_Driver.ctl     = _bTESTFLASHCtl;
    bTESTFLASH_Driver.open    = _bTESTFLASHOpen;
    bTESTFLASH_Driver.write   = _bTESTFLASHWrite;
    bTESTFLASH_Driver._hal_if = NULL;

    memset((void *)bTestFlashMemory, 0xff, TEST_FLASH_SIZE);
    return retval;
}

bDRIVER_REG_INIT(bTESTFLASH_Init);

/**
 * \}
 */

/**
 * \}
 */

/**
 * \}
 */

/************************ Copyright (c) 2022 Bean *****END OF FILE****/
