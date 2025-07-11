/**
 *!
 * \file        b_hal_lcd.c
 * \version     v0.0.1
 * \date        2020/03/25
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SLCDL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
   
/*Includes ----------------------------------------------*/
#include "b_hal.h" 
/** 
 * \addtogroup B_HAL
 * \{
 */

/** 
 * \addtogroup LCD
 * \{
 */

/** 
 * \defgroup LCD_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup LCD_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup LCD_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup LCD_Private_Variables
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup LCD_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup LCD_Private_Functions
 * \{
 */


/**
 * \}
 */
   
/** 
 * \addtogroup LCD_Exported_Functions
 * \{
 */
 
void bHalLcdWriteData(uint16_t dat)
{
#ifdef HAL_LCD_FSMC_ADDR
    ((bHalLcdAddr_t *)HAL_LCD_FSMC_ADDR)->dat = dat;
#else
    bHalGPIO_WritePin(HAL_LCD_RS_PORT, HAL_LCD_RS_PIN, 1);
    bHalGPIO_WritePin(HAL_LCD_RD_PORT, HAL_LCD_RD_PIN, 1);
    bHalGPIO_WritePin(HAL_LCD_CS_PORT, HAL_LCD_CS_PIN, 0);
    bHalGPIO_Write(HAL_LCD_DAT_PORT, dat);
    bHalGPIO_WritePin(HAL_LCD_WR_PORT, HAL_LCD_WR_PIN, 0);
    bHalGPIO_WritePin(HAL_LCD_WR_PORT, HAL_LCD_WR_PIN, 1);    
	bHalGPIO_WritePin(HAL_LCD_CS_PORT, HAL_LCD_CS_PIN, 1);
#endif    
}

void bHalLcdWriteCmd(uint16_t cmd)
{
#ifdef HAL_LCD_FSMC_ADDR
    ((bHalLcdAddr_t *)HAL_LCD_FSMC_ADDR)->reg = cmd;
#else
    bHalGPIO_WritePin(HAL_LCD_RS_PORT, HAL_LCD_RS_PIN, 0);
    bHalGPIO_WritePin(HAL_LCD_RD_PORT, HAL_LCD_RD_PIN, 1);
    bHalGPIO_WritePin(HAL_LCD_CS_PORT, HAL_LCD_CS_PIN, 0);
    bHalGPIO_Write(HAL_LCD_DAT_PORT, cmd);
    bHalGPIO_WritePin(HAL_LCD_WR_PORT, HAL_LCD_WR_PIN, 0);
    bHalGPIO_WritePin(HAL_LCD_WR_PORT, HAL_LCD_WR_PIN, 1);    
	bHalGPIO_WritePin(HAL_LCD_CS_PORT, HAL_LCD_CS_PIN, 1);
#endif   
}

uint16_t bHalLcdReadData()
{
    uint16_t dat;
#ifdef HAL_LCD_FSMC_ADDR
    dat = ((bHalLcdAddr_t *)HAL_LCD_FSMC_ADDR)->dat;
#else
    bHalGPIO_Config(HAL_LCD_DAT_PORT, B_HAL_PINAll, B_HAL_GPIO_INPUT, B_HAL_GPIO_NOPULL);
    bHalGPIO_WritePin(HAL_LCD_RS_PORT, HAL_LCD_RS_PIN, 1);
    bHalGPIO_WritePin(HAL_LCD_RD_PORT, HAL_LCD_RD_PIN, 0);
    bHalGPIO_WritePin(HAL_LCD_CS_PORT, HAL_LCD_CS_PIN, 0);
    bHalGPIO_WritePin(HAL_LCD_RD_PORT, HAL_LCD_RD_PIN, 1);
    dat = bHalGPIO_Read(HAL_LCD_DAT_PORT);  
	bHalGPIO_WritePin(HAL_LCD_CS_PORT, HAL_LCD_CS_PIN, 1);
    bHalGPIO_Config(HAL_LCD_DAT_PORT, B_HAL_PINAll, B_HAL_GPIO_OUTPUT, B_HAL_GPIO_NOPULL);    
#endif   
    return dat;
}

uint16_t bHalLcdReadCmd()
{
    uint16_t cmd;
#ifdef HAL_LCD_FSMC_ADDR
    cmd = ((bHalLcdAddr_t *)HAL_LCD_FSMC_ADDR)->reg;
#else
    bHalGPIO_Config(HAL_LCD_DAT_PORT, B_HAL_PINAll, B_HAL_GPIO_INPUT, B_HAL_GPIO_NOPULL);
    bHalGPIO_WritePin(HAL_LCD_RS_PORT, HAL_LCD_RS_PIN, 0);
    bHalGPIO_WritePin(HAL_LCD_RD_PORT, HAL_LCD_RD_PIN, 0);
    bHalGPIO_WritePin(HAL_LCD_CS_PORT, HAL_LCD_CS_PIN, 0);
    bHalGPIO_WritePin(HAL_LCD_RD_PORT, HAL_LCD_RD_PIN, 1);
    cmd = bHalGPIO_Read(HAL_LCD_DAT_PORT);  
	bHalGPIO_WritePin(HAL_LCD_CS_PORT, HAL_LCD_CS_PIN, 1);
    bHalGPIO_Config(HAL_LCD_DAT_PORT, B_HAL_PINAll, B_HAL_GPIO_OUTPUT, B_HAL_GPIO_NOPULL); 
#endif   
    return cmd;
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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/












