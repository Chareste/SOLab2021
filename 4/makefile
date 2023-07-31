CC		 =  gcc
CFLAGS =	-Wall 
TARGETS= passwd mywc matrix matComparison

.PHONY: clean all
.SUFFIXES: .c .h

all: $(TARGETS)


passwd: passwd.o
	$(CC) $^ -o $@
mywc: mywc.o
	$(CC) $^ -o $@
matrix: matrix.o
	$(CC) $^ -o $@ 
matComparison: matComparison.o
	$(CC) $^ -o $@

passwd.o: passwd.c
mywc.o: mywc.c
matrix.o: matrix.c
matComparison.o: matComparison.c

clean: 
	-rm -f $(TARGETS)