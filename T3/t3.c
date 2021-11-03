#include <stdlib.h>

#include "t3.h"

int eliminar_primeros(Nodo **pa, int k) {
  Nodo* a = *pa;

  if (a == NULL || k == 0){
    return 0;
  }

  int ramaIzq = eliminar_primeros(&a->izq, k);

  if (ramaIzq == k){
    return k;
  }

  *pa = a->der;
  free(a);

  int ramaDer = eliminar_primeros(pa, k - ramaIzq - 1);

  return ramaIzq + ramaDer + 1;
}

Nodo *ultimos(Nodo *a, int *pk) {
  int pkDer;
  pkDer = *pk;

  if (a == NULL){
    *pk = 0;
    return NULL;
  }

  Nodo *ramaDer = ultimos(a->der, &pkDer);

  if (*pk == pkDer){
    return ramaDer;
  }

  pkDer += 1;

  Nodo *nodoNuevo = malloc(sizeof(Nodo));

  nodoNuevo->x = a->x;
  nodoNuevo->der = ramaDer;
  nodoNuevo->izq = NULL;

  if(*pk == pkDer){
    return nodoNuevo;
  }

  int pkIzq = *pk - pkDer;
  Nodo *ramaIzq = ultimos(a->izq, &pkIzq);
  
  nodoNuevo->izq = ramaIzq;

  *pk = pkDer + pkIzq; 

  return nodoNuevo;
}

