#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "t3.h"

#ifdef OPT

// ----------------------------------------------------
// Funcion que entrega el tiempo transcurrido desde el lanzamiento del
// programa en milisegundos

static long long time0= 0;

static long long getTime0() {
    struct rusage ru;
    int rc= getrusage(RUSAGE_SELF, &ru);
    if (rc!=0) {
      perror("getTime");
      exit(1);
    }
    return (long long)ru.ru_utime.tv_sec*1000000+ru.ru_utime.tv_usec;
}

static void resetTime() {
  time0= getTime0();
}

static int getTime() {
  return (getTime0()-time0+500)/1000;
}

#endif

// ----------------------------------------------------
// revisar: verifica que un string es un valor especifico

void revisar(char *s, char *valor) {
  if (strcmp(s, valor)!=0) {
    fprintf(stderr, "El string debio ser \"%s\", no \"%s\"\n", valor, s);
    exit(1);
  }
}

// ----------------------------------------------------
// revisar_int: verifica que un entero es un valor especifico

void revisar_int(int n, int nref) {
  if (n!=nref) {
    fprintf(stderr, "El valor retornado debio ser %d, no %d\n", nref, n);
    exit(1);
  }
}

// ----------------------------------------------------
// Funcion que construye un arbol

Nodo *abb(int x, Nodo *izq, Nodo *der) {
  Nodo *a= malloc(sizeof(*a));
  a->x= x;
  a->izq= izq;
  a->der= der;
  return a;
}

// ----------------------------------------------------
// Funcion que verifica que 2 arboles binarios son iguales

static void revisar_abb(Nodo *a, Nodo *b) {
  if (a==NULL) {
    if (b==NULL)
      return;
    fprintf(stderr, "Arboles distintos: a es NULL, b tiene etiqueta %d\n",
                    b->x);
    exit(1);
  }
  else if (b==NULL) {
    fprintf(stderr, "Arboles distintos: b es NULL, a tiene etiqueta %d\n",
                    a->x);
    exit(1);
  }
  else {
    if (a->x!=b->x) {
      fprintf(stderr, "Etiquetas de arboles son distintas: %d != %d\n",
                      a->x, b->x);
      exit(1);
    }
    revisar_abb(a->izq, b->izq);
    revisar_abb(b->der, b->der);
  }
}

// ----------------------------------------------------
// Funcion que verifica que los campos x de un arbol binario
// desbalanceado por la derecha sean i, i+1, ..., j

static void revisar_des_der(Nodo *a, int i, int j) {
  int ref= i;
  while (ref<=j) {
    if (a==NULL) {
      fprintf(stderr, "El arbol esta incorrectamente vacio\n");
      exit(1);
    }
    if (a->x!=ref) {
      fprintf(stderr, "x es %d, debio ser %d\n", a->x, ref);
      exit(1);
    }
    if (a->izq!=NULL) {
      fprintf(stderr, "Subarbol izquierdo debio ser NULL en nodo %d\n", a->x);
      exit(1);
    }
    a= a->der;
    ref++;
  }
  if (a!=NULL) {
    fprintf(stderr, "El arbol tiene mas nodos de lo que deberia\n");
    exit(1);
  }
}

// ----------------------------------------------------
// Funcion que verifica que los campos x de un arbol binario
// desbalanceado por la derecha sean i, i+1, ...,j

static void revisar_des_izq(Nodo *a, int i, int j) {
  int ref= j;
  while (i<=ref) {
    if (a==NULL) {
      fprintf(stderr, "El arbol esta incorrectamente vacio\n");
      exit(1);
    }
    if (a->x!=ref) {
      fprintf(stderr, "x es %d, debio ser %d\n", a->x, ref);
      exit(1);
    }
    if (a->der!=NULL) {
      fprintf(stderr, "Subarbol derecho debio ser NULL en nodo %d\n", a->x);
      exit(1);
    }
    a= a->izq;
    ref--;
  }
  if (a!=NULL) {
    fprintf(stderr, "El arbol tiene mas nodos de los que deberia\n");
    exit(1);
  }
}

