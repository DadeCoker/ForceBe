#ifndef __B_HAL_IF_H__
#define __B_HAL_IF_H__

#include "b_config.h"
//----------------------------------------------------------------

// debug
#define HAL_LOG_UART B_HAL_UART_1

#define HAL_24CXX_IF                                                                               \
    {                                                                                              \
        .dev_addr = 0xa0, .is_simulation = 1, ._if.simulating_i2c.clk = {B_HAL_GPIOB, B_HAL_PIN6}, \
        ._if.simulating_i2c.sda = {B_HAL_GPIOB, B_HAL_PIN7},                                       \
    }

#define HAL_DS18B20_IF          \
    {                           \
        B_HAL_GPIOA, B_HAL_PIN4 \
    }

#define HAL_TESTFLASH_IF            \
    {                               \
        .e_size = 4096, .w_size = 1 \
    }

#endif
