SET(LIBTHIRDPARY_SRC CmBacktrace/cm_backtrace.c
		   FatFS/diskio.c
		   FatFS/ff.c
		   FatFS/ffsystem.c
		   FatFS/ffunicode.c
		   FlexibleButton/flexible_button.c
		   littlefs/lfs.c
		   littlefs/lfs_util.c
		   nr_micro_shell/src/ansi.c
		   nr_micro_shell/src/ansi_port.c
		   nr_micro_shell/src/nr_micro_shell.c
		   qpn/src/qepn.c
		   qpn/src/qfn.c
  	           qpn/src/qstamp.c
		   qpn/src/qvn.c
		   SFUD/sfud.c
		   SFUD/sfud_sfdp.c
		   UGUI/ugui.c)

INCLUDE_DIRECTORIES(../../Examples/Test/bos_config/ 
			../modules/inc/
			../algorithm/inc/
			../core/inc/
			../../Examples/Test/bos_hal/inc/
			../utils/inc/
			../drivers/inc/
			../thirdparty/SFUD/inc/
			../thirdparty/CmBacktrace/
			../thirdparty/FatFS/
			../thirdparty/FlexibleButton/
			../thirdparty/littlefs/
			../thirdparty/nr_micro_shell/inc/
			../thirdparty/qpn/inc/
			../thirdparty/UGUI/)

ADD_LIBRARY(thirdparty SHARED ${LIBTHIRDPARY_SRC})
