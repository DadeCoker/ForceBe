/**
 *!
 * \file        b_fm25cl.c
 * \version     v0.0.1
 * \date        2020/02/05
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
   
/*Includes ----------------------------------------------*/
#include "b_fm25cl.h"
/** 
 * \addtogroup B_DRIVER
 * \{
 */

/** 
 * \addtogroup FM25CL
 * \{
 */

/** 
 * \defgroup FM25CL_Private_TypesDefinitions
 * \{
 */
 

/**
 * \}
 */
   
/** 
 * \defgroup FM25CL_Private_Defines
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup FM25CL_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup FM25CL_Private_Variables
 * \{
 */
	

/**
 * \}
 */
   
/** 
 * \defgroup FM25CL_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup FM25CL_Private_Functions
 * \{
 */

static void _FM25_WR_Enable(bFM25CL_Driver_t *pdrv)
{	
    ((bFM25CLPrivate_t *)pdrv->_private)->pCS_Control(0);
    ((bFM25CLPrivate_t *)pdrv->_private)->pSPI_ReadWriteByte(SFC_WREN);  
	((bFM25CLPrivate_t *)pdrv->_private)->pCS_Control(1);
}	

static void _FM25_WR_Lock(bFM25CL_Driver_t *pdrv) 
{	
    ((bFM25CLPrivate_t *)pdrv->_private)->pCS_Control(0);
    ((bFM25CLPrivate_t *)pdrv->_private)->pSPI_ReadWriteByte(SFC_WRDI);  
	((bFM25CLPrivate_t *)pdrv->_private)->pCS_Control(1);    
}


/**************************************************************************************************driver interface*****/

static int _FM25_ReadBuff(uint32_t addr, uint8_t *pDat, uint16_t len)
{	
	int i; 
    
    bFM25CL_Driver_t *pdrv;
    bFM25CLPrivate_t *_private;
    if(0 > bDeviceGetCurrentDrv(&pdrv))
    {
        return -1;
    } 
    _private = pdrv->_private;
	_private->pCS_Control(0);
	_private->pSPI_ReadWriteByte(SFC_READ);             
	_private->pSPI_ReadWriteByte((uint8_t)(addr >> 8));   
	_private->pSPI_ReadWriteByte((uint8_t)(addr >> 0));  
    
	for(i = 0;i < len;i++)
    {
        pDat[i] = _private->pSPI_ReadWriteByte(0);
    }
	_private->pCS_Control(1); 
    return len;
}   


static int _FM25_WritBuff(uint32_t addr, uint8_t* pdat,uint16_t len)
{	
	int i; 
    bFM25CL_Driver_t *pdrv;
    bFM25CLPrivate_t *_private;
    if(0 > bDeviceGetCurrentDrv(&pdrv))
    {
        return -1;
    } 
    _private = pdrv->_private;
    
	_FM25_WR_Enable(pdrv);           
	//-----------------------------------------------------------
	_private->pCS_Control(0);
	_private->pSPI_ReadWriteByte(SFC_WRITE);        
	_private->pSPI_ReadWriteByte((unsigned char)(addr>>8));
	_private->pSPI_ReadWriteByte((unsigned char)(addr>>0)); 
	for(i = 0;i < len;i++)
    {
        _private->pSPI_ReadWriteByte(pdat[i]);
    }
	_private->pCS_Control(1);
	//-----------------------------------------------------------
	_FM25_WR_Lock(pdrv);    
    return len;
} 


/**
 * \}
 */
   
/** 
 * \addtogroup FM25CL_Exported_Functions
 * \{
 */
int bFM25CL_Init(bFM25CL_Driver_t *pdrv)
{  
    pdrv->close = NULL;
    pdrv->read = _FM25_ReadBuff;
    pdrv->ctl = NULL;
    pdrv->open = NULL;
    pdrv->write = _FM25_WritBuff;
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

/************************ Copyright (c) 2019 Bean *****END OF FILE****/

