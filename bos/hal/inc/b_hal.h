/**
 *!
 * \file        b_hal.h
 * \version     v0.0.1
 * \date        2019/06/05
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2019 Bean
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
#ifndef __B_HAL_H__
#define __B_HAL_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"
#include "b_hal_flash.h"
#include "b_hal_gpio.h"
#include "b_hal_i2c.h"
#include "b_hal_if.h"
#include "b_hal_it.h"
#include "b_hal_spi.h"
#include "b_hal_qspi.h"
#include "b_hal_uart.h"

/**
 * \addtogroup B_HAL
 * \{
 */

/**
 * \addtogroup HAL
 * \{
 */

/**
 * \defgroup HAL_Exported_TypesDefinitions
 * \{
 */

typedef struct
{
    union
    {
        uint32_t rw_addr;
        struct
        {
            bHalGPIOInstance_t data;
            bHalGPIOInstance_t rs;
            bHalGPIOInstance_t rd;
            bHalGPIOInstance_t wr;
            bHalGPIOInstance_t cs;
        } _io;
        struct
        {
            bHalGPIOInstance_t rs;
            bHalSPIIf_t        _spi;
        } _spi;
    } _if;
    uint8_t if_type;  // 0: _io  1: rw_addr  2: _spi
} bLCD_HalIf_t;

#define LCD_IF_TYPE_IO (0)
#define LCD_IF_TYPE_RWADDR (1)
#define LCD_IF_TYPE_SPI (2)

/**
 * \}
 */

/**
 * \defgroup HAL_Exported_Defines
 * \{
 */
#define MS2TICKS(m) (m / (1000 / TICK_FRQ_HZ))

#if _HALIF_VARIABLE_ENABLE
#define HALIF_KEYWORD static
#else
#define HALIF_KEYWORD const static
#endif

/**
 * \}
 */

/**
 * \defgroup HAL_Exported_Functions
 * \{
 */
void     bHalIncSysTick(void);
void     bHalInit(void);
void     bHalDelayMs(uint16_t xms);
void     bHalDelayUs(uint32_t xus);
uint32_t bHalGetSysTick(void);

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
