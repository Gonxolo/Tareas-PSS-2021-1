#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "reunion.h"

struct reunion {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int contador;
};

Reunion *nuevaReunion(){
  Reunion *r = malloc(sizeof(Reunion));
  pthread_mutex_init(&r->mutex, NULL);
  pthread_cond_init(&r->cond, NULL);
  r->contador = 0;
  return r;
}

void entrar(Reunion *r){
  pthread_mutex_lock(&r->mutex);
  r->contador++;
  pthread_mutex_unlock(&r->mutex);
}

void concluir(Reunion *r){
  pthread_mutex_lock(&r->mutex);
  r->contador--;
  pthread_cond_broadcast(&r->cond);
  while(r->contador>0){
    pthread_cond_wait(&r->cond, &r->mutex);
  }
  pthread_mutex_unlock(&r->mutex);
}
void destruirReunion(Reunion *r){
  free(r);
}
