/**
 *!
 * \file        mcu_stm32f10x_flash.c
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

#if (_MCU_PLATFORM == 1001 || _MCU_PLATFORM == 1002 || _MCU_PLATFORM == 1003 || \
     _MCU_PLATFORM == 1004)

#define FLASH_BASE_ADDR (0x8000000UL)

#if (_MCU_PLATFORM == 1003 || _MCU_PLATFORM == 1004)
#define FLASH_PAGE_SIZE (2048)
#else
#define FLASH_PAGE_SIZE (1024)
#endif

#if (_MCU_PLATFORM == 1001)
#define FLASH_MAX_SIZE (32 * 1024)
#elif (_MCU_PLATFORM == 1002)
#define FLASH_MAX_SIZE (128 * 1024)
#elif (_MCU_PLATFORM == 1003)
#define FLASH_MAX_SIZE (512 * 1024)
#else
#define FLASH_MAX_SIZE (256 * 1024)
#endif

#define FLASH_KEY_1 (0x45670123UL)
#define FLASH_KEY_2 (0xCDEF89ABUL)
#define FLASH_PER_TIMEOUT (0x000B0000UL)
#define FLASH_PG_TIMEOUT (0x00002000UL)

typedef struct
{
    volatile uint32_t ACR;
    volatile uint32_t KEYR;
    volatile uint32_t OPTKEYR;
    volatile uint32_t SR;
    volatile uint32_t CR;
    volatile uint32_t AR;
    volatile uint32_t RESERVED;
    volatile uint32_t OBR;
    volatile uint32_t WRPR;
} McuFlashReg_t;

#define MCU_FLASH ((McuFlashReg_t *)0x40022000)

static int _FlashInit()
{
    return 0;
}

static int _FlashUnlock()
{
    int retval      = 0;
    MCU_FLASH->KEYR = FLASH_KEY_1;
    MCU_FLASH->KEYR = FLASH_KEY_2;
    return retval;
}

static int _FlashLock()
{
    int retval = 0;
    MCU_FLASH->CR |= (0x00000001 << 7);
    return retval;
}

static int _FlashErase(uint32_t raddr, uint8_t pages)
{
    int     retval  = 0;
    int     timeout = 0;
    uint8_t i       = 0;

    raddr = FLASH_BASE_ADDR + raddr;
    raddr = raddr / FLASH_PAGE_SIZE * FLASH_PAGE_SIZE;
    if ((raddr + (pages * FLASH_PAGE_SIZE)) > (FLASH_BASE_ADDR + FLASH_MAX_SIZE) ||
        ((MCU_FLASH->SR) & 0x01) != 0)
    {
        return -1;
    }

    for (i = 0; i < pages; i++)
    {
        MCU_FLASH->SR |= 0xFC;
        MCU_FLASH->CR |= (0x00000001 << 1);
        MCU_FLASH->AR = raddr;
        MCU_FLASH->CR |= (0x00000001 << 6);
        timeout = FLASH_PER_TIMEOUT;
        while (((MCU_FLASH->SR) & 0x01) != 0 && timeout > 0)
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
    return retval;
}

static int _FlashWrite(uint32_t raddr, const uint8_t *pbuf, uint16_t len)
{
    int      timeout = 0;
    uint16_t wdata   = 0;
    uint16_t wlen = (len + 1) / 2, i = 0;
    raddr = FLASH_BASE_ADDR + raddr;
    if (pbuf == NULL || (raddr & 0x1) || (raddr + len) > (FLASH_MAX_SIZE + FLASH_BASE_ADDR) ||
        ((MCU_FLASH->SR) & 0x01) != 0)
    {
        return -1;
    }

    for (i = 0; i < wlen; i++)
    {
        wdata = (wdata << 8) | pbuf[i * 2 + 1];
        wdata = (wdata << 8) | pbuf[i * 2 + 0];

        MCU_FLASH->SR |= 0xFC;
        MCU_FLASH->CR |= (0x00000001 << 0);
        *((volatile uint32_t *)raddr) = wdata;

        timeout = FLASH_PG_TIMEOUT;
        while (((MCU_FLASH->SR) & 0x01) != 0 && timeout > 0)
        {
            timeout--;
        }
        if (timeout <= 0)
        {
            return -2;
        }
        raddr += 2;
    }
    return (wlen * 2);
}

static int _FlashRead(uint32_t raddr, uint8_t *pbuf, uint16_t len)
{
    if (pbuf == NULL || (raddr + FLASH_BASE_ADDR + len) > (FLASH_MAX_SIZE + FLASH_BASE_ADDR))
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
