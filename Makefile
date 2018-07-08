CC = gcc
CFLAGS = -g
LIBS = -ljpeg -lm


all: main

main: main.o jpeg_manage.o c_obscure.o
	$(CC) $^ -o $@ $(LIBS)

main.o: main.c jpeg_manage.h c_obscure.h

jpeg_manage.o: jpeg_manage.h jpeg_manage.c

c_obscure.o: c_obscure.h c_obscure.c

clean:
	rm -rf c_obscure.o jpeg_manage.o main main.o