// ----------------------------------------------------
// Revisa que los campos de un arbol sean i, i+1, i+2, etc.

static int revisar_campos(Nodo *a, int i) {
  if (a==NULL) {
    return i;
  }
  int j= revisar_campos(a->izq, i);
  if (a->x!=j) {
    fprintf(stderr, "El nodo con x=%d debio ser %d\n", a->x, j);
    exit(1);
  }
  j= revisar_campos(a->der, j+1);
  return j;
}

// ----------------------------------------------------
// Libera un arbol binario

static void liberar(Nodo *a) {
  if (a!=NULL) {
    liberar(a->izq);
    liberar(a->der);
    free(a);
  }
}

// ----------------------------------------------------
// Crea una copia de un arbol binario

static Nodo *copia(Nodo *a) {
  if (a==NULL)
    return NULL;
 
  Nodo *an= malloc(sizeof(Nodo));
  an->x= a->x;
  an->izq= copia(a->izq);
  an->der= copia(a->der);
  return an;
}

// ----------------------------------------------------
// Crea un arbol binario de busqueda bien equilibrado en donde 
// los x van de i a j

static Nodo *equilibrado(int i, int j) {
  if (i>j)
    return NULL;
  int k= (i+j+1)/2;
  Nodo *a= malloc(sizeof(Nodo));
  a->x= k;
  a->izq= equilibrado(i, k-1);
  a->der= equilibrado(k+1, j);
  return a;
}

// ----------------------------------------------------
// Crea un arbol binario desequilibrado por la izquierda en donde 
// los x van de i a j

static Nodo *desequilibrado_izq(int i, int j) {
  if (i>j) {
    return NULL;
  }
  Nodo *a= malloc(sizeof(Nodo));
  a->x= j;
  a->izq= desequilibrado_izq(i, j-1);
  a->der= NULL;
  return a;
}

// ----------------------------------------------------
// Crea un arbol binario desequilibrado por la derecha en donde 
// los x van de i a j

static Nodo *desequilibrado_der(int i, int j) {
  if (i>j) {
    return NULL;
  }
  Nodo *a= malloc(sizeof(Nodo));
  a->x= i;
  a->izq= NULL;
  a->der= desequilibrado_der(i+1, j);
  return a;
}

