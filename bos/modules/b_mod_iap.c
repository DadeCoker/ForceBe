/**
 *!
 * \file        b_mod_iap.c
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
#include "modules/inc/b_mod_iap.h"
#if _IAP_ENABLE
#include <string.h>

#include "hal/inc/b_hal.h"

/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup IAP
 * \{
 */

/**
 * \defgroup IAP_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup IAP_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup IAP_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup IAP_Private_Variables
 * \{
 */
static bIapFlag_t bIapFlag = {
    .stat = B_IAP_STA_NULL,
};
/**
 * \}
 */

/**
 * \defgroup IAP_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup IAP_Private_Functions
 * \{
 */

/**
 * \}
 */

/**
 * \addtogroup IAP_Exported_Functions
 * \{
 */

/**
 * \brief 默认使用重启，用户可以根据需求重写此函数
 */
__WEAKDEF void bIapJump2Boot()
{
    bHalIntDisable();
    ((pJumpFunc_t)(*((volatile uint32_t *)(BOOT_START_ADDR + 4))))();
}

/**
 * \brief 跳转到应用程序，用户可以根据需求重写此函数
 */
__WEAKDEF void bIapJump2App()
{
    bHalIntDisable();
    ((pJumpFunc_t)(*((volatile uint32_t *)(APP_START_ADDR + 4))))();
}

int bIapStart(const char *pfname)
{
    char *ptmp = NULL;
    if (pfname == NULL)
    {
        return -1;
    }
    if (strlen(pfname) > B_IAP_FILENAME_MAXLEN)
    {
        return -2;
    }
    //文件名中必须包含当前的固件名，否则认为是非法文件
    ptmp = strstr(pfname, FW_NAME);
    if (ptmp == NULL)
    {
        return -3;
    }
    memset(&bIapFlag, 0, sizeof(bIapFlag_t));
    //设置当前状态并复制文件名
    bIapFlag.stat           = B_IAP_STA_START;
    bIapFlag.app_invalid    = 0;
    bIapFlag.app_fail_count = 0;
    memcpy(bIapFlag.file_name, pfname, strlen(pfname));
    //标志写入到FLASH，默认BOOT的地址是FLASH的起始地址
    bHalFlashUnlock();
    bHalFlashErase(IAP_FLAG_ADDR - BOOT_START_ADDR, 1);
    bHalFlashWrite(IAP_FLAG_ADDR - BOOT_START_ADDR, (const uint8_t *)&bIapFlag, sizeof(bIapFlag_t));
    bHalFlashLock();
    //跳转到执行BOOT代码
    bIapJump2Boot();
    return 0;
}

/**
 * \brief 应用程序调用，清除标致
 */
int bIapAppCheckFlag()
{
    bHalFlashRead(IAP_FLAG_ADDR - BOOT_START_ADDR, (uint8_t *)&bIapFlag, sizeof(bIapFlag_t));
    if (bIapFlag.stat != B_IAP_STA_NULL)
    {
        bIapFlag.stat = B_IAP_STA_NULL;
        bHalFlashUnlock();
        bHalFlashErase(IAP_FLAG_ADDR - BOOT_START_ADDR, 1);
        bHalFlashWrite(IAP_FLAG_ADDR - BOOT_START_ADDR, (const uint8_t *)&bIapFlag,
                       sizeof(bIapFlag_t));
        bHalFlashLock();
    }
    return 0;
}

/**
 * \brief Boot调用，查询接下来做什么操作
 * \return int B_BOOT_JUMP2APP：跳转应用程序  B_BOOT_WAIT_FW：等待接收新固件
 */
int bIapBootCheckFlag()
{
    int retval = B_BOOT_JUMP2APP;
    bHalFlashRead(IAP_FLAG_ADDR - BOOT_START_ADDR, (uint8_t *)&bIapFlag, sizeof(bIapFlag_t));
    if (bIapFlag.stat == B_IAP_STA_READY)
    {
        bIapFlag.app_fail_count += 1;
        if (bIapFlag.app_fail_count >= B_IAP_APP_FAIL_MAXCOUNT)
        {
            bIapFlag.stat        = B_IAP_STA_START;
            bIapFlag.app_invalid = 1;
            retval               = B_BOOT_WAIT_FW;
        }
        bHalFlashUnlock();
        bHalFlashErase(IAP_FLAG_ADDR - BOOT_START_ADDR, 1);
        bHalFlashWrite(IAP_FLAG_ADDR - BOOT_START_ADDR, (const uint8_t *)&bIapFlag,
                       sizeof(bIapFlag_t));
        bHalFlashLock();
    }
    else if (bIapFlag.stat == B_IAP_STA_START)
    {
        retval = B_BOOT_WAIT_FW;
    }
    return retval;
}

/**
 * \brief 反馈更新固件的结果
 * \param stat B_UPDATE_FW_TIMEOUT or B_UPDATE_FW_SUCCESS
 * \return int B_BOOT_JUMP2APP：跳转应用程序  B_BOOT_WAIT_FW：等待接收新固件
 */
int bIapUpdateFWResult(int result)
{
    int retval = B_BOOT_JUMP2APP;
    if (result == B_UPDATE_FW_TIMEOUT)
    {
        if (bIapFlag.app_invalid == 0)
        {
            bIapFlag.stat = B_IAP_STA_NULL;
            retval        = B_BOOT_JUMP2APP;
        }
        else
        {
            retval = B_BOOT_WAIT_FW;
        }
    }
    else if (B_UPDATE_FW_SUCCESS)
    {
        bIapFlag.stat           = B_IAP_STA_READY;
        bIapFlag.app_fail_count = 0;
        bIapFlag.app_invalid    = 0;
        retval                  = B_BOOT_JUMP2APP;
    }
    else
    {
        retval = B_BOOT_WAIT_FW;
    }
    
    if (retval == B_BOOT_JUMP2APP)
    {
        bHalFlashUnlock();
        bHalFlashErase(IAP_FLAG_ADDR - BOOT_START_ADDR, 1);
        bHalFlashWrite(IAP_FLAG_ADDR - BOOT_START_ADDR, (const uint8_t *)&bIapFlag,
                       sizeof(bIapFlag_t));
        bHalFlashLock();
    }
    return retval;
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
#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
