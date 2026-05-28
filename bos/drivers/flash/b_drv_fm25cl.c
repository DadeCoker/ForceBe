/**
 *!
 * \file        b_drv_fm25cl.c
 * \version     v0.0.1
 * \date        2020/02/05
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
#include "b_drv_fm25cl.h"
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup B_DRIVER
 * \{
 */

/** 
 * \addtogroup FM25CL
 * \{
 */

/** 
 * \defgroup FM25CL_Private_TypesDefinitions
 * \{
 */
 

/**
 * \}
 */
   
/** 
 * \defgroup FM25CL_Private_Defines
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup FM25CL_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup FM25CL_Private_Variables
 * \{
 */
bFM25CL_Driver_t bFM25CL_Driver = {
    .init = bFM25CL_Init,
};	

/**
 * \}
 */
   
/** 
 * \defgroup FM25CL_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup FM25CL_Private_Functions
 * \{
 */

static void _FM25_WR_Enable()
{	
    bHalGPIO_WritePin(HAL_FM25CL_CS_PORT, HAL_FM25CL_CS_PIN, 0);
    bHalSPI_SendReceiveByte(HAL_FM25CL_SPI, SFC_WREN);  
	bHalGPIO_WritePin(HAL_FM25CL_CS_PORT, HAL_FM25CL_CS_PIN, 1);
}	

static void _FM25_WR_Lock() 
{	
    bHalGPIO_WritePin(HAL_FM25CL_CS_PORT, HAL_FM25CL_CS_PIN, 0);
    bHalSPI_SendReceiveByte(HAL_FM25CL_SPI, SFC_WRDI);  
	bHalGPIO_WritePin(HAL_FM25CL_CS_PORT, HAL_FM25CL_CS_PIN, 1);    
}


/**************************************************************************************************driver interface*****/

static int _FM25_ReadBuff(uint32_t addr, uint8_t *pDat, uint16_t len)
{	
	int i; 
	bHalGPIO_WritePin(HAL_FM25CL_CS_PORT, HAL_FM25CL_CS_PIN, 0);
	bHalSPI_SendReceiveByte(HAL_FM25CL_SPI, SFC_READ);             
	bHalSPI_SendReceiveByte(HAL_FM25CL_SPI, (uint8_t)(addr >> 8));   
	bHalSPI_SendReceiveByte(HAL_FM25CL_SPI, (uint8_t)(addr >> 0));  
    
	for(i = 0;i < len;i++)
    {
        pDat[i] = bHalSPI_SendReceiveByte(HAL_FM25CL_SPI, 0);
    }
	bHalGPIO_WritePin(HAL_FM25CL_CS_PORT, HAL_FM25CL_CS_PIN, 1); 
    return len;
}   


static int _FM25_WritBuff(uint32_t addr, uint8_t* pdat,uint16_t len)
{	
	int i; 
	_FM25_WR_Enable();           
	//-----------------------------------------------------------
	bHalGPIO_WritePin(HAL_FM25CL_CS_PORT, HAL_FM25CL_CS_PIN, 0);
	bHalSPI_SendReceiveByte(HAL_FM25CL_SPI, SFC_WRITE);        
	bHalSPI_SendReceiveByte(HAL_FM25CL_SPI, (unsigned char)(addr>>8));
	bHalSPI_SendReceiveByte(HAL_FM25CL_SPI, (unsigned char)(addr>>0)); 
	for(i = 0;i < len;i++)
    {
        bHalSPI_SendReceiveByte(HAL_FM25CL_SPI, pdat[i]);
    }
	bHalGPIO_WritePin(HAL_FM25CL_CS_PORT, HAL_FM25CL_CS_PIN, 1);
	//-----------------------------------------------------------
	_FM25_WR_Lock();    
    return len;
} 


/**
 * \}
 */
   
/** 
 * \addtogroup FM25CL_Exported_Functions
 * \{
 */
int bFM25CL_Init()
{  
    bFM25CL_Driver.close = NULL;
    bFM25CL_Driver.read = _FM25_ReadBuff;
    bFM25CL_Driver.ctl = NULL;
    bFM25CL_Driver.open = NULL;
    bFM25CL_Driver.write = _FM25_WritBuff;
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

/**
 * \}
 */

/************************ Copyright (c) 2019 Bean *****END OF FILE****/

