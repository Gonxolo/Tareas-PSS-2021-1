#define _DEFAULT_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include "BigInt.h"

#include "ultdir.h"

typedef struct {
  int n;
  BigInt_t *bits;
} BigNum;

static BigNum *smallNum(BigInt_t x);
static void freeBigNum(BigNum *bx);
static BigNum *bigMul(BigNum *bx, BigNum *by);
static BigNum *seqArrayProd(int a[], int i, int j);

// ----------------------------------------------------
// Funcion que entrega el tiempo transcurrido desde el lanzamiento del
// programa en milisegundos

static long long getUSecsOfDay() {
  struct timeval Timeval;
  gettimeofday(&Timeval, NULL);
  return (long long)Timeval.tv_sec*1000000+Timeval.tv_usec;
}

static int time0= 0;

static int getTime0() {
    return getUSecsOfDay()/1000;
}

static void resetTime() {
  time0= getTime0();
}

static int getTime() {
  return getTime0()-time0;
}

// ----------------------------------------------------
// Funciones para trabajar con big numbers

static BigNum *smallNum(BigInt_t x) {
  BigNum *bx= malloc(sizeof(BigNum));
  bx->n= 1;
  bx->bits= malloc(sizeof(BigInt_t));
  bx->bits[0]= x;
  return bx;
}

static void freeBigNum(BigNum *bx) {
  free(bx->bits);
  free(bx);
}

static BigNum *newBigNum(int n, BigInt_t *bits) {
  BigNum *bx= malloc(sizeof(BigNum));
  int nadj= n;
  while (nadj>0 && bits[nadj-1]==0) {
    nadj--;
  }
  bx->n= nadj;
  if (n==nadj)
    bx->bits= bits;
  else {
    BigInt_t *bitsadj= malloc(nadj*sizeof(BigInt_t));
    for (int i= 0; i<nadj; i++) {
      bitsadj[i]= bits[i];
    }
    bx->bits= bitsadj;
    free(bits);
  }
  return bx;
}

int bigCmp(BigNum *bx, BigNum *by) {
  if (bx->n!=by->n)
    return bx->n - by->n;
  else
    return BigInt_cmp(bx->n, bx->bits, by->bits);
}

#if 1

// This uses the fast Karatsuba multiplication algorithm.  
// Below is my own more intuitive but slow algorithm.
// If you want to test how slow is, change the #if 1 by #if 0
// Compare with time ./test-prod 100000 for both algorithms.
static BigNum *bigMul(BigNum *bx, BigNum *by) {
  int n= bx->n+by->n;
  BigInt_t *bits= malloc(n*sizeof(BigInt_t));
  BigInt_mul(bx->n, bx->bits, by->n, by->bits, n, bits);

  return newBigNum(n, bits);
}

#else

static BigNum *bigMul(BigNum *bx, BigNum *by) {
  int n= bx->n+by->n;
  BigInt_t *bits= malloc(n*sizeof(BigInt_t));
  for (int i= 0; i<n; i++)
    bits[i]= 0;
  BigInt_t carry= 0;
  for (int i= 0; i<by->n; i++) {
    int pos= i;
    for (int j= 0; j<bx->n; j++) {
      BigInt_tmp_t prod= (BigInt_tmp_t)bits[pos] + (BigInt_tmp_t)carry;
      prod += (BigInt_tmp_t)bx->bits[j] * (BigInt_tmp_t)by->bits[i];
      // prod += (BigInt_tmp_t)bx->bits[j] * by->bits[i];
      bits[pos]= prod;
      carry= prod>>(8*sizeof(BigInt_t));
      pos++;
    }
    if (pos<n) {
      bits[pos]= carry;
      carry= 0;
    }
  }
  if (carry!=0) {
    fprintf(stderr, "Asercion fallida: ultimo carry no fue 0\n");
    exit(1);
  }
  
  return newBigNum(n, bits);
}

#endif

// ----------------------------------------------------
// Version secuencial del producto de los elementos de
// un arreglo

static int verbose= 0;

static BigNum *recArrayProd(int a[], int i, int j) {
  if (i>j) {
    fprintf(stderr, "Asercion fallida: i > j\n");
    exit(1);
  }
  if (i==j) {
    return smallNum(a[i]);
  }
  else {
    int h= (i+j)/2;
    BigNum *left= recArrayProd(a, i, h);
    BigNum *right= recArrayProd(a, h+1, j);
    BigNum *prod= bigMul(left, right);
    freeBigNum(left);
    freeBigNum(right);
    return prod;
  }
}

