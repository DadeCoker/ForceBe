/**
 *!
 * \file        b_hal_spi.c
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
 * \addtogroup SPI
 * \{
 */

/**
 * \defgroup SPI_Private_TypesDefinitions
 */
typedef struct
{
    bHalGPIOInstance_t miso;
    bHalGPIOInstance_t mosi;
    bHalGPIOInstance_t clk;
    uint8_t            CPOL;
    uint8_t            CPHA;
} bHalSPIIO_t;

/**
 * \}
 */

/**
 * \addtogroup SPI_Private_Functions
 * \{
 */

static uint8_t _bHalSPIIOWriteRead(bHalSPIIO_t spi, uint8_t dat)
{
    uint8_t polarity = (spi.CPOL == 0) ? 0 : 1;
    uint8_t init_p   = polarity;
    uint8_t i        = 0;
    bHalGpioWritePin(spi.clk.port, spi.clk.pin, polarity);
    if (spi.CPHA)
    {
        polarity = polarity ^ 0x01;
        bHalGpioWritePin(spi.clk.port, spi.clk.pin, polarity);
    }
    for (i = 0; i < 8; i++)
    {
        bHalGpioWritePin(spi.mosi.port, spi.mosi.pin, dat & 0x80);
        dat <<= 1;
        polarity = polarity ^ 0x01;
        bHalGpioWritePin(spi.clk.port, spi.clk.pin, polarity);

        if (bHalGpioReadPin(spi.miso.port, spi.miso.pin))
        {
            dat++;
        }

        polarity = polarity ^ 0x01;
        bHalGpioWritePin(spi.clk.port, spi.clk.pin, polarity);
    }
    if (init_p != polarity)
    {
        polarity = polarity ^ 0x01;
        bHalGpioWritePin(spi.clk.port, spi.clk.pin, polarity);
    }
    return dat;
}

/**
 * \}
 */

/**
 * \addtogroup SPI_Exported_Functions
 * \{
 */

__WEAKDEF int bMcuSpiSetSpeed(const bHalSPIIf_t *spi_if, bHalSPISpeed_t speed)
{
    return -1;
}

__WEAKDEF uint8_t bMcuSpiTransfer(const bHalSPIIf_t *spi_if, uint8_t dat)
{
    return 0;
}

__WEAKDEF int bMcuSpiSend(const bHalSPIIf_t *spi_if, const uint8_t *pbuf, uint16_t len)
{
    return -1;
}

__WEAKDEF int bMcuSpiReceive(const bHalSPIIf_t *spi_if, uint8_t *pbuf, uint16_t len)
{
    return -1;
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

int bHalSpiSetSpeed(const bHalSPIIf_t *spi_if, bHalSPISpeed_t speed)
{
    return bMcuSpiSetSpeed(spi_if, speed);
}

uint8_t bHalSpiTransfer(const bHalSPIIf_t *spi_if, uint8_t dat)
{
    uint8_t     tmp;
    bHalSPIIO_t simulating_spi;
    if (IS_NULL(spi_if))
    {
        return 0;
    }
    if (spi_if->is_simulation == 1)
    {
        simulating_spi.clk  = spi_if->_if.simulating_spi.clk;
        simulating_spi.mosi = spi_if->_if.simulating_spi.mosi;
        simulating_spi.miso = spi_if->_if.simulating_spi.miso;
        simulating_spi.CPHA = spi_if->_if.simulating_spi.CPHA;
        simulating_spi.CPOL = spi_if->_if.simulating_spi.CPOL;

        tmp = _bHalSPIIOWriteRead(simulating_spi, dat);
    }
    else
    {
        tmp = bMcuSpiTransfer(spi_if, dat);
    }
    return tmp;
}

int bHalSpiSend(const bHalSPIIf_t *spi_if, const uint8_t *pbuf, uint16_t len)
{
    int i      = 0;
    int retval = 0;
    if (IS_NULL(spi_if) || IS_NULL(pbuf))
    {
        return -1;
    }
    if (spi_if->is_simulation == 1)
    {
        for (i = 0; i < len; i++)
        {
            bHalSpiTransfer(spi_if, pbuf[i]);
        }
    }
    else
    {
        retval = bMcuSpiSend(spi_if, pbuf, len);
    }
    return retval;
}

int bHalSpiReceive(const bHalSPIIf_t *spi_if, uint8_t *pbuf, uint16_t len)
{
    int i      = 0;
    int retval = 0;
    if (IS_NULL(spi_if) || IS_NULL(pbuf))
    {
        return -1;
    }
    if (spi_if->is_simulation == 1)
    {
        for (i = 0; i < len; i++)
        {
            pbuf[i] = bHalSpiTransfer(spi_if, 0xff);
        }
    }
    else
    {
        retval = bMcuSpiReceive(spi_if, pbuf, len);
    }
    return retval;
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
