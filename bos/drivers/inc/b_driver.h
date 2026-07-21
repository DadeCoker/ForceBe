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

#include <stdint.h>

typedef struct bDriverIf
{
    int status;
    int (*open)(struct bDriverIf *pdrv);
    int (*close)(struct bDriverIf *pdrv);
    int (*ctl)(struct bDriverIf *pdrv, uint8_t cmd, void *param);
    int (*write)(struct bDriverIf *pdrv, uint32_t offset, uint8_t *pbuf, uint16_t len);
    int (*read)(struct bDriverIf *pdrv, uint32_t offset, uint8_t *pbuf, uint16_t len);
    void *_hal_if;
    union
    {
        uint32_t v;
        void *_p;
    }_private;
}bDriverInterface_t; 

typedef int (*pbDriverInit_t)(void);



#define bDRIVER_USED __attribute__((used))
#define bDRIVER_SECTION(x) __attribute__((section(".rodata.bdriver_init" x)))
#define bDRIVER_INIT_START(func)    bDRIVER_USED const pbDriverInit_t _bDriverInitStart bDRIVER_SECTION("0.end") = NULL
#define bDRIVER_INIT(func)          bDRIVER_USED const pbDriverInit_t bdrv_init##func bDRIVER_SECTION("1") = func
#define bDRIVER_INIT_END(func)      bDRIVER_USED const pbDriverInit_t _bDriverInitEnd bDRIVER_SECTION("1.end") = NULL
extern const pbDriverInit_t _bDriverInitStart;    
extern const pbDriverInit_t _bDriverInitEnd;      
#define bDRV_INIT_BASE      (&_bDriverInitStart + 1)
    
#define bDRV_GET_HALIF(name, type, pdrv)         type *name = (type *)(pdrv->_hal_if)

//Flash IC
extern bDriverInterface_t   bW25X_Driver[];
extern bDriverInterface_t   bFM25CL_Driver;

//LCD Controller
extern bDriverInterface_t   bSSD1289_Driver;
extern bDriverInterface_t   bILI9341_Driver;
extern bDriverInterface_t   bILI9320_Driver;
extern bDriverInterface_t   bOLED_Driver;

//Touch
extern bDriverInterface_t   bXPT2046_Driver;

//LoRa Modules
extern bDriverInterface_t   bF8L10D_Driver;

//IO Expander
extern bDriverInterface_t   bPCF8574_Driver;

//Camera
extern bDriverInterface_t   bOV5640_Driver;

//EEPROM
extern bDriverInterface_t   b24CXX_Driver;



#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/

