/**
 *!
 * \file        b_drv_matrixkeys.c
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
#include "drivers/inc/b_drv_matrixkeys.h"

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup MATRIXKEYS
 * \{
 */

/**
 * \defgroup MATRIXKEYS_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup MATRIXKEYS_Private_Defines
 * \{
 */
#define DRIVER_NAME MATRIXKEYS
/**
 * \}
 */

/**
 * \defgroup MATRIXKEYS_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup MATRIXKEYS_Private_Variables
 * \{
 */
bDRIVER_HALIF_TABLE(bMATRIXKEYS_HalIf_t, DRIVER_NAME);
/**
 * \}
 */

/**
 * \defgroup MATRIXKEYS_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup MATRIXKEYS_Private_Functions
 * \{
 */

static int _bMatrixKesyRead(bDriverInterface_t *pdrv, uint32_t off, uint8_t *pbuf, uint32_t len)
{
    bDRIVER_GET_HALIF(_if, bMATRIXKEYS_HalIf_t, pdrv);
    uint8_t  row   = off / _if->columns;
    uint8_t  col   = off % _if->columns;
    uint16_t index = 0;
    if (len == 0 || row >= _if->rows)
    {
        return -1;
    }

    if ((off + len) > (_if->columns * _if->rows))
    {
        len = (_if->columns * _if->rows) - off;
    }

    for (; row < _if->rows; row++)
    {
        bHalGpioConfig(_if->row_io[row].port, _if->row_io[row].pin, B_HAL_GPIO_OUTPUT,
                       B_HAL_GPIO_NOPULL);
        bHalGpioWritePin(_if->row_io[row].port, _if->row_io[row].pin, 0);
        for (; col < _if->columns; col++)
        {
            pbuf[index++] =
                (0 == bHalGpioReadPin(_if->column_io[col].port, _if->column_io[col].pin));
        }
        bHalGpioConfig(_if->row_io[row].port, _if->row_io[row].pin, B_HAL_GPIO_INPUT,
                       B_HAL_GPIO_PULLUP);
        col = 0;
    }
    return len;
}

/**
 * \}
 */

/**
 * \addtogroup MATRIXKEYS_Exported_Functions
 * \{
 */
int bMATRIXKEYS_Init(bDriverInterface_t *pdrv)
{
    bDRIVER_STRUCT_INIT(pdrv, DRIVER_NAME, bMATRIXKEYS_Init);
    pdrv->read = _bMatrixKesyRead;
    return 0;
}

bDRIVER_REG_INIT(B_DRIVER_MATRIXKEYS, bMATRIXKEYS_Init);

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
