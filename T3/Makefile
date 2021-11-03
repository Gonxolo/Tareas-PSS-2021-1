CFLAGS= -g -Wall -pedantic -std=c99
LDFLAGS= -g

all: test-g

t3.o test-t3.o: t3.h

test-t3: t3.o test-t3.o

test-g:
	make clean
	make test-t3
	./test-t3

test-O:
	make clean
	make "CFLAGS=-O3 -Wall -pedantic -std=c99 -DOPT" "LDFLAGS=" test-t3
	./test-t3 comparar

test-O-m32:
	make clean
	make "CFLAGS=-O -m32 -Wall -pedantic -std=c99 -DOPT" "LDFLAGS=-m32" test-t3
	./test-t3

test-vg:
	make clean
	make "CFLAGS= -g -Wall -pedantic -std=c99 -DREPS=100" test-t3
	bash -c 'valgrind --track-origins=yes ./test-t3 |& tee test-t3.log'
	echo
	echo "Examine el reporte de valgrind en el archivo test-t3.log"

test-vg-ddd:
	make clean
	make "CFLAGS= -g -Wall -pedantic -std=c99 -DREPS=100" test-t3
	valgrind --track-origins=yes --vgdb=yes --vgdb-error=0 ./test-t3

clean:
	rm -f *.o
	rm -f test-t3
