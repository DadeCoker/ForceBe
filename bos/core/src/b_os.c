/**
 *!
 * \file        b_os.c
 * \version     v0.0.1
 * \date        2020/02/11
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
#include "b_os.h"

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup BOS
 * \{
 */

/** 
 * \defgroup BOS_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */


/** 
 * \defgroup BOS_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BOS_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BOS_Private_Variables
 * \{
 */


/**
 * \}
 */
   
/** 
 * \defgroup BOS_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup BOS_Private_Functions
 * \{
 */


/**
 * \}
 */
   
/** 
 * \addtogroup BOS_Exported_Functions
 * \{
 */

/**
 * \brief Init 
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bInit()
{
    bHalInit();
    b_log("device number:%d\r\n", bDEV_MAX_NUM);
    return bDeviceInit();
}

/**
 * \brief  Call this function inside the while(1)
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bExec()
{
#if _BATTERY_ENABLE
    static uint32_t b_tick = 0;
    if(bHalGetTick() - b_tick > (MS2TICKS(_BATTERY_D_CYCLE)))
    {
        b_tick = bHalGetTick();
        bBatteryCore();
    }
#endif  

#if _ERROR_MANAGE_ENABLE
    static uint32_t e_tick = 0;
    if(bHalGetTick() - e_tick > (MS2TICKS(1000)))
    {
        e_tick = bHalGetTick();
        bErrorCore();
    }
#endif

#if _XMODEM128_ENABLE
    bXmodem128Timeout();
#endif

#if _YMODEM_ENABLE
    bYmodemTimeout();
#endif

#if _FLEXIBLEBUTTON_ENABLE
    static uint32_t fb_tick = 0;
    if(bHalGetTick() - fb_tick >= (_TICK_FRQ_HZ / FLEX_BTN_SCAN_FREQ_HZ))
    {
        fb_tick = bHalGetTick();
        flex_button_scan();
    }
#endif


#if _EVENT_MANAGE_ENABLE
    bEventCore();
#endif    
    
#if _TX_ENABLE
    bTX_Core();
#endif
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




