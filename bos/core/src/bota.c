/**
 *!
 * \file       bota.c
 * \brief      OTA
 * \version    v0.0.1
 * \date       2019/05/08
 * \author     notrynohigh
 *Last modified by notrynohigh 2019/05/08
 *Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "bota.h"  
#include "map.h"
#include "bprotocol/bprotocol.h"
#include "bdata/bdata.h"
#include "bcore/bcore.h"
#include "bdev/bflash/bflash.h"

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_OTA
 * \brief 在线升级单元
 * \{
 */

/** 
 * \defgroup BOTA_Private_TypesDefinitions
 * \{
 */
typedef bProtocolHead_t bOTA_Head_t;   
/**
 * \}
 */
   
/** 
 * \defgroup BOTA_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BOTA_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BOTA_Private_Variables
 * \{
 */

static bOTA_Ctl_t bOTA_Ctl = {BOTA_S_NULL, 0};

/**
 * \}
 */
   
/** 
 * \defgroup BOTA_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BOTA_Private_Functions
 * \{
 */
static int _bOTA_StartHandler(uint8_t *pbuf, uint16_t len)
{
    int retval = -1;
    uint8_t table[32];
    bOTA_FWInfo_t *p = NULL;
    bOTA_FWACK_t ack;
    if(pbuf == NULL)
    {
        return -1;
    }
    if(bProtocolUnpack(pbuf, len, (uint8_t **)&p, sizeof(bOTA_FWInfo_t)) == -1)
    {
        return -1;
    }
    
    if(p == NULL)
    {
        return -1;
    }

    retval = bDataWrite(bOTA_Ctl.data_no, (uint8_t *)p);
    if(retval == 0)
    {
        ack.number = 0xffff;
        retval = bProtocolPack(bOTA_Ctl.pro_no, BOTA_CMD_FW_ACK, (uint8_t *)&ack, table, sizeof(bOTA_FWACK_t));
        if(retval == 0)
        {
            bOTA_Ctl.status = BOTA_S_ING;
            bOTA_Ctl.number = 0;
        }
    }
    return retval;
}


static int _bOTA_DataHandler(uint8_t *pbuf, uint16_t len)
{
    int retval = -1;
    uint8_t table[32];
    bOTA_FWData_t *p = NULL;
    bOTA_FWACK_t ack;
    
    uint32_t address = MAP_FW_DATA_ADDR;
    
    if(pbuf == NULL)
    {
        return -1;
    }
    if(bProtocolUnpack(pbuf, len, (uint8_t **)&p, sizeof(bOTA_FWData_t)) == -1)
    {
        return -1;
    }
    
    if(p == NULL)
    {
        return -1;
    }
   
    if(bOTA_Ctl.status == BOTA_S_ING)
    {
        if(p->number == bOTA_Ctl.number)
        {
            address += p->number * bCFG_FW_BUF_SIZE;
            
            int fd = -1;
            fd = bOpen(bOTA_Ctl.dev_no, BFD_FLAG_RW);
            if(fd < 0)
            {
                return -1;
            }
      
            bFlashEraseParam_t param;
            param.number = 1;
            if(address == MAP_FW_DATA_ADDR)
            {
                param.address = address;
                retval = bCtl(fd, BFLASH_CMD_ERASE, &param);
            }
            else if((address / MAP_SECTOR_SIZE) != ((address + bCFG_FW_BUF_SIZE) / MAP_SECTOR_SIZE))
            {
                param.address = address + bCFG_FW_BUF_SIZE;
                retval = bCtl(fd, BFLASH_CMD_ERASE, &param);
            }
            else
            {
                retval = 0;
            }
            bLseek(fd, address);   
            if(retval == 0)
            {
                retval = bWrite(fd, p->buf, bCFG_FW_BUF_SIZE);
            }
            bClose(fd);
            if(retval == 0)
            {
                bOTA_Ctl.number += 1;
            }
        }

        ack.number = bOTA_Ctl.number;
        retval = bProtocolPack(bOTA_Ctl.pro_no, BOTA_CMD_FW_ACK, (uint8_t *)&ack, table, sizeof(bOTA_FWACK_t));
    }
    return retval;
}

/**
 * \}
 */
   
/** 
 * \addtogroup BOTA_Exported_Functions
 * \{
 */
/**
 * \brief 初始化，指定存储设备号和协议应用号
 * \param dev_no 存储设备设备号
 * \param pro_no 协议解析应用号
 * \retval 返回值
 *          \arg 0 成功
 *          \arg -1 失败
 */
int bOTA_Init(uint8_t dev_no, uint8_t pro_no)
{
    bOTA_Ctl.status = BOTA_S_NULL;
    bOTA_Ctl.number = 0;
    bOTA_Ctl.dev_no = dev_no;
    bOTA_Ctl.pro_no = pro_no;
    bOTA_Ctl.data_no = bDataRegistInfo(MAP_FW_INFO_ADDR, sizeof(bOTA_FWInfo_t), dev_no);
    if(bOTA_Ctl.data_no < 0)
    {
        return -1;
    }
    return 0;
}

/**
 * \brief 解析函数
 * \param pbuf 接收的数据
 * \param len 数据长度
 * \retval 返回值
 *          \arg 0 成功
 *          \arg -1 失败
 */
int bOTA_Parse(uint8_t *pbuf, uint16_t len)
{
    int retval = -1;
    if(pbuf == NULL || len <= sizeof(bOTA_Head_t))
    {
        return -1;
    }
    
    bOTA_Head_t *phead = (bOTA_Head_t *)pbuf;
    switch(phead->cmd)
    {
        case BOTA_CMD_FW_START:
            retval = _bOTA_StartHandler(pbuf, len);
            break;
        case BOTA_CMD_FW_DATA:
            retval = _bOTA_DataHandler(pbuf, len);
            break;
        default:
            break;
    }
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

