/**
 *!
 * \file        b_hal_qspi.h
 * \version     v0.0.1
 * \date        2020/03/25
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SQSPIL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
#ifndef __B_HAL_QSPI_H__
#define __B_HAL_QSPI_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_config.h" 


/** 
 * \addtogroup B_HAL
 * \{
 */

/** 
 * \addtogroup QSPI
 * \{
 */

/** 
 * \defgroup QSPI_Exported_TypesDefinitions
 * \{
 */
typedef enum
{
    B_HAL_QSPI_1,
    B_HAL_QSPI_INVALID,
}bHalQSPINumber_t; 
/**
 * \}
 */
   
/** 
 * \defgroup QSPI_Exported_Defines
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup QSPI_Exported_Macros
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup QSPI_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup QSPI_Exported_Functions
 * \{
 */
int bHalQSPI_Command(bHalQSPINumber_t qspi,
                     uint32_t cmd, 
                     uint32_t addr, 
                     uint32_t addr_mode, 
                     uint32_t addr_size_bit, 
                     uint32_t data_mode, 
                     uint32_t nb_data);
int bHalQSPI_Send(bHalQSPINumber_t qspi, uint8_t *pbuf, uint16_t len);
int bHalQSPI_Receive(bHalQSPINumber_t qspi, uint8_t *pbuf, uint16_t len);
/**
 * \}
 */

/**
 * \}
 */

/**
 * \}
 */

#ifdef __cplusplus
	}
#endif
 
#endif


/************************ Copyright (c) 2020 Bean *****END OF FILE****/


