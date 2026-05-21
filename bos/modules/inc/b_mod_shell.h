/**
 *!
 * \file        b_mod_shell.h
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
#ifndef __B_MOD_SHELL_H__
#define __B_MOD_SHELL_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"
#if _NR_MICRO_SHELL_ENABLE
#include "nr_micro_shell.h"
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
 * \defgroup SHELL_Exported_TypesDefinitions
 * \{
 */

typedef static_cmd_st       bShellInstance_t;
/**
 * \}
 */
   
/** 
 * \defgroup SHELL_Exported_Defines
 * \{
 */
#define bSHELL_INSTANCE(instance_name, cmd_name, cmd_handler)           bShellInstance_t instance_name = {.cmd = cmd_name,\
                                                                                            .fp = cmd_handler,\
                                                                                            .pnext = NULL};      
/**
 * \}
 */
   
/** 
 * \defgroup SHELL_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SHELL_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SHELL_Exported_Functions
 * \{
 */
///< pbShellInstance \ref bSHELL_INSTANCE
int bShellRegistCmd(bShellInstance_t *pbShellInstance);
int bShellParse(uint8_t *pbuf, uint16_t len);
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

#ifdef __cplusplus
	}
#endif

#endif  

/************************ Copyright (c) 2019 Bean *****END OF FILE****/


