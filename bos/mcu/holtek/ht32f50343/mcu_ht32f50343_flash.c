/**
 *!
 * \file        mcu_ht32f50343_flash.c
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

#if (MCU_PLATFORM == 8001)

#define FLASH_BASE_ADDR (0x0000000UL)

#ifndef FLASH_PAGE_SIZE
#define FLASH_PAGE_SIZE (1024)
#endif

#define FLASH_KEY_1 (0x45670123UL)
#define FLASH_KEY_2 (0xCDEF89ABUL)
#define FLASH_PER_TIMEOUT (0x000B0000UL)
#define FLASH_PG_TIMEOUT (0x00002000UL)

typedef struct
{
    volatile uint32_t TADR;
    volatile uint32_t WRDR;
    uint32_t          RESERVED0[1];
    volatile uint32_t OCMR;
    volatile uint32_t OPCR;
    volatile uint32_t OIER;
    volatile uint32_t OISR;
    uint32_t          RESERVED1[1];
    volatile uint32_t PPSR[4];
    volatile uint32_t CPSR;
    uint32_t          RESERVED2[51];
    volatile uint32_t VMCR;
    uint32_t          RESERVED3[31];
    volatile uint32_t MDID;
    volatile uint32_t PNSR;
    volatile uint32_t PSSR;
    volatile uint32_t DID;
    uint32_t          RESERVED4[28];
    volatile uint32_t CFCR;
    uint32_t          RESERVED6[67];
    volatile uint32_t CID[4];
} McuFlashReg_t;

#define MCU_FLASH ((McuFlashReg_t *)0x40080000)

static uint32_t sMcuFlashSize = 0x10000;

int bMcuFlashInit()
{
    return 0;
}

int bMcuFlashUnlock()
{
    int retval = 0;
    return retval;
}

int bMcuFlashLock()
{
    int retval = 0;
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
    if ((raddr + (pages * FLASH_PAGE_SIZE)) > (sMcuFlashSize))
    {
        return -1;
    }

    for (i = 0; i < pages; i++)
    {
        B_WRITE_REG(MCU_FLASH->TADR, raddr + FLASH_BASE_ADDR);
        B_WRITE_REG(MCU_FLASH->OCMR, 0x00000008);
        B_WRITE_REG(MCU_FLASH->OPCR, 0x00000014);

        timeout = FLASH_PER_TIMEOUT;
        while (((MCU_FLASH->OPCR) & 0x0000000C) != 0x0000000C && timeout > 0)
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

int bMcuFlashWrite(uint32_t raddr, const uint8_t *pbuf, uint32_t len)
{
    int      timeout = 0;
    uint32_t wdata   = 0;
    uint32_t wlen = (len + 3) / 4, i = 0;
    uint32_t tmp;

    if (sMcuFlashSize == 0)
    {
        bMcuFlashInit();
    }
    raddr = FLASH_BASE_ADDR + raddr;
    if (pbuf == NULL || (raddr & 0x3) || (raddr + len) > (sMcuFlashSize + FLASH_BASE_ADDR))
    {
        return -1;
    }

    for (i = 0; i < wlen; i++)
    {
        wdata = (wdata << 8) | pbuf[i * 4 + 3];
        wdata = (wdata << 8) | pbuf[i * 4 + 2];
        wdata = (wdata << 8) | pbuf[i * 4 + 1];
        wdata = (wdata << 8) | pbuf[i * 4 + 0];

        B_WRITE_REG(MCU_FLASH->TADR, raddr);
        B_WRITE_REG(MCU_FLASH->WRDR, wdata);
        B_WRITE_REG(MCU_FLASH->OCMR, 0x00000004);
        B_WRITE_REG(MCU_FLASH->OPCR, 0x00000014);

        timeout = FLASH_PG_TIMEOUT;
        while (((MCU_FLASH->OPCR) & 0x0000000C) != 0x0000000C && timeout > 0)
        {
            timeout--;
        }
        if (timeout <= 0)
        {
            return -2;
        }
        raddr += 4;
    }
    return (wlen * 4);
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
