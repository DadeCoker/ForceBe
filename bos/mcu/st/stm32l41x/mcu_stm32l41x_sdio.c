/**
 *!
 * \file        mcu_stm32l4xx_sdio.c
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SSPIL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */

/*Includes ----------------------------------------------*/
#include "b_config.h"
#include "hal/inc/b_hal.h"

#if (MCU_PLATFORM == 1201)

#if defined (USE_HAL_DRIVER)
#include "stm32l4xx_hal.h"
#endif

#if defined (HAL_SD_MODULE_ENABLED) || defined (HAL_MMC_MODULE_ENABLED)

extern SD_HandleTypeDef hsd1;

int bMcuSDIOReadBlocks(const bHalSDIONumber_t sd, uint8_t *pdata, uint32_t addr, uint32_t xblocks)
{
    if (HAL_SD_ReadBlocks(&hsd1, pdata, addr, xblocks, 0xFFFFFFFF) == HAL_OK)
    {
        return 0;
    }
    return -1;
}

int bMcuSDIOWriteBlocks(const bHalSDIONumber_t sd, uint8_t *pdata, uint32_t addr, uint32_t xblocks)
{
    uint32_t tick = 0;
    if (HAL_SD_WriteBlocks(&hsd1, pdata, addr, xblocks, 0xFFFFFFFF) != HAL_OK)
    {
        return -1;
    }
    tick = bHalGetSysTick();
    while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER)
    {
        if(bHalGetSysTick() - tick >= MS2TICKS(5000))
        {
            return -1;
        }
    }
    return 0;
}

#endif

#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
