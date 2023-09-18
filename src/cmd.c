#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cmd.h"



int cnt_cmd(char *buffer){
  int ret = 0;
  for(size_t i = 0;buffer[i]!=0;i++){
    ret += (buffer[i]==' ') ? 1 : 0;
  }
  return ret;
}

//utiliza malloc para copiar as strings de argumentos
void fill_cmd(Cmd *c,char *buffer){
  int argc = 0;
  c->argv = malloc((cnt_cmd(buffer) + 1) * sizeof(char**)); 
  //numero de espacos + 1 para poder terminar com NULL
  char* tmp;
  for (
    *(c->argv+argc) = strdup(strtok(buffer," \n\t")); 
    *(c->argv+argc) != NULL;
    *(c->argv + ++argc) = (tmp = strtok(NULL," \n\t")) ? strdup(tmp) : NULL
  );
  c->argc = argc;
}

void print_cmd(Cmd *c){
  for(size_t i =0; i<c->argc;i++){
    printf("%s\n",*(c->argv+i));
  }
}

void free_cmd(Cmd *c){
  for(size_t i = 0; i<c->argc;i++){
    free(*(c->argv+i)),*(c->argv+i)=NULL;
  }
  free(c->argv),c->argv=NULL;
}


