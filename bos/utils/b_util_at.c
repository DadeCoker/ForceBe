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
 * \brief Transmit the AT command and wait for a response
 * \param pInstance Pointer to the at instance
 * \param timeout waiting time for response
 * \param presult received response data
 *           it can be NULL, if u dont care the data in the response
 * \param pcmd AT command
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bAT_Write(bAT_Instance_t *pInstance, uint32_t timeout, bAT_RecResult_t *presult, const char *pcmd, ...)
{
    va_list ap;
    int str_len = 0;
    if(pInstance == NULL || pcmd == NULL)
    {
        return -1;
    }
    va_start(ap, pcmd);
    str_len = vsnprintf((char *)pInstance->pbuf, pInstance->buf_len, pcmd, ap);
    va_end(ap);
    if(str_len < 0)
    {
        return -1;
    }
    pInstance->stat = AT_STA_NULL;
    pInstance->ctick = bUtilGetTick();
    pInstance->r_flag = 0;
    pInstance->r_len = 0;
    pInstance->f_send(pInstance->pbuf, str_len);
    
    if(presult == NULL)
    {
        pInstance->stat = AT_STA_NULL;
    }
    else
    {
        pInstance->stat = AT_STA_WAIT;
        while(pInstance->r_flag == 0)
        {
            if((bUtilGetTick() - pInstance->ctick) >= timeout)
            {
                pInstance->stat = AT_STA_NULL;
                return -1;
            }
        }
        presult->pbuf = pInstance->pbuf;
        presult->len = pInstance->r_len;
        pInstance->stat = AT_STA_NULL;
    }
    return 0;
}

/**
 * \brief Call this function after receiving response data
 * \param pInstance Pointer to the at instance
 * \param pbuf pointer to data buffer
 * \param size amount of data
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bAT_Read(bAT_Instance_t *pInstance, uint8_t *pbuf, uint16_t size)
{
    if(pInstance == NULL || pbuf == NULL)
    {
        return -1;
    }
    if(pInstance->stat == AT_STA_WAIT)
    {
        pInstance->r_flag = 1;
        size = (size <= pInstance->buf_len) ? size : pInstance->buf_len;
        memcpy(pInstance->pbuf, pbuf, size);
        pInstance->r_len = size;     
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

