/**
 *!
 * \file        b_hal_flash.h
 * \version     v0.0.1
 * \date        2021/06/13
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 Bean
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SGPIOL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
#ifndef __B_HAL_FLASH_H__
#define __B_HAL_FLASH_H__

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
 * \addtogroup FLASH
 * \{
 */

/**
 * \defgroup FLASH_Exported_Functions
 * \{
 */

int bMcuFlashInit(void);
int bMcuFlashUnlock(void);
int bMcuFlashLock(void);
int bMcuFlashErase(uint32_t raddr, uint8_t pages);
int bMcuFlashWrite(uint32_t raddr, const uint8_t *pbuf, uint16_t len);
int bMcuFlashRead(uint32_t raddr, uint8_t *pbuf, uint16_t len);
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
int bHalFlashInit(void);
int bHalFlashUnlock(void);
int bHalFlashLock(void);
int bHalFlashErase(uint32_t raddr, uint8_t pages);
int bHalFlashWrite(uint32_t raddr, const uint8_t *pbuf, uint16_t len);
int bHalFlashRead(uint32_t raddr, uint8_t *pbuf, uint16_t len);

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

/************************ Copyright (c) 2021 Bean *****END OF FILE****/
