/**
 *!
 * \file        b_drv_spiflash.c
 * \version     v0.0.2
 * \date        2020/05/08
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */

/*Includes ----------------------------------------------*/
#include "drivers/inc/b_drv_spiflash.h"

#include "drivers/sfud/inc/sfud.h"

/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup SPIFLASH
 * \{
 */

/**
 * \defgroup SPIFLASH_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SPIFLASH_Private_Defines
 * \{
 */
#ifndef SFUD_USING_SFDP
#err "please add sfud"
#endif
/**
 * \}
 */

/**
 * \defgroup SPIFLASH_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SPIFLASH_Private_Variables
 * \{
 */

#ifndef HAL_SPIFLASH_TOTAL_NUMBER
#define HAL_SPIFLASH_TOTAL_NUMBER 1
#endif

HALIF_KEYWORD bSPIFLASH_HalIf_t bSPIFLASH_HalIfTable[HAL_SPIFLASH_TOTAL_NUMBER] = HAL_SPIFLASH_IF;
bSPIFLASH_Driver_t              bSPIFLASH_Driver[HAL_SPIFLASH_TOTAL_NUMBER];

static char       bSPIFlashName[HAL_SPIFLASH_TOTAL_NUMBER][4];
extern sfud_flash flash_table[];  // sfud

/**
 * \}
 */

/**
 * \defgroup SPIFLASH_Private_FunctionPrototypes
 * \{
 */

static void _bSPIFlashSPI_Lock(const sfud_spi *spi)
{
    bHalIntDisable();
}

static void _bSPIFlashSPI_Unlock(const sfud_spi *spi)
{
    bHalIntEnable();
}

/**
 * SPI write data then read data
 */
static sfud_err _bSPIFlashSPI_WR(const sfud_spi *spi, const uint8_t *write_buf, size_t write_size,
                                 uint8_t *read_buf, size_t read_size)
{
    sfud_err           result = SFUD_SUCCESS;
    bSPIFLASH_HalIf_t *_if    = (bSPIFLASH_HalIf_t *)spi->_hal_if;
    bHalQSPICmdInfo_t info;
    uint8_t *ptr = (uint8_t *)write_buf;
    size_t count = 0;
    if ((write_size && write_buf == NULL) || (read_size && read_buf == NULL))
    {
        return SFUD_ERR_WRITE;
    }
    if (_if->is_spi == 0)
    {
        info.instruction = ptr[0];
        info.imode = B_HAL_QSPI_MODE_1LINE;
        count++;
        /* get address */
        if (write_size > 1)
        {
            if (write_size >= 4)
            {
                /* address size is 3 Byte */
                info.address = (ptr[1] << 16) | (ptr[2] << 8) | (ptr[3]);
                info.adsize = B_HAL_QSPI_SIZE_24BIT;
                count += 3;
            }
            else
            {
                return SFUD_ERR_READ;
            }
            info.admode = B_HAL_QSPI_MODE_1LINE;
        }
        else
        {
            /* no address stage */
            info.address = 0;
            info.admode = B_HAL_QSPI_MODE_NONE;
            info.adsize = 0;
        }
        info.alternate = 0;
        info.abmode = B_HAL_QSPI_MODE_NONE;
        info.absize = 0;
        if (write_buf && read_buf)
        {
            /* recv data */
            /* set dummy cycles */
            if (count != write_size)
            {
                info.dummy = (write_size - count) * 8;
            }
            else
            {
                info.dummy = 0;
            }

            /* set recv size */
            info.dmode = B_HAL_QSPI_MODE_1LINE;
            info.dsize = read_size;
            bHalQSPISendCmd(_if->_if._qspi, &info);

            if (read_size != 0)
            {
                if (bHalQSPIReceiveData(_if->_if._qspi, read_buf) < 0)
                {
                    result = SFUD_ERR_READ;
                }
            }
            return result;
        }
        else
        {
            /* send data */
            /* set dummy cycles */
            info.dummy = 0;

            /* determine if there is data to send */
            if (write_size - count > 0)
            {
                info.dmode = B_HAL_QSPI_MODE_1LINE;
            }
            else
            {
                info.dmode = B_HAL_QSPI_MODE_NONE;
            }

            /* set send buf and send size */
            info.dsize = write_size - count;
            bHalQSPISendCmd(_if->_if._qspi, &info);
            
            if (write_size - count > 0)
            {
                if (bHalQSPITransmitData(_if->_if._qspi, (uint8_t *)(ptr + count)) < 0)
                {
                    result = SFUD_ERR_WRITE;
                }
            }
            return result;
        }
    }
    else
    {
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 0);
        if (write_buf && write_size)
        {
            bHalSpiSend(&_if->_if._spi, (uint8_t *)write_buf, write_size);
        }
        if (read_buf && read_size)
        {
            bHalSpiReceive(&_if->_if._spi, (uint8_t *)read_buf, read_size);
        }
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
    }
    return result;
}

