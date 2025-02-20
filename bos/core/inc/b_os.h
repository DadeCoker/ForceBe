/**
 *!
 * \file        b_os.h
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
#ifndef __B_OS_H__
#define __B_OS_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_battery.h"
#include "b_core.h"
#include "b_crc32.h"
#include "b_device.h"
#include "b_error.h"
#include "b_event.h"
#include "b_modbus.h"
#include "b_protocol.h"
#include "b_sda.h"
#include "b_sdb.h"
#include "b_sdc.h"
#include "b_sum.h"
#include "b_tx.h"
#include "b_utc.h"
#include "b_fifo.h"
#include "b_at.h"
#include "b_shell.h"
#include "b_kv.h"
#include "b_xm128.h" 
#include "b_ymodem.h" 
#include "b_button.h"

#include "b_hal.h"
#include "b_utils.h"
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS
 * \{
 */


/** 
 * \defgroup BOS_Exported_Functions
 * \{
 */
int bInit(void);
int bExec(void);
/**
 * \}
 */
 
/**
 * \}
 */ 

/**
 * \}
 */
 
#ifdef __cplusplus
	}
#endif 

#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/

