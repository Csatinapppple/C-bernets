#define DEBUG
//#undef DEBUG
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <cctype>
#include <climits>
#include "pod.hpp"
#include "worker.hpp"

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

#ifdef DEBUG
  printf("smallest ram = %d\n",p_vec[0].get_smallest_task_ram()->ram);
  printf("biggest ram = %d\n", p_vec[0].get_biggest_task_ram()->ram);
  printf("smallest cpu = %d\n", p_vec[0].get_smallest_task_cpu()->cpu);
  printf("biggest cpu = %d\n", p_vec[0].get_biggest_task_cpu()->cpu);
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

