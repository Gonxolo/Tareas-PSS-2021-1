typedef struct nodo {
  int x;
  struct nodo *izq, *der;
} Nodo; 

int eliminar_primeros(Nodo **pa, int k);
Nodo *ultimos(Nodo *a, int *pk);
