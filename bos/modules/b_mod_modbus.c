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
#include "modules/inc/b_mod_modbus.h"
#if (defined(_MODBUS_ENABLE) && (_MODBUS_ENABLE == 1))
#include <string.h>

#include "algorithm/inc/algo_crc.h"
#include "utils/inc/b_util_log.h"

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
#pragma pack(1)

typedef struct
{
    uint8_t  addr;
    uint8_t  func;
    uint16_t reg;  // Big endian
    uint16_t num;  // Big endian
    uint16_t crc;  // Little endian
} bMB_RTU_ReadRegs_t;

typedef struct
{
    uint8_t addr;
    uint8_t func;
    uint8_t len;
    uint8_t buf[1];
} bMB_RTU_ReadRegsAck_t;

typedef struct
{
    uint8_t  addr;
    uint8_t  func;
    uint16_t reg;  // Big endian
    uint16_t num;  // Big endian
    uint8_t  len;
    uint8_t  param[1];
} bMB_RTU_WriteRegs_t;

typedef struct
{
    uint8_t  addr;
    uint8_t  func;
    uint16_t reg;  // Big endian
    uint16_t num;  // Big endian
    uint16_t crc;  // Little endian
} bMB_RTU_WriteRegsAck_t;

#pragma pack()
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
static uint8_t bMB_SendBuff[MODBUS_BUF_SIZE];
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

static uint16_t _bMBCRC16(uint8_t *pucFrame, uint16_t usLen)
{
    uint32_t crc_ret = crc_calculate(ALGO_CRC16_MODBUS, pucFrame, usLen);
    return ((uint16_t)(crc_ret & 0xffff))
}

static int _bMB_CheckRTUReadACK(uint8_t *psrc, uint16_t len)
{
    if (psrc == NULL)
    {
        return -1;
    }
    bMB_RTU_ReadRegsAck_t *phead = (bMB_RTU_ReadRegsAck_t *)psrc;

    if ((sizeof(bMB_RTU_ReadRegsAck_t) - 1 + phead->len + 2) > len)
    {
        return -1;
    }

    uint16_t crc  = _bMBCRC16(psrc, sizeof(bMB_RTU_ReadRegsAck_t) - 1 + phead->len);
    uint16_t crc2 = ((uint16_t *)(psrc + sizeof(bMB_RTU_ReadRegsAck_t) - 1 + phead->len))[0];
    if (crc != crc2)
    {
        b_log_e("crc error %x %x\r\n", crc, crc2);
        return -1;
    }
    return 0;
}

static int _bMB_CheckRTUWriteACK(uint8_t *psrc, uint16_t len)
{
    if (psrc == NULL)
    {
        return -1;
    }
    if (sizeof(bMB_RTU_WriteRegsAck_t) > len)
    {
        return -1;
    }
    uint16_t crc  = _bMBCRC16(psrc, sizeof(bMB_RTU_WriteRegsAck_t) - 2);
    uint16_t crc2 = ((uint16_t *)(psrc + (sizeof(bMB_RTU_WriteRegsAck_t) - 2)))[0];
    if (crc != crc2)
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
 * \brief Modbus RTU Read Regiter
 * \param pModbusInstance Pointr to the modbus instance
 * \param addr Device address
 * \param func Function code
 * \param reg  Register address
 * \param num  the number of Register to be read
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bMB_ReadRegs(bModbusInstance_t *pModbusInstance, uint8_t addr, uint8_t func, uint16_t reg,
                 uint16_t num)
{
    bMB_RTU_ReadRegs_t wd;
    if (pModbusInstance == NULL)
    {
        return -1;
    }
    wd.addr = addr;
    wd.func = func;
    wd.reg  = L2B_B2L_16b(reg);
    wd.num  = L2B_B2L_16b(num);
    wd.crc  = _bMBCRC16((uint8_t *)&wd, sizeof(bMB_RTU_ReadRegs_t) - sizeof(wd.crc));
    pModbusInstance->f((uint8_t *)&wd, sizeof(bMB_RTU_ReadRegs_t));
    return 0;
}

/**
 * \brief Modbus write regs
 * \param pModbusInstance Pointr to the modbus instance
 * \param addr Device address
 * \param func Function code
 * \param reg  Register address
 * \param num  The number of Register to write to
 * \param reg_value  Pointer to the register value
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bMB_WriteRegs(bModbusInstance_t *pModbusInstance, uint8_t addr, uint8_t func, uint16_t reg,
                  uint16_t num, uint16_t *reg_value)
{
    bMB_RTU_WriteRegs_t *phead   = (bMB_RTU_WriteRegs_t *)bMB_SendBuff;
    int                  i       = 0;
    uint16_t             tmp_len = 0;
    if (pModbusInstance == NULL || reg_value == NULL)
    {
        return -1;
    }
    phead->addr = addr;
    phead->func = func;
    phead->reg  = L2B_B2L_16b(reg);
    phead->num  = L2B_B2L_16b(num);
    phead->len  = num * 2;
    memcpy(phead->param, (uint8_t *)reg_value, phead->len);
    for (i = 0; i < num; i++)
    {
        ((uint16_t *)phead->param)[i] = L2B_B2L_16b(((uint16_t *)phead->param)[i]);
    }
    tmp_len                                 = sizeof(bMB_RTU_WriteRegs_t) - 1 + phead->len;
    ((uint16_t *)&bMB_SendBuff[tmp_len])[0] = _bMBCRC16(bMB_SendBuff, tmp_len);
    pModbusInstance->f(bMB_SendBuff, tmp_len + 2);
    return 0;
}

int bMB_FeedReceivedData(bModbusInstance_t *pModbusInstance, uint8_t *pbuf, uint16_t len)
{
    bMB_SlaveDeviceData_t   _data;
    int                     i      = 0;
    int                     retval = -1;
    bMB_RTU_ReadRegsAck_t  *pr     = (bMB_RTU_ReadRegsAck_t *)pbuf;
    bMB_RTU_WriteRegsAck_t *pw     = (bMB_RTU_WriteRegsAck_t *)pbuf;
    if (pbuf == NULL)
    {
        return -1;
    }
    if (_bMB_CheckRTUReadACK(pbuf, len) == 0)
    {
        _data.type                      = 0;
        _data.result.r_result.func      = pr->func;
        _data.result.r_result.reg_num   = pr->len / 2;
        _data.result.r_result.reg_value = (uint16_t *)pr->buf;

        for (i = 0; i < _data.result.r_result.reg_num; i++)
        {
            _data.result.r_result.reg_value[i] = L2B_B2L_16b(_data.result.r_result.reg_value[i]);
        }
        pModbusInstance->cb(&_data);
        retval = 0;
    }
    else if (_bMB_CheckRTUWriteACK(pbuf, len) == 0)
    {
        _data.type                    = 1;
        _data.result.w_result.func    = pw->func;
        _data.result.w_result.reg     = L2B_B2L_16b(pw->reg);
        _data.result.w_result.reg_num = L2B_B2L_16b(pw->num);
        pModbusInstance->cb(&_data);
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
