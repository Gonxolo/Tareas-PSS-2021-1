#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  printf("Soy un duende que tiene un gemelo malvado\n");
  pid_t child= fork();
  if (child==0) {
    printf("Soy el hijo del duende y no tengo un gemelo\n");
    exit(0);
  }
  else {
    waitpid(child, NULL, 0);
  }

  return 0;
}
