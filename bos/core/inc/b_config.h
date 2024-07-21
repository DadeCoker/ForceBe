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

//<o> Tick Frequncy HZ
#define _TICK_FRQ_HZ          	    1000

//<q> UTC2000 Enable/Disable
#define _UTC2000_ENABLE           	1

//<q> Debug Enable/Disable
#define _DEBUG_ENABLE           	1

//<e> Battery Enable/Disable
#define _BATTERY_ENABLE           	1
//<o> Battery Threshold (mv)
#define _BATTERY_THRESHOLD      	3350
//</e>


//<e> Error Manage Enable/Disable
#define _ERROR_MANAGE_ENABLE       	1
//<o> Error Queue Length
#define _ERROR_Q_LENGTH          	5
//</e>

//<e> Event Manage Enable/Disable
#define _EVENT_MANAGE_ENABLE       	1
//<o> Event Queue Length
#define _EVENT_Q_LENGTH          	5
//</e>

//<e> Modbus Enable/Disable
#define _MODBUS_ENABLE           	1
//<o> Modbus Instance Number
#define _MODBUS_I_NUMBER          	1
//</e>

//<e> Transmit Enable/Disable
#define _TX_ENABLE           		1
//<o> TX Instance Number
#define _TX_I_NUMBER          	    1
//</e>


//<e> Check Enable/Disable
#define _CHECK_ENABLE             	1
//<q> CRC32
#define _CHECK_CRC32_ENABLE         1
//<q> XOR SUM
#define _CHECK_XOR_SUM_ENABLE       1
//</e>


//<e> Save Data Enable/Disable
#define _SAVE_DATA_ENABLE         	1

//<e> Save Data ClassA
#define _SAVE_DATA_A_ENABLE         1
//<o> ClassA Instance Number
#define _SAVE_DATA_A_I_NUMBER       1
//</e>
//<e> Save Data ClassB
#define _SAVE_DATA_B_ENABLE         1
//<o> ClassB Instance Number
#define _SAVE_DATA_B_I_NUMBER       1
//</e>
//<e> Save Data ClassC
#define _SAVE_DATA_C_ENABLE         1
//<o> ClassC Instance Number
#define _SAVE_DATA_C_I_NUMBER       1
//</e>
//</e>

//<e> Protocol Enable/Disable
#define _PROTO_ENABLE         		1
//<o> Protocol Instance Number
#define _PROTO_I_NUMBER             1
//<e> OTA Enable/Disable
#define _PROTO_OTA_ENABLE         	1
//<o> OTA Info Address
#define _OTA_INFO_ADDRESS           0
//<o> OTA Data Address
#define _OTA_DATA_ADDRESS           0X1000
//<o> OTA Buffer Size
#define _OTA_BUFF_SIZE              150
//</e>
//</e>
//</h>



#if _DEBUG_ENABLE
#include <stdio.h>
#define b_log(...)       printf(__VA_ARGS__)

#else

#define b_log(...)

#endif  


#ifdef __cplusplus
	}
#endif



#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/


