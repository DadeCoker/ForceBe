/**
 *!
 * \file        b_hal_flash.c
 * \version     v0.0.1
 * \date        2020/03/25
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SUARTL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
/*Includes ----------------------------------------------*/
#include "hal/inc/b_hal.h"

/**
 * \addtogroup B_HAL
 * \{
 */

/**
 * \addtogroup FLASH
 * \{
 */

/**
 * \addtogroup FLASH_Exported_Functions
 * \{
 */

__WEAKDEF int bMcuFlashInit()
{
    return -1;
}

__WEAKDEF int bMcuFlashUnlock()
{
    return -1;
}

__WEAKDEF int bMcuFlashLock()
{
    return -1;
}

__WEAKDEF int bMcuFlashErase(uint32_t raddr, uint8_t pages)
{
    return -1;
}

__WEAKDEF int bMcuFlashWrite(uint32_t raddr, const uint8_t *pbuf, uint16_t len)
{
    return -1;
}

__WEAKDEF int bMcuFlashRead(uint32_t raddr, uint8_t *pbuf, uint16_t len)
{
    return -1;
}

__WEAKDEF uint32_t bMcuFlashSectorSize()
{
    return 0;
}

__WEAKDEF uint32_t bMcuFlashChipSize()
{
    return 0;
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

int bHalFlashInit()
{
    return bMcuFlashInit();
}

int bHalFlashUnlock()
{
    return bMcuFlashUnlock();
}

int bHalFlashLock()
{
    return bMcuFlashLock();
}

int bHalFlashErase(uint32_t raddr, uint8_t pages)
{
    return bMcuFlashErase(raddr, pages);
}

int bHalFlashWrite(uint32_t raddr, const uint8_t *pbuf, uint16_t len)
{
    return bMcuFlashWrite(raddr, pbuf, len);
}

int bHalFlashRead(uint32_t raddr, uint8_t *pbuf, uint16_t len)
{
    return bMcuFlashRead(raddr, pbuf, len);
}

uint32_t bHalFlashSectorSize()
{
    return bMcuFlashSectorSize();
}

uint32_t bHalFlashChipSize()
{
    return bMcuFlashChipSize();
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

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
