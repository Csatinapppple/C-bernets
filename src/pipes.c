#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include "pipes.h"


void open_pipes(Pipe *p, size_t n){
  for(size_t i = 0; i<n;i++){
    pipe((*(p+i)).fd);
  }
}

void print_pipes(Pipe *p, size_t n){
  for(size_t i = 0; i<n;i++){
    printf("pipe #%ld, READ[0]=%d, WRITE[1]=%d\n",i,(p+i)->fd[0],(p+i)->fd[1]);
  }
}

void close_pipes(Pipe *p, size_t n){
  for(size_t i = 0; i<n;i++){
    close((p+i)->fd[0]),close((p+i)->fd[1]);
  }
}
