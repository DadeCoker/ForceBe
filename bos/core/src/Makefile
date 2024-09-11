src = b_battery.c b_core.c b_crc32.c b_device.c b_error.c b_event.c b_fifo.c b_modbus.c b_ota.c b_protocol.c b_sda.c b_sdb.c b_sdc.c b_sum.c b_utc.c

INCLUDES = -I ../inc/ -I ../../drivers/inc/ -I ../../hal/ 

CC = gcc
all:
	$(CC) $(INCLUDES) $(HAL_INCLUDE) -c $(src)


clean:
	rm -rf *.o
