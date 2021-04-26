#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "t2.h"

int aislar_palabras(char *s) {

  int n = 0;
  int hay_espacio = 1;
  char *aislado = s;
  
  for (char *comparador = s; *comparador != '\0'; comparador++){
    
    if (( 'a'<=*comparador && *comparador<='z') || ('A'<=*comparador && *comparador<='Z')){
      if(!n){
        n++;
      }
      else if (hay_espacio){
        n++;
        *aislado = ' ';
        aislado++;
      }
      *aislado = *comparador;
      aislado++;
      hay_espacio = 0;
    }
    else{
      hay_espacio = 1;
    }
  }

  *aislado = '\0';

  return n;

}

char *palabras(char *s_orig) {
  
  int n = 0;
  char *aislado = s_orig;
  int hay_espacio = 1;
  
  for (s_orig; *s_orig != '\0'; s_orig++){
    
    if (( 'a'<=*s_orig && *s_orig<='z') || ('A'<=*s_orig && *s_orig<='Z')){
      if(hay_espacio){
        n++;
      }
      n++;
      hay_espacio = 0;

    }
    else{
      hay_espacio = 1;
    }
  }

  if(!n){
    n++;
  }

  s_orig = aislado;

  aislado = (char *) malloc(n * sizeof(char));

  char* resultado = aislado;

  n = 0;

  for (char *comparador = s_orig; *comparador != '\0'; comparador++){
    
    if (( 'a'<=*comparador && *comparador<='z') || ('A'<=*comparador && *comparador<='Z')){
      if(!n){
        n++;
      }
      else if (hay_espacio){
        n++;
        *aislado = ' ';
        aislado++;
      }
      *aislado = *comparador;
      aislado++;
      hay_espacio = 0;
    }
    else{
      hay_espacio = 1;
    }
  }

  *aislado = '\0';

  return resultado;

}
