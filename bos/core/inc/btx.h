/**
 *!
 * \file       btx.h
 * \brief      Transmit data
 * \version    v0.0.1
 * \date       2019/07/02
 * \author     notrynohigh
 * Last modified by notrynohigh 2019/07/02
 * Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
#ifndef __BTX_H__
#define __BTX_H__   
/*Includes ----------------------------------------------*/
#include <stdint.h> 

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_TX
 * \{
 */

/** 
 * \defgroup BTX_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
    uint8_t *ptxBUF;
    uint16_t buf_size;
    uint8_t state;
    uint16_t tx_len;
    uint8_t td_mode;
    volatile uint8_t td_flag;
    uint8_t dev_no;
    uint32_t timeout;
    uint8_t timeout_f;
    int fd;
}bTX_Info_t;
/**
 * \}
 */
   
/** 
 * \defgroup BTX_Exported_Defines
 * \{
 */
#define BTX_NULL		0
#define BTX_REQ			1
#define BTX_TXING		2
#define BTX_WAIT		3
#define BTX_END			4 

#ifndef NULL
#define NULL    ((void *)0)
#endif


#define BTX_F_SYN       0
#define BTX_F_ASYN      1

#define BTX_REQ_0       0
#define BTX_REQ_1       1

/**
 * \}
 */
   
/** 
 * \defgroup BTX_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BTX_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BTX_Exported_Functions
 * \{
 */
int bTXCore(void);
int bTXCpl(int no);
int bTXRegist(uint8_t *pbuf, uint32_t size, uint8_t td_mode, uint8_t dev_no); 
int bTXReq(int no, uint8_t *pbuf, uint16_t size, uint8_t flag);
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
/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/

