/**
 *!
 * \file       berror.c
 * \brief      Error management
 * \version    v0.0.1
 * \date       2019/06/05
 * \author     notrynohigh
 *Last modified by notrynohigh 2019/06/05
 *Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "berror.h"

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_ERROR
 * \brief 错误管理单元
 * \{
 */

/** 
 * \defgroup BERROR_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BERROR_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BERROR_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BERROR_Private_Variables
 * \{
 */
static bErrorInfo_t bErrorRecord[BERROR_RCD_NUMBER];   
static bErrorInfo_t bErrorRecordPlus[BERROR_RCD_NUMBER];

static bErrorInfo_t *pErrorInfo = NULL;
static uint32_t bErrorTick = 0;
/**
 * \}
 */
   
/** 
 * \defgroup BERROR_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BERROR_Private_Functions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \addtogroup BERROR_Exported_Functions
 * \{
 */
 
/**
 * \brief 注册错误号至管理单元
 * \param err 错误号 
 * \param utc 错误发生的时间    
 * \param interval 接受两个相同错误最小间隔
 * \param level 错误等级
 *          \arg \ref BERROR_LEVEL_0 只上传一次
 *          \arg \ref BERROR_LEVEL_1 保证服务器接收到一次
 */ 
int bErrorInfoRegist(uint8_t err, uint32_t utc, uint32_t interval, uint32_t level)
{
    static uint8_t einit = 0;
    static uint8_t index = 0;
    uint32_t i = 0;
    uint32_t tick = 0;
    if(einit == 0)
    {
        for(i = 0;i < BERROR_RCD_NUMBER;i++)
        {
            bErrorRecord[i].err = INVALID_ERR;
            bErrorRecordPlus[i].err = INVALID_ERR;
        }
        einit = 1;
    }
    
    if(level == BERROR_LEVEL_0)
    {
        for(i = 0;i < BERROR_RCD_NUMBER;i++)
        {
            if(bErrorRecord[i].err == err)
            {
                tick = bErrorTick - bErrorRecord[i].s_tick;
                if(tick > bErrorRecord[i].d_tick)
                {
                    bErrorRecord[i].s_tick = 0;
                    bErrorRecord[i].utc = utc;
                }
                return 0;
            }
        }
        
        bErrorRecord[index].err = err;
		bErrorRecord[index].utc = utc;
        bErrorRecord[index].d_tick = interval;
        bErrorRecord[index].s_tick = 0;
        index = (index + 1) % BERROR_RCD_NUMBER;
    }
    else if(level == BERROR_LEVEL_1)
    {
        for(i = 0;i < BERROR_RCD_NUMBER;i++)
        {
            if(bErrorRecordPlus[i].err == err)
            {
                return 0;
            }
        }
        
        for(i = 0;i < BERROR_RCD_NUMBER;i++)
        {
            if(bErrorRecordPlus[i].err == INVALID_ERR)
            {
                bErrorRecordPlus[i].err = err;
                bErrorRecordPlus[i].d_tick = interval;
                bErrorRecordPlus[i].s_tick = 0;
				bErrorRecordPlus[i].utc = utc;
                break;
            }
        }
    }
    else
    {
        return -1;
    }
    return 0;
}    

/**
 * \brief 获取待上传的错误信息 
 * \note 调用这个函数的周期与Intervla时间有关
 * \param pinfo 指向bErrorInfo_t结构指针的指针
 * \retval 返回值
 *          \arg 0 成功
 *          \arg -1 失败
 */
int bErrorGetInfo(bErrorInfo_t **pinfo)
{
    uint32_t i = 0;
    uint32_t tick = 0;
    if(pinfo == NULL)
    {
        return -1;
    }
    bErrorTick++;
    for(i = 0;i < BERROR_RCD_NUMBER;i++)
    {
        if(bErrorRecord[i].err != INVALID_ERR && bErrorRecord[i].s_tick == 0)
        {
            *pinfo = &bErrorRecord[i];
            pErrorInfo = NULL;
            bErrorRecord[i].s_tick = bErrorTick;
            return 0;
        }
    }
    
    for(i = 0;i < BERROR_RCD_NUMBER;i++)
    {
        if(bErrorRecordPlus[i].err != INVALID_ERR)
        {
            tick = bErrorTick - bErrorRecordPlus[i].s_tick;
            if(bErrorRecordPlus[i].s_tick == 0
                || (tick > bErrorRecordPlus[i].d_tick))
            {
                *pinfo = &bErrorRecordPlus[i];
                pErrorInfo = &bErrorRecordPlus[i];
                bErrorRecordPlus[i].s_tick = bErrorTick;
                return 0;
            }
        }
    }   
    return -1;
}

/**
 * \brief 清除错误记录
 * \note 这个函数针对于错误等级1，收到服务器的回复后清除记录
 * \param perr 指向bErrorInfo_t结构的指针
 * \retval 返回值
 *          \arg 0 成功
 *          \arg -1 失败
 */
int bErrorClear(bErrorInfo_t *perr)
{
    if(pErrorInfo != NULL)
    {
		if(perr != NULL)
		{
			perr->err = pErrorInfo->err;
			perr->utc = pErrorInfo->utc;
		}
        pErrorInfo->err = INVALID_ERR;
        pErrorInfo = NULL;
        return 0;
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

