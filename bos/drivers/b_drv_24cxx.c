/**
 *!
 * \file        b_drv_24cxx.c
 * \version     v0.0.1
 * \date        2020/03/25
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
#include "drivers/inc/b_drv_24cxx.h"

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup 24CXX
 * \{
 */

/**
 * \defgroup 24CXX_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup 24CXX_Private_Defines
 * \{
 */
#define DRIVER_NAME 24CXX
/**
 * \}
 */

/**
 * \defgroup 24CXX_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup 24CXX_Private_Variables
 * \{
 */
bDRIVER_HALIF_TABLE(b24CXX_HalIf_t, DRIVER_NAME);
/**
 * \}
 */

/**
 * \defgroup 24CXX_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup 24CXX_Private_Functions
 * \{
 */

static int _b24CXXWrite(bDriverInterface_t *pdrv, uint32_t off, uint8_t *pbuf, uint32_t len)
{
    uint8_t  l_c = off % 8;
    uint16_t i   = 0;
    bDRIVER_GET_HALIF(_if, b24CXX_HalIf_t, pdrv);
    if (len <= l_c)
    {
        bHalI2CMemWrite(_if, off, pbuf, len);
    }
    else
    {
        bHalI2CMemWrite(_if, off, pbuf, l_c);
        bHalDelayMs(5);
        off += l_c;
        pbuf += l_c;
        len -= l_c;
        for (i = 0; i < len / 8; i++)
        {
            bHalI2CMemWrite(_if, off, pbuf, 8);
            bHalDelayMs(5);
            off += 8;
            pbuf += 8;
        }
        if ((len % 8) > 0)
        {
            bHalI2CMemWrite(_if, off, pbuf, (len % 8));
            bHalDelayMs(5);
        }
    }
    return len;
}

static int _b24CXXRead(bDriverInterface_t *pdrv, uint32_t off, uint8_t *pbuf, uint32_t len)
{
    bDRIVER_GET_HALIF(_if, b24CXX_HalIf_t, pdrv);
    bHalI2CMemRead(_if, off, pbuf, len);
    return len;
}

/**
 * \}
 */

/**
 * \addtogroup 24CXX_Exported_Functions
 * \{
 */
int b24CXX_Init(bDriverInterface_t *pdrv)
{
    bDRIVER_STRUCT_INIT(pdrv, DRIVER_NAME, b24CXX_Init);
    pdrv->read  = _b24CXXRead;
    pdrv->write = _b24CXXWrite;
    return 0;
}

bDRIVER_REG_INIT(B_DRIVER_24CXX, b24CXX_Init);

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
