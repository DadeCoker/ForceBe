/**
 *!
 * \file        mcu_mm32spin0x_spi.c
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
#include "hal/inc/b_hal_spi.h"

#if (MCU_PLATFORM == 3002)

int bMcuSpiSetSpeed(const bHalSPIIf_t *spi_if, bHalSPISpeed_t speed)
{
    return -1;
}

uint8_t bMcuSpiTransfer(const bHalSPIIf_t *spi_if, uint8_t dat)
{
    return 0;
}

int bMcuSpiSend(const bHalSPIIf_t *spi_if, const uint8_t *pbuf, uint16_t len)
{
    return -1;
}

int bMcuSpiReceive(const bHalSPIIf_t *spi_if, uint8_t *pbuf, uint16_t len)
{
    return -1;
}

#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
