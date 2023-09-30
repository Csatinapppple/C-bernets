#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <cctype>
#include <climits>

#define DEBUG 1

enum Status
{
  INACTIVE = 0,
  IN_PROGRESS = 1,
  DONE = 2,
};

class Pod;
struct Task
{
  int ram, cpu, status, i;
  Pod *this_pod;
};

class Pod
{
public:
  Task * tasks;
  volatile int size, done;
  pthread_mutex_t mutex;
  
  Pod (int size)
  {
    this->size = size;
    printf("pod size = %d\n",this->size);

    tasks = (Task *) calloc (size, sizeof (Task));
    
    for (int i = 0; i < size; i++)
    {
      (tasks + i)->this_pod = this;
      (tasks + i)->ram = random () % 1000 + 500;
      (tasks + i)->cpu = random () % 4 + 1;
#if DEBUG == 1
      printf("this[%d] = %p\n", i, this);
      printf ("ram[%d] = %d\n", i, (tasks + i)->ram);
      printf ("cpu[%d] = %d\n", i, (tasks + i)->cpu);
#endif
      (tasks + i)->status = INACTIVE;
      (tasks + i)->i = i;
    }
    mutex = PTHREAD_MUTEX_INITIALIZER;
    done=0;
  }
  
  bool
  are_tasks_done()
  {
    for(int ii = 0; ii<size; ii++)
    {
      if((tasks+ii)->status != DONE) return false;
    }
    return true;
  }

  Task *
  get_smallest_task_ram ()
  {
    int ram_cmp = INT_MAX;
    Task *ret = NULL;
    for (int i = 0; i < size; i++)
    {
      if ((tasks + i)->status == INACTIVE && (tasks + i)->ram < ram_cmp)
      {
        ret = tasks + i;
        ram_cmp = (tasks + i)->ram;
      }
    }
    return ret;
  }
  
  Task *
  get_smallest_task_cpu ()
  {
    int cpu_cmp = INT_MAX;
    Task *ret = NULL;
    for (int i = 0; i < size; i++)
    {
      if ((tasks + i)->status == INACTIVE && (tasks + i)->cpu < cpu_cmp)
      {
        ret = tasks + i;
        cpu_cmp = (tasks + i)->ram;
      }
    }
    return ret;
  }
  
  Task *
  get_biggest_task_ram ()
  {
    int ram_cmp = 0;
    Task *ret = NULL;
    for (int i = 0; i < size; i++)
    {
      if ((tasks + i)->status == INACTIVE && (tasks + i)->ram > ram_cmp)
      {
        ret = tasks + i;
        ram_cmp = (tasks + i)->ram;
      }
    }
    return ret;
  }
  
  Task *
  get_biggest_task_cpu ()
  {
    int cpu_cmp = 0;
    Task *ret = NULL;
    for (int i = 0; i < size; i++)
    {
      if ((tasks + i)->status == INACTIVE && (tasks + i)->cpu > cpu_cmp)
      {
        ret = tasks + i;
        cpu_cmp = (tasks + i)->ram;
      }
    }
    return ret;
  }

};

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
  int ram, cpu;

  pthread_mutex_t mutex;

  Worker (int ram, int cpu)
  {
    this->ram = ram, this->cpu = cpu;
    mutex = PTHREAD_MUTEX_INITIALIZER;
  }

  void add_worker (Task * t)
  {
    pthread_t pt;

    if ((this->ram - t->ram) <= 0 || (this->cpu - t->cpu) <= 0)
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
};

void *
work (void *ptr)
{
  Pkg p = *(Pkg *) ptr;
  
  printf("p.Task = %p, p.current_worker = %p\n",p.t->this_pod, p.current_worker);
  printf("p.Task.size = %p\n",&p.t->this_pod->size);
  
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
  p.t->this_pod->done++;

  printf ("Task[%d] done, tasks done until now = %d, total_tasks = %p\n",
	  p.t->i, p.t->this_pod->done, &p.t->this_pod->size);
  
  pthread_mutex_unlock (&p.t->this_pod->mutex);
  return NULL;
}

void
sched_pods (std::vector < Pod >& pod_vec, std::vector < Worker >& worker_vec)
  //schedule jobs from the smallest tasks
  //always assign the job that makes more ram
  //available comparing both Workers
{
  for (int ii = 0; ii < worker_vec.size (); ii++)
  { 
    int lowest_ram_cmp = INT_MAX;
    Task *lowest_ram_task = NULL;
    for (int jj = 0; jj < pod_vec.size (); jj++)
    {
      Task *tmp = pod_vec[jj].get_smallest_task_ram();
      if (tmp == NULL) continue;
      else if(
          worker_vec[ii].ram - tmp->ram >= 0 && 
          worker_vec[ii].cpu - tmp->cpu >= 0 &&
          tmp->ram < lowest_ram_cmp)
      {
        lowest_ram_cmp = tmp->ram;
        lowest_ram_task = tmp;
      }
      else
      {
        printf("OUT OF MEMORY IN WORKER[%d]\n",ii);
      }
    }
    if(lowest_ram_task != NULL)
      worker_vec[ii].add_worker(lowest_ram_task);
  }
}

int
main ()
{
  //srand(time(NULL));
  std::vector < Pod > p_vec = {
    Pod (3),
    Pod (2)
  };

  std::vector < Worker > w_vec = {
    Worker (4000, 16),
    Worker (3000, 8)
  };

#if DEBUG == 1
  std::cout << "smallest ram = " << p_vec[0].get_smallest_task_ram ()->
    ram << std::endl;
  std::cout << "biggest ram = " << p_vec[0].get_biggest_task_ram ()->
    ram << std::endl;
  std::cout << "smallest cpu = " << p_vec[0].get_smallest_task_cpu ()->
    cpu << std::endl;
  std::cout << "biggest cpu = " << p_vec[0].get_biggest_task_cpu ()->
    cpu << std::endl;
#endif

  while (1)
  {
    for (int i = 0; i < w_vec.size (); i++)
    {
      printf ("w_vec[%d] = .ram=%d, .cpu=%d\n",
        i, w_vec[i].ram, w_vec[i].cpu);
    }
 
    int count = 0;
    for (int i = 0; i < p_vec.size (); i++)
    {
      printf ("p_vec[%d] = .size=%d, .done=%d\n",
        i, p_vec[i].size, p_vec[i].done);
      count += p_vec[i].are_tasks_done();
    }
    if(count == p_vec.size()) break;

    sched_pods(p_vec,w_vec);
    sleep(1);
  }

  return 0;
}

