/**
 *!
 * \file        b_mod_trace.c
 * \version     v0.0.1
 * \date        2020/03/10
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO TRACE SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */

/*Includes ----------------------------------------------*/
#include "modules/inc/b_mod_trace.h"
#if (defined(_CMBACKTRACE_ENABLE) && (_CMBACKTRACE_ENABLE == 1))

/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup TRACE
 * \{
 */

/**
 * \defgroup TRACE_Private_Defines
 * \{
 */
#define _VERSION_S(n) #n
#define VERSION_S(n) _VERSION_S(n)

const char *pVersion[2] = {VERSION_S(HW_VERSION), VERSION_S(FW_VERSION)};
/**
 * \}
 */

/**
 * \addtogroup TRACE_Exported_Functions
 * \{
 */

/**
 * \brief Initialize
 * \param pfw_name Firmware name (BabyOS.axf <==> pfw_name = "BabyOS")
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bTraceInit(const char *pfw_name)
{
    cm_backtrace_init(pfw_name, pVersion[0], pVersion[1]);
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
