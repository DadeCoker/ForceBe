/**
 *!
 * \file        b_util_spi.h
 * \version     v0.0.1
 * \date        2020/04/01
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
#ifndef __B_UTIL_SPI_H__
#define __B_UTIL_SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "hal/inc/b_hal.h"

/**
 * \addtogroup B_UTILS
 * \{
 */

/**
 * \addtogroup SPI
 * \{
 */

/**
 * \defgroup SPI_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
    bHalGPIOInstance_t miso;
    bHalGPIOInstance_t mosi;
    bHalGPIOInstance_t clk;
    uint8_t            CPOL;
    uint8_t            CPHA;
} bUtilSPI_t;
/**
 * \}
 */

/**
 * \defgroup SPI_Exported_Functions
 * \{
 */

uint8_t bUtilSPI_WriteRead(bUtilSPI_t spi, uint8_t dat);

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
