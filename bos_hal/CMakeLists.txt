SET(LIBHAL_SRC  b_hal.c 
		b_hal_gpio.c 
		b_hal_i2c.c 
		b_hal_lcd.c 
		b_hal_qspi.c 
		b_hal_sccb.c 
		b_hal_spi.c 
		b_hal_uart.c)

INCLUDE_DIRECTORIES(inc ../bos_config/ 
			../bos/modules/inc
			../bos/core/inc
			../bos/utils/inc)

ADD_LIBRARY(hal SHARED ${LIBHAL_SRC})
