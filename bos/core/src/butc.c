/**
 *!
 * \file       butc.c
 * \brief      UTC2000
 * \version    v0.0.1
 * \date       2019/06/05
 * \author     notrynohigh
 *Last modified by notrynohigh 2019/06/05
 *Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "butc.h"   

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_UTC
 * \brief UTC2000转换单元
 * \{
 */

/** 
 * \defgroup BUTC_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BUTC_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BUTC_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BUTC_Private_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BUTC_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BUTC_Private_Functions
 * \{
 */
static uint8_t _bUTC_CalendarMonthDays( uint8_t lpyr, uint8_t month)
{
    uint8_t days = 31, i = 0;
    uint8_t table[7] = {1,3,5,7,8,10,12};
    if(month < 1 || month > 12)
    {
        return 0;
    }
    if (month == 2)
    {
        days = (28 + lpyr);
    }
    else
    {
        for(i = 0;i < 7;i++)
        {
            if(month == table[i])
            {
                days = 31;
                break;
            }
        }
        if(i >= 7)
        {
            days = 30;
        }
    }
    return days ;
}
 
/**
 * \}
 */
   
/** 
 * \addtogroup BUTC_Exported_Functions
 * \{
 */
/**
 * \brief UTC转时间 \ref bUTC_DateTime_t
 */
void bUTC2Struct( bUTC_DateTime_t *tm, bUTC_t utc )
{
    uint32_t sec_in_day = utc % DAY;
    tm->second = sec_in_day % 60;
    tm->minute = (sec_in_day % 3600) / 60;
    tm->hour = sec_in_day / 3600;

    uint32_t days = utc / DAY;
    
    tm->week = (((days % 7) + 5) % 7) + 1;
    
    tm->year = BEGYEAR;
    while ( days >= YEAR_DAYS( tm->year ) )
    {
      days -= YEAR_DAYS( tm->year );
      tm->year++;
    }
    tm->month = 1;
    while ( days >= _bUTC_CalendarMonthDays( YEAR_CHECK( tm->year ), tm->month ))
    {
      days -= _bUTC_CalendarMonthDays( YEAR_CHECK( tm->year ), tm->month );
      tm->month++;
    }
    tm->day = days + 1;
}

/**
 * \brief 时间转UTC
 */
bUTC_t bStruct2UTC( bUTC_DateTime_t tm)
{
    uint32_t seconds;
	if(!IS_TIME_VALID(tm))
	{
		return 0;
	}
    seconds = (((tm.hour * 60) + tm.minute) * 60) + tm.second;

    uint32_t days = tm.day - 1;
    int8_t month = tm.month;
    month--;
    while ( month > 0 )
    {
        days += _bUTC_CalendarMonthDays( YEAR_CHECK( tm.year ), month );
        month--;
    }

    uint32_t year = (tm.year > 0) ? (tm.year - 1) : 0;
    while ( year >= BEGYEAR )
    {
        days += YEAR_DAYS( year );
        year--;
    }
    
    seconds += (days * DAY);

    return seconds;
}   
/**
 * \}
 */

/**
 * \}
 */

/**
 * \}
 */
  
/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/

