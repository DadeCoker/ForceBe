/**
 *!
 * \file        mcu_stm32f10x_gpio.c
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

/*Includes ----------------------------------------------*/
#include "b_config.h"
#include "hal/inc/b_hal_gpio.h"

#if (_MCU_PLATFORM == 1001)
#include "stm32f10x.h"

static void* GPIO_PortTable[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG};

static void _GpioConfig(bHalGPIOPort_t port, bHalGPIOPin_t pin, bHalGPIODir_t dir,
                        bHalGPIOPull_t pull)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    if (!B_HAL_GPIO_ISVALID(port, pin))
    {
        return;
    }
    if (dir == B_HAL_GPIO_OUTPUT)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    }
    else
    {
        if (pull == B_HAL_GPIO_PULLUP)
        {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        }
        else if (pull == B_HAL_GPIO_PULLDOWN)
        {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
        }
        else
        {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        }
    }
    GPIO_InitStructure.GPIO_Pin   = (pin == B_HAL_PINAll) ? 0xffff : (0x0001 << pin);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_PortTable[port], &GPIO_InitStructure);
}

static void _GpioWritePin(bHalGPIOPort_t port, bHalGPIOPin_t pin, uint8_t s)
{
    if (!B_HAL_GPIO_ISVALID(port, pin))
    {
        return;
    }
    if (s)
    {
        GPIO_SetBits(GPIO_PortTable[port], 0x0001 << pin);
    }
    else
    {
        GPIO_ResetBits(GPIO_PortTable[port], 0x0001 << pin);
    }
}

static uint8_t _GpioReadPin(bHalGPIOPort_t port, bHalGPIOPin_t pin)
{
    if (!B_HAL_GPIO_ISVALID(port, pin))
    {
        return 0;
    }
    return GPIO_ReadInputDataBit(GPIO_PortTable[port], 0x0001 << pin);
}

static void _GpioWrite(bHalGPIOPort_t port, uint16_t dat)
{
    if (!B_HAL_GPIO_ISVALID(port, 0))
    {
        return;
    }
    GPIO_Write(GPIO_PortTable[port], dat);
}

static uint16_t _GpioRead(bHalGPIOPort_t port)
{
    if (!B_HAL_GPIO_ISVALID(port, 0))
    {
        return 0;
    }
    return GPIO_ReadInputData(GPIO_PortTable[port]);
}

bHalGPIODriver_t bHalGPIODriver = {
    .pGpioConfig    = _GpioConfig,
    .pGpioWritePin  = _GpioWritePin,
    .pGpioWritePort = _GpioWrite,
    .pGpioReadPin   = _GpioReadPin,
    .pGpioReadPort  = _GpioRead,
};

#endif
/************************ Copyright (c) 2020 Bean *****END OF FILE****/
