/**
 *!
 * \file        algo_hmac_sha1.c
 * \version     v0.0.1
 * \date        2020/04/27
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
#include "inc/algo_hmac_sha1.h"

#if _ALGO_SHA1_ENABLE

/**
 * \addtogroup ALGORITHM
 * \{
 */

/**
 * \addtogroup HMAC_SHA1
 * \{
 */

/**
 * \defgroup HMAC_SHA1_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup HMAC_SHA1_Private_Defines
 * \{
 */
#define MAX_MESSAGE_LENGTH 128
/**
 * \}
 */

/**
 * \defgroup HMAC_SHA1_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup HMAC_SHA1_Private_Variables
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup HMAC_SHA1_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup HMAC_SHA1_Private_Functions
 * \{
 */
uint32_t ft(int t, uint32_t x, uint32_t y, uint32_t z)
{
    uint32_t a = 0, b = 0, c = 0;

    if (t < 20)
    {
        a = x & y;
        b = (~x) & z;
        c = a ^ b;
    }
    else if (t < 40)
    {
        c = x ^ y ^ z;
    }
    else if (t < 60)
    {
        a = x & y;
        b = a ^ (x & z);
        c = b ^ (y & z);
    }
    else if (t < 80)
    {
        c = (x ^ y) ^ z;
    }

    return c;
}

uint32_t k(int t)
{
    uint32_t c = 0;

    if (t < 20)
    {
        c = 0x5a827999;
    }
    else if (t < 40)
    {
        c = 0x6ed9eba1;
    }
    else if (t < 60)
    {
        c = 0x8f1bbcdc;
    }
    else if (t < 80)
    {
        c = 0xca62c1d6;
    }

    return c;
}

uint32_t rotr(int bits, uint32_t a)
{
    uint32_t c, d, e, f, g;
    c = (0x0001 << bits) - 1;
    d = ~c;

    e = (a & d) >> bits;
    f = (a & c) << (32 - bits);

    g = e | f;

    return (g & 0xffffffff);
}

uint32_t rotl(int bits, uint32_t a)
{
    uint32_t c, d, e, f, g;
    c = (0x0001 << (32 - bits)) - 1;
    d = ~c;

    e = (a & c) << bits;
    f = (a & d) >> (32 - bits);

    g = e | f;

    return (g & 0xffffffff);
}

