#ifndef __B_HAL_IF_H__
#define __B_HAL_IF_H__

// debug
#define HAL_LOG_UART B_HAL_UART_1

// 24cxx
#define HAL_24CXX_IF                                             \
    {                                                            \
        {                                                        \
            .dev_addr               = 0xa0,                      \
            .is_simulation          = 1,                         \
            ._if.simulating_i2c.sda = {B_HAL_GPIOB, B_HAL_PIN7}, \
            ._if.simulating_i2c.clk = {B_HAL_GPIOB, B_HAL_PIN6}, \
        },                                                       \
    }

// button
#if _FLEXIBLEBUTTON_ENABLE
#define HAL_B_BUTTON_GPIO             \
    {                                 \
        {B_HAL_GPIOA, B_HAL_PIN0, 0}, \
    }
#endif

// ds18b20
#define HAL_DS18B20_IF                       \
    {                                        \
        .sBusIo = {B_HAL_GPIOA, B_HAL_PIN4}, \
    }

// fm25cl
#define HAL_FM25CL_IF                                   \
    {                                                   \
        {                                               \
            .is_simulation = 0,                         \
            ._if.spi       = B_HAL_SPI_1,               \
            .cs            = {B_HAL_GPIOB, B_HAL_PIN9}, \
        },                                              \
    }

// lis3dh
#define HAL_LIS3DH_IF                                                 \
    {                                                                 \
        ._if._spi.is_simulation = 0, ._if._spi._if.spi = B_HAL_SPI_1, \
        ._if._spi.cs = {B_HAL_GPIOB, B_HAL_PIN9}, .is_spi = 1,        \
    }

// l3gd20
#define HAL_L3GD20_IF                                                 \
    {                                                                 \
        ._if._spi.is_simulation = 0, ._if._spi._if.spi = B_HAL_SPI_1, \
        ._if._spi.cs = {B_HAL_GPIOB, B_HAL_PIN9}, .is_spi = 1,        \
    }

// lcd
#define HAL_TFT_LCD_IF                               \
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

// ili9320
#define HAL_ILI9320_IF HAL_TFT_LCD_IF
// ili9341
#define HAL_ILI9341_IF HAL_TFT_LCD_IF

// oled
#define HAL_OLED_IF                                                                \
    {                                                                              \
        ._if._i2c.dev_addr = 0x78, ._if._i2c.is_simulation = 1,                    \
        ._if._i2c._if.simulating_i2c.sda = {B_HAL_GPIOB, B_HAL_PIN1},              \
        ._if._i2c._if.simulating_i2c.clk = {B_HAL_GPIOB, B_HAL_PIN0}, .is_spi = 0, \
    }

// ssd1289
#define HAL_SSD1289_IF HAL_TFT_LCD_IF

// pcf8574
#define HAL_PCF8574_IF                                                \
    {                                                                 \
        .dev_addr = 0x40, .is_simulation = 0, ._if.i2c = B_HAL_I2C_1, \
    }

// spiflash
#define HAL_SPIFLASH_TOTAL_NUMBER 1
#define HAL_SPIFLASH_IF                                          \
    {                                                            \
        {                                                        \
            ._if._spi.is_simulation = 0,                         \
            ._if._spi._if.spi       = B_HAL_SPI_1,               \
            ._if._spi.cs            = {B_HAL_GPIOB, B_HAL_PIN9}, \
            .is_spi                 = 1,                         \
        },                                                       \
    }

// sd
#define HAL_SD_IF                                                                     \
    {                                                                                 \
        ._if.spi = B_HAL_SPI_1, .cs = {B_HAL_GPIOD, B_HAL_PIN11}, .is_simulation = 0, \
    }

// xpt2046
#define HAL_XPT2046_IF                                                               \
    {                                                                                \
        ._if.spi = B_HAL_SPI_3, .cs = {B_HAL_GPIOC, B_HAL_PIN9}, .is_simulation = 0, \
    }

#endif
