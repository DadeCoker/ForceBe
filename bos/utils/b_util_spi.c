/**
 *!
 * \file        b_util_spi.c
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

/*Includes ----------------------------------------------*/
#include "utils/inc/b_util_spi.h"

/**
 * \addtogroup B_UTILS
 * \{
 */

/**
 * \addtogroup SPI
 * \{
 */

/**
 * \defgroup SPI_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SPI_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SPI_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SPI_Private_Variables
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SPI_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SPI_Private_Functions
 * \{
 */

/**
 * \}
 */

/**
 * \addtogroup SPI_Exported_Functions
 * \{
 */

uint8_t bUtilSPI_WriteRead(bUtilSPI_t spi, uint8_t dat)
{
    uint8_t polarity = (spi.CPOL == 0) ? 0 : 1;
    uint8_t init_p   = polarity;
    uint8_t i        = 0;
    bHalGPIODriver.pGpioWritePin(spi.clk.port, spi.clk.pin, polarity);
    if (spi.CPHA)
    {
        polarity = polarity ^ 0x01;
        bHalGPIODriver.pGpioWritePin(spi.clk.port, spi.clk.pin, polarity);
    }
    for (i = 0; i < 8; i++)
    {
        bHalGPIODriver.pGpioWritePin(spi.mosi.port, spi.mosi.pin, dat & 0x80);
        dat <<= 1;
        polarity = polarity ^ 0x01;
        bHalGPIODriver.pGpioWritePin(spi.clk.port, spi.clk.pin, polarity);

        if (bHalGPIODriver.pGpioReadPin(spi.miso.port, spi.miso.pin))
        {
            dat++;
        }

        polarity = polarity ^ 0x01;
        bHalGPIODriver.pGpioWritePin(spi.clk.port, spi.clk.pin, polarity);
    }
    if (init_p != polarity)
    {
        polarity = polarity ^ 0x01;
        bHalGPIODriver.pGpioWritePin(spi.clk.port, spi.clk.pin, polarity);
    }
    return dat;
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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
