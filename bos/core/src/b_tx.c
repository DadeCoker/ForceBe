/**
 *!
 * \file        b_tx.c
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
#include "b_tx.h"  
#if _TX_ENABLE
#include "b_core.h"
#include "b_hal.h"
#include <string.h>
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup TX
 * \{
 */

/** 
 * \defgroup TX_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup TX_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup TX_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup TX_Private_Variables
 * \{
 */
static bTX_Info_t bTX_InfoTable[_TX_I_NUMBER]; 
static uint8_t bTX_InfoIndex = 0;
/**
 * \}
 */
   
/** 
 * \defgroup TX_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup TX_Private_Functions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \addtogroup TX_Exported_Functions
 * \{
 */
 
/**
 * \brief Create a TX instance
 * \param pbuf Pointer to data buffer
 * \param size Size of buffer
 * \param td_mode 
 *          \arg \ref BTX_F_SYN 
 *          \arg \ref BTX_F_ASYN 
 * \param dev_no Device Number
 * \retval Instance ID
 *          \arg >=0  valid
 *          \arg -1   invalid
 */
int bTX_Regist(uint8_t *pbuf, uint32_t size, uint8_t td_mode, uint8_t dev_no)
{
    if(pbuf == NULL || bTX_InfoIndex >= bCFG_TX_I_NUM)
    {
        return -1;
    }
    bTX_InfoTable[bTX_InfoIndex].ptxBUF = pbuf;
    bTX_InfoTable[bTX_InfoIndex].buf_size = size;
    bTX_InfoTable[bTX_InfoIndex].dev_no = dev_no;
    bTX_InfoTable[bTX_InfoIndex].state = BTX_NULL;
    bTX_InfoTable[bTX_InfoIndex].td_mode = td_mode;
    bTX_InfoTable[bTX_InfoIndex].td_flag = 0;
    bTX_InfoTable[bTX_InfoIndex].tx_len = 0;
    bTX_InfoTable[bTX_InfoIndex].fd = -1;
    bTX_InfoTable[bTX_InfoIndex].timeout = 0;
    bTX_InfoTable[bTX_InfoIndex].timeout_f = 0;
    bTX_InfoIndex += 1;
    return (bTX_InfoIndex - 1);
}


/**
 * \brief Transmit data
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bTX_Core()
{
    int i = 0;
    int retval = -1;
    for(i = 0;i < bTX_InfoIndex;i++)
    {
        if(bTX_InfoTable[i].state == BTX_REQ)
        {
            bTX_InfoTable[i].fd = bOpen(bTX_InfoTable[i].dev_no, BCORE_FLAG_RW);
            if(bTX_InfoTable[i].fd < 0)
            {
                b_log("tx open err\r\n");
                continue;
            }  
            bTX_InfoTable[i].td_flag = 0;
            retval = bWrite(bTX_InfoTable[i].fd, bTX_InfoTable[i].ptxBUF, bTX_InfoTable[i].tx_len);
            if(retval >= 0)
            {
                if(bTX_InfoTable[i].td_mode == BTX_F_SYN)
                {
                    bTX_InfoTable[i].state = BTX_NULL;
                    bClose(bTX_InfoTable[i].fd);
                    b_log("tx done\r\n");
                }
                else
                {
                    bTX_InfoTable[i].state = BTX_WAIT;
                    bTX_InfoTable[i].timeout_f = 1;
                    bTX_InfoTable[i].timeout = bHalGetTick();
                }
            }
            else
            {
                bClose(bTX_InfoTable[i].fd);
                b_log("tx write err\r\n");
                continue;
            }
        }
        
        if(bTX_InfoTable[i].state == BTX_WAIT)
        {
            if(bTX_InfoTable[i].td_flag || 
                (((bHalGetTick() - bTX_InfoTable[i].timeout) > (2 * _TICK_FRQ_HZ)) && bTX_InfoTable[i].timeout_f == 1))
            {
                bTX_InfoTable[i].timeout_f = 0;
                bTX_InfoTable[i].td_flag = 0;
                bTX_InfoTable[i].state = BTX_NULL;
                bClose(bTX_InfoTable[i].fd);
                b_log("tx done %d\r\n", bHalGetTick() - bTX_InfoTable[i].timeout);
            }
        }
    }
    return 0;
}



int bTX_Request(int no, uint8_t *pbuf, uint16_t size, uint8_t flag)
{

    if(no < 0 || no >= bTX_InfoIndex || pbuf == NULL)
    {
        return -1;
    }

    if(flag == BTX_REQ_LEVEL0 && bTX_InfoTable[no].state != BTX_NULL)
    {
        return -1;
    }

    if(size > bTX_InfoTable[no].buf_size)
    {
        return -1;
    }
    b_log("req %d data\r\n", size);
#if 0    
    uint8_t i = 0;
    for(i = 0;i < size;i++)
    {
        b_log("%d ", pbuf[i]);
    }
    b_log("\r\n");
#endif    
    memcpy(bTX_InfoTable[no].ptxBUF, pbuf, size);
    bTX_InfoTable[no].tx_len = size;
    bTX_InfoTable[no].state = BTX_REQ;
    return 0;
}




int bTX_CplCallback(int no)
{
    if(no < 0 || no >= bTX_InfoIndex)
    {
        return -1;
    }

    if(bTX_InfoTable[no].td_mode == BTX_F_SYN)
    {
        return -1;
    }
    bTX_InfoTable[no].td_flag = 1;
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
#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/

