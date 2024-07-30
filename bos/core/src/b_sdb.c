/**
 *!
 * \file        b_sdb.c
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
#include "b_sdb.h"
#if (_SAVE_DATA_ENABLE && _SAVE_DATA_B_ENABLE)
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

static bSDB_Info_t bSDB_InfoTable[_SAVE_DATA_B_I_NUMBER];
static uint32_t bSDB_InfoIndex = 0;

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
static uint8_t _bSDB_CalSUM(uint8_t *pbuf, uint8_t len)
{
    uint8_t tmp, i;
    tmp = pbuf[0];
    for(i = 1;i < len;i++)
    {
        tmp ^= pbuf[i];
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
 * \brief Create a SDB instance
 * \param address Specify base address
 * \param usize Size of saved data
 * \param dev_no Device number
 * \retval Instance ID
 *          \arg >=0  valid
 *          \arg -1   invalid
 */
int bSDB_Regist(uint32_t address, uint32_t usize, uint8_t dev_no)
{
    if(bSDB_InfoIndex >= _SAVE_DATA_B_I_NUMBER || usize == 0)
    {
        return -1;    
    }
    bSDB_InfoTable[bSDB_InfoIndex].flag = bSDB_HEAD;
    bSDB_InfoTable[bSDB_InfoIndex].usize = usize;
    bSDB_InfoTable[bSDB_InfoIndex].address = address;
    bSDB_InfoTable[bSDB_InfoIndex].dev_no = dev_no;
    bSDB_InfoIndex += 1;
    return (bSDB_InfoIndex - 1);
}



/**
 * \brief Save data ClassB Write
 * \param no Instance ID \ref bSDB_Regist
 * \param pbuf Pointer to data buffer
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bSDB_Write(int no, uint8_t *pbuf)
{
    uint8_t head = bSDB_HEAD, sum = 0;
    int retval = 0;
             
    if(pbuf == NULL || no >= bSDB_InfoIndex || no < 0)
    {
        return -1;
    }
    
    if(bSDB_InfoTable[no].flag != bSDB_HEAD)
    {
        return -1;
    }

    int d_fd = -1;
    d_fd = bOpen(bSDB_InfoTable[no].dev_no, BCORE_FLAG_RW);
    if(d_fd < 0)
    {
        return -1;
    }

	bCMD_Struct_t cmd_s;
    cmd_s.type = bCMD_ERASE;
	cmd_s.param.erase.addr = bSDB_InfoTable[no].address;
	cmd_s.param.erase.num = 1;

    retval = bCtl(d_fd, bCMD_ERASE, &cmd_s);
    if(retval >= 0)
    {
        bLseek(d_fd, bSDB_InfoTable[no].address);
        retval = bWrite(d_fd, &head, 1);
    }
    sum = _bSDB_CalSUM(pbuf, bSDB_InfoTable[no].usize);
    
    if(retval >= 0)
    {
        retval = bWrite(d_fd, pbuf, bSDB_InfoTable[no].usize);
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
 * \param no Instance ID \ref bSDB_Regist
 * \param pbuf Pointer to data buffer
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bSDB_Read(int no, uint8_t *pbuf)
{
    uint8_t head = 0, sum = 0, r_sum = 0;
    int retval = 0;
            
    if(pbuf == NULL || no >= bSDB_InfoIndex || no < 0)
    {
        return -1;
    }
    
    if(bSDB_InfoTable[no].flag != bSDB_HEAD)
    {
        return -1;
    }

    int d_fd = -1;
    d_fd = bOpen(bSDB_InfoTable[no].dev_no, BCORE_FLAG_R);
    if(d_fd < 0)
    {
        return -1;
    }

    bLseek(d_fd, bSDB_InfoTable[no].address);
    
    retval = bRead(d_fd, &head, 1);
    if(head != bSDB_HEAD)
    {
        retval = -1;
    }
    
    if(retval >= 0)
    {
        retval = bRead(d_fd, pbuf, bSDB_InfoTable[no].usize);
        sum = _bSDB_CalSUM(pbuf, bSDB_InfoTable[no].usize);
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

#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/



