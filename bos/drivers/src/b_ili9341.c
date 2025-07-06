/**
 *!
 * \file        b_ili9341.c
 * \version     v0.0.1
 * \date        2020/03/02
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
#include "b_ili9341.h"
#include "b_utils.h"
/** 
 * \addtogroup B_DRIVER
 * \{
 */

/** 
 * \addtogroup ILI9341
 * \{
 */

/** 
 * \defgroup ILI9341_Private_TypesDefinitions
 * \{
 */
 

/**
 * \}
 */
   
/** 
 * \defgroup ILI9341_Private_Defines
 * \{
 */
#ifndef _LCD_X_SIZE
#define _LCD_X_SIZE     240
#endif

#ifndef _LCD_Y_SIZE
#define _LCD_Y_SIZE     320
#endif
/**
 * \}
 */
   
/** 
 * \defgroup ILI9341_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup ILI9341_Private_Variables
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup ILI9341_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup ILI9341_Private_Functions
 * \{
 */

/****************************************************************/
//static void _ILI9341WriteReg(bILI9341_Driver_t *pdrv, uint16_t cmd, uint16_t dat)
//{
//    ((bILI9341Private_t *)pdrv->_private)->pWriteCmd(cmd);
//	((bILI9341Private_t *)pdrv->_private)->pWriteDat(dat);
//}

/******************************************************************/


static void _bILI9341SetCursor(bILI9341_Driver_t *pdrv, uint16_t Xpos, uint16_t Ypos)
{	 
    bILI9341Private_t *_private = (bILI9341Private_t *)pdrv->_private;
    _private->pWriteCmd(0X2A);
    _private->pWriteDat(Xpos >> 8);
    _private->pWriteDat(Xpos & 0XFF);
    _private->pWriteCmd(0X2B);
    _private->pWriteDat(Ypos >> 8);
    _private->pWriteDat(Ypos & 0XFF);	
    _private->pWriteCmd(0X2C); 	
} 	

/************************************************************************************************************driver interface*******/


static int _bILI9341Write(uint32_t addr, uint8_t *pbuf, uint16_t len)
{
    uint16_t x = addr % _LCD_X_SIZE;
    uint16_t y = addr / _LCD_X_SIZE;
    uint16_t color;
    bILI9341_Driver_t *pdrv;
    if(y >= _LCD_Y_SIZE || pbuf == NULL || len < 2)
    {
        return -1;
    }
    
    if(0 > bDeviceGetCurrentDrv(&pdrv))
    {
        return -1;
    }     
    
    color = ((uint16_t *)pbuf)[0];
    _bILI9341SetCursor(pdrv, x, y);
    ((bILI9341Private_t *)pdrv->_private)->pWriteDat(color);
    return 0;
}


/**
 * \}
 */
   
/** 
 * \addtogroup ILI9341_Exported_Functions
 * \{
 */
int bILI9341_Init(bILI9341_Driver_t *pdrv)
{      
    uint16_t id;
    bILI9341Private_t *_private = (bILI9341Private_t *)pdrv->_private;
    _private->pWriteCmd(0XD3); 
    id = _private->pReadDat();
    id = _private->pReadDat();
    id = _private->pReadDat();
    id <<= 8;
    id |= _private->pReadDat();
    b_log("id:%x\r\n", id);
    if(id != 0x9341)
    {
        return -1;
    }
    _private->pWriteCmd(0xCF);  
    _private->pWriteDat(0x00); 
    _private->pWriteDat(0xC1); 
    _private->pWriteDat(0X30); 
    _private->pWriteCmd(0xED);  
    _private->pWriteDat(0x64); 
    _private->pWriteDat(0x03); 
    _private->pWriteDat(0X12); 
    _private->pWriteDat(0X81); 
    _private->pWriteCmd(0xE8);  
    _private->pWriteDat(0x85); 
    _private->pWriteDat(0x10); 
    _private->pWriteDat(0x7A); 
    _private->pWriteCmd(0xCB);  
    _private->pWriteDat(0x39); 
    _private->pWriteDat(0x2C); 
    _private->pWriteDat(0x00); 
    _private->pWriteDat(0x34); 
    _private->pWriteDat(0x02); 
    _private->pWriteCmd(0xF7);  
    _private->pWriteDat(0x20); 
    _private->pWriteCmd(0xEA);  
    _private->pWriteDat(0x00); 
    _private->pWriteDat(0x00); 
    _private->pWriteCmd(0xC0);    
    _private->pWriteDat(0x1B);   
    _private->pWriteCmd(0xC1);   
    _private->pWriteDat(0x01);  
    _private->pWriteCmd(0xC5);  
    _private->pWriteDat(0x30); 	 
    _private->pWriteDat(0x30); 	 
    _private->pWriteCmd(0xC7);   
    _private->pWriteDat(0XB7); 
    _private->pWriteCmd(0x36);   
    _private->pWriteDat(0x08); 
    _private->pWriteCmd(0x3A);   
    _private->pWriteDat(0x55); 
    _private->pWriteCmd(0xB1);   
    _private->pWriteDat(0x00);   
    _private->pWriteDat(0x1A); 
    _private->pWriteCmd(0xB6);    
    _private->pWriteDat(0x0A); 
    _private->pWriteDat(0xA2); 
    _private->pWriteCmd(0xF2);    
    _private->pWriteDat(0x00); 
    _private->pWriteCmd(0x26);    
    _private->pWriteDat(0x01); 
    _private->pWriteCmd(0xE0);  
    _private->pWriteDat(0x0F); 
    _private->pWriteDat(0x2A); 
    _private->pWriteDat(0x28); 
    _private->pWriteDat(0x08); 
    _private->pWriteDat(0x0E); 
    _private->pWriteDat(0x08); 
    _private->pWriteDat(0x54); 
    _private->pWriteDat(0XA9); 
    _private->pWriteDat(0x43); 
    _private->pWriteDat(0x0A); 
    _private->pWriteDat(0x0F); 
    _private->pWriteDat(0x00); 
    _private->pWriteDat(0x00); 
    _private->pWriteDat(0x00); 
    _private->pWriteDat(0x00); 		 
    _private->pWriteCmd(0XE1);  
    _private->pWriteDat(0x00); 
    _private->pWriteDat(0x15); 
    _private->pWriteDat(0x17); 
    _private->pWriteDat(0x07); 
    _private->pWriteDat(0x11); 
    _private->pWriteDat(0x06); 
    _private->pWriteDat(0x2B); 
    _private->pWriteDat(0x56); 
    _private->pWriteDat(0x3C); 
    _private->pWriteDat(0x05); 
    _private->pWriteDat(0x10); 
    _private->pWriteDat(0x0F); 
    _private->pWriteDat(0x3F); 
    _private->pWriteDat(0x3F); 
    _private->pWriteDat(0x0F); 
    _private->pWriteCmd(0x2B); 
    _private->pWriteDat(0x00);
    _private->pWriteDat(0x00);
    _private->pWriteDat(0x01);
    _private->pWriteDat(0x3f);
    _private->pWriteCmd(0x2A); 
    _private->pWriteDat(0x00);
    _private->pWriteDat(0x00);
    _private->pWriteDat(0x00);
    _private->pWriteDat(0xef);	 
    _private->pWriteCmd(0x11); 
    bHalDelayMS(120);
    _private->pWriteCmd(0x29); 
    
    pdrv->close = NULL;
    pdrv->read = NULL;
    pdrv->ctl = NULL;
    pdrv->open = NULL;
    pdrv->write = _bILI9341Write;
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

