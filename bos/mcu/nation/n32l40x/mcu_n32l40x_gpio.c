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

//                 Register Address

#define GPIO_REG_OFF (0x400)
#define GPIO_REG_BASE (0x40010800UL)
#define GPIO_REG_MODE (GPIO_REG_BASE + 0x00)
#define GPIO_REG_OTYPE (GPIO_REG_BASE + 0x04)
#define GPIO_REG_UPD (GPIO_REG_BASE + 0x0C)
#define GPIO_REG_ID (GPIO_REG_BASE + 0x10)
#define GPIO_REG_OD (GPIO_REG_BASE + 0x14)
#define GPIO_REG_BSC (GPIO_REG_BASE + 0x18)

static void _GpioConfig(bHalGPIOPort_t port, bHalGPIOPin_t pin, bHalGPIODir_t dir,
                        bHalGPIOPull_t pull)
{
    uint32_t dir_val  = 0;
    uint32_t pull_val = 0;
    if (port > B_HAL_GPIOD)
    {
        return;
    }
    if (dir == B_HAL_GPIO_OUTPUT)
    {
        dir_val = 1;
    }
    if (pull == B_HAL_GPIO_PULLUP)
    {
        pull_val = 1;
    }
    else if (pull == B_HAL_GPIO_PULLDOWN)
    {
        pull_val = 2;
    }

    if (pin == B_HAL_PINAll)
    {
        if (dir == B_HAL_GPIO_OUTPUT)
        {
            *((volatile uint32_t *)(GPIO_REG_MODE + port * GPIO_REG_OFF)) = 0x55555555;
        }
        else
        {
            *((volatile uint32_t *)(GPIO_REG_MODE + port * GPIO_REG_OFF)) = 0;
        }

        if (pull == B_HAL_GPIO_PULLUP)
        {
            *((volatile uint32_t *)(GPIO_REG_UPD + port * GPIO_REG_OFF)) = 0x55555555;
        }
        else if (pull == B_HAL_GPIO_PULLDOWN)
        {
            *((volatile uint32_t *)(GPIO_REG_UPD + port * GPIO_REG_OFF)) = 0xAAAAAAAA;
        }
        else
        {
            *((volatile uint32_t *)(GPIO_REG_UPD + port * GPIO_REG_OFF)) = 0;
        }
    }
    else
    {
        *((volatile uint32_t *)(GPIO_REG_MODE + port * GPIO_REG_OFF)) &= ~(0x11 << pin);
        *((volatile uint32_t *)(GPIO_REG_MODE + port * GPIO_REG_OFF)) |= (dir_val << pin);

        *((volatile uint32_t *)(GPIO_REG_UPD + port * GPIO_REG_OFF)) &= ~(0x11 << pin);
        *((volatile uint32_t *)(GPIO_REG_UPD + port * GPIO_REG_OFF)) |= (pull_val << pin);
    }
}

static void _GpioWritePin(bHalGPIOPort_t port, bHalGPIOPin_t pin, uint8_t s)
{
    uint32_t cs_val = 0x1 << pin;
    if (port > B_HAL_GPIOD)
    {
        return;
    }
    if (s == 0)
    {
        cs_val <<= 16;
    }
    *((volatile uint32_t *)(GPIO_REG_BSC + port * GPIO_REG_OFF)) = cs_val;
}

static uint8_t _GpioReadPin(bHalGPIOPort_t port, bHalGPIOPin_t pin)
{
    uint32_t id_val = 0;
    if (port > B_HAL_GPIOD || pin == B_HAL_PINAll)
    {
        return 0;
    }
    id_val = *((volatile uint32_t *)(GPIO_REG_ID + port * GPIO_REG_OFF));
    return ((id_val & (0x0001 << pin)) != 0);
}

static void _GpioWrite(bHalGPIOPort_t port, uint16_t dat)
{
    if (port > B_HAL_GPIOD)
    {
        return;
    }
    *((volatile uint32_t *)(GPIO_REG_OD + port * GPIO_REG_OFF)) = dat;
}

static uint16_t _GpioRead(bHalGPIOPort_t port)
{
    uint32_t id_val = 0;
    if (port > B_HAL_GPIOD)
    {
        return 0;
    }
    id_val = *((volatile uint32_t *)(GPIO_REG_ID + port * GPIO_REG_OFF));
    return (id_val & 0xffff);
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
