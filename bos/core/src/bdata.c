/**
 *!
 * \file       bdata.c
 * \brief      W/R data
 * \version    v0.0.1
 * \date       2019/05/08
 * \author     notrynohigh
 *Last modified by notrynohigh 2019/05/08
 *Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "bdata.h"
#include "bcrc/bsum.h"
#include "bconfig.h"
#include <string.h>
#include "bfd/bfd.h"
#include "bcore/bcore.h"
#include "bdev/bflash/bflash.h"

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_DATA
 * \brief 最小擦除单位内的读写应用
 * \{
 */


/** 
 * \defgroup BDATA_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BDATA_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BDATA_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BDATA_Private_Variables
 * \{
 */

static bDataInfo_t bDataInfo[bCFG_BDATA_I_NUM];
static uint32_t bDataInfoIndex = 0;

/**
 * \}
 */
   
/** 
 * \defgroup BDATA_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BDATA_Private_Functions
 * \{
 */



/**
 * \}
 */
   
/** 
 * \addtogroup BDATA_Exported_Functions
 * \{
 */

/**
 * \brief 获取一个应用实体
 * \param address 起始地址
 * \param usize 存数的大小
 * \param dev_no 设备号
 * \retval 返回应用号，后续操作需要用到
 */
int bDataRegistInfo(uint32_t address, uint32_t usize, uint8_t dev_no)
{
    if(bDataInfoIndex >= bCFG_BDATA_I_NUM)
    {
        return -1;    
    }
    bDataInfo[bDataInfoIndex].flag = D_HEAD_F;
    bDataInfo[bDataInfoIndex].usize = usize;
    bDataInfo[bDataInfoIndex].address = address;
    bDataInfo[bDataInfoIndex].dev_no = dev_no;
    bDataInfoIndex += 1;
    return (bDataInfoIndex - 1);
}

/**
 * \brief 数据的写入
 * \note 写入的格式是 头|数据|校验和
 * \param no 应用号 \ref bDataRegistInfo
 * \param pbuf 数据Buffer
 * \retval 返回值
 *          \arg 0 成功
 *          \arg -1 失败
 */
int bDataWrite(int no, uint8_t *pbuf)
{
    uint8_t head = D_HEAD_F, sum = 0;
    int retval = 0;
             
    if(pbuf == NULL || no >= bDataInfoIndex || no < 0)
    {
        return -1;
    }
    
    if(bDataInfo[no].flag != D_HEAD_F)
    {
        return -1;
    }

    int d_fd = -1;
    d_fd = bOpen(bDataInfo[no].dev_no, BFD_FLAG_RW);
    if(d_fd < 0)
    {
        return -1;
    }
    
    bFlashEraseParam_t param;
    param.address = bDataInfo[no].address;
    param.number =1;
    retval = bCtl(d_fd, BFLASH_CMD_ERASE, &param);
    if(retval >= 0)
    {
        bLseek(d_fd, bDataInfo[no].address);
        retval = bWrite(d_fd, &head, 1);
    }
    sum = bSUM(pbuf, bDataInfo[no].usize);
    
    if(retval >= 0)
    {
        retval = bWrite(d_fd, pbuf, bDataInfo[no].usize);
    }

    if(retval >= 0)
    {
        retval = bWrite(d_fd, &sum, 1);
    }
    if(retval >= 0)
    {
        retval = bCtl(d_fd, BFLASH_CMD_FLUSH, NULL);
    }
    bClose(d_fd);
    return retval;
}


/**
 * \brief 数据的读取
 * \param no 应用号 \ref bDataRegistInfo
 * \param pbuf 数据Buffer
 * \retval 返回值
 *          \arg 0 成功
 *          \arg -1 失败
 */
int bDataRead(int no, uint8_t *pbuf)
{
    uint8_t head = 0, sum = 0, r_sum = 0;
    int retval = 0;
            
    if(pbuf == NULL || no >= bDataInfoIndex || no < 0)
    {
        return -1;
    }
    
    if(bDataInfo[no].flag != D_HEAD_F)
    {
        return -1;
    }

    int d_fd = -1;
    d_fd = bOpen(bDataInfo[no].dev_no, BFD_FLAG_R);
    if(d_fd < 0)
    {
        return -1;
    }

    bLseek(d_fd, bDataInfo[no].address);
    
    retval = bRead(d_fd, &head, 1);
    if(head != D_HEAD_F)
    {
        retval = -1;
    }
    
    if(retval >= 0)
    {
        retval = bRead(d_fd, pbuf, bDataInfo[no].usize);
        sum = bSUM(pbuf, bDataInfo[no].usize);
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
/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/

