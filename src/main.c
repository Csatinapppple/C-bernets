#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "cmd.h"
#include "pipes.h"

#define ARR_SIZE(x) (sizeof(x)/sizeof(x[0]))

/*
     pipe 1  pipe 2
  curl | grep | wc

  é preciso saber para aonde redirecionar a saida de um programa para o outro programa a espera
  tambem e preciso se ter um lugar para guardar o output de um programa quando nao se tem mais
  pipes disponiveis.


protocolo de comunicacao de escalonador para trabalhador

0x0 = um inteiro indicando se a algo no buffer, 1 para presente 0 para nao presente
0x1 ate 0x200 = uma serie de chars indicando o comando desejado

protocolo de comunicacao de trabalhador para escalonador

0x0 = tipo de mensagem (fim de trabalho, )
0x1 = um inteiro indicando um pipe livre
*/



void trabalho1(int fd[2]){
  static Pipe p[3];
  Cmd c;
  char buf[BUFSIZ];
  while(true){
    read(fd[READ],buf,BUFSIZ);
    switch (buf[0]) {
      case 1:
        fill_cmd(&c,buf+2);
        if(fork()==0){
          

          execlp("ls","ls",NULL);
        }
        break;
      default:
        break;
    }
  }

}

int main(char **argv, int argc){

  char buffer[BUFFER_CMD]="\1\2ls -l -a -w";
  
  printf(buffer);

  Cmd c;

  return 0;
}
