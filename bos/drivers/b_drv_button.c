/**
 *!
 * \file        b_drv_button.c
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
#include "drivers/inc/b_drv_button.h"

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup BUTTON
 * \{
 */

/**
 * \defgroup BUTTON_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup BUTTON_Private_Defines
 * \{
 */
#define DRIVER_NAME BUTTON
/**
 * \}
 */

/**
 * \defgroup BUTTON_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup BUTTON_Private_Variables
 * \{
 */
bDRIVER_HALIF_TABLE(bBUTTON_HalIf_t, DRIVER_NAME);
/**
 * \}
 */

/**
 * \defgroup BUTTON_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup BUTTON_Private_Functions
 * \{
 */

static int _bBUTTONRead(bDriverInterface_t *pdrv, uint32_t off, uint8_t *pbuf, uint32_t len)
{
    if (len == 0)
    {
        return -1;
    }
    bDRIVER_GET_HALIF(_if, bBUTTON_HalIf_t, pdrv);
    pbuf[0] = (bHalGpioReadPin(_if->port, _if->pin) == _if->level);
    return 1;
}

/**
 * \}
 */

/**
 * \addtogroup BUTTON_Exported_Functions
 * \{
 */
int bBUTTON_Init(bDriverInterface_t *pdrv)
{
    bDRIVER_STRUCT_INIT(pdrv, DRIVER_NAME, bBUTTON_Init);
    pdrv->read = _bBUTTONRead;
    return 0;
}

bDRIVER_REG_INIT(B_DRIVER_BUTTON, bBUTTON_Init);

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
