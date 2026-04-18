/**
 *!
 * \file        b_mod_qpn.c
 * \version     v0.0.2
 * \date        2020/05/09
 * \author      Alex_Shen(bestnike@126.com)
 * \note        modified: 2020.05.10 notrynohigh v0.0.2
 *******************************************************************************
 * @attention
 * 
 * Copyright (c) 2020 Alex_Shen
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
#include "b_mod_qpn.h"
#include "b_utils.h"
#if _QPN_ENABLE
/*Includes AO -------------------------------------------*/
#include "blinky.h"    //blinky  Examples/qpn/
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup MODULES
 * \{
 */

/** 
 * \addtogroup QPN
 * \{
 */


/** 
 * \defgroup QPN_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup QPN_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup QPN_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup QPN_Private_Variables
 * \{
 */
/* Local-scope objects -----------------------------------------------------*/
QActiveCB const Q_ROM QF_active[] = {
    { (QActive *)0,           (QEvt *)0,        0U                      },
    //add more ...
    { (QActive *)&AO_Blinky, l_blinkyQSto, Q_DIM(l_blinkyQSto) }   //Blinky Examples/qpn/
};

static const AO_InitFunc_t AO_InitFuncTable[] = {
    //add more ...
    Blinky_ctor,                  //Blinky Examples/qpn/
    NULL,
};

static bPollingFunc_t QPN_CorePollFunc = {
    .pPollingFunction = NULL,
};

/**
 * \}
 */
   
/** 
 * \defgroup QPN_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup QPN_Private_Functions
 * \{
 */
static void _bQPN_Run() 
{
    static uint32_t tick = 0;
    if(bUtilGetTick() - tick >= QPN_PER_SEC)
    {
        tick = bUtilGetTick();
        QF_tickXISR(0U); /* process time events for rate 0 */
        QF_run(); /* transfer control to QF-nano */
    }
}

/**
 * \}
 */
   
/** 
 * \addtogroup QPN_Exported_Functions
 * \{
 */
void bQPN_Init()
{
    int i = 0;
    while(AO_InitFuncTable[i])
    {
        AO_InitFuncTable[i]();
        i++;
    }
    if(QPN_CorePollFunc.pPollingFunction == NULL)
    {
        QPN_CorePollFunc.pPollingFunction = _bQPN_Run;
        bRegistPollingFunc(&QPN_CorePollFunc);
    }    
	QF_init(Q_DIM(QF_active));      /* initialize the QF-nano framework */
	QF_Ready_run();
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


/**
 * \}
 */
#endif


/************************ Copyright (c) 2020 Alex_Shen *****END OF FILE****/


