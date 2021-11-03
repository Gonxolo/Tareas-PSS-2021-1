#define _POSIX_C_SOURCE 1
#include <sys/types.h>
#include <setjmp.h>
#include <signal.h>

#include "ultdir.h"

typedef void (*sighandler_t)();
sigjmp_buf jmpbuf;

void handler(int signal){
  siglongjmp(jmpbuf, 1);
}

volatile char * lastPos;
char reader;

char *ultimaDireccionValida(char *ptr) {
  
  void (*hdlr)();
  hdlr = signal(SIGSEGV, handler);

  if(sigsetjmp(jmpbuf, 1)==0){
    while(1==1){
      lastPos=ptr;
      reader=*ptr;
      hdlr = signal(SIGSEGV, hdlr);
      ptr++;
    }
  }
  else{
    lastPos--;
    hdlr = signal(SIGSEGV, hdlr);
    return (char *) lastPos;
  }

}
