/**
 *!
 * \file        b_protocol.c
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
#include "b_protocol.h"
#if _PROTO_ENABLE
#include "b_sum.h"
#include "b_tx.h"
#include <string.h>
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup PROTOCOL
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

static bProtocolInfo_t  bProtocolInfo[_PROTO_I_NUMBER];
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
 * \brief Create a protocol instance
 * \param id system id
 * \param tx_no TX instance ID
 * \param f Dispatch Function \ref pdispatch
 * \retval Instance ID
 *          \arg >=0  valid
 *          \arg -1   invalid
 */
int bProtocolRegist(uint32_t id, uint8_t tx_no, pdispatch f)
{
    if(bProtocolInfoIndex >= _PROTO_I_NUMBER || f == NULL)
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
 * \brief Check ID and call dispatch function
 * \param no Protocol instance \ref bProtocolRegist
 * \param pbuf Pointer to data buffer
 * \param len Amount of data
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
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



/**
 * \brief Call this function after system ID is changed
 * \param no Protocol instance \ref bProtocolRegist
 * \param id System ID
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
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
 * \brief pack and start a TX request
 * \param no Protocol instance \ref bProtocolRegist
 * \param cmd Protocol command
 * \param psrc Pointer to payload
 * \param pdes Pointer to data buffer
 * \param size size of payload
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
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
    
    return bTX_Request(bProtocolInfo[no].tx_no, pdes, length, BTX_REQ_LEVEL1);
}


/**
 * \brief Parse data
 * \param pbuf Pointer to data buffer
 * \param len Amount of data
 * \param pdata Pointer to payload
 * \param size size of payload
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
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
#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/

