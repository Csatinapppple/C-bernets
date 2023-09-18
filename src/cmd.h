#ifndef CMD_H_
#define CMD_H_

#include <string.h>
#include <stdio.h>

#define BUFFER_CMD 255

struct cmd{
  char **argv;
  int argc;
};

typedef struct cmd Cmd;

extern int cnt_cmd(char *buffer);
//utiliza malloc
extern void fill_cmd(Cmd *c, char* buffer);
extern void print_cmd(Cmd *c);
extern void free_cmd(Cmd *c);

#endif

