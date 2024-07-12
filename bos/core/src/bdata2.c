/**
 *!
 * \file       bdata2.c
 * \brief      Save data to a contiguous area
 * \version    v0.0.1
 * \date       2019/05/08
 * \author     notrynohigh
 *Last modified by notrynohigh 2019/05/08
 *Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "bdata2.h"
#include "bconfig.h"
#include <string.h>
#include "bcore/bcore.h"
#include "bdev/bflash/bflash.h"

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_DATA2
 * \brief 一块区域连续存储固定长度的应用
 * \{
 */

/** 
 * \defgroup BDATA2_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BDATA2_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BDATA2_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BDATA2_Private_Variables
 * \{
 */

static bData2Info_t bData2Info[bCFG_BDATA2_I_NUM];
static uint32_t bData2InfoIndex = 0;

/**
 * \}
 */
   
/** 
 * \defgroup BDATA2_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BDATA2_Private_Functions
 * \{
 */

static int _bData2Count(int no)
{
    uint32_t i = 0, addr;
    uint8_t head;
    int retval = -1;
    
    if(no >= bCFG_BDATA2_I_NUM || no < 0)
    {
        return -1;
    }

    int d_fd = -1;
    d_fd = bOpen(bData2Info[no].dev_no, BFD_FLAG_R);
    if(d_fd < 0)
    {
        return -1;
    }
    for(i = 0;i < bData2Info[no].max_num;i++)
    {
        addr = i * (bData2Info[no].st.usize + 1) + bData2Info[no].st.address;
        bLseek(d_fd, addr);
        retval = bRead(d_fd, &head, 1);
        if(0 == retval)
        {
            if(head != D_HEAD_F)
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
    bData2Info[no].index = i;
    bClose(d_fd);
    return 0;
}




/**
 * \}
 */
   
/** 
 * \addtogroup BDATA2_Exported_Functions
 * \{
 */

/**
 * \brief 获取一个实体
 * \param st 注册一个实体必要的参数 \ref bData2Struct_t
 * \param dev_no 设备号
 * \retval 返回应用号
 */
int bData2RegistInfo(bData2Struct_t st, uint8_t dev_no)
{
    if(bData2InfoIndex >= bCFG_BDATA2_I_NUM || st.size <= st.usize || (st.size & (st.esize - 1)))
    {
        return -1;    
    }
    
    bData2Info[bData2InfoIndex].flag = D_HEAD_F;
    bData2Info[bData2InfoIndex].max_num = st.size / (st.usize + 1);
    bData2Info[bData2InfoIndex].index = 0;

    bData2Info[bData2InfoIndex].st.address = st.address;
    bData2Info[bData2InfoIndex].st.esize = st.esize;
    bData2Info[bData2InfoIndex].st.size = st.size;
    bData2Info[bData2InfoIndex].st.usize = st.usize;

    bData2Info[bData2InfoIndex].dev_no = dev_no;

    if(_bData2Count(bData2InfoIndex) < 0)
    {
        return -1;
    }
    
    bData2InfoIndex += 1;
    return (bData2InfoIndex - 1);
}


/**
 * \brief 追加数据
 * \param no 应用号 \ref bData2RegistInfo
 * \param pbuf 数据的存储buf
 * \retval 返回值
 *          \arg 0 成功
 *          \arg -1 失败
 */
int bData2Write(int no, uint8_t *pbuf)
{
    uint8_t head = D_HEAD_F;
    int retval = -1;
    uint32_t addr = 0;
    
    if(pbuf == NULL || no >= bData2InfoIndex || no < 0)
    {
        return -1;
    }

    if(bData2Info[no].flag != D_HEAD_F)
    {
        return -1;
    }

    addr = bData2Info[no].index * (bData2Info[no].st.usize + 1) + bData2Info[no].st.address;

    int d_fd = -1;
    d_fd = bOpen(bData2Info[no].dev_no, BFD_FLAG_RW);
    if(d_fd < 0)
    {
        return -1;
    }
    
    bFlashEraseParam_t param;
    param.number =1;
    
    if(bData2Info[no].index == 0)
    {
        param.address = addr;
        bCtl(d_fd, BFLASH_CMD_ERASE, &param);
    }
    else if((addr / bData2Info[no].st.esize) != ((addr + bData2Info[no].st.usize + 1) / bData2Info[no].st.esize))
    {
        param.address = addr + bData2Info[no].st.usize + 1;
        bCtl(d_fd, BFLASH_CMD_ERASE, &param);
    }

    bLseek(d_fd, addr);
    retval = bWrite(d_fd, &head, 1);
    if(retval >= 0)
    {
        retval = bWrite(d_fd, pbuf, bData2Info[no].st.usize);
    }
    bData2Info[no].index = (bData2Info[no].index + 1) % (bData2Info[no].max_num);
    bClose(d_fd);
    return retval;
}

/**
 * \brief 读取数据
 * \param no 应用号 \ref bData2RegistInfo
 * \param index 序号
 * \param pbuf 数据的存储buf
 * \retval 返回值
 *          \arg 0 成功
 *          \arg -1 失败
 */
int bData2Read(int no, uint32_t index, uint8_t *pbuf)
{
    uint8_t head = 0;
    uint32_t addr = 0;
    int retval = 0;
   
    if(pbuf == NULL || no >= bData2InfoIndex || no < 0)
    {
        return -1;
    }

    if(bData2Info[no].flag != D_HEAD_F || bData2Info[no].max_num <= index)
    {
        return -1;
    }
    addr = index * (bData2Info[no].st.usize + 1) + bData2Info[no].st.address;
    int d_fd = -1;
    d_fd = bOpen(bData2Info[no].dev_no, BFD_FLAG_RW);
    if(d_fd < 0)
    {
        return -1;
    }
    bLseek(d_fd, addr);
    retval = bRead(d_fd, &head, 1);
    if(head != D_HEAD_F)
    {
        retval = -1;
    }
    
    if(retval >= 0)
    {
        retval = bRead(d_fd, pbuf, bData2Info[no].st.usize);
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
  
/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/

