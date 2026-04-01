/**
 *!
 * \file        b_mod_qpn.h
 * \version     v0.0.2
 * \date        2020/05/09
 * \author      Alex_Shen(bestnike@126.com)
 * \note        modified: 2020.05.10 notrynohigh v0.0.2
 *******************************************************************************
 * @attention
 * 
 * Copyright (c) 2020 Alex_Shen
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
#ifndef __B_MOD_QPN__
#define __B_MOD_QPN__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/ 
#include "b_config.h"
#if _QPN_ENABLE
#include "qpn.h"     /* QP-nano API */

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup MODULES
 * \{
 */

/** 
 * \addtogroup QPN
 * \{
 */


/** 
 * \defgroup QPN_Exported_TypesDefinitions
 * \{
 */
typedef void (*AO_InitFunc_t)(void);

/**
 * \}
 */
   
/** 
 * \defgroup QPN_Exported_Defines
 * \{
 */
 
 /**
 * \}
 */
   
/** 
 * \defgroup QPN_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup QPN_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup QPN_Exported_Functions
 * \{
 */
void bQPN_Init(void);


///<Called in bExec()
int bQPN_Run(void);
void bQPN_ISR_TimerCompaVect(void);
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
/************************ Copyright (c) 2020 Alex_Shen *****END OF FILE****/


