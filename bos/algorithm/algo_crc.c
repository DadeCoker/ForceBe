/**
 *!
 * \file        algo_crc.c
 * \version     v0.0.1
 * \date        2020/05/13
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
#include "inc/algo_crc.h"

#if _ALGO_CRC_ENABLE

/**
 * \addtogroup ALGORITHM
 * \{
 */

/**
 * \addtogroup CRC
 * \{
 */

/**
 * \defgroup CRC_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup CRC_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup CRC_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup CRC_Private_Variables
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup CRC_Private_Functions
 * \{
 */

/*
 * Name:    CRC-8               x8+x2+x+1
 *          Initial value 0x00
 */
uint8_t crc8_d(uint8_t crc_8, uint8_t *data, uint32_t length, uint8_t flag)
{
    uint8_t i;
    uint8_t crc = crc_8;  // Initial value
    (void)flag;
    while (length--)
    {
        crc ^= *data++;  // crc ^= *data; data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x07;
            else
                crc <<= 1;
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-8/ITU           x8+x2+x+1
 *          Initial value 0x00
 *****************************************************************************/
uint8_t crc8_itu(uint8_t crc_8, uint8_t *data, uint32_t length, uint8_t flag)
{
    uint8_t i;
    uint8_t crc = crc_8;  // Initial value
    if (flag)
    {
        crc = crc ^ 0x55;
    }
    while (length--)
    {
        crc ^= *data++;  // crc ^= *data; data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x07;
            else
                crc <<= 1;
        }
    }
    return crc ^ 0x55;
}

/*
 * Name:    CRC-8/ROHC          x8+x2+x+1
 *          Initial value 0xff
 */
uint8_t crc8_rohc(uint8_t crc_8, uint8_t *data, uint32_t length, uint8_t flag)
{
    uint8_t i;
    uint8_t crc = crc_8;  // Initial value
    (void)flag;
    while (length--)
    {
        crc ^= *data++;  // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xE0;  // 0xE0 = reverse 0x07
            else
                crc = (crc >> 1);
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-8/MAXIM         x8+x5+x4+1
 *          Initial value 0x00
 *****************************************************************************/

uint8_t crc8_maxim(uint8_t crc_8, uint8_t *data, uint32_t length, uint8_t flag)
{
    uint8_t i;
    uint8_t crc = crc_8;  // Initial value
    (void)flag;
    while (length--)
    {
        crc ^= *data++;  // crc ^= *data; data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0x8C;  // 0x8C = reverse 0x31
            else
                crc >>= 1;
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-16/IBM          x16+x15+x2+1
 *          Initial value 0x00
 ****************************************************************************/
uint16_t crc16_ibm(uint16_t crc_16, uint8_t *data, uint32_t length, uint8_t flag)
{
    uint8_t  i;
    uint16_t crc = crc_16;  // Initial value
    (void)flag;
    while (length--)
    {
        crc ^= *data++;  // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;  // 0xA001 = reverse 0x8005
            else
                crc = (crc >> 1);
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-16/MAXIM          x16+x15+x2+1
 *          Initial value 0x00
 *****************************************************************************/
uint16_t crc16_maxim(uint16_t crc_16, uint8_t *data, uint32_t length, uint8_t flag)
{
    uint8_t  i;
    uint16_t crc = crc_16;  // Initial value
    if (flag)
    {
        crc = ~crc;
    }
    while (length--)
    {
        crc ^= *data++;  // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;  // 0xA001 = reverse 0x8005
            else
                crc = (crc >> 1);
        }
    }
    return ~crc;  // crc^0xffff
}

/******************************************************************************
 * Name:    CRC-16/USB          x16+x15+x2+1
 *          Initial value 0xffff
 *****************************************************************************/
uint16_t crc16_usb(uint16_t crc_16, uint8_t *data, uint32_t length, uint8_t flag)
{
    uint8_t  i;
    uint16_t crc = crc_16;  // Initial value
    if (flag)
    {
        crc = ~crc;
    }
    while (length--)
    {
        crc ^= *data++;  // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;  // 0xA001 = reverse 0x8005
            else
                crc = (crc >> 1);
        }
    }
    return ~crc;  // crc^0xffff
}

/******************************************************************************
 * Name:    CRC-16/MODBUS       x16+x15+x2+1
 *          Initial value 0xffff
 *****************************************************************************/
uint16_t crc16_modbus(uint16_t crc_16, uint8_t *data, uint32_t length, uint8_t flag)
{
    uint8_t  i;
    uint16_t crc = crc_16;  // Initial value
    (void)flag;
    while (length--)
    {
        crc ^= *data++;  // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;  // 0xA001 = reverse 0x8005
            else
                crc = (crc >> 1);
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-16/CCITT        x16+x12+x5+1
 *          Initial value 0x00
 *****************************************************************************/
uint16_t crc16_ccitt(uint16_t crc_16, uint8_t *data, uint32_t length, uint8_t flag)
{
    uint8_t  i;
    uint16_t crc = crc_16;  // Initial value
    (void)flag;
    while (length--)
    {
        crc ^= *data++;  // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0x8408;  // 0x8408 = reverse 0x1021
            else
                crc = (crc >> 1);
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-16/CCITT-FALSE   x16+x12+x5+1
 *          Initial value 0xffff
 *****************************************************************************/
uint16_t crc16_ccitt_false(uint16_t crc_16, uint8_t *data, uint32_t length, uint8_t flag)
{
    uint8_t  i;
    uint16_t crc = crc_16;  // Initial value
    (void)flag;
    while (length--)
    {
        crc ^= (uint16_t)(*data++) << 8;  // crc ^= (uint6_t)(*data)<<8; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-16/X25          x16+x12+x5+1
 *          Initial value 0xffff
 *****************************************************************************/
uint16_t crc16_x25(uint16_t crc_16, uint8_t *data, uint32_t length, uint8_t flag)
{
    uint8_t  i;
    uint16_t crc = crc_16;  // Initial value
    if (flag)
    {
        crc = ~crc;
    }
    while (length--)
    {
        crc ^= *data++;  // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0x8408;  // 0x8408 = reverse 0x1021
            else
                crc = (crc >> 1);
        }
    }
    return ~crc;  // crc^Xorout
}

/******************************************************************************
 * Name:    CRC-16/XMODEM       x16+x12+x5+1
 *          Initial value 0x00
 *****************************************************************************/
uint16_t crc16_xmodem(uint16_t crc_16, uint8_t *data, uint32_t length, uint8_t flag)
{
    uint8_t  i;
    uint16_t crc = crc_16;  // Initial value
    (void)flag;
    while (length--)
    {
        crc ^= (uint16_t)(*data++) << 8;  // crc ^= (uint16_t)(*data)<<8; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-16/DNP          x16+x13+x12+x11+x10+x8+x6+x5+x2+1
 *          Initial value 0x00
 *****************************************************************************/
uint16_t crc16_dnp(uint16_t crc_16, uint8_t *data, uint32_t length, uint8_t flag)
{
    uint8_t  i;
    uint16_t crc = crc_16;  // Initial value
    if (flag)
    {
        crc = ~crc;
    }
    while (length--)
    {
        crc ^= *data++;  // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA6BC;  // 0xA6BC = reverse 0x3D65
            else
                crc = (crc >> 1);
        }
    }
    return ~crc;  // crc^Xorout
}

/******************************************************************************
 * Name:    CRC-32  x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
 *          Initial value 0xffffffff
 *****************************************************************************/
uint32_t crc32_d(uint32_t crc_32, uint8_t *data, uint32_t length, uint8_t flag)
{
    uint8_t  i;
    uint32_t crc = crc_32;  // Initial value
    if (flag)
    {
        crc = ~crc;
    }
    while (length--)
    {
        crc ^= *data++;  // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;  // 0xEDB88320= reverse 0x04C11DB7
            else
                crc = (crc >> 1);
        }
    }
    return ~crc;
}

/******************************************************************************
 * Name:    CRC-32/MPEG-2  x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
 *          Initial value 0xffffffff
 *****************************************************************************/
uint32_t crc32_mpeg_2(uint32_t crc_32, uint8_t *data, uint32_t length, uint8_t flag)
{
    uint8_t  i;
    uint32_t crc = crc_32;  // Initial value
    (void)flag;
    while (length--)
    {
        crc ^= (uint32_t)(*data++) << 24;  // crc ^=(uint32_t)(*data)<<24; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 0x80000000)
                crc = (crc << 1) ^ 0x04C11DB7;
            else
                crc <<= 1;
        }
    }
    return crc;
}

/**
 * \}
 */

/**
 * \addtogroup CRC_Exported_Functions
 * \{
 */

/**
#define ALGO_CRC8 (0)               // Initial value 0x00
#define ALGO_CRC8_ITU (1)           // Initial value 0x00
#define ALGO_CRC8_ROHC (2)          // Initial value 0xff
#define ALGO_CRC8_MAXIM (3)         // Initial value 0x00
#define ALGO_CRC16_IBM (4)          // Initial value 0x00
#define ALGO_CRC16_MAXIM (5)        // Initial value 0x00
#define ALGO_CRC16_USB (6)          // Initial value 0xffff
#define ALGO_CRC16_MODBUS (7)       // Initial value 0xffff
#define ALGO_CRC16_CCITT (8)        // Initial value 0x00
#define ALGO_CRC16_CCITT_FALSE (9)  // Initial value 0xffff
#define ALGO_CRC16_X25 (10)         // Initial value 0xffff
#define ALGO_CRC16_XMODEM (11)      // Initial value 0x00
#define ALGO_CRC16_DNP (12)         // Initial value 0x00
#define ALGO_CRC32 (13)             // Initial value 0xffffffff
#define ALGO_CRC32_MPEG2 (14)       // Initial value 0xffffffff
 */

uint32_t crc_calculate(uint8_t type, uint8_t *pbuf, uint32_t len)
{
    uint32_t retval  = 0;
    uint32_t crc_val = 0;
    if (pbuf == NULL || len == 0)
    {
        return 0;
    }
    if (CRC_INITIAL_VALUE_IS_FF(type))
    {
        crc_val = 0xffffffff;
    }
    switch (type)
    {
        case ALGO_CRC8:
        {
            retval = crc8_d(crc_val & 0xff, pbuf, len, 0);
        }
        break;
        case ALGO_CRC8_ITU:
        {
            retval = crc8_itu(crc_val & 0xff, pbuf, len, 0);
        }
        break;
        case ALGO_CRC8_ROHC:
        {
            retval = crc8_rohc(crc_val & 0xff, pbuf, len, 0);
        }
        break;
        case ALGO_CRC8_MAXIM:
        {
            retval = crc8_maxim(crc_val & 0xff, pbuf, len, 0);
        }
        break;
        case ALGO_CRC16_IBM:
        {
            retval = crc16_ibm(crc_val & 0xffff, pbuf, len, 0);
        }
        break;
        case ALGO_CRC16_MAXIM:
        {
            retval = crc16_maxim(crc_val & 0xffff, pbuf, len, 0);
        }
        break;
        case ALGO_CRC16_USB:
        {
            retval = crc16_usb(crc_val & 0xffff, pbuf, len, 0);
        }
        break;
        case ALGO_CRC16_MODBUS:
        {
            retval = crc16_modbus(crc_val & 0xffff, pbuf, len, 0);
        }
        break;
        case ALGO_CRC16_CCITT:
        {
            retval = crc16_ccitt(crc_val & 0xffff, pbuf, len, 0);
        }
        break;
        case ALGO_CRC16_CCITT_FALSE:
        {
            retval = crc16_ccitt_false(crc_val & 0xffff, pbuf, len, 0);
        }
        break;
        case ALGO_CRC16_X25:
        {
            retval = crc16_x25(crc_val & 0xffff, pbuf, len, 0);
        }
        break;
        case ALGO_CRC16_XMODEM:
        {
            retval = crc16_xmodem(crc_val & 0xffff, pbuf, len, 0);
        }
        break;
        case ALGO_CRC16_DNP:
        {
            retval = crc16_dnp(crc_val & 0xffff, pbuf, len, 0);
        }
        break;
        case ALGO_CRC32:
        {
            retval = crc32_d(crc_val, pbuf, len, 0);
        }
        break;
        case ALGO_CRC32_MPEG2:
        {
            retval = crc32_mpeg_2(crc_val, pbuf, len, 0);
        }
        break;
        default:
            break;
    }
    return retval;
}

void crc_calculate_sbs(algo_crc_sbs_t *phandle, uint8_t *pbuf, uint32_t len)
{
    uint32_t crc_val = 0, retval = 0;
    uint8_t  flag = 0;
    if (phandle == NULL || pbuf == NULL || len == 0)
    {
        return;
    }
    flag = (phandle->flag != 0);
    if (flag)
    {
        crc_val = phandle->crc;
    }
    else
    {
        if (CRC_INITIAL_VALUE_IS_FF(phandle->type))
        {
            crc_val = 0xffffffff;
        }
        phandle->flag = 1;
    }

    switch (phandle->type)
    {
        case ALGO_CRC8:
        {
            retval = crc8_d(crc_val & 0xff, pbuf, len, flag);
        }
        break;
        case ALGO_CRC8_ITU:
        {
            retval = crc8_itu(crc_val & 0xff, pbuf, len, flag);
        }
        break;
        case ALGO_CRC8_ROHC:
        {
            retval = crc8_rohc(crc_val & 0xff, pbuf, len, flag);
        }
        break;
        case ALGO_CRC8_MAXIM:
        {
            retval = crc8_maxim(crc_val & 0xff, pbuf, len, flag);
        }
        break;
        case ALGO_CRC16_IBM:
        {
            retval = crc16_ibm(crc_val & 0xffff, pbuf, len, flag);
        }
        break;
        case ALGO_CRC16_MAXIM:
        {
            retval = crc16_maxim(crc_val & 0xffff, pbuf, len, flag);
        }
        break;
        case ALGO_CRC16_USB:
        {
            retval = crc16_usb(crc_val & 0xffff, pbuf, len, flag);
        }
        break;
        case ALGO_CRC16_MODBUS:
        {
            retval = crc16_modbus(crc_val & 0xffff, pbuf, len, flag);
        }
        break;
        case ALGO_CRC16_CCITT:
        {
            retval = crc16_ccitt(crc_val & 0xffff, pbuf, len, flag);
        }
        break;
        case ALGO_CRC16_CCITT_FALSE:
        {
            retval = crc16_ccitt_false(crc_val & 0xffff, pbuf, len, flag);
        }
        break;
        case ALGO_CRC16_X25:
        {
            retval = crc16_x25(crc_val & 0xffff, pbuf, len, flag);
        }
        break;
        case ALGO_CRC16_XMODEM:
        {
            retval = crc16_xmodem(crc_val & 0xffff, pbuf, len, flag);
        }
        break;
        case ALGO_CRC16_DNP:
        {
            retval = crc16_dnp(crc_val & 0xffff, pbuf, len, flag);
        }
        break;
        case ALGO_CRC32:
        {
            retval = crc32_d(crc_val, pbuf, len, flag);
        }
        break;
        case ALGO_CRC32_MPEG2:
        {
            retval = crc32_mpeg_2(crc_val, pbuf, len, flag);
        }
        break;
        default:
        {
            retval = crc_val;
        }
        break;
    }
    phandle->crc = retval;
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

#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
