/**
 *!
 * \file        b_hal_qspi.h
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
#ifndef __B_HAL_QSPI_H__
#define __B_HAL_QSPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

/**
 * \addtogroup B_HAL
 * \{
 */

/**
 * \addtogroup QSPI
 * \{
 */

/**
 * \defgroup QSPI_Exported_TypesDefinitions
 * \{
 */
typedef enum
{
    B_HAL_QSPI_1,
    B_HAL_QSPI_INVALID,
} bHalQSPINumber_t;

typedef struct
{
    uint32_t instruction;
    uint32_t address;
    uint32_t alternate;
    uint32_t dummy;
    uint32_t imode;
    uint32_t admode;
    uint32_t abmode;
    uint32_t dmode;
    uint32_t adsize;
    uint32_t absize;
    uint32_t dsize;
} bHalQSPICmdInfo_t;

/**
 * \}
 */

/**
 * \defgroup QSPI_Exported_Definitions
 * \{
 */

#define B_HAL_QSPI_MODE_NONE (0)
#define B_HAL_QSPI_MODE_1LINE (1)
#define B_HAL_QSPI_MODE_2LINES (2)
#define B_HAL_QSPI_MODE_4LINES (3)

#define B_HAL_QSPI_SIZE_8BIT (0)
#define B_HAL_QSPI_SIZE_16BIT (1)
#define B_HAL_QSPI_SIZE_24BIT (2)
#define B_HAL_QSPI_SIZE_32BIT (3)

/**
 * \}
 */

/**
 * \defgroup QSPI_Exported_Functions
 * \{
 */
int bMcuQSPISendCmd(const bHalQSPINumber_t qspi, const bHalQSPICmdInfo_t *pcmd);
int bMcuQSPIReceiveData(const bHalQSPINumber_t qspi, uint8_t *pbuf);
int bMcuQSPITransmitData(const bHalQSPINumber_t qspi, const uint8_t *pbuf);
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
int bHalQSPISendCmd(const bHalQSPINumber_t qspi, const bHalQSPICmdInfo_t *pcmd);
int bHalQSPIReceiveData(const bHalQSPINumber_t qspi, uint8_t *pbuf);
int bHalQSPITransmitData(const bHalQSPINumber_t qspi, const uint8_t *pbuf);
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
