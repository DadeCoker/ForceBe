/**
 *!
 * \file        b_hal_sccb.c
 * \version     v0.0.1
 * \date        2020/03/25
 * \author      Bean(sccbtrysccbhigh@outlook.com)
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
 * The above copyright sccbtice and this permission sccbtice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SSCCBL THE
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
 * \addtogroup SCCB
 * \{
 */

/** 
 * \defgroup SCCB_Private_TypesDefinitions
 * \{
 */
typedef struct
{
    bHalGPIOInstance_t scl;
    bHalGPIOInstance_t sda;
}SCCB_GPIO_Instance_t;
/**
 * \}
 */
   
/** 
 * \defgroup SCCB_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SCCB_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SCCB_Private_Variables
 * \{
 */
 
 
 
 
static SCCB_GPIO_Instance_t SCCB_GPIO_Instance[B_HAL_SCCB_NUMBER] = {
    [B_HAL_SCCB_1] = {
        .scl = {B_HAL_GPIOB, B_HAL_PIN4},
        .sda = {B_HAL_GPIOB, B_HAL_PIN3}
    },
    [B_HAL_SCCB_2] = {
        .scl = {B_HAL_GPIOA, B_HAL_PIN3},
        .sda = {B_HAL_GPIOA, B_HAL_PIN4}
    },   
};
/**
 * \}
 */
   
/** 
 * \defgroup SCCB_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SCCB_Private_Functions
 * \{
 */
static void _SCCB_Delay()
{
    uint32_t i;
    for(i = 0;i < 500;i++)
    {
        ;
    }
}
/**
 * \}
 */
   
/** 
 * \addtogroup SCCB_Exported_Functions
 * \{
 */

void SCCB_Start(bHalSCCBNumber_t sccb)
{
    bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].sda.port, SCCB_GPIO_Instance[sccb].sda.pin, 1);
    bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].scl.port, SCCB_GPIO_Instance[sccb].scl.pin, 1);
    _SCCB_Delay();
    bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].sda.port, SCCB_GPIO_Instance[sccb].sda.pin, 0);
    _SCCB_Delay();
    bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].scl.port, SCCB_GPIO_Instance[sccb].scl.pin, 0);
}    

void SCCB_Stop(bHalSCCBNumber_t sccb)
{
    bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].sda.port, SCCB_GPIO_Instance[sccb].sda.pin, 0);
    _SCCB_Delay();
    bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].scl.port, SCCB_GPIO_Instance[sccb].scl.pin, 1);
    _SCCB_Delay();
    bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].sda.port, SCCB_GPIO_Instance[sccb].sda.pin, 1);
    _SCCB_Delay();
}

void SCCB_NoAck(bHalSCCBNumber_t sccb)
{
    bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].sda.port, SCCB_GPIO_Instance[sccb].sda.pin, 1);
    bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].scl.port, SCCB_GPIO_Instance[sccb].scl.pin, 1);
    _SCCB_Delay();
    bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].scl.port, SCCB_GPIO_Instance[sccb].scl.pin, 0);
    _SCCB_Delay();
    bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].sda.port, SCCB_GPIO_Instance[sccb].sda.pin, 0);
}

void SCCB_SendByte(bHalSCCBNumber_t sccb, uint8_t dat)
{
	uint8_t j;	 
	for(j = 0;j < 8;j++) 
	{
		if(dat & 0x80)
        {
            bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].sda.port, SCCB_GPIO_Instance[sccb].sda.pin, 1);	
        }
		else 
        {
            bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].sda.port, SCCB_GPIO_Instance[sccb].sda.pin, 0);
        }
		dat <<= 1;
        _SCCB_Delay();
		bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].scl.port, SCCB_GPIO_Instance[sccb].scl.pin, 1);
        _SCCB_Delay();
		bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].scl.port, SCCB_GPIO_Instance[sccb].scl.pin, 0);		   
	}			 
	bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].scl.port, SCCB_GPIO_Instance[sccb].scl.pin, 1);
    _SCCB_Delay();
	bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].scl.port, SCCB_GPIO_Instance[sccb].scl.pin, 0);	 
}	


uint8_t SCCB_ReceiveByte(bHalSCCBNumber_t sccb)
{
	uint8_t temp=0,j;    
	bHalGPIO_Config(SCCB_GPIO_Instance[sccb].sda.port, SCCB_GPIO_Instance[sccb].sda.pin, B_HAL_GPIO_INPUT, B_HAL_GPIO_NOPULL);
	for(j = 8;j > 0;j--) 
	{		     	  
        _SCCB_Delay();
		bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].scl.port, SCCB_GPIO_Instance[sccb].scl.pin, 1);
		temp = temp << 1;
		if(bHalGPIO_ReadPin(SCCB_GPIO_Instance[sccb].sda.port, SCCB_GPIO_Instance[sccb].sda.pin))
        {
            temp++;
        }            
        _SCCB_Delay();
		bHalGPIO_WritePin(SCCB_GPIO_Instance[sccb].scl.port, SCCB_GPIO_Instance[sccb].scl.pin, 0);
	}	
	bHalGPIO_Config(SCCB_GPIO_Instance[sccb].sda.port, SCCB_GPIO_Instance[sccb].sda.pin, B_HAL_GPIO_OUTPUT, B_HAL_GPIO_NOPULL);  
	return temp;
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


