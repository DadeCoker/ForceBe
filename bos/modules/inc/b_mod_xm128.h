/**
 *!
 * \file        b_mod_xm128.h
 * \version     v0.0.1
 * \date        2020/02/03
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
#ifndef __B_MOD_XM128_H__
#define __B_MOD_XM128_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"  
#if _XMODEM128_ENABLE

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup MODULES
 * \{
 */

/** 
 * \addtogroup XMODEM128
 * \{
 */

/** 
 * \defgroup XMODEM128_Exported_TypesDefinitions
 * \{
 */

typedef struct
{
    uint8_t soh;
    uint8_t number;
    uint8_t xnumber;
    uint8_t dat[128];
    uint8_t check;
}bXmodem128Info_t;


typedef void (*pcb_t)(uint16_t number, uint8_t *pbuf); 
typedef void (*psend)(uint8_t cmd);

/**
 * \}
 */
   
/** 
 * \defgroup XMODEM128_Exported_Defines
 * \{
 */

#define XMODEM128_SOH   0x01
#define XMODEM128_EOT   0x04
#define XMODEM128_ACK   0x06
#define XMODEM128_NAK   0x15
#define XMODEM128_CAN   0x18

/**
 * \}
 */
   
/** 
 * \defgroup XMODEM128_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup XMODEM128_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup XMODEM128_Exported_Functions
 * \{
 */

int bXmodem128Init(pcb_t fcb, psend fs);
int bXmodem128Parse(uint8_t *pbuf, uint8_t len);
int bXmodem128Start(void);
int bXmodem128Stop(void);

///<Called in bExec()
void bXmodem128Timeout(void);

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

