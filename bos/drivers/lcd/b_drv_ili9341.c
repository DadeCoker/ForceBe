/**
 *!
 * \file        b_drv_ili9341.c
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
#include "b_drv_ili9341.h"

/** 
 * \addtogroup BABYOS
 * \{
 */


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
bILI9341_Driver_t bILI9341_Driver;
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


/******************************************************************/


static void _bILI9341SetCursor(uint16_t Xpos, uint16_t Ypos)
{	 
    bHalLcdWriteCmd(0X2A);
    bHalLcdWriteData(Xpos >> 8);
    bHalLcdWriteData(Xpos & 0XFF);
    bHalLcdWriteCmd(0X2B);
    bHalLcdWriteData(Ypos >> 8);
    bHalLcdWriteData(Ypos & 0XFF);	
    bHalLcdWriteCmd(0X2C); 	
} 	

/************************************************************************************************************driver interface*******/


static int _bILI9341Write(bILI9341_Driver_t *pdrv, uint32_t addr, uint8_t *pbuf, uint16_t len)
{
    uint16_t x = addr % _LCD_X_SIZE;
    uint16_t y = addr / _LCD_X_SIZE;
    bLCD_WriteStruct_t *pcolor = (bLCD_WriteStruct_t *)pbuf;
    if(y >= _LCD_Y_SIZE || pbuf == NULL || len < sizeof(bLCD_WriteStruct_t))
    {
        return -1;
    }   
    _bILI9341SetCursor(x, y);
    bHalLcdWriteData(pcolor->color);
    return 2;
}


/**
 * \}
 */
   
/** 
 * \addtogroup ILI9341_Exported_Functions
 * \{
 */
int bILI9341_Init()
{      
    uint16_t id;
    bHalLcdWriteCmd(0XD3); 
    id = bHalLcdReadData();
    id = bHalLcdReadData();
    id = bHalLcdReadData();
    id <<= 8;
    id |= bHalLcdReadData();
    b_log("id:%x\r\n", id);
    if(id != 0x9341)
    {
        bILI9341_Driver.status = -1;
        return -1;
    }
    bHalLcdWriteCmd(0xCF);  
    bHalLcdWriteData(0x00); 
    bHalLcdWriteData(0xC1); 
    bHalLcdWriteData(0X30); 
    bHalLcdWriteCmd(0xED);  
    bHalLcdWriteData(0x64); 
    bHalLcdWriteData(0x03); 
    bHalLcdWriteData(0X12); 
    bHalLcdWriteData(0X81); 
    bHalLcdWriteCmd(0xE8);  
    bHalLcdWriteData(0x85); 
    bHalLcdWriteData(0x10); 
    bHalLcdWriteData(0x7A); 
    bHalLcdWriteCmd(0xCB);  
    bHalLcdWriteData(0x39); 
    bHalLcdWriteData(0x2C); 
    bHalLcdWriteData(0x00); 
    bHalLcdWriteData(0x34); 
    bHalLcdWriteData(0x02); 
    bHalLcdWriteCmd(0xF7);  
    bHalLcdWriteData(0x20); 
    bHalLcdWriteCmd(0xEA);  
    bHalLcdWriteData(0x00); 
    bHalLcdWriteData(0x00); 
    bHalLcdWriteCmd(0xC0);    
    bHalLcdWriteData(0x1B);   
    bHalLcdWriteCmd(0xC1);   
    bHalLcdWriteData(0x01);  
    bHalLcdWriteCmd(0xC5);  
    bHalLcdWriteData(0x30); 	 
    bHalLcdWriteData(0x30); 	 
    bHalLcdWriteCmd(0xC7);   
    bHalLcdWriteData(0XB7); 
    bHalLcdWriteCmd(0x36);   
    bHalLcdWriteData(0x08); 
    bHalLcdWriteCmd(0x3A);   
    bHalLcdWriteData(0x55); 
    bHalLcdWriteCmd(0xB1);   
    bHalLcdWriteData(0x00);   
    bHalLcdWriteData(0x1A); 
    bHalLcdWriteCmd(0xB6);    
    bHalLcdWriteData(0x0A); 
    bHalLcdWriteData(0xA2); 
    bHalLcdWriteCmd(0xF2);    
    bHalLcdWriteData(0x00); 
    bHalLcdWriteCmd(0x26);    
    bHalLcdWriteData(0x01); 
    bHalLcdWriteCmd(0xE0);  
    bHalLcdWriteData(0x0F); 
    bHalLcdWriteData(0x2A); 
    bHalLcdWriteData(0x28); 
    bHalLcdWriteData(0x08); 
    bHalLcdWriteData(0x0E); 
    bHalLcdWriteData(0x08); 
    bHalLcdWriteData(0x54); 
    bHalLcdWriteData(0XA9); 
    bHalLcdWriteData(0x43); 
    bHalLcdWriteData(0x0A); 
    bHalLcdWriteData(0x0F); 
    bHalLcdWriteData(0x00); 
    bHalLcdWriteData(0x00); 
    bHalLcdWriteData(0x00); 
    bHalLcdWriteData(0x00); 		 
    bHalLcdWriteCmd(0XE1);  
    bHalLcdWriteData(0x00); 
    bHalLcdWriteData(0x15); 
    bHalLcdWriteData(0x17); 
    bHalLcdWriteData(0x07); 
    bHalLcdWriteData(0x11); 
    bHalLcdWriteData(0x06); 
    bHalLcdWriteData(0x2B); 
    bHalLcdWriteData(0x56); 
    bHalLcdWriteData(0x3C); 
    bHalLcdWriteData(0x05); 
    bHalLcdWriteData(0x10); 
    bHalLcdWriteData(0x0F); 
    bHalLcdWriteData(0x3F); 
    bHalLcdWriteData(0x3F); 
    bHalLcdWriteData(0x0F); 
    bHalLcdWriteCmd(0x2B); 
    bHalLcdWriteData(0x00);
    bHalLcdWriteData(0x00);
    bHalLcdWriteData(0x01);
    bHalLcdWriteData(0x3f);
    bHalLcdWriteCmd(0x2A); 
    bHalLcdWriteData(0x00);
    bHalLcdWriteData(0x00);
    bHalLcdWriteData(0x00);
    bHalLcdWriteData(0xef);	 
    bHalLcdWriteCmd(0x11); 
    bUtilDelayMS(120);
    bHalLcdWriteCmd(0x29); 
    bILI9341_Driver.status = 0;
    bILI9341_Driver.close = NULL;
    bILI9341_Driver.read = NULL;
    bILI9341_Driver.ctl = NULL;
    bILI9341_Driver.open = NULL;
    bILI9341_Driver.write = _bILI9341Write;
    return 0;
}

bDRIVER_REG_INIT(bILI9341_Init);

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

