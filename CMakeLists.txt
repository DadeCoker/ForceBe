PROJECT(BABYOS)
ADD_SUBDIRECTORY(bos/algorithm)
ADD_SUBDIRECTORY(bos/core)
ADD_SUBDIRECTORY(bos/utils/)
ADD_SUBDIRECTORY(bos/drivers)
ADD_SUBDIRECTORY(bos/modules)
ADD_SUBDIRECTORY(bos/thirdparty)
ADD_SUBDIRECTORY(bos_hal)

INCLUDE_DIRECTORIES(bos_config/ 
			bos/modules/inc/
			bos/algorithm/inc/
			bos/core/inc/
			bos/utils/inc/
			bos_hal/inc/
			bos/drivers/inc/
			bos/thirdparty/SFUD/inc/
			bos/thirdparty/CmBacktrace/
			bos/thirdparty/FatFS/
			bos/thirdparty/FlexibleButton/
			bos/thirdparty/littlefs/
			bos/thirdparty/nr_micro_shell/inc/
			bos/thirdparty/qpn/inc/
			bos/thirdparty/UGUI/)



ADD_EXECUTABLE(babyos main.c)
TARGET_LINK_LIBRARIES(babyos algorithm core driver module hal utils thirdparty)
