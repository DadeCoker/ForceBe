/**
 *!
 * \file        b_core.c
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
#include "core/inc/b_core.h"

#include "b_section.h"
#include "core/inc/b_device.h"
#include "hal/inc/b_hal.h"
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
 * \addtogroup CORE
 * \{
 */

/**
 * \defgroup CORE_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup CORE_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup CORE_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup CORE_Private_Variables
 * \{
 */

static bCoreFd_t bCoreFdTable[B_REG_DEV_NUMBER];

bSECTION_DEF_FLASH(bos_polling, pbPoling_t);
/**
 * \}
 */

/**
 * \defgroup CORE_Private_FunctionPrototypes
 * \{
 */
static void _bCoreMonitor(void);
BOS_REG_POLLING_FUNC(_bCoreMonitor);
/**
 * \}
 */

/**
 * \defgroup CORE_Private_Functions
 * \{
 */

static void _bCoreMonitor()
{
    ;
}

static int _bCoreCreateFd(uint32_t dev_no, uint8_t flag)
{
    int            i    = 0;
    int            fd   = -1;
    static uint8_t init = 0;
    if (init == 0)
    {
        for (i = 0; i < B_REG_DEV_NUMBER; i++)
        {
            bCoreFdTable[i].status = BCORE_STA_NULL;
        }
        init = 1;
    }

    for (i = 0; i < B_REG_DEV_NUMBER; i++)
    {
        if (bCoreFdTable[i].status == BCORE_STA_OPEN)
        {
            if (dev_no == bCoreFdTable[i].number)
            {
                return -1;
            }
        }
    }

    for (i = 0; i < B_REG_DEV_NUMBER; i++)
    {
        if (bCoreFdTable[i].status == BCORE_STA_NULL)
        {
            bCoreFdTable[i].flag   = flag;
            bCoreFdTable[i].number = dev_no;
            bCoreFdTable[i].status = BCORE_STA_OPEN;
            bCoreFdTable[i].lseek  = 0;
            fd                     = i;
            break;
        }
    }
    return fd;
}

static int _bCoreDeleteFd(int fd)
{
    if (fd < 0)
    {
        return -1;
    }
    if (bCoreFdTable[fd].status == BCORE_STA_NULL)
    {
        return -1;
    }
    bCoreFdTable[fd].status = BCORE_STA_NULL;
    return 0;
}

/**
 * \}
 */

/**
 * \addtogroup CORE_Exported_Functions
 * \{
 */

/**
 * \brief Open a device
 * \param dev_no Device Number
 * \param flag Open Flag
 *          \arg \ref BCORE_FLAG_R
 *          \arg \ref BCORE_FLAG_W
 *          \arg \ref BCORE_FLAG_RW
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bOpen(uint32_t dev_no, uint8_t flag)
{
    int fd     = -1;
    int retval = 0;
    if (!IS_VALID_FLAG(flag))
    {
        return -1;
    }
    fd = _bCoreCreateFd(dev_no, flag);
    if (fd < 0)
    {
        return -1;
    }
    retval = bDeviceOpen(dev_no);
    if (retval >= 0 || retval == B_DEVICE_FUNC_NULL)
    {
        return fd;
    }
    _bCoreDeleteFd(fd);
    return -1;
}

int bRead(int fd, uint8_t *pdata, uint32_t len)
{
    int retval;
    if (fd < 0 || fd >= B_REG_DEV_NUMBER || pdata == NULL)
    {
        return -1;
    }

    if (!READ_IS_VALID(bCoreFdTable[fd].flag) || bCoreFdTable[fd].status == BCORE_STA_NULL)
    {
        return -1;
    }

    retval = bDeviceRead(bCoreFdTable[fd].number, bCoreFdTable[fd].lseek, pdata, len);
    if (retval >= 0)
    {
        bCoreFdTable[fd].lseek += len;
    }
    return retval;
}

int bWrite(int fd, uint8_t *pdata, uint32_t len)
{
    int retval;
    if (fd < 0 || fd >= B_REG_DEV_NUMBER || pdata == NULL)
    {
        return -1;
    }

    if (!WRITE_IS_VALID(bCoreFdTable[fd].flag) || bCoreFdTable[fd].status == BCORE_STA_NULL)
    {
        return -1;
    }

    retval = bDeviceWrite(bCoreFdTable[fd].number, bCoreFdTable[fd].lseek, pdata, len);
    if (retval >= 0)
    {
        bCoreFdTable[fd].lseek += len;
    }
    return retval;
}

int bLseek(int fd, uint32_t off)
{
    if (fd < 0 || fd >= B_REG_DEV_NUMBER)
    {
        return -1;
    }

    if (bCoreFdTable[fd].status == BCORE_STA_NULL)
    {
        return -1;
    }
    bCoreFdTable[fd].lseek = off;
    return 0;
}

int bCtl(int fd, uint8_t cmd, void *param)
{
    if (fd < 0 || fd >= B_REG_DEV_NUMBER)
    {
        return -1;
    }
    return bDeviceCtl(bCoreFdTable[fd].number, cmd, param);
}

int bClose(int fd)
{
    if (fd < 0 || fd >= B_REG_DEV_NUMBER)
    {
        return -1;
    }

    if (bCoreFdTable[fd].status == BCORE_STA_NULL)
    {
        return -1;
    }
    bDeviceClose(bCoreFdTable[fd].number);
    return _bCoreDeleteFd(fd);
}

int bCoreIsIdle()
{
    int i;
    for (i = 0; i < B_REG_DEV_NUMBER; i++)
    {
        if (bCoreFdTable[i].status == BCORE_STA_OPEN)
        {
            return -1;
        }
    }
    return 0;
}

/**
 * \brief Init
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bInit()
{
    bHalInit();
    b_log("______________________________________________\r\n");
    b_log("    ____                         __       __  \r\n");
    b_log("    /   )          /           /    )   /    \\\r\n");
    b_log("---/__ /-----__---/__---------/----/----\\-----\r\n");
    b_log("  /    )   /   ) /   ) /   / /    /      \\    \r\n");
    b_log("_/____/___(___(_(___/_(___/_(____/___(____/___\r\n");
    b_log("                         /                    \r\n");
    b_log("                     (_ /                     \r\n");
    b_log("HW:%d.%d.%d FW:%d.%d.%d COMPILE:%s-%s\r\n", (HW_VERSION / 10000),
          (HW_VERSION % 10000) / 100, HW_VERSION % 100, (FW_VERSION / 10000),
          (FW_VERSION % 10000) / 100, FW_VERSION % 100, __DATE__, __TIME__);
    b_log("device number:%d\r\n", B_REG_DEV_NUMBER);
    return bDeviceInit();
}

int bReinit(uint32_t dev_no)
{
    return bDeviceReinit(dev_no);
}

/**
 * \brief  Call this function inside the while(1)
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bExec()
{
    bSECTION_FOR_EACH(bos_polling, pbPoling_t, polling)
    {
        (*polling)();
    }
    return 0;
}

/**
 * \brief  eg. bModifyHalIf(OLED, sizeof(bOLED_HalIf_t),
 *                           (uint8_t)(&(((bOLED_HalIf_t *)0)->_if._i2c.dev_addr)),
 *                            &dev_addr, 1);
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bModifyHalIf(uint32_t dev_no, uint32_t type_size, uint32_t off, const uint8_t *pval,
                 uint8_t len)
{
    if ((off + len) > type_size || pval == NULL || len == 0 || type_size == 0)
    {
        return -1;
    }
    return bDeviceModifyHalIf(dev_no, off, pval, len);
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
