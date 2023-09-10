#include <string.h>
#include <stdio.h>
#include "comando.h"

void preenche_argumentos(Comando *c,char *buffer){
  int argc = 0;
  for (
    *(c->argv+argc) = strtok(buffer," \n\t"); 
    *(c->argv+argc) != NULL;
    *(c->argv + ++argc) = strtok(NULL," \n\t")
  );
  c->argc = argc;
}

void imprime_comandos(Comando *c){
  for(size_t i =0; i<c->argc;i++){
    printf("%s\n",*(c->argv+i));
  }
}
