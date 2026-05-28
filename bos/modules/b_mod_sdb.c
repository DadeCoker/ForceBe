/**
 *!
 * \file        b_mod_sdb.c
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
#include "b_mod_sdb.h"
#if (_SAVE_DATA_ENABLE && _SAVE_DATA_B_ENABLE)
#include "b_core.h"
#include "b_driver.h"
#include <string.h>
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup MODULES
 * \{
 */

/** 
 * \addtogroup SAVE_DATA
 * \{
 */


/** 
 * \defgroup SDB_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SDB_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SDB_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SDB_Private_Variables
 * \{
 */



/**
 * \}
 */
   
/** 
 * \defgroup SDB_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SDB_Private_Functions
 * \{
 */
static uint8_t _bSDB_CalSUM(uint8_t *pbuf, uint32_t len)
{
    uint8_t tmp;
    uint32_t i;
    tmp = pbuf[0];
    for(i = 1;i < len;i++)
    {
        tmp += pbuf[i];
    }
    return tmp;
}


/**
 * \}
 */
   
/** 
 * \addtogroup SDB_Exported_Functions
 * \{
 */


/**
 * \brief Save data ClassB Write
 * \param pSDB_Instance Pointer to the SDB instance
 * \param pbuf Pointer to data buffer
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bSDB_Write(bSDB_Instance_t *pSDB_Instance, uint8_t *pbuf)
{
    uint8_t head = bSDB_HEAD, sum = 0;
    int retval = 0;
             
    if(pbuf == NULL || pSDB_Instance == NULL)
    {
        return -1;
    }
    
    if(pSDB_Instance->flag != bSDB_HEAD)
    {
        return -1;
    }

    int d_fd = -1;
    d_fd = bOpen(pSDB_Instance->dev_no, BCORE_FLAG_RW);
    if(d_fd < 0)
    {
        return -1;
    }

	bCMD_Erase_t cmd_s;
	cmd_s.addr = pSDB_Instance->address;
	cmd_s.num = 1;

    retval = bCtl(d_fd, bCMD_ERASE_SECTOR, &cmd_s);
    if(retval >= 0)
    {
        bLseek(d_fd, pSDB_Instance->address);
        retval = bWrite(d_fd, &head, 1);
    }
    sum = _bSDB_CalSUM(pbuf, pSDB_Instance->usize);
    
    if(retval >= 0)
    {
        retval = bWrite(d_fd, pbuf, pSDB_Instance->usize);
    }

    if(retval >= 0)
    {
        retval = bWrite(d_fd, &sum, 1);
    }
    bClose(d_fd);
    return retval;
}




/**
 * \brief Save data ClassB Read
 * \param pSDB_Instance Pointer to the SDB instance
 * \param pbuf Pointer to data buffer
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bSDB_Read(bSDB_Instance_t *pSDB_Instance, uint8_t *pbuf)
{
    uint8_t head = 0, sum = 0, r_sum = 0;
    int retval = 0;
            
    if(pbuf == NULL || pSDB_Instance == NULL)
    {
        return -1;
    }
    
    if(pSDB_Instance->flag != bSDB_HEAD)
    {
        return -1;
    }

    int d_fd = -1;
    d_fd = bOpen(pSDB_Instance->dev_no, BCORE_FLAG_R);
    if(d_fd < 0)
    {
        return -1;
    }

    bLseek(d_fd, pSDB_Instance->address);
    
    retval = bRead(d_fd, &head, 1);
    if(head != bSDB_HEAD)
    {
        retval = -1;
    }
    
    if(retval >= 0)
    {
        retval = bRead(d_fd, pbuf, pSDB_Instance->usize);
        sum = _bSDB_CalSUM(pbuf, pSDB_Instance->usize);
    }

    if(retval >= 0)
    {
        retval = bRead(d_fd, &r_sum, 1);
    }

    if(sum != r_sum)
    {
        retval = -1;
    }
    bClose(d_fd);  
    return retval;
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

/**
 * \}
 */ 

#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/



