/**
 *!
 * \file        b_hal_qspi.c
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
 * \addtogroup QSPI
 * \{
 */

/**
 * \addtogroup QSPI_Exported_Functions
 * \{
 */

__WEAKDEF int bMcuQSPISendCmd(const bHalQSPINumber_t qspi, const bHalQSPICmdInfo_t *pcmd)
{
    return 0;
}

__WEAKDEF int bMcuQSPIReceiveData(const bHalQSPINumber_t qspi, uint8_t *pbuf)
{
    return 0;
}

__WEAKDEF int bMcuQSPITransmitData(const bHalQSPINumber_t qspi, const uint8_t *pbuf)
{
    return 0;
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

int bHalQSPISendCmd(const bHalQSPINumber_t qspi, const bHalQSPICmdInfo_t *pcmd)
{
    if (qspi > B_HAL_QSPI_1 || IS_NULL(pcmd))
    {
        return -1;
    }
    return bMcuQSPISendCmd(qspi, pcmd);
}

int bHalQSPIReceiveData(const bHalQSPINumber_t qspi, uint8_t *pbuf)
{
    if (qspi > B_HAL_QSPI_1 || IS_NULL(pbuf))
    {
        return -1;
    }
    return bMcuQSPIReceiveData(qspi, pbuf);
}

int bHalQSPITransmitData(const bHalQSPINumber_t qspi, const uint8_t *pbuf)
{
    if (qspi > B_HAL_QSPI_1 || IS_NULL(pbuf))
    {
        return -1;
    }
    return bMcuQSPITransmitData(qspi, pbuf);
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
