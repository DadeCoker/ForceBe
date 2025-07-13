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



#include "b_drv_w25x.h"
#include "b_drv_xpt2046.h"
#include "b_drv_ssd1289.h"
#include "b_drv_f8l10d.h"
#include "b_drv_fm25cl.h"
#include "b_drv_ili9341.h"
#include "b_drv_pcf8574.h"



//Flash IC
extern bDriverInterface_t   bW25X_Driver;
extern bDriverInterface_t   bFM25CL_Driver;

//LCD Controller
extern bDriverInterface_t   bSSD1289_Driver;
extern bDriverInterface_t   bILI9341_Driver;

//Touch
extern bDriverInterface_t   bXPT2046_Driver;

//LoRa Modules
extern bDriverInterface_t   bF8L10D_Driver;

//IO Expander
extern bDriverInterface_t   bPCF8574_Driver;

//Camera
extern bDriverInterface_t   bOV5640_Driver;

#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/

