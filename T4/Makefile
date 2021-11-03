CFLAGS= -g -Wall -pedantic -std=c99
LDFLAGS= -g

all: test-g

test-g:
	make clean
	make eliminar
	make run

run:
	@echo "==============================================================="
	@echo "Se eliminaran las palabras nadar calcetines posada y barco."
	@echo "Los diccionarios resultantes deben ser ref-1.txt ref-2.txt "
	@echo "ref-3.txt y ref-3.txt (barco no esta en el diccionario)"
	@echo "Si un test falla, compare dict.txt con el archivo de referencia."
	@echo "A veces hay que mostrar el contenido en hexadecimal para ver"
	@echo "la diferencia.  Por ejemplo con od -x dict.txt ; od -x ref-1.txt"
	@echo "==============================================================="
	cp dict-orig.txt dict.txt
	./eliminar dict.txt nadar
	@echo diff dict.txt ref-1.txt
	@diff dict.txt ref-1.txt && echo test 1 aprobado
	./eliminar dict.txt calcetines
	@echo diff dict.txt ref-2.txt
	@diff dict.txt ref-2.txt && echo test 2 aprobado
	./eliminar dict.txt posada
	@echo diff dict.txt ref-3.txt
	@diff dict.txt ref-3.txt && echo test 3 aprobado
	./eliminar dict.txt barco
	@echo "diff dict.txt ref-3.txt      (no cambia)"
	@diff dict.txt ref-3.txt && echo test 4 aprobado
	@echo "====================================="
	@echo "Ahora se eliminara techo de dict2.txt"
	@echo "====================================="
	cp dict2-orig.txt dict2.txt
	./eliminar dict2.txt techo
	@echo diff dict2.txt ref-4.txt
	@diff dict2.txt ref-4.txt && echo test 5 aprobado
	@echo "==================================================="
	@echo "En el siguiente ejemplo falta el parametro palabra."
	@echo "Se debe mostrar el mensaje de error del enunciado."
	@echo "==================================================="
	@echo ./eliminar dict.txt
	@./eliminar dict.txt 2> err.txt ; test "$$?" -eq 1 || ( echo "Codigo de retorno no es 1" ; false)
	@diff err.txt ref-err.txt || ( echo "Mensaje de error incorrecto o no se escribio en la salida estandar de errores"; false )
	@cat err.txt
	@echo diff err.txt ref-err.txt
	@echo test 6 aprobado
	@echo "============================================================="
	@echo "A continuacion se espera que eliminar reporte errorers porque"
	@echo "d.txt no existe y porque no tiene permiso para rodict.txt."
	@echo "Los codigos de retorno deben ser 2 y 13."
	@echo "============================================================="
	rm -f d.txt
	@echo ./eliminar d.txt hola
	@./eliminar d.txt hola ; test "$$?" -eq 2 || ( echo "Codigo de retorno no es 2" ; false )
	@echo test 7 aprobado
	chmod u-w rodict.txt
	@echo ./eliminar rodict.txt hola
	@./eliminar rodict.txt hola ; test "$$?" -eq 13 || (echo "Codigo de retorno no es 13" ; false )
	@echo test 8 aprobado
	@echo "================================================"
	@echo "Felicitaciones: todos los tests fueron aprobados"
	@echo "================================================"

test-O:
	make clean
	make "CFLAGS=-O3 -Wall -pedantic -std=c99" "LDFLAGS=" eliminar
	make run

test-O-m32:
	make clean
	make "CFLAGS=-O -m32 -Wall -pedantic -std=c99" "LDFLAGS=-m32" eliminar
	make run

test-vg:
	make clean
	make eliminar
	cp dict-orig.txt dict.txt
	bash -c 'valgrind --track-origins=yes ./eliminar dict.txt nadar |& tee test-1.log'
	diff dict.txt ref-1.txt && echo test 1 aprobado
	bash -c 'valgrind --track-origins=yes ./eliminar dict.txt calcetines |& tee test-2.log'
	diff dict.txt ref-2.txt && echo test 2 aprobado
	bash -c 'valgrind --track-origins=yes ./eliminar dict.txt posada |& tee test-2.log'
	diff dict.txt ref-3.txt && echo test 3 aprobado
	bash -c 'valgrind --track-origins=yes ./eliminar dict.txt barco |& tee test-2.log'
	diff dict.txt ref-3.txt && echo test 4 aprobado
	@echo "================================================"
	@echo "Felicitaciones: todos los tests fueron aprobados"
	@echo "================================================"

test-vg-ddd:
	make clean
	make eliminar
	cp dict-orig.txt dict.txt
	@echo Invoque en el terminal: ddd \&
	@echo Invoque en el terminal: valgrind --track-origins=yes --vgdb=yes --vgdb-error=0 ./eliminar dict.txt \<palabra\>
	@echo Invoque en ddd: target remote \| vgdb

clean:
	rm -f *.o *.log
	rm -f eliminar
