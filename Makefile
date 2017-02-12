CC=gcc
CFLAGS=-std=gnu99
DEBUGFLAGS=-ggdb -g3 -gdwarf-2
RELEASEFLAGS=-O3

.PHONY: default clean test

default: dynobj_test

OBJS=main.o dynobj.o

dynobj_test: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c dynobj.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	find . -name "*.o" -type f -delete
	find . -name "*~" -type f -delete
	rm dynobj_test

format:
	clang-format -i dynobj.c dynobj.h

test: dynobj_test
	./dynobj_test
