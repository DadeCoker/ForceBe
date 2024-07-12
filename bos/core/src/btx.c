/**
 *!
 * \file       btx.c
 * \brief      Transmit data
 * \version    v0.0.1
 * \date       2019/07/02
 * \author     notrynohigh
 *Last modified by notrynohigh 2019/07/02
 *Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "btx.h"
#include <string.h>
#include "bconfig.h"
#include "bcore/bcore.h"
#include "bhal.h"

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_TX
 * \brief 发送数据单元
 * \{
 */

/** 
 * \defgroup BTXRX_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BTX_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BTX_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BTX_Private_Variables
 * \{
 */
static bTX_Info_t bTX_InfoTable[bCFG_TX_I_NUM]; 
static uint8_t bTX_InfoIndex = 0;
/**
 * \}
 */
   
/** 
 * \defgroup BTX_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BTX_Private_Functions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \addtogroup BTX_Exported_Functions
 * \{
 */
 
/**
 * \brief 获取一个实体
 * \param pbuf 数据发送的缓存
 * \param size 缓存大小
 * \param td_mode 发送完成信号类型
 *          \arg \ref BTX_F_SYN 同步，发送后立即完成
 *          \arg \ref BTX_F_ASYN 异步，发送完后等待结束信号
 * \param dev_no 设备号
 * \retval 应用号
 */
int bTXRegist(uint8_t *pbuf, uint32_t size, uint8_t td_mode, uint8_t dev_no)
{
    if(pbuf == NULL || bTX_InfoIndex >= bCFG_TX_I_NUM)
    {
        return -1;
    }
    bTX_InfoTable[bTX_InfoIndex].ptxBUF = pbuf;
    bTX_InfoTable[bTX_InfoIndex].buf_size = size;
    bTX_InfoTable[bTX_InfoIndex].dev_no = dev_no;
    bTX_InfoTable[bTX_InfoIndex].state = BTX_NULL;
    bTX_InfoTable[bTX_InfoIndex].td_mode = td_mode;
    bTX_InfoTable[bTX_InfoIndex].td_flag = 0;
    bTX_InfoTable[bTX_InfoIndex].tx_len = 0;
    bTX_InfoTable[bTX_InfoIndex].fd = -1;
    bTX_InfoTable[bTX_InfoIndex].timeout = 0;
    bTX_InfoTable[bTX_InfoIndex].timeout_f = 0;
    bTX_InfoIndex += 1;
    return (bTX_InfoIndex - 1);
}


/**
 * \brief 检测并调用发送
 * \note 放入while循环里调用
 * \retval 返回值
 *          \arg 0 成功
 *          \arg -1 失败
 */
int bTXCore()
{
    int i = 0;
    int retval = -1;
    for(i = 0;i < bTX_InfoIndex;i++)
    {
        if(bTX_InfoTable[i].state == BTX_REQ)
        {
            bTX_InfoTable[i].fd = bOpen(bTX_InfoTable[i].dev_no, BFD_FLAG_RW);
            if(bTX_InfoTable[i].fd < 0)
            {
                b_log("tx open err\r\n");
                continue;
            }  
            bTX_InfoTable[i].td_flag = 0;
            retval = bWrite(bTX_InfoTable[i].fd, bTX_InfoTable[i].ptxBUF, bTX_InfoTable[i].tx_len);
            if(retval >= 0)
            {
                if(bTX_InfoTable[i].td_mode == BTX_F_SYN)
                {
                    bTX_InfoTable[i].state = BTX_NULL;
                    bClose(bTX_InfoTable[i].fd);
                    b_log("tx done\r\n");
                }
                else
                {
                    bTX_InfoTable[i].state = BTX_WAIT;
                    bTX_InfoTable[i].timeout_f = 1;
                    bTX_InfoTable[i].timeout = bHalGetTick();
                }
            }
            else
            {
                bClose(bTX_InfoTable[i].fd);
                b_log("tx write err\r\n");
                continue;
            }
        }
        
        if(bTX_InfoTable[i].state == BTX_WAIT)
        {
            if(bTX_InfoTable[i].td_flag || 
                (((bHalGetTick() - bTX_InfoTable[i].timeout) > 2000) && bTX_InfoTable[i].timeout_f == 1))
            {
                bTX_InfoTable[i].timeout_f = 0;
                bTX_InfoTable[i].td_flag = 0;
                bTX_InfoTable[i].state = BTX_NULL;
                bClose(bTX_InfoTable[i].fd);
                b_log("tx done %d\r\n", bHalGetTick() - bTX_InfoTable[i].timeout);
            }
        }
    }
    return 0;
}


/**
 * \brief 请求发送数据
 * \param no 应用号
 * \param pbuf 数据存放区
 * \param size 需要发送数据的长度
 * \param flag 当前请求的等级
 *          \arg \ref BTX_REQ_0 普通等级，空闲时才能请求成功
 *          \arg \ref BTX_REQ_1 超高等级，立即插入等待发送
 * \retval 返回值
 *          \arg 0 成功
 *          \arg -1 失败
 */
int bTXReq(int no, uint8_t *pbuf, uint16_t size, uint8_t flag)
{

    if(no < 0 || no >= bTX_InfoIndex || pbuf == NULL)
    {
        return -1;
    }

    if(flag == BTX_REQ_0 && bTX_InfoTable[no].state != BTX_NULL)
    {
        return -1;
    }

    if(size > bTX_InfoTable[no].buf_size)
    {
        return -1;
    }
    b_log("req %d data\r\n", size);
#if 0    
    uint8_t i = 0;
    for(i = 0;i < size;i++)
    {
        b_log("%d ", pbuf[i]);
    }
    b_log("\r\n");
#endif    
    memcpy(bTX_InfoTable[no].ptxBUF, pbuf, size);
    bTX_InfoTable[no].tx_len = size;
    bTX_InfoTable[no].state = BTX_REQ;
    return 0;
}



/**
 * \brief 通知发送完成
 * \param no 应用号
 * \retval 返回值
 *          \arg 0 成功
 *          \arg -1 失败
 */
int bTXCpl(int no)
{
    if(no < 0 || no >= bTX_InfoIndex)
    {
        return -1;
    }

    if(bTX_InfoTable[no].td_mode == BTX_F_SYN)
    {
        return -1;
    }
    bTX_InfoTable[no].td_flag = 1;
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
  
/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/

