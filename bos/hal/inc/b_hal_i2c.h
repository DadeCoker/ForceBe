/**
 *!
 * \file        b_hal_i2c.h
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SI2CL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
#ifndef __B_HAL_I2C_H__
#define __B_HAL_I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

#include "hal/inc/b_hal_gpio.h"

/**
 * \addtogroup B_HAL
 * \{
 */

/**
 * \addtogroup I2C
 * \{
 */

/**
 * \defgroup I2C_Exported_TypesDefinitions
 * \{
 */
typedef enum
{
    B_HAL_I2C_1,
    B_HAL_I2C_2,
    B_HAL_I2C_3,
    B_HAL_I2C_4,
    B_HAL_I2C_NUMBER,
} bHalI2CNumber_t;

typedef struct
{
    uint8_t dev_addr;
    uint8_t is_simulation;
    union
    {
        bHalI2CNumber_t i2c;
        struct
        {
            bHalGPIOInstance_t clk;
            bHalGPIOInstance_t sda;
        } simulating_i2c;
    } _if;
} bHalI2CIf_t;

/**
 * \}
 */

/**
 * \defgroup I2C_Exported_Functions
 * \{
 */
uint8_t bMcuI2CReadByte(const bHalI2CIf_t *i2c_if);
int     bMcuI2CWriteByte(const bHalI2CIf_t *i2c_if, uint8_t dat);
int     bMcuI2CMemWrite(const bHalI2CIf_t *i2c_if, uint16_t mem_addr, const uint8_t *pbuf,
                        uint16_t len);
int     bMcuI2CMemRead(const bHalI2CIf_t *i2c_if, uint16_t mem_addr, uint8_t *pbuf, uint16_t len);
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
uint8_t bHalI2CReadByte(const bHalI2CIf_t *i2c_if);
int     bHalI2CWriteByte(const bHalI2CIf_t *i2c_if, uint8_t dat);
int     bHalI2CMemWrite(const bHalI2CIf_t *i2c_if, uint16_t mem_addr, const uint8_t *pbuf,
                        uint16_t len);
int     bHalI2CMemRead(const bHalI2CIf_t *i2c_if, uint16_t mem_addr, uint8_t *pbuf, uint16_t len);

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
