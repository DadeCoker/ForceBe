/**
 *!
 * \file        b_driver.h
 * \version     v0.0.1
 * \date        2019/06/05
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2019 Bean
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
#ifndef __B_DRIVER_H__
#define __B_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include <stdint.h>

#include "b_driver_cmd.h"
#include "b_section.h"
#include "hal/inc/b_hal.h"

/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup CORE
 * \{
 */

/**
 * \addtogroup DRIVER
 * \{
 */

/**
 * \defgroup DRIVER_Exported_TypesDefinitions
 * \{
 */
typedef struct bDriverIf
{
    int status;
    int (*init)(void);
    int (*open)(struct bDriverIf *pdrv);
    int (*close)(struct bDriverIf *pdrv);
    int (*ctl)(struct bDriverIf *pdrv, uint8_t cmd, void *param);
    int (*write)(struct bDriverIf *pdrv, uint32_t offset, uint8_t *pbuf, uint16_t len);
    int (*read)(struct bDriverIf *pdrv, uint32_t offset, uint8_t *pbuf, uint16_t len);
    void *_hal_if;
    union
    {
        uint32_t v;
        void    *_p;
    } _private;
} bDriverInterface_t;

typedef int (*pbDriverInit_t)(void);
/**
 * \}
 */

/**
 * \defgroup DRIVER_Exported_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup DRIVER_Exported_Macros
 * \{
 */
#define bDRIVER_REG_INIT_0(func) \
    bSECTION_ITEM_REGISTER_FLASH(driver_init_0, pbDriverInit_t, CONCAT_2(init0, func)) = func

#define bDRIVER_REG_INIT(func) \
    bSECTION_ITEM_REGISTER_FLASH(driver_init, pbDriverInit_t, CONCAT_2(init, func)) = func

#define bDRIVER_REG_INIT_1 bDRIVER_REG_INIT

#define bDRV_GET_HALIF(name, type, pdrv) type *name = (type *)((pdrv)->_hal_if)
/**
 * \}
 */

/**
 * \defgroup DRIVER_Exported_Variables
 * \{
 */

extern bDriverInterface_t bSPIFLASH_Driver[];
extern bDriverInterface_t bFM25CL_Driver[];
extern bDriverInterface_t b24CXX_Driver[];
extern bDriverInterface_t bSD_Driver;
extern bDriverInterface_t bMCUFLASH_Driver;

extern bDriverInterface_t bSSD1289_Driver;
extern bDriverInterface_t bILI9341_Driver;
extern bDriverInterface_t bILI9320_Driver;
extern bDriverInterface_t bST7789_Driver;
extern bDriverInterface_t bOLED_Driver;

extern bDriverInterface_t bXPT2046_Driver;

extern bDriverInterface_t bPCF8574_Driver;

extern bDriverInterface_t bLIS3DH_Driver;

extern bDriverInterface_t bDS18B20_Driver;

extern bDriverInterface_t bESP12F_Driver;
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

#ifdef __cplusplus
}
#endif

#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
