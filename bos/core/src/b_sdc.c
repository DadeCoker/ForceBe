/**
 *!
 * \file        b_sdc.c
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
#include "b_sdc.h"
#if (_SAVE_DATA_ENABLE && _SAVE_DATA_C_ENABLE)
#include "b_core.h"
#include "b_device.h"
#include <string.h>
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup SAVE_DATA
 * \{
 */

/** 
 * \defgroup SDC_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SDC_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SDC_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SDC_Private_Variables
 * \{
 */

static bSDC_Info_t bSDC_InfoTable[_SAVE_DATA_C_I_NUMBER];
static uint32_t bSDC_InfoIndex = 0;

/**
 * \}
 */
   
/** 
 * \defgroup SDC_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SDC_Private_Functions
 * \{
 */

static int _bSDC_Count(int no)
{
    uint32_t i = 0, addr;
    uint8_t head;
    int retval = -1;
    
    int d_fd = -1;
    d_fd = bOpen(bSDC_InfoTable[no].dev_no, BCORE_FLAG_R);
    if(d_fd < 0)
    {
        return -1;
    }
    for(i = 0;i < bSDC_InfoTable[no].max_num;i++)
    {
        addr = i * (bSDC_InfoTable[no].st.usize + 1) + bSDC_InfoTable[no].st.address;
        bLseek(d_fd, addr);
        retval = bRead(d_fd, &head, 1);
        if(0 == retval)
        {
            if(head != bSDC_HEAD)
            {
                break;
            }
        }
        else
        {
            bClose(d_fd);
            return -1;
        }
    }
    bSDC_InfoTable[no].index = i;
    bClose(d_fd);
    return 0;
}




/**
 * \}
 */
   
/** 
 * \addtogroup SDC_Exported_Functions
 * \{
 */

/**
 * \brief Create a SDC instance
 * \param st Information for Creating instance \ref bSDC_Struct_t
 * \param dev_no Device number
 * \retval Instance ID
 *          \arg >=0  valid
 *          \arg -1   invalid
 */
int bSDC_Regist(bSDC_Struct_t st, uint8_t dev_no)
{
    if(bSDC_InfoIndex >= _SAVE_DATA_C_I_NUMBER || st.size <= st.usize || (st.size & (st.esize - 1)))
    {
        return -1;    
    }
    
    bSDC_InfoTable[bSDC_InfoIndex].flag = bSDC_HEAD;
    bSDC_InfoTable[bSDC_InfoIndex].max_num = st.size / (st.usize + 1);
    bSDC_InfoTable[bSDC_InfoIndex].index = 0;

    bSDC_InfoTable[bSDC_InfoIndex].st.address = st.address;
    bSDC_InfoTable[bSDC_InfoIndex].st.esize = st.esize;
    bSDC_InfoTable[bSDC_InfoIndex].st.size = st.size;
    bSDC_InfoTable[bSDC_InfoIndex].st.usize = st.usize;

    bSDC_InfoTable[bSDC_InfoIndex].dev_no = dev_no;

    if(_bSDC_Count(bSDC_InfoIndex) < 0)
    {
        return -1;
    }
    
    bSDC_InfoIndex += 1;
    return (bSDC_InfoIndex - 1);
}


/**
 * \brief Save data ClassC Write
 * \param no Instance ID \ref bSDC_Regist
 * \param pbuf Pointer to data buffer
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bSDC_Write(int no, uint8_t *pbuf)
{
    uint8_t head = bSDC_HEAD;
    int retval = -1;
    uint32_t addr = 0;
    
    if(pbuf == NULL || no >= bSDC_InfoIndex || no < 0)
    {
        return -1;
    }

    if(bSDC_InfoTable[no].flag != bSDC_HEAD)
    {
        return -1;
    }

    addr = bSDC_InfoTable[no].index * (bSDC_InfoTable[no].st.usize + 1) + bSDC_InfoTable[no].st.address;

    int d_fd = -1;
    d_fd = bOpen(bSDC_InfoTable[no].dev_no, BCORE_FLAG_RW);
    if(d_fd < 0)
    {
        return -1;
    }
    
	bCMD_Struct_t cmd_s;
    cmd_s.type = bCMD_ERASE;	
	cmd_s.param.erase.num = 1;
    if(bSDC_InfoTable[no].index == 0)
    {
        cmd_s.param.erase.addr = addr;
        bCtl(d_fd, bCMD_ERASE, &cmd_s);
    }
    else if((addr / bSDC_InfoTable[no].st.esize) != ((addr + bSDC_InfoTable[no].st.usize + 1) / bSDC_InfoTable[no].st.esize))
    {
        cmd_s.param.erase.addr = addr + bSDC_InfoTable[no].st.usize + 1;
        bCtl(d_fd, bCMD_ERASE, &cmd_s);
    }

    bLseek(d_fd, addr);
    retval = bWrite(d_fd, &head, 1);
    if(retval >= 0)
    {
        retval = bWrite(d_fd, pbuf, bSDC_InfoTable[no].st.usize);
    }
    bSDC_InfoTable[no].index = (bSDC_InfoTable[no].index + 1) % (bSDC_InfoTable[no].max_num);
    bClose(d_fd);
    return retval;
}


/**
 * \brief Save data ClassC Read
 * \param no Instance ID \ref bSDC_Regist
 * \param pbuf Pointer to data buffer
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bSDC_Read(int no, uint32_t index, uint8_t *pbuf)
{
    uint8_t head = 0;
    uint32_t addr = 0;
    int retval = 0;
   
    if(pbuf == NULL || no >= bSDC_InfoIndex || no < 0)
    {
        return -1;
    }

    if(bSDC_InfoTable[no].flag != bSDC_HEAD || bSDC_InfoTable[no].max_num <= index)
    {
        return -1;
    }
    addr = index * (bSDC_InfoTable[no].st.usize + 1) + bSDC_InfoTable[no].st.address;
    int d_fd = -1;
    d_fd = bOpen(bSDC_InfoTable[no].dev_no, BCORE_FLAG_RW);
    if(d_fd < 0)
    {
        return -1;
    }
    bLseek(d_fd, addr);
    retval = bRead(d_fd, &head, 1);
    if(head != bSDC_HEAD)
    {
        retval = -1;
    }
    
    if(retval >= 0)
    {
        retval = bRead(d_fd, pbuf, bSDC_InfoTable[no].st.usize);
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
  

#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/


