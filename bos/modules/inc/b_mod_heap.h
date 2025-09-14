/**
 *!
 * \file        b_mod_heap.h
 * \version     v0.0.1
 * \date        2020/04/02
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
#ifndef __B_MOD_HEAP_H__
#define __B_MOD_HEAP_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h"  
#if _HEAP_ENABLE
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup MODULES
 * \{
 */

/** 
 * \addtogroup HEAP
 * \{
 */

/** 
 * \defgroup HEAP_Exported_TypesDefinitions
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup HEAP_Exported_Defines
 * \{
 */
#define HEAP_ADDRESS          _EX_HEAP_ADDRESS     
#define HEAP_SIZE             _HEAP_SIZE    
/**
 * \}
 */
   
/** 
 * \defgroup HEAP_Exported_Macros
 * \{
 */
#define configTOTAL_HEAP_SIZE       HEAP_SIZE
#define portBYTE_ALIGNMENT			8

#if portBYTE_ALIGNMENT == 8
	#define portBYTE_ALIGNMENT_MASK ( 0x0007 )
#endif

#ifndef mtCOVERAGE_TEST_MARKER
	#define mtCOVERAGE_TEST_MARKER()
#endif

#ifndef configASSERT
	#define configASSERT( x )
	#define configASSERT_DEFINED 0
#else
	#define configASSERT_DEFINED 1
#endif

#ifndef traceMALLOC
    #define traceMALLOC( pvAddress, uiSize )
#endif

#ifndef traceFREE
    #define traceFREE( pvAddress, uiSize )
#endif 
/**
 * \}
 */
   
/** 
 * \defgroup HEAP_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup HEAP_Exported_Functions
 * \{
 */
void *bMalloc(uint32_t xWantedSize);
void bFree( void *pv );
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