int main(int argc, char *argv[]) {
  printf("=====================================\n");
printf("Tests de la funcion eliminar_primeros\n");
printf("=====================================\n\n");
{
    printf("Caso arbol nulo: debe continuar vacio\n");
    Nodo *a= NULL;
    revisar_int(eliminar_primeros(&a, 0), 0);
    revisar_abb(a, NULL);
    revisar_int(eliminar_primeros(&a, 10), 0);
    revisar_abb(a, NULL);
    printf("Caso arbol con un nodo x\n");
    a= abb('x', NULL, NULL);
    Nodo *ref= copia(a);
    printf("Se eliminan 0 nodos: debe seguir x\n");
    revisar_int(eliminar_primeros(&a, 0), 0);
    revisar_abb(a, ref);
    printf("Se eliminan 100 nodos: debe quedar vacio\n");
    revisar_int(eliminar_primeros(&a, 100), 1);
    revisar_abb(a, NULL);
    liberar(ref);
  }

  {
    printf("Caso nodo x con subarbol izquierdo y\n");
    Nodo *y= abb('y', NULL, NULL);
    Nodo *x= abb('x', y, NULL);
    Nodo *ref= abb('x', NULL, NULL);
    Nodo *x2= copia(x);
    printf("Primero se elimina un solo nodo: debe quedar x\n");
    revisar_int(eliminar_primeros(&x, 1), 1);
    revisar_abb(x, ref);
    liberar(x);
    liberar(ref);
    printf("Ahora se eliminan los 2 nodos: debe quedar vacio\n");
    revisar_int(eliminar_primeros(&x2, 2), 2);
    revisar_abb(x2, NULL);
  }

  {
    printf("Caso nodo x con subarbol derecho y\n");
    Nodo *y= abb('y', NULL, NULL);
    Nodo *x= abb('x', NULL, y);
    Nodo *ref= copia(y);
    Nodo *x2= copia(x);
    printf("Primero se elimina un solo nodo: debe quedar y\n");
    revisar_int(eliminar_primeros(&x, 1), 1);
    revisar_abb(x, ref);
    liberar(x);
    liberar(ref);
    printf("Ahora se eliminan los 2 nodos: debe quedar vacio\n");
    revisar_int(eliminar_primeros(&x2, 2), 2);
    revisar_abb(x2, NULL);
  }

  {
    printf("Caso nodo x con subarbol izquierdo 'y' y derecho x\n");
    Nodo *z= abb('z', NULL, NULL);
    Nodo *y= abb('y', NULL, NULL);
    Nodo *x= abb('x', y, z);
    Nodo *ref1= abb('x', NULL, copia(z));
    Nodo *ref2= copia(z);
    Nodo *x2= copia(x);
    Nodo *x3= copia(x);
    printf("Primero se elimina un solo nodo: debe quedar x z\n");
    revisar_int(eliminar_primeros(&x, 1), 1);
    revisar_abb(x, ref1);
    liberar(x);
    liberar(ref1);
    printf("Ahora se eliminan 2 nodos: debe quedar z\n");
    revisar_int(eliminar_primeros(&x2, 2), 2);
    revisar_abb(x2, ref2);
    liberar(x2);
    liberar(ref2);
    printf("Ahora se eliminan los 3 nodos: debe quedar vacio\n");
    revisar_int(eliminar_primeros(&x3, 1000), 3);
    revisar_abb(x3, NULL);
  }

  {
    printf("El ejemplo del enunciado\n");
    Nodo *a= abb('v',
               abb('s',
                 abb('r', NULL, NULL),
                 abb('u', abb('t', NULL, NULL), NULL) ),
               abb('x', abb('w', NULL, NULL), NULL) );
    Nodo *ref= abb('v',
                 abb('u', NULL, NULL),
                 abb('x', abb('w', NULL, NULL), NULL) );
    revisar_int(eliminar_primeros(&a, 3), 3);
    revisar_abb(a, ref);
    liberar(a);
    liberar(ref);
  }

  int n= 100;

  {
    printf("Caso arbol equilibrado de %d nodos\n", n);
    Nodo *a= equilibrado(1, n);
    int quedan= n, elim= 1;
    while (quedan>0) {
      if (elim>quedan) {
        elim= quedan;
      }
      revisar_int(eliminar_primeros(&a, elim), elim);
      quedan -= elim;
      revisar_int(revisar_campos(a, n-quedan+1), n+1);
      elim++;
    }
  }

  {
    printf("Caso arbol desequilibrado por la izquierda de %d nodos\n", n);
    Nodo *a= desequilibrado_izq(1, n);
    int quedan= n, elim= 1;
    while (quedan>0) {
      if (elim>quedan) {
        elim= quedan;
      }
      revisar_int(eliminar_primeros(&a, elim), elim);
      quedan -= elim;
      revisar_des_izq(a, n-quedan+1, n);
      elim++;
    }
  }

  {
    printf("Caso arbol desequilibrado por la derecha de %d nodos\n", n);
    Nodo *a= desequilibrado_der(1, n);
    int quedan= n, elim= 1;
    while (quedan>0) {
      if (elim>quedan) {
        elim= quedan;
      }
      revisar_int(eliminar_primeros(&a, elim), elim);
      quedan -= elim;
      revisar_des_der(a, n-quedan+1, n);
      elim++;
    }
  }

  printf("\n===========================\n");
  printf(  "Tests de la funcion ultimos\n");
  printf(  "===========================\n\n");

  {
    printf("Caso arbol nulo: debe continuar vacio\n");
    Nodo *a= NULL;
    int k= 100;
    revisar_abb(ultimos(a, &k), NULL);
    revisar_int(k, 0);
    printf("Caso arbol con una hoja x\n");
    a= abb('x', NULL, NULL);
    Nodo *ref= copia(a);
    printf("Ultimos 0 nodos: debe quedar vacio\n");
    k= 0;
    revisar_abb(ultimos(a, &k), NULL);
    revisar_int(k, 0);
    revisar_abb(a, ref);
    printf("Ultimos 100 nodos: debe seguir x\n");
    k= 100;
    Nodo *result= ultimos(a, &k);
    revisar_abb(result, ref);
    revisar_abb(a, ref);
    revisar_int(k, 1);
    liberar(a);
    liberar(ref);
    liberar(result);
  }

  {
    printf("Caso nodo x con subarbol izquierdo y\n");
    Nodo *y= abb('y', NULL, NULL);
    Nodo *x= abb('x', y, NULL);
    Nodo *ref= abb('x', NULL, NULL);
    Nodo *x2= copia(x);
    printf("Primero se obtiene el ultimo nodo: debe ser x\n");
    int k= 1;
    Nodo *result= ultimos(x, &k);
    revisar_abb(result, ref);
    revisar_int(k, 1);
    revisar_abb(x, x2);
    liberar(result);
    printf("Ahora se obtienen los 2 ultimos nodos: y x\n");
    k= 2;
    result= ultimos(x, &k);
    revisar_abb(result, x2);
    revisar_int(k, 2);
    revisar_abb(x, x2);
    liberar(result);
    liberar(x);
    liberar(ref);
    liberar(x2);
  }

  {
    printf("Caso nodo x con subarbol derecho y\n");
    Nodo *y= abb('y', NULL, NULL);
    Nodo *x= abb('x', NULL, y);
    Nodo *ref= copia(y);
    Nodo *x2= copia(x);
    printf("Primero se obtiene el ultimo nodo: debe ser y\n");
    int k= 1;
    Nodo *result= ultimos(x, &k);
    revisar_abb(result, ref);
    revisar_int(k, 1);
    revisar_abb(x, x2);
    liberar(result);
    printf("Ahora se obtienen los 2 ultimos nodos: x y\n");
    k= 2;
    result= ultimos(x, &k);
    revisar_abb(result, x2);
    revisar_int(k, 2);
    revisar_abb(x, x2);
    liberar(result);
    liberar(x);
    liberar(ref);
    liberar(x2);
  }

  {
    printf("Caso nodo x con subarbol izquierdo 'y' y derecho z\n");
    Nodo *z= abb('z', NULL, NULL);
    Nodo *y= abb('y', NULL, NULL);
    Nodo *x= abb('x', y, z);
    Nodo *ref1= abb('x', NULL, copia(z));
    Nodo *ref2= copia(z);
    Nodo *x3= copia(x);
    printf("Primero se obtiene el ultimo nodo: debe ser z\n");
    int k= 1;
    Nodo *result= ultimos(x, &k);
    revisar_abb(result, ref2); // Tiene que ser igual a z
    revisar_int(k, 1);
    revisar_abb(x, x3);
    liberar(result);
    printf("Ahora se obtienen los 2 ultimos nodos: x z\n");
    k= 2;
    result= ultimos(x, &k);
    revisar_abb(result, ref1);
    revisar_int(k, 2);
    revisar_abb(x, x3);
    liberar(result);
    printf("Ahora se obtienen los 3 ultimos nodos: y x z\n");
    k= 3;
    result= ultimos(x, &k);
    revisar_abb(result, x3);
    revisar_int(k, 3);
    revisar_abb(x, x3);
    liberar(result);
    liberar(x);
    liberar(ref1);
    liberar(ref2);
    liberar(x3);
  }

  {
    printf("El ejemplo del enunciado\n");
    Nodo *a= abb('v',
               abb('s',
                 abb('r', NULL, NULL),
                 abb('u', abb('t', NULL, NULL), NULL) ),
               abb('x', abb('w', NULL, NULL), NULL) );
    Nodo *a2= copia(a);
    Nodo *ref= abb('v',
                 abb('u', NULL, NULL),
                 abb('x', abb('w', NULL, NULL), NULL) );
    int k= 4;
    Nodo *result= ultimos(a, &k);
    revisar_abb(result, ref);
    revisar_int(k, 4);
    revisar_abb(a, a2);
    liberar(result);
    liberar(a);
    liberar(a2);
    liberar(ref);
  }

  n= 100;

  {
    printf("Caso arbol equilibrado de %d nodos\n", n);
    Nodo *a= equilibrado(1, n);
    Nodo *a2= copia(a);
    for (int i= 0; i<=n; i++) {
      int k= i;
      Nodo *result= ultimos(a, &k);
      revisar_int(revisar_campos(result, n-i+1), n+1);
      revisar_int(k, i);
      revisar_abb(a, a2);
      liberar(result);
    }
    liberar(a);
    liberar(a2);
  }

  {
    printf("Caso arbol desequilibrado por la izquierda de %d nodos\n", n);
    Nodo *a= desequilibrado_izq(1, n);
    Nodo *a2= copia(a);
    for (int i= 0; i<=n; i++) {
      int k= i;
      Nodo *result= ultimos(a, &k);
      revisar_des_izq(result, n-i+1, n);
      revisar_int(k, i);
      revisar_abb(a, a2);
      liberar(result);
    }
    liberar(a);
    liberar(a2);
  }

  {
    printf("Caso arbol desequilibrado por la derecha de %d nodos\n", n);
    Nodo *a= desequilibrado_der(1, n);
    Nodo *a2= copia(a);
    for (int i= 0; i<=n; i++) {
      int k= i;
      Nodo *result= ultimos(a, &k);
      revisar_des_der(result, n-i+1, n);
      revisar_int(k, i);
      revisar_abb(a, a2);
      liberar(result);
    } 
    liberar(a);
    liberar(a2);
  }


#ifndef OPT

  printf("Felicitaciones: su solucion es correcta\n");
  printf("No olvide verificar que es eficiente con make test-O\n");

#else

  n= 50000;
  int r= 100;
  Nodo *eq_array[r];
  Nodo *des_izq_array[r];
  Nodo *des_der_array[r];

  for (int i= 0; i<r; i++) {
    eq_array[i]= equilibrado(1, n);
    des_izq_array[i]= desequilibrado_izq(1, n);
    des_der_array[i]= desequilibrado_der(1, n);
  }

  printf("\n=========\n"); 
  printf(  "Benchmark\n"); 
  printf(  "=========\n\n"); 

  printf("Midiendo tiempo para la funcion eliminar_primeros\n");
  resetTime();
  for (int i= 0; i<r; i++) {
    Nodo *eq= eq_array[i];
    Nodo *des_izq= des_izq_array[i];
    Nodo *des_der= des_der_array[i];

    int quedan= n, elim= 1;
    while (quedan>0) {
      if (elim>quedan) {
        elim= quedan;
      }
      revisar_int(eliminar_primeros(&eq, elim), elim);
      revisar_int(eliminar_primeros(&des_izq, elim), elim);
      revisar_int(eliminar_primeros(&des_der, elim), elim);
      quedan -= elim;
      elim++;
    }
  }

  int tiempo_eliminar= getTime();

  printf("Tiempo para eliminar_primeros= %d\n", tiempo_eliminar);

  n= 10000;

  Nodo *eq= equilibrado(1, n);
  Nodo *des_izq= desequilibrado_izq(1, n);
  Nodo *des_der= desequilibrado_der(1, n);

  printf("Midiendo tiempo para la funcion ultimos\n");

  resetTime();
  {
    for (int j= 0; j<=n; j++) {
      int k= j;
      Nodo *result= ultimos(eq, &k);
      revisar_int(k, j);
      liberar(result);
      result= ultimos(des_izq, &k);
      revisar_int(k, j);
      liberar(result);
      result= ultimos(des_der, &k);
      revisar_int(k, j);
      liberar(result);
    } 
  }

  int tiempo_ultimos= getTime();

  liberar(eq);
  liberar(des_izq);
  liberar(des_der);

  printf("Tiempo para ultimos= %d\n", tiempo_ultimos);
  
  if (argc==2 && strcmp(argv[1], "comparar")==0) {
    printf("\n**********************************************************\n");
    printf(  "Ejecutando la solucion eficiente del profesor\n");
    printf(  "El tiempo quedara almacenado en el archivo tiempo_prof.txt\n");
    printf(  "**********************************************************\n\n");
    system("rm -f tiempo_prof.txt ; ./sol-profesor tiempo_prof.txt > /dev/null");
    FILE *arch_prof= fopen("tiempo_prof.txt", "r");
    if (arch_prof==NULL) {
      perror("tiempo_prof.txt");
      fprintf(stderr,
              "Algo inesperado sucedio: no se pudo leer tiempo_prof.txt\n");
      exit(1);
    }
    int tiempo_prof_eliminar= -1;
    int tiempo_prof_ultimos= -1;
    fscanf(arch_prof, "%d", &tiempo_prof_eliminar);
    fscanf(arch_prof, "%d", &tiempo_prof_ultimos);
    fclose(arch_prof);
    if (tiempo_prof_eliminar==-1 || tiempo_prof_ultimos==-1) {
      fprintf(stderr,
              "Algo inesperado sucedio: no se pudo leer tiempo_prof.txt\n");
      exit(1);
    }


    printf("\n===========================\n");
    printf(  "Resumen\n");
    printf(  "===========================\n\n");
    printf("La solucion del profesor de eliminar_primeros tomo %d milisegundos\n",
           tiempo_prof_eliminar);
    printf("La solucion del profesor de ultimos tomo %d milisegundos\n",
           tiempo_prof_ultimos);
    double cuociente_eliminar= (double)tiempo_eliminar/
                                       tiempo_prof_eliminar;
    printf("Cuociente entre el tiempo de su solucion de eliminar_primeros y la del profesor es %f\n",
           cuociente_eliminar);
    double cuociente_ultimos= (double)tiempo_ultimos/tiempo_prof_ultimos;
    printf("Cuociente entre el tiempo de su solucion de ultimos y la del profesor es %f\n",
           cuociente_ultimos);
    if (cuociente_eliminar>=2.0) {
      fprintf(stderr,
              "Su solucion de eliminar_primeros es correcta, pero demasiado\n");
      fprintf(stderr, "ineficiente sera rechazada\n");
      exit(2);
    }
    if (cuociente_ultimos>=2.0) {
      fprintf(stderr,
              "Su solucion de ultimos es correcta, pero demasiado\n");
      fprintf(stderr, "ineficiente sera rechazada\n");
      exit(2);
    }
    printf("Felicitaciones: su solucion es correcta y eficiente\n");
  }
  else if (argc==2) {
    FILE *arch_prof= fopen(argv[1], "w");
    fprintf(arch_prof, "%d %d\n", tiempo_eliminar, tiempo_ultimos);
    fclose(arch_prof);
  }
  else {
    printf("Felicitaciones: su solucion es correcta\n");
    printf("No olvide verificar que es eficiente con make test-O\n");
  }

#endif

  return 0;
}

