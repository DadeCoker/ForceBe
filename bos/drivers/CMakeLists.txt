SET(LIBDRIVER_SRC camera/b_drv_ov5640.c
		flash/b_drv_24cxx.c
		flash/b_drv_fm25cl.c
		flash/b_drv_sd.c
		flash/b_drv_spiflash.c
		lcd/b_drv_ili9320.c
		lcd/b_drv_ili9341.c
		lcd/b_drv_oled.c
		lcd/b_drv_ssd1289.c
		gsensor/b_drv_lis3dh.c
		touch/b_drv_xpt2046.c)

INCLUDE_DIRECTORIES(inc 
			../../bos_config/ 
			../modules/inc/ 
			../core/inc/ 
			../../bos_hal/inc/ 
			../utils/inc/ 
			../algorithm/inc/
			../thirdparty/SFUD/inc/)

ADD_LIBRARY(driver SHARED ${LIBDRIVER_SRC})
