SET(LIBHAL_SRC  hal/b_hal.c 
		hal/b_hal_gpio.c 
		hal/b_hal_i2c.c 
		hal/b_hal_lcd.c 
		hal/b_hal_qspi.c 
		hal/b_hal_sccb.c 
		hal/b_hal_spi.c 
		hal/b_hal_uart.c 
		utils/b_util_at.c 
		utils/b_util_delay.c 
		utils/b_util_i2c.c 
		utils/b_util_spi.c)

INCLUDE_DIRECTORIES(../config/ ../modules/inc/ ../core/inc/ ../drivers/inc/ hal/inc/ utils/inc/ ../algorithm/inc/)

ADD_LIBRARY(hal SHARED ${LIBHAL_SRC})
