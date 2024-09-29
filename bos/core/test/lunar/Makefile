obj = test.o ../../src/b_lunar.o
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
	

