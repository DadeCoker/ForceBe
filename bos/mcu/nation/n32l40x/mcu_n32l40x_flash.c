/**
 *!
 * \file        mcu_n32l40x_flash.c
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

#if (_MCU_PLATFORM == 2001)

#define FLASH_BASE_ADDR (0x8000000UL)
#define FLASH_END_ADDR (0x801FFFFUL)
#define FLASH_PAGE_SIZE (2048)

#define FLASH_KEY_1 (0x45670123UL)
#define FLASH_KEY_2 (0xCDEF89ABUL)
#define FLASH_PER_TIMEOUT (0x000B0000UL)
#define FLASH_PG_TIMEOUT (0x00002000UL)

//                 Register Address
#define FLASH_REG_BASE (0x40022000UL)
#define FLASH_REG_KEY ((volatile uint32_t *)(FLASH_REG_BASE + 0x04))
#define FLASH_REG_STS ((volatile uint32_t *)(FLASH_REG_BASE + 0x0C))
#define FLASH_REG_CTRL ((volatile uint32_t *)(FLASH_REG_BASE + 0x10))
#define FLASH_REG_ADD ((volatile uint32_t *)(FLASH_REG_BASE + 0x14))

static int _FlashInit()
{
    return 0;
}

static int _FlashUnlock()
{
    int retval     = 0;
    *FLASH_REG_KEY = FLASH_KEY_1;
    *FLASH_REG_KEY = FLASH_KEY_2;
    return retval;
}

static int _FlashLock()
{
    int retval = 0;
    *FLASH_REG_CTRL |= (0x00000001 << 7);
    return retval;
}

static int _FlashErase(uint32_t raddr, uint8_t pages)
{
    int      retval  = 0;
    uint32_t timeout = 0;
    uint8_t  i       = 0;

    raddr = FLASH_BASE_ADDR + raddr;
    raddr = raddr / FLASH_PAGE_SIZE * FLASH_PAGE_SIZE;
    if ((raddr + (pages * FLASH_PAGE_SIZE)) > FLASH_END_ADDR || ((*FLASH_REG_STS) & 0x01) != 0)
    {
        return -1;
    }

    for (i = 0; i < pages; i++)
    {
        *FLASH_REG_STS |= 0xFC;
        *FLASH_REG_CTRL |= (0x00000001 << 1);
        *FLASH_REG_ADD = raddr;
        *FLASH_REG_CTRL |= (0x00000001 << 6);
        timeout = FLASH_PER_TIMEOUT;
        while (((*FLASH_REG_STS) & 0x01) != 0 && timeout != 0)
        {
            timeout--;
        }
        if (timeout == 0)
        {
            retval = -2;
            break;
        }
        raddr += FLASH_PAGE_SIZE;
    }
    return retval;
}

static int _FlashWrite(uint32_t raddr, const uint8_t *pbuf, uint16_t len)
{
    uint32_t wdata   = 0;
    uint32_t timeout = 0;
    uint16_t wlen = (len + 3) / 4, i = 0;
    raddr = FLASH_BASE_ADDR + raddr;
    if (pbuf == NULL || (raddr & 0x3) || (raddr + len) > FLASH_END_ADDR ||
        ((*FLASH_REG_STS) & 0x01) != 0)
    {
        return -1;
    }

    for (i = 0; i < wlen; i++)
    {
        wdata = (wdata << 8) | pbuf[i * 4 + 3];
        wdata = (wdata << 8) | pbuf[i * 4 + 2];
        wdata = (wdata << 8) | pbuf[i * 4 + 1];
        wdata = (wdata << 8) | pbuf[i * 4 + 0];

        *FLASH_REG_STS |= 0xFC;
        *FLASH_REG_CTRL |= (0x00000001 << 0);
        *((volatile uint32_t *)raddr) = wdata;
        timeout                       = FLASH_PG_TIMEOUT;
        while (((*FLASH_REG_STS) & 0x01) != 0 && timeout != 0)
        {
            timeout--;
        }
        if (timeout == 0)
        {
            return -2;
        }
        raddr += 4;
    }
    return (wlen * 4);
}

static int _FlashRead(uint32_t raddr, uint8_t *pbuf, uint16_t len)
{
    if (pbuf == NULL || (raddr + FLASH_BASE_ADDR + len) > FLASH_END_ADDR)
    {
        return -1;
    }
    raddr = FLASH_BASE_ADDR + raddr;
    memcpy(pbuf, (const uint8_t *)raddr, len);
    return len;
}

bHalFlashDriver_t bHalFlashDriver = {
    .pFlashInit   = _FlashInit,
    .pFlashUnlock = _FlashUnlock,
    .pFlashLock   = _FlashLock,
    .pFlashErase  = _FlashErase,
    .pFlashWrite  = _FlashWrite,
    .pFlashRead   = _FlashRead,
};

#endif

/************************ Copyright (c) 2021 Bean *****END OF FILE****/
