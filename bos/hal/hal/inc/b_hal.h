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
#include "stm32f1xx_hal.h"

#include "b_hal_uart.h"
#include "b_hal_lcd.h"
#include "b_hal_gpio.h"
#include "b_hal_spi.h"
#include "b_hal_i2c.h"
#include "b_hal_sccb.h"
#include "b_hal_qspi.h"
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

/**
 * \}
 */
   
/** 
 * \defgroup HAL_Exported_Defines
 * \{
 */

#define HAL_LOG_UART                    B_HAL_UART_1

/**                           Flash IC                                 */
///< W25X
#define HAL_W25X_IF                     {{0, .xspi.spi = B_HAL_SPI_2, {B_HAL_GPIOB, B_HAL_PIN12}},}
///<24cxx
#define HAL_24CXX_IF                    {{B_HAL_I2C_2, 0xa0},}




/**                           b_mod_button                           */
///<b_mod_button {port, pin, pressed_logic_level}
#if _FLEXIBLEBUTTON_ENABLE
#define HAL_B_BUTTON_GPIO               {{B_HAL_GPIOA, B_HAL_PIN3, 0},\
                                         {B_HAL_GPIOA, B_HAL_PIN2, 0},\
                                         {B_HAL_GPIOC, B_HAL_PIN13, 0},}
#endif

/**
 * \}
 */
   
/** 
 * \defgroup HAL_Exported_Macros
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup HAL_Exported_Variables
 * \{
 */
extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c2;
extern SPI_HandleTypeDef hspi2;
/**
 * \}
 */
   
/** 
 * \defgroup HAL_Exported_Functions
 * \{
 */
void bHalEnterCritical(void); 
void bHalExitCritical(void);
void bHalInit(void);


///<Depend on the platform
void bHalIncSysTick(void);

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


