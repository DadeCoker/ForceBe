/**
 *!
 * \file        b_util_asyntx.c
 * \version     v0.0.1
 * \date        2020/03/15
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
#include "b_utils.h"
#include "b_hal.h"
/** 
 * \addtogroup B_UTILS
 * \{
 */

/** 
 * \addtogroup ASYN_TX
 * \{
 */

/** 
 * \defgroup ASYN_TX_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup ASYN_TX_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup ASYN_TX_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup ASYN_TX_Private_Variables
 * \{
 */
static bAsyntxInfo_t bAsyntxInfoTable[_ASYN_TX_I_NUMBER]; 
static uint8_t bAsyntxInfoIndex = 0;
/**
 * \}
 */
   
/** 
 * \defgroup ASYN_TX_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup ASYN_TX_Private_Functions
 * \{
 */
/**
 * \brief Transmit data
 */
static void _bAsyntxCore()
{
    int i = 0;
    for(i = 0;i < bAsyntxInfoIndex;i++)
    {
        if(bAsyntxInfoTable[i].state == BASYN_TX_WAIT)
        {
            if(bAsyntxInfoTable[i].td_flag || 
                (((bUtilGetTick() - bAsyntxInfoTable[i].s_tick) > (MS2TICKS(bAsyntxInfoTable[i].timeout))) 
                    && bAsyntxInfoTable[i].timeout_f == 1))
            {
                bAsyntxInfoTable[i].timeout_f = 0;
                bAsyntxInfoTable[i].td_flag = 0;
                bAsyntxInfoTable[i].state = BASYN_TX_NULL;
                b_log("ASYN_TX done %d\r\n", bUtilGetTick() - bAsyntxInfoTable[i].s_tick);
            }
        }
    }
} 

BOS_REG_POLLING_FUNC(_bAsyntxCore);

/**
 * \}
 */
   
/** 
 * \addtogroup ASYN_TX_Exported_Functions
 * \{
 */
 
/**
 * \brief Create a ASYN_TX instance
 * \param f Pointer to the function sending data
 * \param timeout_ms The timeout that waits for the send to complete
 * \retval Instance ID
 *          \arg >=0  valid
 *          \arg -1   invalid
 */
int bAsyntxRegist(pSendBytes f, uint32_t timeout_ms)
{
    if(f == NULL || bAsyntxInfoIndex >= _ASYN_TX_I_NUMBER)
    {
        return -1;
    }
    bAsyntxInfoTable[bAsyntxInfoIndex].state = BASYN_TX_NULL;
    bAsyntxInfoTable[bAsyntxInfoIndex].td_flag = 0;
    bAsyntxInfoTable[bAsyntxInfoIndex].timeout = timeout_ms;
    bAsyntxInfoTable[bAsyntxInfoIndex].timeout_f = 0;
    bAsyntxInfoTable[bAsyntxInfoIndex].s_tick = 0;
    bAsyntxInfoTable[bAsyntxInfoIndex].f = f;
    bAsyntxInfoIndex += 1;
    return (bAsyntxInfoIndex - 1);
}



int bAsyntxRequest(int no, uint8_t *pbuf, uint16_t size, uint8_t flag)
{
    if(no < 0 || no >= bAsyntxInfoIndex || pbuf == NULL)
    {
        return -1;
    }

    if(flag == BASYN_TX_REQ_L0 && bAsyntxInfoTable[no].state != BASYN_TX_NULL)
    {
        return -1;
    }   
    bAsyntxInfoTable[no].f(pbuf, size);
    bAsyntxInfoTable[no].state = BASYN_TX_WAIT;
    bAsyntxInfoTable[no].timeout_f = 1;
    bAsyntxInfoTable[no].s_tick = bUtilGetTick();
    return 0;
}


int bAsyntxCplCallback(int no)
{
    if(no < 0 || no >= bAsyntxInfoIndex)
    {
        return -1;
    }
    bAsyntxInfoTable[no].td_flag = 1;
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

/************************ Copyright (c) 2019 Bean *****END OF FILE****/