#ifdef SFUD_USING_QSPI
/**
 * read flash data by QSPI
 */
static sfud_err _bSPIFlashQSPI_Read(const struct __sfud_spi *spi, uint32_t addr,
                                    sfud_qspi_read_cmd_format *qspi_read_cmd_format,
                                    uint8_t *read_buf, size_t read_size)
{
    sfud_err result = SFUD_SUCCESS;

    /**
     * add your qspi read flash data code
     */

    return result;
}
#endif /* SFUD_USING_QSPI */
/**
 * \}
 */

/**
 * \defgroup SPIFLASH_Private_Functions
 * \{
 */

sfud_err sfud_spi_port_init(sfud_flash *flash)
{
    sfud_err result = SFUD_SUCCESS;
    flash->spi.wr   = _bSPIFlashSPI_WR;
#if SFUD_USING_QSPI
    flash->spi.qspi_read = _bSPIFlashQSPI_Read;  // Required when QSPI mode enable
#endif
    flash->spi.lock      = _bSPIFlashSPI_Lock;
    flash->spi.unlock    = _bSPIFlashSPI_Unlock;
    flash->spi.user_data = NULL;
    flash->retry.delay   = NULL;
    flash->retry.times   = 10000;  // Required
    return result;
}

/****************************************************driver interface******/
static int _bSPIFLASH_Open(bSPIFLASH_Driver_t *pdrv)
{
    bDRV_GET_HALIF(_if, bSPIFLASH_HalIf_t, pdrv);
    uint8_t cmd = 0xab;
    bHalQSPICmdInfo_t info;
    if (_if->is_spi == 0)
    {
        info.abmode = B_HAL_QSPI_MODE_NONE;
        info.absize = 0;
        info.address = 0;
        info.admode = B_HAL_QSPI_MODE_NONE;
        info.adsize = 0;
        info.alternate = 0;
        info.dmode = B_HAL_QSPI_MODE_NONE;
        info.dsize = 0;
        info.dummy = 0;
        info.imode = B_HAL_QSPI_MODE_1LINE;
        info.instruction = cmd;
        bHalQSPISendCmd(_if->_if._qspi, &info);
    }
    else
    {
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 0);
        bHalSpiSend(&_if->_if._spi, &cmd, 1);
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
    }
    bHalDelayUs(10);
    return 0;
}

static int _bSPIFLASH_Close(bSPIFLASH_Driver_t *pdrv)
{
    bDRV_GET_HALIF(_if, bSPIFLASH_HalIf_t, pdrv);
    uint8_t cmd = 0xb9;
    bHalQSPICmdInfo_t info;
    if (_if->is_spi == 0)
    {
        info.abmode = B_HAL_QSPI_MODE_NONE;
        info.absize = 0;
        info.address = 0;
        info.admode = B_HAL_QSPI_MODE_NONE;
        info.adsize = 0;
        info.alternate = 0;
        info.dmode = B_HAL_QSPI_MODE_NONE;
        info.dsize = 0;
        info.dummy = 0;
        info.imode = B_HAL_QSPI_MODE_1LINE;
        info.instruction = cmd;
        bHalQSPISendCmd(_if->_if._qspi, &info);
    }
    else
    {
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 0);
        bHalSpiSend(&_if->_if._spi, &cmd, 1);
        bHalGpioWritePin(_if->_if._spi.cs.port, _if->_if._spi.cs.pin, 1);
    }
    bHalDelayUs(10);
    return 0;
}

