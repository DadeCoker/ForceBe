/**
 *!
 * \file        mcu_n32g45x_spi.c
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

#if (MCU_PLATFORM == 2101)
//         Register Address

#define SPI1_BASE_ADDR (0x40013000)
#define SPI2_BASE_ADDR (0x40003800)
#define SPI3_BASE_ADDR (0x40003C00)

typedef struct
{
    volatile uint16_t CR1;
    uint16_t          RESERVED0;
    volatile uint16_t CR2;
    uint16_t          RESERVED1;
    volatile uint16_t SR;
    uint16_t          RESERVED2;
    volatile uint16_t DR;
    uint16_t          RESERVED3;
    volatile uint16_t CRCPR;
    uint16_t          RESERVED4;
    volatile uint16_t RXCRCR;
    uint16_t          RESERVED5;
    volatile uint16_t TXCRCR;
    uint16_t          RESERVED6;
    volatile uint16_t I2SCFGR;
    uint16_t          RESERVED7;
    volatile uint16_t I2SPR;
    uint16_t          RESERVED8;
} McuSpiReg_t;

#define MCU_SPI1 ((McuSpiReg_t *)SPI1_BASE_ADDR)
#define MCU_SPI2 ((McuSpiReg_t *)SPI2_BASE_ADDR)
#define MCU_SPI3 ((McuSpiReg_t *)SPI3_BASE_ADDR)

static McuSpiReg_t *SpiTable[3] = {MCU_SPI1, MCU_SPI2, MCU_SPI3};

int bMcuSpiSetSpeed(const bHalSPIIf_t *spi_if, bHalSPISpeed_t speed)
{
    uint16_t     SpeedVal = 1;
    McuSpiReg_t *pSpi     = NULL;
    if (IS_NULL(spi_if) || (speed >= B_HAL_SPI_SPEED_INVALID))
    {
        return -1;
    }
    if (speed == B_HAL_SPI_SLOW)
    {
        SpeedVal = 6;
    }
    if (spi_if->_if.spi > B_HAL_SPI_3)
    {
        return -1;
    }
    pSpi = SpiTable[spi_if->_if.spi];
    while (pSpi->SR & 0x80)
    {
        ;
    }
    pSpi->CR1 &= ~(0x7 << 3);
    pSpi->CR1 |= (SpeedVal << 3);
    return 0;
}

uint8_t bMcuSpiTransfer(const bHalSPIIf_t *spi_if, uint8_t dat)
{
    int          i = 0;
    uint8_t      tmp;
    McuSpiReg_t *pSpi = NULL;
    if (IS_NULL(spi_if))
    {
        return 0;
    }
    if (spi_if->_if.spi > B_HAL_SPI_3)
    {
        return 0;
    }
    pSpi = SpiTable[spi_if->_if.spi];
    if ((pSpi->CR1 & (0x1 << 6)) == 0)
    {
        pSpi->CR1 |= (0x1 << 6);
    }
    while ((pSpi->SR & 0x02) == 0)
    {
        ;
    }
    pSpi->DR = dat;
    while ((pSpi->SR & 0x01) == 0)
    {
        ;
    }
    tmp = pSpi->DR;
    for (i = 0; i < 8; i++)
    {
        ;
    }
    return tmp;
}

int bMcuSpiSend(const bHalSPIIf_t *spi_if, const uint8_t *pbuf, uint16_t len)
{
    int i = 0;
    if (IS_NULL(spi_if) || IS_NULL(pbuf))
    {
        return -1;
    }
    for (i = 0; i < len; i++)
    {
        bMcuSpiTransfer(spi_if, pbuf[i]);
    }
    return 0;
}

int bMcuSpiReceive(const bHalSPIIf_t *spi_if, uint8_t *pbuf, uint16_t len)
{
    int i = 0;
    if (IS_NULL(spi_if) || IS_NULL(pbuf))
    {
        return -1;
    }
    for (i = 0; i < len; i++)
    {
        pbuf[i] = bMcuSpiTransfer(spi_if, 0xff);
    }
    return 0;
}

#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
