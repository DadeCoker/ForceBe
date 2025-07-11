/**
 *!
 * \file        b_mod_utc.h
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
#ifndef __B_MOD_UTC_H__
#define __B_MOD_UTC_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"  
#if _UTC2000_ENABLE
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup MODULES
 * \{
 */

/** 
 * \addtogroup UTC
 * \{
 */

/** 
 * \defgroup UTC_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  week;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;
}bUTC_DateTime_t; 

typedef uint32_t    bUTC_t;

/**
 * \}
 */
   
/** 
 * \defgroup UTC_Exported_Defines
 * \{
 */
#define	YEAR_CHECK(yr)	(((yr % 400) == 0x0) || (((yr % 4) == 0) && ((yr % 100) != 0)))

#define	YEAR_DAYS(yr)	(YEAR_CHECK(yr) ? 366 : 365)
#define MAXCALCTICKS    ((uint16_t)(13105))
#define	BEGYEAR	        2000                        // UTC started at 00:00:00 January 1, 2000
#define	DAY             86400UL                     // 24 hours * 60 minutes * 60 seconds   
/**
 * \}
 */
   
/** 
 * \defgroup UTC_Exported_Macros
 * \{
 */
#define IS_TIME_VALID(n)        (n.year < 2999 && (n.month > 0 && n.month <= 12) && (n.day > 0 && n.day <= 31) && \
                                        n.week <= 7 && n.hour <= 23 && n.minute <= 59 && n.second <= 59)   
/**
 * \}
 */
   
/** 
 * \defgroup UTC_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup UTC_Exported_Functions
 * \{
 */
void bUTC2Struct( bUTC_DateTime_t *tm, bUTC_t utc );
bUTC_t bStruct2UTC( bUTC_DateTime_t tm);
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

