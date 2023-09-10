#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "comando.h"

#define READ 0
#define WRITE 1

typedef struct {
  int e[2];
}Pipe;



int main(){
 
  FILE *f = fopen()

  char buffer[BUFFER_COMMAND];

  Comando c;

  puts("insira o comando ");
  fgets(buffer,BUFFER_COMMAND,stdin);

  preenche_argumentos(&c, buffer);


  printf("argc = %d\n",c.argc);

  imprime_comandos(&c);
  

  execvp(*c.argv,c.argv);

  return 0;
}
