/**
 *!
 * \file        b_drv_w25x.h
 * \version     v0.0.2
 * \date        2020/05/08
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
#ifndef __B_DRV_W25X_H__
#define __B_DRV_W25X_H__

#ifdef __cplusplus
 extern "C" {
#endif

/*Includes ----------------------------------------------*/
#include "b_drv_class_flash.h"

/** 
 * \addtogroup BABYOS
 * \{
 */


/** 
 * \addtogroup B_DRIVER
 * \{
 */

/** 
 * \addtogroup W25X
 * \{
 */

/** 
 * \defgroup W25X_Exported_TypesDefinitions
 * \{
 */


typedef struct
{
    uint8_t type;     //0: spi   1:qspi
    union
    {
        bHalQSPINumber_t qspi;
        bHalSPINumber_t spi;
    }xspi;
    bHalGPIOInstance_t cs;
}bW25X_HalIf_t;



typedef bDriverInterface_t bW25X_Driver_t;  

/**
 * \}
 */
   
/** 
 * \defgroup W25X_Exported_Defines
 * \{
 */
#define W25X_PAGE_SIZE 			      0X100		
#define W25X_SECTOR_SIZE 			  ((uint32_t)0X1000)	

#define W25X_WRITEENABLE		      0X06		
#define W25X_WRITEDISABLE		      0X04		
#define W25X_READSTATUSREG		      0X05		
#define W25X_WRITESTATUSREG		      0X01	
#define W25X_READDATA			      0X03		
#define W25X_FASTREADDATA		      0X0B		
#define W25X_FASTREADDUAL		      0X3B		 
#define W25X_PAGEPROGRAM		      0X02		
#define W25X_BLOCKERASE			      0XD8	
#define W25X_SECTORERASE		      0X20		
#define W25X_CHIPERASE			      0XC7		
#define W25X_POWERDOWN			      0XB9		
#define W25X_RELEASEPOWERDOWN		  0XAB		
#define W25X_DEVICEID			      0XAB		
#define W25X_MANUFACTDEVICEID		  0X90		
#define W25X_JEDECDEVICEID		      0X9F 
#define W25X_READUNIQUEID			  0X4B		

#define W25X_RESETENABLE              0X66
#define W25X_RESETCMD                 0X99
#define WIP_FLAG					  0X01		
#define WEL_FLAG					  0X02     
#define DUMMY_BYTE				      0XFF	
#define W25X_Enable4ByteAddr          0xB7

#define W25X_ReadStatusReg1		      0x05 
#define W25X_ReadStatusReg2		      0x35 
#define W25X_ReadStatusReg3		      0x15 


#define WinbondID		              0xEF

#define W25Q80 	        ((uint16_t)0X0013) 	
#define W25Q16 	        ((uint16_t)0X0014) 
#define W25Q32 	        ((uint16_t)0X0015) 
#define W25Q64 	        ((uint16_t)0X0016) 
#define W25Q128	        ((uint16_t)0X0017)
#define W25Q256         ((uint16_t)0X0018)

/**
 * \}
 */
   
/** 
 * \defgroup W25X_Exported_Macros
 * \{
 */


#define bW25X_ID_IS_VALID(n)    (n == W25Q80 || n == W25Q16 || n == W25Q32 || \
                                 n == W25Q64 || n == W25Q128 || n == W25Q256)

/**
 * \}
 */
   
/** 
 * \defgroup W25X_Exported_Variables
 * \{
 */
  
/**
 * \}
 */
   
/** 
 * \defgroup W25X_Exported_Functions
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

#ifdef __cplusplus
	}
#endif
 
#endif


/************************ Copyright (c) 2020 Bean *****END OF FILE****/



