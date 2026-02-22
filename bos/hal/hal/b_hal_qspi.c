/**
 *!
 * \file        b_hal_qspi.c
 * \version     v0.0.1
 * \date        2020/05/04
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
   
/*Includes ----------------------------------------------*/
#include "b_hal.h" 
/** 
 * \addtogroup B_HAL
 * \{
 */

/** 
 * \addtogroup QSPI
 * \{
 */

/** 
 * \defgroup QSPI_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup QSPI_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup QSPI_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup QSPI_Private_Variables
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup QSPI_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup QSPI_Private_Functions
 * \{
 */


/**
 * \}
 */
   
/** 
 * \addtogroup QSPI_Exported_Functions
 * \{
 */

int bHalQSPI_Send(uint8_t no, uint8_t *pbuf, uint16_t len)
{
    if(pbuf == NULL)
    {
        return -1;
    }
    switch(no)
    {
        case B_HAL_QSPI_1:
//            HAL_QSPI_Transmit(&hqspi, pbuf, 0xffff);
            break;        
        default:
            break;
    }
    return 0;
}

int bHalQSPI_Receive(uint8_t no, uint8_t *pbuf, uint16_t len)
{
    if(pbuf == NULL)
    {
        return -1;
    }
    switch(no)
    {
        case B_HAL_QSPI_1:
//            HAL_QSPI_Receive(&hqspi, pbuf, 0xffff);
            break;        
        default:
            break;
    }
    return 0;
}

int bHalQSPI_Command(uint8_t no,
                     uint32_t cmd, 
                     uint32_t addr, 
                     uint32_t addr_mode, 
                     uint32_t addr_size_bit, 
                     uint32_t data_mode, 
                     uint32_t nb_data)
{
//    QSPI_CommandTypeDef QSPI_Command;
    
//    QSPI_Command.Instruction = cmd;                 
//    QSPI_Command.Address = addr;                          
//    QSPI_Command.DummyCycles = 0;                  
//    QSPI_Command.InstructionMode = QSPI_INSTRUCTION_1_LINE;			
//    QSPI_Command.AddressMode = (addr_mode == 0) ? QSPI_ADDRESS_NONE : QSPI_ADDRESS_1_LINE; 
//    if(addr_size_bit == 24)
//    {
//        QSPI_Command.AddressSize = QSPI_ADDRESS_24_BITS;
//    }
//    else
//    {
//        QSPI_Command.AddressSize = QSPI_ADDRESS_32_BITS;
//    }			
//    QSPI_Command.DataMode = (data_mode == 0) ? QSPI_DATA_NONE : QSPI_DATA_1_LINE;             			
//    QSPI_Command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;       	
//    QSPI_Command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; 
//    QSPI_Command.DdrMode = QSPI_DDR_MODE_DISABLE;          
//    QSPI_Command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
//    QSPI_Command.NbData = nb_data;
    switch(no)
    {
        case B_HAL_QSPI_1:
//            HAL_QSPI_Command(&hqspi, &QSPI_Command, 0xffff);
            break;        
        default:
            break;
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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/


