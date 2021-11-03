#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prod.h"

BigNum *parArrayProd(int a[], int i, int j, int p) {

  if (i==j) return smallNum(a[i]);
  if (p==1) return seqArrayProd(a, i, j);
  else {
    BigNum *product;
    int k = (i+j)/2;
    int fd[2];
    pipe(fd);
    pid_t pid= fork();
    if (pid==0) {
      close(fd[0]);
      BigNum *left = parArrayProd(a, i, k, p/2);
      write(fd[1], &left->n, sizeof(int));
      write(fd[1], left->bits, left->n*sizeof(BigInt_t));
      exit(0);
    }
    else {
      close(fd[1]);
      BigNum *right = parArrayProd(a, k+1, j, p-p/2);
      BigNum *left = malloc(sizeof(BigNum));
      leer(fd[0], &left->n, sizeof(int));
      left->bits = malloc(left->n*sizeof(BigInt_t));
      leer(fd[0], left->bits, left->n*sizeof(BigInt_t));
      
      product = bigMul(left,right);

      freeBigNum(left);
      freeBigNum(right);

      close(fd[0]);

      waitpid(pid, NULL, 0);
    }
    return product;
  }
}
