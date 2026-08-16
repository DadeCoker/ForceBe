/**
 *!
 * \file        b_drv_w25x.c
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
#include "b_drv_w25x.h"
/** 
 * \addtogroup BABYOS
 * \{
 */


/** 
 * \addtogroup B_DRIVER
 * \{
 */

/** 
 * \addtogroup W25X
 * \{
 */

/** 
 * \defgroup W25X_Private_TypesDefinitions
 * \{
 */
typedef struct
{
    uint16_t id;
    uint32_t size;
    uint8_t  str[8];
}bW25X_Info_t;   

/**
 * \}
 */
   
/** 
 * \defgroup W25X_Private_Defines
 * \{
 */
#if HAL_W25X_QSPI_EN
#define W25X_CS_RESET(_if)        
#define W25X_CS_SET(_if)          
#else
#define W25X_CS_RESET(_if)         bHalGPIO_WritePin((_if)->cs.port, (_if)->cs.pin, 0)
#define W25X_CS_SET(_if)           bHalGPIO_WritePin((_if)->cs.port, (_if)->cs.pin, 1)
#endif
/**
 * \}
 */
   
/** 
 * \defgroup W25X_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup W25X_Private_Variables
 * \{
 */


static const bW25X_Info_t bW25X_Info[] = 
{
    {W25Q80, 0X00100000, "w25q80"},
    {W25Q16, 0X00200000, "w25q16"},
    {W25Q32, 0X00400000, "w25q32"},
    {W25Q64, 0X00800000, "w25q64"},  
    {W25Q128,0X01000000, "w25q128"},
    {W25Q256,0X02000000, "w25q256"},     
}; 

static uint32_t bW25X_UserCount = 0;
static const bW25X_HalIf_t bW25X_HalIfTable[] = HAL_W25X_IF;
bW25X_Driver_t bW25X_Driver[sizeof(bW25X_HalIfTable) / sizeof(bW25X_HalIf_t)];

/**
 * \}
 */
   
/** 
 * \defgroup W25X_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup W25X_Private_Functions
 * \{
 */
static int _bW25X_SPI_R(bW25X_HalIf_t *_if, uint8_t *pbuf, uint16_t len)
{
    int retval = 0;
#if HAL_W25X_QSPI_EN
    retval = bHalQSPI_Receive(_if->qspi, pbuf, len);
#else
    retval = bHalSPI_Receive(_if->spi, pbuf, len);
#endif    
    return retval;
}

static int _bW25X_SPI_W(bW25X_HalIf_t *_if, uint8_t *pbuf, uint16_t len)
{
    int retval = 0;
#if HAL_W25X_QSPI_EN
    retval = bHalQSPI_Send(_if->qspi, pbuf, len);
#else
    retval = bHalSPI_Send(_if->spi, pbuf, len);
#endif     
    return retval;      
}

