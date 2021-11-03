# Programe su tarea en el archivo prod.c
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

test-prod: test-prod.o prod.o BigInt.o

test-prod.o prod.o prod-thread.o BigInt.o: prod.h BigInt.h

prod-thread: test-prod.o prod-thread.o BigInt.o

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
	@$(MAK) DBG=-O DEF="-DOPT=1" "LDFLAGS=" test-prod
	./test-prod

run-g:
	@$(MAK) test-prod
	./test-prod | tee run.log
	@grep "Llamada secuencial " run.log | wc -l > sec.log
	@echo "Verificando que se crearon 4 procesos pesados"
	@diff sec.log 4.Txt || ( echo "No aprueba: no se crearon 4 procesos" ; false )
	@echo Aprobado

ddd:
	@$(MAK) test-prod
	@( sleep 1; cat .ddd.txt ) &
	ddd test-prod

run-valgrind:
	@$(MAK) DEF="-DVALGRIND=1" test-prod
	@( sleep 1; cat .run-vg.txt ) &
	bash -c "valgrind $(VGFLAGS) ./test-prod |& tee run.log | grep -v '^=='"
	@echo
	@(grep "lost: [1-9]" run.log && echo "*** Hay memory leaks! ***") || true

ddd-valgrind:
	@$(MAK) DEF="-DVALGRIND=1" test-prod
	ddd test-prod &
	@( sleep 1; cat .ddd-vg.txt ) &
	valgrind $(VGFLAGS) --vgdb=yes --vgdb-error=0 ./test-prod

rerun-valgrind:
	@$(MAK) DEF="-DVALGRIND=1" test-prod
	@( sleep 1; cat .ddd-vg.txt ) &
	valgrind $(VGFLAGS) --vgdb=yes --vgdb-error=0 ./test-prod

run-drd:
	@echo "No tiene sentido correr valgrind/drd con procesos pesados"

run-sanitize:
	@echo "No tiene sentido correr sanitize con procesos pesados"

ddd-drd:
	@echo "No tiene sentido correr sanitize con procesos pesados"

rerun-drd:
	@echo "No tiene sentido correr sanitize con procesos pesados"

zip:
	@rm -f resultados.txt prod.zip
	@echo ==== run-O ==== | tee resultados.txt
	@$(MAK) DBG=-O "LDFLAGS=" test-prod | tee -a resultados.txt
	@echo "./test-prod" | tee -a resultados.txt
	@bash -c "./test-prod |& tee -a resultados.txt"
	@echo ==== run-g ==== | tee -a resultados.txt
	@$(MAK) test-prod | tee -a resultados.txt
	@echo "./test-prod" | tee -a resultados.txt
	@bash -c "./test-prod |& tee -a resultados.txt"
	@echo ==== zip ==== | tee -a resultados.txt
	zip -r prod.zip resultados.txt prod.c
	@echo "Entregue por u-cursos el archivo prod.zip"

process:
	@ps a | grep valgrind | grep vgdb=yes | grep -v grep

clean:
	rm -f *.o test-prod prod-thread *.log resultados.txt prod.zip duende
