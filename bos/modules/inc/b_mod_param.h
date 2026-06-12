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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO PARAM SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
#ifndef __B_MOD_PARAM_H__
#define __B_MOD_PARAM_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"
#if _PARAM_ENABLE
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
 * \defgroup PARAM_Exported_TypesDefinitions
 * \{
 */
typedef struct bParamStruct
{
    uint8_t size;
    char *name;
    void *addr;
    struct bParamStruct *pnext;
}bParamStruct_t;


typedef bParamStruct_t       bParamInstance_t;

/**
 * \}
 */
   
/** 
 * \defgroup PARAM_Exported_Defines
 * \{
 */

#define _PARAM2STR(n)       (#n)

#define bPARAM_INSTANCE(instance_name, param, param_size)       bParamInstance_t instance_name = {.size = param_size,\
                                                                                            .name = _PARAM2STR(param),\
                                                                                            .addr = &(param),\
                                                                                            .pnext = NULL};

/**
 * \}
 */
   
/** 
 * \defgroup PARAM_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup PARAM_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup PARAM_Exported_Functions
 * \{
 */
int bParamInit(void); 
///< pParamInstance \ref bPARAM_INSTANCE
int bParamRegist(bParamInstance_t *pParamInstance);
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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/


