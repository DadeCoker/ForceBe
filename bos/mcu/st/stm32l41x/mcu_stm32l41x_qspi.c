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

#include "stm32l4xx_hal.h"

#if defined(QUADSPI)

extern QSPI_HandleTypeDef hqspi;

int bMcuQSPISendCmd(const bHalQSPINumber_t qspi, const bHalQSPICmdInfo_t *pcmd)
{
    QSPI_CommandTypeDef cmd;
    cmd.Address = pcmd->address;
    cmd.AddressMode = pcmd->admode << 10;
    cmd.AddressSize = pcmd->adsize << 12;
    cmd.AlternateByteMode = pcmd->abmode << 14;
    cmd.AlternateBytes = pcmd->alternate;
    cmd.AlternateBytesSize = pcmd->absize << 16;
    cmd.DataMode = pcmd->dmode << 24;
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.DummyCycles = pcmd->dummy;
    cmd.Instruction = pcmd->instruction;
    cmd.InstructionMode = pcmd->imode << 8;
    cmd.NbData = pcmd->dsize;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    if(HAL_OK == HAL_QSPI_Command(&hqspi, &cmd, 0xFFFFFFFF))
    {
        return 0;
    }
    return -1;
}

int bMcuQSPIReceiveData(const bHalQSPINumber_t qspi, uint8_t *pbuf)
{
    if(HAL_OK == HAL_QSPI_Receive(&hqspi, pbuf, 0xFFFFFFFF))
    {
        return 0;
    }
    return -1;
}

int bMcuQSPITransmitData(const bHalQSPINumber_t qspi, const uint8_t *pbuf)
{
    if(HAL_OK == HAL_QSPI_Transmit(&hqspi, (uint8_t *)pbuf, 0xFFFFFFFF))
    {
        return 0;
    }
    return -1;
}




#endif

#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
