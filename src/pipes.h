#ifndef PIPES_H_
#define PIPES_H_

#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

#define READ 0
#define WRITE 1

#define MSG_START '\1'

struct pipe{int fd[2];};

typedef struct pipe Pipe;

extern void open_pipes(Pipe *p, size_t n);
extern void print_pipes(Pipe *p, size_t n);
extern void close_pipes(Pipe *p, size_t n);

#endif 
