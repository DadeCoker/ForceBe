/**
 *!
 * \file        b_mod_param.c
 * \version     v0.0.1
 * \date        2020/05/24
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO PARAM SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */

/*Includes ----------------------------------------------*/
#include "modules/inc/b_mod_param.h"
#if _PARAM_ENABLE
#if (_NR_MICRO_SHELL_ENABLE == 0)
#error "please enable _NR_MICRO_SHELL_ENABLE"
#endif
#include <stdlib.h>
#include <string.h>

#include "modules/inc/b_mod_shell.h"
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup PARAM
 * \{
 */

/**
 * \defgroup PARAM_Private_TypesDefinitions
 * \{
 */
static void _ShellParamHandle(char argc, char *argv);
bSHELL_REG_INSTANCE("param", _ShellParamHandle);

/**
 * \}
 */

/**
 * \defgroup PARAM_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup PARAM_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup PARAM_Private_Variables
 * \{
 */
bSECTION_DEF_FLASH(b_mod_param, bParamInstance_t);
/**
 * \}
 */

/**
 * \defgroup PARAM_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup PARAM_Private_Functions
 * \{
 */

static int _copy2int(void *addr, uint8_t size)
{
    int tmp = 0;
    if (size > 4)
        size = 4;
    memcpy(&tmp, addr, size);
    return tmp;
}

static void _ShellParamHandle(char argc, char *argv)
{
    int val;
    if (argc > 1)
    {
        bSECTION_FOR_EACH(b_mod_param, bParamInstance_t, ptmp)
        {
            if (!strcmp(ptmp->name, &argv[argv[1]]))
            {
                if (argc == 3)
                {
                    val = atoi(&argv[argv[2]]);
                    memcpy(ptmp->addr, (void *)&val, ptmp->size);
                }
                else if (argc == 2)
                {
                    b_log("%s:%d\r\n", ptmp->name, _copy2int(ptmp->addr, ptmp->size));
                }
                break;
            }
        }
    }
    else
    {
        bSECTION_FOR_EACH(b_mod_param, bParamInstance_t, ptmp)
        {
            b_log(": %s\r\n", ptmp->name);
        }
    }
}
/**
 * \}
 */

/**
 * \addtogroup PARAM_Exported_Functions
 * \{
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

/**
 * \}
 */
#endif

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
