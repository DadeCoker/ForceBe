/**
 *!
 * \file        b_mod_sda.h
 * \version     v0.0.2
 * \date        2020/05/16
 * \author      Bean(notrynohigh@outlook.com)
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
#ifndef __B_MOD_SDA_H__
#define __B_MOD_SDA_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

#include "b_config.h"

#if (_SAVE_DATA_ENABLE && _SAVE_DATA_A_ENABLE)
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup SAVE_DATA
 * \{
 */

/**
 * \defgroup SDA_Exported_TypesDefinitions
 * \{
 */

typedef struct
{
    uint32_t time_interval;  // s
    uint32_t total_time;     // s
    uint32_t data_size;
    uint32_t fbase_address;
    uint32_t fsector_size;
} bSDA_Struct_t;

typedef struct
{
    uint8_t       dev_no;
    uint8_t       e_flag;
    uint16_t      n_per_eu;
    uint16_t      un_number;
    uint32_t      total_size;
    bSDA_Struct_t st;
} bSDA_Info_t;

typedef bSDA_Info_t bSDA_Instance_t;

/**
 * \}
 */

/**
 * \defgroup SDA_Exported_Defines
 * \{
 */

#define bSDA_INSTANCE(name, _time_interval, _total_time, _data_size, _fbase_addr, _fsector_size, \
                      _dev_no)                                                                   \
    bSDA_Instance_t name = {.st.time_interval = _time_interval,                                  \
                            .st.total_time    = _total_time,                                     \
                            .st.data_size     = _data_size,                                      \
                            .st.fbase_address = _fbase_addr,                                     \
                            .st.fsector_size  = _fsector_size,                                   \
                            .dev_no           = _dev_no};

/**
 * \}
 */

/**
 * \defgroup SDA_Exported_Functions
 * \{
 */
///< pSDA_Instance \ref bSDA_INSTANCE
int bSDA_Init(bSDA_Instance_t *pSDA_Instance);
int bSDA_Write(bSDA_Instance_t *pSDA_Instance, uint32_t utc, uint8_t *pbuf);
int bSDA_Read(bSDA_Instance_t *pSDA_Instance, uint32_t utc, uint8_t *pbuf);
int bSDA_TimeChanged(bSDA_Instance_t *pSDA_Instance, uint32_t o_utc, uint32_t n_utc);

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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
