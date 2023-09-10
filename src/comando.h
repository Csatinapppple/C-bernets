#ifndef COMANDO_H_
#define COMANDO_H_

#include <string.h>
#include <stdio.h>

#define MAX_ARG_COMMAND 16
#define BUFFER_COMMAND 255

struct comando{
  char *argv[MAX_ARG_COMMAND];
  int argc;
};

typedef struct comando Comando;

extern void preenche_argumentos(Comando *c, char* buffer);

extern void imprime_comandos(Comando *c);

#endif

