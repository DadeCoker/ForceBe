/**
 *!
 * \file        b_util_at.c
 * \version     v0.0.1
 * \date        2019/12/26
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
#include "b_utils.h" 
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

/** 
 * \addtogroup B_UTILS
 * \{
 */

/** 
 * \addtogroup AT
 * \{
 */

/** 
 * \defgroup AT_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup AT_Private_Defines
 * \{
 */
 
/**
 * \}
 */
   
/** 
 * \defgroup AT_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup AT_Private_Variables
 * \{
 */
static bAT_Info_t  bAT_Info[_AT_I_NUMBER];
static uint8_t bAT_InfoIndex = 0;   
/**
 * \}
 */
   
/** 
 * \defgroup AT_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup AT_Private_Functions
 * \{
 */
  
/**
 * \}
 */
   
/** 
 * \addtogroup AT_Exported_Functions
 * \{
 */
 
/**
 * \brief Create an AT instance
 * \param ptx pointer to a function that to send AT data
 * \retval Instance ID
 *          \arg >=0  valid
 *          \arg -1   invalid
 */
int bAT_Regist(pAT_TX ptx)
{
    if(ptx == NULL || bAT_InfoIndex >= _AT_I_NUMBER)
    {
        return -1;
    }
    bAT_Info[bAT_InfoIndex].AT_TXFunc = ptx;
    bAT_Info[bAT_InfoIndex].ctick = 0;
    bAT_Info[bAT_InfoIndex].r_flag = 0;
    bAT_InfoIndex += 1;
    return (bAT_InfoIndex - 1);
}    


/**
 * \brief Transmit the AT command and wait for a response
 * \param no AT instance id \ref bAT_Regist
 * \param pe_resp pointer to a struct that indicate timeout and received data
 *           it can be NULL, if u dont care the data in the response
 * \param pcmd AT command
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bAT_Write(int no, bAT_ExpectedResp_t *pe_resp, const char *pcmd, ...)
{
    va_list ap;
    int str_len = 0;
    if(no < 0 || no >= bAT_InfoIndex || pcmd == NULL)
    {
        return -1;
    }
    va_start(ap, pcmd);
    str_len = vsnprintf((char *)bAT_Info[no].buf, _AT_BUF_LEN, pcmd, ap);
    va_end(ap);
    if(str_len < 0)
    {
        return -1;
    }
    bAT_Info[no].stat = AT_STA_NULL;
    bAT_Info[no].ctick = bUtilGetTick();
    bAT_Info[no].r_flag = 0;
    bAT_Info[no].r_len = 0;
    bAT_Info[no].AT_TXFunc(bAT_Info[no].buf, str_len);
    
    if(pe_resp == NULL)
    {
        bAT_Info[no].stat = AT_STA_NULL;
    }
    else
    {
        bAT_Info[no].stat = AT_STA_WAIT;
        while(bAT_Info[no].r_flag == 0)
        {
            if((bUtilGetTick() - bAT_Info[no].ctick) > pe_resp->timeout)
            {
                bAT_Info[no].stat = AT_STA_NULL;
                return -1;
            }
        }
        pe_resp->pResp = bAT_Info[no].buf;
        pe_resp->len = bAT_Info[no].r_len;
        bAT_Info[no].stat = AT_STA_NULL;
    }
    return 0;
}

/**
 * \brief Call this function after receiving response data
 * \param no AT instance id \ref bAT_Regist
 * \param pbuf pointer to data buffer
 * \param size amount of data
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bAT_Read(int no, uint8_t *pbuf, uint16_t size)
{
    if(no < 0 || no >= bAT_InfoIndex || pbuf == NULL)
    {
        return -1;
    }
    if(bAT_Info[no].stat == AT_STA_WAIT)
    {
        bAT_Info[no].r_flag = 1;
        size = (size <= _AT_BUF_LEN) ? size : _AT_BUF_LEN;
        memcpy(bAT_Info[no].buf, pbuf, size);
        bAT_Info[no].r_len = size;     
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


/************************ Copyright (c) 2019 Bean *****END OF FILE****/

