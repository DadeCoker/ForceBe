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
#define NULL    ((void *)0)
#endif

//<<< Use Configuration Wizard in Context Menu >>>

//<h> Compile Configuration

//<o> Hardware version
#define HW_VERSION                  200310

//<o> Firmware version
#define FW_VERSION                  30000

//<o> Tick Frequncy HZ
#define _TICK_FRQ_HZ          	    1000
#define MS2TICKS(m)                 (m / (1000 / _TICK_FRQ_HZ))
//<q> UTC2000 Enable/Disable
#define _UTC2000_ENABLE           	0

//<q> Lunar Enable/Disable
#define _LUNAR_ENABLE           	0

//<o> Debug level
//<0=> off
//<1=> all
//<2=> warn+error
//<3=> error
#define _DEBUG_ENABLE           	1


//<e> Battery Enable/Disable
#define _BATTERY_ENABLE           	1
//<o> Battery Threshold (mv)
#define _BATTERY_THRESHOLD      	3350
//<o> Battery Detection Cycle (ms)
#define _BATTERY_D_CYCLE            60000
//</e>


//<e> Error Manage Enable/Disable
#define _ERROR_MANAGE_ENABLE       	1
//<o> Error Queue Length
#define _ERROR_Q_LENGTH          	5
//</e>

//<e> Event Manage Enable/Disable
#define _EVENT_MANAGE_ENABLE       	0
//<o> Event Queue Length
#define _EVENT_Q_LENGTH          	5
//</e>

//<e> Modbus Enable/Disable
#define _MODBUS_ENABLE           	0
//<o> Modbus Instance Number
#define _MODBUS_I_NUMBER          	1
//</e>

//<e> Protocol Enable/Disable
#define _PROTO_ENABLE         		0
//<o> Size of The Device ID Field
//<2=> 2Bytes
//<4=> 4Bytes
#define _PROTO_FID_SIZE             4
//<o> Size of The Len Field
//<1=> 1Byte
//<2=> 2Bytes
#define _PROTO_FLEN_SIZE            1 
//<o> Protocol Instance Number
#define _PROTO_I_NUMBER             1
//<e> Encrypt Enable/Disable 
#define _PROTO_ENCRYPT_ENABLE       0
//<o> Secret Key 1
#define _SECRET_KEY1                1
//<o> Secret Key 2
#define _SECRET_KEY2                22
//<o> Secret Key 3
#define _SECRET_KEY3                333
//<o> Secret Key 4
#define _SECRET_KEY4                4444
//</e>
//</e>

//<e> Save Data Enable/Disable
#define _SAVE_DATA_ENABLE         	0

//<e> Save Data ClassA
#define _SAVE_DATA_A_ENABLE         0
//<o> ClassA Instance Number
#define _SAVE_DATA_A_I_NUMBER       1
//</e>
//<e> Save Data ClassB
#define _SAVE_DATA_B_ENABLE         0
//<o> ClassB Instance Number
#define _SAVE_DATA_B_I_NUMBER       1
//</e>
//<e> Save Data ClassC
#define _SAVE_DATA_C_ENABLE         0
//<o> ClassC Instance Number
#define _SAVE_DATA_C_I_NUMBER       1
//</e>
//</e>


//<q> KV Enable/Disable
#define _KV_ENABLE             	    0



//<e> FIFO Enable/Disable
#define _FIFO_ENABLE             	0
//<o> FIFO Instance Number
#define _FIFO_I_NUMBER              1
//</e>


//<q> XMODEM128 Enable/Disable
#define _XMODEM128_ENABLE           0

//<q> YMODEM Enable/Disable
#define _YMODEM_ENABLE              0


//<e> Menu Enable/Disable
#define _MENU_ENABLE                0
//<o> Menu Item Number
#define _MENU_ITEM_NUMBER           10
//</e>

//<e> Heap4 Enable/Disable
#define _HEAP_ENABLE                0
//<o> Heap Size
#define _HEAP_SIZE                  1024
//<e> External SDRAM ?
#define _EX_SDRAM_ENABLE            0
//<o> Heap Address
#define _EX_HEAP_ADDRESS            0xC0000000
//</e>
//</e>


