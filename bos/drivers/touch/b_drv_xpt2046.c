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
#include "b_drv_xpt2046.h"

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
bXPT2046_Driver_t bXPT2046_Driver;

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
static void _bXPT2046_SPIW(uint8_t dat)
{
    bHalSPI_Send(HAL_XPT2046_SPI, &dat, 1);
}

static uint8_t _bXPT2046_SPIR()
{
    uint8_t tmp;
    bHalSPI_Receive(HAL_XPT2046_SPI, &tmp, 1);
    return tmp;
}

static uint16_t _bXPT2046ReadVal(uint8_t r)
{
    uint16_t l, h;
    bHalGPIO_WritePin(HAL_XPT2046_CS_PORT, HAL_XPT2046_CS_PIN, 0);
    _bXPT2046_SPIW(r);
    h = _bXPT2046_SPIR();
    l = _bXPT2046_SPIR();
    bHalGPIO_WritePin(HAL_XPT2046_CS_PORT, HAL_XPT2046_CS_PIN, 1);
    return ((h << 8) | (l)) >> 3;
}

/**************************************************************************************************driver
 * interface*****/
static int _bXPT2046Read(bXPT2046_Driver_t *pdrv, uint32_t addr, uint8_t *pbuf, uint16_t len)
{
    bTouchAD_ReadStruct_t *pxy = (bTouchAD_ReadStruct_t *)pbuf;

    if (len < sizeof(bTouchAD_ReadStruct_t) || pbuf == NULL)
    {
        return -1;
    }
    pxy->x_ad = _bXPT2046ReadVal(XPT2046_X);
    pxy->y_ad = _bXPT2046ReadVal(XPT2046_Y);
    return sizeof(bTouchAD_ReadStruct_t);
}

static int _bXPT2046Close(bXPT2046_Driver_t *pdrv)
{
    _bXPT2046ReadVal(XPT2046_I);
    return 0;
}

/**
 * \}
 */

/**
 * \addtogroup XPT2046_Exported_Functions
 * \{
 */
int bXPT2046_Init()
{
    _bXPT2046ReadVal(XPT2046_I);
    bXPT2046_Driver.close  = _bXPT2046Close;
    bXPT2046_Driver.read   = _bXPT2046Read;
    bXPT2046_Driver.ctl    = NULL;
    bXPT2046_Driver.open   = NULL;
    bXPT2046_Driver.write  = NULL;
    bXPT2046_Driver.status = 0;
    return 0;
}

bDRIVER_REG_INIT(bXPT2046_Init);

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
