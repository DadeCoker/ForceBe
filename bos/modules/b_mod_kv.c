/**
 *!
 * \file        b_mod_kv.c
 * \version     v0.0.2
 * \date        2020/04/09
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO KV SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
   
/*Includes ----------------------------------------------*/
#include "b_mod_kv.h"
#if _KV_ENABLE
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
 * \addtogroup KV
 * \{
 */

/** 
 * \defgroup KV_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup KV_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup KV_Private_Macros
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup KV_Private_Variables
 * \{
 */
static bKV_Info_t bKV_Info; 
const static bKV_Index_t InvalidIndex = {
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
};
/**
 * \}
 */
   
/** 
 * \defgroup KV_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup KV_Private_Functions
 * \{
 */

static uint32_t _bKV_GenerateID(const char *key)
{
   uint32_t id = 0;
   uint16_t klen = strlen(key);
   while(klen) 
   {
        id = (id << 5) + id + *key++;
        klen -= 1;
   }
    return id;
}

static int _bKV_AllocateSpace(uint32_t size, uint32_t s_addr)
{
    uint32_t size_d2 = size / 5;
    uint32_t num = (size_d2 - strlen(bKV_HEAD_STR)) / sizeof(bKV_Index_t);
    if(num > 200)
    {
        num = 200;
    }
    if(num < 3)
    {
        b_log_e("please allocate more space\r\n");
        return -1;
    }
    
    bKV_Info.t_max = num;
    bKV_Info.ts_address = s_addr + strlen(bKV_HEAD_STR);
    bKV_Info.te_address = bKV_Info.ts_address + (num * sizeof(bKV_Index_t)) - 1;
    
    bKV_Info.ds_address = bKV_Info.te_address + 1;
    bKV_Info.d_size = size_d2 * 2;
    bKV_Info.de_address = bKV_Info.ds_address + bKV_Info.d_size - 1;
    bKV_Info.d_index = bKV_Info.ds_address;
    return 0;
}

static int _bKV_ISFirstTime(int fd)
{
    int retval = 0;
    uint8_t buf[16];
    uint8_t buf2[16];

    bLseek(fd, bKV_Info.str_address);
    retval = bRead(fd, buf, strlen(bKV_HEAD_STR));
    
    if(bKV_Info.e_size > 0)
    {
        bLseek(fd, bKV_Info.str_address + bKV_Info.e_size);
        retval = bRead(fd, buf2, strlen(bKV_HEAD_STR));
    }
    
    if(retval > 0)
    {
        if(strncmp(bKV_HEAD_STR, (const char *)buf, strlen(bKV_HEAD_STR)) == 0 || 
            strncmp(bKV_HEAD_STR, (const char *)buf2, strlen(bKV_HEAD_STR)) == 0)
        {
            return -1;
        }
    }
    else
    {
        b_log_e("_bKV_ISNew read dev error\r\n");
        return -2;
    }
    return 0;
}

static int _bKV_ClearSector(int fd, uint8_t t)
{
    bCMD_Struct_t cmd;
    int retval = -1;
    if(bKV_Info.e_size == 0)
    {
        return 0;
    }

    if(t & bKV_SECTOR_T1)
    {
        cmd.param.erase.addr = bKV_Info.ts_address;
        cmd.param.erase.num = 1;
        retval = bCtl(fd, bCMD_ERASE, &cmd); 
    }
    if(t & bKV_SECTOR_T2)
    {
        cmd.param.erase.addr = bKV_Info.ts_address + bKV_Info.e_size;
        cmd.param.erase.num = 1;
        retval = bCtl(fd, bCMD_ERASE, &cmd); 
    }
    if(t & bKV_SECTOR_D1)
    {
        cmd.param.erase.addr = bKV_Info.ds_address;
        cmd.param.erase.num = bKV_Info.d_size / bKV_Info.e_size;
        retval = bCtl(fd, bCMD_ERASE, &cmd); 
    }
    if(t & bKV_SECTOR_D2)
    {
        cmd.param.erase.addr = bKV_Info.ds_address + bKV_Info.d_size;
        cmd.param.erase.num = bKV_Info.d_size / bKV_Info.e_size;
        retval = bCtl(fd, bCMD_ERASE, &cmd); 
    }
    return retval;
}

static int _bKV_AddHeadString(int fd)
{
    int retval = -1;
    bLseek(fd, bKV_Info.str_address + bKV_Info.e_size * bKV_Info.index);
    retval = bWrite(fd, (uint8_t *)bKV_HEAD_STR, strlen(bKV_HEAD_STR));
    return retval; 
}



static int _bKV_Locate(int fd, uint8_t t)
{
    bKV_Index_t tmp;
    uint16_t max_num = bKV_Info.t_max;
    uint32_t left = 1, right = max_num, index = max_num / 2;
    uint8_t buf[16];
    int i = 0;
    uint32_t addr = 0;
    if(t > 1 || (bKV_Info.e_size == 0 && t != 0))
    {
        return -1;
    }
    
    bLseek(fd, bKV_Info.str_address + bKV_Info.e_size * t);
    bRead(fd, buf, strlen(bKV_HEAD_STR));
    if(strncmp(bKV_HEAD_STR, (const char *)buf, strlen(bKV_HEAD_STR)))
    {
        return -1;
    }
    
    while(left < right)
    {
        index = left + ((right - left) / 2);
        bLseek(fd, bKV_Info.ts_address + t * bKV_Info.e_size + index * sizeof(bKV_Index_t));
        bRead(fd, (uint8_t *)&tmp, sizeof(bKV_Index_t));
        if(tmp.address == 0xFFFFFFFF || tmp.id == 0xFFFFFFFF || tmp.len == 0xFFFFFFFF)
        {
            right = index;
        }
        else
        {
            left = index + 1;
        }
    }
    
    if(right == 0)
    {
        bKV_Info.d_index = bKV_Info.ds_address;
    }
    else
    {
        if(bKV_Info.e_size == 0)
        {
            for(i = 0;i < right;i++)
            {
                bLseek(fd, bKV_Info.ts_address + t * bKV_Info.e_size + i * sizeof(bKV_Index_t));
                bRead(fd, (uint8_t *)&tmp, sizeof(bKV_Index_t));
                if(addr < (tmp.address + tmp.len))
                {
                    addr = tmp.address + tmp.len;
                }
            }
            bKV_Info.d_index = addr;
        }
        else
        {
            bLseek(fd, bKV_Info.ts_address + t * bKV_Info.e_size + (right - 1) * sizeof(bKV_Index_t));
            bRead(fd, (uint8_t *)&tmp, sizeof(bKV_Index_t));
            bKV_Info.d_index = tmp.address + tmp.len;
        }
    }
    return right;
}


static int _bKV_LoadInfo(int fd)
{
    int t0, t1;
    t0 = _bKV_Locate(fd, 0);
    t1 = _bKV_Locate(fd, 1);
    if(bKV_Info.e_size == 0)
    {
        if(bKV_Info.d_index > bKV_Info.de_address)
        {
            bKV_Info.index = 1;
        }
        if(t0 < 0)
        {
            return -1;
        }
        bKV_Info.t_index = t0;
    }
    else
    {
        if(t0 < 0 && t1 < 0)
        {
            return -1;
        }
        bKV_Info.t_index = t0;
        bKV_Info.index = 0;
        if(t1 > t0)
        {
            bKV_Info.index = 1;
            bKV_Info.t_index = t1;
        } 
    }    
    b_log("t_index:%d index:%d\r\n", bKV_Info.t_index, bKV_Info.index);    
    return 0;
}

static int _bKV_ISExist(int fd, uint32_t id, bKV_Index_t *pt)
{
    int i = 0;
    bKV_Index_t tmp;   
    
    for(i = (bKV_Info.t_index - 1);i >= 0; i--)
    {
        bLseek(fd, bKV_Info.ts_address + bKV_Info.index * bKV_Info.e_size + i * sizeof(bKV_Index_t));
        bRead(fd, (uint8_t *)&tmp, sizeof(bKV_Index_t));
        
        if(bKV_Info.e_size > 0 &&  tmp.statu != 0xffffffff)
        {
            continue;
        }
        
        if(tmp.id == id)
        {  
            memcpy(pt, &tmp, sizeof(bKV_Index_t));
            return i;
        }
    }
    return -1;
}

static int _bKV_MoveData(int fd, uint32_t s_addr, uint32_t d_addr, uint16_t size)
{
    uint8_t buf[64];
    uint16_t r_len, len = size;
    for(;;)
    {
        bLseek(fd, s_addr);
        r_len = (len > 64) ? 64 : len;
        bRead(fd, buf, r_len);
        len = len - r_len;
        s_addr += r_len;
        bLseek(fd, d_addr);
        bWrite(fd, buf, r_len);
        d_addr += r_len;
        if(len == 0)
        {
            break;
        }
    }
    return 0;
}


static int _bKV_ArrangeSpace(int fd)
{
    uint32_t i = 0;
    uint32_t addr = 0;
    bKV_Index_t tmp;
    uint32_t index_tmp = 0, t_addr;

    index_tmp = bKV_Info.index;
    bKV_Info.index = (bKV_Info.index + 1) % 2;
    
    t_addr = bKV_Info.ts_address + bKV_Info.index * bKV_Info.e_size;
    addr = bKV_Info.ds_address + bKV_Info.index * bKV_Info.d_size;    
    if(bKV_Info.e_size > 0)
    {
        bLseek(fd, bKV_Info.str_address + bKV_Info.index * bKV_Info.e_size);
        bWrite(fd, (uint8_t *)bKV_HEAD_STR, strlen(bKV_HEAD_STR));
    }
    
    for(i = 0;i < bKV_Info.t_index;i++)
    {
        bLseek(fd, bKV_Info.ts_address + i * sizeof(bKV_Index_t) + index_tmp * bKV_Info.e_size);
        bRead(fd, (uint8_t *)&tmp, sizeof(bKV_Index_t));
        if(tmp.statu != 0xffffffff && bKV_Info.e_size > 0)
        {
            continue;
        }
        _bKV_MoveData(fd, tmp.address, addr, tmp.len);
        tmp.address = addr;
        addr += tmp.len;
        bLseek(fd, t_addr);
        bWrite(fd, (uint8_t *)&tmp, sizeof(bKV_Index_t) - sizeof(uint32_t));
        t_addr += sizeof(bKV_Index_t);
    }

    bKV_Info.d_index = addr;
    bKV_Info.t_index = (t_addr - (bKV_Info.ts_address + bKV_Info.index * bKV_Info.e_size)) / sizeof(bKV_Index_t);
    if(index_tmp == 0)
    {
        _bKV_ClearSector(fd, bKV_SECTOR_T1 | bKV_SECTOR_D1);
    }
    else
    {
        _bKV_ClearSector(fd, bKV_SECTOR_T2 | bKV_SECTOR_D2);
    }
    b_log_w("t_index:%d index:%d\r\n",bKV_Info.t_index, bKV_Info.index );
    return 0;
}


static int _bKV_AddNew(int fd, uint32_t id, uint8_t *pbuf, uint16_t len)
{
    bKV_Index_t tmp;
    
    if(bKV_Info.d_index + len > (bKV_Info.de_address + bKV_Info.index * bKV_Info.d_size) || bKV_Info.t_index >= bKV_Info.t_max)
    {
        _bKV_ArrangeSpace(fd);
    }
    
    if(bKV_Info.d_index + len > (bKV_Info.de_address + bKV_Info.index * bKV_Info.d_size) || bKV_Info.t_index >= bKV_Info.t_max)
    {
        return -1;
    }
    
    tmp.address = bKV_Info.d_index;
    tmp.id = id;
    tmp.len = len;
  
    bLseek(fd, bKV_Info.ts_address + bKV_Info.index * bKV_Info.e_size + bKV_Info.t_index * sizeof(bKV_Index_t));
    bWrite(fd, (uint8_t *)&tmp, sizeof(bKV_Index_t) - sizeof(uint32_t));
    bKV_Info.d_index += len;
    bKV_Info.t_index += 1;
    bLseek(fd, tmp.address);
    bWrite(fd, pbuf, len);
    b_log_w("n: t_index:%d index:%d\r\n", bKV_Info.t_index, bKV_Info.index);  
    return 0;
}


static int _bKV_ModifyValue(int fd, uint32_t index, bKV_Index_t t, uint32_t id, uint8_t *pbuf, uint16_t len)
{
    int retval = 0;
    if(len > t.len || (bKV_Info.e_size > 0))
    {
        if(bKV_Info.d_index + len > (bKV_Info.de_address + bKV_Info.index * bKV_Info.d_size) || (bKV_Info.t_index >= bKV_Info.t_max && bKV_Info.e_size > 0))
        {
            _bKV_ArrangeSpace(fd);
        }
        if(bKV_Info.d_index + len > (bKV_Info.de_address + bKV_Info.index * bKV_Info.d_size) || (bKV_Info.t_index >= bKV_Info.t_max && bKV_Info.e_size > 0))
        {
            return -1;
        }
    }
      
    if(bKV_Info.e_size == 0)
    {
        if(len <= t.len)
        {
            t.len = len;
        }
        else
        {
            t.len = len;
            t.address = bKV_Info.d_index;
            bKV_Info.d_index += len;
        }
        bLseek(fd, bKV_Info.ts_address + bKV_Info.index * bKV_Info.e_size + index * sizeof(bKV_Index_t));
        bWrite(fd, (uint8_t *)&t, sizeof(bKV_Index_t) - sizeof(uint32_t));
        bLseek(fd, t.address);
        bWrite(fd, pbuf, len);
    }
    else
    {
        t.statu = 0x12345678;
        bLseek(fd, bKV_Info.ts_address + bKV_Info.index * bKV_Info.e_size + index * sizeof(bKV_Index_t) + sizeof(bKV_Index_t) - sizeof(uint32_t));
        bWrite(fd, (uint8_t *)&t.statu, sizeof(uint32_t));
        retval = _bKV_AddNew(fd, id, pbuf, len);
    }
    b_log_w("t_index:%d index:%d\r\n", bKV_Info.t_index, bKV_Info.index);  
    return retval;
}

static int _bKV_Get(int fd, bKV_Index_t t, uint8_t *pbuf)
{
    int retval = 0;
    bLseek(fd, t.address);
    retval = bRead(fd, pbuf, t.len);
    return retval;
}

static int _bKV_DeleteKey(int fd, uint32_t index, bKV_Index_t t)
{
    int retval = 0;
    if(bKV_Info.e_size == 0)
    {
        bLseek(fd, bKV_Info.ts_address + (bKV_Info.t_index - 1) * sizeof(bKV_Index_t));
        bRead(fd, (uint8_t *)&t, sizeof(bKV_Index_t));
        bLseek(fd, bKV_Info.ts_address + index * sizeof(bKV_Index_t));
        bWrite(fd, (uint8_t *)&t, sizeof(bKV_Index_t));
        bLseek(fd, bKV_Info.ts_address + (bKV_Info.t_index - 1) * sizeof(bKV_Index_t));
        bWrite(fd, (uint8_t *)&InvalidIndex, sizeof(bKV_Index_t));
        bKV_Info.t_index -= 1;
    }
    else
    {
        t.statu = 0x12345678;
        bLseek(fd, bKV_Info.ts_address + bKV_Info.index * bKV_Info.e_size + index * sizeof(bKV_Index_t) + sizeof(bKV_Index_t) - sizeof(uint32_t));
        bWrite(fd, (uint8_t *)&t.statu, sizeof(uint32_t));
    }
    return retval;
}

static void _bKV_InvalidTable(int fd)
{
    int i = 0;
    for(i = 0;i < bKV_Info.t_max;i++)
    {
        bLseek(fd, bKV_Info.ts_address + i * sizeof(bKV_Index_t));
        bWrite(fd, (uint8_t *)&InvalidIndex, sizeof(bKV_Index_t));
    }
}


/**
 * \}
 */
   
/** 
 * \addtogroup KV_Exported_Functions
 * \{
 */

/**
 * \brief KV Initialize
 * \param dev_no Device number
 * \param s_addr Start address
 * \param size The amount of storage space that KV can use
 * \param e_size  The minimum unit that performs erasure \note if it neednt to be erased, its 0
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bKV_Init(int dev_no, uint32_t s_addr, uint32_t size, uint32_t e_size)
{
    int retval = 0;
    int fd = -1;
    bKV_Info.status = bKV_ERROR;
    if(dev_no < 0)
    {
        b_log_e("dev_no %d error\r\n", dev_no);
        return -1;
    }
    bKV_Info.dev_no = dev_no;
    
    bKV_Info.index = 0;
    bKV_Info.e_size = e_size;
    bKV_Info.str_address = s_addr;
    bKV_Info.t_index = 0;
    if(e_size > 0)
    {
        if(size < 3 * e_size)
        {
            b_log_e("at least 4 times e_size\r\n");
            return -1;
        }
        bKV_Info.ts_address = s_addr + strlen(bKV_HEAD_STR);
        bKV_Info.te_address = s_addr + e_size - 1;
        bKV_Info.t_max = (bKV_Info.te_address - bKV_Info.ts_address + 1) / sizeof(bKV_Index_t);
        
        bKV_Info.ds_address = s_addr + e_size + e_size;
        bKV_Info.d_size = ((size / e_size - 2) >> 1) * e_size;
        bKV_Info.de_address = bKV_Info.ds_address + bKV_Info.d_size - 1;
        bKV_Info.d_index = bKV_Info.ds_address;
    }
    else
    {
        if(0 > _bKV_AllocateSpace(size, s_addr))
        {
            return -1;
        }
    }

    fd = bOpen(bKV_Info.dev_no, BCORE_FLAG_RW);
    if(fd < 0)
    {
        b_log_e("... open dev error\r\n");
        return -1;
    }
    
    retval = _bKV_ISFirstTime(fd);
    if(retval == -2)
    {
        bKV_Info.status = bKV_ERROR;
        return -1;
    }
    if(retval == 0)
    {   
        bKV_Info.index = 0;
        if(0 == _bKV_ClearSector(fd, bKV_SECTOR_ALL))
        {
            b_log("KV clear sector...ok\r\n");
            if(0 > _bKV_AddHeadString(fd))
            {
                return -1;
            }
        }
        if(e_size == 0)
        {
            _bKV_InvalidTable(fd);
        }
    }
    else
    {
        if(_bKV_LoadInfo(fd) < 0)
        {
            b_log_e("load info error...\r\n");
            bKV_Info.status = bKV_ERROR;
            return -1;
        }
    }
    bClose(fd);
    bKV_Info.status = bKV_IDLE;
    b_log("k/v max num:%d index:%d\r\n", bKV_Info.t_max, bKV_Info.t_index);
    return 0;
}



int bKV_Set(const char *key, uint8_t *pvalue, uint16_t len)
{
    uint32_t id = 0;
    int retval = 0, fd = -1;
    bKV_Index_t tmp;
    if(key == NULL || pvalue == NULL || bKV_Info.status != bKV_IDLE)
    {
        return -1;
    }
    fd = bOpen(bKV_Info.dev_no, BCORE_FLAG_RW);
    if(fd < 0)
    {
        b_log_e("... open dev error\r\n");
        return -1;
    }  
    
    bKV_Info.status = bKV_BUSY;
    id = _bKV_GenerateID(key);
    retval = _bKV_ISExist(fd, id, &tmp);

    if(retval < 0)
    {
        _bKV_AddNew(fd, id, pvalue, len);
    }
    else
    {
        _bKV_ModifyValue(fd, retval, tmp, id, pvalue, len);
    }
    bClose(fd);
    bKV_Info.status = bKV_IDLE;
    return retval;
}


int bKV_Get(const char *key, uint8_t *pvalue)
{
    uint32_t id = 0;
    int retval = 0, fd = -1;
    bKV_Index_t tmp;
    if(key == NULL || pvalue == NULL || bKV_Info.status != bKV_IDLE)
    {
        return -1;
    }
    
    fd = bOpen(bKV_Info.dev_no, BCORE_FLAG_RW);
    if(fd < 0)
    {
        b_log_e("... open dev error\r\n");
        return -1;
    } 
    
    bKV_Info.status = bKV_BUSY;
    id = _bKV_GenerateID(key);
    retval = _bKV_ISExist(fd, id, &tmp);
    if(retval < 0)
    {
        bKV_Info.status = bKV_IDLE;
        return -1;
    }
    retval = _bKV_Get(fd, tmp, pvalue);
    bClose(fd);
    bKV_Info.status = bKV_IDLE;
    return retval;
}


int bKV_Delete(const char *key)
{
    uint32_t id = 0;
    int retval = 0, fd = -1;
    bKV_Index_t tmp;
    if(key == NULL || bKV_Info.status != bKV_IDLE)
    {
        return -1;
    }  

    fd = bOpen(bKV_Info.dev_no, BCORE_FLAG_RW);
    if(fd < 0)
    {
        b_log_e("... open dev error\r\n");
        return -1;
    }     
    
    bKV_Info.status = bKV_BUSY;
    id = _bKV_GenerateID(key);
    retval = _bKV_ISExist(fd, id, &tmp);
    if(retval < 0)
    {
        bKV_Info.status = bKV_IDLE;
        return -1;
    }
    _bKV_DeleteKey(fd, retval, tmp);
    bClose(fd);
    bKV_Info.status = bKV_IDLE;
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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/

