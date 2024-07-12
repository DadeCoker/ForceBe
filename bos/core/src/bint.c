/**
 *!
 * \file       bint.c
 * \brief      Software interrupt
 * \version    v0.0.1
 * \date       2019/06/06
 * \author     notrynohigh
 *Last modified by notrynohigh 2019/06/06
 *Copyright (c) 2019 by NOTRYNOHIGH. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "bint.h"   
#include "bconfig.h"

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS_INT
 * \brief 中断管理
 * \{
 */

/** 
 * \defgroup BINT_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BINT_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BINT_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BINT_Private_Variables
 * \{
 */
static bIntInfo_t bIntInfo[bCFG_BINT_I_NUM];   
/**
 * \}
 */
   
/** 
 * \defgroup BINT_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BINT_Private_Functions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \addtogroup BINT_Exported_Functions
 * \{
 */
 
/**
 * \brief 注册中断
 * \note 这个中断管理适用于不需要及时响应的场景
 * \param number 序号相当于优先级
 * \param phandler 中断服务函数
 * \retval 返回值
 *          \arg 0 成功
 *          \arg -1 失败
 */ 
int bIntRegist(uint8_t number, pIntHandler_t phandler)
{
    static uint8_t init = 0;
    uint8_t i = 0;
    
    if(number >= bCFG_BINT_I_NUM || phandler == NULL)
    {
        return -1;
    }
    
    if(init == 0)
    {
        for(i = 0;i < bCFG_BINT_I_NUM;i++)
        {
            bIntInfo[i].enable = 0;
        }
        init = 1;
    }
    
    if(bIntInfo[number].enable == 1)
    {
        return -1;
    }
    
    bIntInfo[number].enable = 1;
    bIntInfo[number].trigger = 0;
    bIntInfo[number].phandler = phandler;
    return 0;
} 


/**
 * \brief 中断出发后调用
 * \note 一般在MCU的中断服务函数中调用
 * \param number 中断注册的序号
 * \retval 返回值
 *          \arg 0 成功
 *          \arg -1 失败
 */
int bIntTrigger(uint8_t number)
{
    if(number >= bCFG_BINT_I_NUM)
    {
        return -1;
    }  
    
    if(bIntInfo[number].enable == 0)
    {
        return -1;
    }
    
    bIntInfo[number].trigger = 1;
    return 0;
}

/**
 * \brief 中断检测并调用响应的执行函数
 * \note 放入while循环里调用
 * \retval 返回值
 *          \arg 0 成功
 *          \arg -1 失败
 */
int bIntCore()
{
    uint8_t i = 0;
    for(i = 0;i < bCFG_BINT_I_NUM;i++)
    {
        if(bIntInfo[i].trigger == 1 && bIntInfo[i].enable == 1)
        {
            bIntInfo[i].phandler();
            bIntInfo[i].trigger = 0;
        }
    }
    return 0;
}

/**
 * \brief 用来查询当前是否有未处理完的中断
 * \retval 返回值
 *          \arg 0 忙
 *          \arg -1 空闲
 */
int bIntBusy()
{
    uint8_t i = 0;
    for(i = 0;i < bCFG_BINT_I_NUM;i++)
    {
        if(bIntInfo[i].trigger == 1 && bIntInfo[i].enable == 1)
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