void sha1(uint8_t *message, int message_length, uint8_t *digest)
{
    int i;
    int num_blocks;
    int block_remainder;
    int padded_length;

    uint32_t l;
    uint32_t t;
    uint32_t h[5];
    uint32_t a, b, c, d, e;
    uint32_t w[80];
    uint32_t temp;

#ifdef SHA1_DEBUG
    int x, y;
#endif

    /* Calculate the number of 512 bit blocks */

    padded_length = message_length + 8; /* Add length for l */
    padded_length = padded_length + 1;  /* Add the 0x01 bit postfix */

    l = message_length * 8;

    num_blocks      = padded_length / 64;
    block_remainder = padded_length % 64;

    if (block_remainder > 0)
    {
        num_blocks++;
    }

    padded_length = padded_length + (64 - block_remainder);

    /* clear the padding field */
    for (i = message_length; i < (num_blocks * 64); i++)
    {
        message[i] = 0x00;
    }

    /* insert b1 padding bit */
    message[message_length] = 0x80;

    /* Insert l */
    message[(num_blocks * 64) - 1] = (uint8_t)(l & 0xff);
    message[(num_blocks * 64) - 2] = (uint8_t)((l >> 8) & 0xff);
    message[(num_blocks * 64) - 3] = (uint8_t)((l >> 16) & 0xff);
    message[(num_blocks * 64) - 4] = (uint8_t)((l >> 24) & 0xff);

    /* Set initial hash state */
    h[0] = 0x67452301;
    h[1] = 0xefcdab89;
    h[2] = 0x98badcfe;
    h[3] = 0x10325476;
    h[4] = 0xc3d2e1f0;

#ifdef SHA1_DEBUG
    b_log("INITIAL message_length = %d\n", message_length);
    b_log("INITIAL padded_length = %d\n", padded_length);
    b_log("INITIAL num_blocks = %d\n", num_blocks);

    for (x = 0; x < num_blocks; x++)
    {
        b_log("\t\t");
        for (y = 0; y < 16; y++)
        {
            b_log("%02x ", message[y + (x * 64)]);
        }
        b_log("\n");
        b_log("\t\t");
        for (y = 0; y < 16; y++)
        {
            b_log("%02x ", message[16 + y + (x * 64)]);
        }
        b_log("\n");
        b_log("\t\t");
        for (y = 0; y < 16; y++)
        {
            b_log("%02x ", message[32 + y + (x * 64)]);
        }
        b_log("\n");
        b_log("\t\t");
        for (y = 0; y < 16; y++)
        {
            b_log("%02x ", message[48 + y + (x * 64)]);
        }
        b_log("\n");
    }

#endif

    for (i = 0; i < num_blocks; i++)
    {
        /* Prepare the message schedule */
        for (t = 0; t < 80; t++)
        {
            if (t < 16)
            {
                w[t] = (256 * 256 * 256) * message[(i * 64) + (t * 4)];
                w[t] += (256 * 256) * message[(i * 64) + (t * 4) + 1];
                w[t] += (256) * message[(i * 64) + (t * 4) + 2];
                w[t] += message[(i * 64) + (t * 4) + 3];
            }
            else if (t < 80)
            {
                w[t] = rotl(1, (w[t - 3] ^ w[t - 8] ^ w[t - 14] ^ w[t - 16]));
            }
        }

#ifdef SHA1_DEBUG
        b_log("\tW(0) = %08lX \t W(9)  = %08lX \n", w[0], w[8]);
        b_log("\tW(1) = %08lX \t W(10) = %08lX \n", w[1], w[9]);
        b_log("\tW(2) = %08lX \t W(11) = %08lX \n", w[2], w[10]);
        b_log("\tW(3) = %08lX \t W(12) = %08lX \n", w[3], w[11]);
        b_log("\tW(4) = %08lX \t W(13) = %08lX \n", w[4], w[12]);
        b_log("\tW(5) = %08lX \t W(14) = %08lX \n", w[5], w[13]);
        b_log("\tW(6) = %08lX \t W(15) = %08lX \n", w[6], w[14]);
        b_log("\tW(7) = %08lX \t W(16) = %08lX \n\n", w[7], w[15]);

#endif
        /* Initialize the five working variables */
        a = h[0];
        b = h[1];
        c = h[2];
        d = h[3];
        e = h[4];

        /* iterate a-e 80 times */

        for (t = 0; t < 80; t++)
        {
            temp = (rotl(5, a) + ft(t, b, c, d)) & 0xffffffff;
            temp = (temp + e) & 0xffffffff;
            temp = (temp + k(t)) & 0xffffffff;
            temp = (temp + w[t]) & 0xffffffff;
            e    = d;
            d    = c;
            c    = rotl(30, b);
            b    = a;
            a    = temp;
#ifdef SHA1_DEBUG
            b_log("t = %2ld\t %08lx, %08lx, %08lx, %08lx, %08lx\n", t, a, b, c, d, e);
#endif
        }

        /* compute the ith intermediate hash value */
#ifdef SHA1_DEBUG
        b_log("  +   \t %08lx, %08lx, %08lx, %08lx, %08lx\n", h[0], h[1], h[2], h[3], h[4]);
#endif
        h[0] = (a + h[0]) & 0xffffffff;
        h[1] = (b + h[1]) & 0xffffffff;
        h[2] = (c + h[2]) & 0xffffffff;
        h[3] = (d + h[3]) & 0xffffffff;
        h[4] = (e + h[4]) & 0xffffffff;

#ifdef SHA1_DEBUG
        b_log("  =   \t %08lx, %08lx, %08lx, %08lx, %08lx\n", h[0], h[1], h[2], h[3], h[4]);
#endif
    }

    digest[3] = (uint8_t)(h[0] & 0xff);
    digest[2] = (uint8_t)((h[0] >> 8) & 0xff);
    digest[1] = (uint8_t)((h[0] >> 16) & 0xff);
    digest[0] = (uint8_t)((h[0] >> 24) & 0xff);

    digest[7] = (uint8_t)(h[1] & 0xff);
    digest[6] = (uint8_t)((h[1] >> 8) & 0xff);
    digest[5] = (uint8_t)((h[1] >> 16) & 0xff);
    digest[4] = (uint8_t)((h[1] >> 24) & 0xff);

    digest[11] = (uint8_t)(h[2] & 0xff);
    digest[10] = (uint8_t)((h[2] >> 8) & 0xff);
    digest[9]  = (uint8_t)((h[2] >> 16) & 0xff);
    digest[8]  = (uint8_t)((h[2] >> 24) & 0xff);

    digest[15] = (uint8_t)(h[3] & 0xff);
    digest[14] = (uint8_t)((h[3] >> 8) & 0xff);
    digest[13] = (uint8_t)((h[3] >> 16) & 0xff);
    digest[12] = (uint8_t)((h[3] >> 24) & 0xff);

    digest[19] = (uint8_t)(h[4] & 0xff);
    digest[18] = (uint8_t)((h[4] >> 8) & 0xff);
    digest[17] = (uint8_t)((h[4] >> 16) & 0xff);
    digest[16] = (uint8_t)((h[4] >> 24) & 0xff);
}
/**
 * \}
 */

