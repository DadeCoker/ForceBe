SET(LIBCORE_SRC b_core.c b_device.c b_os.c)

INCLUDE_DIRECTORIES(inc ../config/ ../modules/inc/ ../core/inc/ ../drivers/inc/ ../hal/hal/inc/ ../hal/utils/inc/ ../algorithm/inc/)

ADD_LIBRARY(core SHARED ${LIBCORE_SRC})
