/**
 *!
 * \file        b_drv_w25x.c
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
#include "b_drv_w25x.h"
#include "b_utils.h"
#include "b_hal.h"
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


static uint16_t bW25X_ID = 0;
static uint32_t bW25X_UserCount = 0;

bW25X_Driver_t bW25X_Driver = {
    .init = bW25X_Init,
};

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
#ifdef HAL_W25X_CS_PORT
#define W25X_CS_SET()       bHalGPIO_WritePin(HAL_W25X_CS_PORT, HAL_W25X_CS_PIN, 1)
#define W25X_CS_RESET()     bHalGPIO_WritePin(HAL_W25X_CS_PORT, HAL_W25X_CS_PIN, 0)
#else
#define W25X_CS_SET()       
#define W25X_CS_RESET()     
#endif



static int _bW25X_SPI_R(uint8_t *pbuf, uint16_t len)
{
#ifdef HAL_W25X_SPI
    return bHalSPI_Receive(HAL_W25X_SPI, pbuf, len);
#else    
	return bHalQSPI_Receive(HAL_W25X_QSPI, pbuf, len);
#endif    
}

static int _bW25X_SPI_W(uint8_t *pbuf, uint16_t len)
{
#ifdef HAL_W25X_SPI    
	return bHalSPI_Send(HAL_W25X_SPI, pbuf, len);
#else
    return bHalSPI_Send(HAL_W25X_QSPI, pbuf, len);
#endif    
}

static int _bW25X_WriteCmd(uint8_t cmd, uint32_t addr, uint32_t addr_mode, uint32_t addr_size_bit, uint32_t data_mode, uint16_t nb_data)
{
#ifdef HAL_W25X_SPI   
    uint8_t cmd_table[5], i = 0, bytes = addr_size_bit / 8;
    int j = 0;
    cmd_table[i++] = cmd;
    if(addr_mode)
    {
        for(j = (bytes - 1);j >= 0;j--)
        {
            cmd_table[i++] = (addr >> (j * 8)) & 0xff;
        }
    }
    bHalSPI_Send(HAL_W25X_SPI, cmd_table, i);
#else
    bHalQSPI_Command(HAL_W25X_QSPI, cmd, addr, addr_mode, addr_size_bit, data_mode, nb_data);
#endif    
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////

static void _bW25X_WriteEnable()    
{
    W25X_CS_RESET();
    _bW25X_WriteCmd(W25X_WRITEENABLE, 0, 0, 0, 0, 0);
    W25X_CS_SET();
}


static void _bW25X_WaitEnd()    
{
    uint8_t status = 0;
    uint16_t err_count = 0;
    do 
    {
        bUtilDelayUS(5000);
        W25X_CS_RESET();
        _bW25X_WriteCmd(W25X_READSTATUSREG, 0, 0, 0, 1, 1);
        _bW25X_SPI_R(&status, 1); 
        W25X_CS_SET();
        err_count++;
    }
    while((status & WIP_FLAG) == 1 && err_count < 400);
}



static void _bW25X_EraseSector(uint32_t addr)    
{    
    _bW25X_WriteEnable();
    _bW25X_WaitEnd();
    W25X_CS_RESET();    
    if(bW25X_ID == W25Q256)         
    {
        _bW25X_WriteCmd(W25X_SECTORERASE, addr, 1, 32, 0, 0);  
    } 
    else
    {
        _bW25X_WriteCmd(W25X_SECTORERASE, addr, 1, 24, 0, 0); 
    }
    W25X_CS_SET();
    _bW25X_WaitEnd();
}




static uint16_t _bW25X_ReadID()
{
    uint16_t tmp = 0;	  
    W25X_CS_RESET();
    _bW25X_WriteCmd(W25X_MANUFACTDEVICEID, 0, 1, 24, 1, 2); 
    _bW25X_SPI_R((uint8_t *)&tmp, 2); 
    W25X_CS_SET();
    return ((tmp & 0xff00) >> 8);
}



static void _bW25X_Sleep()
{
    bHalEnterCritical();
    if(bW25X_UserCount > 0)
    {
        bW25X_UserCount--;
        if(bW25X_UserCount == 0)
        {
            W25X_CS_RESET();
            _bW25X_WriteCmd(W25X_POWERDOWN, 0, 0, 0, 0, 0); 
            W25X_CS_SET();
            bUtilDelayUS(10);  
        }
    }
    bHalExitCritical();
}


static void _bW25X_Wakeup()    
{
    bHalEnterCritical();
    if(bW25X_UserCount == 0)
    {
        W25X_CS_RESET();
        _bW25X_WriteCmd(W25X_RELEASEPOWERDOWN, 0, 0, 0, 0, 0); 
        W25X_CS_SET();
        bUtilDelayUS(50);
    }
    bW25X_UserCount++;
    bHalExitCritical();
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


static void _bW25X_WritePage(uint8_t * pbuf, uint32_t addr, uint16_t len)
{
    if (len > W25X_PAGE_SIZE)
    {
        len = W25X_PAGE_SIZE;
    }
    _bW25X_WriteEnable();
    W25X_CS_RESET();

    if(bW25X_ID == W25Q256)         
    {
        _bW25X_WriteCmd(W25X_PAGEPROGRAM, addr, 1, 32, 1, len);    
    }   
    else
    {
        _bW25X_WriteCmd(W25X_PAGEPROGRAM, addr, 1, 24, 1, len); 
    }
    _bW25X_SPI_W(pbuf, len);
    W25X_CS_SET();
    _bW25X_WaitEnd();
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
	W25X_CS_RESET(); 
    _bW25X_WriteCmd(command, 0, 0, 0, 1, 1); 
    _bW25X_SPI_R(&byte, 1);    
	W25X_CS_SET();                        
	return byte;   
}

/*********************************************************************************driver interface******/
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


static int _bW25X_ReadBuf(uint32_t addr, uint8_t * pbuf, uint16_t len)    
{   
    bHalEnterCritical();
    W25X_CS_RESET();
    if(bW25X_ID == W25Q256)         
    {
        _bW25X_WriteCmd(W25X_READDATA, addr, 1, 32, 1, len);   
    }   
    else
    {
        _bW25X_WriteCmd(W25X_READDATA, addr, 1, 24, 1, len);
    }        
    _bW25X_SPI_R(pbuf, len);
    W25X_CS_SET();
    bHalExitCritical();
    return len;
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
                W25X_CS_RESET(); 
                _bW25X_WriteCmd(W25X_Enable4ByteAddr, 0, 0, 0, 0, 0);  
                W25X_CS_SET();  
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

/**
 * \}
 */

/************************ Copyright (c) 2019 Bean *****END OF FILE****/

