/**
 *!
 * \file        mcu_n32l40x_gpio.c
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

#if (_MCU_PLATFORM == 2001)
#include "n32l40x.h"

static GPIO_Module*   GPIO_PortTable[] = {GPIOA, GPIOB, GPIOC, GPIOD};
const static uint16_t GPIO_PinTable[]  = {
    GPIO_PIN_0,  GPIO_PIN_1,  GPIO_PIN_2,  GPIO_PIN_3,  GPIO_PIN_4,  GPIO_PIN_5,
    GPIO_PIN_6,  GPIO_PIN_7,  GPIO_PIN_8,  GPIO_PIN_9,  GPIO_PIN_10, GPIO_PIN_11,
    GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_All};

static void _GpioConfig(bHalGPIOPort_t port, bHalGPIOPin_t pin, bHalGPIODir_t dir,
                        bHalGPIOPull_t pull)
{
    GPIO_InitType GPIO_InitStruct;
    GPIO_InitStruct(&GPIO_InitStruct);
    if (port > B_HAL_GPIOD)
    {
        return;
    }
    GPIO_InitStruct.Pin       = GPIO_PinTable[pin];
    GPIO_InitStruct.GPIO_Mode = (dir == B_HAL_GPIO_INPUT) ? GPIO_Mode_Input : GPIO_Mode_Out_PP;
    if (pull != B_HAL_GPIO_NOPULL)
    {
        GPIO_InitStruct.GPIO_Pull = (pull == B_HAL_GPIO_PULLUP) ? GPIO_Pull_Up : GPIO_Pull_Down;
    }
    GPIO_InitStruct.GPIO_Slew_Rate = GPIO_Slew_Rate_High;
    GPIO_InitPeripheral(GPIO_PortTable[port], &GPIO_InitStruct);
}

static void _GpioWritePin(bHalGPIOPort_t port, bHalGPIOPin_t pin, uint8_t s)
{
    if (port > B_HAL_GPIOD)
    {
        return;
    }
    Bit_OperateType sta = (s) ? Bit_SET : Bit_RESET;
    GPIO_WriteBit(GPIO_PortTable[port], GPIO_PinTable[pin], sta);
}

static uint8_t _GpioReadPin(bHalGPIOPort_t port, bHalGPIOPin_t pin)
{
    if (port > B_HAL_GPIOD)
    {
        return 0;
    }
    return GPIO_ReadInputDataBit(GPIO_PortTable[port], GPIO_PinTable[pin]);
}

static void _GpioWrite(bHalGPIOPort_t port, uint16_t dat)
{
    if (port > B_HAL_GPIOD)
    {
        return;
    }
    GPIO_Write(GPIO_PortTable[port], dat);
}

static uint16_t _GpioRead(bHalGPIOPort_t port)
{
    if (port > B_HAL_GPIOD)
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
