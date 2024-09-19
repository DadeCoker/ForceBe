/**
 *!
 * \file        b_f8l10d.c
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
#include "b_f8l10d.h"
#include "b_os.h"
#include "b_hal.h"
#include "b_utils.h"
#include <string.h>
/** 
 * \addtogroup B_DRIVER
 * \{
 */

/** 
 * \addtogroup F8L10D
 * \{
 */

/** 
 * \defgroup F8L10D_Private_TypesDefinitions
 * \{
 */
 

/**
 * \}
 */
   
/** 
 * \defgroup F8L10D_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup F8L10D_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup F8L10D_Private_Variables
 * \{
 */

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;

bF8L10D_Driver_t bF8L10D_Driver = 
{
        .init = bF8L10D_Init,
};

static int bF8L10D_AT_No = -1;

static uint8_t bF8L10D_Buf[200];

/**
 * \}
 */
   
/** 
 * \defgroup F8L10D_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup F8L10D_Private_Functions
 * \{
 */
static int _bF8L10D_UartTX(uint8_t *pbuf, uint16_t len)
{
	HAL_UART_Transmit(&huart2, pbuf, len, 0xffff);
    return 0;
}


static int _bF8L10D_EnterATMode()
{
    uint8_t n_try = 0;
    int retval = -1;
    bAT_ExpectedResp_t resp;
    while(n_try < 6)
    {
        resp.timeout = 1000;
        resp.pResp = NULL;
        resp.len = 0;
        retval = bAT_Write(bF8L10D_AT_No, &resp, "+++");
        if(retval == 0 && resp.pResp != NULL)
        {
            if(strncmp((const char *)resp.pResp, "\r\nOK\r\n", 6) == 0)
            {
                return 0;
            }
        }
        n_try++;
    }
    return -1;
}

static int _bF8L10D_ExitATMode()
{
    uint8_t n_try = 0;
    int retval = -1;
    bAT_ExpectedResp_t resp;
    while(n_try < 6)
    {
        resp.timeout = 1000;
        resp.pResp = NULL;
        resp.len = 0;
        retval = bAT_Write(bF8L10D_AT_No, &resp, "AT+ESC\r\n");
        if(retval == 0 && resp.pResp != NULL)
        {
            if(strncmp((const char *)resp.pResp, "\r\nOK\r\n", 6) == 0)
            {
                return 0;
            }
        }
        n_try++;
    }
    return -1;
}


static int _bF8L10D_GetPID(uint16_t *pid)
{
    uint8_t n_try = 0;
    int retval = -1;
    int tmp;
    bAT_ExpectedResp_t resp;
    while(n_try < 6)
    {
        resp.timeout = 1000;
        resp.pResp = NULL;
        resp.len = 0;
        retval = bAT_Write(bF8L10D_AT_No, &resp, "AT+PID?\r\n");
        if(retval == 0 && resp.pResp != NULL)
        {
            retval = sscanf((const char *)resp.pResp, "\r\n+PID:%d\r\nOK\r\n", &tmp);
            if(retval > 0)
            {
                *pid = tmp;
                return 0;
            }
        }
        n_try++;
    }
    return -1; 
}



/**
 * \}
 */
   
/** 
 * \addtogroup F8L10D_Exported_Functions
 * \{
 */
int bF8L10D_Init()
{  
    uint16_t pid;
    bF8L10D_AT_No = bAT_Regist(_bF8L10D_UartTX);
    if(bF8L10D_AT_No < 0)
    {
        b_log("regist AT error....\r\n");
        return -1;
    }
    
    HAL_UART_Receive_DMA(&huart2, bF8L10D_Buf, 200);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
    bHalDelayMS(2000);            //delay 2s after power-on   
    
    if(0 > _bF8L10D_EnterATMode())
    {
        b_log("enter at mode error...\r\n");
        return -1;
    }
    b_log("at at at.....\r\n");
    
    
    if(0 > _bF8L10D_GetPID(&pid))
    {
        b_log("pid error.....\r\n");
    }
    else
    {
        b_log("pid %d\r\n", pid);
    }
    
    if(0 > _bF8L10D_ExitATMode())
    {
        b_log("exit error.....\r\n");
        return -1;
    }
    
    
    b_log("exit.....\r\n");

    return 0;
}


void bF8L10D_UartIdle()
{
    uint16_t len;
	__HAL_UART_CLEAR_IDLEFLAG(&huart2);
	HAL_UART_DMAStop(&huart2);
	len = 200 - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);
    if(len > 0)
    {
        bAT_Read(bF8L10D_AT_No, bF8L10D_Buf, len);	
    }
    HAL_UART_Receive_DMA(&huart2, bF8L10D_Buf, 200);
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

