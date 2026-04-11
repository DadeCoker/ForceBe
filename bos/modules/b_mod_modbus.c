/**
 *!
 * \file        b_mod_modbus.c
 * \version     v0.0.2
 * \date        2020/05/13
 * \author      Bean(notrynohigh@outlook.com)
 * \note        This is not a complete Modbus-RTU stack. It only supports 
 *              function code: 03(read holding registers), 16(preset multiple registers).
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
   
/*Includes ----------------------------------------------*/
#include "b_mod_modbus.h"
#if _MODBUS_ENABLE
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
 * \addtogroup MODBUS
 * \{
 */

/** 
 * \defgroup MODBUS_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup MODBUS_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup MODBUS_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup MODBUS_Private_Variables
 * \{
 */
static bMB_Info_t bMB_InfoTable[_MODBUS_I_NUMBER];
static uint32_t bMB_InfoIndex = 0;   
static uint8_t bMB_SendBuff[_MODBUS_BUF_SIZE];

static const uint8_t aucCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};

static const uint8_t aucCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};
/**
 * \}
 */
   
/** 
 * \defgroup MODBUS_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup MODBUS_Private_Functions
 * \{
 */

static uint16_t _bMBCRC16( uint8_t * pucFrame, uint16_t usLen )
{
    uint8_t ucCRCHi = 0xFF;
    uint8_t ucCRCLo = 0xFF;
    int iIndex;

    while( usLen-- )
    {
        iIndex = ucCRCLo ^ *( pucFrame++ );
        ucCRCLo = ( uint8_t )( ucCRCHi ^ aucCRCHi[iIndex] );
        ucCRCHi = aucCRCLo[iIndex];
    }
    return ( uint16_t )( ucCRCHi << 8 | ucCRCLo );
}

static int _bMB_CheckRTUReadACK(uint8_t *psrc, uint16_t len)
{
    if(psrc == NULL)
    {
        return -1;
    }
    bMB_RTU_ReadRegsAck_t *phead = (bMB_RTU_ReadRegsAck_t *)psrc;
    
    if((sizeof(bMB_RTU_ReadRegsAck_t) - 1 + phead->len + 2) > len)
    {
        return -1;
    }
    
    uint16_t crc = _bMBCRC16(psrc, sizeof(bMB_RTU_ReadRegsAck_t) - 1 + phead->len);
    uint16_t crc2 = ((uint16_t *)(psrc + sizeof(bMB_RTU_ReadRegsAck_t) - 1 + phead->len))[0];
    if(crc != crc2)
    {
        b_log_e("crc error %x %x\r\n", crc, crc2);
        return -1;
    }
    return 0;
}


static int _bMB_CheckRTUWriteACK(uint8_t *psrc, uint16_t len)
{
    if(psrc == NULL)
    {
        return -1;
    }
    if(sizeof(bMB_RTU_WriteRegsAck_t) > len)
    {
        return -1;
    }
    uint16_t crc = _bMBCRC16(psrc, sizeof(bMB_RTU_WriteRegsAck_t) - 2);
    uint16_t crc2 = ((uint16_t *)(psrc + (sizeof(bMB_RTU_WriteRegsAck_t) - 2)))[0];
    if(crc != crc2)
    {
        b_log_e("crc error %x %x\r\n", crc, crc2);
        return -1;
    }
    return 0;
}

/**
 * \}
 */
   
/** 
 * \addtogroup MODBUS_Exported_Functions
 * \{
 */
 
/**
 * \brief Create a MB instance
 * \param sf Pointer to the function to send the bytes
 * \param cb A Callback function to receive the response from the slave device
 * \retval Instance ID
 *          \arg >=0  valid
 *          \arg -1   invalid
 */
int bMB_Regist(pMB_Send_t sf, pMB_Callback_t cb)
{
    if(bMB_InfoIndex >= _MODBUS_I_NUMBER || sf == NULL || cb == NULL)
    {
        return -1;    
    }
    bMB_InfoTable[bMB_InfoIndex].f = sf;
    bMB_InfoTable[bMB_InfoIndex].cb = cb;
    bMB_InfoIndex += 1;
    return (bMB_InfoIndex - 1);
}


