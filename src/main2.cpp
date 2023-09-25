#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <pthread.h>

typedef struct{
  int ram,
      cpu,
      job;
  Pod *p;
}pkg;

typedef struct{
  int ram,
      cpu;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
}Pod;

void init_pod(Pod *p, int ram, int cpu){
  p->ram = ram,p->cpu = cpu;
  p->mutex = PTHREAD_MUTEX_INITIALIZER;
  p->cond = PTHREAD_COND_INITIALIZER;
}

void *work(void *ptr)

void add_worker(Pod *p,int ram, int cpu){
  pthread_t t;
  if(p->ram - ram < 0 || p->cpu - cpu < 0){
    printf(
        "ERROR ADDING WORKER: NOT ENOUGH RAM(%d) OR MEMORY(%d)\n",
        p->ram-ram,
        p->cpu-cpu
                                   
    );
    return;
  }

  this->ram-=ram;
  this->cpu-=cpu;
  Worker w = {.ram = ram, .cpu=cpu};
  pthread_create(&t,NULL,&work,&w);
  pthread_cond_wait(&cond,&mutex);
}
