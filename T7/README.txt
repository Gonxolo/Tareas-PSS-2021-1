==========================================================
Esta es la documentacion para compilar y ejecutar su tarea
==========================================================

Se esta ejecutando el comando: less README.txt

***************************
*** Para salir: tecla q ***
***************************

Para avanzar a una nueva pagina: tecla <page down>
Para retroceder a la pagina anterior: tecla <page up>
Para avanzar una sola linea: tecla <enter>
Para buscar un texto: tecla / seguido del texto (/...texto...)
         por ejemplo: /valgrind

-----------------------------------------------

Opciones de compilacion/ejecucion:

make run-duende: Opcional.  Solo demuestra un comportamiento extrano de fork.
make run-O : Compila y ejecuta con opciones de optimizacion
make run-g : Compila y ejecuta con opciones de depuracion
make ddd : Compila y lanza ddd para depurar
make run-valgrind : Compila y ejecuta con valgrind
             Entrega un reporte para todos los procesos!
             Es normal que aparezca memoria reportada como still reachable.
make ddd-valgrind : Compila y lanza ddd para depurar con valgrind
make rerun-valgrind : Re-ejecuta el programa para ddd/valgrind

make zip: Ejecuta todas las pruebas y genera el archivo prod.zip
         que Ud. debe entregar por U-cursos

make clean: hace limpieza borrando todos los archivos generados

No tiene sentido sanitize de threads para procesos pesados.

*** Depurar programas que usan fork es dificil! ***

run-valgrind graba una copia de la salida en run.log

Si ddd deja de responder cuando corre valgrind, mate el
proceso con:

kill -9 <pid>

En donde <pid> el numero de proceso reportado por valgrind en el formato:

==<pid>==

Por ejemplo si <pid> es 3423:

kill -9 3423

Termine ddd con quit
y vuelva a lanzar make ddd-valgrind o make ddd-drd
