/**
 *!
 * \file        b_hal_gpio.c
 * \version     v0.0.1
 * \date        2020/03/25
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SGPIOL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
   
/*Includes ----------------------------------------------*/
#include "b_hal.h" 
/** 
 * \addtogroup B_HAL
 * \{
 */

/** 
 * \addtogroup GPIO
 * \{
 */

/** 
 * \defgroup GPIO_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup GPIO_Private_Defines
 * \{
 */
bSECTION_DEF_FLASH(b_hal_gpio, bHalGPIO_EXTI_t);   
/**
 * \}
 */
   
/** 
 * \defgroup GPIO_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup GPIO_Private_Variables
 * \{
 */

                                       
/**
 * \}
 */
   
/** 
 * \defgroup GPIO_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup GPIO_Private_Functions
 * \{
 */


/**
 * \}
 */
   
/** 
 * \addtogroup GPIO_Exported_Functions
 * \{
 */
/**
 * \brief Configure GPIO, Dont Initialize GPIO with this function. It's only used to switch in/out mode.
 * \param port the port number
 * \param pin the pin number 
 * \param mode gpio mode
 *         \arg B_HAL_GPIO_OUTPUT output mode
 *         \arg B_HAL_GPIO_INPUT  input mode
 * \param pull gpio pull
 *         \arg B_HAL_GPIO_NOPULL no-pull mode
 *         \arg B_HAL_GPIO_PULLUP pull-up mode 
 *         \arg B_HAL_GPIO_PULLDOWN pull-down mode 
 */ 
void bHalGPIO_Config(bHalGPIOPort_t port, bHalGPIOPin_t pin, uint8_t mode, uint8_t pull)
{

}


void bHalGPIO_WritePin(bHalGPIOPort_t port, bHalGPIOPin_t pin, uint8_t s)
{

}

uint8_t bHalGPIO_ReadPin(bHalGPIOPort_t port, bHalGPIOPin_t pin)
{

}

void bHalGPIO_Write(bHalGPIOPort_t port, uint16_t dat)
{

}

uint16_t bHalGPIO_Read(bHalGPIOPort_t port)
{

}



/**
 * \brief This function handles EXTI interrupts.
 * \param pin Specifies the pins connected EXTI line \ref bHalGPIOPin_t
 */ 
void bHalGPIO_EXTI_IRQHandler(bHalGPIOPin_t pin)
{
    if(pin >= B_HAL_PINAll)
    {
        return;
    }
    bSECTION_FOR_EACH(b_hal_gpio, bHalGPIO_EXTI_t, ptmp) 
    {
        if(ptmp->pin == pin)
        {
            if(ptmp->handler)
            {
                ptmp->handler();
                break;
            }
        }
    }
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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/