static int _bW25X_WriteCmd(bW25X_HalIf_t *_if, uint8_t cmd, uint32_t addr, uint32_t addr_mode, uint32_t addr_size_bit, uint32_t data_mode, uint16_t nb_data)
{
    uint8_t cmd_table[5], i = 0, bytes = addr_size_bit / 8;
    int j = 0;
    
#if HAL_W25X_QSPI_EN
    bHalQSPI_Command(_if->qspi, cmd, addr, addr_mode, addr_size_bit, data_mode, nb_data);
#else
    cmd_table[i++] = cmd;
    if(addr_mode)
    {
        for(j = (bytes - 1);j >= 0;j--)
        {
            cmd_table[i++] = (addr >> (j * 8)) & 0xff;
        }
    }
    bHalSPI_Send(_if->spi, cmd_table, i);
#endif    
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////

static void _bW25X_WriteEnable(bW25X_HalIf_t *_if)    
{
    W25X_CS_RESET(_if);
    _bW25X_WriteCmd(_if, W25X_WRITEENABLE, 0, 0, 0, 0, 0);
    W25X_CS_SET(_if);
}


static void _bW25X_WaitEnd(bW25X_HalIf_t *_if)    
{
    uint8_t status = 0;
    uint16_t err_count = 0;
    do 
    {
        bUtilDelayUS(5000);
        W25X_CS_RESET(_if);  
        _bW25X_WriteCmd(_if, W25X_READSTATUSREG, 0, 0, 0, 1, 1);
        _bW25X_SPI_R(_if, &status, 1); 
        W25X_CS_SET(_if);  
        err_count++;
    }
    while((status & WIP_FLAG) == 1 && err_count < 400);
}



static void _bW25X_EraseSector(bW25X_Driver_t *pdrv, uint32_t addr)    
{    
    bDRV_GET_HALIF(_if, bW25X_HalIf_t, pdrv);
    _bW25X_WriteEnable(_if);
    _bW25X_WaitEnd(_if);
    W25X_CS_RESET(_if);    
    if(pdrv->_private.v == W25Q256)         
    {
        _bW25X_WriteCmd(_if, W25X_SECTORERASE, addr, 1, 32, 0, 0);  
    } 
    else
    {
        _bW25X_WriteCmd(_if, W25X_SECTORERASE, addr, 1, 24, 0, 0); 
    }
    W25X_CS_SET(_if);
    _bW25X_WaitEnd(_if);
}




static uint16_t _bW25X_ReadID(bW25X_HalIf_t *_if)
{
    uint16_t tmp = 0;	  
    W25X_CS_RESET(_if);
    _bW25X_WriteCmd(_if, W25X_MANUFACTDEVICEID, 0, 1, 24, 1, 2); 
    _bW25X_SPI_R(_if, (uint8_t *)&tmp, 2); 
    W25X_CS_SET(_if);
    return ((tmp & 0xff00) >> 8);
}



static void _bW25X_Sleep(bW25X_HalIf_t *_if)
{
    bHalEnterCritical();
    if(bW25X_UserCount > 0)
    {
        bW25X_UserCount--;
        if(bW25X_UserCount == 0)
        {
            W25X_CS_RESET(_if);
            _bW25X_WriteCmd(_if, W25X_POWERDOWN, 0, 0, 0, 0, 0); 
            W25X_CS_SET(_if);
            bUtilDelayUS(10);  
        }
    }
    bHalExitCritical();
}


static void _bW25X_Wakeup(bW25X_HalIf_t *_if)    
{
    bHalEnterCritical();
    if(bW25X_UserCount == 0)
    {
        W25X_CS_RESET(_if);
        _bW25X_WriteCmd(_if, W25X_RELEASEPOWERDOWN, 0, 0, 0, 0, 0); 
        W25X_CS_SET(_if);
        bUtilDelayUS(50);
    }
    bW25X_UserCount++;
    bHalExitCritical();
}

static int _bW25X_Erase(bW25X_Driver_t *pdrv, uint32_t addr, uint32_t s_num)
{
    uint32_t i = 0;    
    bHalEnterCritical();
    addr = addr & 0xfffff000;
    for(i = 0;i < s_num;i++)
    {       
        _bW25X_EraseSector(pdrv, addr);
        addr += W25X_SECTOR_SIZE;
    }
    bHalExitCritical();   
    return 0;
}


static void _bW25X_WritePage(bW25X_Driver_t *pdrv, uint8_t * pbuf, uint32_t addr, uint16_t len)
{
    bDRV_GET_HALIF(_if, bW25X_HalIf_t, pdrv);
    if (len > W25X_PAGE_SIZE)
    {
        len = W25X_PAGE_SIZE;
    }
    _bW25X_WriteEnable(_if);
    W25X_CS_RESET(_if);

    if(pdrv->_private.v == W25Q256)         
    {
        _bW25X_WriteCmd(_if, W25X_PAGEPROGRAM, addr, 1, 32, 1, len);    
    }   
    else
    {
        _bW25X_WriteCmd(_if, W25X_PAGEPROGRAM, addr, 1, 24, 1, len); 
    }
    _bW25X_SPI_W(_if, pbuf, len);
    W25X_CS_SET(_if);
    _bW25X_WaitEnd(_if);
}

static uint8_t _bW25X_ReadSR(bW25X_HalIf_t *_if, uint8_t regno)   
{  
	uint8_t byte=0,command=0; 
    switch(regno)
    {
        case 1:
            command = W25X_ReadStatusReg1;  
            break;
        case 2:
            command = W25X_ReadStatusReg2;   
            break;
        case 3:
            command = W25X_ReadStatusReg3;    
            break;
        default:
            command=W25X_ReadStatusReg1;    
            break;
    }    
	W25X_CS_RESET(_if);
    _bW25X_WriteCmd(_if, command, 0, 0, 0, 1, 1); 
    _bW25X_SPI_R(_if, &byte, 1);    
	W25X_CS_SET(_if);                     
	return byte;   
}

static void _bW25X_32bitAddr(bW25X_HalIf_t *_if)
{
    uint8_t tmp;
    tmp = _bW25X_ReadSR(_if, 3);
    if((tmp & 0X01) == 0)			      
    {
        W25X_CS_RESET(_if); 
        _bW25X_WriteCmd(_if, W25X_Enable4ByteAddr, 0, 0, 0, 0, 0);  
        W25X_CS_SET(_if);  
    }
}    



/*********************************************************************************driver interface******/
static int _bW25X_Open(bW25X_Driver_t *pdrv)
{   
    _bW25X_Wakeup(pdrv->_hal_if);
    return 0;
}

static int _bW25X_Close(bW25X_Driver_t *pdrv)
{   
    _bW25X_Sleep(pdrv->_hal_if);
    return 0;
}


static int _bW25X_ReadBuf(bW25X_Driver_t *pdrv, uint32_t addr, uint8_t * pbuf, uint16_t len)    
{   
    bDRV_GET_HALIF(_if, bW25X_HalIf_t, pdrv);
    bHalEnterCritical();
    W25X_CS_RESET(_if);
    if(pdrv->_private.v == W25Q256)         
    {
        _bW25X_WriteCmd(pdrv->_hal_if, W25X_READDATA, addr, 1, 32, 1, len);   
    }   
    else
    {
        _bW25X_WriteCmd(pdrv->_hal_if, W25X_READDATA, addr, 1, 24, 1, len);
    }        
    _bW25X_SPI_R(pdrv->_hal_if, pbuf, len);
    W25X_CS_SET(_if);
    bHalExitCritical();
    return len;
}




static int _bW25X_WriteBuf(bW25X_Driver_t *pdrv, uint32_t addr, uint8_t * pbuf, uint16_t len) 
{
    uint16_t first_page_number = W25X_PAGE_SIZE - (uint16_t)(addr % W25X_PAGE_SIZE);
    uint16_t pages, i = 0;
    uint16_t last_page_number;
    
    bHalEnterCritical();
    if(len > first_page_number)
    {
        _bW25X_WritePage(pdrv, pbuf, addr, first_page_number);
        addr += first_page_number;
        pbuf += first_page_number;
        pages = (len - first_page_number) / W25X_PAGE_SIZE;
        last_page_number = (len - first_page_number) % W25X_PAGE_SIZE;
        for( i = 0;i < pages;i++)
        {
            _bW25X_WritePage(pdrv, pbuf, addr, W25X_PAGE_SIZE);
            addr += W25X_PAGE_SIZE;
            pbuf += W25X_PAGE_SIZE;
        }
        if(last_page_number != 0)
        {
            _bW25X_WritePage(pdrv, pbuf, addr, last_page_number);
        }
    }
    else
    {
        _bW25X_WritePage(pdrv, pbuf, addr, len);
    } 
    bHalExitCritical();
    return len;
}





static int _bW25X_Ctl(bW25X_Driver_t *pdrv, uint8_t cmd, void * param)
{
    int retval = -1;    
    
    switch(cmd)
    {
        case bCMD_ERASE_SECTOR:
            {
                if(param == NULL)
                {
                    return -1;
                }
                bCMD_Erase_t *p = (bCMD_Erase_t *)param;
                retval = _bW25X_Erase(pdrv, p->addr, p->num);
            }
            break;
        default:
            break;
    }
    return retval;
}



/**
 * \}
 */
   
/** 
 * \addtogroup W25X_Exported_Functions
 * \{
 */
int bW25X_Init()
{
    uint8_t i = 0, num_drv = sizeof(bW25X_HalIfTable) / sizeof(bW25X_HalIf_t);
    uint16_t id;
    
    for(i = 0;i < num_drv;i++)
    {
        bW25X_Driver[i]._hal_if = (void *)&bW25X_HalIfTable[i];
        _bW25X_Wakeup(bW25X_Driver[i]._hal_if);
        id = _bW25X_ReadID(bW25X_Driver[i]._hal_if);
        if(bW25X_ID_IS_VALID(id))
        {
            if(id == W25Q256)
            {
                _bW25X_32bitAddr(bW25X_Driver[i]._hal_if);
            }
            bW25X_Driver[i].status = 0;
            bW25X_Driver[i]._private.v = id;
            bW25X_Driver[i].open = _bW25X_Open;
            bW25X_Driver[i].close = _bW25X_Close;
            bW25X_Driver[i].ctl = _bW25X_Ctl;
            bW25X_Driver[i].read = _bW25X_ReadBuf;
            bW25X_Driver[i].write = _bW25X_WriteBuf;
        }
        else
        {
            bW25X_Driver[i].status = -1;
        }
        _bW25X_Sleep(bW25X_Driver[i]._hal_if);
        b_log("id:%d, name:%s, size:%dM\r\n", id, bW25X_Info[id - W25Q80].str, bW25X_Info[id - W25Q80].size / 1024 / 1024);
    }
    return 0;
}


bDRIVER_REG_INIT(bW25X_Init);


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

