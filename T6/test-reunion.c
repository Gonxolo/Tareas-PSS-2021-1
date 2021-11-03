#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "reunion.h"

static int debug= 1;

typedef struct {
  pthread_mutex_t m;
  pthread_cond_t c;
  int entraron, salieron, esperados;
} Verif;
  
typedef struct {
  Reunion *r;
  int delay;
  char *nombre;
  Verif *pv;
} Param;

static void *fun(void *ptr) {
  Param *param= ptr;
  Verif *pv= param->pv;
  pthread_mutex_lock(&pv->m);
  if (debug)
    printf("Entrar de %s\n", param->nombre);
  pthread_mutex_unlock(&pv->m);
  entrar(param->r);
  pthread_mutex_lock(&pv->m);
  pv->entraron++;
  pthread_cond_broadcast(&pv->c);
  pthread_mutex_unlock(&pv->m);
  if (param->delay>0) sleep(param->delay);
  pthread_mutex_lock(&pv->m);
  pthread_cond_broadcast(&pv->c);
  while (pv->entraron<pv->esperados)
    pthread_cond_wait(&pv->c, &pv->m);
  pv->salieron++;
  if (debug)
    printf("Concluir de %s\n", param->nombre);
  pthread_mutex_unlock(&pv->m);
  concluir(param->r);
  pthread_mutex_lock(&pv->m);
  pthread_cond_broadcast(&pv->c);
  if (pv->salieron!=pv->esperados) {
    fprintf(stderr, "Funcionario %s salio antes de que todos concluyeran\n",
            param->nombre);
    exit(1);
  }
  pthread_mutex_unlock(&pv->m);
  return NULL;
}

static void *testEnun(void *ptr) {
  Reunion *r= nuevaReunion();
  pthread_t f1, f2, f3;
  Verif v= { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, 0, 0, 3 };
  Param p1= { r, 1, "F1", &v };
  Param p2= { r, 4, "F2", &v };
  Param p3= { r, 4, "F3", &v };
  pthread_create(&f3, 0, fun, &p3);
  sleep(1);
  pthread_create(&f2, 0, fun, &p2);
  sleep(1);
  pthread_create(&f1, 0, fun, &p1);
  pthread_join(f1, NULL);
  pthread_join(f2, NULL);
  pthread_join(f3, NULL);
  destruirReunion(r);
  pthread_cond_destroy(&v.c);
  pthread_mutex_destroy(&v.m);
  return NULL;
}

static void *testSinPausa(void *ptr) {
  int n= (intptr_t)ptr;
  Reunion *r= nuevaReunion();
  Verif v= { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, 0, 0, n };
  pthread_t f[n];
  Param p[n];
  for (int i= 0; i<n; i++) {
    p[i].r= r;
    p[i].delay= 0;
    p[i].nombre= malloc(10);
    sprintf(p[i].nombre, "F%d", i);
    p[i].pv= &v;
    pthread_create(&f[i], 0, fun, &p[i]);
  }
  for (int i= 0; i<n; i++) {
    pthread_join(f[i], NULL);
    free(p[i].nombre);
  }
  destruirReunion(r);
  pthread_cond_destroy(&v.c);
  pthread_mutex_destroy(&v.m);
  return NULL;
}


int main() {
  {
    printf("Test 1: un solo participante\n");
    Reunion *r= nuevaReunion();
    pthread_t f;
    Verif v= { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, 0, 0, 1 };
    Param p= { r, 1, "F", &v };
    pthread_create(&f, 0, fun, &p);
    pthread_join(f, NULL);
    destruirReunion(r);
    pthread_cond_destroy(&v.c);
    pthread_mutex_destroy(&v.m);
    printf("Test 1 aprobado\n");
  }
  {
    printf("Test 2: 3 participantes, igual al diagrama del enunciado\n");
    int pausa= 1;
    testEnun(&pausa);
    printf("Test 2 aprobado\n");
  }
  {
    int n= 10;
    printf("Test 3: el mismo test 2 con %d instancias en paralelo\n", n);
    debug= 0;
    pthread_t af[n];
    for (int i= 0; i<n; i++) {
      if (pthread_create(&af[i], 0, testEnun, NULL)!=0) {
        perror("pthread_create");
        exit(i);
      }
    }
    for (int i= 0; i<n; i++) {
      pthread_join(af[i], NULL);
    }
    printf("Test 3 aprobado\n");
  }
  {
#ifdef VALGRIND
    int m= 100, maxthr= 10;
#else
    int m= 10000, maxthr= 20;
#endif
    int fin= 0;
    printf("Test de robustez: %d instancias en paralelo sin espera\n", m);
    pthread_t t[m];
    for (int i= 0; i<m; i++) {
      if (i-fin>=maxthr) {
        pthread_join(t[fin], NULL);
        fin++;
      }
      if (pthread_create( &t[i], NULL, testSinPausa,
                          (void*)(intptr_t)(i%6+1) )) {
        perror("pthread_create");
        exit(1);
      }
    }
    while (fin<m) {
      pthread_join(t[fin], NULL);
      fin++;
    }
  }

  printf("Felicitaciones: su tarea ha aprobado todos los tests.\n");
  return 0;
}
