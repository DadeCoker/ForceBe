/**
 *!
 * \file        b_drv_xpt2046.c
 * \version     v0.0.1
 * \date        2020/02/05
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
#include "drivers/inc/b_drv_xpt2046.h"

/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup XPT2046
 * \{
 */

/**
 * \defgroup XPT2046_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup XPT2046_Private_Defines
 * \{
 */

#define DRIVER_NAME XPT2046

#define XPT2046_I 0x80
#define XPT2046_X 0xD0
#define XPT2046_Y 0x90
/**
 * \}
 */

/**
 * \defgroup XPT2046_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup XPT2046_Private_Variables
 * \{
 */
bDRIVER_HALIF_TABLE(bXPT2046_HalIf_t, DRIVER_NAME);

/**
 * \}
 */

/**
 * \defgroup XPT2046_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup XPT2046_Private_Functions
 * \{
 */
static void _bXPT2046_SPIW(bDriverInterface_t *pdrv, uint8_t dat)
{
    bDRIVER_GET_HALIF(_if, bXPT2046_HalIf_t, pdrv);
    bHalSpiSend(_if, &dat, 1);
}

static uint8_t _bXPT2046_SPIR(bDriverInterface_t *pdrv)
{
    uint8_t tmp;
    bDRIVER_GET_HALIF(_if, bXPT2046_HalIf_t, pdrv);
    bHalSpiReceive(_if, &tmp, 1);
    return tmp;
}

static uint16_t _bXPT2046ReadVal(bDriverInterface_t *pdrv, uint8_t r)
{
    uint16_t l, h;
    bDRIVER_GET_HALIF(_if, bXPT2046_HalIf_t, pdrv);
    bHalGpioWritePin(_if->cs.port, _if->cs.pin, 0);
    _bXPT2046_SPIW(pdrv, r);
    h = _bXPT2046_SPIR(pdrv);
    l = _bXPT2046_SPIR(pdrv);
    bHalGpioWritePin(_if->cs.port, _if->cs.pin, 1);
    return ((h << 8) | (l)) >> 3;
}

static int _bXPT2046Read(bDriverInterface_t *pdrv, uint32_t addr, uint8_t *pbuf, uint32_t len)
{
    bTouchAdVal_t *pxy = (bTouchAdVal_t *)pbuf;

    if (len < sizeof(bTouchAdVal_t) || pbuf == NULL)
    {
        return -1;
    }
    pxy->x_ad = _bXPT2046ReadVal(pdrv, XPT2046_X);
    pxy->y_ad = _bXPT2046ReadVal(pdrv, XPT2046_Y);
    return sizeof(bTouchAdVal_t);
}

static int _bXPT2046Close(bDriverInterface_t *pdrv)
{
    _bXPT2046ReadVal(pdrv, XPT2046_I);
    return 0;
}

/**
 * \}
 */

/**
 * \addtogroup XPT2046_Exported_Functions
 * \{
 */
int bXPT2046_Init(bDriverInterface_t *pdrv)
{
    bDRIVER_STRUCT_INIT(pdrv, DRIVER_NAME, bXPT2046_Init);
    pdrv->close = _bXPT2046Close;
    pdrv->read  = _bXPT2046Read;
    _bXPT2046ReadVal(pdrv, XPT2046_I);
    return 0;
}

bDRIVER_REG_INIT(B_DRIVER_XPT2046, bXPT2046_Init);

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

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
