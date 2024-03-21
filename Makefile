CC = gcc
CFLAG = -Wall -Wextra -Wno-missing-braces -std=gnu17 -g
LIB = -lpthread #-fsanitize=leak,bounds,address
SRC = src/*.c

.PHONY: static test

croutine: $(SRC) 
	$(CC) $(CFLAG) -shared -fPIC $(SRC) -o lib$@.so $(LIB)

static: $(SRC) 
	$(CC) $(CFLAG) $(SRC) -c $(LIB)
	ar rcs lib$@.a *.o

clean: 
	rm *.o *.a *.so *.dll test

test: test.c 
	@ $(CC) $(CFLAG) $< -o $@ $(LIB) -z execstack -Wl,-rpath=./ -L. -lcroutine
