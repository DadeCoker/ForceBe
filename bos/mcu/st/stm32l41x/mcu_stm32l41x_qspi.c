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

#if (defined(STM32L41X_L46X))

typedef struct
{
    volatile uint32_t CR;    /*!< QUADSPI Control register*/
    volatile uint32_t DCR;   /*!< QUADSPI Device Configuration register */
    volatile uint32_t SR;    /*!< QUADSPI Status register*/
    volatile uint32_t FCR;   /*!< QUADSPI Flag Clear register */
    volatile uint32_t DLR;   /*!< QUADSPI Data Length register */
    volatile uint32_t CCR;   /*!< QUADSPI Communication Configuration register */
    volatile uint32_t AR;    /*!< QUADSPI Address register */
    volatile uint32_t ABR;   /*!< QUADSPI Alternate Bytes register */
    volatile uint32_t DR;    /*!< QUADSPI Data register */
    volatile uint32_t PSMKR; /*!< QUADSPI Polling Status Mask register */
    volatile uint32_t PSMAR; /*!< QUADSPI Polling Status Match register*/
    volatile uint32_t PIR;   /*!< QUADSPI Polling Interval register*/
    volatile uint32_t LPTR;  /*!< QUADSPI Low Power Timeout register */
} McuQspiReg_t;

#define QSPI_BASE_ADDR (0xA0001000UL)

#define MCU_QSPI1 ((McuQspiReg_t *)QSPI_BASE_ADDR)

static McuQspiReg_t *QSpiTable[1] = {MCU_QSPI1};

#define BUSY_BIT (0x1 << 5)
#define CTCF_BIT (0x1 << 1)

#define FTF_BIT (0x1 << 2)
#define TCF_BIT (0x1 << 1)

#define FMODE_BIT (0x3 << 26)
#define DMODE_BIT (0x3 << 24)
#define DUMMY_BIT (0x1f << 18)
#define ABSIZE_BIT (0x3 << 16)
#define ABMODE_BIT (0x3 << 14)
#define ADSIZE_BIT (0x3 << 12)
#define ADMODE_BIT (0x3 << 10)
#define IMODE_BIT (0x3 << 8)
#define INSTRUCTION_BIT (0xff << 0)

#define FMODE_WRITE_BIT (0x0 << 26)
#define FMODE_READ_BIT (0x1 << 26)

#define WAIT_TIMEOUT (0xFFFFF)

int bMcuQSPISendCmd(const bHalQSPINumber_t qspi, const bHalQSPICmdInfo_t *pcmd)
{
    McuQspiReg_t *qspi_r   = QSpiTable[qspi];
    uint32_t      ccr_data = 0;
    uint32_t      timeout  = 0;
    if (qspi != B_HAL_QSPI_1)
    {
        return -1;
    }
    //  busy ?
    timeout = 0;
    while (B_READ_BIT(qspi_r->SR, BUSY_BIT))
    {
        timeout += 1;
        if (timeout > WAIT_TIMEOUT)
        {
            return -2;
        }
    }
    // config qspi
    if (pcmd->dmode != B_HAL_QSPI_MODE_NONE)
    {
        B_WRITE_REG(qspi_r->DLR, pcmd->dsize - 1);
    }

    ccr_data = B_READ_REG(qspi_r->CCR);

    B_CLEAR_BIT(ccr_data, ABMODE_BIT);
    B_CLEAR_BIT(ccr_data, ABSIZE_BIT);
    B_CLEAR_BIT(ccr_data, ADMODE_BIT);
    B_CLEAR_BIT(ccr_data, ADSIZE_BIT);
    B_CLEAR_BIT(ccr_data, IMODE_BIT);
    B_CLEAR_BIT(ccr_data, INSTRUCTION_BIT);
    B_CLEAR_BIT(ccr_data, DUMMY_BIT);
    B_CLEAR_BIT(ccr_data, FMODE_BIT);
    B_CLEAR_BIT(ccr_data, DMODE_BIT);

    if (pcmd->dmode != B_HAL_QSPI_MODE_NONE)
    {
        B_SET_BIT(ccr_data, (pcmd->dmode & 0x3) << 24);
    }

    if (pcmd->abmode != B_HAL_QSPI_MODE_NONE)
    {
        B_WRITE_REG(qspi_r->ABR, pcmd->alternate);
        B_SET_BIT(ccr_data, (pcmd->abmode & 0x3) << 14);
        B_SET_BIT(ccr_data, (pcmd->absize & 0x3) << 16);
    }

    if (pcmd->admode != B_HAL_QSPI_MODE_NONE)
    {
        B_SET_BIT(ccr_data, (pcmd->admode & 0x3) << 10);
        B_SET_BIT(ccr_data, (pcmd->adsize & 0x3) << 12);
    }

    if (pcmd->imode != B_HAL_QSPI_MODE_NONE)
    {
        B_SET_BIT(ccr_data, (pcmd->imode & 0x3) << 8);
        B_SET_BIT(ccr_data, pcmd->instruction & 0xff);
    }
    B_SET_BIT(ccr_data, (pcmd->dummy & 0x1f) << 18);
    B_SET_BIT(ccr_data, FMODE_WRITE_BIT);

    B_WRITE_REG(qspi_r->CCR, ccr_data);

    if (pcmd->admode != B_HAL_QSPI_MODE_NONE)
    {
        B_WRITE_REG(qspi_r->AR, pcmd->address);
    }

    timeout = 0;
    if (pcmd->dmode == B_HAL_QSPI_MODE_NONE)
    {
        while (B_READ_BIT(qspi_r->SR, TCF_BIT) == 0)
        {
            timeout += 1;
            if (timeout > WAIT_TIMEOUT)
            {
                return -3;
            }
        }
        B_CLEAR_BIT(qspi_r->FCR, CTCF_BIT);
    }
    return 0;
}

