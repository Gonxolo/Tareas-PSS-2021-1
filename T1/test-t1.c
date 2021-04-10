#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include "t1.h"

// ----------------------------------------------------
// Funcion que entrega el tiempo transcurrido desde el lanzamiento del
// programa en milisegundos

static int time0= 0;

static int getTime0() {
    struct timeval Timeval;
    gettimeofday(&Timeval, NULL);
    return Timeval.tv_sec*1000+Timeval.tv_usec/1000;
}

static void resetTime() {
  time0= getTime0();
}

static int getTime() {
  return getTime0()-time0;
}

// ----------------------------------------------------
// Funcion que genera n sumas vectoriales aleatorias con sus resultados

static void gen(unsigned long long avx[], unsigned long long avy[],
                unsigned long long avz[], int n) {
  for (int i= 0; i<n; i++) {
    unsigned long long vx= 0, vy= 0, vz= 0;
    for (int j= 0; j<8; j++) {
      unsigned char x= rand() % 256;
      unsigned char y= rand() % 256;
      unsigned char z= x+y;
      vx = (vx*256) + x;
      vy = (vy*256) + y;
      vz = (vz*256) + z;
    }
    avx[i]= vx;
    avy[i]= vy;
    avz[i]= vz;
  }
}

// ----------------------------------------------------
// Funcion que revisa que un resultado sea correcto

void test(unsigned long long res, unsigned long long res_ref, char *err) {
  if (res != res_ref) {
    fprintf(stderr, "Resultado %llx es incorrecto\n", res);
    fprintf(stderr, "Debio ser %llx\n", res_ref);
    fprintf(stderr, "%s\n", err);
    exit(1);
  }
}

// ----------------------------------------------------
// Funcion que revisa que una serie de resultados sean correctos

void verificar(unsigned long long avx[], unsigned long long avy[],
               unsigned long long avz[], unsigned long long avz_ref[],
               int n) {
  for (int i= 0; i<n; i++) {
    if (avz[i] != avz_ref[i]) {
      fprintf(stderr, "Error en la %d-esima suma\n", i);
      fprintf(stderr, "al sumar %llx con %llx\n", avx[i], avy[i]);
      test(avz[i], avz_ref[i], "Error en una de las sumas vectoriales");
    }
  }
}

// ----------------------------------------------------
// Funcion que llama a sumaVecs para hacer n sumas vectoriales

unsigned long long correr(unsigned long long avx[], unsigned long long avy[],
            unsigned long long avz[], int n) {
  unsigned long long total= 0;
  for (int i= 0; i<n; i++) {
    unsigned long long res= sumaVecs(avx[i], avy[i]);
    avz[i]= sumaVecs(avx[i], avy[i]);
    total += res;
  }
  return total;
}

#define N 10000000

int main(int argc, char *argv[]) {
  // T1
  test(sumaVecs(0, 0), 0,
    "Error en T1: La suma de solo ceros debe ser cero");
  // T2
  test(sumaVecs(3, 7), 10,
    "Error en T2: La suma de los enteros de menor significancia debio ser 10");
  // T3
  test(sumaVecs(100ULL<<56, 101ULL<<56), 201ULL<<56,
    "Error en T3: La suma de los enteros de mayor significancia debio ser 201");
  // T4
  test(sumaVecs((34ULL<<56)+(120ULL<<24)+64ULL,
                ( 3ULL<<56)+(120ULL<<24)+20ULL),
                (37ULL<<56)+(240ULL<<24)+84ULL,
    "Error en T4: La suma con 3 enteros distintos de cero es incorrecta");
  // T5
  test(sumaVecs(100ULL<<40, 158ULL<<40), 2ULL<<40,
    "Error en T5: La suma de enteros de 8 bits (100+158) "
    "con desborde debio ser 2");
  // T6: el test del enunciado
  test(sumaVecs(0xff016701a1000003,
                0x010903ff1f0000ff),
                0x000a6a00c0000002,
    "Error en T6: el resultado es incorrecto");
   
  printf("\nVamos bien, paso los primeros tests\n\n");

  // Verificacion y benchmark
  int n= N;

  printf("Se generan %d sumas aleatorias vectorias\n", n);
  unsigned long long *avx= malloc(n*sizeof(unsigned long long)); 
  unsigned long long *avy= malloc(n*sizeof(unsigned long long)); 
  unsigned long long *avz= malloc(n*sizeof(unsigned long long)); 
  unsigned long long *avz_ref= malloc(n*sizeof(unsigned long long)); 
  gen(avx, avy, avz_ref, n);

  printf("Ejecutando las %d sumas vectoriales\n", n);
  unsigned long long total= correr(avx, avy, avz, n);

  printf("Verificando correctitud de los resultados\n");
  verificar(avx, avy, avz, avz_ref, n);

  int repet= 50;
  printf("Midiendo el tiempo que toma realizar %d veces estas sumas\n",
         repet);
  resetTime();
  int tiempo_ini= getTime();
  for (int i= 0; i<repet; i++) {
    if (total!=correr(avx, avy, avz, n)) {
      fprintf(stderr, "Error: resultado inconsistente\n");
      exit(1);
    }
  }
  int tiempo_fin= getTime();
  int tiempo_sol= tiempo_fin-tiempo_ini;
  printf("Su solucion tomo %d milisegundos\n", tiempo_sol);
  printf("Los resultados son correctos\n");

  if (argc==2 && strcmp(argv[1], "comparar")==0) {
    printf("Ejecutando la solucion eficiente del profesor\n");
    printf("El tiempo quedara almacenado en el archivo tiempo_prof.txt\n");
    system("rm -f tiempo_prof.txt ; ./sol-profesor tiempo_prof.txt > /dev/null");
    FILE *arch_prof= fopen("tiempo_prof.txt", "r");
    if (arch_prof==NULL) {
      perror("tiempo_prof.txt");
      fprintf(stderr,
              "Algo inesperado sucedio: no se pudo leer tiempo_prof.txt\n");
      exit(1);
    }
    int tiempo_prof= -1;
    fscanf(arch_prof, "%d", &tiempo_prof);
    fclose(arch_prof);
    if (tiempo_prof==-1) {
      fprintf(stderr,
              "Algo inesperado sucedio: no se pudo leer tiempo_prof.txt\n");
      exit(1);
    }

    printf("La solucion eficiente del profesor tomo %d milisegundos\n",
           tiempo_prof);
    double cuociente= (double)tiempo_sol/tiempo_prof;
    printf("Cuociente entre el tiempo de su solucion y la del profesor es %f\n",
           cuociente);
    if (cuociente>=4.0) {
      fprintf(stderr,
              "Su solucion es correcta, pero demasiado ineficiente,\n");
      fprintf(stderr, "sera rechazada\n");
      exit(2);
    }
    if (cuociente>=1.8) {
      fprintf(stderr,
              "Su solucion es correcta, aunque no suficientemente\n"
              "eficiente, sera aprobada pero se le descontara un punto\n");
      return 0;
    }
    
    printf("\nFelicitaciones: su solucion es correcta y eficiente\n");
  }
  else if (argc==2) {
    FILE *arch_prof= fopen(argv[1], "w");
    fprintf(arch_prof, "%d\n", tiempo_sol);
    fclose(arch_prof);
  }
  else {
    printf("\nFelicitaciones: su solucion es correcta\n");
    printf("No olvide probar su tarea con: make test-O\n");
    printf("para revisar que sea eficiente\n");
  }

  return 0;
}
