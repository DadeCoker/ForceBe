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

const static b24CXX_HalIf_t b24CXX_HalIfTable[] = HAL_24CXX_IF;
b24CXX_Driver_t             b24CXX_Driver[sizeof(b24CXX_HalIfTable) / sizeof(b24CXX_HalIf_t)];
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

static int _b24CXXWrite(b24CXX_Driver_t *pdrv, uint32_t off, uint8_t *pbuf, uint16_t len)
{
    uint8_t  l_c = off % 8;
    uint16_t i   = 0;
    bDRV_GET_HALIF(_if, b24CXX_HalIf_t, pdrv);
    if (len <= l_c)
    {
        bHalI2C_MemWrite(_if->iic, _if->addr, off, pbuf, len);
    }
    else
    {
        bHalI2C_MemWrite(_if->iic, _if->addr, off, pbuf, l_c);
        bUtilDelayMS(5);
        off += l_c;
        pbuf += l_c;
        len -= l_c;
        for (i = 0; i < len / 8; i++)
        {
            bHalI2C_MemWrite(_if->iic, _if->addr, off, pbuf, 8);
            bUtilDelayMS(5);
            off += 8;
            pbuf += 8;
        }
        if ((len % 8) > 0)
        {
            bHalI2C_MemWrite(_if->iic, _if->addr, off, pbuf, (len % 8));
            bUtilDelayMS(5);
        }
    }
    return len;
}

static int _b24CXXRead(b24CXX_Driver_t *pdrv, uint32_t off, uint8_t *pbuf, uint16_t len)
{
    bDRV_GET_HALIF(_if, b24CXX_HalIf_t, pdrv);
    bHalI2C_MemRead(_if->iic, _if->addr, off, pbuf, len);
    return len;
}

/**
 * \}
 */

/**
 * \addtogroup 24CXX_Exported_Functions
 * \{
 */
int b24CXX_Init()
{
    uint8_t i = 0, num_drv = sizeof(b24CXX_HalIfTable) / sizeof(b24CXX_HalIf_t);
    for (i = 0; i < num_drv; i++)
    {
        b24CXX_Driver[i]._hal_if = (void *)&b24CXX_HalIfTable[i];
        b24CXX_Driver[i].status  = 0;
        b24CXX_Driver[i].close   = NULL;
        b24CXX_Driver[i].read    = _b24CXXRead;
        b24CXX_Driver[i].ctl     = NULL;
        b24CXX_Driver[i].open    = NULL;
        b24CXX_Driver[i].write   = _b24CXXWrite;
    }
    return 0;
}

bDRIVER_REG_INIT(b24CXX_Init);

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
