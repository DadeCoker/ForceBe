/**
 *!
 * \file        b_hal_gpio.h
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SGPIOL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
#ifndef __B_HAL_GPIO_H__
#define __B_HAL_GPIO_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h" 


/** 
 * \addtogroup B_HAL
 * \{
 */

/** 
 * \addtogroup GPIO
 * \{
 */

/** 
 * \defgroup GPIO_Exported_TypesDefinitions
 * \{
 */
  
/**
 * \}
 */
   
/** 
 * \defgroup GPIO_Exported_Defines
 * \{
 */
#define B_HAL_GPIO_INPUT        0
#define B_HAL_GPIO_OUTPUT       1

#define B_HAL_GPIO_NOPULL       0
#define B_HAL_GPIO_PULLUP       1
#define B_HAL_GPIO_PULLDOWN     2
/**
 * \}
 */
   
/** 
 * \defgroup GPIO_Exported_Macros
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup GPIO_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup GPIO_Exported_Functions
 * \{
 */

void bHalGPIO_Config(uint8_t port, uint8_t pin, uint8_t mode, uint8_t pull);
void bHalGPIO_WritePin(uint8_t port, uint8_t pin, uint8_t s);
uint8_t bHalGPIO_ReadPin(uint8_t port, uint8_t pin);
void bHalGPIO_Write(uint8_t port, uint16_t dat);
uint16_t bHalGPIO_Read(uint8_t port);

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


