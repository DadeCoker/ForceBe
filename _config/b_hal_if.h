#ifndef __B_HAL_IF_H__
#define __B_HAL_IF_H__

// debug
#define HAL_LOG_UART B_HAL_UART_1

// button
#if _FLEXIBLEBUTTON_ENABLE
#define HAL_B_BUTTON_GPIO             \
    {                                 \
        {B_HAL_GPIOA, B_HAL_PIN0, 0}, \
    }
#endif

// 24cxx
#define HAL_24CXX_IF         \
    {                        \
        {B_HAL_I2C_1, 0xa0}, \
    }

// spiflash
#define HAL_SPIFLASH_QSPI_EN 0
#define HAL_SPIFLASH_TOTAL_NUMBER 1
#define HAL_SPIFLASH_IF                                         \
    {                                                           \
        {._if.spi = B_HAL_SPI_1, {B_HAL_GPIOB, B_HAL_PIN9}, 1}, \
    }

// xpt2046
#define HAL_XPT2046_IF              \
    {                               \
        B_HAL_SPI_3,                \
        {                           \
            B_HAL_GPIOC, B_HAL_PIN9 \
        }                           \
    }

// sd
#define HAL_SD_IF                    \
    {                                \
        B_HAL_SPI_1,                 \
        {                            \
            B_HAL_GPIOD, B_HAL_PIN11 \
        }                            \
    }

// lcd
#define HAL_SSD1289_IF                               \
    {                                                \
        ._if._io =                                   \
            {                                        \
                .data = {B_HAL_GPIOE, B_HAL_PINAll}, \
                .rs   = {B_HAL_GPIOD, B_HAL_PIN13},  \
                .wr   = {B_HAL_GPIOB, B_HAL_PIN14},  \
                .rd   = {B_HAL_GPIOD, B_HAL_PIN15},  \
                .cs   = {B_HAL_GPIOC, B_HAL_PIN8},   \
            },                                       \
        .is_rw_addr = 0,                             \
    }

// ds18b20    
#define HAL_DS18B20_IF {.sBusIo = {B_HAL_GPIOB, B_HAL_PIN0},}    
    
#endif
