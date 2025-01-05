/**
 *!
 * \file        b_ymodem.h
 * \version     v0.0.1
 * \date        2020/02/12
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
#ifndef __B_YMODEM_H__
#define __B_YMODEM_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"  
#if _YMODEM_ENABLE

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup YMODEM
 * \{
 */

/** 
 * \defgroup YMODEM_Exported_TypesDefinitions
 * \{
 */

typedef struct
{
    uint8_t soh;
    uint8_t number;
    uint8_t xnumber;
    uint8_t dat[128];
    uint8_t crc_h;
    uint8_t crc_l;
}bYmodem128Info_t;


typedef struct
{
    uint8_t stx;
    uint8_t number;
    uint8_t xnumber;
    uint8_t dat[1024];
    uint8_t crc_h;
    uint8_t crc_l;
}bYmodem1kInfo_t;



typedef void (*pymcb_t)(uint8_t t, uint8_t number, uint8_t *pbuf, uint16_t len); 
typedef void (*pymsend)(uint8_t cmd);

/**
 * \}
 */
   
/** 
 * \defgroup YMODEM_Exported_Defines
 * \{
 */

#define YMODEM_SOH   0x01
#define YMODEM_STX   0x02
#define YMODEM_EOT   0x04
#define YMODEM_ACK   0x06
#define YMODEM_NAK   0x15
#define YMODEM_CAN   0x18
#define YMODEM_C     0x43


#define YMODEM_FILENAME     0
#define YMODEM_FILEDATA     1

/**
 * \}
 */
   
/** 
 * \defgroup YMODEM_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup YMODEM_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup YMODEM_Exported_Functions
 * \{
 */

int bYmodemInit(pymcb_t fcb, pymsend fs);
int bYmodemParse(uint8_t *pbuf, uint16_t len);
int bYmodemStart(void);
int bYmodemStop(void);
void bYmodemTimeout(void);
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

