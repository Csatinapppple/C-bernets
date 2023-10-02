#ifndef POD_HPP_
#define POD_HPP_

#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <cctype>
#include <climits>


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
  
  Pod (int size);
  
  bool
  are_tasks_done();
	
  Task *
  get_smallest_task_ram ();
  
  Task *
  get_smallest_task_cpu ();
  
  Task *
  get_biggest_task_ram ();
  
  Task *
  get_biggest_task_cpu ();

};


#endif
