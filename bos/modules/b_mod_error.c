/**
 *!
 * \file        b_mod_error.c
 * \version     v1.0.1
 * \date        2020/02/24
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
#include "modules/inc/b_mod_error.h"

#include "b_hal.h"
#include "core/inc/b_section.h"

#if _ERROR_MANAGE_ENABLE
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup ERROR
 * \{
 */

/**
 * \defgroup ERROR_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup ERROR_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup ERROR_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup ERROR_Private_Variables
 * \{
 */
static bErrorInfo_t bErrorRecord[_ERROR_Q_LENGTH];
static pecb         bFcb = NULL;
/**
 * \}
 */

/**
 * \defgroup ERROR_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup ERROR_Private_Functions
 * \{
 */

/**
 * \brief Find out an error that should be handled
 */
static void _bErrorCore()
{
    uint32_t i    = 0;
    uint32_t tick = 0;
    tick          = bUtilGetTick();
    for (i = 0; i < _ERROR_Q_LENGTH; i++)
    {
        if (bErrorRecord[i].err == INVALID_ERR)
        {
            continue;
        }
        if (bErrorRecord[i].s_tick == 0 ||
            (bErrorRecord[i].type == BERROR_LEVEL_1 && bErrorRecord[i].ack == 0 &&
             (tick - bErrorRecord[i].s_tick > bErrorRecord[i].d_tick)))
        {
            bErrorRecord[i].s_tick = tick;
            if (bFcb != NULL)
            {
                bFcb(&bErrorRecord[i]);
            }
        }
    }
}

BOS_REG_POLLING_FUNC(_bErrorCore);

/**
 * \}
 */

/**
 * \addtogroup ERROR_Exported_Functions
 * \{
 */

int bErrorInit(pecb cb)
{
    int i = 0;
    if (cb == NULL)
    {
        return -1;
    }
    bFcb = cb;
    for (i = 0; i < _ERROR_Q_LENGTH; i++)
    {
        bErrorRecord[i].err = INVALID_ERR;
    }
    return 0;
}

/**
 * \brief Register an error
 * \param err Error number
 * \param utc Current time
 * \param interval interval time (s)
 * \param level
 *          \arg \ref BERROR_LEVEL_0
 *          \arg \ref BERROR_LEVEL_1
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bErrorRegist(uint8_t err, uint32_t utc, uint32_t interval, uint32_t level)
{
    static uint8_t index = 0;
    uint32_t       i     = 0;
    uint32_t       tick  = 0;

    if (level != BERROR_LEVEL_0 && level != BERROR_LEVEL_1)
    {
        return -1;
    }

    for (i = 0; i < _ERROR_Q_LENGTH; i++)
    {
        if (bErrorRecord[i].err == err)
        {
            tick = bUtilGetTick() - bErrorRecord[i].s_tick;
            if (tick > bErrorRecord[i].d_tick)
            {
                bErrorRecord[i].s_tick = 0;
                bErrorRecord[i].utc    = utc;
                bErrorRecord[i].type   = level;
                bErrorRecord[i].ack    = 0;
            }
            break;
        }
    }
    if (i >= _ERROR_Q_LENGTH)
    {
        bErrorRecord[index].err    = err;
        bErrorRecord[index].utc    = utc;
        bErrorRecord[index].d_tick = MS2TICKS(interval * 1000);
        bErrorRecord[index].s_tick = 0;
        bErrorRecord[index].type   = level;
        bErrorRecord[index].ack    = 0;
        index                      = (index + 1) % _ERROR_Q_LENGTH;
    }
    return 0;
}

/**
 * \brief Remove error from queue
 * \param e_no Error number
 * \retval Result
 *          \arg 0  OK
 *          \arg -1 ERR
 */
int bErrorClear(uint8_t e_no)
{
    int i;
    if (e_no == INVALID_ERR)
    {
        return -1;
    }
    for (i = 0; i < _ERROR_Q_LENGTH; i++)
    {
        if (bErrorRecord[i].err == e_no)
        {
            bErrorRecord[i].err = INVALID_ERR;
        }
    }
    return 0;
}

/**
 * \brief Check that error is in the bErrorRecordLx \ref bErrorRecordL0 bErrorRecordL1
 * \param e_no Error number
 * \retval Result
 *          \arg 0  Exist
 *          \arg -1 None
 */
int bErrorIS_Exist(uint8_t e_no)
{
    int i;
    if (e_no == INVALID_ERR)
    {
        return -1;
    }
    for (i = 0; i < _ERROR_Q_LENGTH; i++)
    {
        if (bErrorRecord[i].err == e_no)
        {
            return 0;
        }
    }
    return -1;
}

int bErrorIS_Empty()
{
    int i = 0;
    for (i = 0; i < _ERROR_Q_LENGTH; i++)
    {
        if (bErrorRecord[i].err != INVALID_ERR)
        {
            return -1;
        }
    }
    return 0;
}

int bErrorAck(uint8_t e_no)
{
    int i;
    for (i = 0; i < _ERROR_Q_LENGTH; i++)
    {
        if (bErrorRecord[i].err == e_no)
        {
            bErrorRecord[i].ack = 1;
        }
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

/**
 * \}
 */
#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
