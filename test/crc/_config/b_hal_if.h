#ifndef __B_HAL_IF_H__
#define __B_HAL_IF_H__

//----------------------------------------------------------------
//     驱动文件的命名规则 b_drv_<驱动名小写>.c .h
//     每个驱动文件里有宏定义  #define DRIVER_NAME 驱动名大写
//     例如： spiflash驱动
//           驱动文件为 b_drv_spiflash.c .h
//           c文件里面定义宏 #define DRIVER_NAME SPIFLASH
//     驱动需要在此文件（b_hal_if.h）定义HAL层接口
//     #define HAL_<DRIVER_NAME>_IF
//     例如spiflash #define HAL_SPIFLASH_IF {具体的数据接口查看h文件}
//--------------------------------------------------------------------
//     如果有多个spiflash：
//     #define HAL_SPIFLASH_IF {第一个SPIFLASH},{第二个SPIFLASH}
//--------------------------------------------------------------------

#include "b_config.h"

// debug
#define HAL_LOG_UART B_HAL_UART_1

#define HAL_24CXX_IF                                                                               \
    {                                                                                              \
        .dev_addr = 0xa0, .is_simulation = 1, ._if.simulating_i2c.clk = {B_HAL_GPIOB, B_HAL_PIN6}, \
        ._if.simulating_i2c.sda = {B_HAL_GPIOB, B_HAL_PIN7},                                       \
    }

#define HAL_KEY_IF                                                                              \
    {B_HAL_GPIOC, B_HAL_PIN4, 0}, {B_HAL_GPIOB, B_HAL_PIN10, 0}, {B_HAL_GPIOC, B_HAL_PIN13, 0}, \
    {                                                                                           \
        B_HAL_GPIOA, B_HAL_PIN0, 0                                                              \
    }

#define HAL_DS18B20_IF          \
    {                           \
        B_HAL_GPIOA, B_HAL_PIN4 \
    }

#define HAL_ESP12F_IF B_HAL_UART_2

#define HAL_FM25CL_IF                                                                \
    {                                                                                \
        .is_simulation = 0, .cs = {B_HAL_GPIOB, B_HAL_PIN1}, ._if.spi = B_HAL_SPI_1, \
    }

#define HAL_ILI9320_IF                                       \
    {                                                        \
        .if_type = 2,                                        \
        ._if._spi._spi =                                     \
            {                                                \
                .is_simulation = 0,                          \
                .cs            = {B_HAL_GPIOD, B_HAL_PIN13}, \
                ._if.spi       = B_HAL_SPI_1,                \
            },                                               \
        ._if._spi.rs = {B_HAL_GPIOD, B_HAL_PIN15},           \
    }

#define HAL_ILI9341_IF HAL_ILI9320_IF

#define HAL_SSD1289_IF HAL_ILI9320_IF

#define HAL_ST7789_IF HAL_ILI9320_IF

#define HAL_LIS3DH_IF                                   \
    {                                                   \
        .is_spi   = 1,                                  \
        ._if._spi = {                                   \
            .is_simulation = 0,                         \
            .cs            = {B_HAL_GPIOB, B_HAL_PIN1}, \
            ._if.spi       = B_HAL_SPI_1,               \
        },                                              \
    }

#define HAL_MATRIXKEYS_IF                                                                          \
    {                                                                                              \
        .rows = 4, .columns = 4, .row_io = {{B_HAL_GPIOE, B_HAL_PIN8}, {B_HAL_GPIOE, B_HAL_PIN9}}, \
        .column_io = {{B_HAL_GPIOE, B_HAL_PIN10}, {B_HAL_GPIOE, B_HAL_PIN11}},                     \
    }

#define HAL_MCUFLASH_IF \
    {                   \
        0               \
    }

#define HAL_OLED_IF                                              \
    {                                                            \
        .is_spi   = 0,                                           \
        ._if._i2c = {                                            \
            .dev_addr               = 0x78,                      \
            .is_simulation          = 1,                         \
            ._if.simulating_i2c.clk = {B_HAL_GPIOB, B_HAL_PIN0}, \
            ._if.simulating_i2c.sda = {B_HAL_GPIOB, B_HAL_PIN1}, \
        },                                                       \
    }

#define HAL_PCF8574_IF                                                                             \
    {                                                                                              \
        .dev_addr = 0xa0, .is_simulation = 1, ._if.simulating_i2c.clk = {B_HAL_GPIOB, B_HAL_PIN6}, \
        ._if.simulating_i2c.sda = {B_HAL_GPIOB, B_HAL_PIN7},                                       \
    }

#define HAL_SD_IF                                        \
    {                                                    \
        .is_spi   = 1,                                   \
        ._if._spi = {                                    \
            .is_simulation = 0,                          \
            .cs            = {B_HAL_GPIOD, B_HAL_PIN11}, \
            ._if.spi       = B_HAL_SPI_1,                \
        },                                               \
    }

#define HAL_SPIFLASH_IF                                 \
    {                                                   \
        .is_spi   = 1,                                  \
        ._if._spi = {                                   \
            .is_simulation = 0,                         \
            .cs            = {B_HAL_GPIOB, B_HAL_PIN9}, \
            ._if.spi       = B_HAL_SPI_1,               \
        },                                              \
    }

#define HAL_TESTFLASH_IF             \
    {                                \
        .e_size = 4096, .w_size = 1, \
    }

#define HAL_XPT2046_IF                                                               \
    {                                                                                \
        ._if.spi = B_HAL_SPI_3, .cs = {B_HAL_GPIOC, B_HAL_PIN9}, .is_simulation = 0, \
    }

#endif
