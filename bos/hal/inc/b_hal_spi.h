/**
 *!
 * \file        b_hal_spi.h
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
#ifndef __B_HAL_SPI_H__
#define __B_HAL_SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

#include "hal/inc/b_hal_gpio.h"

/**
 * \addtogroup B_HAL
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
typedef enum
{
    B_HAL_SPI_1,
    B_HAL_SPI_2,
    B_HAL_SPI_3,
    B_HAL_SPI_4,
    B_HAL_SPI_INVALID,
} bHalSPINumber_t;

typedef enum
{
    B_HAL_SPI_SLOW,
    B_HAL_SPI_FAST,
    B_HAL_SPI_SPEED_INVALID,
} bHalSPISpeed_t;

typedef struct
{
    uint8_t is_simulation;
    union
    {
        bHalSPINumber_t spi;
        struct
        {
            bHalGPIOInstance_t miso;
            bHalGPIOInstance_t mosi;
            bHalGPIOInstance_t clk;
            uint8_t            CPOL;
            uint8_t            CPHA;
        } simulating_spi;
    } _if;
    bHalGPIOInstance_t cs;
} bHalSPIIf_t;

typedef struct
{
    int (*pSetSpeed)(bHalSPIIf_t *spi_if, bHalSPISpeed_t speed);
    int (*pSend)(bHalSPIIf_t *spi_if, const uint8_t *pbuf, uint16_t len);
    int (*pReceive)(bHalSPIIf_t *spi_if, uint8_t *pbuf, uint16_t len);
    uint8_t (*pTransfer)(bHalSPIIf_t *spi_if, uint8_t dat);
} const bHalSPIDriver_t;

/**
 * \}
 */

/**
 * \defgroup SPI_Exported_Variables
 * \{
 */

extern bHalSPIDriver_t bHalSPIDriver;

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
