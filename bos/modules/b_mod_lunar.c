/**
 *!
 * \file        b_mod_lunar.c
 * \version     v0.0.1
 * \date        2019/12/31
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

/*Includes ----------------------------------------------*/
#include "modules/inc/b_mod_lunar.h"
#if _LUNAR_ENABLE
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup LUNAR
 * \{
 */

/**
 * \defgroup LUNAR_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup LUNAR_Private_Defines
 * \{
 */
#ifndef NULL
#define NULL ((void *)0)
#endif
/**
 * \}
 */

/**
 * \defgroup LUNAR_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup LUNAR_Private_Variables
 * \{
 */
const static uint32_t bLunarCalendarTable[199] = {
    /*1901-1910*/
    0x04AE53, 0x0A5748, 0x5526BD, 0x0D2650, 0x0D9544, 0x46AAB9, 0x056A4D, 0x09AD42, 0x24AEB6,
    0x04AE4A,
    /*1911-1920*/
    0x6A4DBE, 0x0A4D52, 0x0D2546, 0x5D52BA, 0x0B544E, 0x0D6A43, 0x296D37, 0x095B4B, 0x749BC1,
    0x049754,
    /*1921-1930*/
    0x0A4B48, 0x5B25BC, 0x06A550, 0x06D445, 0x4ADAB8, 0x02B64D, 0x095742, 0x2497B7, 0x04974A,
    0x664B3E,
    /*1931-1940*/
    0x0D4A51, 0x0EA546, 0x56D4BA, 0x05AD4E, 0x02B644, 0x393738, 0x092E4B, 0x7C96BF, 0x0C9553,
    0x0D4A48,
    /*1941-1950*/
    0x6DA53B, 0x0B554F, 0x056A45, 0x4AADB9, 0x025D4D, 0x092D42, 0x2C95B6, 0x0A954A, 0x7B4ABD,
    0x06CA51,
    /*1951-1960*/
    0x0B5546, 0x555ABB, 0x04DA4E, 0x0A5B43, 0x352BB8, 0x052B4C, 0x8A953F, 0x0E9552, 0x06AA48,
    0x6AD53C,
    /*1961-1970*/
    0x0AB54F, 0x04B645, 0x4A5739, 0x0A574D, 0x052642, 0x3E9335, 0x0D9549, 0x75AABE, 0x056A51,
    0x096D46,
    /*1971-1980*/
    0x54AEBB, 0x04AD4F, 0x0A4D43, 0x4D26B7, 0x0D254B, 0x8D52BF, 0x0B5452, 0x0B6A47, 0x696D3C,
    0x095B50,
    /*1981-1990*/
    0x049B45, 0x4A4BB9, 0x0A4B4D, 0xAB25C2, 0x06A554, 0x06D449, 0x6ADA3D, 0x0AB651, 0x093746,
    0x5497BB,
    /*1991-2000*/
    0x04974F, 0x064B44, 0x36A537, 0x0EA54A, 0x86B2BF, 0x05AC53, 0x0AB647, 0x5936BC, 0x092E50,
    0x0C9645,
    /*2001-2010*/
    0x4D4AB8, 0x0D4A4C, 0x0DA541, 0x25AAB6, 0x056A49, 0x7AADBD, 0x025D52, 0x092D47, 0x5C95BA,
    0x0A954E,
    /*2011-2020*/
    0x0B4A43, 0x4B5537, 0x0AD54A, 0x955ABF, 0x04BA53, 0x0A5B48, 0x652BBC, 0x052B50, 0x0A9345,
    0x474AB9,
    /*2021-2030*/
    0x06AA4C, 0x0AD541, 0x24DAB6, 0x04B64A, 0x69573D, 0x0A4E51, 0x0D2646, 0x5E933A, 0x0D534D,
    0x05AA43,
    /*2031-2040*/
    0x36B537, 0x096D4B, 0xB4AEBF, 0x04AD53, 0x0A4D48, 0x6D25BC, 0x0D254F, 0x0D5244, 0x5DAA38,
    0x0B5A4C,
    /*2041-2050*/
    0x056D41, 0x24ADB6, 0x049B4A, 0x7A4BBE, 0x0A4B51, 0x0AA546, 0x5B52BA, 0x06D24E, 0x0ADA42,
    0x355B37,
    /*2051-2060*/
    0x09374B, 0x8497C1, 0x049753, 0x064B48, 0x66A53C, 0x0EA54F, 0x06B244, 0x4AB638, 0x0AAE4C,
    0x092E42,
    /*2061-2070*/
    0x3C9735, 0x0C9649, 0x7D4ABD, 0x0D4A51, 0x0DA545, 0x55AABA, 0x056A4E, 0x0A6D43, 0x452EB7,
    0x052D4B,
    /*2071-2080*/
    0x8A95BF, 0x0A9553, 0x0B4A47, 0x6B553B, 0x0AD54F, 0x055A45, 0x4A5D38, 0x0A5B4C, 0x052B42,
    0x3A93B6,
    /*2081-2090*/
    0x069349, 0x7729BD, 0x06AA51, 0x0AD546, 0x54DABA, 0x04B64E, 0x0A5743, 0x452738, 0x0D264A,
    0x8E933E,
    /*2091-2099*/
    0x0D5252, 0x0DAA47, 0x66B53B, 0x056D4F, 0x04AE45, 0x4A4EB9, 0x0A4D4C, 0x0D1541, 0x2D92B5};
const static uint32_t bLunarMonthSum[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

/**
 * \}
 */

/**
 * \defgroup LUNAR_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup LUNAR_Private_Functions
 * \{
 */

/**
 * \}
 */

/**
 * \addtogroup LUNAR_Exported_Functions
 * \{
 */

int bSolar2Lunar(uint16_t syear, uint8_t smonth, uint8_t sday, bLunarInfo_t *plunar)
{
    int l_year, s_year, days_m;
    int index, flag;

    if (plunar == NULL)
    {
        return -1;
    }

    if (((bLunarCalendarTable[syear - 1901] & 0x0060) >> 5) == 1)
    {
        l_year = (bLunarCalendarTable[syear - 1901] & 0x001F) - 1;
    }
    else
    {
        l_year = (bLunarCalendarTable[syear - 1901] & 0x001F) - 1 + 31;
    }
    s_year = bLunarMonthSum[smonth - 1] + sday - 1;

    if ((!(syear % 4)) && (smonth > 2))
    {
        s_year++;
    }

    if (s_year >= l_year)
    {
        s_year -= l_year;
        smonth = 1;
        index  = 1;
        flag   = 0;
        if ((bLunarCalendarTable[syear - 1901] & (0x80000 >> (index - 1))) == 0)
        {
            days_m = 29;
        }
        else
        {
            days_m = 30;
        }
        while (s_year >= days_m)
        {
            s_year -= days_m;
            index++;
            if (smonth == ((bLunarCalendarTable[syear - 1901] & 0xF00000) >> 20))
            {
                flag = ~flag;
                if (flag == 0)
                {
                    smonth++;
                }
            }
            else
            {
                smonth++;
            }
            if ((bLunarCalendarTable[syear - 1901] & (0x80000 >> (index - 1))) == 0)
            {
                days_m = 29;
            }
            else
            {
                days_m = 30;
            }
        }
        sday = s_year + 1;
    }
    else
    {
        l_year -= s_year;
        syear--;
        smonth = 12;
        if (((bLunarCalendarTable[syear - 1901] & 0xF00000) >> 20) == 0)
        {
            index = 12;
        }
        else
        {
            index = 13;
        }
        flag = 0;
        if ((bLunarCalendarTable[syear - 1901] & (0x80000 >> (index - 1))) == 0)
        {
            days_m = 29;
        }
        else
        {
            days_m = 30;
        }
        while (l_year > days_m)
        {
            l_year -= days_m;
            index--;
            if (flag == 0)
            {
                smonth--;
            }
            if (smonth == ((bLunarCalendarTable[syear - 1901] & 0xF00000) >> 20))
            {
                flag = ~flag;
            }
            if ((bLunarCalendarTable[syear - 1901] & (0x80000 >> (index - 1))) == 0)
            {
                days_m = 29;
            }
            else
            {
                days_m = 30;
            }
        }
        sday = days_m - l_year + 1;
    }
    plunar->day   = sday;
    plunar->month = smonth;
    plunar->year  = syear;
    return 0;
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

/**
 * \}
 */
#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
