/**
 *!
 * \file        mcu_hc32l13x_flash.c
 * \version     v0.0.1
 * \date        2021/06/13
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 Bean
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
#include <string.h>

#include "b_config.h"
#include "hal/inc/b_hal_flash.h"

#if (defined(HC32L07X))

#define FLASH_BASE_ADDR (0x0000000UL)
#define FLASH_PAGE_SIZE (512)
#define FLASH_MAX_SIZE (128 * 1024)

#define FLASH_KEY_1 (0x5A5AUL)
#define FLASH_KEY_2 (0xA5A5UL)
#define FLASH_PER_TIMEOUT (0x000B0000UL)
#define FLASH_PG_TIMEOUT (0x00002000UL)

typedef struct
{
    volatile uint32_t TNVS;
    volatile uint32_t TPGS;
    volatile uint32_t TPROG;
    volatile uint32_t TSERASE;
    volatile uint32_t TMERASE;
    volatile uint32_t TPRCV;
    volatile uint32_t TSRCV;
    volatile uint32_t TMRCV;
    volatile uint32_t CR;
    volatile uint32_t IFR;
    volatile uint32_t ICLR;
    volatile uint32_t BYPASS;
    volatile uint32_t SLOCK;
} McuFlashReg_t;

#define MCU_FLASH ((McuFlashReg_t *)0x40020000)

int bMcuFlashInit()
{
    return 0;
}

int bMcuFlashUnlock()
{
    int retval        = 0;
    MCU_FLASH->BYPASS = FLASH_KEY_1;
    MCU_FLASH->BYPASS = FLASH_KEY_2;
    MCU_FLASH->SLOCK  = 0xFFFFFFFF;
    return retval;
}

int bMcuFlashLock()
{
    int retval        = 0;
    MCU_FLASH->BYPASS = FLASH_KEY_1;
    MCU_FLASH->BYPASS = FLASH_KEY_2;
    MCU_FLASH->SLOCK  = 0x0;
    return retval;
}

int bMcuFlashErase(uint32_t raddr, uint32_t pages)
{
    int      retval  = 0;
    int      timeout = 0;
    uint32_t i       = 0;

    raddr = FLASH_BASE_ADDR + raddr;
    raddr = raddr / FLASH_PAGE_SIZE * FLASH_PAGE_SIZE;

    if ((raddr + (pages * FLASH_PAGE_SIZE)) > (FLASH_BASE_ADDR + FLASH_MAX_SIZE) ||
        ((MCU_FLASH->CR) & (0x1 << 4)) != 0)
    {
        return -1;
    }

    for (i = 0; i < pages; i++)
    {
        timeout = FLASH_PER_TIMEOUT;
        while ((MCU_FLASH->CR & 0x3) != 2 && timeout > 0)
        {
            MCU_FLASH->BYPASS = FLASH_KEY_1;
            MCU_FLASH->BYPASS = FLASH_KEY_2;
            MCU_FLASH->CR &= ~(0x3 << 0);
            MCU_FLASH->BYPASS = FLASH_KEY_1;
            MCU_FLASH->BYPASS = FLASH_KEY_2;
            MCU_FLASH->CR |= 0x2;
            timeout--;
        }

        if (timeout <= 0)
        {
            retval = -2;
            break;
        }

        *((volatile uint8_t *)raddr) = 0;

        timeout = FLASH_PER_TIMEOUT;
        while (((MCU_FLASH->CR) & (0x1 << 4)) != 0 && timeout > 0)
        {
            timeout--;
        }
        if (timeout <= 0)
        {
            retval = -2;
            break;
        }
        raddr += FLASH_PAGE_SIZE;
    }

    timeout = FLASH_PER_TIMEOUT;
    while ((MCU_FLASH->CR & 0x3) != 0 && timeout > 0)
    {
        MCU_FLASH->BYPASS = FLASH_KEY_1;
        MCU_FLASH->BYPASS = FLASH_KEY_2;
        MCU_FLASH->CR &= ~(0x3 << 0);
        MCU_FLASH->BYPASS = FLASH_KEY_1;
        MCU_FLASH->BYPASS = FLASH_KEY_2;
        MCU_FLASH->CR |= 0x0;
        timeout--;
    }

    if (timeout <= 0)
    {
        return -2;
    }

    return retval;
}

int bMcuFlashWrite(uint32_t raddr, const uint8_t *pbuf, uint32_t len)
{
    int     timeout = 0;
    uint8_t wdata   = 0;
    uint8_t wlen = len, i = 0;
    raddr = FLASH_BASE_ADDR + raddr;
    if (pbuf == NULL || (raddr & 0x1) || (raddr + len) > (FLASH_MAX_SIZE + FLASH_BASE_ADDR) ||
        ((MCU_FLASH->CR) & (0x1 << 4)) != 0)
    {
        return -1;
    }

    timeout = FLASH_PER_TIMEOUT;
    while ((MCU_FLASH->CR & 0x3) != 1 && timeout > 0)
    {
        MCU_FLASH->BYPASS = FLASH_KEY_1;
        MCU_FLASH->BYPASS = FLASH_KEY_2;
        MCU_FLASH->CR &= ~(0x3 << 0);
        MCU_FLASH->BYPASS = FLASH_KEY_1;
        MCU_FLASH->BYPASS = FLASH_KEY_2;
        MCU_FLASH->CR |= 0x1;
        timeout--;
    }

    if (timeout <= 0)
    {
        return -2;
    }

    for (i = 0; i < wlen; i++)
    {
        wdata = pbuf[i];

        *((volatile uint8_t *)raddr) = wdata;

        timeout = FLASH_PER_TIMEOUT;
        while (((MCU_FLASH->CR) & (0x1 << 4)) != 0 && timeout > 0)
        {
            timeout--;
        }
        if (timeout <= 0)
        {
            return -2;
        }
        raddr += 1;
    }

    timeout = FLASH_PER_TIMEOUT;
    while ((MCU_FLASH->CR & 0x3) != 0 && timeout > 0)
    {
        MCU_FLASH->BYPASS = FLASH_KEY_1;
        MCU_FLASH->BYPASS = FLASH_KEY_2;
        MCU_FLASH->CR &= ~(0x3 << 0);
        MCU_FLASH->BYPASS = FLASH_KEY_1;
        MCU_FLASH->BYPASS = FLASH_KEY_2;
        MCU_FLASH->CR |= 0x0;
        timeout--;
    }

    if (timeout <= 0)
    {
        return -2;
    }

    return wlen;
}

int bMcuFlashRead(uint32_t raddr, uint8_t *pbuf, uint32_t len)
{
    if (pbuf == NULL || (raddr + FLASH_BASE_ADDR + len) > (FLASH_MAX_SIZE + FLASH_BASE_ADDR))
    {
        return -1;
    }
    raddr = FLASH_BASE_ADDR + raddr;
    memcpy(pbuf, (const uint8_t *)raddr, len);
    return len;
}

#endif

/************************ Copyright (c) 2021 Bean *****END OF FILE****/
