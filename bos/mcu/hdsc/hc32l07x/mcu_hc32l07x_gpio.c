/**
 *!
 * \file        mcu_hc32l13x_gpio.c
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

#if (MCU_PLATFORM == 4002)

//         Register Address

#define GPIO_REG_OFF (0x40UL)
#define GPIO_REG_BASE (0x40020C00UL)

typedef struct
{
    volatile uint32_t PXSEL[16];
    volatile uint32_t Reserved[48];
    volatile uint32_t DIR;
    volatile uint32_t IN;
    volatile uint32_t OUT;
    volatile uint32_t ADS;
    volatile uint32_t BSET;
    volatile uint32_t BCLR;
    volatile uint32_t BSETCLR;
    volatile uint32_t DR;
    volatile uint32_t PU;
    volatile uint32_t PD;
    volatile uint32_t OD;
} McuGpioReg_t;

void bMcuGpioConfig(bHalGPIOPort_t port, bHalGPIOPin_t pin, bHalGPIODir_t dir, bHalGPIOPull_t pull)
{
    int           i     = 0;
    McuGpioReg_t *pGpio = (McuGpioReg_t *)(GPIO_REG_BASE + port * GPIO_REG_OFF);

    if (!B_HAL_GPIO_ISVALID(port, pin) || (port > B_HAL_GPIOD))
    {
        return;
    }

    for (i = 0; i < 16; i++)
    {
        if (pin == B_HAL_PINAll)
        {
            pGpio->PXSEL[i] = 0;
        }
        else if (i == pin)
        {
            pGpio->PXSEL[i] = 0;
            break;
        }
    }

    if (pin == B_HAL_PINAll)
    {
        pGpio->DIR = (dir == B_HAL_GPIO_OUTPUT) ? 0 : 0xFFFF;
        pGpio->PU  = (pull == B_HAL_GPIO_PULLUP) ? 0xFFFF : 0;
        pGpio->PD  = (pull == B_HAL_GPIO_PULLDOWN) ? 0xFFFF : 0;
    }
    else
    {
        pGpio->DIR &= ~(0x1 << pin);
        pGpio->DIR |= (dir == B_HAL_GPIO_OUTPUT) ? 0 : (0x1 << pin);
        pGpio->PU &= ~(0x1 << pin);
        pGpio->PU = (pull == B_HAL_GPIO_PULLUP) ? (0x1 << pin) : 0;
        pGpio->PD &= ~(0x1 << pin);
        pGpio->PD = (pull == B_HAL_GPIO_PULLDOWN) ? (0x1 << pin) : 0;
    }
}

void bMcuGpioWritePin(bHalGPIOPort_t port, bHalGPIOPin_t pin, uint8_t s)
{
    uint32_t      cs_val = 0x00000001 << pin;
    McuGpioReg_t *pGpio  = (McuGpioReg_t *)(GPIO_REG_BASE + port * GPIO_REG_OFF);
    if (!B_HAL_GPIO_ISVALID(port, pin) || pin == B_HAL_PINAll || (port > B_HAL_GPIOD))
    {
        return;
    }
    if (s == 1)
    {
        cs_val <<= 16;
    }
    pGpio->BSETCLR = cs_val;
}

uint8_t bMcuGpioReadPin(bHalGPIOPort_t port, bHalGPIOPin_t pin)
{
    uint32_t      id_val = 0;
    McuGpioReg_t *pGpio  = (McuGpioReg_t *)(GPIO_REG_BASE + port * GPIO_REG_OFF);
    if (!B_HAL_GPIO_ISVALID(port, pin) || pin == B_HAL_PINAll || (port > B_HAL_GPIOD))
    {
        return 0;
    }
    id_val = pGpio->IN;
    return ((id_val & (0x0001 << pin)) != 0);
}

void bMcuGpioWritePort(bHalGPIOPort_t port, uint16_t dat)
{
    McuGpioReg_t *pGpio = (McuGpioReg_t *)(GPIO_REG_BASE + port * GPIO_REG_OFF);
    if (port > B_HAL_GPIOD)
    {
        return;
    }
    pGpio->OUT = dat;
}

uint16_t bMcuGpioReadPort(bHalGPIOPort_t port)
{
    uint32_t      id_val = 0;
    McuGpioReg_t *pGpio  = (McuGpioReg_t *)(GPIO_REG_BASE + port * GPIO_REG_OFF);
    if (port > B_HAL_GPIOD)
    {
        return 0;
    }
    id_val = pGpio->IN;
    return (id_val & 0xffff);
}

#endif
/************************ Copyright (c) 2020 Bean *****END OF FILE****/
