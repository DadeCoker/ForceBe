/**
 *!
 * \file        mcu_stm32l4xx_sdio.c
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SSPIL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */

/*Includes ----------------------------------------------*/
#include "b_config.h"
#include "hal/inc/b_hal.h"

#if (MCU_PLATFORM == 1201)

//         Register Address

#define SDIO_BASE_ADDR (0x40012800)

typedef struct
{
    volatile uint32_t       POWER;
    volatile uint32_t       CLKCR;
    volatile uint32_t       ARG;
    volatile uint32_t       CMD;
    volatile const uint32_t RESPCMD;
    volatile const uint32_t RESP1;
    volatile const uint32_t RESP2;
    volatile const uint32_t RESP3;
    volatile const uint32_t RESP4;
    volatile uint32_t       DTIMER;
    volatile uint32_t       DLEN;
    volatile uint32_t       DCTRL;
    volatile const uint32_t DCOUNT;
    volatile const uint32_t STA;
    volatile uint32_t       ICR;
    volatile uint32_t       MASK;
    uint32_t                RESERVED0[2];
    volatile const uint32_t FIFOCNT;
    uint32_t                RESERVED1[13];
    volatile uint32_t       FIFO;
} McuSDIOReg_t;

#define MCU_SDIO ((McuSDIOReg_t *)SDIO_BASE_ADDR)

#define RESPONSE_NONE (0)
#define RESPONSE_SHORT (1)
#define RESPONSE_LONG (3)

#define CMD_FLAGS ((uint32_t)(0xC5))
#define DATA_FLAGS ((uint32_t)(0x53A))
#define ALL_FLAGS ((uint32_t)(0x4005FF))

#define CMD_READ_SINGLE_BLOCK ((uint8_t)17U)
#define CMD_READ_MULT_BLOCK ((uint8_t)18U)
#define CMD_WRITE_SINGLE_BLOCK ((uint8_t)24U)                                
#define CMD_WRITE_MULT_BLOCK ((uint8_t)25U) 


#define CMDTIMEOUT ((uint32_t)5000U)
#define MAXERASETIMEOUT ((uint32_t)63000U)
#define STOPTRANSFERTIMEOUT ((uint32_t)100000000U)

static int _bMcuSDIOSendCmd(McuSDIOReg_t *psdio, uint32_t cmd, uint32_t arg, uint32_t response)
{
    uint32_t tmpreg = 0;
    psdio->ARG      = arg;
    tmpreg |= (uint32_t)((cmd << 0) | (response << 6) | (0x1 << 10));
    B_MODIFY_REG(psdio->CMD, 0xFFF, tmpreg);
    return 0;
}

static int _bMcuSDIOGetCmdResp(McuSDIOReg_t *psdio, uint8_t cmd, uint32_t timeout)
{
    uint32_t sta_reg;
    uint32_t count = bHalGetSysTick();
    do
    {
        if (bHalGetSysTick() - count >= MS2TICKS(timeout))
        {
            return -1;
        }
        sta_reg = psdio->STA;
    } while (((sta_reg & (0x45)) == 0U) || ((sta_reg & (0x1 << 11)) != 0U));

    if (sta_reg & (0x04))  // timeout
    {
        psdio->ICR = 0x04;
        return -1;
    }

    if (sta_reg & (0x01))  // crcfail
    {
        psdio->ICR = 0x01;
        return -1;
    }
    psdio->ICR = CMD_FLAGS;

    if ((uint8_t)(psdio->RESPCMD) != cmd)
    {
        return -1;
    }
    return 0;
}

