/**
 *!
 * \file        b_mod_shell.c
 * \version     v0.0.1
 * \date        2020/01/02
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO SHELL SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */

/*Includes ----------------------------------------------*/
#include "modules/inc/b_mod_shell.h"

#include <string.h>

#include "utils/inc/b_util_log.h"

#if (defined(_NR_MICRO_SHELL_ENABLE) && (_NR_MICRO_SHELL_ENABLE == 1))
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup SHELL
 * \{
 */

/**
 * \defgroup SHELL_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SHELL_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SHELL_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SHELL_Private_Variables
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SHELL_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SHELL_Private_Functions
 * \{
 */
static void _CmdBosHandler(char argc, char *argv)
{
    if (argc == 2)
    {
        if (!strcmp("-v", &argv[argv[1]]))
        {
            b_log("Version:%d.%d.%d\r\n", FW_VERSION / 10000, (FW_VERSION % 10000) / 100,
                  FW_VERSION % 100);
        }
    }
}

bSHELL_REG_INSTANCE("bos", _CmdBosHandler);

/**
 * \}
 */

/**
 * \addtogroup SHELL_Exported_Functions
 * \{
 */

void bShellInit()
{
    shell_init();
}

int bShellParse(uint8_t *pbuf, uint16_t len)
{
    if (pbuf == NULL || len == 0)
    {
        return -1;
    }
    while (len--)
    {
        shell(*pbuf);
        pbuf++;
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
#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
