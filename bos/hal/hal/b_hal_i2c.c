/**
 *!
 * \file        b_hal_i2c.c
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SI2CL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
   
/*Includes ----------------------------------------------*/
#include "b_hal.h" 
#include "b_utils.h"
/** 
 * \addtogroup B_HAL
 * \{
 */

/** 
 * \addtogroup I2C
 * \{
 */

/** 
 * \defgroup I2C_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup I2C_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup I2C_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup I2C_Private_Variables
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup I2C_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup I2C_Private_Functions
 * \{
 */


/**
 * \}
 */
   
/** 
 * \addtogroup I2C_Exported_Functions
 * \{
 */

void bHalI2C_SendByte(bHalI2CNumber_t i2c, uint8_t dev_addr, uint8_t dat)
{
    switch(i2c)
    {
        case B_HAL_I2C_1:
            
            break;        
        case B_HAL_I2C_2:
            
            break;

        default:
            break;
    }
}

uint8_t bHalI2C_ReceiveByte(bHalI2CNumber_t i2c, uint8_t dev_addr)
{
    uint8_t tmp;
    switch(i2c)
    {
        case B_HAL_I2C_1:
            
            break;        
        case B_HAL_I2C_2:
            
            break;
        default:
            break;
    }
    return tmp;
}


int bHalI2C_MemWrite(bHalI2CNumber_t i2c, uint8_t dev_addr, uint16_t mem_addr, uint8_t *pbuf, uint16_t len)
{
    int retval = 0;
    switch(i2c)
    {
        case B_HAL_I2C_1:

            break;        
        case B_HAL_I2C_2:
            
            break;
        case B_HAL_I2C_3:

            break;        
        default:
            break;
    }
    return retval;
}


int bHalI2C_MemRead(bHalI2CNumber_t i2c, uint8_t dev_addr, uint16_t mem_addr, uint8_t *pbuf, uint16_t len)
{
    int retval = 0;
    switch(i2c)
    {
        case B_HAL_I2C_1:
 
            break;        
        case B_HAL_I2C_2:
            
            break;
        case B_HAL_I2C_3:

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
 * \}
 */


/**
 * \}
 */

/************************ Copyright (c) 2020 Bean *****END OF FILE****/


