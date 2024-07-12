/**
 *!
 * \file       butc.h
 * \brief      UTC2000
 * \version    v0.0.1
 * \date       2019/06/05
 * \author     notrynohigh
 * Last modified by notrynohigh 2019/06/05
 * Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
#ifndef __BUTC_H__
#define __BUTC_H__
/*Includes ----------------------------------------------*/
#include <stdint.h>   

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_UTC
 * \{
 */

/** 
 * \defgroup BUTC_Exported_TypesDefinitions
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
 * \defgroup BUTC_Exported_Defines
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
 * \defgroup BUTC_Exported_Macros
 * \{
 */
#define IS_TIME_VALID(n)        (n.year < 2999 && (n.month > 0 && n.month <= 12) && (n.day > 0 && n.day <= 31) && \
                                        n.week <= 7 && n.hour <= 23 && n.minute <= 59 && n.second <= 59)   
/**
 * \}
 */
   
/** 
 * \defgroup BUTC_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BUTC_Exported_Functions
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
 
#endif  
/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/

