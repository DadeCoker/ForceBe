/**
 *!
 * \file        b_driver.h
 * \version     v0.0.1
 * \date        2019/06/05
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 * 
 * Copyright (c) 2019 Bean
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
#ifndef __B_DRIVER_H__
#define __B_DRIVER_H__



#include "b_suart.h"
#include "b_w25x.h"
#include "b_xpt2046.h"
#include "b_ssd1289.h"
#include "b_f8l10d.h"


#define NEW_SUART_DRV(name, hal)        SUART_Driver_t name = {.init = SUART_Init,._private = &hal}

#define NEW_W25X_DRV(name, hal)         bW25X_Driver_t name = {.init = bW25X_Init,._private = &hal}

#define NEW_XPT2046_DRV(name, hal)      bXPT2046_Driver_t name = {.init = bXPT2046_Init,._private = &hal}

#define NEW_SSD1289_DRV(name, hal)      bSSD1289_Driver_t name = {.init = bSSD1289_Init,._private = &hal}

#define NEW_F8L10D_DRV(name, hal)       bF8L10D_Driver_t name = {.init = bF8L10D_Init,._private = &hal}





extern bF8L10D_Driver_t F8l10dDriver;


#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/

