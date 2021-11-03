==========================================================
Esta es la documentacion para compilar y ejecutar su tarea
==========================================================

Se esta ejecutando el comando: less README.txt

Para avanzar a una nueva pagina: tecla <page down>
Para retroceder a la pagina anterior: tecla <page up>
Para avanzar una sola linea: tecla <enter>
Para salir: tecla q

-----------------------------------------------

Opciones de compilacion/ejecucion:

make run-O : Compila y ejecuta con opciones de optimizacion
make run-g : Compila y ejecuta con opciones de depuracion
make ddd : Compila y lanza ddd para depurar
make run-valgrind : Compila y ejecuta con valgrind
make ddd-valgrind : Compila y lanza ddd para depurar con valgrind
make rerun-valgrind : Re-ejecuta el programa para ddd/valgrind
make run-sanitize : Compila y ejecuta con valgrind/sanitize threads para
         detectar dataraces
make ddd-drd : Compila y lanza ddd para depurar con drd.  Este es otro
         detector de dataraces, pero a veces detecta erradamente un
         datarace en printf
make rerun-drd : Re-ejecuta el programa para ddd/drd

make zip: Ejecuta todas las pruebas y genera el archivo reunion.zip
         que Ud. debe entregar por U-cursos

make process: Reporta con el comando ps los pid de los procesos activos
         en ejecucion con valgrind.  Es util por si debe matarlos con
         kill -9 pid.
make clean: hace limpieza borrando todos los archivos generados

run-valgrind y run-drd graban una copia de la salida en run.log

Si ddd deja de responder cuando corre valgrind, mate el
proceso con:

kill -9 <pid>

En donde <pid> el numero de proceso reportado por valgrind en el formato:

==<pid>==

Por ejemplo si <pid> es 3423:

kill -9 3423

Termine ddd con quit
y vuelva a lanzar make ddd-valgrind o make ddd-drd
