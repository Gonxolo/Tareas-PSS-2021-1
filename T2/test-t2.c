#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include "t2.h"

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
// Revisar: verifica que un string es un valor especifico

void revisar(char *s, char *valor) {
  if (strcmp(s, valor)!=0) {
    fprintf(stderr, "El string debio ser \"%s\", no \"%s\"\n", valor, s);
    exit(1);
  }
}

void revisar_int(int n, int nref) {
  if (n!=nref) {
    fprintf(stderr, "El valor retornado debio ser %d, no %d\n", nref, n);
    exit(1);
  }
}

// ----------------------------------------------------
// revisar_mutable: verifica correctitud del resultado y
// que no se alteren los alrededores del string

void revisar_mutable(char *s, char *referencia, int nref) {
  int len= strlen(s);
  char a[len+1000+1];
  for (int i= 0; i<500; i++)
    a[i]= ~((i+34)%256);
  strcpy(&a[500], s);
  for (int i= 0; i<500; i++)
    a[500+i+len+1]= ~((i+34)%256);

  int n= aislar_palabras(&a[500]);

  revisar(&a[500], referencia);
  revisar_int(n, nref);
  for (int i= 0; i<500; i++) {
    if (a[i]!= (char)~((i+34)%256)) {
      fprintf(stderr, "aislar_palabras escribio fuera del string\n");
      exit(1);
    }
  }

  for (int i= 0; i<500; i++) {
    if (a[500+i+len+1]!= (char)~((i+34)%256)) {
      fprintf(stderr, "aislar_palabras escribio fuera del string\n");
      exit(1);
    }
  }
}

// ----------------------------------------------------
// revisar_inmutable: verifica correctitud del resultado

void revisar_inmutable(char *s, char *referencia) {
  char *res= palabras(s);
  revisar(res, referencia);
  free(res);
}

