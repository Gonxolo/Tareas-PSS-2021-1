#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// errno.h se necesita para poder usar la variable errno

int WORD_LENGTH = 10;
int DEF_LENGTH = 69;
// + \n = 80 caracteres
int ANCHO = 80;

int found = -1;

int main(int argc, char *argv[]) {
  char objetivo[WORD_LENGTH+1];
  if (argc!=3) {
    fprintf(stderr, "Uso: %s <archivo> <palabra>\n", argv[0]);
    return 1;
  }

  FILE *f= fopen(argv[1], "r+");
  if (f==NULL) {
    int rc = errno;
    perror(argv[1]);
    exit(rc);
  }

  char buf[ANCHO + 1];
  char reemplazo[ANCHO + 1];
  size_t rc;
  int leidos = 0;
  char *fin;

  int n = strlen(argv[2]);

  if (n<WORD_LENGTH){
    strcpy(objetivo, argv[2]);
    for(int i=n; i<WORD_LENGTH; i++){
      objetivo[i] = ' ';
    }
    objetivo[WORD_LENGTH] = '\0';
  }
  else{
    strcpy(objetivo,argv[2]);
    objetivo[WORD_LENGTH] = '\0';
  }
  
  while ((rc= fread(buf, ANCHO, 1, f))==1){
    if (found < 0){
      fin = buf;
      for(int i=0; i<10; i++)
        fin++;
      char guardado;
      guardado = *fin;
      *fin=0;
      if(strcmp(objetivo, buf)==0){
        *fin = guardado;
        found = leidos;
      }
      *fin = guardado;
    }

    if (*buf==' ')
      break;
    leidos++;

  }

  if (found>=0){
    fseek(f, ANCHO*(leidos-1), SEEK_SET);
    fread(reemplazo, ANCHO, 1, f);
    fseek(f, ANCHO*found, SEEK_SET);
    fwrite(reemplazo, ANCHO, 1, f);
    fseek(f, ANCHO*(leidos-1), SEEK_SET);
    for (int i = 0; i < ANCHO-1; i++){
      putc(' ',f);
    }
    
  }

  fclose(f); // Si no cierra el archivo, valgrind reportara fugas
  return 0;
}

