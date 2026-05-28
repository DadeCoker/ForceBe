/**
 *!
 * \file        b_drv_24cxx.c
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
#include "b_drv_flash.h"
#include "b_drv_24cxx.h"
/** 
 * \addtogroup B_DRIVER
 * \{
 */

/** 
 * \addtogroup 24CXX
 * \{
 */

/** 
 * \defgroup 24CXX_Private_TypesDefinitions
 * \{
 */
 

/**
 * \}
 */
   
/** 
 * \defgroup 24CXX_Private_Defines
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup 24CXX_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup 24CXX_Private_Variables
 * \{
 */
b24CXX_Driver_t b24CXX_Driver = {
    .init = b24CXX_Init,
};
/**
 * \}
 */
   
/** 
 * \defgroup 24CXX_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup 24CXX_Private_Functions
 * \{
 */	

/************************************************************************************************************driver interface*******/


static int _b24CXXWrite(uint32_t off, uint8_t *pbuf, uint16_t len)
{
    uint8_t l_c = off % 8;
    uint16_t i = 0;
    if(len <= l_c)
    {
        bHalI2C_MemWrite(HAL_24CXX_I2C, HAL_24CXX_I2C_ADDR, off, pbuf, len);
    }
    else
    {
        bHalI2C_MemWrite(HAL_24CXX_I2C, HAL_24CXX_I2C_ADDR, off, pbuf, l_c);
        bUtilDelayMS(5);
        off += l_c;
        pbuf += l_c;
        len -= l_c;
        for(i = 0;i < len / 8;i++)
        {
            bHalI2C_MemWrite(HAL_24CXX_I2C, HAL_24CXX_I2C_ADDR, off, pbuf, 8);
            bUtilDelayMS(5);
            off += 8;
            pbuf += 8;
        }
        if((len % 8) > 0)
        {
            bHalI2C_MemWrite(HAL_24CXX_I2C, HAL_24CXX_I2C_ADDR, off, pbuf, (len % 8));
            bUtilDelayMS(5);
        }
    }
    return len;
}


static int _b24CXXRead(uint32_t off, uint8_t *pbuf, uint16_t len)
{
    bHalI2C_MemRead(HAL_24CXX_I2C, HAL_24CXX_I2C_ADDR, off, pbuf, len);
    return len;
}

/**
 * \}
 */
   
/** 
 * \addtogroup 24CXX_Exported_Functions
 * \{
 */
int b24CXX_Init()
{          
    b24CXX_Driver.close = NULL;
    b24CXX_Driver.read = _b24CXXRead;
    b24CXX_Driver.ctl = NULL;
    b24CXX_Driver.open = NULL;
    b24CXX_Driver.write = _b24CXXWrite;
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

