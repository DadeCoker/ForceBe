SET(LIBMODULES_SRC b_mod_battery.c
		b_mod_button.c
		b_mod_error.c
		b_mod_event.c
		b_mod_fifo.c
		b_mod_fs.c
		b_mod_gui.c
		b_mod_heap.c
		b_mod_kv.c
		b_mod_log.c
		b_mod_lunar.c
		b_mod_menu.c
		b_mod_modbus.c
		b_mod_param.c
		b_mod_protocol.c
		b_mod_qpn.c
		b_mod_sda.c
		b_mod_sdb.c
		b_mod_shell.c
		b_mod_timer.c
		b_mod_trace.c
		b_mod_utc.c
		b_mod_xm128.c
		b_mod_ymodem.c)

INCLUDE_DIRECTORIES(inc ../../Examples/Test/bos_config/ 
			../core/inc/ 
			../../Examples/Test/bos_hal/inc/ 
			../utils/inc/ 
			../algorithm/inc/
			../thirdparty/SFUD/inc/
			../thirdparty/CmBacktrace/
			../thirdparty/FatFS/
			../thirdparty/FlexibleButton/
			../thirdparty/littlefs/
			../thirdparty/nr_micro_shell/inc/
			../thirdparty/qpn/inc/
			../thirdparty/UGUI/)

ADD_LIBRARY(module SHARED ${LIBMODULES_SRC})
