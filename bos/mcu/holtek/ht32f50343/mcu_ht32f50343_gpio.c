/**
 *!
 * \file        mcu_ht32f50343_gpio.c
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

#if (defined(TH32F50343))

//         Register Address

#define GPIO_REG_OFF (0x2000UL)
#define GPIO_REG_BASE (0x400B0000UL)

typedef struct
{
    volatile uint32_t DIRCR; /*!< 0x000         Data Direction Control Register */
    volatile uint32_t INER;  /*!< 0x004         Input function enable register  */
    volatile uint32_t PUR;   /*!< 0x008         Pull-Up Selection Register   */
    volatile uint32_t PDR;   /*!< 0x00C         Pull-Down Selection Register   */
    volatile uint32_t ODR;   /*!< 0x010         Open Drain Selection Register   */
    volatile uint32_t DRVR;  /*!< 0x014         Drive Current Selection Register  */
    volatile uint32_t LOCKR; /*!< 0x018         Lock Register */
    volatile uint32_t DINR;  /*!< 0x01c         Data Input Register  */
    volatile uint32_t DOUTR; /*!< 0x020         Data Output Register */
    volatile uint32_t SRR;   /*!< 0x024         Output Set and Reset Control Register   */
    volatile uint32_t RR;    /*!< 0x028         Output Reset Control Register    */
} McuGpioReg_t;

void bMcuGpioConfig(bHalGPIOPort_t port, bHalGPIOPin_t pin, bHalGPIODir_t dir, bHalGPIOPull_t pull)
{
    uint32_t      dircr_val = 0;
    uint32_t      pur_val   = 0;
    uint32_t      pdr_val   = 0;
    uint32_t      iner_val  = 0;
    McuGpioReg_t *pGpio     = (McuGpioReg_t *)(GPIO_REG_BASE + port * GPIO_REG_OFF);

    if (!B_HAL_GPIO_ISVALID(port, pin))
    {
        return;
    }

    if (dir == B_HAL_GPIO_OUTPUT)
    {
        dircr_val = (pin == B_HAL_PINAll) ? 0x0000FFFF : 1;
    }
    else
    {
        iner_val = (pin == B_HAL_PINAll) ? 0x0000FFFF : 1;
    }

    if (pull != B_HAL_GPIO_NOPULL)
    {
        if (pull == B_HAL_GPIO_PULLUP)
        {
            pur_val = (pin == B_HAL_PINAll) ? 0x0000FFFF : 1;
        }
        else
        {
            pdr_val = (pin == B_HAL_PINAll) ? 0x0000FFFF : 1;
        }
    }

    if (pin == B_HAL_PINAll)
    {
        B_WRITE_REG(pGpio->DIRCR, dircr_val);
        B_WRITE_REG(pGpio->INER, iner_val);
        B_WRITE_REG(pGpio->PUR, pur_val);
        B_WRITE_REG(pGpio->PDR, pdr_val);
    }
    else
    {
        B_MODIFY_REG(pGpio->DIRCR, (0x00000001 << (pin)), (dircr_val << (pin)));
        B_MODIFY_REG(pGpio->INER, (0x00000001 << (pin)), (iner_val << (pin)));
        B_MODIFY_REG(pGpio->PUR, (0x00000001 << (pin)), (pur_val << (pin)));
        B_MODIFY_REG(pGpio->PDR, (0x00000001 << (pin)), (pdr_val << (pin)));
    }
}

void bMcuGpioWritePin(bHalGPIOPort_t port, bHalGPIOPin_t pin, uint8_t s)
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
    pGpio->SRR = cs_val;
}

uint8_t bMcuGpioReadPin(bHalGPIOPort_t port, bHalGPIOPin_t pin)
{
    uint32_t      id_val = 0;
    McuGpioReg_t *pGpio  = (McuGpioReg_t *)(GPIO_REG_BASE + port * GPIO_REG_OFF);
    if (!B_HAL_GPIO_ISVALID(port, pin) || pin == B_HAL_PINAll)
    {
        return 0;
    }
    id_val = pGpio->DINR;
    return ((id_val & (0x0001 << pin)) != 0);
}

void bMcuGpioWritePort(bHalGPIOPort_t port, uint16_t dat)
{
    McuGpioReg_t *pGpio = (McuGpioReg_t *)(GPIO_REG_BASE + port * GPIO_REG_OFF);
    if (!B_HAL_GPIO_ISVALID(port, 0))
    {
        return;
    }
    pGpio->DOUTR = dat;
}

uint16_t bMcuGpioReadPort(bHalGPIOPort_t port)
{
    uint32_t      id_val = 0;
    McuGpioReg_t *pGpio  = (McuGpioReg_t *)(GPIO_REG_BASE + port * GPIO_REG_OFF);
    if (!B_HAL_GPIO_ISVALID(port, 0))
    {
        return 0;
    }
    id_val = pGpio->DINR;
    return (id_val & 0xffff);
}

#endif
/************************ Copyright (c) 2020 Bean *****END OF FILE****/
