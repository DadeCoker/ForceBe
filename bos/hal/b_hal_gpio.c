/**
 *!
 * \file        b_hal_gpio.c
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SUARTL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
/*Includes ----------------------------------------------*/
#include "hal/inc/b_hal.h"

/**
 * \addtogroup B_HAL
 * \{
 */

/**
 * \addtogroup GPIO
 * \{
 */

/**
 * \addtogroup GPIO_Exported_Functions
 * \{
 */

__WEAKDEF void bMcuGpioConfig(bHalGPIOPort_t port, bHalGPIOPin_t pin, bHalGPIODir_t dir,
                              bHalGPIOPull_t pull)
{
    ;
}

__WEAKDEF void bMcuGpioWritePin(bHalGPIOPort_t port, bHalGPIOPin_t pin, uint8_t s)
{
    ;
}

__WEAKDEF void bMcuGpioWritePort(bHalGPIOPort_t port, uint16_t dat)
{
    ;
}

__WEAKDEF uint8_t bMcuGpioReadPin(bHalGPIOPort_t port, bHalGPIOPin_t pin)
{
    return 0;
}

__WEAKDEF uint16_t bMcuGpioReadPort(bHalGPIOPort_t port)
{
    return 0;
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

void bHalGpioConfig(bHalGPIOPort_t port, bHalGPIOPin_t pin, bHalGPIODir_t dir, bHalGPIOPull_t pull)
{
    bMcuGpioConfig(port, pin, dir, pull);
}

void bHalGpioWritePin(bHalGPIOPort_t port, bHalGPIOPin_t pin, uint8_t s)
{
    bMcuGpioWritePin(port, pin, s);
}

void bHalGpioWritePort(bHalGPIOPort_t port, uint16_t dat)
{
    bMcuGpioWritePort(port, dat);
}

uint8_t bHalGpioReadPin(bHalGPIOPort_t port, bHalGPIOPin_t pin)
{
    return bMcuGpioReadPin(port, pin);
}

uint16_t bHalGpioReadPort(bHalGPIOPort_t port)
{
    return bMcuGpioReadPort(port);
}

/**
 * \}
 */

/**
 * \}
 */

/**
 * \}
 */

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
