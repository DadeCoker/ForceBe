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
#include <stdint.h>

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

typedef enum
{
    B_HAL_GPIO_INPUT,
    B_HAL_GPIO_OUTPUT,
    B_HAL_GPIO_DIR_INVALID,
} bHalGPIODir_t;

typedef enum
{
    B_HAL_GPIO_NOPULL,
    B_HAL_GPIO_PULLUP,
    B_HAL_GPIO_PULLDOWN,
    B_HAL_GPIO_PULL_INVALID,
} bHalGPIOPull_t;

typedef struct
{
    bHalGPIOPort_t port;
    bHalGPIOPin_t  pin;
} bHalGPIOInstance_t;

typedef enum
{
    B_HAL_GPIO_EXTI0,
    B_HAL_GPIO_EXTI1,
    B_HAL_GPIO_EXTI2,
    B_HAL_GPIO_EXTI3,
    B_HAL_GPIO_EXTI4,
    B_HAL_GPIO_EXTI5,
    B_HAL_GPIO_EXTI6,
    B_HAL_GPIO_EXTI7,
    B_HAL_GPIO_EXTI8,
    B_HAL_GPIO_EXTI9,
    B_HAL_GPIO_EXTI10,
    B_HAL_GPIO_EXTI11,
    B_HAL_GPIO_EXTI12,
    B_HAL_GPIO_EXTI13,
    B_HAL_GPIO_EXTI14,
    B_HAL_GPIO_EXTI15,
    B_HAL_GPIO_EXIT_LINE_INVALID
} bHalGPIOExtiLine_t;

typedef enum
{
    B_HAL_GPIO_EXTI_RISE,
    B_HAL_GPIO_EXTI_FALL,
    B_HAL_GPIO_EXTI_BOTH,
    B_HAL_GPIO_EXTI_HIGH,
    B_HAL_GPIO_EXTI_LOW,
    B_HAL_GPIO_EXIT_TRIG_INVALID
} bHalGPIOExtiTrig_t;

typedef void (*pbHalGpioExtiCallback_t)(bHalGPIOExtiLine_t line, bHalGPIOExtiTrig_t trig, void *arg);

typedef struct bHalGPIOExti
{
    bHalGPIOExtiLine_t      line;
    bHalGPIOExtiTrig_t      trig;
    pbHalGpioExtiCallback_t cb;
    void                   *arg;
    struct bHalGPIOExti    *next;
} bHalGPIOExti_t;

/**
 * \}
 */

/**
 * \defgroup GPIO_Exported_Macros
 * \{
 */

#define B_HAL_GPIO_ISVALID(port, pin) (port != B_HAL_GPIO_INVALID && pin != B_HAL_PIN_INVALID)

#define B_HAL_GPIO_ADD_EXTI_CALLBACK(_line, _trig, _cb, _arg) \
    static bHalGPIOExti_t exti_##_line = {                    \
        .line = _line,                                        \
        .trig = _trig,                                        \
        .cb   = _cb,                                          \
        .arg  = _arg,                                         \
        .next = NULL,                                         \
    };                                                        \
    bHalGpioAddExtiCallback(&exti_##_line);
/**
 * \}
 */

/**
 * \defgroup GPIO_Exported_Functions
 * \{
 */

void bMcuGpioConfig(bHalGPIOPort_t port, bHalGPIOPin_t pin, bHalGPIODir_t dir, bHalGPIOPull_t pull);
void bMcuGpioWritePin(bHalGPIOPort_t port, bHalGPIOPin_t pin, uint8_t s);
void bMcuGpioWritePort(bHalGPIOPort_t port, uint16_t dat);
uint8_t  bMcuGpioReadPin(bHalGPIOPort_t port, bHalGPIOPin_t pin);
uint16_t bMcuGpioReadPort(bHalGPIOPort_t port);
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
void bHalGpioConfig(bHalGPIOPort_t port, bHalGPIOPin_t pin, bHalGPIODir_t dir, bHalGPIOPull_t pull);
void bHalGpioWritePin(bHalGPIOPort_t port, bHalGPIOPin_t pin, uint8_t s);
void bHalGpioWritePort(bHalGPIOPort_t port, uint16_t dat);
uint8_t  bHalGpioReadPin(bHalGPIOPort_t port, bHalGPIOPin_t pin);
uint16_t bHalGpioReadPort(bHalGPIOPort_t port);

// 驱动层调用，注册外部中断回调
// 建议不要直接调用此函数， 使用 B_HAL_GPIO_ADD_EXTI_CALLBACK 替代
int bHalGpioAddExtiCallback(bHalGPIOExti_t *pexti);

// MCU的中断服务函数，调用此函数通知HAL层，中断触发
void bHalGpioNotifyExti(bHalGPIOExtiLine_t line, bHalGPIOExtiTrig_t trig);
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
