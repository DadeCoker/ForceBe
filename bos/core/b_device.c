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
#include "utils/inc/b_util_log.h"
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
static bDriverNumber_t bDriverNumberTable[B_REG_DRV_NUMBER] = {
#define B_DEVICE_REG(dev, driver, desc) driver,
#include "b_device_list.h"
    B_DRIVER_NULL,
};

static const char *bDeviceDescTable[B_REG_DRV_NUMBER] = {
#define B_DEVICE_REG(dev, driver, desc) desc,
#include "b_device_list.h"
    "null",
};

static bDriverInterface_t bDriverInterfaceTable[B_REG_DRV_NUMBER];

bSECTION_DEF_FLASH(driver_init_0, bDriverRegInit_t);
bSECTION_DEF_FLASH(driver_init, bDriverRegInit_t);
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

static int _bDriverNullInit(bDriverInterface_t *pdrv)
{
    (void)pdrv;  // prevent unused warning
    if (strcmp(bDeviceDescTable[0], "null") == 0)
    {
        b_log_i("No device is registered\r\n");
    }
    return 0;
}

bDRIVER_REG_INIT_0(B_DRIVER_NULL, _bDriverNullInit);
bDRIVER_REG_INIT(B_DRIVER_NULL, _bDriverNullInit);

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
    uint8_t i = 0, j = 0;
    memset(bDriverInterfaceTable, 0, sizeof(bDriverInterfaceTable));
    bSECTION_FOR_EACH(driver_init_0, bDriverRegInit_t, pdriver_init_0)
    {
        j = 0;
        for (i = 0; i < B_REG_DRV_NUMBER; i++)
        {
            if (bDriverNumberTable[i] == pdriver_init_0->drv_number)
            {
                bDriverInterfaceTable[i].drv_no = j++;
                bDriverInterfaceTable[i].status = pdriver_init_0->init(&bDriverInterfaceTable[i]);
            }
        }
    }
    bSECTION_FOR_EACH(driver_init, bDriverRegInit_t, pdriver_init)
    {
        j = 0;
        for (i = 0; i < B_REG_DRV_NUMBER; i++)
        {
            if (bDriverNumberTable[i] == pdriver_init->drv_number)
            {
                bDriverInterfaceTable[i].drv_no = j++;
                bDriverInterfaceTable[i].status = pdriver_init->init(&bDriverInterfaceTable[i]);
            }
        }
    }
    return 0;
}

int bDeviceReinit(uint8_t no)
{
    if (no >= B_REG_DRV_NUMBER)
    {
        return -1;
    }
    if (bDriverInterfaceTable[no].init == NULL)
    {
        return B_DEVICE_FUNC_NULL;
    }
    return bDriverInterfaceTable[no].init(&bDriverInterfaceTable[no]);
}

int bDeviceOpen(uint8_t no)
{
    int retval = B_DEVICE_FUNC_NULL;
    if (no >= B_REG_DRV_NUMBER)
    {
        return -1;
    }
    if (bDriverInterfaceTable[no].status == 0)
    {
        if (bDriverInterfaceTable[no].open != NULL)
        {
            retval = bDriverInterfaceTable[no].open(&bDriverInterfaceTable[no]);
        }
    }
    else
    {
        b_log_e("%s err\r\n", bDeviceDescTable[no]);
        retval = B_DEVICE_STAT_ERR;
    }
    return retval;
}

int bDeviceRead(uint8_t no, uint32_t offset, uint8_t *pdata, uint32_t len)
{
    int retval = B_DEVICE_FUNC_NULL;
    if (no >= B_REG_DRV_NUMBER || pdata == NULL)
    {
        return -1;
    }
    if (bDriverInterfaceTable[no].status == 0)
    {
        if (bDriverInterfaceTable[no].read != NULL)
        {
            retval = bDriverInterfaceTable[no].read(&bDriverInterfaceTable[no], offset, pdata, len);
        }
    }
    else
    {
        b_log_e("%s err\r\n", bDeviceDescTable[no]);
        retval = B_DEVICE_STAT_ERR;
    }
    return retval;
}

int bDeviceWrite(uint8_t no, uint32_t address, uint8_t *pdata, uint32_t len)
{
    int retval = B_DEVICE_FUNC_NULL;
    if (no >= B_REG_DRV_NUMBER || pdata == NULL)
    {
        return -1;
    }
    if (bDriverInterfaceTable[no].status == 0)
    {
        if (bDriverInterfaceTable[no].write != NULL)
        {
            retval =
                bDriverInterfaceTable[no].write(&bDriverInterfaceTable[no], address, pdata, len);
        }
    }
    else
    {
        b_log_e("%s err\r\n", bDeviceDescTable[no]);
        retval = B_DEVICE_STAT_ERR;
    }
    return retval;
}

int bDeviceClose(uint8_t no)
{
    int retval = B_DEVICE_FUNC_NULL;
    if (no >= B_REG_DRV_NUMBER)
    {
        return -1;
    }
    if (bDriverInterfaceTable[no].status == 0)
    {
        if (bDriverInterfaceTable[no].close != NULL)
        {
            retval = bDriverInterfaceTable[no].close(&bDriverInterfaceTable[no]);
        }
    }
    else
    {
        b_log_e("%s err\r\n", bDeviceDescTable[no]);
        retval = B_DEVICE_STAT_ERR;
    }
    return retval;
}

int bDeviceCtl(uint8_t no, uint8_t cmd, void *param)
{
    int retval = B_DEVICE_FUNC_NULL;
    if (no >= B_REG_DRV_NUMBER)
    {
        return -1;
    }
    if (bDriverInterfaceTable[no].status == 0)
    {
        if (bDriverInterfaceTable[no].ctl != NULL)
        {
            retval = bDriverInterfaceTable[no].ctl(&bDriverInterfaceTable[no], cmd, param);
        }
    }
    else
    {
        b_log_e("%s err\r\n", bDeviceDescTable[no]);
        retval = B_DEVICE_STAT_ERR;
    }
    return retval;
}

int bDeviceModifyHalIf(uint8_t no, uint32_t offset, const uint8_t *pVal, uint8_t size)
{
    uint32_t halif_addr = 0;
    if (no >= B_REG_DRV_NUMBER || pVal == NULL || size == 0)
    {
        return -1;
    }
    if (bDriverInterfaceTable[no]._hal_if == NULL)
    {
        return -2;
    }
    halif_addr = ((uint32_t)bDriverInterfaceTable[no]._hal_if) + offset;
#if _HALIF_VARIABLE_ENABLE
    memcpy((uint8_t *)halif_addr, pVal, size);
    return 0;
#else
    halif_addr = halif_addr;
    return -1;
#endif
}

int bDeviceISNormal(uint8_t no)
{
    if (no >= B_REG_DRV_NUMBER)
    {
        return -1;
    }
    b_log("%s :%d\r\n", bDeviceDescTable[no], bDriverInterfaceTable[no].status);
    return bDriverInterfaceTable[no].status;
}

/**
 * \brief Read device private information
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bDeviceReadMessage(uint8_t no, bDeviceMsg_t *pmsg)
{
    if (pmsg == NULL || no >= B_REG_DRV_NUMBER)
    {
        return -1;
    }
    pmsg->v = bDriverInterfaceTable[no]._private.v;
    return 0;
}

/**
 * \brief Write device private information
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bDeviceWriteMessage(uint8_t no, bDeviceMsg_t *pmsg)
{
    if (pmsg == NULL || no >= B_REG_DRV_NUMBER)
    {
        return -1;
    }
    bDriverInterfaceTable[no]._private.v = pmsg->v;
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

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
