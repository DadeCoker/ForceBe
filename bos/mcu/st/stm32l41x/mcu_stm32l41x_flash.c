/**
 *!
 * \file        mcu_stm32l4xx_flash.c
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

#if (MCU_PLATFORM == 1201)

#define FLASH_BASE_ADDR (0x8000000UL)

#ifndef FLASH_PAGE_SIZE
#define FLASH_PAGE_SIZE (2048)
#endif

#define FLASH_KEY_1 (0x45670123UL)
#define FLASH_KEY_2 (0xCDEF89ABUL)
#define FLASH_PER_TIMEOUT (0x000B0000UL)
#define FLASH_PG_TIMEOUT (0x00002000UL)

typedef struct
{
    volatile uint32_t ACR;
    uint32_t          PDKEYR;
    volatile uint32_t KEYR;
    volatile uint32_t OPTKEYR;
    volatile uint32_t SR;
    volatile uint32_t CR;
    volatile uint32_t ECCR;
    uint32_t          RESERVED1;
    volatile uint32_t OPTR;
    volatile uint32_t PCROP1SR;
    volatile uint32_t PCROP1ER;
    volatile uint32_t WRP1AR;
    volatile uint32_t WRP1BR;
} McuFlashReg_t;

#define MCU_FLASH ((McuFlashReg_t *)0x40022000)

static uint32_t sMcuFlashSize = 0;

int bMcuFlashInit()
{
    sMcuFlashSize = (*((volatile uint16_t *)0x1FFF75E0)) * 1024;
    return 0;
}

int bMcuFlashUnlock()
{
    int retval      = 0;
    MCU_FLASH->KEYR = FLASH_KEY_1;
    MCU_FLASH->KEYR = FLASH_KEY_2;
    return retval;
}

int bMcuFlashLock()
{
    int      retval = 0;
    uint32_t tmp;
    tmp           = MCU_FLASH->CR | 0x80000000;
    MCU_FLASH->CR = tmp;
    return retval;
}

int bMcuFlashErase(uint32_t raddr, uint32_t pages)
{
    int      retval  = 0;
    int      timeout = 0;
    uint32_t i       = 0;
    uint32_t tmp;

    if (sMcuFlashSize == 0)
    {
        bMcuFlashInit();
    }
    raddr = raddr / FLASH_PAGE_SIZE * FLASH_PAGE_SIZE;
    if ((raddr + (pages * FLASH_PAGE_SIZE)) > (sMcuFlashSize) ||
        ((MCU_FLASH->SR) & (0x00001 << 16)) != 0)
    {
        return -1;
    }

    for (i = 0; i < pages; i++)
    {
        MCU_FLASH->SR = 0xC3FF;
        tmp           = (MCU_FLASH->CR & ~(0xFF << 3));
        MCU_FLASH->CR = (tmp | ((raddr / FLASH_PAGE_SIZE) << 3) | (0x1 << 1) | (0x00001 << 16));

        timeout = FLASH_PER_TIMEOUT;
        while (((MCU_FLASH->SR) & (0x00001 << 16)) != 0 && timeout > 0)
        {
            timeout--;
        }
        MCU_FLASH->SR = 0xC3FF;
        tmp           = MCU_FLASH->CR & ~(0x00000001 << 1);
        MCU_FLASH->CR = tmp;
        if (timeout <= 0)
        {
            retval = -2;
            break;
        }
        raddr += FLASH_PAGE_SIZE;
    }
    return retval;
}

int bMcuFlashWrite(uint32_t raddr, const uint8_t *pbuf, uint32_t len)
{
    int      timeout  = 0;
    uint32_t wdata[2] = {0, 0};
    uint32_t wlen = (len + 7) / 8, i = 0;
    uint32_t tmp;

    if (sMcuFlashSize == 0)
    {
        bMcuFlashInit();
    }
    raddr = FLASH_BASE_ADDR + raddr;
    if (pbuf == NULL || (raddr & 0x7) || (raddr + len) > (sMcuFlashSize + FLASH_BASE_ADDR) ||
        ((MCU_FLASH->SR) & (0x00001 << 16)) != 0)
    {
        return -1;
    }

    for (i = 0; i < wlen; i++)
    {
        wdata[1] = (wdata[1] << 8) | pbuf[i * 8 + 7];
        wdata[1] = (wdata[1] << 8) | pbuf[i * 8 + 6];
        wdata[1] = (wdata[1] << 8) | pbuf[i * 8 + 5];
        wdata[1] = (wdata[1] << 8) | pbuf[i * 8 + 4];

        wdata[0]                            = (wdata[0] << 8) | pbuf[i * 8 + 3];
        wdata[0]                            = (wdata[0] << 8) | pbuf[i * 8 + 2];
        wdata[0]                            = (wdata[0] << 8) | pbuf[i * 8 + 1];
        wdata[0]                            = (wdata[0] << 8) | pbuf[i * 8 + 0];
        MCU_FLASH->SR                       = 0xC3FF;
        tmp                                 = MCU_FLASH->CR | (0x00000001 << 0);
        MCU_FLASH->CR                       = tmp;
        *((volatile uint32_t *)raddr)       = wdata[0];
        *((volatile uint32_t *)(raddr + 4)) = wdata[1];

        timeout = FLASH_PG_TIMEOUT;
        while (((MCU_FLASH->SR) & (0x00001 << 16)) != 0 && timeout > 0)
        {
            timeout--;
        }
        MCU_FLASH->SR = 0xC3FF;
        tmp           = MCU_FLASH->CR & ~(0x00000001 << 0);
        MCU_FLASH->CR = tmp;
        if (timeout <= 0)
        {
            return -2;
        }
        raddr += 8;
    }
    return (wlen * 8);
}

int bMcuFlashRead(uint32_t raddr, uint8_t *pbuf, uint32_t len)
{
    if (sMcuFlashSize == 0)
    {
        bMcuFlashInit();
    }
    if (pbuf == NULL || (raddr + FLASH_BASE_ADDR + len) > (sMcuFlashSize + FLASH_BASE_ADDR))
    {
        return -1;
    }
    raddr = FLASH_BASE_ADDR + raddr;
    memcpy(pbuf, (const uint8_t *)raddr, len);
    return len;
}

uint32_t bMcuFlashSectorSize()
{
    return FLASH_PAGE_SIZE;
}

uint32_t bMcuFlashChipSize()
{
    return sMcuFlashSize;
}

#endif

/************************ Copyright (c) 2021 Bean *****END OF FILE****/
