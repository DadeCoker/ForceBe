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
typedef enum
{
    B_DRIVER_NULL = 0,
    B_DRIVER_24CXX,
    B_DRIVER_DS18B20,
    B_DRIVER_ESP12F,
    B_DRIVER_FM25CL,
    B_DRIVER_ILI9320,
    B_DRIVER_ILI9341,
    B_DRIVER_KEY,
    B_DRIVER_LIS3DH,
    B_DRIVER_MATRIXKEYS,
    B_DRIVER_MCUFLASH,
    B_DRIVER_OLED,
    B_DRIVER_PCF8574,
    B_DRIVER_SD,
    B_DRIVER_SPIFLASH,
    B_DRIVER_SSD1289,
    B_DRIVER_ST7789,
    B_DRIVER_TESTFLASH,
    B_DRIVER_XPT2046,
    B_DRIVER_NUMBER
} bDriverNumber_t;
*/
/**
    B_DEVICE_REG(dev_1, bDriverNumber_t, "description")
    .....
    B_DEVICE_REG(dev_n, bDriverNumber_t, "description")
 */

B_DEVICE_REG(bTESTFLASH, B_DRIVER_TESTFLASH, "testflash")

#undef B_DEVICE_REG

/************************ Copyright (c) 2019 Bean *****END OF FILE****/
