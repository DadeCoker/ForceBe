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
//#include "stm32f1xx_hal.h"   //add the platform h file

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
//
//                               Flash  
//
///< SPIFLASH
#define HAL_SPIFLASH_QSPI_EN            0
#define HAL_SPIFLASH_TOTAL_NUMBER       1 
#define HAL_SPIFLASH_IF                 {{B_HAL_QSPI_INVALID, B_HAL_SPI_2, {B_HAL_GPIOB, B_HAL_PIN12}},}    ////{{qspi, spi, {cs_port, cs_pin}},} 

///< FM25CL
#define HAL_FM25CL_IF                   {{B_HAL_SPI_1, {B_HAL_GPIOB, B_HAL_PIN9}},}

///< 24cxx
#define HAL_24CXX_IF                    {{B_HAL_I2C_1, 0xa0},}

///< SD
#define HAL_SD_SPI                      B_HAL_SPI_2
#define HAL_SD_CS_PORT                  B_HAL_GPIOD
#define HAL_SD_CS_PIN                   B_HAL_PIN2

//
//                               LCD   
//
#if 0
#define HAL_LCD_FSMC_ADDR               ((uint32_t)(0x60000000 | 0x0007FFFE))
#else
#define HAL_LCD_RS_PORT                 B_HAL_GPIOD                 
#define HAL_LCD_RS_PIN                  B_HAL_PIN13
#define HAL_LCD_WR_PORT                 B_HAL_GPIOB
#define HAL_LCD_WR_PIN                  B_HAL_PIN14
#define HAL_LCD_RD_PORT                 B_HAL_GPIOD
#define HAL_LCD_RD_PIN                  B_HAL_PIN15
#define HAL_LCD_CS_PORT                 B_HAL_GPIOC
#define HAL_LCD_CS_PIN                  B_HAL_PIN8
#define HAL_LCD_DAT_PORT                B_HAL_GPIOE 
#endif

///< OLED
#define HAL_OLED_I2C                    B_HAL_I2C_1
#define HAL_OLED_I2C_ADDR               0X78


//
//                               TOUCH   
//
///< XPT2046
#define HAL_XPT2046_SPI                 B_HAL_SPI_3
#define HAL_XPT2046_CS_PORT             B_HAL_GPIOC             
#define HAL_XPT2046_CS_PIN              B_HAL_PIN9



//
//                               CAMERA   
//
///<OV5640
#define HAL_OV5640_SCCB                 B_HAL_SCCB_1
#define HAL_OV5640_SCCB_ADDR            0X78
#define HAL_OV5640_RESET_PORT           B_HAL_GPIOA
#define HAL_OV5640_RESET_PIN            B_HAL_PIN15


//
//                               IO   
//
///<PCF8574
#define HAL_PCF8574_I2C                 B_HAL_I2C_2
#define HAL_PCF8574_I2C_ADDR            0X40


//
//                               G-Sensor  
//
//ST-LIS3DH
#define HAL_LIS3DH_I2C_ENABLE           0
#if HAL_LIS3DH_I2C_ENABLE
#define HAL_LIS3DH_I2C                  B_HAL_I2C_1
#define HAL_LIS3DH_I2C_ADDR             0X30
#else
#define HAL_LIS3DH_SPI                  B_HAL_SPI_1
#define HAL_LIS3DH_CS_PORT              B_HAL_GPIOB
#define HAL_LIS3DH_CS_PIN               B_HAL_PIN0
#endif
#define HAL_LIS3DH_INT_PIN              B_HAL_PIN4


//
//                               BUTTON   
//
///< b_mod_button {port, pin, pressed_logic_level}
#if _FLEXIBLEBUTTON_ENABLE
#define HAL_B_BUTTON_GPIO               {{B_HAL_GPIOA, B_HAL_PIN0, 0},}
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


