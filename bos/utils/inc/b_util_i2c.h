/**
 *!
 * \file        b_util_i2c.h
 * \version     v0.0.1
 * \date        2020/04/01
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
#ifndef __B_UTIL_I2C_H__
#define __B_UTIL_I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_hal.h"

/**
 * \addtogroup B_UTILS
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
typedef struct
{
    bHalGPIOInstance_t sda;
    bHalGPIOInstance_t clk;
} bUtilI2C_t;
/**
 * \}
 */

/**
 * \defgroup I2C_Exported_Functions
 * \{
 */

void bUtilI2C_Start(bUtilI2C_t i2c);
void bUtilI2C_Stop(bUtilI2C_t i2c);
int  bUtilI2C_ACK(bUtilI2C_t i2c);
void bUtilI2C_mACK(bUtilI2C_t i2c);

void    bUtilI2C_WriteByte(bUtilI2C_t i2c, uint8_t dat);
uint8_t bUtilI2C_ReadByte(bUtilI2C_t i2c);

int     bUtilI2C_WriteData(bUtilI2C_t i2c, uint8_t dev, uint8_t addr, uint8_t dat);
uint8_t bUtilI2C_ReadData(bUtilI2C_t i2c, uint8_t dev, uint8_t addr);

int bUtilI2C_ReadBuff(bUtilI2C_t i2c, uint8_t dev, uint8_t addr, uint8_t *pdat, uint8_t len);
int bUtilI2C_WriteBuff(bUtilI2C_t i2c, uint8_t dev, uint8_t addr, uint8_t *pdat, uint8_t len);

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