//</h>


//<h> Third Party Configuration

//<e> NR Micro Shell Enable/Disable
#define _NR_MICRO_SHELL_ENABLE              0

//<s> The user's name
#define NR_SHELL_USER_NAME                 "nr@bos:"

//<o> ANSI command line buffer size
#define NR_ANSI_LINE_SIZE                   100

#define NR_SHELL_CMD_LINE_MAX_LENGTH        NR_ANSI_LINE_SIZE

//<o> Maximum username length
#define NR_SHELL_USER_NAME_MAX_LENGTH       30

//<o> Maximum command name length
#define NR_SHELL_CMD_NAME_MAX_LENGTH        10

//<o> Maximum number of parameters in a command
#define NR_SHELL_CMD_PARAS_MAX_NUM          10

//<o> History commands
//<0-3>
#define NR_SHELL_MAX_CMD_HISTORY_NUM        3

//<o> History command cache length
#define NR_SHELL_CMD_HISTORY_BUF_LENGTH     253


//<o> End of line
//<0=> \n
//<1=> \r
//<2=> \r\n
#define NR_SHELL_END_OF_LINE                1

//<q> Support all ANSI codes enable/disable
#define NR_SHLL_FULL_ANSI                   1

//<q> Show logo enable/disable
#define NR_SHELL_SHOW_LOG                   1

//<q> Echo enable/disable
#define NR_SHELL_ECHO_ENABLE                1


#if _DEBUG_ENABLE
#define shell_printf(fmt, args...)          b_log(fmt, ##args);

#if NR_SHELL_ECHO_ENABLE
#define ansi_show_char(x)                   b_log("%c", x)
#else
#define ansi_show_char(x)
#endif

#endif
//</e>

//<e> FlexibleButton Enable/Disable
#define _FLEXIBLEBUTTON_ENABLE              0
//<o> How often flex_button_scan () is called
#define FLEX_BTN_SCAN_FREQ_HZ               50 
//<o> Number of Buttons
#define FLEX_BTN_NUMBER                     3
//</e>

//<e> UGUI Enable/Disable
#define _UGUI_ENABLE                        0
//<o> X PHYSICAL SIZE
#define _LCD_X_SIZE                         240 
//<o> Y PHYSICAL SIZE
#define _LCD_Y_SIZE                         320 

//<o> X TOUCH AD MIN
#define _X_TOUCH_AD_MIN                     476 
//<o> X TOUCH AD MAX
#define _X_TOUCH_AD_MAX                     3752 

//<o> Y TOUCH AD MIN
#define _Y_TOUCH_AD_MIN                     338 
//<o> Y TOUCH AD MAX
#define _Y_TOUCH_AD_MAX                     3592 

//<o> LCD DISPLAY MODE
//<0=> H
//<1=> V
#define _LCD_DISP_MODE                      0

//</e>


//<e> CmBacktrace Enable/Disable
#define _CMBACKTRACE_ENABLE                 0

//<o> CMB_CPU_ARM_CORTEX_M?
//<0=> ARM_CORTEX_M0
//<1=> ARM_CORTEX_M3
//<2=> ARM_CORTEX_M4
//<3=> ARM_CORTEX_M7
#define CMB_CPU_PLATFORM_TYPE               1

//<o> Print Language
//<0=> English
//<1=> Chinese
#define CMB_PRINT_LANGUAGE                  1
//</e>


//</h>

#if _DEBUG_ENABLE
#include "b_mod_log.h"
#else
#define b_log_i(...) 
#define b_log_w(...) 
#define b_log_e(...) 
#define b_log(...)
#endif

typedef struct bPollingFuncStruct
{
    void (*pPollingFunction)(void);
    struct bPollingFuncStruct *pnext;
}bPollingFunc_t; 

int bRegistPollingFunc(bPollingFunc_t *pfunc);

#ifdef __cplusplus
	}
#endif



#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/


