/**
 *!
 * \file        b_drv_fm25cl.h
 * \version     v0.0.1
 * \date        2020/02/05
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
#ifndef __B_DRV_FM25CL_H__
#define __B_DRV_FM25CL_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "drivers/inc/b_driver.h"

/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup FM25CL
 * \{
 */

/**
 * \defgroup FM25CL_Exported_TypesDefinitions
 * \{
 */
//<HALIF 1 SPI
typedef bHalSPIIf_t bFM25CL_HalIf_t;

typedef bDriverInterface_t bFM25CL_Driver_t;

/**
 * \}
 */

/**
 * \defgroup FM25CL_Exported_Defines
 * \{
 */

#define SFC_WREN 0x06   // Set write enable latch
#define SFC_WRDI 0x04   // Write disable
#define SFC_RDSR 0x05   // Read Status Register
#define SFC_WRSR 0x01   // Write Status Register
#define SFC_READ 0x03   // Read memory data
#define SFC_WRITE 0x02  // Write memory data

/**
 * \}
 */

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

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