int main(int argc, char *argv[]) {
  printf("Verificando funcion palabras\n");
  revisar_inmutable("", "");
  revisar_inmutable(" ", "");
  revisar_inmutable("  ", "");
  revisar_inmutable("        ", "");
  revisar_inmutable("a", "a");
  revisar_inmutable("Z ", "Z");
  revisar_inmutable("  z ", "z");
  revisar_inmutable("abcdefghijklmnopqrstuvwxyz", "abcdefghijklmnopqrstuvwxyz");
  revisar_inmutable("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  revisar_inmutable("0123456789", "");
  revisar_inmutable("!·$%&/()=?¿,;.:-[]{}+", "");
  revisar_inmutable("a123bc4def56g7", "a bc def g");
  revisar_inmutable("   a   bc def  g ", "a bc def g");
  revisar_inmutable("  return ('a'<=ch && ch<='z') || ('A'<=ch && ch<='Z');",
                    "return a ch ch z A ch ch Z");

  printf("Bien: aprobo los tests de la funcion palabras\n");
  printf("Verificando funcion aislar_palabras\n");
  revisar_mutable("", "", 0);
  revisar_mutable(" ", "", 0);
  revisar_mutable("  ", "", 0);
  revisar_mutable("        ", "", 0);
  revisar_mutable("a", "a", 1);
  revisar_mutable("Z ", "Z", 1);
  revisar_mutable("  z ", "z", 1);
  revisar_mutable("abcdefghijklmnopqrstuvwxyz",
                  "abcdefghijklmnopqrstuvwxyz", 1);
  revisar_mutable("ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                  "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 1);
  revisar_mutable("0123456789", "", 0);
  revisar_mutable("!·$%&/()=?¿,;.:-[]{}+", "", 0);
  revisar_mutable("a123bc4def56g7", "a bc def g", 4);
  revisar_mutable("   a   bc def  g ", "a bc def g", 4);
  revisar_mutable("  return ('a'<=ch && ch<='z') || ('A'<=ch && ch<='Z');",
                    "return a ch ch z A ch ch Z", 9);
  
  printf("Bien: aprobo los tests de la funcion aislar_palabras\n");

  printf("Se medira la eficiencia de la funcion palabras\n");
  printf("Se verificara que palabras no pida mas memoria que la necesaria.\n");
  printf("Si Ud. no es eficiente en el uso de la memoria, se acabara la\n");
  printf("memoria y tomara mucho tiempo hasta que termine en seg. fault.\n");
  int tiempo_palabras;
  int reps= 1000;
#ifdef REPS
  reps= REPS;
#endif
  {
    int size= 1000000;
    printf("Apareceran %d puntos.\n", reps/100);
    char *big= malloc(size+1);
    char *sims= "!·$%&/()=?¿,;.:-[]{}+0123456789";
    int sim_len= strlen(sims);
    char *alfas= "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int alfa_len= strlen(alfas);
    char **resultados= malloc(reps*sizeof(char*));
    for (int i= 0; i<size; i++)
      big[i]= sims[i%sim_len];
    big[size]= 0;
    resetTime();
    for (int i= 0; i<reps; i++) {
      int pos= rand()%size;
      char save= big[pos];
      char c= alfas[i%alfa_len];
      big[pos]= c;
      resultados[i]= palabras(big);
      if (resultados[i][0]!=c || resultados[i][1]!=0) {
        fprintf(stderr, "Resultado incorrecto\n");
        exit(1);
      }
      big[pos]= save;
      if (i%100==0) {
        printf(".");
        fflush(stdout);
      }
    }
    tiempo_palabras= getTime();
    for (int i= 0; i<reps; i++) {
      free(resultados[i]);
    }
    free(resultados);
    free(big);
  }
  printf("\nTiempo para palabras= %d\n", tiempo_palabras);

  printf("Se medira la eficiencia de la funcion aislar_palabras\n");
  int tiempo_aislar;
  reps *= 600;
  {
    char *tests[]= {
        "",
        " ",
        "  ",
        "        ",
        "a",
        "Z ",
        "  z ",
        "abcdefghijklmnopqrstuvwxyz",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
        "0123456789",
        "!·$%&/()=?¿,;.:-[]{}+",
        "a123bc4def56g7",
        "   a   bc def  g ",
        "  return ('a'<=ch && ch<='z') || ('A'<=ch && ch<='Z');",
        NULL
    };
    int tam= 0;
    while (tests[tam]!=NULL)
      tam++;
    char **copias= malloc(tam*sizeof(char*));
    for (int j= 0; j<tam; j++) {
      copias[j]= malloc(strlen(tests[j])+1);
    }
    resetTime();
    int cnt= 0;
    for (int i= 0; i<reps; i++) {
      for (int j= 0; j<tam; j++) {
        strcpy(copias[j], tests[j]);
        cnt += aislar_palabras(copias[j]);
      }
      if (i%(100*600)==0) {
        printf(".");
        fflush(stdout);
      }
    }
    tiempo_aislar= getTime();
    for (int j= 0; j<tam; j++) {
      free(copias[j]);
    }
    free(copias);
  }
  printf("\nTiempo para aislar_palabras= %d\n", tiempo_aislar);

  if (argc==2 && strcmp(argv[1], "comparar")==0) {
    printf("\n**********************************************************\n");
    printf(  "Ejecutando la solucion eficiente del profesor\n");
    printf(  "El tiempo quedara almacenado en el archivo tiempo_prof.txt\n");
    printf(  "**********************************************************\n\n");
    system("rm -f tiempo_prof.txt ; ./sol-profesor tiempo_prof.txt");
    FILE *arch_prof= fopen("tiempo_prof.txt", "r");
    if (arch_prof==NULL) {
      perror("tiempo_prof.txt");
      fprintf(stderr,
              "Algo inesperado sucedio: no se pudo leer tiempo_prof.txt\n");
      exit(1);
    }
    int tiempo_prof_palabras= -1;
    int tiempo_prof_aislar= -1;
    fscanf(arch_prof, "%d", &tiempo_prof_palabras);
    fscanf(arch_prof, "%d", &tiempo_prof_aislar);
    fclose(arch_prof);
    if (tiempo_prof_palabras==-1 || tiempo_prof_aislar==-1) {
      fprintf(stderr,
              "Algo inesperado sucedio: no se pudo leer tiempo_prof.txt\n");
      exit(1);
    }

    printf("La solucion del profesor de palabras tomo %d milisegundos\n",
           tiempo_prof_palabras);
    printf("La solucion del profesor de aislar_palabras tomo %d milisegundos\n",
           tiempo_prof_aislar);
    double cuociente_palabras= (double)tiempo_palabras/tiempo_prof_palabras;
    printf("Cuociente entre el tiempo de su solucion de palabras y la del profesor es %f\n",
           cuociente_palabras);
    double cuociente_aislar= (double)tiempo_aislar/tiempo_prof_aislar;
    printf("Cuociente entre el tiempo de su solucion de aislar_palabras y la del profesor es %f\n",
           cuociente_aislar);
    if (cuociente_palabras>=2.0) {
      fprintf(stderr,
              "Su solucion de palabras es correcta, pero demasiado\n");
      fprintf(stderr, "ineficiente sera rechazada\n");
      exit(2);
    }
    if (cuociente_aislar>=2.0) {
      fprintf(stderr,
              "Su solucion de aislar_palabras es correcta, pero demasiado\n");
      fprintf(stderr, "ineficiente sera rechazada\n");
      exit(2);
    }
    
    printf("\nFelicitaciones: su solucion es correcta y eficiente\n");
  }
  else if (argc==2) {
    FILE *arch_prof= fopen(argv[1], "w");
    fprintf(arch_prof, "%d %d\n", tiempo_palabras, tiempo_aislar);
    fclose(arch_prof);
  }
  else {
    printf("\nFelicitaciones: su solucion es correcta\n");
    printf("No olvide probar su tarea con: make test-O\n");
    printf("para revisar que sea eficiente\n");
  }

  return 0;
}
