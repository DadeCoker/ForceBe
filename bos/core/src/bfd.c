/**
 *!
 * \file       bfd.c
 * \brief      FD management
 * \version    v0.0.1
 * \date       2019/05/07
 * \author     notrynohigh
 *Last modified by notrynohigh 2019/05/07
 *Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "bfd.h"
#include "bconfig.h"
#include "string.h"

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_FD
 * \brief 句柄管理单元
 * \{
 */

/** 
 * \defgroup BFD_Private_TypesDefBFDions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BFD_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BFD_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BFD_Private_Variables
 * \{
 */

static bFD_Info_t bFD_Info[bCFG_BFD_I_NUM];


/**
 * \}
 */
   
/** 
 * \defgroup BFD_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BFD_Private_Functions
 * \{
 */
static int _bFD_GetNew(uint8_t no, uint8_t flag)
{
    int i = 0;
    int fd = -1;
    static uint8_t init = 0;
    if(init == 0)
    {
        init = 1;
        for(i = 0;i < bCFG_BFD_I_NUM;i++)
        {
            bFD_Info[i].status = BFD_STA_NULL;
        }
    }
    
    if(IS_VALID_TYPE(t) && IS_VALID_STYPE(sub_t) && IS_VALID_FLAG(flag))
    {
        ;
    }
    else
    {
        return -1;
    }

    for(i = 0;i < bCFG_BFD_I_NUM;i++)
    {
        if(bFD_Info[i].status == BFD_STA_OPEN)
        {
            if(no == bFD_Info[i].number && sub_t == bFD_Info[i].sub_type && t == bFD_Info[i].type)
            {
                return -1;
            }
        }
    }

    
    for(i = 0;i < bCFG_BFD_I_NUM;i++)
    {
        if(bFD_Info[i].status == BFD_STA_NULL)
        {
            bFD_Info[i].flag = flag;
            bFD_Info[i].number = no;
            bFD_Info[i].status = BFD_STA_OPEN;
            bFD_Info[i].sub_type = sub_t;
            bFD_Info[i].type = t;
            bFD_Info[i].lseek = 0;
            fd = i;
            break;
        }
    }
    return fd;
} 

static int _bFD_Delete(int fd)
{
    if(fd < 0)
    {
        return -1;
    }
    if(bFD_Info[fd].status == BFD_STA_NULL)
    {
        return -1;
    }
    bFD_Info[fd].status = BFD_STA_NULL;
    return 0;
}

/**
 * \}
 */
   
/** 
 * \addtogroup BFD_Exported_Functions
 * \{
 */



int bFD_Open(uint8_t no, uint8_t flag)
{
    int fd = -1;
    if(IS_VALID_FLAG(flag) == 0)
    {
        return -1;
    }
    
    fd = _bFD_GetNew(no, flag);
    if(fd < 0)
    {
        return -1;
    }
    
    if(t == BFD_TYPE_MODULE)
    {
        return fd;
    }
    
    
    if(bOpenTable[sub_t](no) < 0)
    {
        _bFD_Delete(fd);
        fd = -1;
    }
    return fd;
}



int bFD_Read(int fd, uint8_t *pdata, uint16_t len)
{
    int retval = -1;
    if(fd < 0 || fd >= bCFG_BFD_I_NUM || pdata == NULL)
    {
        return -1;
    }
    
    if(bFD_Info[fd].flag == BFD_FLAG_W || bFD_Info[fd].status == BFD_STA_NULL)
    {
        return -1;
    }
    
    if((IS_VALID_STYPE(bFD_Info[fd].sub_type) == 0) || bFD_Info[fd].type == BFD_TYPE_MODULE)
    {
        return -1;
    }
    
    retval = bReadTable[bFD_Info[fd].sub_type](bFD_Info[fd].number, bFD_Info[fd].lseek, pdata, len);
    if(retval >= 0)
    {
        bFD_Info[fd].lseek += len;
    }
    return retval;
}


int bFD_Write(int fd, uint8_t *pdata, uint16_t len)
{
    int retval = -1;
    if(fd < 0 || fd >= bCFG_BFD_I_NUM || pdata == NULL)
    {
        return -1;
    }
    
    if(bFD_Info[fd].flag == BFD_FLAG_R || bFD_Info[fd].status == BFD_STA_NULL)
    {
        return -1;
    }
    
    if((IS_VALID_STYPE(bFD_Info[fd].sub_type) == 0) || bFD_Info[fd].type == BFD_TYPE_MODULE)
    {
        return -1;
    }
    
    retval = bWriteTable[bFD_Info[fd].sub_type](bFD_Info[fd].number, bFD_Info[fd].lseek, pdata, len);
    if(retval >= 0)
    {
        bFD_Info[fd].lseek += len;
    }
    return retval;
}


int bFD_Lseek(int fd, uint32_t off)
{
    if(fd < 0 || fd >= bCFG_BFD_I_NUM)
    {
        return -1;
    }
    
    if(bFD_Info[fd].status == BFD_STA_NULL)
    {
        return -1;
    }
    bFD_Info[fd].lseek = off;
    return 0;
}


int bFD_Ctl(int fd, uint8_t cmd, void *param)
{
    if(fd < 0 || fd >= bCFG_BFD_I_NUM)
    {
        return -1;
    }
    
    if(bFD_Info[fd].status == BFD_STA_NULL)
    {
        return -1;
    }
        
    if((IS_VALID_STYPE(bFD_Info[fd].sub_type) == 0) || bFD_Info[fd].type == BFD_TYPE_MODULE)
    {
        return -1;
    }
    
    return bCtlTable[bFD_Info[fd].sub_type](bFD_Info[fd].number, cmd, param);
}


int bFD_Close(int fd)
{
    if(fd < 0 || fd >= bCFG_BFD_I_NUM)
    {
        return -1;
    }
    
    if(bFD_Info[fd].status == BFD_STA_NULL)
    {
        return -1;
    }

    if((IS_VALID_STYPE(bFD_Info[fd].sub_type)) && bFD_Info[fd].type == BFD_TYPE_DEVICE)
    {
        bCloseTable[bFD_Info[fd].sub_type](bFD_Info[fd].number);
    }
    
    return _bFD_Delete(fd);
}


int bFD_GetInfo(int fd, bFD_Info_t *pinfo)
{
    if(fd < 0 || fd >= bCFG_BFD_I_NUM || pinfo == NULL)
    {
        return -1;
    }
    memcpy(pinfo, &bFD_Info[fd], sizeof(bFD_Info_t));
    return 0;
}

int bFD_Busy()
{
    int i = 0;
    for(i = 0;i < bCFG_BFD_I_NUM;i++)
    {
        if(bFD_Info[i].status == BFD_STA_OPEN)
        {
            return 0;
        }
    }
    return -1;
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
  
/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/




