/**
 *!
 * \file        b_device_list.h
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

/**
           for example !

/////B_DEVICE_REG(dev, driver_interface, desc);
 */

B_DEVICE_REG(SSD1289, bSSD1289_Driver, "lcd")
B_DEVICE_REG(XPT2046, bXPT2046_Driver, "touch")
B_DEVICE_REG(SPIFLASH, bSPIFLASH_Driver[0], "flash")
B_DEVICE_REG(_24C02, b24CXX_Driver[0], "24c02")
B_DEVICE_REG(SD, bSD_Driver, "sd card")
B_DEVICE_REG(DS18B20, bDS18B20_Driver, "temp")
B_DEVICE_REG(MCUFLASH, bMCUFLASH_Driver, "st")

// If there is no device to register, uncomment the null device
B_DEVICE_REG(null, bNullDriver, "null")

#undef B_DEVICE_REG

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
