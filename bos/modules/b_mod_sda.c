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

static bSDA_Info_t bSDA_InfoTable[_SAVE_DATA_A_I_NUMBER];
static uint8_t bSDA_InfoIndex = 0;


static const uint8_t uNumbers[6] = {60, 60, 24, 31, 12, 0xff};

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
static int _bSDA_CheckInfo(bSDA_Info_t *pinfo)
{
    if(pinfo->base_address >= pinfo->data_address)
    {
        return -1;
    }
    return 0;
}


static int _bSDA_GetTimeValue(uint8_t unit, uint32_t utc)
{
    bUTC_DateTime_t tm;
    int retval = -1;
    if(!IS_UNIT_VALID(unit))
    {
        return -1;
    }
    
    bUTC2Struct( &tm, utc);
    
    switch(unit)
    {
        case UNIT_SECOND:
            retval = tm.second;
            break;
        case UNIT_MINUTE:
            retval = tm.minute;
            break;
        case UNIT_HOUR:
            retval = tm.hour;
            break;
        case UNIT_DAY:
            retval = tm.day - 1;
            break;
        case UNIT_MONTH:
            retval = tm.month - 1;
            break;
        case UNIT_YEAR:
            retval = tm.year;
            break;
        default:
            break;
    }
    return retval;
}



static int _bSDA_CalOffset(bSDA_Info_t *pinfo, uint32_t utc, uint32_t *poffset)
{
    uint32_t tmp[UNIT_NUMBER];
    int retval;
    int8_t unit;
    
    unit = pinfo->st.min_unit;
    tmp[unit] = pinfo->st.min_size;
    unit += 1;
    tmp[unit] = pinfo->un_number * pinfo->st.ferase_size;
    while(unit != pinfo->st.total_unit)
    {
        unit++;
        tmp[unit] = uNumbers[unit - 1] * tmp[unit - 1];
    }
    
    uint32_t offset = 0;
    unit--;
    while(unit >= pinfo->st.min_unit)
    {
        retval = _bSDA_GetTimeValue(unit, utc);
        if(retval < 0)
        {
            return -1;
        }
		if(unit == pinfo->st.min_unit)
		{
			retval = retval / pinfo->st.min_number;
		}
        offset += (uint32_t)retval * tmp[unit]; 
        unit--;
    }
    *poffset = offset;
    return 0;
}


static int _bSDA_CalAddress(bSDA_Info_t *pinfo, uint32_t utc, uint32_t *paddr)
{
    uint32_t address;
    uint32_t per_size = 0, offset = 0;
    int retval;
    
    address = pinfo->data_address;
    if(pinfo->st.total_number > 1)
    {
        per_size = pinfo->total_size / pinfo->st.total_number;
        retval = _bSDA_GetTimeValue(pinfo->st.total_unit, utc);
        retval = retval % pinfo->st.total_number;
        address += retval * per_size;
    }    
    
    if(pinfo->st.total_unit != pinfo->st.min_unit)
    {
        if(_bSDA_CalOffset(pinfo, utc, &offset) < 0)
        {
            return -1;
        }
        address += offset;
    }
    *paddr = address;
    return 0;
}



/**
 * \}
 */
   
/** 
 * \addtogroup SDA_Exported_Functions
 * \{
 */

/**
 * \brief Create a SDA instance
 * \param st Information for Creating instance \ref bSDA_Struct_t
 * \param dev_no Device number
 * \retval Instance ID
 *          \arg >=0  valid
 *          \arg -1   invalid
 */
int bSDA_Regist(bSDA_Struct_t st, uint8_t dev_no)
{
    uint8_t unit = st.min_unit;
    if(bSDA_InfoIndex >= _SAVE_DATA_A_I_NUMBER || (!IS_UNIT_VALID(st.min_unit)) || (!IS_UNIT_VALID(st.total_unit)))
    {
        return -1;
    }
    
    if(st.min_size > st.ferase_size)
    {
        return -2;
    }

    bSDA_Info_t *pinfo = &bSDA_InfoTable[bSDA_InfoIndex];
    
    pinfo->n_per_eu = st.ferase_size / st.min_size;
    pinfo->un_number = ((uNumbers[st.min_unit] / st.min_number) + (pinfo->n_per_eu - 1)) / pinfo->n_per_eu;
    
    if(st.total_unit == st.min_unit)
    {
        pinfo->total_size = st.total_number * st.ferase_size;
    }
    else if(st.total_unit > st.min_unit)
    {
        unit = st.min_unit;
        pinfo->total_size = pinfo->un_number * st.ferase_size;
        unit++;
        while(unit != st.total_unit)
        {
            pinfo->total_size = pinfo->total_size * uNumbers[unit];
            unit++;
        }
        pinfo->total_size = pinfo->total_size * st.total_number;
    }
    else
    {
        return -1;
    }
    pinfo->total_size += st.ferase_size;
    if(pinfo->total_size > st.fsize)
    {
        b_log_e("please rearrange total size %d-->%d\r\n", st.fsize, pinfo->total_size);
        return -1;
    }
    pinfo->base_address = st.fbase_address;
    pinfo->data_address = st.fbase_address + st.ferase_size;
    pinfo->dev_no = dev_no;
    pinfo->e_flag = 1;
    memcpy(&(pinfo->st), &st, sizeof(bSDA_Struct_t));
    b_log("peu:%d\tunn:%d\tba:%d\tts:%d\r\n", pinfo->n_per_eu, pinfo->un_number, pinfo->base_address, pinfo->total_size);
    bSDA_InfoIndex += 1;
    return (bSDA_InfoIndex - 1);
}


