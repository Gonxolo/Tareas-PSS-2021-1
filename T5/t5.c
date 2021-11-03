#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <float.h>

#include "t5.h"


typedef struct{
  int *z;
  int n;
  double **m;
  int nperm;
  double min;
} Args;



void *thread_function(void *p){
  Args *args = (Args *)p;
  int n = args->n;
  int nperm = args->nperm;
  double **m = args->m;
  int *z = args->z;

  args->min = viajante(z, n, m, nperm);

  return NULL;
}

double viajante_par(int z[], int n, double **m, int nperm, int p) {
  pthread_t pid[p];
  Args args[p];
  int intervalo = nperm/p;
  int resto = nperm%p;
  
  int *Aux[p];
  
  for (int i = 0; i<p; i++){
    Aux[i] = malloc((n+1) * sizeof(int *));
  }
  for (int i=0; i<p; i++){
    args[i].z = Aux[i];
    args[i].n = n;
    args[i].m = m;
    args[i].nperm = intervalo;
    if (resto > 0){
      args[i].nperm++;
      resto--;
    }
    args[i].min = DBL_MAX;
    pthread_create(&pid[i], NULL, thread_function, &args[i]);
  }
  double minimum = DBL_MAX;

  for (int i=0; i<p; i++){
    pthread_join(pid[i], NULL);
    if(args[i].min < minimum){
      minimum = args[i].min;
      for (int j=0; j<=n; j++){
        z[j] = args[i].z[j];
      }
    }
  }

  for (int i=0; i<p; i++){
    free(Aux[i]);
  }
  return minimum;
}