static BigNum *seqArrayProd(int a[], int i, int j) {
  if (verbose) {
    printf("Llamada secuencial con i=%d j=%d\n", i, j);
    fflush(stdout);
  }
  return recArrayProd(a, i, j);
}

#if 0
// ----------------------------------------------------
// Calculo del factorial

// This iterative algorithm to compute factorial is simple and intuitive,
// but it is slow for big numbers
static BigNum *slowBigFact(int n) {
  BigNum *bp= smallNum(1);
  for (int i= 1; i<=n; i++) {
    BigNum *bi= smallNum(i);
    BigNum *newbp= bigMul(bp, bi);
    freeBigNum(bp);
    freeBigNum(bi);
    bp= newbp;
  }
  return bp;
}
#endif

// This is the recursive divide and conquer algorithm.  It happens to
// be way faster than the iterative algorithm.  Why?
static BigNum *fastBigFact(int n) {
  if (n==0)
    return smallNum(0);

  int *a= malloc(n*sizeof(int));

  // Fill a with 1, 2, 3, ..., n
  for (int i= 0; i<n; i++) {
    a[i]= i+1;
  }

#if 1
  // Filling a with 1, 2, 3, ..., n would work but the product of the first
  // half of the array would be much smaller than the second half,
  // making an unbalanced size of big numbers.
  // So the the array is filled with a permutation of 1, 2, 3, ..., n
  // to mix randomly small numbers with big numbers.
  // If you want to experiment without the permutation, change the
  // the #if 1 by #if 0
  // Try with ./test-prod 100000. It does segmentation fault because
  // of stack overflow.

  // Do a random permutation to balance the size of the numbers
  for (int i= 0; i<n; i++) {
    int r= random()%(n-i) + i;
    int tmp= a[i];
    a[i]= a[r];
    a[r]= tmp;
  }
#endif

  // Compute big product of array numbers
  BigNum *bf= seqArrayProd(a, 0, n-1);    // sequential
  free(a);
  return bf;
}

// ----------------------------------------------------
// Conversion de un big number a un string hexadecimal
// (convertir a decimal tomaria demasiado tiempo)

static char *bigNum2HexString(BigNum *bx) {
  char *str= malloc(bx->n*sizeof(BigInt_t)*2+1);
  char *res= str;
  int imax= sizeof(BigInt_t)*2;
  sprintf(str, "%lx", bx->bits[bx->n-1]);
  str += strlen(str);

  for (int k= bx->n-2; k>=0; k--) {
    BigInt_t w= bx->bits[k];
    for (int i= 1; i<=imax; i++) {
      int hex= w & 0x0f;
      str[imax-i]= hex>=10 ? 'a'+hex-10 : '0'+hex;
      w >>= 4;
    }
    str += imax;
  }
  *str= 0;
  return res;
}

int main(int argc, char *argv[]) {
  if (argc>2) {
    fprintf(stderr, "Usage: %s <number>\n", argv[0]);
    exit(1);
  }
  if (argc==2) {
    int n= atoi(argv[1]);
#if 1
    BigNum *bf= fastBigFact(n);
#else
    // If you want to experiment with the slow version
    // change the #if 1 by #if 0
    // Compare with time ./test-prod 100000
    BigNum *bf= slowBigFact(n);
#endif
    char *bigstr= bigNum2HexString(bf);
    printf("%s\n", bigstr);
    freeBigNum(bf);
    free(bigstr);
    char *u= ultimaDireccionValida(bigstr);
    printf("Ultima direccion valida es %p\n", u);
  }
  else {
    for (int n= 1000; n<=50000; n +=100) {
      resetTime();
      BigNum *bf= fastBigFact(n);
      int time= getTime();
      char *bigstr= bigNum2HexString(bf);
      if (n%1000==0) {
        printf("Calculando factorial de %d\n", n);
        printf("Numero de digitos hexadecimales = %ld\n", strlen(bigstr));
        printf("Tiempo de calculo = %d milisegundos\n", time);
        char *u= ultimaDireccionValida(bigstr);
        printf("Direccion de bigstr= %p\n", bigstr);
        printf("Ultima direccion valida = %p\n", u);
        char *s= sbrk(0);
        printf("Ultima direccion reportada por sbrk(0) = %p\n", s);
        if (u+1!=s) {
          fprintf(stderr, "El valor retornado por ultimaDireccionValida "
                  "no es consistente con sbrk\n");
          exit(1);
        }
        printf("-------------------------------------------\n");
      }
    }
  }

  printf("Felicitaciones: aprobo este modo de ejecucion\n");

  return 0;
}
