/**
 *!
 * \file        b_config.h
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
#ifndef __B_CONFIG_H__
#define __B_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

//<<< Use Configuration Wizard in Context Menu >>>

//<h> Compile Configuration

//<o> Hardware version
#define HW_VERSION 200310

//<o> Firmware version
#define FW_VERSION 60000

//<o> Tick Frequncy HZ
#define _TICK_FRQ_HZ 1000
#define MS2TICKS(m) (m / (1000 / _TICK_FRQ_HZ))

//<q> Algorithm Enable/Disable
#define _BOS_ALGO_ENABLE 1

//<q> Modules Enable/Disable
#define _BOS_MODULES_ENABLE 1

//<q> UTC2000 Enable/Disable
#define _UTC2000_ENABLE 1

//<q> Lunar Enable/Disable
#define _LUNAR_ENABLE 1

//<o> Debug level
//<0=> off
//<1=> all
//<2=> warn+error
//<3=> error
#define _DEBUG_ENABLE 1

//<e> Battery Enable/Disable
#define _BATTERY_ENABLE 1
//<o> Battery Threshold (mv)
#define _BATTERY_THRESHOLD 3350
//<o> Battery Detection Cycle (ms)
#define _BATTERY_D_CYCLE 60000
//</e>

//<e> Error Manage Enable/Disable
#define _ERROR_MANAGE_ENABLE 1
//<o> Error Queue Length
#define _ERROR_Q_LENGTH 10
//</e>

//<q> Event Manage Enable/Disable
#define _EVENT_MANAGE_ENABLE 1

//<e> Modbus Enable/Disable
#define _MODBUS_ENABLE 1
//<o> Modbus Buffer Size
#define _MODBUS_BUF_SIZE 200
//</e>

//<e> Protocol Enable/Disable
#define _PROTO_ENABLE 1
//<o> Size of The Device ID Field
//<2=> 2Bytes
//<4=> 4Bytes
#define _PROTO_FID_SIZE 4
//<o> Size of The Len Field
//<1=> 1Byte
//<2=> 2Bytes
#define _PROTO_FLEN_SIZE 1
//<e> Encrypt Enable/Disable
#define _PROTO_ENCRYPT_ENABLE 0
//<o> Secret Key 1
#define _SECRET_KEY1 1
//<o> Secret Key 2
#define _SECRET_KEY2 22
//<o> Secret Key 3
#define _SECRET_KEY3 333
//<o> Secret Key 4
#define _SECRET_KEY4 4444
//</e>
//</e>

//<e> Save Data Enable/Disable
#define _SAVE_DATA_ENABLE 1

//<q> Save Data ClassA
#define _SAVE_DATA_A_ENABLE 0

//<q> Save Data ClassB
#define _SAVE_DATA_B_ENABLE 1

//</e>

//<q> KV Enable/Disable
#define _KV_ENABLE 1

//<q> FIFO Enable/Disable
#define _FIFO_ENABLE 1

//<q> XMODEM128 Enable/Disable
#define _XMODEM128_ENABLE 1

//<q> YMODEM Enable/Disable
#define _YMODEM_ENABLE 1

//<e> Menu Enable/Disable
#define _MENU_ENABLE 1
//<o> Menu Item Number
#define _MENU_ITEM_NUMBER 10
//</e>

//<e> Heap4 Enable/Disable
#define _HEAP_ENABLE 1
//<o> Heap Size
#define _HEAP_SIZE 1024
//<e> External SDRAM ?
#define _EX_SDRAM_ENABLE 0
//<o> Heap Address
#define _EX_HEAP_ADDRESS 0xC0000000
//</e>
//</e>

//<q> Soft-timer Enable/Disable
#define _TIMER_ENABLE 1

//<q> Tuning Parameters Enable/Disable
#define _PARAM_ENABLE 0

//</h>

//<h> Third Party Configuration

//<e> NR Micro Shell Enable/Disable
#define _NR_MICRO_SHELL_ENABLE 1

//<s> The user's name
#define NR_SHELL_USER_NAME "nr@bos:"

//<o> ANSI command line buffer size
#define NR_ANSI_LINE_SIZE 100

#define NR_SHELL_CMD_LINE_MAX_LENGTH NR_ANSI_LINE_SIZE

//<o> Maximum username length
#define NR_SHELL_USER_NAME_MAX_LENGTH 30

//<o> Maximum number of parameters in a command
#define NR_SHELL_CMD_PARAS_MAX_NUM 10

//<o> History commands
//<0-3>
#define NR_SHELL_MAX_CMD_HISTORY_NUM 1

//<o> History command cache length
#define NR_SHELL_CMD_HISTORY_BUF_LENGTH 100

//<o> End of line
//<0=> \n
//<1=> \r
//<2=> \r\n
#define NR_SHELL_END_OF_LINE 1

//<q> Support all ANSI codes enable/disable
#define NR_SHLL_FULL_ANSI 1

//<q> Echo enable/disable
#define NR_SHELL_ECHO_ENABLE 1

#if _DEBUG_ENABLE
#define shell_printf(fmt, args...) b_log(fmt, ##args);
#endif
//</e>

//<e> FlexibleButton Enable/Disable
#define _FLEXIBLEBUTTON_ENABLE 1
//<o> How often flex_button_scan () is called
#define FLEX_BTN_SCAN_FREQ_HZ 50
//<o> Number of Buttons
#define FLEX_BTN_NUMBER 3
//</e>

//<e> QPN Enable/Disable
#define _QPN_ENABLE 0
//<o> How often ISR_TIMER_COMPA_vect() and qpn_run() is called (ms)
#define QPN_PER_SEC 10
//</e>

//<e> UGUI Enable/Disable
#define _UGUI_ENABLE 1
//<o> X PHYSICAL SIZE
#define _LCD_X_SIZE 240
//<o> Y PHYSICAL SIZE
#define _LCD_Y_SIZE 320

//<o> X TOUCH AD MIN
#define _X_TOUCH_AD_MIN 476
//<o> X TOUCH AD MAX
#define _X_TOUCH_AD_MAX 3752

//<o> Y TOUCH AD MIN
#define _Y_TOUCH_AD_MIN 338
//<o> Y TOUCH AD MAX
#define _Y_TOUCH_AD_MAX 3592

//<o> LCD DISPLAY MODE
//<0=> H
//<1=> V
#define _LCD_DISP_MODE 0

//<o> Font
//<0=> FONT_6X8
//<1=> FONT_8X12
//<2=> FONT_12X16
//<3=> FONT_XBF
#define _GUI_FONT 0
//<o> Encoding
//<0=> UTF-8
//<1=> UNICODE
#define _ENCODING 0
//<o> XBF Base Address
#define _XBF_FILE_ADDR 0
//</e>

//<e> CmBacktrace Enable/Disable
#define _CMBACKTRACE_ENABLE 0

//<o> CMB_CPU_ARM_CORTEX_M?
//<0=> ARM_CORTEX_M0
//<1=> ARM_CORTEX_M3
//<2=> ARM_CORTEX_M4
//<3=> ARM_CORTEX_M7
#define CMB_CPU_PLATFORM_TYPE 1

//<o> Print Language
//<0=> English
//<1=> Chinese
#define CMB_PRINT_LANGUAGE 1
//</e>

//<e> FS Enable/Disable
#define _FS_ENABLE 1

//<q> bFS_Test() Enable/Disable
#define _FS_TEST_ENABLE 1

//<o> FatFS or LittleFS
//<0=> FatFS
//<1=> LittleFS
#define _FS_SELECT 0

//<e> SPIFLASH Enable/Disable (Device No:SPIFLASH)
#define _SPIFLASH_ENABLE 0
//<o> SPI Flash x(MB)
#define _SPIFLASH_SIZE 8
//</e>

//<e> SD Card Enable/Disable (Device No:SD)
#define _SD_ENABLE 1
//<o> SD Card x(G)
#define _SD_SIZE 1
//</e>
//</e>

//</h>

#if _DEBUG_ENABLE
#include "modules/inc/b_mod_log.h"
#else
#define b_log_i(...)
#define b_log_w(...)
#define b_log_e(...)
#define b_log(...)
#endif

#ifdef __cplusplus
}
#endif

#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
