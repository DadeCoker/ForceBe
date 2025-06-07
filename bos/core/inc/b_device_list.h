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

//B_DEVICE_REG(SSD1289, bSSD1289_Driver, "LCD")
//B_DEVICE_REG(XPT2046, bXPT2046_Driver, "TOUCH")
B_DEVICE_REG(SPIFLASH, SPIFlashDriver, "flash")

B_DEVICE_REG(SUART1, SUART_Driver1, "suart1")

B_DEVICE_REG(SUART2, SUART_Driver2, "suart2")

//B_DEVICE_REG(null, bNullDriver, "null")



#undef B_DEVICE_REG



/************************ Copyright (c) 2019 Bean *****END OF FILE****/




