/**
 *!
 * \file        b_hal_spi.c
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SSPIL THE
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
 * \addtogroup SPI
 * \{
 */

/** 
 * \defgroup SPI_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SPI_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SPI_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SPI_Private_Variables
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup SPI_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SPI_Private_Functions
 * \{
 */


/**
 * \}
 */
   
/** 
 * \addtogroup SPI_Exported_Functions
 * \{
 */

int bHalSPI_SetSpeed(bHalSPINumber_t spi, uint32_t clk_hz)
{
    uint32_t spi_bus_clk = 72000000;
    uint8_t div = 0;
    if(clk_hz > 18000000)
    {
        clk_hz =  18000000;    
    }
    while(spi_bus_clk > clk_hz)
    {
        spi_bus_clk >>= 1;
        div++;
    }
    switch(spi)
    {
        case B_HAL_SPI_1:

            break;        
        case B_HAL_SPI_2:
            while(hspi2.Instance->SR & 0x80);
            hspi2.Instance->CR1 &= ~(0x7 << 3);
            hspi2.Instance->CR1 |= (div - 1) << 3;
            break;
        case B_HAL_SPI_3:

            break;
        default:
            break;
    }
    return 0;
}
 

uint8_t bHalSPI_SendReceiveByte(bHalSPINumber_t spi, uint8_t dat)
{
    uint8_t tmp;
    switch(spi)
    {
        case B_HAL_SPI_1:

            break;        
        case B_HAL_SPI_2:
            HAL_SPI_TransmitReceive(&hspi2, &dat, &tmp, 1, 0xfff);
            break;
        case B_HAL_SPI_3:

            break;
        default:
            break;
    }
    return tmp;
}



int bHalSPI_Send(bHalSPINumber_t spi, uint8_t *pbuf, uint16_t len)
{
    if(pbuf == NULL)
    {
        return -1;
    }
    switch(spi)
    {
        case B_HAL_SPI_1:

            break;        
        case B_HAL_SPI_2:
            HAL_SPI_Transmit(&hspi2, pbuf, len, 0xfff);
            break;
        case B_HAL_SPI_3:

            break;
        default:
            break;
    }
    return 0;
}

int bHalSPI_Receive(bHalSPINumber_t spi, uint8_t *pbuf, uint16_t len)
{
    if(pbuf == NULL)
    {
        return -1;
    }
    switch(spi)
    {
        case B_HAL_SPI_1:

            break;        
        case B_HAL_SPI_2:
            HAL_SPI_Receive(&hspi2, pbuf, len, 0xfff);
            break;
        case B_HAL_SPI_3:

            break;
        default:
            break;
    }
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


