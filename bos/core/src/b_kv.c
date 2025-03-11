/**
 *!
 * \file        b_kv.c
 * \version     v0.0.1
 * \date        2020/01/08
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
#include "b_kv.h"
#if _KV_ENABLE
#include "b_core.h"
#include "b_device.h"
#include <string.h>

/** 
 * \addtogroup BABYOS
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
static bKV_Index_t bKV_IndexTable[_KV_PAIR_NUMBER];
/**
 * \}
 */
   
/** 
 * \defgroup KV_Private_FunctionPrototypes
 * \{
 */
static int _bKV_ISFirstTime()
{
    int retval = 0;
    int fd = -1;
    bKV_Index_t tmp;
    fd = bOpen(bKV_Info.dev_no, BCORE_FLAG_RW);
    if(fd < 0)
    {
        b_log_e("_bKV_ISNew open dev error\r\n");
        return -2;
    }
    bLseek(fd, bKV_Info.s_address);
    retval = bRead(fd, (uint8_t *)&tmp, sizeof(bKV_Index_t));
    bClose(fd);
    if(retval > 0)
    {
        if(tmp.id == 0x12345678 && tmp.address == 0x87654321 && tmp.len == 0x88888888)
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



static int _bKV_ClearSector(uint8_t t)
{
    int fd = -1;
    bCMD_Struct_t cmd;
    int retval = -1;
    const bKV_Index_t kv_f = {.id = 0x12345678, .address = 0x87654321, .len = 0x88888888};
    cmd.type = bCMD_ERASE;
    fd = bOpen(bKV_Info.dev_no, BCORE_FLAG_RW);
    if(fd < 0)
    {
        b_log_e("_bKV_ClearSector open dev error\r\n");
        return -1;
    }
    
    if(t & bKV_SECTOR_T1)
    {
        cmd.param.erase.addr = bKV_Info.t_address[0];
        cmd.param.erase.num = 1;
        retval = bCtl(fd, bCMD_ERASE, &cmd); 
        bLseek(fd, bKV_Info.t_address[0]);
        bWrite(fd, (uint8_t *)&kv_f, sizeof(bKV_Index_t));
    }
    if(t & bKV_SECTOR_T2)
    {
        cmd.param.erase.addr = bKV_Info.t_address[1];
        cmd.param.erase.num = 1;
        retval = bCtl(fd, bCMD_ERASE, &cmd); 
        bLseek(fd, bKV_Info.t_address[1]);
        bWrite(fd, (uint8_t *)&kv_f, sizeof(bKV_Index_t));
    }
    if(t & bKV_SECTOR_D1)
    {
        cmd.param.erase.addr = bKV_Info.d_address[0];
        cmd.param.erase.num = 1;
        retval = bCtl(fd, bCMD_ERASE, &cmd); 
    }
    if(t & bKV_SECTOR_D2)
    {
        cmd.param.erase.addr = bKV_Info.d_address[1];
        cmd.param.erase.num = 1;
        retval = bCtl(fd, bCMD_ERASE, &cmd); 
    }
    bClose(fd);
    return retval;
}


static int _bKV_Locate(uint8_t t)
{
    int fd = -1;
    bKV_Index_t tmp;
    uint16_t max_num = bKV_Info.e_size / (sizeof(bKV_Index_t));
    uint16_t left = 1, right = max_num, index = max_num / 2;
    if(t > 1)
    {
        return -1;
    }
    
    fd = bOpen(bKV_Info.dev_no, BCORE_FLAG_RW);
    if(fd < 0)
    {
        b_log_e("_bKV_LoadInfo open dev error\r\n");
        return -1;
    }
    
    while(left < right)
    {
        index = left + ((right - left) / 2);
        bLseek(fd, bKV_Info.t_address[t] + index * sizeof(bKV_Index_t));
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
    bClose(fd);
    return right;
}

static int _bKV_UpdateIndexTable(uint8_t t, uint16_t end_index)
{
    uint16_t i = 0, j = 0;
    int fd = -1;
    bKV_Index_t tmp;
    if(t > 1)
    {
        return -1;
    } 
    
    fd = bOpen(bKV_Info.dev_no, BCORE_FLAG_RW);
    if(fd < 0)
    {
        b_log_e("_bKV_... open dev error\r\n");
        return -1;
    }
    
    for(i = end_index - 1;i > 0;i--)
    {
        bLseek(fd, bKV_Info.t_address[t] + i * sizeof(bKV_Index_t));
        bRead(fd, (uint8_t *)&tmp, sizeof(bKV_Index_t));
        
        for(j = 0;j < _KV_PAIR_NUMBER;j++)
        {
            if(bKV_IndexTable[j].id == tmp.id)
            {
                break;
            }
        }
        if(j >= _KV_PAIR_NUMBER)
        {
            for(j = 0;j < _KV_PAIR_NUMBER;j++)
            {
                if(bKV_IndexTable[j].id == 0 && bKV_IndexTable[j].address == 0 && bKV_IndexTable[j].len == 0)
                {
                    memcpy(&bKV_IndexTable[j], &tmp, sizeof(bKV_Index_t));
                    break;
                }
            }
            if(j >= _KV_PAIR_NUMBER)
            {
                break;
            }
        }
        
    }
    bClose(fd);
    return 0;
}


static int _bKV_LoadInfo()
{
    int t0, t1, end_index;
    t0 = _bKV_Locate(0);
    t1 = _bKV_Locate(1);
    if(t0 < 0 || t1 < 0)
    {
        return -1;
    }
    end_index = t0;
    bKV_Info.index = 0;
    if(t0 < t1)
    {
        bKV_Info.index = 1;
        end_index = t1;
    }   
    if(_bKV_UpdateIndexTable(bKV_Info.index, end_index) < 0)
    {
        return -1;
    }
    bKV_Info.tc_address = end_index * sizeof(bKV_Index_t) + bKV_Info.t_address[bKV_Info.index]; 
    bKV_Info.dc_address = bKV_IndexTable[0].address + bKV_IndexTable[0].len;
    return 0;
}

static uint32_t _bKV_GenerateID(uint8_t *key)
{
   uint32_t id = 0;
   uint16_t klen = strlen((const char *)key);
   while(klen) 
   {
        id = (id << 5) + id + *key++;
        klen -= 1;
   }
    return id;
}

static int _bKV_ISExist(uint32_t id)
{
    uint16_t i = 0;
    for(i = 0;i < _KV_PAIR_NUMBER;i++)
    {
        if(bKV_IndexTable[i].id == id)
        {
            return i;
        }
    }
    return -1;
}

static int _bKV_TableAdd(uint32_t id, uint32_t address, uint32_t len)
{
    uint16_t i = 0;
    for(i = 0;i < _KV_PAIR_NUMBER;i++)
    {
        if(bKV_IndexTable[i].id == 0 && bKV_IndexTable[i].address == 0 && bKV_IndexTable[i].len == 0)
        {
            bKV_IndexTable[i].id = id;
            bKV_IndexTable[i].address = address;
            bKV_IndexTable[i].len = len;
            return 0;
        }
        else if(bKV_IndexTable[i].id == id)
        {
            bKV_IndexTable[i].address = address;
            bKV_IndexTable[i].len = len;
            return 0;
        }
    }
    return -1;
}

static int _bKV_SwitchSector()
{
    uint8_t s, d;
    int fd = -1;
    uint16_t i = 0;
    uint32_t dt_address, dd_address, len, r_len, addr;
    uint8_t buf[32];
    bKV_Index_t tmp;
    s = bKV_Info.index;
    d = (bKV_Info.index + 1) % 2;
  
    if(d == 0)
    {
        _bKV_ClearSector(bKV_SECTOR_T1 | bKV_SECTOR_D1);
    }
    else
    {
        _bKV_ClearSector(bKV_SECTOR_T2 | bKV_SECTOR_D2);
    }
    dt_address = bKV_Info.t_address[d] + sizeof(bKV_Index_t);
    dd_address = bKV_Info.d_address[d];
    
    fd = bOpen(bKV_Info.dev_no, BCORE_FLAG_RW);
    if(fd < 0)
    {
        b_log_e("... open dev error\r\n");
        return -1;
    }
    for(i = 0;i < _KV_PAIR_NUMBER;i++)
    {
        if(bKV_IndexTable[i].id != 0 && bKV_IndexTable[i].address != 0 && bKV_IndexTable[i].len != 0)
        {
            bLseek(fd, dt_address);
            tmp.address = dd_address;
            tmp.id = bKV_IndexTable[i].id;
            tmp.len = bKV_IndexTable[i].len;
            if(bKV_IndexTable[i].address < bKV_Info.d_address[s] || bKV_IndexTable[i].address >= (bKV_Info.d_address[s] + bKV_Info.e_size))
            {
                continue;
            }
            bWrite(fd, (uint8_t *)&tmp, sizeof(bKV_Index_t));
            dt_address += sizeof(bKV_Index_t);
            
            len = bKV_IndexTable[i].len;
            addr = bKV_IndexTable[i].address;
            for(;;)
            {
                bLseek(fd, addr);
                r_len = (len > 32) ? 32 : len;
                bRead(fd, buf, r_len);
                len = len - r_len;
                addr += r_len;
                bLseek(fd, dd_address);
                bWrite(fd, buf, r_len);
                dd_address += r_len;
                if(len == 0)
                {
                    break;
                }
            }
        }
    }
    bClose(fd);
    bKV_Info.dc_address = dd_address;
    bKV_Info.tc_address = dt_address;
    bKV_Info.index = d; 
    
    if(s == 0)
    {
        _bKV_ClearSector(bKV_SECTOR_T1 | bKV_SECTOR_D1);
    }
    else
    {
        _bKV_ClearSector(bKV_SECTOR_T2 | bKV_SECTOR_D2);
    }
    return 0;
}

static int _bKV_Set(uint32_t id, uint8_t *pbuf, uint16_t len)
{
    int fd = -1;
    int retval = 0, i = 0;
    fd = bOpen(bKV_Info.dev_no, BCORE_FLAG_RW);
    if(fd < 0)
    {
        b_log_e("... open dev error\r\n");
        return -1;
    }
    
    for(i = 0;i < _KV_PAIR_NUMBER;i++)
    {
        if(bKV_IndexTable[i].id == id)
        {
            bLseek(fd, bKV_Info.tc_address);
            retval = bWrite(fd, (uint8_t *)&bKV_IndexTable[i], sizeof(bKV_Index_t));
            bKV_Info.tc_address += sizeof(bKV_Index_t);
            if(retval >= 0)
            {
                bLseek(fd, bKV_Info.dc_address);
                retval = bWrite(fd, pbuf, len);
                bKV_Info.dc_address += len;
            }
            break;
        }
    }
    bClose(fd);
    return retval;
}

static int _bKV_Get(uint16_t index, uint8_t *pbuf)
{
    int fd = -1;
    int retval = 0;
    fd = bOpen(bKV_Info.dev_no, BCORE_FLAG_RW);
    if(fd < 0)
    {
        b_log_e("... open dev error\r\n");
        return -1;
    }
    bLseek(fd, bKV_IndexTable[index].address);
    retval = bRead(fd, pbuf, bKV_IndexTable[index].len);
    bClose(fd);
    return retval;
}


/**
 * \}
 */
   
/** 
 * \defgroup KV_Private_Functions
 * \{
 */

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
 * \param size The amount of storage space that KV can use \note at lease 4 * e_size
 * \param e_size  Sector size (sector erase)
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bKV_Init(int dev_no, uint32_t s_addr, uint32_t size, uint32_t e_size)
{
    int retval = 0;
    if(dev_no < 0)
    {
        b_log_e("dev_no %d error\r\n", dev_no);
        return -1;
    }
    if(size < 3 * e_size)
    {
        b_log_e("size too small, at least 4 times e_size\r\n");
        return -1;
    }
    
    memset(&bKV_IndexTable[0], 0, sizeof(bKV_IndexTable));
    
    bKV_Info.dev_no = dev_no;
    bKV_Info.s_address = s_addr;
    bKV_Info.e_size = e_size;
    bKV_Info.t_address[0] = s_addr;
    bKV_Info.t_address[1] = s_addr + e_size;
    bKV_Info.d_address[0] = s_addr + e_size + e_size;
    bKV_Info.d_address[1] = s_addr + e_size + e_size + e_size;   
    bKV_Info.status = bKV_IDLE;
    
    retval = _bKV_ISFirstTime();
    if(retval == -2)
    {
        bKV_Info.status = bKV_ERROR;
        return -1;
    }
    if(retval == 0)
    {   
        bKV_Info.index = 0;
        if(0 == _bKV_ClearSector(bKV_SECTOR_ALL))
        {
            b_log("KV clear sector...ok\r\n");
        }
        bKV_Info.tc_address = bKV_Info.t_address[0] + sizeof(bKV_Index_t);
        bKV_Info.dc_address = bKV_Info.d_address[0];
    }
    else
    {
        if(_bKV_LoadInfo() < 0)
        {
            b_log_e("load info error...\r\n");
            bKV_Info.status = bKV_ERROR;
            return -1;
        }
    }
    return 0;
}



int bKV_Set(uint8_t *key, uint8_t *pvalue, uint16_t len)
{
    uint32_t id = 0;
    int retval;
    if(key == NULL || pvalue == NULL || bKV_Info.status != bKV_IDLE)
    {
        return -1;
    }
    bKV_Info.status = bKV_BUSY;
    id = _bKV_GenerateID(key);
    retval = _bKV_ISExist(id);
    if(retval < 0)
    {
       if(_bKV_TableAdd(id, 0, 0) < 0)
       {
           bKV_Info.status = bKV_IDLE;
           return -1;
       }
    }
    if((bKV_Info.tc_address + sizeof(bKV_Index_t)) > (bKV_Info.t_address[bKV_Info.index] + bKV_Info.e_size)
        || (bKV_Info.dc_address + len) > (bKV_Info.d_address[bKV_Info.index] + bKV_Info.e_size)) 
    {
        b_log("switch sector.... \r\n");
        if(0 > _bKV_SwitchSector())
        {
            bKV_Info.status = bKV_IDLE;
            return -1;
        }
    }        
    _bKV_TableAdd(id, bKV_Info.dc_address, len);
    retval = _bKV_Set(id, pvalue, len);
    bKV_Info.status = bKV_IDLE;
    return retval;
}


int bKV_Get(uint8_t *key, uint8_t *pvalue)
{
    uint32_t id = 0;
    int retval;
    if(key == NULL || pvalue == NULL || bKV_Info.status != bKV_IDLE)
    {
        return -1;
    }
    bKV_Info.status = bKV_BUSY;
    id = _bKV_GenerateID(key);
    retval = _bKV_ISExist(id);
    if(retval < 0)
    {
        bKV_Info.status = bKV_IDLE;
        return -1;
    }
    retval = _bKV_Get(retval, pvalue);
    bKV_Info.status = bKV_IDLE;
    return retval;
}


int bKV_Delete(uint8_t *key)
{
    uint32_t id = 0;
    int retval;
    if(key == NULL || bKV_Info.status != bKV_IDLE)
    {
        return -1;
    }  
    bKV_Info.status = bKV_BUSY;
    id = _bKV_GenerateID(key);
    retval = _bKV_ISExist(id);
    if(retval < 0)
    {
        bKV_Info.status = bKV_IDLE;
        return -1;
    }
    bKV_IndexTable[retval].address = 0;
    bKV_IndexTable[retval].id = 0;
    bKV_IndexTable[retval].len = 0;
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
#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/

