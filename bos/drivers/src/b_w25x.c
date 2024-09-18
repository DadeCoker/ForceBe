/**
 *!
 * \file        b_w25x.c
 * \version     v0.0.1
 * \date        2019/06/05
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 * 
 * Copyright (c) 2019 Bean
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
#include "b_w25x.h"
#include "b_hal.h"
#include "b_utils.h"
#include <string.h>

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

bW25X_Driver_t bW25X_Driver = 
{
        .init = bW25X_Init,
};
static uint16_t bW25X_ID = 0;
static uint32_t bW25X_UserCount = 0;
/**
 * \}
 */
   
/** 
 * \defgroup W25X_Private_FunctionPrototypes
 * \{
 */
extern SPI_HandleTypeDef hspi2;
/**
 * \}
 */
   
/** 
 * \defgroup W25X_Private_Functions
 * \{
 */
static uint8_t _bW25X_SPI_RW(uint8_t dat)
{
	uint8_t retval;
	HAL_SPI_TransmitReceive(&hspi2, &dat, &retval, 1, 0xfff);
    return retval;
}

static void _bW25X_WriteEnable()    
{
    FLASH_CS_RESET();
    _bW25X_SPI_RW(W25X_WRITEENABLE);
    FLASH_CS_SET();
}


static void _bW25X_WaitEnd()    
{
    uint8_t status = 0;
    uint16_t err_count = 0;
    do 
    {
        bHalDelayUS(5000);
        FLASH_CS_RESET();
        _bW25X_SPI_RW(W25X_READSTATUSREG);        
        status = _bW25X_SPI_RW(DUMMY_BYTE);
        FLASH_CS_SET();
        err_count++;
    }
    while((status & WIP_FLAG) == 1 && err_count < 400);
}



static void _bW25X_EraseSector(uint32_t addr)    
{    
    _bW25X_WriteEnable();
    _bW25X_WaitEnd();
    FLASH_CS_RESET();
    _bW25X_SPI_RW(W25X_SECTORERASE);
    if(bW25X_ID == W25Q256)         
    {
        _bW25X_SPI_RW(((addr & 0xFF000000) >> 24));    
    }       
    _bW25X_SPI_RW((addr & 0xFF0000) >> 16);
    _bW25X_SPI_RW((addr & 0xFF00) >> 8);
    _bW25X_SPI_RW(addr & 0xFF);
    FLASH_CS_SET();
    _bW25X_WaitEnd();
}




static uint16_t _bW25X_ReadID()
{
    uint16_t tmp = 0;	  
    FLASH_CS_RESET();				    
    _bW25X_SPI_RW(0x90);    
    _bW25X_SPI_RW(0x00); 	    
    _bW25X_SPI_RW(0x00); 	    
    _bW25X_SPI_RW(0x00); 	 			   
    _bW25X_SPI_RW(0xFF);  
    tmp |= _bW25X_SPI_RW(0xFF);	 
    FLASH_CS_SET();
    return tmp;
}



static void _bW25X_Sleep()
{
    bHalEnterCritical();
    if(bW25X_UserCount > 0)
    {
        bW25X_UserCount--;
        if(bW25X_UserCount == 0)
        {
            FLASH_CS_RESET();
            _bW25X_SPI_RW(W25X_POWERDOWN);
            FLASH_CS_SET();
            bHalDelayUS(10);  
        }
    }
    bHalExitCritical();
}


static void _bW25X_Wakeup()    
{
    bHalEnterCritical();
    if(bW25X_UserCount == 0)
    {
        FLASH_CS_RESET();
        _bW25X_SPI_RW(W25X_RELEASEPOWERDOWN);
        FLASH_CS_SET();
        bHalDelayUS(50);
    }
    bW25X_UserCount++;
    bHalExitCritical();
}

static int _bW25X_Open()
{
    _bW25X_Wakeup();
    return 0;
}

static int _bW25X_Close()
{
    _bW25X_Sleep();
    return 0;
}

static int _bW25X_Erase(uint32_t addr, uint32_t s_num)
{
    uint32_t i = 0;    
    bHalEnterCritical();
    addr = addr & 0xfffff000;
    for(i = 0;i < s_num;i++)
    {       
        _bW25X_EraseSector(addr);
        addr += W25X_SECTOR_SIZE;
    }
    bHalExitCritical();   
    return 0;
}


