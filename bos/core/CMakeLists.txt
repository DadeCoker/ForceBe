SET(LIBCORE_SRC b_core.c b_device.c b_os.c)

INCLUDE_DIRECTORIES(inc 
			../../bos_config/ 
			../modules/inc/ 
			../core/inc/ 
			../drivers/inc/ 
			../../bos_hal/inc/ 
			../utils/inc/ 
			../algorithm/inc/)

ADD_LIBRARY(core SHARED ${LIBCORE_SRC})