int bMcuSDIOReadBlocks(const bHalSDIONumber_t sd, uint8_t *pdata, uint32_t addr, uint32_t xblocks)
{
    McuSDIOReg_t *psdio  = MCU_SDIO;
    uint32_t      tmpreg = 0, data = 0, tick = 0;
    int           retval = 0, i = 0;
    uint32_t      dataremaining = 0;
    uint8_t      *tempbuff      = pdata;

    psdio->DCTRL  = 0;
    psdio->DTIMER = 0xFFFFFFFF;
    psdio->DLEN   = xblocks * 512;
    tmpreg |= (uint32_t)(0x93);
    B_MODIFY_REG(psdio->DCTRL, 0x4F3, tmpreg);

    if (xblocks > 1)
    {
        _bMcuSDIOSendCmd(psdio, CMD_READ_MULT_BLOCK, addr, RESPONSE_SHORT);
        retval = _bMcuSDIOGetCmdResp(psdio, CMD_READ_MULT_BLOCK, CMDTIMEOUT);
    }
    else
    {
        _bMcuSDIOSendCmd(psdio, CMD_READ_SINGLE_BLOCK, addr, RESPONSE_SHORT);
        retval = _bMcuSDIOGetCmdResp(psdio, CMD_READ_SINGLE_BLOCK, CMDTIMEOUT);
    }

    if (retval < 0)
    {
        psdio->ICR = ALL_FLAGS;
        return -1;
    }

    dataremaining = xblocks * 512;
    tick          = bHalGetSysTick();
    while (!((psdio->STA) & (0x12A)))
    {
        if (((psdio->STA) & (0x8000)) && (dataremaining > 0))
        {
            for (i = 0; i < 8; i++)
            {
                data      = psdio->FIFO;
                *tempbuff = (uint8_t)(data & 0xFFU);
                tempbuff++;
                dataremaining--;
                *tempbuff = (uint8_t)((data >> 8U) & 0xFFU);
                tempbuff++;
                dataremaining--;
                *tempbuff = (uint8_t)((data >> 16U) & 0xFFU);
                tempbuff++;
                dataremaining--;
                *tempbuff = (uint8_t)((data >> 24U) & 0xFFU);
                tempbuff++;
                dataremaining--;
            }
        }

        if ((bHalGetSysTick() - tick) >= MS2TICKS(6000))
        {
            psdio->ICR = ALL_FLAGS;
            return -1;
        }
    }

    tick       = bHalGetSysTick();
    while ((((psdio->STA) & (0x200000))) && (dataremaining > 0U))
    {
        data      = psdio->FIFO;
        *tempbuff = (uint8_t)(data & 0xFFU);
        tempbuff++;
        dataremaining--;
        *tempbuff = (uint8_t)((data >> 8U) & 0xFFU);
        tempbuff++;
        dataremaining--;
        *tempbuff = (uint8_t)((data >> 16U) & 0xFFU);
        tempbuff++;
        dataremaining--;
        *tempbuff = (uint8_t)((data >> 24U) & 0xFFU);
        tempbuff++;
        dataremaining--;

        if ((bHalGetSysTick() - tick) >= MS2TICKS(6000))
        {
            psdio->ICR = ALL_FLAGS;
            return -1;
        }
    }
    psdio->ICR = ALL_FLAGS;
    return 0;
}

int bMcuSDIOWriteBlocks(const bHalSDIONumber_t sd, uint8_t *pdata, uint32_t addr, uint32_t xblocks)
{
    McuSDIOReg_t *psdio  = MCU_SDIO;
    uint32_t      tmpreg = 0, data = 0, tick = 0;
    int           retval = 0, i = 0;
    uint32_t      dataremaining = 0;
    uint8_t      *tempbuff      = pdata;

    psdio->DCTRL  = 0;
    psdio->DTIMER = 0xFFFFFFFF;
    psdio->DLEN   = xblocks * 512;
    tmpreg |= (uint32_t)(0x91);
    B_MODIFY_REG(psdio->DCTRL, 0x4F3, tmpreg);

    if (xblocks > 1)
    {
        _bMcuSDIOSendCmd(psdio, CMD_WRITE_MULT_BLOCK, addr, RESPONSE_SHORT);
        retval = _bMcuSDIOGetCmdResp(psdio, CMD_WRITE_MULT_BLOCK, CMDTIMEOUT);
    }
    else
    {
        _bMcuSDIOSendCmd(psdio, CMD_WRITE_SINGLE_BLOCK, addr, RESPONSE_SHORT);
        retval = _bMcuSDIOGetCmdResp(psdio, CMD_WRITE_SINGLE_BLOCK, CMDTIMEOUT);
    }

    if (retval < 0)
    {
        psdio->ICR = ALL_FLAGS;
        return -1;
    }

    dataremaining = xblocks * 512;
    tick          = bHalGetSysTick();
    while (!((psdio->STA) & (0x11A)))
    {
        if (((psdio->STA) & (0x4000)) && (dataremaining > 0))
        {
            for (i = 0; i < 8; i++)
            {
              data = (uint32_t)(*tempbuff);
              tempbuff++;
              dataremaining--;
              data |= ((uint32_t)(*tempbuff) << 8U);
              tempbuff++;
              dataremaining--;
              data |= ((uint32_t)(*tempbuff) << 16U);
              tempbuff++;
              dataremaining--;
              data |= ((uint32_t)(*tempbuff) << 24U);
              tempbuff++;
              dataremaining--;
              psdio->FIFO = data;
            }
        }

        if ((bHalGetSysTick() - tick) >= MS2TICKS(6000))
        {
            psdio->ICR = ALL_FLAGS;
            return -1;
        }
    }
    psdio->ICR = ALL_FLAGS;
    return 0;
}

int bMcuSDIOErase(const bHalSDIONumber_t sd, uint32_t addr, uint32_t xblocks)
{
    return 0;
}

#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
