# Programe su tarea en el archivo reunion.c
# Para revisar las opciones de compilacion y ejecucion,
# ingrese en el terminal el comando: make

# Con opciones de debugging
DEF=
DBG=-g
INCLUDE=
CFLAGS=$(DBG) $(DEF) -Wall -pedantic -std=c99 $(INCLUDE)
LDFLAGS=-g
LDLIBS=-pthread

VGFLAGS= --leak-check=full --show-leak-kinds=all --track-origins=yes

MAK=make -B --no-print-directory

all:
	@cat README.txt | less

test-reunion: test-reunion.o reunion.o

test-reunion.o reunion.o: reunion.h

run-O:
	@$(MAK) DBG=-O "LDFLAGS=" test-reunion
	./test-reunion

run-g:
	@$(MAK) test-reunion
	./test-reunion

ddd:
	@$(MAK) test-reunion
	@( sleep 1; cat .ddd.txt ) &
	ddd test-reunion

run-valgrind:
	@$(MAK) DEF="-DVALGRIND=1" test-reunion
	@( sleep 1; cat .run-vg.txt ) &
	bash -c "valgrind $(VGFLAGS) ./test-reunion |& tee run.log"

ddd-valgrind:
	@$(MAK) DEF="-DVALGRIND=1" test-reunion
	ddd test-reunion &
	@( sleep 1; cat .ddd-vg.txt ) &
	valgrind $(VGFLAGS) --vgdb=yes --vgdb-error=0 ./test-reunion

rerun-valgrind:
	@$(MAK) DEF="-DVALGRIND=1" test-reunion
	@( sleep 1; cat .ddd-vg.txt ) &
	valgrind $(VGFLAGS) --vgdb=yes --vgdb-error=0 ./test-reunion

run-drd:
	@$(MAK) DEF="-DVALGRIND=1" test-reunion
	@( sleep 1; cat .run-vg.txt ) &
	bash -c "valgrind --tool=drd ./test-reunion |& tee run.log"

run-sanitize:
	@$(MAK) DEF="-fsanitize=thread -DVALGRIND=1" "LDFLAGS=-fsanitize=thread $(LDFLAGS)" test-reunion
	./test-reunion

ddd-drd:
	@$(MAK) DEF="-DVALGRIND=1" test-reunion
	ddd test-reunion &
	@( sleep 1; cat .ddd-vg.txt ) &
	valgrind --tool=drd --vgdb=yes --vgdb-error=0 ./test-reunion

rerun-drd:
	@$(MAK) DEF="-DVALGRIND=1" test-reunion
	@( sleep 1; cat .ddd-vg.txt ) &
	valgrind --tool=drd --vgdb=yes --vgdb-error=0 ./test-reunion

zip:
	@rm -f resultados.txt reunion.zip
	@echo ==== run-O ==== | tee resultados.txt
	@$(MAK) DBG=-O "LDFLAGS=" test-reunion | tee -a resultados.txt
	@echo "./test-reunion" | tee -a resultados.txt
	@bash -c "./test-reunion |& tee -a resultados.txt"
	@echo ==== run-g ==== | tee -a resultados.txt
	@$(MAK) test-reunion | tee -a resultados.txt
	@echo "./test-reunion" | tee -a resultados.txt
	@bash -c "./test-reunion |& tee -a resultados.txt"
	@echo ==== run-valgrind ==== | tee -a resultados.txt
	@$(MAK) DEF="-DVALGRIND=1" test-reunion | tee -a resultados.txt
	@echo "valgrind $(VGFLAGS) ./test-reunion" | tee -a resultados.txt
	@bash -c "valgrind $(VGFLAGS) ./test-reunion |& tee -a resultados.txt"
	@echo ==== run-sanitize ==== | tee -a resultados.txt
	@$(MAK) DEF="-DVALGRIND=1" test-reunion | tee -a resultados.txt
	@echo "./test-reunion" | tee -a resultados.txt
	@bash -c "./test-reunion |& tee -a resultados.txt"
	@echo ==== zip ==== | tee -a resultados.txt
	zip -r reunion.zip resultados.txt reunion.c
	@echo "Entregue por u-cursos el archivo reunion.zip"

process:
	@ps a | grep valgrind | grep vgdb=yes | grep -v grep

clean:
	rm -f *.o test-reunion *.log resultados.txt reunion.zip
