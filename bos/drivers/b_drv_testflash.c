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
 * \defgroup TESTFLASH_Private_Defines
 * \{
 */

#define DRIVER_NAME TESTFLASH

#define TEST_FLASH_SIZE (4 * 1024 * 1024)
#define IS_VALID_W_SIZE(w) ((w) == 1 || (w) == 2 || (w) == 4 || (w) == 8)
/**
 * \}
 */

/**
 * \defgroup TESTFLASH_Private_TypesDefinitions
 * \{
 */
typedef struct
{
    uint8_t data[TEST_FLASH_SIZE];
} bTestFlashPrivate_t;

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
bDRIVER_HALIF_TABLE(bTESTFLASH_HalIf_t, DRIVER_NAME);
static bTestFlashPrivate_t bTestFlashPrivate[bDRIVER_HALIF_NUM(bTESTFLASH_HalIf_t, DRIVER_NAME)];
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

static int _bTESTFLASHOpen(bDriverInterface_t *pdrv)
{
    return 0;
}

static int _bTESTFLASHClose(bDriverInterface_t *pdrv)
{
    return 0;
}

static int _bTESTFLASHWrite(bDriverInterface_t *pdrv, uint32_t off, uint8_t *pbuf, uint32_t len)
{
    uint32_t i = 0;
    bDRIVER_GET_HALIF(_if, bTESTFLASH_HalIf_t, pdrv);
    bDRIVER_GET_PRIVATE(_private, bTestFlashPrivate_t, pdrv);
    if (off >= TEST_FLASH_SIZE)
    {
        return -1;
    }
    len = (len + _if->w_size - 1) / _if->w_size;
    for (i = 0; i < len; i++)
    {
        if ((off + (i * _if->w_size)) >= TEST_FLASH_SIZE)
        {
            break;
        }
        if (_if->w_size == 1)
        {
            if (((uint8_t *)_private->data)[off + i] == 0xff)
            {
                ((uint8_t *)_private->data)[off + i] = ((uint8_t *)pbuf)[i];
            }
        }
        if (_if->w_size == 2)
        {
            if (((uint16_t *)_private->data)[off + i] == 0xffff)
            {
                ((uint16_t *)_private->data)[off + i] = ((uint16_t *)pbuf)[i];
            }
        }
        if (_if->w_size == 4)
        {
            if (((uint32_t *)_private->data)[off + i] == 0xffffffff)
            {
                ((uint32_t *)_private->data)[off + i] = ((uint32_t *)pbuf)[i];
            }
        }
        if (_if->w_size == 8)
        {
            if (((uint64_t *)_private->data)[off + i] == 0xffffffffffffffff)
            {
                ((uint64_t *)_private->data)[off + i] = ((uint64_t *)pbuf)[i];
            }
        }
    }
    return (i * _if->w_size);
}

static int _bTESTFLASHRead(bDriverInterface_t *pdrv, uint32_t off, uint8_t *pbuf, uint32_t len)
{
    bDRIVER_GET_PRIVATE(_private, bTestFlashPrivate_t, pdrv);
    if (off >= TEST_FLASH_SIZE)
    {
        return -1;
    }
    len = (len > (TEST_FLASH_SIZE - off)) ? (TEST_FLASH_SIZE - off) : len;
    memcpy(pbuf, (const void *)&_private->data[off], len);
    return len;
}

static int _bTESTFLASHCtl(bDriverInterface_t *pdrv, uint8_t cmd, void *param)
{
    int retval = -1;
    int i      = 0;
    bDRIVER_GET_HALIF(_if, bTESTFLASH_HalIf_t, pdrv);
    bDRIVER_GET_PRIVATE(_private, bTestFlashPrivate_t, pdrv);
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
                    perase_param->addr = (perase_param->addr) / _if->e_size * _if->e_size;
                    for (i = 0; i < perase_param->num; i++)
                    {
                        if ((perase_param->addr + i * _if->e_size) >= TEST_FLASH_SIZE)
                        {
                            break;
                        }
                        memset((void *)&_private->data[perase_param->addr + i * _if->e_size], 0xff,
                               _if->e_size);
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
                ((uint32_t *)param)[0] = _if->e_size;
                retval                 = 0;
            }
        }
        break;
        case bCMD_GET_SECTOR_COUNT:
        {
            if (param)
            {
                ((uint32_t *)param)[0] = TEST_FLASH_SIZE / _if->e_size;
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
int bTESTFLASH_Init(bDriverInterface_t *pdrv)
{
    bTestFlashPrivate_t *pflash = NULL;
    bDRIVER_STRUCT_INIT(pdrv, DRIVER_NAME, bTESTFLASH_Init);
    pdrv->close       = _bTESTFLASHClose;
    pdrv->read        = _bTESTFLASHRead;
    pdrv->ctl         = _bTESTFLASHCtl;
    pdrv->open        = _bTESTFLASHOpen;
    pdrv->write       = _bTESTFLASHWrite;
    pdrv->_private._p = (void *)&bTestFlashPrivate[pdrv->drv_no];

    pflash = (bTestFlashPrivate_t *)pdrv->_private._p;
    memset(pflash->data, 0xff, TEST_FLASH_SIZE);

    if (!IS_VALID_W_SIZE(((bTESTFLASH_HalIf_t *)pdrv->hal_if)->w_size))
    {
        return -1;
    }
    return 0;
}

bDRIVER_REG_INIT(B_DRIVER_TESTFLASH, bTESTFLASH_Init);

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
