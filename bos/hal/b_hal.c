/**
 *!
 * \file        b_hal.c
 * \version     v0.0.1
 * \date        2019/06/05
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 * 
 * Copyright (c) 2019 Bean
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
#include "b_hal.h" 
#include "b_driver.h"
#include <string.h>
/** 
 * \addtogroup B_HAL
 * \{
 */

/** 
 * \addtogroup HAL
 * \{
 */

/** 
 * \defgroup HAL_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup HAL_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup HAL_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup HAL_Private_Variables
 * \{
 */
extern UART_HandleTypeDef huart1;
volatile uint32_t bSysTick = 0;

/**
 * \}
 */
   
/** 
 * \defgroup HAL_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup HAL_Private_Functions
 * \{
 */

/****************************************************************************SSD1289*******/


/****************************************************************************xpt2046*******/


/*******************************************************************************w25x*******/
extern SPI_HandleTypeDef hspi2;
static uint8_t _HalW25X_SPI_RW(uint8_t dat)
{
    uint8_t tmp;
    HAL_SPI_TransmitReceive(&hspi2, &dat, &tmp, 1, 0xfff);
    return tmp;
}

static void _HalW25X_CS(uint8_t s)
{
    if(s)
    {
        HAL_GPIO_WritePin(W25X_CS_GPIO_Port, W25X_CS_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(W25X_CS_GPIO_Port, W25X_CS_Pin, GPIO_PIN_RESET);
    }
}

bW25X_Private_t bW25X_HalIF = {
    .pSPI_ReadWriteByte = _HalW25X_SPI_RW,
    .pCS_Control = _HalW25X_CS,
};

NEW_W25X_DRV(SPIFlashDriver, bW25X_HalIF);

/*******************************************************************************suart*******/
static void _HalSUART_TX(uint8_t s)
{
    if(s)
    {
        HAL_GPIO_WritePin(SUART_TX_GPIO_Port, SUART_TX_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(SUART_TX_GPIO_Port, SUART_TX_Pin, GPIO_PIN_RESET);
    }    
}

static void _HalSUART2_TX(uint8_t s)
{
    if(s)
    {
        HAL_GPIO_WritePin(SUART2_TX_GPIO_Port, SUART2_TX_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(SUART2_TX_GPIO_Port, SUART2_TX_Pin, GPIO_PIN_RESET);
    }    
}

static uint8_t _HalSUART_RX()
{
    return HAL_GPIO_ReadPin(SUART_TX_GPIO_Port, SUART_TX_Pin);
}

static uint8_t _HalSUART2_RX()
{
    return HAL_GPIO_ReadPin(SUART2_TX_GPIO_Port, SUART2_TX_Pin);
}

bSUART_Private_t bSUART_Private1 = {
    .pTxPIN_Control = _HalSUART_TX,
    .RxPIN_Read = _HalSUART_RX,
};

bSUART_Private_t bSUART_Private2 = {
    .pTxPIN_Control = _HalSUART2_TX,
    .RxPIN_Read = _HalSUART2_RX,
};


NEW_SUART_DRV(SUART_Driver1, bSUART_Private1);
NEW_SUART_DRV(SUART_Driver2, bSUART_Private2);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    S_UartTxTimerHandler(&SUART_Driver1);
    S_UartRxTimerHandler(&SUART_Driver1);
    S_UartTxTimerHandler(&SUART_Driver2);
    S_UartRxTimerHandler(&SUART_Driver2);    
}


/**
 * \}
 */
   
/** 
 * \addtogroup HAL_Exported_Functions
 * \{
 */

void bLogOutput(void *p)
{
    HAL_UART_Transmit(&huart1, p, strlen(p), 0xffff);    
}


void bHalEnterCritical()
{

}    

void bHalExitCritical()
{

}


/**
 * \brief Call this function _TICK_FRQ_HZ times per second \ref _TICK_FRQ_HZ
 */
void bHalIncSysTick()
{
    bSysTick += 1;
}

void bHalInit()
{
    // Add code ...gpio init or some other functions
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


