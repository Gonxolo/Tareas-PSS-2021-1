# Programe su tarea en el archivo ultdir.c
# Para revisar las opciones de compilacion y ejecucion,
# ingrese en el terminal el comando: make

# Con opciones de debugging
DEF=
DBG=-g
INCLUDE=
CFLAGS=$(DBG) $(DEF) -Wall -pedantic -std=c99 $(INCLUDE)
LDFLAGS=-g
LDLIBS=-pthread

VGFLAGS= --leak-check=full --track-origins=yes
#VGFLAGS= --child-silent-after-fork=yes --track-origins=yes

MAK=make -B --no-print-directory

all:
	@less README.txt

test-ultdir: test-ultdir.o ultdir.o BigInt.o

test-ultdir.o ultdir.o BigInt.o: ultdir.h BigInt.h

run-duende: duende
	@echo "# Se ejecuta duende, mostrando su salida estandar en el terminal"
	@echo -n "Comando: ";
	./duende
	@echo "# Se ejecuta duende nuevamente redirigiendo su salida a run.log"
	@echo -n "Comando: ";
	./duende > run.log
	@echo "# Este es el archivo run.log generado por duende:"
	@echo -n "Comando: ";
	cat run.log
	@echo "# Este es el programa duende.c:"
	@echo -n "Comando: ";
	cat duende.c
	@echo "# Por que se escribe 2 veces el mismo mensaje, cuando se"
	@echo "# redirige la salida estandar a un archivo?  Necesita usar"
	@echo "# todo lo aprendido en PSS para responder esta pregunta"

run-O:
	@$(MAK) DBG=-O DEF="-DOPT=1" "LDFLAGS=" test-ultdir
	./test-ultdir

run-g:
	@$(MAK) test-ultdir
	./test-ultdir

ddd:
	@$(MAK) test-ultdir
	@( sleep 1; cat .ddd.txt ) &
	ddd test-ultdir

run-valgrind:
	@$(MAK) DEF="-DVALGRIND=1" test-ultdir
	@( sleep 1; cat .run-vg.txt ) &
	bash -c "valgrind $(VGFLAGS) ./test-ultdir |& tee run.log | grep -v '^=='"
	@echo
	@(grep "lost: [1-9]" run.log && echo "*** Hay memory leaks! ***") || true

ddd-valgrind:
	@$(MAK) DEF="-DVALGRIND=1" test-ultdir
	ddd test-ultdir &
	@( sleep 1; cat .ddd-vg.txt ) &
	valgrind $(VGFLAGS) --vgdb=yes --vgdb-error=0 ./test-ultdir

rerun-valgrind:
	@$(MAK) DEF="-DVALGRIND=1" test-ultdir
	@( sleep 1; cat .ddd-vg.txt ) &
	valgrind $(VGFLAGS) --vgdb=yes --vgdb-error=0 ./test-ultdir

run-drd:
	@echo "No tiene sentido correr valgrind/drd con procesos pesados"

run-sanitize:
	@echo "No tiene sentido correr sanitize con procesos pesados"

ddd-drd:
	@echo "No tiene sentido correr sanitize con procesos pesados"

rerun-drd:
	@echo "No tiene sentido correr sanitize con procesos pesados"

zip:
	@rm -f resultados.txt ultdir.zip
	@echo ==== run-O ==== | tee resultados.txt
	@$(MAK) DBG=-O "LDFLAGS=" test-ultdir | tee -a resultados.txt
	@echo "./test-ultdir" | tee -a resultados.txt
	@bash -c "./test-ultdir |& tee -a resultados.txt"
	@echo ==== run-g ==== | tee -a resultados.txt
	@$(MAK) test-ultdir | tee -a resultados.txt
	@echo "./test-ultdir" | tee -a resultados.txt
	@bash -c "./test-ultdir |& tee -a resultados.txt"
	@echo ==== zip ==== | tee -a resultados.txt
	zip -r ultdir.zip resultados.txt ultdir.c
	@echo "Entregue por u-cursos el archivo ultdir.zip"

process:
	@ps a | grep valgrind | grep vgdb=yes | grep -v grep

clean:
	rm -f *.o test-ultdir *.log resultados.txt ultdir.zip duende
