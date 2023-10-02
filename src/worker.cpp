#include "worker.hpp"

Worker::Worker (int ram, int cpu)
{
  this->ram = ram, this->cpu = cpu;
  mutex = PTHREAD_MUTEX_INITIALIZER;
}

void 
Worker::add_worker (Task * t)
{
  pthread_t pt;

  if ((this->ram - t->ram) < 0 || (this->cpu - t->cpu) < 0)
  {
    printf ("ERROR ADDING WORKER: NOT ENOUGH RAM(%d) OR MEMORY(%d)\n",
      this->ram - ram, this->cpu - cpu);
    return;
  }

  t->status = IN_PROGRESS;
  this->ram -= t->ram;
  this->cpu -= t->cpu;
  
  Pkg *p = (Pkg *) malloc (sizeof (Pkg));
  
  p->t = t;
  p->current_worker = this;
  

  pthread_create (&pt, NULL, &work, p);
  pthread_detach (pt);
}

void *
work (void *ptr)
{
  Pkg p = *(Pkg *) ptr;
  free (ptr);

  sleep (random () % 10 + 1);
  pthread_mutex_lock (&p.current_worker->mutex);
  
  //access memory in Worker
  printf ("workdone, returning %d ram and %d cpu\n", p.t->ram, p.t->cpu);
  
  p.current_worker->ram += p.t->ram;
  p.current_worker->cpu += p.t->cpu;
  
  pthread_mutex_unlock (&p.current_worker->mutex);
  
  p.t->status = DONE;
  
  pthread_mutex_lock (&p.t->this_pod->mutex);
  //access memory in Pod
  
  printf("address of pod size inside thread = %p\n",&p.t->this_pod->size);
  
  p.t->this_pod->done++;

  printf ("Task[%d] done, tasks done until now = %d, total_tasks = %d\n",
	  p.t->i, p.t->this_pod->done, p.t->this_pod->size);
  pthread_mutex_unlock (&p.t->this_pod->mutex);
  
  return NULL;
}

