/**
 *!
 * \file        b_mod_battery.c
 * \version     v0.0.1
 * \date        2020/03/26
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
#include "modules/inc/b_mod_battery.h"

#include "hal/inc/b_hal.h"
#include "core/inc/b_section.h"

#if _BATTERY_ENABLE
/**
 * \addtogroup BABYOS
 * \{
 */

/**
 * \addtogroup MODULES
 * \{
 */

/**
 * \addtogroup BATTERY
 * \{
 */

/**
 * \defgroup BATTERY_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup BATTERY_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup BATTERY_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup BATTERY_Private_Variables
 * \{
 */
static uint8_t         bBatteryStatus  = BATTERY_STA_LOW;
static uint16_t        bBatteryVoltage = 0;
static pBatteryGetmV_t bpBatteryGetmV  = NULL;

/**
 * \}
 */

/**
 * \defgroup BATTERY_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup BATTERY_Private_Functions
 * \{
 */
static void _bBatteryCalculate()
{
    uint32_t        mv      = 0;
    uint16_t        min_tmp = 0xffff, max_tmp = 0, tmp = 0, i;
    static uint32_t tick = (uint32_t)(0 - MS2TICKS(_BATTERY_D_CYCLE));
    if (bHalGetSysTick() - tick >= MS2TICKS(_BATTERY_D_CYCLE))
    {
        tick = bHalGetSysTick();
        for (i = 0; i < 5; i++)
        {
            if (bpBatteryGetmV)
            {
                tmp = bpBatteryGetmV();
            }
            mv += tmp;
            if (tmp > max_tmp)
            {
                max_tmp = tmp;
            }
            if (tmp < min_tmp)
            {
                min_tmp = tmp;
            }
        }
        tmp = (mv - min_tmp - max_tmp) / 3;

        if (tmp >= _BATTERY_THRESHOLD)
        {
            bBatteryStatus = BATTERY_STA_NORMAL;
        }
        else
        {
            bBatteryStatus = BATTERY_STA_LOW;
        }
        bBatteryVoltage = tmp;
    }
}

BOS_REG_POLLING_FUNC(_bBatteryCalculate);

/**
 * \}
 */

/**
 * \addtogroup BATTERY_Exported_Functions
 * \{
 */
int bBatteryInit(pBatteryGetmV_t f)
{
    bpBatteryGetmV = f;
    return 0;
}

/**
 * \brief Get Status
 * \retval Battery Status
 *          \arg \ref bBAT_STA_NORMAL
 *          \arg \ref bBAT_STA_LOW
 */
uint8_t bBatGetStatus()
{
    return bBatteryStatus;
}

/**
 * \brief Get Voltage
 * \retval Battery voltage value (mv)
 */
uint16_t bBatGetVoltageValue()
{
    return bBatteryVoltage;
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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
