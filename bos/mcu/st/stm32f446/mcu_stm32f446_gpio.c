/**
 *!
 * \file        mcu_stm32f446_gpio.c
 * \version     v0.0.1
 * \date        2021/12/09
 * \author      X1aSheng(michael@zhis.net)
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 X1aSheng
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

#if (_MCU_PLATFORM == C_N_MCU_STM32F446)

//         Register Address

#define GPIO_REG_OFF (0x400UL)
#define GPIO_REG_BASE (0x40020000UL)

typedef struct
{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];

} McuGpioReg_t;

static void _GpioConfig(bHalGPIOPort_t port, bHalGPIOPin_t pin, bHalGPIODir_t dir,
                        bHalGPIOPull_t pull)
{
    uint32_t      mode_val   = 0;
    uint32_t      otype_val  = 0;
    uint32_t      ospeed_val = 0;
    uint32_t      pupd_val   = 0;
    McuGpioReg_t *pGpio      = (McuGpioReg_t *)(GPIO_REG_BASE + port * GPIO_REG_OFF);

    if (!B_HAL_GPIO_ISVALID(port, pin))
    {
        return;
    }

    if (dir == B_HAL_GPIO_OUTPUT)
    {
        mode_val   = (pin == B_HAL_PINAll) ? 0x55555555 : 5;
        otype_val  = (pin == B_HAL_PINAll) ? 0x00000000 : 0;
        ospeed_val = (pin == B_HAL_PINAll) ? 0xffffffff : 3;
        pupd_val   = (pin == B_HAL_PINAll) ? 0x00000000 : 0;
    }
    else if (pull != B_HAL_GPIO_NOPULL)
    {
        mode_val = (pin == B_HAL_PINAll) ? 0x00000000 : 0;
        if (pull == B_HAL_GPIO_PULLUP)
        {
            pupd_val = (pin == B_HAL_PINAll) ? 0x55555555 : 1;
        }
        else
        {
            pupd_val = (pin == B_HAL_PINAll) ? 0xAAAAAAAA : 2;
        }
    }

    if (pin == B_HAL_PINAll)
    {
        pGpio->MODER   = mode_val;
        pGpio->OTYPER  = otype_val;
        pGpio->OSPEEDR = ospeed_val;
        pGpio->PUPDR   = pupd_val;
    }
    else
    {
        pGpio->MODER &= ~(0x00000003 << (pin * 2));
        pGpio->MODER |= (mode_val << (pin * 2));
        pGpio->OTYPER &= ~(0x00000001 << (pin * 1));
        pGpio->OTYPER |= (otype_val << (pin * 1));
        pGpio->OSPEEDR &= ~(0x00000003 << (pin * 2));
        pGpio->OSPEEDR |= (ospeed_val << (pin * 2));
        pGpio->PUPDR &= ~(0x00000003 << (pin * 2));
        pGpio->PUPDR |= (pupd_val << (pin * 2));
    }
}

static void _GpioWritePin(bHalGPIOPort_t port, bHalGPIOPin_t pin, uint8_t s)
{
    uint32_t      cs_val = 0x00000001 << pin;
    McuGpioReg_t *pGpio  = (McuGpioReg_t *)(GPIO_REG_BASE + port * GPIO_REG_OFF);
    if (!B_HAL_GPIO_ISVALID(port, pin) || pin == B_HAL_PINAll)
    {
        return;
    }
    if (s == 0)
    {
        cs_val <<= 16;
    }
    pGpio->BSRR = cs_val;
}

static uint8_t _GpioReadPin(bHalGPIOPort_t port, bHalGPIOPin_t pin)
{
    uint32_t      id_val = 0;
    McuGpioReg_t *pGpio  = (McuGpioReg_t *)(GPIO_REG_BASE + port * GPIO_REG_OFF);
    if (!B_HAL_GPIO_ISVALID(port, pin) || pin == B_HAL_PINAll)
    {
        return 0;
    }
    id_val = pGpio->IDR;
    return ((id_val & (0x0001 << pin)) != 0);
}

static void _GpioWrite(bHalGPIOPort_t port, uint16_t dat)
{
    McuGpioReg_t *pGpio = (McuGpioReg_t *)(GPIO_REG_BASE + port * GPIO_REG_OFF);
    if (!B_HAL_GPIO_ISVALID(port, 0))
    {
        return;
    }
    pGpio->ODR = dat;
}

static uint16_t _GpioRead(bHalGPIOPort_t port)
{
    uint32_t      id_val = 0;
    McuGpioReg_t *pGpio  = (McuGpioReg_t *)(GPIO_REG_BASE + port * GPIO_REG_OFF);
    if (!B_HAL_GPIO_ISVALID(port, 0))
    {
        return 0;
    }
    id_val = pGpio->IDR;
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
/************************ Copyright (c) 2021 X1aSheng *****END OF FILE****/
