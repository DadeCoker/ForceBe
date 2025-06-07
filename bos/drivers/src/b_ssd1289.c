/**
 *!
 * \file        b_ssd1289.c
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
#include "b_ssd1289.h"
/** 
 * \addtogroup B_DRIVER
 * \{
 */

/** 
 * \addtogroup SSD1289
 * \{
 */

/** 
 * \defgroup SSD1289_Private_TypesDefinitions
 * \{
 */
 

/**
 * \}
 */
   
/** 
 * \defgroup SSD1289_Private_Defines
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
 * \defgroup SSD1289_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SSD1289_Private_Variables
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup SSD1289_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup SSD1289_Private_Functions
 * \{
 */

/****************************************************************/
static void _SSD1289WriteReg(bSSD1289_Driver_t *pdrv, uint16_t cmd, uint16_t dat)
{
    ((bSSD1289Private_t *)pdrv->_private)->pWriteCmd(cmd);
	((bSSD1289Private_t *)pdrv->_private)->pWriteDat(dat);
}

/******************************************************************/

static void _bSSD1289SetWindow(bSSD1289_Driver_t *pdrv, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    _SSD1289WriteReg(pdrv, 0x0044, ((x2 << 8) | x1));
	_SSD1289WriteReg(pdrv, 0x0045, y1);
	_SSD1289WriteReg(pdrv, 0x0046, y2);
	_SSD1289WriteReg(pdrv, 0x004E, x1);
	_SSD1289WriteReg(pdrv, 0x004F, y1);
	((bSSD1289Private_t *)pdrv->_private)->pWriteCmd(0x0022);
}

static void _bSSD1289FillFrame(bSSD1289_Driver_t *pdrv, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	uint32_t i = 0;
    uint32_t j = (x2 - x1 + 1) * (y2 - y1 + 1);
	_bSSD1289SetWindow(pdrv, x1, y1, x2, y2);
	for (i = 0; i < j; i++)
    {
        ((bSSD1289Private_t *)pdrv->_private)->pWriteDat(color);
    }
}


/************************************************************************************************************driver interface*******/


static int _bSSD1289Write(uint32_t addr, uint8_t *pbuf, uint16_t len)
{
    uint16_t x = addr % _LCD_X_SIZE;
    uint16_t y = addr / _LCD_X_SIZE;
    uint16_t color;
    bSSD1289_Driver_t *pdrv;
    if(y >= _LCD_Y_SIZE || pbuf == NULL || len < 2)
    {
        return -1;
    }
    
    if(0 > bDeviceGetCurrentDrv(&pdrv))
    {
        return -1;
    }     
    
    color = ((uint16_t *)pbuf)[0];
    _bSSD1289SetWindow(pdrv, x, y, x, y);
    ((bSSD1289Private_t *)pdrv->_private)->pWriteDat(color);
    return 0;
}

static int _bSSD1289Ctl(uint8_t cmd, void * param)
{
    bSSD1289_Driver_t *pdrv;
    if(0 > bDeviceGetCurrentDrv(&pdrv))
    {
        return -1;
    }
    
    switch(cmd)
    {
        case bCMD_FILL_FRAME:
            {
                bCMD_Struct_t *p = (bCMD_Struct_t *)param;
                _bSSD1289FillFrame(pdrv, p->param.fill_frame.x1, p->param.fill_frame.y1
                                ,p->param.fill_frame.x2, p->param.fill_frame.y2
                                ,p->param.fill_frame.color);
            }
            break;
    }
    return 0;
}

/**
 * \}
 */
   
/** 
 * \addtogroup SSD1289_Exported_Functions
 * \{
 */
int bSSD1289_Init(bSSD1289_Driver_t *pdrv)
{      
	_SSD1289WriteReg(pdrv, 0x0007, 0x0021);
	_SSD1289WriteReg(pdrv, 0x0000, 0x0001);
	_SSD1289WriteReg(pdrv, 0x0007, 0x0023);
	_SSD1289WriteReg(pdrv, 0x0010, 0x0000);
	_SSD1289WriteReg(pdrv, 0x0007, 0x0033);
	_SSD1289WriteReg(pdrv, 0x0011, 0x6838); 
	_SSD1289WriteReg(pdrv, 0x0002, 0x0600);
    _SSD1289WriteReg(pdrv, 0x0001, 0x2B3F); 
 
    pdrv->close = NULL;
    pdrv->read = NULL;
    pdrv->ctl = _bSSD1289Ctl;
    pdrv->open = NULL;
    pdrv->write = _bSSD1289Write;
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

