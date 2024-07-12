/**
 *!
 * \file       bprotocol.c
 * \brief      Common protocol
 * \version    v0.0.1
 * \date       2019/05/08
 * \author     notrynohigh
 *Last modified by notrynohigh 2019/05/08
 *Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "bprotocol.h"  
#include "bcrc/bsum.h"
#include <string.h>
#include "bconfig.h"
#include "btx/btx.h"

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_PROTOCOL
 * \brief 通用的通讯协议接口，通讯格式 |头|ID|长度|指令|数据|校验和|
 * \{
 */


/** 
 * \defgroup PROTOCOL_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup PROTOCOL_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup PROTOCOL_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup PROTOCOL_Private_Variables
 * \{
 */

static bProtocolInfo_t  bProtocolInfo[bCFG_PRO_I_NUM];
static uint8_t bProtocolInfoIndex = 0;
/**
 * \}
 */
   
/** 
 * \defgroup PROTOCOL_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup PROTOCOL_Private_Functions
 * \{
 */


/**
 * \}
 */
   
/** 
 * \addtogroup PROTOCOL_Exported_Functions
 * \{
 */

/**
 * \brief 获取一个实体
 * \param id 通讯中ID字段
 * \param tx_no 通讯应用号
 * \param f 分发函数
 * \retval 应用序号
 */
int bProtocolRegist(uint32_t id, uint8_t tx_no, pdispatch f)
{
    if(bProtocolInfoIndex >= bCFG_PRO_I_NUM || f == NULL)
    {
        return -1;
    }
    bProtocolInfo[bProtocolInfoIndex].id = id;
    bProtocolInfo[bProtocolInfoIndex].tx_no = tx_no;
    bProtocolInfo[bProtocolInfoIndex].f = f;
    bProtocolInfoIndex += 1;
    return (bProtocolInfoIndex - 1);
}

/**
 * \brief 解析数据并调用分发函数
 * \param no 应用号
 * \param pbuf 接收到的数据buffer
 * \param len 接收到的数据长度
 * \retval 返回值
 *          \arg 0 成功
 *          \arg -1 失败
 */
int bProtocolParseCmd(uint8_t no, uint8_t *pbuf, uint8_t len)
{
    bProtocolHead_t *phead = (bProtocolHead_t *)pbuf;
    if(pbuf == NULL || len < (sizeof(bProtocolHead_t) + 1) || no >= bProtocolInfoIndex)
    {
        return -1;
    }
    
    if(phead->head != PROTOCOL_HEAD || (phead->device_id != bProtocolInfo[no].id && (phead->device_id != 0xffffffff) && (bProtocolInfo[no].id != 0xffffffff)))
    {
        return -1;
    }
    return bProtocolInfo[no].f(pbuf, len);
}

int bProtocolSetID(uint8_t no, uint32_t id)
{
    if(no >= bProtocolInfoIndex)
    {
        return -1;
    }
    bProtocolInfo[no].id = id;
    return 0;
}



/**
 * \brief 组包并发送的函数
 * \param no 应用号
 * \param cmd 指令号
 * \param psrc 数据源
 * \param pdes 数据目的地址
 * \param size 数据源数据的长度
 * \retval 返回值
 *          \arg 0 成功
 *          \arg -1 失败
 */
int bProtocolPack(uint8_t no, uint8_t cmd, uint8_t *psrc, uint8_t *pdes, uint8_t size)
{
    uint8_t length = 0;
    bProtocolHead_t *phead;
    
    if((psrc == NULL && size > 0) || pdes == NULL || no >= bProtocolInfoIndex)
    {
        return -1;
    }
    
    phead = (bProtocolHead_t *)pdes;
    phead->head = PROTOCOL_HEAD;
    phead->device_id = bProtocolInfo[no].id;
    phead->cmd = cmd;
    phead->len = 1 + size;
    
    if(psrc != NULL)
    {
        memcpy(pdes + sizeof(bProtocolHead_t), psrc, size);
    }
    
    length = sizeof(bProtocolHead_t) + phead->len;
    *(pdes + length - 1) = bSUM(pdes, length - 1);
    
    return bTXReq(bProtocolInfo[no].tx_no, pdes, length, BTX_REQ_1);
}

/**
 * \brief 拆包函数
 * \param pbuf 数据源
 * \param len 数据源数据的长度
 * \param pdata 数据起始地址
 * \param size 需要解析的数据的长度
 * \retval 返回值
 *          \arg 0 成功
 *          \arg -1 失败
 */
int bProtocolUnpack(uint8_t *pbuf, uint8_t len, uint8_t **pdata, uint8_t size)
{
    if(pbuf == NULL)
    {
        return -1;
    }
    
    bProtocolHead_t *phead = (bProtocolHead_t *)pbuf;
    uint8_t length = phead->len + sizeof(bProtocolHead_t);
    if(phead->len != (size + 1) || length > len)
    {
        return -1;
    }  
    uint8_t crc = bSUM(pbuf, length - 1);
    if(crc != pbuf[length - 1])
    {
        b_log("crc error!%d %d", crc, pbuf[length - 1]);
        return -1;
    }
    if(pdata == NULL && size > 0)
    {
        return -1;
    }
    
    if(pdata != NULL)
    {
        *pdata = &pbuf[sizeof(bProtocolHead_t)];
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
  
/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/