/**
 * \brief Modbus RTU Read Regiter
 * \param no Instance ID \ref bMB_Regist
 * \param addr Device address
 * \param func Function code
 * \param reg  Register address
 * \param num  the number of Register to be read
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bMB_ReadRegs(int no, uint8_t addr, uint8_t func, uint16_t reg, uint16_t num)
{
    bMB_RTU_ReadRegs_t wd;
    if(no < 0 || no >= bMB_InfoIndex)
    {
        return -1;
    }
    wd.addr = addr;
    wd.func = func;
    wd.reg = L2B_B2L_16b(reg);
    wd.num = L2B_B2L_16b(num);
    wd.crc = _bMBCRC16((uint8_t *)&wd, sizeof(bMB_RTU_ReadRegs_t) - sizeof(wd.crc));
    bMB_InfoTable[no].f((uint8_t *)&wd, sizeof(bMB_RTU_ReadRegs_t));
    return 0;
}


/**
 * \brief Modbus write regs
 * \param no Instance ID \ref bMB_Regist
 * \param addr Device address
 * \param func Function code
 * \param reg  Register address
 * \param num  The number of Register to write to
 * \param reg_value  Pointer to the register value 
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bMB_WriteRegs(int no, 
                  uint8_t addr, 
                  uint8_t func, 
                  uint16_t reg, 
                  uint16_t num, 
                  uint16_t *reg_value)
{
    bMB_RTU_WriteRegs_t *phead = (bMB_RTU_WriteRegs_t *)bMB_SendBuff;
    int i = 0;
    uint16_t tmp_len = 0;
    if(no < 0 || no >= bMB_InfoIndex || reg_value == NULL)
    {
        return -1;
    }
    phead->addr = addr;
    phead->func = func;
    phead->reg = L2B_B2L_16b(reg);
    phead->num = L2B_B2L_16b(num);
    phead->len = num * 2;
    memcpy(phead->param, (uint8_t *)reg_value, phead->len);
    for(i = 0;i < num;i++)
    {
        ((uint16_t *)phead->param)[i] = L2B_B2L_16b(((uint16_t *)phead->param)[i]);
    }
    tmp_len = sizeof(bMB_RTU_WriteRegs_t) - 1 + phead->len;
    ((uint16_t *)&bMB_SendBuff[tmp_len])[0] = _bMBCRC16(bMB_SendBuff, tmp_len);
    bMB_InfoTable[no].f(bMB_SendBuff, tmp_len + 2);
    return 0;
}



int bMB_FeedReceivedData(int no, uint8_t *pbuf, uint16_t len)
{
    bMB_SlaveDeviceData_t _data;
    int i = 0;
    int retval = -1;
    bMB_RTU_ReadRegsAck_t *pr = (bMB_RTU_ReadRegsAck_t *)pbuf;
    bMB_RTU_WriteRegsAck_t *pw = (bMB_RTU_WriteRegsAck_t *)pbuf;
    if(pbuf == NULL)
    {
        return -1;
    }
    if(_bMB_CheckRTUReadACK(pbuf, len) == 0)
    {
        _data.type = 0;
        _data.result.r_result.func = pr->func;
        _data.result.r_result.reg_num = pr->len / 2;
        _data.result.r_result.reg_value = (uint16_t *)pr->buf;
        
        for(i = 0;i < _data.result.r_result.reg_num;i++)
        {
            _data.result.r_result.reg_value[i] = L2B_B2L_16b(_data.result.r_result.reg_value[i]);
        }
        bMB_InfoTable[no].cb(&_data);
        retval = 0;
    }
    else if(_bMB_CheckRTUWriteACK(pbuf, len) == 0)
    {
        _data.type = 1;
        _data.result.w_result.func = pw->func;
        _data.result.w_result.reg = L2B_B2L_16b(pw->reg);
        _data.result.w_result.reg_num = L2B_B2L_16b(pw->num);
        bMB_InfoTable[no].cb(&_data);
        retval = 0;
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


/**
 * \}
 */
#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/