static int _bW25X_ReadBuf(uint32_t addr, uint8_t * pbuf, uint16_t len)    
{   
    uint16_t len_tmp = len;
    bHalEnterCritical();
    FLASH_CS_RESET();
    _bW25X_SPI_RW(W25X_READDATA);
    if(bW25X_ID == W25Q256)         
    {
        _bW25X_SPI_RW((uint8_t)((addr & 0xFF000000) >> 24));    
    }    
    _bW25X_SPI_RW((addr & 0xFF0000) >> 16);
    _bW25X_SPI_RW((addr & 0xFF00) >> 8);
    _bW25X_SPI_RW(addr & 0xFF);
    
    while(len_tmp--)
    {
        *pbuf = _bW25X_SPI_RW(0xff);
        pbuf++;
    }
    
    FLASH_CS_SET();
    bHalExitCritical();
    return len;
}


static void _bW25X_WritePage(uint8_t * pbuf, uint32_t addr, uint16_t len)
{
    _bW25X_WriteEnable();
    FLASH_CS_RESET();
    _bW25X_SPI_RW(W25X_PAGEPROGRAM);
    if(bW25X_ID == W25Q256)         
    {
        _bW25X_SPI_RW((uint8_t)((addr & 0xFF000000) >> 24));    
    }     
    _bW25X_SPI_RW((addr & 0xFF0000) >> 16);
    _bW25X_SPI_RW((addr & 0xFF00) >> 8);
    _bW25X_SPI_RW(addr & 0xFF);
    if (len > W25X_PAGE_SIZE)
    {
        len = W25X_PAGE_SIZE;
    }
    while(len--)
    {
        _bW25X_SPI_RW(*pbuf);
        pbuf++;
    }
	
    FLASH_CS_SET();
    _bW25X_WaitEnd();
}

static int _bW25X_WriteBuf(uint32_t addr, uint8_t * pbuf, uint16_t len) 
{
    uint16_t first_page_number = W25X_PAGE_SIZE - (uint16_t)(addr % W25X_PAGE_SIZE);
    uint16_t pages, i = 0;
    uint16_t last_page_number;
    
    bHalEnterCritical();
    if(len > first_page_number)
    {
        _bW25X_WritePage(pbuf, addr, first_page_number);
        addr += first_page_number;
        pbuf += first_page_number;
        pages = (len - first_page_number) / W25X_PAGE_SIZE;
        last_page_number = (len - first_page_number) % W25X_PAGE_SIZE;
        for( i = 0;i < pages;i++)
        {
            _bW25X_WritePage(pbuf, addr, W25X_PAGE_SIZE);
            addr += W25X_PAGE_SIZE;
            pbuf += W25X_PAGE_SIZE;
        }
        if(last_page_number != 0)
        {
            _bW25X_WritePage(pbuf, addr, last_page_number);
        }
    }
    else
    {
        _bW25X_WritePage(pbuf, addr, len);
    } 
    bHalExitCritical();
    return 0;
}


static uint8_t _bW25X_ReadSR(uint8_t regno)   
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
	FLASH_CS_RESET();  
	_bW25X_SPI_RW(command);             
	byte=_bW25X_SPI_RW(0Xff);         
	FLASH_CS_SET();                          
	return byte;   
}


static int _bW25X_Ctl(uint8_t cmd, void * param)
{
    int retval = -1;
    switch(cmd)
    {
        case bCMD_ERASE:
            {
                if(param == NULL)
                {
                    return -1;
                }
                bCMD_Struct_t *p = (bCMD_Struct_t *)param;
                retval = _bW25X_Erase(p->param.erase.addr, p->param.erase.num);
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
    uint8_t tmp;
    _bW25X_Wakeup();
    bW25X_ID = _bW25X_ReadID();   
    if(bW25X_ID_IS_VALID(bW25X_ID))
    {
        if(bW25X_ID == W25Q256)
        {
            tmp = _bW25X_ReadSR(3);              
            if((tmp & 0X01) == 0)			      
            {
                FLASH_CS_RESET();
                _bW25X_SPI_RW(W25X_Enable4ByteAddr); 
                FLASH_CS_SET();
            }
        }
        bW25X_Driver.open = _bW25X_Open;
        bW25X_Driver.close = _bW25X_Close;
        bW25X_Driver.ctl = _bW25X_Ctl;
        bW25X_Driver.read = _bW25X_ReadBuf;
        bW25X_Driver.write = _bW25X_WriteBuf;
        _bW25X_Sleep();
        b_log("id:%d, name:%s, size:%dM", bW25X_ID, bW25X_Info[bW25X_ID - W25Q80].str, bW25X_Info[bW25X_ID - W25Q80].size / 1024 / 1024);
        return 0;
    }
    _bW25X_Sleep();
    return -1;
}

/**
 * \}
 */

/**
 * \}
 */

/**
 * \}
 */

/************************ Copyright (c) 2019 Bean *****END OF FILE****/

