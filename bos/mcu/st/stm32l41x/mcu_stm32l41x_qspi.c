/**
 *!
 * \file        mcu_stm32l4xx_qspi.c
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
#include "hal/inc/b_hal_qspi.h"

#if (MCU_PLATFORM == 1201)

//         Register Address

#define QSPI_BASE_ADDR (0xA0001000)

typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t DCR;
    volatile uint32_t SR;
    volatile uint32_t FCR;
    volatile uint32_t DLR;
    volatile uint32_t CCR;
    volatile uint32_t AR;
    volatile uint32_t ABR;
    volatile uint32_t DR;
    volatile uint32_t PSMKR;
    volatile uint32_t PSMAR;
    volatile uint32_t PIR;
    volatile uint32_t LPTR;
} McuQSPIReg_t;

#define MCU_QSPI ((McuQSPIReg_t *)QSPI_BASE_ADDR)

#define QSPI_WAIT_TIMEOUT (0x00B00000)
int bMcuQSPISendCmd(const bHalQSPINumber_t qspi, const bHalQSPICmdInfo_t *pcmd)
{
    McuQSPIReg_t *pQspi   = MCU_QSPI;
    uint32_t      reg     = 0;
    uint32_t      timeout = 0;
    while (B_READ_BIT(pQspi->SR, (0x0001 << 5)) != 0)
    {
        timeout++;
        if (timeout >= QSPI_WAIT_TIMEOUT)
        {
            return -1;
        }
    }
    reg = (pcmd->dmode << 24) | (pcmd->dummy << 18) | (pcmd->admode << 10) | (pcmd->imode << 8) |
          (pcmd->abmode << 14);
    if (pcmd->dmode != B_HAL_QSPI_MODE_NONE)
    {
        B_WRITE_REG(pQspi->DLR, pcmd->dsize - 1);
    }
    if (pcmd->imode != B_HAL_QSPI_MODE_NONE)
    {
        reg |= pcmd->instruction;
    }
    if (pcmd->abmode != B_HAL_QSPI_MODE_NONE)
    {
        reg |= (pcmd->absize << 16);
        B_WRITE_REG(pQspi->ABR, pcmd->alternate);
    }
    if (pcmd->admode != B_HAL_QSPI_MODE_NONE)
    {
        reg |= (pcmd->adsize << 12);
    }
    B_WRITE_REG(pQspi->CCR, reg);
    if (pcmd->admode != B_HAL_QSPI_MODE_NONE)
    {
        B_WRITE_REG(pQspi->AR, pcmd->address);
    }
    if (pcmd->dmode == B_HAL_QSPI_MODE_NONE)
    {
        B_WRITE_REG(pQspi->FCR, (0x00000002));
    }
    for (timeout = 0; timeout < 0xff; timeout++)
    {
        ;
    }    
    return 0;
}

int bMcuQSPIReceiveData(const bHalQSPINumber_t qspi, uint8_t *pbuf)
{
    McuQSPIReg_t *pQspi   = MCU_QSPI;
    uint32_t      timeout = 0;
    uint32_t      count = 0, size = 0;

    uint32_t           addr_reg = B_READ_REG(pQspi->AR);
    volatile uint32_t *data_reg = &pQspi->DR;

    size = B_READ_REG(pQspi->DLR) + 1U;

    B_MODIFY_REG(pQspi->CCR, 0x0C000000, 0x04000000);
    B_WRITE_REG(pQspi->AR, addr_reg);

    while (count < size)
    {
        timeout = 0;
        while (B_READ_BIT(pQspi->SR, (0x00000006)) == 0)
        {
            timeout++;
            if (timeout >= QSPI_WAIT_TIMEOUT)
            {
                return -1;
            }
        }
        pbuf[count] = *((volatile uint8_t *)data_reg);
        count++;
    }
    timeout = 0;
    while (B_READ_BIT(pQspi->SR, (0x00000002)) == 0)
    {
        timeout++;
        if (timeout >= QSPI_WAIT_TIMEOUT)
        {
            break;
        }
    }
    B_WRITE_REG(pQspi->FCR, 0x00000002);
    for (timeout = 0; timeout < 0xff; timeout++)
    {
        ;
    }
    return size;
}

int bMcuQSPITransmitData(const bHalQSPINumber_t qspi, const uint8_t *pbuf)
{
    McuQSPIReg_t *pQspi   = MCU_QSPI;
    uint32_t      timeout = 0;
    uint32_t      count = 0, size = 0;

    volatile uint32_t *data_reg = &pQspi->DR;
    size                        = B_READ_REG(pQspi->DLR) + 1U;

    B_MODIFY_REG(pQspi->CCR, 0x0C000000, 0x00000000);
    while (count < size)
    {
        timeout = 0;
        while (B_READ_BIT(pQspi->SR, (0x00000004)) == 0)
        {
            timeout++;
            if (timeout >= QSPI_WAIT_TIMEOUT)
            {
                return -1;
            }
        }
        *((volatile uint8_t *)data_reg) = pbuf[count];
        count++;
    }
    timeout = 0;
    while (B_READ_BIT(pQspi->SR, (0x00000002)) == 0)
    {
        timeout++;
        if (timeout >= QSPI_WAIT_TIMEOUT)
        {
            break;
        }
    }
    B_WRITE_REG(pQspi->FCR, (0x00000002));
    for (timeout = 0; timeout < 0xff; timeout++)
    {
        ;
    }
    return size;
}

#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
