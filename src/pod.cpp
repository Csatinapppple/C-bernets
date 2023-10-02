#include "pod.hpp"

Pod::Pod (int size)
{
  this->size = size;
  printf("pod size = %d\n",this->size);

  tasks = (Task *) calloc (size, sizeof (Task));
  
  for (int i = 0; i < size; i++)
  {
    (tasks + i)->this_pod = this;
    (tasks + i)->ram = random () % 1000 + 500;
    (tasks + i)->cpu = random () % 4 + 1;
#ifdef DEBUG
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
Pod::are_tasks_done()
{
  for(int ii = 0; ii<size; ii++)
  {
    if((tasks+ii)->status != DONE) return false;
  }
  return true;
}

Task *
Pod::get_smallest_task_ram ()
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
Pod::get_smallest_task_cpu ()
{
  int cpu_cmp = INT_MAX;
  Task *ret = NULL;
  for (int i = 0; i < size; i++)
  {
    if ((tasks + i)->status == INACTIVE && (tasks + i)->cpu < cpu_cmp)
    {
      ret = tasks + i;
      cpu_cmp = (tasks + i)->cpu;
    }
  }
  return ret;
}

Task *
Pod::get_biggest_task_ram ()
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
Pod::get_biggest_task_cpu ()
{
  int cpu_cmp = 0;
  Task *ret = NULL;
  for (int i = 0; i < size; i++)
  {
    if ((tasks + i)->status == INACTIVE && (tasks + i)->cpu > cpu_cmp)
    {
      ret = tasks + i;
      cpu_cmp = (tasks + i)->cpu;
    }
  }
  return ret;
}
