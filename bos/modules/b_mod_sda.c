/**
 *!
 * \file        b_mod_sda.c
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
#include "b_mod_sda.h"
#if (_SAVE_DATA_ENABLE && _SAVE_DATA_A_ENABLE)
#include "b_mod_utc.h"
#include "b_core.h"
#include "b_device.h"
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
 * \defgroup SDA_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup SDA_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SDA_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SDA_Private_Variables
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup SDA_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SDA_Private_Functions
 * \{
 */
static uint32_t _bSDA_CalAddress(bSDA_Instance_t *pSDA_Instance, uint32_t utc)
{
    uint32_t address, num;
    uint32_t unum = 0, uoff = 0;
    
    address = pSDA_Instance->st.fbase_address;
    num = (utc % pSDA_Instance->st.total_time) / (pSDA_Instance->st.time_interval);
    if(num == 0)
    {
        unum = pSDA_Instance->un_number - 1;
        uoff = pSDA_Instance->n_per_eu - 1;
    }
    else
    {
        num = num - 1;
        unum = num / (pSDA_Instance->n_per_eu);
        uoff = num % (pSDA_Instance->n_per_eu);
    }
    address += unum * pSDA_Instance->st.fsector_size + uoff * pSDA_Instance->st.data_size;
    return address;
}



/**
 * \}
 */
   
/** 
 * \addtogroup SDA_Exported_Functions
 * \{
 */

/**
 * \brief Initialize bSDA instance
 * \param pSDA_Instance Pointer to the bSDA instance
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bSDA_Init(bSDA_Instance_t *pSDA_Instance)
{
    if(pSDA_Instance == NULL)
    {
        return -1;
    }
    if(pSDA_Instance->st.data_size > pSDA_Instance->st.fsector_size)
    {
        return -2;
    }
    pSDA_Instance->n_per_eu = pSDA_Instance->st.fsector_size / pSDA_Instance->st.data_size;
    pSDA_Instance->un_number = ((pSDA_Instance->st.total_time / pSDA_Instance->st.time_interval) + (pSDA_Instance->n_per_eu - 1)) / pSDA_Instance->n_per_eu;
    pSDA_Instance->total_size = pSDA_Instance->un_number * pSDA_Instance->st.fsector_size;
    b_log_e("sda total size %d\r\n", pSDA_Instance->total_size);
    pSDA_Instance->e_flag = 1;
    return 0;
}


/**
 * \brief Save data ClassA Write
 * \param pSDA_Instance Pointer to the bSDA instance
 * \param utc Current time UTC_2000 (s)
 * \param pbuf Pointer to data buffer
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bSDA_Write(bSDA_Instance_t *pSDA_Instance, uint32_t utc, uint8_t *pbuf)
{
    uint32_t address = 0;
    int retval = -1;
    
    if(pbuf == NULL || pSDA_Instance == NULL)
    {
        return -1;
    }
    address = _bSDA_CalAddress(pSDA_Instance, utc);

    int d_fd = -1;
    d_fd = bOpen(pSDA_Instance->dev_no, BCORE_FLAG_RW);
    if(d_fd < 0)
    {
        return -1;
    }
    
	bCMD_Struct_t cmd_s;
	cmd_s.param.erase.addr = address;
	cmd_s.param.erase.num = 1;
    bLseek(d_fd, address);

    if((address % (pSDA_Instance->st.fsector_size) == 0) || pSDA_Instance->e_flag == 1)
    {
        bCtl(d_fd, bCMD_ERASE, &cmd_s);
        pSDA_Instance->e_flag = 0;
    }
    retval = bWrite(d_fd, pbuf, pSDA_Instance->st.data_size);
    bClose(d_fd);
    return retval;
}


/**
 * \brief Save data ClassA Read
 * \param pSDA_Instance Pointer to the bSDA instance
 * \param utc Current time UTC_2000 (s)
 * \param pbuf Pointer to data buffer
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bSDA_Read(bSDA_Instance_t *pSDA_Instance, uint32_t utc, uint8_t *pbuf)
{
    uint32_t address = 0;
    int retval = -1;
    
    if(pbuf == NULL || pSDA_Instance == NULL)
    {
        return -1;
    }
    address = _bSDA_CalAddress(pSDA_Instance, utc);
    int d_fd = -1;
    d_fd = bOpen(pSDA_Instance->dev_no, BCORE_FLAG_R);
    if(d_fd < 0)
    {
        return -1;
    }
    bLseek(d_fd, address);
    retval = bRead(d_fd, pbuf, pSDA_Instance->st.data_size);
    bClose(d_fd);
    return retval;
}

/**
 * \brief Call this function after setting time
 * \param pSDA_Instance Pointer to the bSDA instance
 * \param o_utc old UTC
 * \param n_utc new UTC
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bSDA_TimeChanged(bSDA_Instance_t *pSDA_Instance, uint32_t o_utc, uint32_t n_utc)
{
    uint32_t address_o, address_n;
    
    if(pSDA_Instance == NULL)
    {
        return -1;
    }

    address_o = _bSDA_CalAddress(pSDA_Instance, o_utc);
    address_n = _bSDA_CalAddress(pSDA_Instance, n_utc);
    

    if(address_o / pSDA_Instance->st.fsector_size != address_n / pSDA_Instance->st.fsector_size)
    {
        pSDA_Instance->e_flag = 1;
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

/**
 * \}
 */
#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/

