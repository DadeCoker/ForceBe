/**
 *!
 * \file        b_hal.c
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
#include "bhal.h"   

/** 
 * \addtogroup B_HAL
 * \{
 */

/** 
 * \addtogroup HAL
 * \{
 */

/** 
 * \defgroup HAL_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup HAL_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup HAL_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup HAL_Private_Variables
 * \{
 */
extern UART_HandleTypeDef huart1;
#define DEBUG_UART_HANDL    huart1
//extern SPI_HandleTypeDef hspi1;

//#define BHAL_SX_SPI             hspi1         


#define STM32F0_UID_ADDR            0x1FFFF7ACUL
#define STM32F1_UID_ADDR            0x1FFFF7E8UL

/**
 * \}
 */
   
/** 
 * \defgroup HAL_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup HAL_Private_Functions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \addtogroup HAL_Exported_Functions
 * \{
 */

int fputc(int c, FILE *p)
{
    uint8_t ch = (uint8_t)(c & 0xff);
    HAL_UART_Transmit(&DEBUG_UART_HANDL, &ch, 1, 0xff);
    return c;
}

#ifdef BHAL_SX_SPI
int bHAL_SPI_Transmit_SX(uint8_t *pbuf, uint16_t len)
{
    HAL_SPI_Transmit(&BHAL_SX_SPI, pbuf, len, 0xfff);
    return 0;
}


int bHAL_SPI_Receive_SX(uint8_t *pbuf, uint16_t len)
{
    HAL_SPI_Receive(&BHAL_SX_SPI, pbuf, len, 0xfff);
    return 0;
}
#else
int bHAL_SPI_Transmit_SX(uint8_t *pbuf, uint16_t len)
{
    return 0;
}


int bHAL_SPI_Receive_SX(uint8_t *pbuf, uint16_t len)
{
    return 0;
}
#endif
///<<< enter/exit critical

void bHalEnterCritical()
{

}    

void bHalExitCritical()
{

}


///<<< delay
void bHalDelayMS(uint16_t xms)
{
    HAL_Delay(xms);
}

void bHalDelayUS(uint16_t xus)
{
    while(xus--)
    {
        __nop();
    }
}

uint32_t bHalGetTick()
{
    return HAL_GetTick();
}

void bHalChipProtect()
{
    FLASH_OBProgramInitTypeDef OBInit;
    __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
    HAL_FLASHEx_OBGetConfig(&OBInit);
    if(OBInit.RDPLevel == OB_RDP_LEVEL_0)
    {
        OBInit.OptionType = OPTIONBYTE_RDP;
        OBInit.RDPLevel = OB_RDP_LEVEL_1;
            
        HAL_FLASH_Unlock();
        HAL_FLASH_OB_Unlock();
        HAL_FLASHEx_OBProgram(&OBInit);
        HAL_FLASH_OB_Lock();
        HAL_FLASH_Lock();
    }
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
}

void bHalFeedWTD()
{
#if _WDG_ENABLE
    HAL_IWDG_Refresh(&hiwdg);
#endif 
}


void bHalGetSTM32MCUID(uint32_t id[3])
{
    id[0] = *(volatile uint32_t*)(STM32F1_UID_ADDR);
    id[1] = *(volatile uint32_t*)(STM32F1_UID_ADDR + 4);
    id[2] = *(volatile uint32_t*)(STM32F1_UID_ADDR + 8);
}



int bHalFlashWrite(uint32_t address, uint8_t *pbuf, uint16_t len)
{
    uint16_t i = 0;
    uint16_t *pdata = (uint16_t *)pbuf;
    if(pbuf == NULL)
    {
        return -1;
    }
    HAL_FLASH_Unlock();
    for(i = 0;i < len / 2;i++)
    {
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, pdata[i]);
            address += 2;
    }    
    HAL_FLASH_Lock();
    return 0;
}

int bHalErasePage(uint32_t addr)
{
    FLASH_EraseInitTypeDef EraseInit;
    uint32_t errcode;
    EraseInit.NbPages = 1;
    EraseInit.PageAddress = addr;
    EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    
    
    HAL_FLASH_Unlock();
    HAL_FLASHEx_Erase(&EraseInit, &errcode);
    HAL_FLASH_Lock();
    return 0;
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


