/**
 *!
 * \file        b_hal_flash.c
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SGPIOL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */

/*Includes ----------------------------------------------*/
#include <string.h>

#include "b_hal.h"

/**
 * \addtogroup B_HAL
 * \{
 */

/**
 * \addtogroup FLASH
 * \{
 */

/**
 * \defgroup FLASH_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup FLASH_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup FLASH_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup FLASH_Private_Variables
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup FLASH_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup FLASH_Private_Functions
 * \{
 */

/**
 * \}
 */

/**
 * \addtogroup FLASH_Exported_Functions
 * \{
 */

int bHalFlashInit()
{
    return 0;
}

int bHalFlashUnlock()
{
    int retval = -1;
    if (HAL_FLASH_Unlock() == HAL_OK)
    {
        retval = 0;
    }
    return retval;
}

int bHalFlashLock()
{
    int retval = -1;
    if (HAL_FLASH_Lock() == HAL_OK)
    {
        retval = 0;
    }
    return retval;
}

int bHalFlashErase(uint32_t page_addr, uint8_t pages)
{
    int                    retval = -1;
    FLASH_EraseInitTypeDef EraseInit;
    uint32_t               PageError;

    EraseInit.NbPages     = pages;
    EraseInit.PageAddress = page_addr;
    EraseInit.TypeErase   = FLASH_TYPEERASE_PAGES;

    if (HAL_FLASHEx_Erase(&EraseInit, &PageError) == HAL_OK)
    {
        retval = 0;
    }
    return retval;
}

int bHalFlashWrite(uint32_t addr, uint8_t *pbuf, uint16_t len)
{
    int      retval = -1;
    uint64_t wdata  = 0;
    uint16_t wlen = (len + 1) / 2, i = 0;

    if (pbuf == NULL || (addr & 0x1))
    {
        return -1;
    }

    for (i = 0; i < wlen; i++)
    {
        wdata = pbuf[i * 2 + 1];
        wdata = (wdata << 8) | pbuf[i * 2];
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr, wdata);
        addr += 2;
    }
    return (wlen * 2);
}

int bHalFlashRead(uint32_t addr, uint8_t *pbuf, uint16_t len)
{
    if (pbuf == NULL)
    {
        return -1;
    }
    memcpy(pbuf, (const uint8_t *)addr, len);
    return len;
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

/************************ Copyright (c) 2021 Bean *****END OF FILE****/
