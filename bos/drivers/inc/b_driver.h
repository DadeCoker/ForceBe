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
    int (*init)(struct bDriverIf *pdrv);
    int (*open)(struct bDriverIf *pdrv);
    int (*close)(struct bDriverIf *pdrv);
    int (*ctl)(struct bDriverIf *pdrv, uint8_t cmd, void *param);
    int (*write)(struct bDriverIf *pdrv, uint32_t offset, uint8_t *pbuf, uint32_t len);
    int (*read)(struct bDriverIf *pdrv, uint32_t offset, uint8_t *pbuf, uint32_t len);
    void   *hal_if;
    uint8_t drv_no;
    union
    {
        uint32_t v;
        void    *_p;
    } _private;
} bDriverInterface_t;

typedef enum
{
    B_DRIVER_NULL = 0,
    B_DRIVER_SPIFLASH,
    B_DRIVER_FM25CL,
    B_DRIVER_24CXX,
    B_DRIVER_SD,
    B_DRIVER_MCUFLASH,
    B_DRIVER_TESTFLASH,
    B_DRIVER_SSD1289,
    B_DRIVER_ILI9341,
    B_DRIVER_ILI9320,
    B_DRIVER_ST7789,
    B_DRIVER_OLED,
    B_DRIVER_XPT2046,
    B_DRIVER_PCF8574,
    B_DRIVER_LIS3DH,
    B_DRIVER_DS18B20,
    B_DRIVER_ESP12F,
    B_DRIVER_NUMBER
} bDriverNumber_t;

typedef struct
{
    bDriverNumber_t drv_number;
    int (*init)(bDriverInterface_t *pdrv);
} bDriverRegInit_t;

/**
 * \}
 */

/**
 * \defgroup DRIVER_Exported_Defines
 * \{
 */
#define _bDRIVER_HALIF_TABLE(halif_type, drv_name) \
    HALIF_KEYWORD halif_type bHalIf_##drv_name[] = {HAL_##drv_name##_IF}
#define _bDRIVER_HALIF_INSTANCE(drv_name, drv_no) (void *)&bHalIf_##drv_name[drv_no]
#define _bDRIVER_HALIF_NUM(halif_type, drv_name) (sizeof(bHalIf_##drv_name) / sizeof(halif_type))

#define bDRIVER_HALIF_TABLE(halif_type, drv_name) _bDRIVER_HALIF_TABLE(halif_type, drv_name)
#define bDRIVER_HALIF_INSTANCE(drv_name, drv_no) _bDRIVER_HALIF_INSTANCE(drv_name, drv_no)
#define bDRIVER_HALIF_NUM(halif_type, drv_name) _bDRIVER_HALIF_NUM(halif_type, drv_name)

#define _bDRIVER_STRUCT_INIT(pdrv, drv_name, init_f)                        \
    do                                                                      \
    {                                                                       \
        pdrv->status     = 0;                                               \
        pdrv->init       = init_f;                                          \
        pdrv->open       = NULL;                                            \
        pdrv->close      = NULL;                                            \
        pdrv->ctl        = NULL;                                            \
        pdrv->write      = NULL;                                            \
        pdrv->read       = NULL;                                            \
        pdrv->hal_if     = _bDRIVER_HALIF_INSTANCE(drv_name, pdrv->drv_no); \
        pdrv->_private.v = 0;                                               \
    } while (0)
#define bDRIVER_STRUCT_INIT(pdrv, drv_name, init_f) _bDRIVER_STRUCT_INIT(pdrv, drv_name, init_f)

#define bDRIVER_GET_HALIF(name, type, pdrv) type *name = (type *)((pdrv)->hal_if)

/**
 * \}
 */

/**
 * \defgroup DRIVER_Exported_Macros
 * \{
 */
#define bDRIVER_REG_INIT_0(drv_num, init_f)                                                  \
    bSECTION_ITEM_REGISTER_FLASH(driver_init_0, bDriverRegInit_t, CONCAT_2(init0, func)) = { \
        drv_num, init_f}

#define bDRIVER_REG_INIT(drv_num, init_f)                                                         \
    bSECTION_ITEM_REGISTER_FLASH(driver_init, bDriverRegInit_t, CONCAT_2(init, func)) = {drv_num, \
                                                                                         init_f}

#define bDRIVER_REG_INIT_1 bDRIVER_REG_INIT
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