/**
 * \addtogroup HMAC_SHA1_Exported_Functions
 * \{
 */

/******************************************************/
/* hmac-sha1()                                        */
/* Performs the hmac-sha1 keyed secure hash algorithm */
/******************************************************/

void hmac_sha1(uint8_t *key, int key_length, uint8_t *data, int data_length, uint8_t *digest)

{
    uint8_t k0[64];
    uint8_t k0xorIpad[64];
    uint8_t step7data[64];
    uint8_t step5data[MAX_MESSAGE_LENGTH + 128];
    uint8_t step8data[64 + 20];
    int     b    = 64; /* blocksize */
    uint8_t ipad = 0x36;

    uint8_t opad = 0x5c;
    int     i;

    for (i = 0; i < 64; i++)
    {
        k0[i] = 0x00;
    }

    if (key_length != b) /* Step 1 */
    {
        /* Step 2 */
        if (key_length > b)
        {
            sha1(key, key_length, digest);
            for (i = 0; i < 20; i++)
            {
                k0[i] = digest[i];
            }
        }
        else if (key_length < b) /* Step 3 */
        {
            for (i = 0; i < key_length; i++)
            {
                k0[i] = key[i];
            }
        }
    }
    else
    {
        for (i = 0; i < b; i++)
        {
            k0[i] = key[i];
        }
    }

    /* Step 4 */
    for (i = 0; i < 64; i++)
    {
        k0xorIpad[i] = k0[i] ^ ipad;
    }

    /* Step 5 */
    for (i = 0; i < 64; i++)
    {
        step5data[i] = k0xorIpad[i];
    }
    for (i = 0; i < data_length; i++)
    {
        step5data[i + 64] = data[i];
    }

    /* Step 6 */
    sha1(step5data, data_length + b, digest);

    /* Step 7 */
    for (i = 0; i < 64; i++)
    {
        step7data[i] = k0[i] ^ opad;
    }

    /* Step 8 */
    for (i = 0; i < 64; i++)
    {
        step8data[i] = step7data[i];
    }
    for (i = 0; i < 20; i++)
    {
        step8data[i + 64] = digest[i];
    }

    /* Step 9 */
    sha1(step8data, b + 20, digest);
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
