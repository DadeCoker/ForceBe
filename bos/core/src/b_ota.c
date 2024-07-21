/**
 *!
 * \file        b_ota.c
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
#include "b_ota.h"
#if (_PROTO_ENABLE && _PROTO_OTA_ENABLE)  
#include "b_protocol.h"
#include "b_sdb.h"
#include "b_core.h"
#include "b_device.h"

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup OTA
 * \{
 */

/** 
 * \defgroup OTA_Private_TypesDefinitions
 * \{
 */
typedef bProtocolHead_t bOTA_Head_t;   
/**
 * \}
 */
   
/** 
 * \defgroup OTA_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup OTA_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup OTA_Private_Variables
 * \{
 */

static bOTA_Ctl_t bOTA_Ctl = {BOTA_S_NULL, 0};

/**
 * \}
 */
   
/** 
 * \defgroup OTA_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup OTA_Private_Functions
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

    retval = bSDB_Write(bOTA_Ctl.data_no, (uint8_t *)p);
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
    
    uint32_t address = _OTA_DATA_ADDRESS;
    
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
            address += p->number * _OTA_BUFF_SIZE;
            
            int fd = -1;
            fd = bOpen(bOTA_Ctl.dev_no, BCORE_FLAG_RW);
            if(fd < 0)
            {
                return -1;
            }
      
            bCMD_Struct_t cmd_s;
            cmd_s.type = bCMD_ERASE;
        	cmd_s.param.erase.num = 1;
            if(address == _OTA_DATA_ADDRESS)
            {
                cmd_s.param.erase.addr = address;
                retval = bCtl(fd, bCMD_ERASE, &cmd_s);
            }
            else if((address / BOTA_SECTOR_SIZE) != ((address + _OTA_BUFF_SIZE) / BOTA_SECTOR_SIZE))
            {
                cmd_s.param.erase.addr = address + _OTA_BUFF_SIZE;
                retval = bCtl(fd, bCMD_ERASE, &cmd_s);
            }
            else
            {
                retval = 0;
            }
            bLseek(fd, address);   
            if(retval == 0)
            {
                retval = bWrite(fd, p->buf, _OTA_BUFF_SIZE);
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
 * \addtogroup OTA_Exported_Functions
 * \{
 */
 
/**
 * \brief OTA initialize
 * \param dev_no Device number (spi flash)
 * \param pro_no Protocol instance ID
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bOTA_Init(uint8_t dev_no, uint8_t pro_no)
{
    bOTA_Ctl.status = BOTA_S_NULL;
    bOTA_Ctl.number = 0;
    bOTA_Ctl.dev_no = dev_no;
    bOTA_Ctl.pro_no = pro_no;
    bOTA_Ctl.data_no = bSDB_Regist(_OTA_INFO_ADDRESS, sizeof(bOTA_FWInfo_t), dev_no);
    if(bOTA_Ctl.data_no < 0)
    {
        return -1;
    }
    return 0;
}

/**
 * \brief OTA parse data
 * \param pbuf Pointer to data buffer
 * \param len Amount of data
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
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
#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/