static int _bSPIFLASH_ReadBuf(bSPIFLASH_Driver_t *pdrv, uint32_t addr, uint8_t *pbuf, uint16_t len)
{
    sfud_flash *flash = (sfud_flash *)(pdrv->_private._p);
    sfud_read(flash, addr, len, pbuf);
    return len;
}

static int _bSPIFLASH_WriteBuf(bSPIFLASH_Driver_t *pdrv, uint32_t addr, uint8_t *pbuf, uint16_t len)
{
    sfud_flash *flash = (sfud_flash *)(pdrv->_private._p);
    sfud_write(flash, addr, len, pbuf);
    return len;
}

static int _bSPIFLASH_Ctl(bSPIFLASH_Driver_t *pdrv, uint8_t cmd, void *param)
{
    int         retval = -1;
    sfud_flash *flash  = (sfud_flash *)(pdrv->_private._p);
    switch (cmd)
    {
        case bCMD_ERASE_SECTOR:
        {
            if (param)
            {
                bFlashErase_t *perase_param = (bFlashErase_t *)param;
                sfud_erase(flash, perase_param->addr, perase_param->num * flash->chip.erase_gran);
                retval = 0;
            }
        }
        break;
        case bCMD_GET_SECTOR_SIZE:
        {
            if (param)
            {
                ((uint32_t *)param)[0] = flash->chip.erase_gran;
                retval                 = 0;
            }
        }
        break;
        case bCMD_GET_SECTOR_COUNT:
        {
            if (param)
            {
                ((uint32_t *)param)[0] = flash->chip.capacity / flash->chip.erase_gran;
                retval                 = 0;
            }
        }
        break;
    }
    return retval;
}

/**
 * \}
 */

/**
 * \addtogroup SPIFLASH_Exported_Functions
 * \{
 */
int bSPIFLASH_Init()
{
    size_t i = 0, number = sizeof(bSPIFLASH_HalIfTable) / sizeof(bSPIFLASH_HalIf_t);
    int    retval = 0;
    for (i = 0; i < number; i++)
    {
        sprintf(bSPIFlashName[i], "%03d", i);
        flash_table[i].name        = bSPIFlashName[i];
        flash_table[i].spi._hal_if = (void *)&bSPIFLASH_HalIfTable[i];

        bSPIFLASH_Driver[i].init        = bSPIFLASH_Init;
        bSPIFLASH_Driver[i].open        = _bSPIFLASH_Open;
        bSPIFLASH_Driver[i].close       = _bSPIFLASH_Close;
        bSPIFLASH_Driver[i].ctl         = _bSPIFLASH_Ctl;
        bSPIFLASH_Driver[i].read        = _bSPIFLASH_ReadBuf;
        bSPIFLASH_Driver[i].write       = _bSPIFLASH_WriteBuf;
        bSPIFLASH_Driver[i].status      = 0;
        bSPIFLASH_Driver[i]._private._p = &flash_table[i];
        bSPIFLASH_Driver[i]._hal_if     = (void *)&bSPIFLASH_HalIfTable[i];
    }

    for (i = 0; i < number; i++)
    {
        _bSPIFLASH_Open(&bSPIFLASH_Driver[i]);  // wakeup flash
    }

    if (sfud_init() != SFUD_SUCCESS)
    {
        for (i = 0; i < number; i++)
        {
            bSPIFLASH_Driver[i].status = -1;
        }
        retval = -1;
    }

    for (i = 0; i < number; i++)
    {
        _bSPIFLASH_Close(&bSPIFLASH_Driver[i]);  // powerdown flash
    }

    return retval;
}

bDRIVER_REG_INIT(bSPIFLASH_Init);

/**
 * \}
 */

/**
 * \}
 */

/**
 * \}
 */

/**
 * \}
 */

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
