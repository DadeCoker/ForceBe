/**
 *!
 * \file        b_drv_ili9320.c
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
#include "b_drv_ili9320.h"

/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup ILI9320
 * \{
 */

/**
 * \defgroup ILI9320_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup ILI9320_Private_Defines
 * \{
 */
#ifndef _LCD_X_SIZE
#define _LCD_X_SIZE 240
#endif

#ifndef _LCD_Y_SIZE
#define _LCD_Y_SIZE 320
#endif
/**
 * \}
 */

/**
 * \defgroup ILI9320_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup ILI9320_Private_Variables
 * \{
 */
bILI9320_Driver_t bILI9320_Driver;
/**
 * \}
 */

/**
 * \defgroup ILI9320_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup ILI9320_Private_Functions
 * \{
 */

/******************************************************************/

static void _bILI9320WriteReg(uint16_t reg, uint16_t dat)
{
    bHalLcdWriteCmd(reg);
    bHalLcdWriteData(dat);
}

static void _bILI9320SetCursor(uint16_t Xpos, uint16_t Ypos)
{
    _bILI9320WriteReg(0X20, Xpos);
    _bILI9320WriteReg(0X21, Ypos);
    bHalLcdWriteCmd(0X22);
}

/************************************************************************************************************driver
 * interface*******/

static int _bILI9320Write(bILI9320_Driver_t *pdrv, uint32_t addr, uint8_t *pbuf, uint16_t len)
{
    uint16_t            x      = addr % _LCD_X_SIZE;
    uint16_t            y      = addr / _LCD_X_SIZE;
    bLCD_WriteStruct_t *pcolor = (bLCD_WriteStruct_t *)pbuf;
    if (y >= _LCD_Y_SIZE || pbuf == NULL || len < sizeof(bLCD_WriteStruct_t))
    {
        return -1;
    }
    _bILI9320SetCursor(x, y);
    bHalLcdWriteData(pcolor->color);
    return 2;
}

/**
 * \}
 */

/**
 * \addtogroup ILI9320_Exported_Functions
 * \{
 */
int bILI9320_Init()
{
    uint16_t id;
    bHalLcdWriteCmd(0x0);
    id = bHalLcdReadData();
    b_log("id:%x\r\n", id);
    if (id != 0x9320)
    {
        bILI9320_Driver.status = -1;
        return -1;
    }
    _bILI9320WriteReg(0x00, 0x0000);
    _bILI9320WriteReg(0x01, 0x0100);  // Driver Output Contral.
    _bILI9320WriteReg(0x02, 0x0700);  // LCD Driver Waveform Contral.
    _bILI9320WriteReg(0x03, 0x1010);  // Entry Mode Set.

    _bILI9320WriteReg(0x04, 0x0000);    // Scalling Contral.
    _bILI9320WriteReg(0x08, 0x0202);    // Display Contral 2.(0x0207)
    _bILI9320WriteReg(0x09, 0x0000);    // Display Contral 3.(0x0000)
    _bILI9320WriteReg(0x0a, 0x0000);    // Frame Cycle Contal.(0x0000)
    _bILI9320WriteReg(0x0c, (1 << 0));  // Extern Display Interface Contral 1.(0x0000)
    _bILI9320WriteReg(0x0d, 0x0000);    // Frame Maker Position.
    _bILI9320WriteReg(0x0f, 0x0000);    // Extern Display Interface Contral 2.
    bUtilDelayMS(50);
    _bILI9320WriteReg(0x07, 0x0101);  // Display Contral.
    bUtilDelayMS(50);
    _bILI9320WriteReg(
        0x10, (1 << 12) | (0 << 8) | (1 << 7) | (1 << 6) | (0 << 4));  // Power Control 1.(0x16b0)
    _bILI9320WriteReg(0x11, 0x0007);                                   // Power Control 2.(0x0001)
    _bILI9320WriteReg(0x12, (1 << 8) | (1 << 4) | (0 << 0));           // Power Control 3.(0x0138)
    _bILI9320WriteReg(0x13, 0x0b00);                                   // Power Control 4.
    _bILI9320WriteReg(0x29, 0x0000);                                   // Power Control 7.

    _bILI9320WriteReg(0x2b, (1 << 14) | (1 << 4));
    _bILI9320WriteReg(0x50, 0);  // Set X Star

    _bILI9320WriteReg(0x51, 239);  // Set Y Star
    _bILI9320WriteReg(0x52, 0);    // Set Y End.t.
    _bILI9320WriteReg(0x53, 319);  //

    _bILI9320WriteReg(0x60, 0x2700);  // Driver Output Control.
    _bILI9320WriteReg(0x61, 0x0001);  // Driver Output Control.
    _bILI9320WriteReg(0x6a, 0x0000);  // Vertical Srcoll Control.

    _bILI9320WriteReg(0x80, 0x0000);  // Display Position? Partial Display 1.
    _bILI9320WriteReg(0x81, 0x0000);  // RAM Address Start? Partial Display 1.
    _bILI9320WriteReg(0x82, 0x0000);  // RAM Address End-Partial Display 1.
    _bILI9320WriteReg(0x83, 0x0000);  // Displsy Position? Partial Display 2.
    _bILI9320WriteReg(0x84, 0x0000);  // RAM Address Start? Partial Display 2.
    _bILI9320WriteReg(0x85, 0x0000);  // RAM Address End? Partial Display 2.

    _bILI9320WriteReg(0x90, (0 << 7) | (16 << 0));  // Frame Cycle Contral.(0x0013)
    _bILI9320WriteReg(0x92, 0x0000);                // Panel Interface Contral 2.(0x0000)
    _bILI9320WriteReg(0x93, 0x0001);                // Panel Interface Contral 3.
    _bILI9320WriteReg(0x95, 0x0110);                // Frame Cycle Contral.(0x0110)
    _bILI9320WriteReg(0x97, (0 << 8));              //
    _bILI9320WriteReg(0x98, 0x0000);                // Frame Cycle Contral.
    _bILI9320WriteReg(0x07, 0x0173);                //(0x0173)
    bILI9320_Driver.status = 0;
    bILI9320_Driver.close  = NULL;
    bILI9320_Driver.read   = NULL;
    bILI9320_Driver.ctl    = NULL;
    bILI9320_Driver.open   = NULL;
    bILI9320_Driver.write  = _bILI9320Write;
    return 0;
}

bDRIVER_REG_INIT(bILI9320_Init);

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
