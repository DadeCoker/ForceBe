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
typedef enum
{
    B_HAL_UART_1,
}bHalUartNumber_t;  

typedef enum
{
    B_HAL_GPIOA,
    B_HAL_GPIOB,
    B_HAL_GPIOC,
    B_HAL_GPIOD,
    B_HAL_GPIOE,
    B_HAL_GPIOF,
    B_HAL_GPIOG,
}bHalGPIOPort_t;

typedef enum
{
    B_HAL_PIN1,
    B_HAL_PIN2,
    B_HAL_PIN3,
    B_HAL_PIN4,
    B_HAL_PIN5,
    B_HAL_PIN6,
    B_HAL_PIN7,
    B_HAL_PIN8,
    B_HAL_PIN9,
    B_HAL_PIN10,
    B_HAL_PIN11,
    B_HAL_PIN12,
    B_HAL_PIN13,
    B_HAL_PIN14,
    B_HAL_PIN15,
    B_HAL_PIN16,
    B_HAL_PINAll,     
}bHalGPIOPin_t;
/**
 * \}
 */
   
/** 
 * \defgroup HAL_Exported_Defines
 * \{
 */

#define HAL_LOG_UART_PORT       B_HAL_UART_1


#ifdef LCD_FSMC
#define HAL_LCD_FSMC_ADDR       ((uint32_t)(0x60000000 | 0x0007FFFE))
#else
#define HAL_LCD_RS_PORT         B_HAL_GPIOD                 
#define HAL_LCD_RS_PIN          B_HAL_PIN13
#define HAL_LCD_WR_PORT         B_HAL_GPIOB
#define HAL_LCD_WR_PIN          B_HAL_PIN14
#define HAL_LCD_RD_PORT         B_HAL_GPIOD
#define HAL_LCD_RD_PIN          B_HAL_PIN15
#define HAL_LCD_CS_PORT         B_HAL_GPIOC
#define HAL_LCD_CS_PIN          B_HAL_PIN8
#define HAL_LCD_DAT_PORT        B_HAL_GPIOE 
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
void bHalIncSysTick(void);
void bHalInit(void);
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


