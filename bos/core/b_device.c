/**
 *!
 * \file        b_device.c
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

/*Includes ----------------------------------------------*/
#include "core/inc/b_device.h"

#include "drivers/inc/b_driver.h"


/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup CORE
 * \{
 */

/**
 * \addtogroup DEVICE
 * \{
 */

/**
 * \defgroup DEVICE_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup DEVICE_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup DEVICE_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup DEVICE_Private_Variables
 * \{
 */

static bDriverInterface_t bNullDriver;

static bDriverInterface_t *bDriverTable[bDEV_MAX_NUM] = {
#define B_DEVICE_REG(dev, driver, desc) &driver,
#include "b_device_list.h"
};

static const char *bDeviceDescTable[bDEV_MAX_NUM] = {
#define B_DEVICE_REG(dev, driver, desc) desc,
#include "b_device_list.h"
};

bSECTION_DEF_FLASH(driver_init_0, pbDriverInit_t);
bSECTION_DEF_FLASH(driver_init, pbDriverInit_t);
/**
 * \}
 */

/**
 * \defgroup DEVICE_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup DEVICE_Private_Functions
 * \{
 */

/**
 * \}
 */

/**
 * \addtogroup DEVICE_Exported_Functions
 * \{
 */

/**
 * \brief Device Initialize
 */
int bDeviceInit()
{
    memset(&bNullDriver, 0, sizeof(bNullDriver));
    bSECTION_FOR_EACH(driver_init_0, pbDriverInit_t, pdriver_init_0)
    {
        (*pdriver_init_0)();
    }    
    bSECTION_FOR_EACH(driver_init, pbDriverInit_t, pdriver_init)
    {
        (*pdriver_init)();
    }
    return 0;
}

int bDeviceOpen(uint8_t no)
{
    int retval = 0;
    if (no >= bDEV_MAX_NUM)
    {
        return -1;
    }
    if (bDriverTable[no]->status == 0)
    {
        if (bDriverTable[no]->open != NULL)
        {
            retval = bDriverTable[no]->open(bDriverTable[no]);
        }
    }
    else
    {
        b_log_e("%s err\r\n", bDeviceDescTable[no]);
        retval = -255;
    }
    return retval;
}

int bDeviceRead(uint8_t no, uint32_t offset, uint8_t *pdata, uint16_t len)
{
    int retval = 0;
    if (no >= bDEV_MAX_NUM || pdata == NULL)
    {
        return -1;
    }
    if (bDriverTable[no]->status == 0)
    {
        if (bDriverTable[no]->read != NULL)
        {
            retval = bDriverTable[no]->read(bDriverTable[no], offset, pdata, len);
        }
    }
    else
    {
        b_log_e("%s err\r\n", bDeviceDescTable[no]);
        retval = -255;
    }
    return retval;
}

int bDeviceWrite(uint8_t no, uint32_t address, uint8_t *pdata, uint16_t len)
{
    int retval = 0;
    if (no >= bDEV_MAX_NUM || pdata == NULL)
    {
        return -1;
    }
    if (bDriverTable[no]->status == 0)
    {
        if (bDriverTable[no]->write != NULL)
        {
            retval = bDriverTable[no]->write(bDriverTable[no], address, pdata, len);
        }
    }
    else
    {
        b_log_e("%s err\r\n", bDeviceDescTable[no]);
        retval = -255;
    }
    return retval;
}

int bDeviceClose(uint8_t no)
{
    int retval = 0;
    if (no >= bDEV_MAX_NUM)
    {
        return -1;
    }
    if (bDriverTable[no]->status == 0)
    {
        if (bDriverTable[no]->close != NULL)
        {
            retval = bDriverTable[no]->close(bDriverTable[no]);
        }
    }
    else
    {
        b_log_e("%s err\r\n", bDeviceDescTable[no]);
        retval = -255;
    }
    return retval;
}

int bDeviceCtl(uint8_t no, uint8_t cmd, void *param)
{
    int retval = 0;
    if (no >= bDEV_MAX_NUM)
    {
        return -1;
    }
    if (bDriverTable[no]->status == 0)
    {
        if (bDriverTable[no]->ctl != NULL)
        {
            retval = bDriverTable[no]->ctl(bDriverTable[no], cmd, param);
        }
    }
    else
    {
        b_log_e("%s err\r\n", bDeviceDescTable[no]);
        retval = -255;
    }
    return retval;
}

int bDeviceISNormal(uint8_t no)
{
    if (no >= bDEV_MAX_NUM)
    {
        return -1;
    }
    b_log("%s :%d\r\n", bDeviceDescTable[no], bDriverTable[no]->status);
    return bDriverTable[no]->status;
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

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
