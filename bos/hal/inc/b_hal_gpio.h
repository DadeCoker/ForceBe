/**
 *!
 * \file        b_hal_gpio.h
 * \version     v0.0.2
 * \date        2021/07/17
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
typedef enum
{
    B_HAL_GPIOA,
    B_HAL_GPIOB,
    B_HAL_GPIOC,
    B_HAL_GPIOD,
    B_HAL_GPIOE,
    B_HAL_GPIOF,
    B_HAL_GPIOG,
    B_HAL_GPIO_INVALID,
} bHalGPIOPort_t;

typedef enum
{
    B_HAL_PIN0,
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
    B_HAL_PINAll,
    B_HAL_PIN_INVALID,
} bHalGPIOPin_t;

typedef struct
{
    bHalGPIOPort_t port;
    bHalGPIOPin_t  pin;
} bHalGPIOInstance_t;

typedef void (*pbExtiHandler_t)(void);

typedef struct bHalGPIO_EXTI_Struct
{
    bHalGPIOPin_t   pin;
    pbExtiHandler_t handler;
} bHalGPIO_EXTI_t;

/**
 * \}
 */

/**
 * \defgroup GPIO_Exported_Defines
 * \{
 */
#define B_HAL_GPIO_INPUT 0
#define B_HAL_GPIO_OUTPUT 1

#define B_HAL_GPIO_NOPULL 0
#define B_HAL_GPIO_PULLUP 1
#define B_HAL_GPIO_PULLDOWN 2

/**
 * \}
 */

/**
 * \defgroup GPIO_Exported_Macros
 * \{
 */

#define B_HAL_GPIO_ISVALID(port, pin) (port != B_HAL_GPIO_INVALID && pin != B_HAL_PIN_INVALID)

#define bHAL_REG_GPIO_EXTI(_pin, _handler)                                              \
    bSECTION_ITEM_REGISTER_FLASH(b_hal_gpio, bHalGPIO_EXTI_t, CONCAT_2(exti, _pin)) = { \
        .pin = _pin, .handler = _handler}
/**
 * \}
 */

/**
 * \defgroup GPIO_Exported_Functions
 * \{
 */

void     bHalGPIO_Config(bHalGPIOPort_t port, bHalGPIOPin_t pin, uint8_t mode, uint8_t pull);
void     bHalGPIO_WritePin(bHalGPIOPort_t port, bHalGPIOPin_t pin, uint8_t s);
uint8_t  bHalGPIO_ReadPin(bHalGPIOPort_t port, bHalGPIOPin_t pin);
void     bHalGPIO_Write(bHalGPIOPort_t port, uint16_t dat);
uint16_t bHalGPIO_Read(bHalGPIOPort_t port);

void bHalGPIO_EXTI_IRQHandler(bHalGPIOPin_t pin);

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
