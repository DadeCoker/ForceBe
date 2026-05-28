/**
 *!
 * \file        b_drv_pcf8574.c
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
   
/*Includes ----------------------------------------------*/
#include "b_drv_io_expander.h"
#include "b_drv_pcf8574.h"
/** 
 * \addtogroup B_DRIVER
 * \{
 */

/** 
 * \addtogroup PCF8574
 * \{
 */

/** 
 * \defgroup PCF8574_Private_TypesDefinitions
 * \{
 */
 

/**
 * \}
 */
   
/** 
 * \defgroup PCF8574_Private_Defines
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup PCF8574_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup PCF8574_Private_Variables
 * \{
 */
bPCF8574_Driver_t bPCF8574_Driver = {
    .init = bPCF8574_Init,
};
/**
 * \}
 */
   
/** 
 * \defgroup PCF8574_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup PCF8574_Private_Functions
 * \{
 */	

/************************************************************************************************************driver interface*******/


static int _bPCF8574Write(uint32_t off, uint8_t *pbuf, uint16_t len)
{
    uint8_t tmp;
    if(off >= 8 || len != 1)
    {
        return -1;
    }
    
    tmp = bHalI2C_ReceiveByte(HAL_PCF8574_I2C, HAL_PCF8574_I2C_ADDR);
    if(pbuf[0])
    {
        tmp |= 1 << off;
    }
    else
    {
        tmp &= ~(1 << off);
    }
    bHalI2C_SendByte(HAL_PCF8574_I2C, HAL_PCF8574_I2C_ADDR, tmp);
    return len;
}


static int _bPCF8574Read(uint32_t off, uint8_t *pbuf, uint16_t len)
{
    uint8_t tmp;
    if(off >= 8 || len != 1)
    {
        return -1;
    }  
    tmp = bHalI2C_ReceiveByte(HAL_PCF8574_I2C, HAL_PCF8574_I2C_ADDR);
    if(tmp & (1 << off))
    {
        pbuf[0] = 1;
    }
    else
    {
        pbuf[0] = 0;
    }
    return len;
}

/**
 * \}
 */
   
/** 
 * \addtogroup PCF8574_Exported_Functions
 * \{
 */
int bPCF8574_Init()
{          
    bPCF8574_Driver.close = NULL;
    bPCF8574_Driver.read = _bPCF8574Read;
    bPCF8574_Driver.ctl = NULL;
    bPCF8574_Driver.open = NULL;
    bPCF8574_Driver.write = _bPCF8574Write;
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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/