int bMcuQSPIReceiveData(const bHalQSPINumber_t qspi, uint8_t *pbuf)
{
    McuQspiReg_t *qspi_r  = QSpiTable[qspi];
    uint32_t      timeout = 0;
    if (qspi != B_HAL_QSPI_1)
    {
        return -1;
    }

    uint32_t           addr_reg = B_READ_REG(qspi_r->AR);
    volatile uint32_t *data_reg = &qspi_r->DR;

    uint32_t len = 0;

    len = B_READ_REG(qspi_r->DLR) + 1;

    B_MODIFY_REG(qspi_r->CCR, FMODE_BIT, FMODE_READ_BIT);
    B_WRITE_REG(qspi_r->AR, addr_reg);

    while (len > 0)
    {
        timeout = 0;
        while (B_READ_BIT(qspi_r->SR, FTF_BIT | TCF_BIT) == 0)
        {
            timeout += 1;
            if (timeout > WAIT_TIMEOUT)
            {
                return -3;
            }
        }
        *pbuf = *((volatile uint8_t *)data_reg);
        pbuf++;
        len--;
    }

    timeout = 0;
    while (B_READ_BIT(qspi_r->SR, TCF_BIT) == 0)
    {
        timeout += 1;
        if (timeout > WAIT_TIMEOUT)
        {
            break;
        }
    }
    B_CLEAR_BIT(qspi_r->FCR, CTCF_BIT);
    return 0;
}

int bMcuQSPITransmitData(const bHalQSPINumber_t qspi, const uint8_t *pbuf)
{
    McuQspiReg_t *qspi_r  = QSpiTable[qspi];
    uint32_t      timeout = 0;
    if (qspi != B_HAL_QSPI_1)
    {
        return -1;
    }

    uint32_t           addr_reg = B_READ_REG(qspi_r->AR);
    volatile uint32_t *data_reg = &qspi_r->DR;

    uint32_t len = 0;

    len = B_READ_REG(qspi_r->DLR) + 1;

    B_MODIFY_REG(qspi_r->CCR, FMODE_BIT, FMODE_WRITE_BIT);
    B_WRITE_REG(qspi_r->AR, addr_reg);

    while (len > 0)
    {
        timeout = 0;
        while (B_READ_BIT(qspi_r->SR, FTF_BIT) == 0)
        {
            timeout += 1;
            if (timeout > WAIT_TIMEOUT)
            {
                return -3;
            }
        }
        *((volatile uint8_t *)data_reg) = *pbuf;
        pbuf++;
        len--;
    }

    timeout = 0;
    while (B_READ_BIT(qspi_r->SR, TCF_BIT) == 0)
    {
        timeout += 1;
        if (timeout > WAIT_TIMEOUT)
        {
            break;
        }
    }
    B_CLEAR_BIT(qspi_r->FCR, CTCF_BIT);
    return 0;
}

#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
