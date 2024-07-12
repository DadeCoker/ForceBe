/**
 *!
 * \file       bfd.h
 * \brief      FD management
 * \version    v0.0.1
 * \date       2019/05/07
 * \author     notrynohigh
 * Last modified by notrynohigh 2019/05/07
 * Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
#ifndef __BFD_H__
#define __BFD_H__ 
/*Includes ----------------------------------------------*/
#include <stdint.h> 
#include "bdev/bdev.h"
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_FD
 * \{
 */

/** 
 * \defgroup BFD_Exported_TypesDefBFDions
 * \{
 */

typedef struct
{
    uint8_t number;
    uint8_t flag;
    uint8_t status;
    uint32_t lseek;
}bFD_Info_t;


/**
 * \}
 */
   
/** 
 * \defgroup BFD_Exported_Defines
 * \{
 */
#define BFD_TYPE_MODULE         0U
#define BFD_TYPE_DEVICE         1U


typedef enum
{
#ifdef BDEV_FLASH
    BFD_STYPE_FLASH,
#endif 
#ifdef BDEV_CC    
    BFD_STYPE_CC,
#endif    
#ifdef BDEV_GSENSOR      
    BFD_STYPE_GSENSOR,
#endif    
    BFD_STYPE_NUMBER
}BFD_TypeEnum_t;


#define BFD_FLAG_R              0U
#define BFD_FLAG_W              1U
#define BFD_FLAG_RW             2U

#define BFD_STA_NULL            0U
#define BFD_STA_OPEN            1U



#define IS_VALID_TYPE(n)        (n == BFD_TYPE_MODULE || n == BFD_TYPE_DEVICE)

#define IS_VALID_STYPE(n)       ((n) < BFD_STYPE_NUMBER)

#define IS_VALID_FLAG(n)        (n == BFD_FLAG_R || n == BFD_FLAG_W || n == BFD_FLAG_RW)
/**
 * \}
 */
   
/** 
 * \defgroup BFD_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BFD_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BFD_Exported_Functions
 * \{
 */
int bFD_Open(uint8_t t, uint8_t sub_t, uint8_t no, uint8_t flag);
int bFD_Ctl(int fd, uint8_t cmd, void *param);
int bFD_Read(int fd, uint8_t *pdata, uint16_t len);
int bFD_Write(int fd, uint8_t *pdata, uint16_t len);
int bFD_Lseek(int fd, uint32_t off);
int bFD_Close(int fd);
int bFD_GetInfo(int fd, bFD_Info_t *pinfo);
int bFD_Busy(void);
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






