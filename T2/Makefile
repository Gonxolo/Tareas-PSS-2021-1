CFLAGS= -g -Wall -pedantic -std=c99
LDFLAGS= -g

all: test-g

t2.o test-t2.o: t2.h

test-t2: t2.o test-t2.o

test-g:
	make clean
	make test-t2
	./test-t2

test-O:
	make clean
	make "CFLAGS=-O3 -Wall -pedantic -std=c99 -DREPS=30000" "LDFLAGS=" test-t2
	./test-t2 comparar

test-O-m32:
	make clean
	make "CFLAGS=-O -m32 -Wall -pedantic -std=c99" "LDFLAGS=-m32" test-t2
	./test-t2

test-vg:
	make clean
	make "CFLAGS= -g -Wall -pedantic -std=c99 -DREPS=100" test-t2
	bash -c 'valgrind ./test-t2 |& tee test-t2.log'
	echo
	echo "Examine el reporte de valgrind en el archivo test-t2.log"

test-vg-ddd:
	make clean
	make "CFLAGS= -g -Wall -pedantic -std=c99 -DREPS=100" test-t2
	valgrind --vgdb=yes --vgdb-error=0 ./test-t2

clean:
	rm -f *.o
	rm -f test-t2
