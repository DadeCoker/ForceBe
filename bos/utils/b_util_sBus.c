/**
 *!
 * \file        b_util_sBus.c
 * \version     v0.0.1
 * \date        2021/04/29
 * \author      PolyGithub(baoli.chen@outlook.com)
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 polyGithub
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
#include "b_util_sBus.h"

/**
 * \addtogroup B_UTILS
 * \{
 */

/**
 * \addtogroup Single BUS
 * \{
 */

/**
 * \defgroup SBUS_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SBUS_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SBUS_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SBUS_Private_Variables
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SBUS_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SBUS_Private_Functions
 * \{
 */
 /*如下的计算方法：
 
一个时钟周期的时间是：1/48M = 0.02083us；
一个for循环是 6个时钟周期（0x08000284-0x08000290），时间是：0.12498us ;
当 xus= 8时，延时时间是：1us.
*/

static void bDelayUS(uint32_t xus)
{
	xus = 8 * xus;
	for(; xus!=0; xus--);
}
/**
 * \}
 */

/**
 * \addtogroup SBUS_Exported_Functions
 * \{
 */

uint8_t bUtilSbus_Ready(bUtilSbus_t Sbus)
{
	uint16_t cp = 0;
	uint16_t wait = 1000; //user define, ack timeout 
	bHalGPIO_Config(Sbus.sBusIo.port, Sbus.sBusIo.pin, B_HAL_GPIO_OUTPUT, B_HAL_GPIO_NOPULL);
	bHalGPIO_WritePin(Sbus.sBusIo.port, Sbus.sBusIo.pin, 0);
	bDelayUS(495);	//将总线拉低480us~960us
	bHalGPIO_WritePin(Sbus.sBusIo.port, Sbus.sBusIo.pin, 1);
	bHalGPIO_Config(Sbus.sBusIo.port, Sbus.sBusIo.pin, B_HAL_GPIO_INPUT, B_HAL_GPIO_NOPULL);
	cp = 0;
	while(((bHalGPIO_ReadPin(Sbus.sBusIo.port, Sbus.sBusIo.pin))==1) && cp++<wait); //拉高总线，如果设备做出反应会将在15us~60us后总线拉低,然后其以拉低总线60-240us的方式发出存在脉冲
	if(cp>=wait){return 0;}
	bDelayUS(495);	
	return 1;
}	

void SbusWriteByte(bUtilSbus_t Sbus,uint8_t dat) 
{
	uint8_t  j;
	bHalGPIO_Config(Sbus.sBusIo.port, Sbus.sBusIo.pin, B_HAL_GPIO_OUTPUT, B_HAL_GPIO_NOPULL);
	for (j = 0; j < 8; j++) {
		bHalGPIO_WritePin(Sbus.sBusIo.port, Sbus.sBusIo.pin, 0);
		bDelayUS(2);	
		bHalGPIO_WritePin(Sbus.sBusIo.port, Sbus.sBusIo.pin, 1);
		bDelayUS(2);     	  
		bHalGPIO_WritePin(Sbus.sBusIo.port, Sbus.sBusIo.pin, dat & 0x01);//写入一个数据，从最低位开始
		bDelayUS(68); //延时68us，持续时间最少60us	
		bHalGPIO_WritePin(Sbus.sBusIo.port, Sbus.sBusIo.pin, 1);//释放总线，至少1us给总线恢复时间才能接着写入第二个数值
		bDelayUS(2);
		dat = dat >> 1;
	}
}

uint8_t  SbusReadByte(bUtilSbus_t Sbus)
 {
	uint8_t byte = 0;
	uint8_t bit = 0;
	uint8_t  j;
	for (j = 8; j > 0; j--) {
		bHalGPIO_Config(Sbus.sBusIo.port, Sbus.sBusIo.pin, B_HAL_GPIO_OUTPUT, B_HAL_GPIO_NOPULL);	
		bHalGPIO_WritePin(Sbus.sBusIo.port, Sbus.sBusIo.pin, 0);//先将总线拉低1us产生读时序
		bDelayUS(2);  
		bHalGPIO_WritePin(Sbus.sBusIo.port, Sbus.sBusIo.pin, 1);//然后释放总线
		bDelayUS(6);//延时6us等待数据稳定
		bHalGPIO_Config(Sbus.sBusIo.port, Sbus.sBusIo.pin, B_HAL_GPIO_INPUT, B_HAL_GPIO_NOPULL);
		bit = bHalGPIO_ReadPin(Sbus.sBusIo.port, Sbus.sBusIo.pin);
		//将byte左移一位，然后与上右移7位后的bi，注意移动之后移掉那位补0。
		byte = (byte >> 1) | (bit << 7);
		bDelayUS(68); //读取完之后等待48us再接着读取下一个数
	}
	return byte;
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

/************************ Copyright (c) 2021 polyGithub *****END OF FILE****/