/**
 * \brief Save data ClassA Write
 * \param no Instance ID \ref bSDA_Regist
 * \param utc Current time UTC_2000 (s)
 * \param pbuf Pointer to data buffer
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bSDA_Write(int no, uint32_t utc, uint8_t *pbuf)
{
    uint32_t address = 0;
    int retval = -1;
    
    if(pbuf == NULL || no >= bSDA_InfoIndex || no < 0)
    {
        return -1;
    }
    
    bSDA_Info_t *pinfo = &bSDA_InfoTable[no];
    
    if(_bSDA_CheckInfo(pinfo) < 0)
    {
        return -1;
    }
  
    if(_bSDA_CalAddress(pinfo, utc, &address) < 0)
    {
        return -1;
    }

    int d_fd = -1;
    d_fd = bOpen(pinfo->dev_no, BCORE_FLAG_RW);
    if(d_fd < 0)
    {
        return -1;
    }
    
	bCMD_Struct_t cmd_s;
	cmd_s.param.erase.addr = address;
	cmd_s.param.erase.num = 1;
	
    bLseek(d_fd, address);
    if(pinfo->st.total_unit == pinfo->st.min_unit)
    {
        bCtl(d_fd, bCMD_ERASE, &cmd_s);
        retval = bWrite(d_fd, pbuf, pinfo->st.min_size);
    }
    else 
    {
        if((address % (pinfo->st.ferase_size) == 0) || ((address + pinfo->st.min_size) / pinfo->st.ferase_size) != (address / pinfo->st.ferase_size) 
            || pinfo->e_flag == 1)
        {
            bCtl(d_fd, bCMD_ERASE, &cmd_s);
            pinfo->e_flag = 0;
        }
        retval = bWrite(d_fd, pbuf, pinfo->st.min_size);
    }
    bClose(d_fd);
    return retval;
}


/**
 * \brief Save data ClassA Read
 * \param no Instance ID \ref bSDA_Regist
 * \param utc Current time UTC_2000 (s)
 * \param pbuf Pointer to data buffer
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bSDA_Read(int no, uint32_t utc, uint8_t *pbuf)
{
    uint32_t address = 0;
    int retval = -1;
    
    if(pbuf == NULL || no >= bSDA_InfoIndex || no < 0)
    {
        return -1;
    }
    
    bSDA_Info_t *pinfo = &bSDA_InfoTable[no];
    if(_bSDA_CheckInfo(pinfo) < 0)
    {
        return -1;
    }
    
    if(_bSDA_CalAddress(pinfo, utc, &address) < 0)
    {
        return -1;
    }
    
    int d_fd = -1;
    d_fd = bOpen(pinfo->dev_no, BCORE_FLAG_R);
    if(d_fd < 0)
    {
        return -1;
    }
    bLseek(d_fd, address);
    retval = bRead(d_fd, pbuf, pinfo->st.min_size);
    bClose(d_fd);
    return retval;
}

/**
 * \brief Call this function after setting time
 * \param no Instance ID \ref bSDA_Regist
 * \param o_utc old UTC
 * \param n_utc new UTC
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bSDA_TimeChanged(int no, uint32_t o_utc, uint32_t n_utc)
{
    uint32_t address_o, address_n;
    
    if(no >= bSDA_InfoIndex || no < 0)
    {
        return -1;
    }
    bSDA_Info_t *pinfo = &bSDA_InfoTable[no];
    if(_bSDA_CalAddress(pinfo, o_utc, &address_o) < 0)
    {
        return -1;
    }
    
    if(_bSDA_CalAddress(pinfo, n_utc, &address_n) < 0)
    {
        return -1;
    }
    
    if(address_o / pinfo->st.ferase_size != address_n / pinfo->st.ferase_size)
    {
        pinfo->e_flag = 1;
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

