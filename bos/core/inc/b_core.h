/**
 *!
 * \file        b_core.h
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
#ifndef __B_CORE_H__
#define __B_CORE_H__ 

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup CORE
 * \{
 */

/** 
 * \defgroup CORE_Exported_TypesDefinitions
 * \{
 */

typedef struct
{
    uint8_t dev_no;
    uint8_t name[8];
}bCoreDevTable_t;


typedef struct
{
    uint8_t number;
    uint8_t flag;
    uint8_t status;
    uint32_t lseek;
}bCoreFd_t;

/**
 * \}
 */
   
/** 
 * \defgroup CORE_Exported_Defines
 * \{
 */

#define BCORE_FLAG_R            0
#define BCORE_FLAG_W            1
#define BCORE_FLAG_RW           2

#define BCORE_STA_NULL          0
#define BCORE_STA_OPEN          1

#define BCORE_FD_MAX            10

#ifndef NULL
#define NULL    ((void *)0)
#endif
/**
 * \}
 */
   
/** 
 * \defgroup CORE_Exported_Macros
 * \{
 */
#define IS_VALID_FLAG(n)        (n == BCORE_FLAG_R || n == BCORE_FLAG_W || n == BCORE_FLAG_RW) 
/**
 * \}
 */
   
/** 
 * \defgroup CORE_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup CORE_Exported_Functions
 * \{
 */

int bOpen(uint8_t dev_no, uint8_t flag);
int bRead(int fd, uint8_t *pdata, uint16_t len);
int bWrite(int fd, uint8_t *pdata, uint16_t len);
int bCtl(int fd, uint8_t cmd, void *param);
int bLseek(int fd, uint32_t off);
int bClose(int fd);

int bCoreIsIdle(void); 
void bEnterInterrupt(void);
void bExitInterrupt(void);
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


/************************ Copyright (c) 2019 Bean *****END OF FILE****/


