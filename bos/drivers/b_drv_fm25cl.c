/**
 *!
 * \file        b_drv_fm25cl.c
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
#include "drivers/inc/b_drv_fm25cl.h"

/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup FM25CL
 * \{
 */

/**
 * \defgroup FM25CL_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup FM25CL_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup FM25CL_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup FM25CL_Private_Variables
 * \{
 */
HALIF_KEYWORD bFM25CL_HalIf_t bFM25CL_HalIfTable[] = HAL_FM25CL_IF;
bFM25CL_Driver_t              bFM25CL_Driver[sizeof(bFM25CL_HalIfTable) / sizeof(bFM25CL_HalIf_t)];
/**
 * \}
 */

/**
 * \defgroup FM25CL_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup FM25CL_Private_Functions
 * \{
 */

static void _FM25_WR_Enable(bFM25CL_HalIf_t *_if)
{
    uint8_t cmd = SFC_WREN;
    bHalGpioWritePin(_if->cs.port, _if->cs.pin, 0);
    bHalSpiSend(_if, &cmd, 1);
    bHalGpioWritePin(_if->cs.port, _if->cs.pin, 1);
}

static void _FM25_WR_Lock(bFM25CL_HalIf_t *_if)
{
    uint8_t cmd = SFC_WRDI;
    bHalGpioWritePin(_if->cs.port, _if->cs.pin, 0);
    bHalSpiSend(_if, &cmd, 1);
    bHalGpioWritePin(_if->cs.port, _if->cs.pin, 1);
}

/**************************************************************************************************driver
 * interface*****/

static int _FM25_ReadBuff(bFM25CL_Driver_t *pdrv, uint32_t addr, uint8_t *pDat, uint32_t len)
{
    uint8_t cmd[3];
    bDRV_GET_HALIF(_if, bFM25CL_HalIf_t, pdrv);

    cmd[0] = SFC_READ;
    cmd[1] = (uint8_t)(addr >> 8);
    cmd[2] = (uint8_t)(addr >> 0);

    bHalGpioWritePin(_if->cs.port, _if->cs.pin, 0);
    bHalSpiSend(_if, cmd, 3);
    bHalSpiReceive(_if, pDat, len);
    bHalGpioWritePin(_if->cs.port, _if->cs.pin, 1);
    return len;
}

static int _FM25_WritBuff(bFM25CL_Driver_t *pdrv, uint32_t addr, uint8_t *pdat, uint32_t len)
{
    uint8_t cmd[3];
    bDRV_GET_HALIF(_if, bFM25CL_HalIf_t, pdrv);
    _FM25_WR_Enable(_if);
    //-----------------------------------------------------------
    cmd[0] = SFC_WRITE;
    cmd[1] = (uint8_t)(addr >> 8);
    cmd[2] = (uint8_t)(addr >> 0);
    bHalGpioWritePin(_if->cs.port, _if->cs.pin, 0);
    bHalSpiSend(_if, cmd, 3);
    bHalSpiSend(_if, pdat, len);
    bHalGpioWritePin(_if->cs.port, _if->cs.pin, 1);
    //-----------------------------------------------------------
    _FM25_WR_Lock(_if);
    return len;
}

/**
 * \}
 */

/**
 * \addtogroup FM25CL_Exported_Functions
 * \{
 */
int bFM25CL_Init()
{
    uint8_t i = 0, num_drv = (sizeof(bFM25CL_HalIfTable) / sizeof(bFM25CL_HalIf_t));
    for (i = 0; i < num_drv; i++)
    {
        bFM25CL_Driver[i].init    = bFM25CL_Init;
        bFM25CL_Driver[i].status  = 0;
        bFM25CL_Driver[i].close   = NULL;
        bFM25CL_Driver[i].read    = _FM25_ReadBuff;
        bFM25CL_Driver[i].ctl     = NULL;
        bFM25CL_Driver[i].open    = NULL;
        bFM25CL_Driver[i].write   = _FM25_WritBuff;
        bFM25CL_Driver[i]._hal_if = (void *)&bFM25CL_HalIfTable[i];
    }
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

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
