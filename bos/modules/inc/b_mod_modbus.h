/**
 *!
 * \file        b_mod_modbus.h
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
#ifndef __B_MOD_MODBUS_H__
#define __B_MOD_MODBUS_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

#include "b_config.h"

#if _MODBUS_ENABLE

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
 * \defgroup MODBUS_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
    uint8_t   func;
    uint8_t   reg_num;
    uint16_t *reg_value;
} bMB_ReadResult_t;

typedef struct
{
    uint8_t  func;
    uint16_t reg;
    uint16_t reg_num;
} bMB_WriteResult_t;

typedef struct
{
    uint8_t type;  // 0: read     1:write
    union
    {
        bMB_ReadResult_t  r_result;
        bMB_WriteResult_t w_result;
    } result;
} bMB_SlaveDeviceData_t;

typedef void (*pMB_Send_t)(uint8_t *pbuf, uint16_t len);
typedef void (*pMB_Callback_t)(bMB_SlaveDeviceData_t *pdata);

typedef struct
{
    pMB_Send_t     f;
    pMB_Callback_t cb;
} bMB_Info_t;

typedef bMB_Info_t bModbusInstance_t;
/**
 * \}
 */

/**
 * \defgroup MODBUS_Exported_Defines
 * \{
 */

#define bMODBUS_INSTANCE(name, pSendData, pCallback) \
    bModbusInstance_t name = {.f = pSendData, .cb = pCallback};

/**
 * \}
 */

/**
 * \defgroup MODBUS_Exported_Macros
 * \{
 */
#define L2B_B2L_16b(n) ((((n)&0xff) << 8) | (((n)&0xff00) >> 8))
/**
 * \}
 */

/**
 * \defgroup MODBUS_Exported_Functions
 * \{
 */
///< pModbusInstance \ref  bMODBUS_INSTANCE
int bMB_ReadRegs(bModbusInstance_t *pModbusInstance, uint8_t addr, uint8_t func, uint16_t reg,
                 uint16_t num);
int bMB_WriteRegs(bModbusInstance_t *pModbusInstance, uint8_t addr, uint8_t func, uint16_t reg,
                  uint16_t num, uint16_t *reg_value);
int bMB_FeedReceivedData(bModbusInstance_t *pModbusInstance, uint8_t *pbuf, uint16_t len);
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

#ifdef __cplusplus
}
#endif

#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
