/**
 *!
 * \file        b_hal_sdio.h
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
#ifndef __B_HAL_SDIO_H__
#define __B_HAL_SDIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

/**
 * \addtogroup B_HAL
 * \{
 */

/**
 * \addtogroup SDIO
 * \{
 */

/**
 * \defgroup SDIO_Exported_TypesDefinitions
 * \{
 */
typedef enum
{
    B_HAL_SDIO_1,
    B_HAL_SDIO_INVALID,
} bHalSDIONumber_t;

/**
 * \}
 */

/**
 * \defgroup SDIO_Exported_Definitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SDIO_Exported_Functions
 * \{
 */
int bMcuSDIOReadBlocks(const bHalSDIONumber_t sd, uint8_t *pdata, uint32_t addr, uint32_t xblocks);
int bMcuSDIOWriteBlocks(const bHalSDIONumber_t sd, uint8_t *pdata, uint32_t addr, uint32_t xblocks);
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
int bHalSDIOReadBlocks(const bHalSDIONumber_t sd, uint8_t *pdata, uint32_t addr, uint32_t xblocks);
int bHalSDIOWriteBlocks(const bHalSDIONumber_t sd, uint8_t *pdata, uint32_t addr, uint32_t xblocks);
/**
 * \}
 */

/**
 * \}
 */

/**
 * \}
 */

#ifdef __cplusplus
}
#endif

#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
