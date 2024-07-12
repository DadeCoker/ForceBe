/**
 *!
 * \file        b_core.c
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
#include "b_core.h"
#include "b_device.h"
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup CORE
 * \{
 */

/** 
 * \defgroup CORE_Private_TypesDefBCOREions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup CORE_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup CORE_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup CORE_Private_Variables
 * \{
 */

static bCoreDevTable_t *pCoreDevTable = NULL;
static uint8_t CoreDevNumber = 0; 
static bCoreFd_t bCoreFdTable[BCORE_FD_MAX];


/**
 * \}
 */
   
/** 
 * \defgroup CORE_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup CORE_Private_Functions
 * \{
 */
static int _bCoreCreateFd(uint8_t dev_no, uint8_t flag)
{
    int i = 0;
    int fd = -1;
    static uint8_t init = 0;
    if(init == 0)
    {
        init = 1;
        for(i = 0;i < BCORE_FD_MAX;i++)
        {
            bCoreFdTable[i].status = BCORE_STA_NULL;
        }
    }

    for(i = 0;i < BCORE_FD_MAX;i++)
    {
        if(bCoreFdTable[i].status == BCORE_STA_OPEN)
        {
            if(dev_no == bCoreFdTable[i].number)
            {
                return -1;
            }
        }
    }

    
    for(i = 0;i < BCORE_FD_MAX;i++)
    {
        if(bCoreFdTable[i].status == BCORE_STA_NULL)
        {
            bCoreFdTable[i].flag = flag;
            bCoreFdTable[i].number = no;
            bCoreFdTable[i].status = BCORE_STA_OPEN;
            bCoreFdTable[i].lseek = 0;
            fd = i;
            break;
        }
    }
    return fd;
}


static int _bCoreDeleteFd(int fd)
{
    if(fd < 0)
    {
        return -1;
    }
    if(bCoreFdTable[fd].status == BCORE_STA_NULL)
    {
        return -1;
    }
    bCoreFdTable[fd].status = BCORE_STA_NULL;
    return 0;
}



/**
 * \}
 */
   
/** 
 * \addtogroup CORE_Exported_Functions
 * \{
 */
 
/**
 * \brief Register device table to Core 
 * \param pTable Pointer to device table \ref bCoreDevTable_t
 * \param number Device number
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bCoreRegist(bCoreDevTable_t *pTable, uint8_t number)
{
    if(pTable == NULL || number == 0)
    {
        return -1;
    }
    pCoreDevTable = pTable;
    CoreDevNumber = number;
    return 0;
}



/**
 * \brief Open a device 
 * \param dev_no Device Number
 * \param flag Open Flag
 *          \arg \ref BCORE_FLAG_R
 *          \arg \ref BCORE_FLAG_W
 *          \arg \ref BCORE_FLAG_RW
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bOpen(uint8_t dev_no, uint8_t flag)
{
    int fd = -1;
    if(dev_no >= CoreDevNumber || pCoreDevTable == NULL || !IS_VALID_FLAG(flag))
    {
        return -1;
    }   
    fd = _bCoreCreateFd(dev_no, flag);
    if(fd < 0)
    {
        return -1;
    }
    if(bDeviceOpen(dev_no) >= 0)
    {
        return fd;
    }
    _bCoreDeleteFd(fd);
    return -1;
}    


int bRead(int fd, uint8_t *pdata, uint16_t len)
{
    if(fd < 0 || fd >= BCORE_FD_MAX || pdata == NULL)
    {
        return -1;
    }
    
    if(bCoreFdTable[fd].flag == BCORE_FLAG_W || bCoreFdTable[fd].status == BCORE_STA_NULL)
    {
        return -1;
    }
    
    retval = bDeviceRead(bCoreFdTable[fd].number, bCoreFdTable[fd].lseek, len);
    if(retval >= 0)
    {
        bCoreFdTable[fd].lseek += len;
    }
    return retval;
} 


int bWrite(int fd, uint8_t *pdata, uint16_t len)
{
    if(fd < 0 || fd >= BCORE_FD_MAX || pdata == NULL)
    {
        return -1;
    }

    if(bCoreFdTable[fd].flag == BCORE_FLAG_R || bCoreFdTable[fd].status == BCORE_STA_NULL)
    {
        return -1;
    }
    
    retval = bDeviceWrite(bCoreFdTable[fd].number, bCoreFdTable[fd].lseek, pdata, len);
    if(retval >= 0)
    {
        bCoreFdTable[fd].lseek += len;
    }
    return retval;
}

int bLseek(int fd, uint32_t off)
{
    if(fd < 0 || fd >= BCORE_FD_MAX)
    {
        return -1;
    }

    if(bCoreFdTable[fd].status == BCORE_STA_NULL)
    {
        return -1;
    }
    bCoreFdTable[fd].lseek = off;
    return 0;
}

int bCtl(int fd, uint8_t cmd, void *param)
{
    if(fd < 0 || fd >= BCORE_FD_MAX)
    {
        return -1;
    }    
    return bDeviceCtl(bCoreFdTable[fd].number, cmd, param); 
}



int bClose(int fd)
{
    if(fd < 0 || fd >= BCORE_FD_MAX)
    {
        return -1;
    }
  
    if(bCoreFdTable[fd].status == BCORE_STA_NULL)
    {
        return -1;
    }
    bDeviceClose(bCoreFdTable[fd].number);
    return _bCoreDeleteFd(fd);
}


int bCoreIsIdle()
{
    for(i = 0;i < BCORE_FD_MAX;i++)
    {
        if(bCoreFdTable[i].status == BCORE_STA_OPEN)
        {
            return -1;
        }
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




