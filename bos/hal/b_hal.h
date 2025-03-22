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
#define FLASH_CS_RESET()            HAL_GPIO_WritePin(W25X_CS_GPIO_Port, W25X_CS_Pin, GPIO_PIN_RESET)
#define FLASH_CS_SET()              HAL_GPIO_WritePin(W25X_CS_GPIO_Port, W25X_CS_Pin, GPIO_PIN_SET)

#define S_TX_PIN_SET()              HAL_GPIO_WritePin(SUART_TX_GPIO_Port, SUART_TX_Pin, GPIO_PIN_SET)
#define S_TX_PIN_RESET()            HAL_GPIO_WritePin(SUART_TX_GPIO_Port, SUART_TX_Pin, GPIO_PIN_RESET)
#define S_RX_PIN_READ()             HAL_GPIO_ReadPin(SUART_RX_GPIO_Port, SUART_RX_Pin)

#define TP_CS_RESET()               HAL_GPIO_WritePin(TP_CS_GPIO_Port, TP_CS_Pin, GPIO_PIN_RESET)
#define TP_CS_SET()                 HAL_GPIO_WritePin(TP_CS_GPIO_Port, TP_CS_Pin, GPIO_PIN_SET)

/********************************************************************************************LCD*****/
#define LCD_CS_SET()                HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)
#define LCD_CS_RESET()              HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)

#define LCD_WR_SET()                HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_SET)
#define LCD_WR_RESET()              HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_RESET)

#define LCD_RD_SET()                HAL_GPIO_WritePin(LCD_RD_GPIO_Port, LCD_RD_Pin, GPIO_PIN_SET)
#define LCD_RD_RESET()              HAL_GPIO_WritePin(LCD_RD_GPIO_Port, LCD_RD_Pin, GPIO_PIN_RESET)

#define LCD_RS_SET()                HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET)
#define LCD_RS_RESET()              HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET)

#define LCD_DB_WRITE(n)             do{GPIOE->ODR = n;}while(0)
#define LCD_DB_READ()               ((uint16_t)GPIOE->IDR)

extern void LCD_DB_Init(unsigned char t);

#define LCD_DB_OUTPUT()             LCD_DB_Init(1)
#define LCD_DB_INPUT()              LCD_DB_Init(0)
/******************************************************************************************************/

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


