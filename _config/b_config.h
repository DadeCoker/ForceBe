/**
 *!
 * \file        b_config.h
 * \version     v0.0.1
 * \date        2019/06/05
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 Bean
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

//<<< Use Configuration Wizard in Context Menu >>>

//<h> Version Configuration
//<o> Hardware version
#define HW_VERSION 211212
//<o> Firmware version
#define FW_VERSION 80104
//<s> Firware Name
#define FW_NAME "BabyOS"
//</h>

//<h> Platform Configuration
//<o> Tick Frequncy HZ
#define TICK_FRQ_HZ 1000
//<o> MCU Platform
//<1001=> STM32F10X_LD
//<1002=> STM32F10X_MD
//<1003=> STM32F10X_HD
//<1004=> STM32F10X_CL
//<1101=> STM32G0X0
//<1201=> STM32L41X~L46X
//<2001=> NATION_L40X
//<2101=> NATION_G45X
//<3001=> MM32SPIN2X
//<3002=> MM32SPIN0X
//<4001=> HC32L13X
//<4002=> HC32L07X
//<7001=> CH32F103
//<8001=> TH32F50343
//<9999=> UBUNTU
#define MCU_PLATFORM 9999
//</h>

//<h> Hal Configuration
//<q> Hardware Interface Variable Enable/Disable
#define _HALIF_VARIABLE_ENABLE 0
//</h>

//<h> Utils Configuration
//<o> Debug level
//<0=> off
//<1=> all
//<2=> warn+error
//<3=> error
#define _DEBUG_ENABLE 1

//<e> MemoryPool Enable/Disable
#define _MEMP_ENABLE 0
//<o> MemoryPool Block Size (B)
#define MEMP_BLOCK_SIZE 32
//<o> MemoryPool Max Size (B)
#define MEMP_MAX_SIZE 1024
//<q> MemoryPool Monitor Enable/Disable
#define _MEMP_MONITOR_ENABLE 1
//</e>

//<e> AT Enable/Disable
#define _AT_ENABLE 0
//<o> AT Queue Length
#define AT_Q_LEN 3
//<o> AT Command Retry Count
#define AT_RETRY_COUNT 2
//</e>

//</h>

//<h> Drivers Configuration

//<o> PCF8574 Default Output Value
#define PCF8574_DEFAULT_OUTPUT 0

//<o> Maximum number of rows of matrix keys
#define MATRIX_KEYS_ROWS 4
//<o> Maximum number of columns of matrix keys
#define MATRIX_KEYS_COLUMNS 4

//</h>

//<h> Modules Configuration
//<q> Algorithm Enable/Disable
#define _BOS_ALGO_ENABLE 0

//<q> Modules Enable/Disable
#define _BOS_MODULES_ENABLE 1

//<e> Adchub Enable/Disable
#define _ADCHUB_ENABLE 0
//<o> Filter Buffer Size
#define FILTER_BUF_SIZE 10
//</e>

//<e> Error Manage Enable/Disable
#define _ERROR_MANAGE_ENABLE 0
//<o> Error Queue Length
#define ERROR_Q_LENGTH 10
//</e>

//<e> Modbus Enable/Disable
#define _MODBUS_ENABLE 0
//<o> Modbus Buffer Size
#define MODBUS_BUF_SIZE 200
//</e>

//<e> Protocol Enable/Disable
#define _PROTO_ENABLE 0
//<o> Size of The Device ID Field
//<2=> 2Bytes
//<4=> 4Bytes
#define PROTO_FID_SIZE 4
//<o> Size of The Len Field
//<1=> 1Byte
//<2=> 2Bytes
#define PROTO_FLEN_SIZE 2
//<e> Encrypt Enable/Disable
#define _PROTO_ENCRYPT_ENABLE 0
//<o> Secret Key 1
#define SECRET_KEY1 1
//<o> Secret Key 2
#define SECRET_KEY2 22
//<o> Secret Key 3
#define SECRET_KEY3 333
//<o> Secret Key 4
#define SECRET_KEY4 4444
//</e>
//</e>

//<q> KV Enable/Disable
#define _KV_ENABLE 0

//<q> XMODEM128 Enable/Disable
#define _XMODEM128_ENABLE 0

//<q> YMODEM Enable/Disable
#define _YMODEM_ENABLE 0

//<e> Menu Enable/Disable
#define _MENU_ENABLE 0
//<o> Menu Item Number
#define MENU_ITEM_NUMBER 10
//</e>

//<q> Soft-timer Enable/Disable
#define _TIMER_ENABLE 0

//<q> Soft-pwm Enable/Disable
#define _PWM_ENABLE 0

//<q> Tuning Parameters Enable/Disable
#define _PARAM_ENABLE 0

//<q> State Machine Enable/Disable
#define _STATE_ENABLE 0

//<e> IAP Enable/Disable
#define _IAP_ENABLE 0

//<q> File Checksum Enable/Disable
#define _IAP_CHECKSUM_ENABLE 1

//<o> McuFlash Base Address
#define MCUFLASH_BASE_ADDR 0x08000000

//<o> Bootloader Start Address
#define BOOT_START_ADDR 0x08000000

//<o> IAP Flag Address
#define IAP_FLAG_ADDR 0x08014000

//<o> App Start Address
#define APP_START_ADDR 0x08014800

//<o> Receive New Firmware
//<0=> Bootloader
//<1=> Application
#define RECEIVE_FIRMWARE_MODE 1

//<o> File Cache
//<0=> NONE
//<1=> On-Chip Flash
//<2=> SPI FLASH
#define IAP_FILE_CACHE 2

//<o> Firmware Save Address
#define IAP_FW_SAVE_ADDR 0x00000000

//<e> Backup Enable/Disable
#define _BACKUP_ENABLE 0

//<o> Backup Location
//<0=> On-Chip Flash
//<1=> SPI FLASH
#define IAP_BACKUP_LOCATION 1

//<o> Backup Address
#define IAP_BACKUP_ADDR 0x00032000

//<o> Backup Area Size
#define IAP_BACKUP_SIZE 0x14000

//</e>
//</e>

//</h>

//<h> Third Party Configuration

//<e> CmbackTrace Enable/Disable
#define _CMBACKTRACE_ENABLE 0

//<o> ARM Cortex M ?
//<0=> ARM_CORTEX_M0
//<1=> ARM_CORTEX_M3
//<2=> ARM_CORTEX_M4
//<3=> ARM_CORTEX_M7
//<4=> ARM_CORTEX_M33
#define CMB_CPU_PLATFORM_TYPE 1

//</e>

//<e> NR Micro Shell Enable/Disable
#define _NR_MICRO_SHELL_ENABLE 0

//<s> The user's name
#define NR_SHELL_USER_NAME "nr@bos:"

//<o> ANSI command line buffer size
#define NR_ANSI_LINE_SIZE 100

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

//</e>

//<e> FlexibleButton Enable/Disable
#define _FLEXIBLEBUTTON_ENABLE 0
//<o> How often flex_button_scan () is called
#define FLEX_BTN_SCAN_FREQ_HZ 50
//<o> Short Press Time, default xms
#define FLEX_BTN_SHORT_XMS 1000
//<o> Long Press Time, default xms
#define FLEX_BTN_LONG_XMS 3000
//<o> LongLong Press Time, default xms
#define FLEX_BTN_LLONG_XMS 5000
//<o> Multiple clicks interval, default xms
#define MULTIPLE_CLICKS_INTERVAL_XMS 300
//</e>

//<e> UGUI Enable/Disable
#define _UGUI_ENABLE 0

//<o> Font
//<0=> FONT_6X8
//<1=> FONT_8X12
//<2=> FONT_12X16
//<3=> FONT_XBF
#define GUI_FONT 0
//<o> Encoding
//<0=> UTF-8
//<1=> UNICODE
#define GUI_ENCODING 0
//<o> XBF Base Address
#define XBF_FILE_ADDR 0
//</e>

//<e> FS Enable/Disable
#define _FS_ENABLE 0

//<q> bFS_Test() Enable/Disable
#define _FS_TEST_ENABLE 1

//<o> FatFS or LittleFS
//<0=> FatFS
//<1=> LittleFS
#define FS_SELECT 0

//<e> SPIFLASH Enable/Disable (Device No: bSPIFLASH)
#define _SPIFLASH_ENABLE 1
//<o> SPI Flash x(MB)
#define SPIFLASH_SIZE 8
//</e>

//<e> SD Card Enable/Disable (Device No: bSD)
#define _SD_ENABLE 0
//<o> SD Card x(G)
#define SD_SIZE_XG 1
//</e>
//</e>

//<e> CJSON Enable/Disable
#define _CJSON_ENABLE 0

//<o> Malloc & Free Functions
//<0=> Default(malloc/free)
//<1=> bMalloc&bFree
#define CJSON_MEM_FUNCTIONS 0

//</e>

//<e> COREMARK Enable/Disable
#define _COREMARK_ENABLE 0

//<q> HAS_FLOAT Define to 1 if the platform supports floating point.
#define HAS_FLOAT 1

//<s> COMPILER_VERSION Please put compiler version here (e.g. gcc 4.1)
#define COMPILER_VERSION "ARMCC V5.06"

//<s> COMPILER_FLAGS Please put compiler flags here (e.g. -o3)
#define COMPILER_FLAGS "-O3"

//</e>

//</h>

#include "b_type.h"

#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
