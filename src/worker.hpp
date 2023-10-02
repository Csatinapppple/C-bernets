
#ifndef WORKER_HPP_
#define WORKER_HPP_

#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include "pod.hpp"

class Worker;
struct Pkg
{
  Task *t;
  Worker *current_worker;
};

void *work (void *ptr);

class Worker
{
public:
  volatile int ram, cpu;

  pthread_mutex_t mutex;

  Worker (int ram, int cpu);

  void add_worker (Task * t);
};

extern
void *
work (void *ptr);

#endif
