/**
 *!
 * \file        algo_crc.h
 * \version     v0.0.1
 * \date        2020/04/27
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
#ifndef __B_ALGO_CRC_H__
#define __B_ALGO_CRC_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

#include "b_config.h"

/**
 * \addtogroup ALGORITHM
 * \{
 */

/**
 * \addtogroup CRC
 * \{
 */

/**
 * \defgroup CRC_Exported_Defines
 * \{
 */

#define ALGO_CRC8 (0)               // Initial value 0x00
#define ALGO_CRC8_ITU (1)           // Initial value 0x00
#define ALGO_CRC8_ROHC (2)          // Initial value 0xff
#define ALGO_CRC8_MAXIM (3)         // Initial value 0x00
#define ALGO_CRC16_IBM (4)          // Initial value 0x00
#define ALGO_CRC16_MAXIM (5)        // Initial value 0x00
#define ALGO_CRC16_USB (6)          // Initial value 0xffff
#define ALGO_CRC16_MODBUS (7)       // Initial value 0xffff
#define ALGO_CRC16_CCITT (8)        // Initial value 0x00
#define ALGO_CRC16_CCITT_FALSE (9)  // Initial value 0xffff
#define ALGO_CRC16_X25 (10)         // Initial value 0xffff
#define ALGO_CRC16_XMODEM (11)      // Initial value 0x00
#define ALGO_CRC16_DNP (12)         // Initial value 0x00
#define ALGO_CRC32 (13)             // Initial value 0xffffffff
#define ALGO_CRC32_MPEG2 (14)       // Initial value 0xffffffff

#define INITIAL_VALUE_IS_FF(t)                                                            \
    (((t) == ALGO_CRC8_ROHC) || ((t) == ALGO_CRC16_USB) || ((t) == ALGO_CRC16_MODBUS) ||  \
     ((t) == ALGO_CRC16_CCITT_FALSE) || ((t) == ALGO_CRC16_X25) || ((t) == ALGO_CRC32) || \
     ((t) == ALGO_CRC32_MPEG2))

/**
 * \}
 */
#if _ALGO_CRC_ENABLE
/**
 * \defgroup CRC_Exported_Functions
 * \{
 */

uint32_t crc_calculate(uint8_t type, uint32_t crc_val, uint8_t *pbuf, uint32_t len);

/**
 * \}
 */
#endif
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
