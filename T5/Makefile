CFLAGS= -g -Wall -pedantic -std=c99
LDFLAGS= -g
LDLIBS= -lm -lpthread

all: test-t5

test-t5.o t5.o: t5.h

test-t5: t5.o test-t5.o

test-g:
	make clean
	make
	./test-t5

test-O:
	make clean
	make "CFLAGS= -O -Wall -pedantic -std=c99" LDFLAGS=
	./test-t5

test-O-m32:
	make clean
	make "CFLAGS= -m32 -O -Wall -pedantic -std=c99" "LDFLAGS=-m32"
	./test-t5

test-valgrind-ddd:
	make clean
	make "CFLAGS=-g -Wall -pedantic -std=c99 -DVALGRIND"
	valgrind --vgdb=yes --vgdb-error=0 ./test-t5

test-valgrind:
	make clean
	make "CFLAGS=-g -Wall -pedantic -std=c99 -DVALGRIND"
	valgrind ./test-t5

clean:
	rm -f *.o *.log
	rm -f test-t5
