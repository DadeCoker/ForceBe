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
typedef struct
{
    sfud_flash sflash;
} bSpiFlashPrivate_t;

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

#define DRIVER_NAME SPIFLASH

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

bDRIVER_HALIF_TABLE(bSPIFLASH_HalIf_t, DRIVER_NAME);
static bSpiFlashPrivate_t bSpiFlashPrivate[bDRIVER_HALIF_NUM(bSPIFLASH_HalIf_t, DRIVER_NAME)];
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
    sfud_err            result = SFUD_SUCCESS;
    bDriverInterface_t *pdrv   = (bDriverInterface_t *)spi->user_data;
    bHalQSPICmdInfo_t   info;
    uint8_t            *ptr   = (uint8_t *)write_buf;
    size_t              count = 0;
    bDRIVER_GET_HALIF(_if, bSPIFLASH_HalIf_t, pdrv);
    if ((write_size && write_buf == NULL) || (read_size && read_buf == NULL))
    {
        return SFUD_ERR_WRITE;
    }
    if (_if->is_spi == 0)
    {
        info.instruction = ptr[0];
        info.imode       = B_HAL_QSPI_MODE_1LINE;
        count++;
        /* get address */
        if (write_size > 1)
        {
            if (write_size >= 4)
            {
                /* address size is 3 Byte */
                info.address = (ptr[1] << 16) | (ptr[2] << 8) | (ptr[3]);
                info.adsize  = B_HAL_QSPI_SIZE_24BIT;
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
            info.admode  = B_HAL_QSPI_MODE_NONE;
            info.adsize  = 0;
        }
        info.alternate = 0;
        info.abmode    = B_HAL_QSPI_MODE_NONE;
        info.absize    = 0;
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
/**
 * \}
 */

/**
 * \defgroup SPIFLASH_Private_Functions
 * \{
 */

sfud_err sfud_spi_port_init(sfud_flash *flash)
{
    sfud_err result    = SFUD_SUCCESS;
    flash->spi.wr      = _bSPIFlashSPI_WR;
    flash->spi.lock    = _bSPIFlashSPI_Lock;
    flash->spi.unlock  = _bSPIFlashSPI_Unlock;
    flash->retry.delay = NULL;
    flash->retry.times = 0xFFFFFFFF;  // Required
    return result;
}

/****************************************************driver interface******/
static int _bSPIFLASH_Open(bDriverInterface_t *pdrv)
{
    bDRIVER_GET_HALIF(_if, bSPIFLASH_HalIf_t, pdrv);
    uint8_t           cmd = 0xab;
    bHalQSPICmdInfo_t info;
    if (_if->is_spi == 0)
    {
        info.abmode      = B_HAL_QSPI_MODE_NONE;
        info.absize      = 0;
        info.address     = 0;
        info.admode      = B_HAL_QSPI_MODE_NONE;
        info.adsize      = 0;
        info.alternate   = 0;
        info.dmode       = B_HAL_QSPI_MODE_NONE;
        info.dsize       = 0;
        info.dummy       = 0;
        info.imode       = B_HAL_QSPI_MODE_1LINE;
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

static int _bSPIFLASH_Close(bDriverInterface_t *pdrv)
{
    bDRIVER_GET_HALIF(_if, bSPIFLASH_HalIf_t, pdrv);
    uint8_t           cmd = 0xb9;
    bHalQSPICmdInfo_t info;
    if (_if->is_spi == 0)
    {
        info.abmode      = B_HAL_QSPI_MODE_NONE;
        info.absize      = 0;
        info.address     = 0;
        info.admode      = B_HAL_QSPI_MODE_NONE;
        info.adsize      = 0;
        info.alternate   = 0;
        info.dmode       = B_HAL_QSPI_MODE_NONE;
        info.dsize       = 0;
        info.dummy       = 0;
        info.imode       = B_HAL_QSPI_MODE_1LINE;
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

static int _bSPIFLASH_ReadBuf(bDriverInterface_t *pdrv, uint32_t addr, uint8_t *pbuf, uint32_t len)
{
    sfud_flash *flash = &((bSpiFlashPrivate_t *)(pdrv->_private._p))->sflash;
    sfud_read(flash, addr, len, pbuf);
    return len;
}

static int _bSPIFLASH_WriteBuf(bDriverInterface_t *pdrv, uint32_t addr, uint8_t *pbuf, uint32_t len)
{
    sfud_flash *flash = &((bSpiFlashPrivate_t *)(pdrv->_private._p))->sflash;
    sfud_write(flash, addr, len, pbuf);
    return len;
}

static int _bSPIFLASH_Ctl(bDriverInterface_t *pdrv, uint8_t cmd, void *param)
{
    int         retval = -1;
    sfud_flash *flash  = &((bSpiFlashPrivate_t *)(pdrv->_private._p))->sflash;
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
int bSPIFLASH_Init(bDriverInterface_t *pdrv)
{
    int                 retval = 0;
    bSpiFlashPrivate_t *p_data;
    bDRIVER_STRUCT_INIT(pdrv, DRIVER_NAME, bSPIFLASH_Init);
    pdrv->open        = _bSPIFLASH_Open;
    pdrv->close       = _bSPIFLASH_Close;
    pdrv->ctl         = _bSPIFLASH_Ctl;
    pdrv->read        = _bSPIFLASH_ReadBuf;
    pdrv->write       = _bSPIFLASH_WriteBuf;
    pdrv->_private._p = (void *)&bSpiFlashPrivate[pdrv->drv_no];

    p_data                       = (bSpiFlashPrivate_t *)(pdrv->_private._p);
    p_data->sflash.index         = pdrv->drv_no;
    p_data->sflash.name          = pdrv->pdes;
    p_data->sflash.spi.user_data = (void *)pdrv;

    _bSPIFLASH_Open(pdrv);

    if (sfud_device_init(&p_data->sflash) != SFUD_SUCCESS)
    {
        retval = -1;
    }
    _bSPIFLASH_Close(pdrv);
    return retval;
}

bDRIVER_REG_INIT(B_DRIVER_SPIFLASH, bSPIFLASH_Init);

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
