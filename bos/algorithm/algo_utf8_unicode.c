/**
 *!
 * \file        algo_utf8_unicode.c
 * \version     v0.0.1
 * \date        2020/06/25
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

/*Includes ----------------------------------------------*/
#include "algorithm/inc/algo_utf8_unicode.h"

#include <string.h>
/**
 * \addtogroup ALGORITHM
 * \{
 */

/**
 * \addtogroup UTF8_UNICODE
 * \{
 */

/**
 * \defgroup UTF8_UNICODE_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup UTF8_UNICODE_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup UTF8_UNICODE_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup UTF8_UNICODE_Private_Variables
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup UTF8_UNICODE_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup UTF8_UNICODE_Private_Functions
 * \{
 */

/**
 * \}
 */

/**
 * \addtogroup UTF8_UNICODE_Exported_Functions
 * \{
 */

int utf8_to_unicode_size(const uint8_t utf8)
{
    uint8_t c = utf8;
    // 0xxxxxxx 返回0
    // 10xxxxxx 不存在
    // 110xxxxx 返回2
    // 1110xxxx 返回3
    // 11110xxx 返回4
    // 111110xx 返回5
    // 1111110x 返回6
    if (c < 0x80)
        return 0;
    if (c >= 0x80 && c < 0xC0)
        return -1;
    if (c >= 0xC0 && c < 0xE0)
        return 2;
    if (c >= 0xE0 && c < 0xF0)
        return 3;
    if (c >= 0xF0 && c < 0xF8)
        return 4;
    if (c >= 0xF8 && c < 0xFC)
        return 5;
    if (c >= 0xFC)
        return 6;
    return -1;
}

int unicode_to_utf8(unsigned long unic, unsigned char *utf8, int utf8_size)
{
    if (utf8 == NULL || utf8_size < 6)
    {
        return -1;
    }

    if (unic <= 0x0000007F)
    {
        // * U-00000000 - U-0000007F:  0xxxxxxx
        *utf8 = (unic & 0x7F);
        return 1;
    }
    else if (unic >= 0x00000080 && unic <= 0x000007FF)
    {
        // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
        *(utf8 + 1) = (unic & 0x3F) | 0x80;
        *utf8       = ((unic >> 6) & 0x1F) | 0xC0;
        return 2;
    }
    else if (unic >= 0x00000800 && unic <= 0x0000FFFF)
    {
        // * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
        *(utf8 + 2) = (unic & 0x3F) | 0x80;
        *(utf8 + 1) = ((unic >> 6) & 0x3F) | 0x80;
        *utf8       = ((unic >> 12) & 0x0F) | 0xE0;
        return 3;
    }
    else if (unic >= 0x00010000 && unic <= 0x001FFFFF)
    {
        // * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(utf8 + 3) = (unic & 0x3F) | 0x80;
        *(utf8 + 2) = ((unic >> 6) & 0x3F) | 0x80;
        *(utf8 + 1) = ((unic >> 12) & 0x3F) | 0x80;
        *utf8       = ((unic >> 18) & 0x07) | 0xF0;
        return 4;
    }
    else if (unic >= 0x00200000 && unic <= 0x03FFFFFF)
    {
        // * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(utf8 + 4) = (unic & 0x3F) | 0x80;
        *(utf8 + 3) = ((unic >> 6) & 0x3F) | 0x80;
        *(utf8 + 2) = ((unic >> 12) & 0x3F) | 0x80;
        *(utf8 + 1) = ((unic >> 18) & 0x3F) | 0x80;
        *utf8       = ((unic >> 24) & 0x03) | 0xF8;
        return 5;
    }
    else if (unic >= 0x04000000 && unic <= 0x7FFFFFFF)
    {
        // * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(utf8 + 5) = (unic & 0x3F) | 0x80;
        *(utf8 + 4) = ((unic >> 6) & 0x3F) | 0x80;
        *(utf8 + 3) = ((unic >> 12) & 0x3F) | 0x80;
        *(utf8 + 2) = ((unic >> 18) & 0x3F) | 0x80;
        *(utf8 + 1) = ((unic >> 24) & 0x3F) | 0x80;
        *utf8       = ((unic >> 30) & 0x01) | 0xFC;
        return 6;
    }

    return 0;
}

int utf8_to_unicode(const uint8_t *utf8, uint32_t *unicode)
{
    if (utf8 == NULL || unicode == NULL)
    {
        return -1;
    }

    char b1, b2, b3, b4, b5, b6;

    *unicode          = 0x0;
    int      utfbytes = utf8_to_unicode_size(*utf8);
    uint8_t *ptmp     = (uint8_t *)unicode;

    switch (utfbytes)
    {
        case 0:
            *ptmp = *utf8;
            utfbytes += 1;
            break;
        case 2:
            b1 = *utf8;
            b2 = *(utf8 + 1);
            if ((b2 & 0xE0) != 0x80)
                return 0;
            *ptmp       = (b1 << 6) + (b2 & 0x3F);
            *(ptmp + 1) = (b1 >> 2) & 0x07;
            break;
        case 3:
            b1 = *utf8;
            b2 = *(utf8 + 1);
            b3 = *(utf8 + 2);
            if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80))
                return 0;
            *ptmp       = (b2 << 6) + (b3 & 0x3F);
            *(ptmp + 1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
            break;
        case 4:
            b1 = *utf8;
            b2 = *(utf8 + 1);
            b3 = *(utf8 + 2);
            b4 = *(utf8 + 3);
            if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) || ((b4 & 0xC0) != 0x80))
                return 0;
            *ptmp       = (b3 << 6) + (b4 & 0x3F);
            *(ptmp + 1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
            *(ptmp + 2) = ((b1 << 2) & 0x1C) + ((b2 >> 4) & 0x03);
            break;
        case 5:
            b1 = *utf8;
            b2 = *(utf8 + 1);
            b3 = *(utf8 + 2);
            b4 = *(utf8 + 3);
            b5 = *(utf8 + 4);
            if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) || ((b4 & 0xC0) != 0x80) ||
                ((b5 & 0xC0) != 0x80))
                return 0;
            *ptmp       = (b4 << 6) + (b5 & 0x3F);
            *(ptmp + 1) = (b3 << 4) + ((b4 >> 2) & 0x0F);
            *(ptmp + 2) = (b2 << 2) + ((b3 >> 4) & 0x03);
            *(ptmp + 3) = (b1 << 6);
            break;
        case 6:
            b1 = *utf8;
            b2 = *(utf8 + 1);
            b3 = *(utf8 + 2);
            b4 = *(utf8 + 3);
            b5 = *(utf8 + 4);
            b6 = *(utf8 + 5);
            if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) || ((b4 & 0xC0) != 0x80) ||
                ((b5 & 0xC0) != 0x80) || ((b6 & 0xC0) != 0x80))
                return 0;
            *ptmp       = (b5 << 6) + (b6 & 0x3F);
            *(ptmp + 1) = (b5 << 4) + ((b6 >> 2) & 0x0F);
            *(ptmp + 2) = (b3 << 2) + ((b4 >> 4) & 0x03);
            *(ptmp + 3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);
            break;
        default:
            return 0;
    }
    return utfbytes;
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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
