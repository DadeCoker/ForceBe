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

/*******************************************************************************f8l10d*******/
#if 0
uint8_t UartTmpBuffer[250];
extern UART_HandleTypeDef huart2; 
extern DMA_HandleTypeDef hdma_usart2_rx;
static void _F810D_UartSend(uint8_t *pbuf, uint16_t len)
{
    HAL_UART_Transmit(&huart2, pbuf, len, 0xfff);
}

static void _F8L10D_SleepPin(uint8_t s)
{
    if(s)
    {
        HAL_GPIO_WritePin(LoRaSLEEP_GPIO_Port, LoRaSLEEP_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(LoRaSLEEP_GPIO_Port, LoRaSLEEP_Pin, GPIO_PIN_RESET);
    }
}

static void _F8L10D_ResetPin(uint8_t s)
{
    if(s)
    {
        HAL_GPIO_WritePin(LoRaRESET_GPIO_Port, LoRaRESET_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(LoRaRESET_GPIO_Port, LoRaRESET_Pin, GPIO_PIN_RESET);
    }
}

bF8L10D_Private_t bF8L10D_Private = {
    .pUartSend = _F810D_UartSend,
    .pSleepPin_Control = _F8L10D_SleepPin,
    .pResetPin_Control = _F8L10D_ResetPin,
};

NEW_F8L10D_DRV(F8l10dDriver, bF8L10D_Private);       //Create new driver

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)       //TX Done 
{
    if(GPIO_Pin == LoRaTXDone_Pin)
    {
        bF8L10D_TXDoneIrqHandler(&F8l10dDriver);
    }
}

void UartIdleIrqHandler()                           //Received data
{
    uint16_t count = 0;
    __HAL_UART_CLEAR_IDLEFLAG(&huart2);
    HAL_UART_DMAStop(&huart2);
    count = 250 - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);
    bF8L10D_RXCplHandler(&F8l10dDriver, UartTmpBuffer, count);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart2, UartTmpBuffer, 250);
}
#endif
/*******************************************************************************f8l10d end*******/


/********************************************************************************suart**********/
#if 0
static void _TX1_Control(uint8_t s)
{
    if(s)
    {
        HAL_GPIO_WritePin(TX1_GPIO_Port, TX1_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(TX1_GPIO_Port, TX1_Pin, GPIO_PIN_RESET);
    }
}

static uint8_t _RX1_Read()
{
    return HAL_GPIO_ReadPin(RX1_GPIO_Port, RX1_Pin);
}

static void _TX2_Control(uint8_t s)
{
    if(s)
    {
        HAL_GPIO_WritePin(TX2_GPIO_Port, TX2_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(TX2_GPIO_Port, TX2_Pin, GPIO_PIN_RESET);
    }
}

static uint8_t _RX2_Read()
{
    return HAL_GPIO_ReadPin(RX2_GPIO_Port, RX2_Pin);
}

bSUART_Private_t bSUART1_Private = {
    .pTxPIN_Control = _TX1_Control,
    .RxPIN_Read = _RX1_Read,
};

bSUART_Private_t bSUART2_Private = {
    .pTxPIN_Control = _TX2_Control,
    .RxPIN_Read = _RX2_Read,
};

NEW_SUART_DRV(SUART1_DRV, bSUART1_Private);

NEW_SUART_DRV(SUART2_DRV, bSUART2_Private);


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    S_UartTimerHandler(&SUART1_DRV);
    S_UartTimerHandler(&SUART2_DRV);  
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == RX1_Pin)
    {
        S_UartEXTI_Handler(&SUART1_DRV);
    }
    else if(GPIO_Pin == RX2_Pin)
    {
        S_UartEXTI_Handler(&SUART2_DRV);
    }    
}
#endif
/*****************************************************************************suart end**********/


/********************************************************************************spiflash**********/
#if 1
extern SPI_HandleTypeDef hspi2;
static uint8_t _W25X_SPI_ReadWrite(uint8_t byte)
{
    uint8_t tmp;
    HAL_SPI_TransmitReceive(&hspi2, &byte, &tmp, 1, 0xfff);
    return tmp;
}
static void _W25X_CS(uint8_t s)
{
    if(s)   HAL_GPIO_WritePin(W25X_CS_GPIO_Port, W25X_CS_Pin, GPIO_PIN_SET);
    else    HAL_GPIO_WritePin(W25X_CS_GPIO_Port, W25X_CS_Pin, GPIO_PIN_RESET);
}
bW25X_Private_t bW25X_Private = {
    .pSPI_ReadWriteByte = _W25X_SPI_ReadWrite,
    .pCS_Control = _W25X_CS,
};

NEW_W25X_DRV(bW25X_Driver, bW25X_Private);

#endif
/*****************************************************************************spiflash end**********/

/**
 * \}
 */
   
/** 
 * \addtogroup HAL_Exported_Functions
 * \{
 */
extern UART_HandleTypeDef huart1;
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


