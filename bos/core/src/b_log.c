/**
 *!
 * \file        b_log.c
 * \version     v0.0.1
 * \date        2020/02/25
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
#include "b_log.h"
#if _DEBUG_ENABLE
#include "b_hal.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup LOG
 * \{
 */

/** 
 * \defgroup LOG_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */


/** 
 * \defgroup LOG_Private_Defines
 * \{
 */
      
#define B_LOG_BUF_MIN               64

#if (B_LOG_BUF_SIZE < B_LOG_BUF_MIN)	 
	 #error "error: B_LOG_BUF_SIZE < 64"	 
#endif

/**
 * \}
 */
   
/** 
 * \defgroup LOG_Private_Macros
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup LOG_Private_Variables
 * \{
 */
static uint8_t bLogBuff[B_LOG_BUF_SIZE];
static const uint8_t bLogParamTable[3] = {B_LOG_I_PARAM, B_LOG_W_PARAM, B_LOG_E_PARAM};
static const char bLogPrefix[3] = {'I', 'W', 'E'};

/**
 * \}
 */
   
/** 
 * \defgroup LOG_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup LOG_Private_Functions
 * \{
 */

/**
 * \}
 */
   
/** 
 * \addtogroup LOG_Exported_Functions
 * \{
 */


void bLogOutput(void *p)
{
    bHalUartSend(HAL_LOG_UART, p, strlen(p));
}


/**
 * \brief Create and output string
 * \param type b_log_i b_log_w b_log_e b_log >> 0, 1, 2, 3
 * \param ptr_file file name
 * \param ptr_func function name
 * \param line line number
 * \retval none
 */
void bLogOut(uint8_t type, const char *ptr_file, const char *ptr_func, uint32_t line, const char *fmt, ...)
{
    uint32_t buf_len = 0;
    uint32_t param = 0;
    uint8_t name_len = 0;
    int str_len = 0;
    char *p_tmp = NULL, *pbuf = (char *)bLogBuff;
    char line_number[8];
    va_list ap;

    if(type >= 4)   
    {
        return;
    }        
    
    memset(pbuf, 0, B_LOG_BUF_SIZE);
    param = bLogParamTable[type];

    if(type < 3)
    {
        pbuf[buf_len++] = bLogPrefix[type];
        pbuf[buf_len++] = ':';
        if(param & B_LOG_PARAM_FILE)
        {
            name_len = strlen(ptr_file);
            p_tmp = (char *)ptr_file;
            if(name_len > 10)
            {
                p_tmp = (char *)(ptr_file + name_len - 10);
                name_len = 10;
            }
            memcpy(pbuf + buf_len, p_tmp, name_len);
            buf_len += name_len;
            pbuf[buf_len++] = ' ';
        }
        if(param & B_LOG_PARAM_FUNC)
        {
            p_tmp = (char *)ptr_func;
            name_len = strlen(ptr_func);
            memcpy(pbuf + buf_len, p_tmp, name_len);
            buf_len += name_len;
            pbuf[buf_len++] = ' ';
        }
        if(param & B_LOG_PARAM_LINE)
        {
            str_len = sprintf(line_number, "%d", line);
            if(str_len < 0)
            {
                return;
            }
            memcpy(pbuf + buf_len, line_number, str_len);
            buf_len += str_len;
            pbuf[buf_len++] = ' ';
        }
    }
    va_start(ap, fmt);
    str_len = vsnprintf(pbuf + buf_len, B_LOG_BUF_SIZE - buf_len - 1, fmt, ap);
    va_end(ap);
    if(str_len < 0)
    {
        return;
    }
    bLogOutput(pbuf);
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




