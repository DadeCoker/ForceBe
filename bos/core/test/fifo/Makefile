obj = test_fifo.o ../../src/b_fifo.o
INCLUDES = -I ../../inc/
src = ../../src/

test:bos $(obj)
	gcc $(obj) -o $@

bos:
	make -C ../../src/
%.o:%.c
	gcc $(INCLUDES) -c $< 

clean:
	rm -rf *.o test
	cd ../../src/ && make clean
	

