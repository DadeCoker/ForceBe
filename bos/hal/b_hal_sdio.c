/**
 *!
 * \file        b_hal_sdio.c
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
 * \addtogroup SDIO
 * \{
 */

/**
 * \addtogroup SDIO_Exported_Functions
 * \{
 */

__WEAKDEF int bMcuSDIOReadBlocks(const bHalSDIONumber_t sd, uint8_t *pdata, uint32_t addr,
                                 uint32_t xblocks)
{
    return 0;
}

__WEAKDEF int bMcuSDIOWriteBlocks(const bHalSDIONumber_t sd, uint8_t *pdata, uint32_t addr,
                                  uint32_t xblocks)
{
    return 0;
}

__WEAKDEF int bMcuSDIOErase(const bHalSDIONumber_t sd, uint32_t addr, uint32_t xblocks)
{
    return 0;
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

int bHalSDIOReadBlocks(const bHalSDIONumber_t sd, uint8_t *pdata, uint32_t addr, uint32_t xblocks)
{
    if (sd > B_HAL_SDIO_1 || IS_NULL(pdata) || xblocks == 0)
    {
        return -1;
    }
    return bMcuSDIOReadBlocks(sd, pdata, addr, xblocks);
}

int bHalSDIOWriteBlocks(const bHalSDIONumber_t sd, uint8_t *pdata, uint32_t addr, uint32_t xblocks)
{
    if (sd > B_HAL_SDIO_1 || IS_NULL(pdata) || xblocks == 0)
    {
        return -1;
    }
    return bMcuSDIOWriteBlocks(sd, pdata, addr, xblocks);
}

int bHalSDIOErase(const bHalSDIONumber_t sd, uint32_t addr, uint32_t xblocks)
{
    if (sd > B_HAL_SDIO_1 || xblocks == 0)
    {
        return -1;
    }
    return bMcuSDIOErase(sd, addr, xblocks);
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
