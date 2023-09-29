#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <cctype>
#include <climits>

#define DEBUG 0

typedef unsigned int uint;

enum Status{
	INACTIVE=0,
  IN_PROGRESS=1,
	DONE=2,
};

class Job;
struct Task{
	int ram,
			 cpu,
			 status,
       i;
  Job *current_job;
};

class Job{
public:
	Task *tasks;
	int size,
			done;
  pthread_mutex_t mutex;
	Job(int size){
		this->size=size;
		tasks=(Task*)calloc(size,sizeof(Task));
    for(int i = 0; i<size; i++){
      (tasks+i)->current_job=this;
      (tasks+i)->ram=random()%1000+500;
      (tasks+i)->cpu=random()%4+1;
#if DEBUG == 1     
      printf("ram[%d] = %d\n",i,(tasks+i)->ram);
      printf("cpu[%d] = %d\n",i,(tasks+i)->cpu);
#endif
      (tasks+i)->status=INACTIVE;
      (tasks+i)->i=i;
    }
    mutex=PTHREAD_MUTEX_INITIALIZER;
	}

  Task *get_smallest_task_ram(){
    int ram_cmp = INT_MAX;
    Task *ret = NULL;
    for(int i = 0; i<size; i++){
      if((tasks+i)->status == INACTIVE && (tasks+i)->ram < ram_cmp){
        ret = tasks+i;
        ram_cmp = (tasks+i)->ram;
      }
    }
    return ret;
  }
  Task *get_smallest_task_cpu(){
    int cpu_cmp = INT_MAX;
    Task *ret = NULL;
    for(int i = 0; i<size; i++){
      if((tasks+i)->status == INACTIVE && (tasks+i)->cpu < cpu_cmp){
        ret = tasks+i;
        cpu_cmp = (tasks+i)->ram;
      }
    }
    return ret;
  }
  Task *get_biggest_task_ram(){
    int ram_cmp =0;
    Task *ret = NULL;
    for(int i = 0; i<size; i++){
      if((tasks+i)->status == INACTIVE && (tasks+i)->ram > ram_cmp){
        ret = tasks+i;
        ram_cmp = (tasks+i)->ram;
      }
    }
    return ret;
  }
  Task *get_biggest_task_cpu(){
    int cpu_cmp = 0;
    Task *ret = NULL;
    for(int i = 0; i<size; i++){
      if((tasks+i)->status == INACTIVE && (tasks+i)->cpu > cpu_cmp){
        ret = tasks+i;
        cpu_cmp = (tasks+i)->ram;
      }
    }
    return ret;
  }

};

class Pod;
struct Pkg {
  Task* t;
  Pod *current_pod;
};

void *work(void *ptr);

class Pod{
public:
  int ram,
      cpu;
  
  pthread_mutex_t mutex;

  Pod(int ram,int cpu){
    this->ram = ram,this->cpu = cpu;
    mutex = PTHREAD_MUTEX_INITIALIZER;
  }

  void add_worker(Task* t){
    pthread_t pt;
    if((this->ram - t->ram) < 0 || (this->cpu - t->cpu) < 0){
      printf(
          "ERROR ADDING WORKER: NOT ENOUGH RAM(%d) OR MEMORY(%d)\n",
          this->ram-ram,
          this->cpu-cpu
      );
      return;
    }
    this->ram-=t->ram;
    this->cpu-=t->cpu;
    Pkg *p = (Pkg *)malloc(sizeof(Pkg));
    p->t = t;
    p->current_pod = this;
    pthread_create(&pt,NULL,&work,p);
    pthread_detach(pt);
  }
};

void *work(void *ptr){
  Pkg p = *(Pkg *)ptr;
  free(ptr);
  sleep(random()%10);
  pthread_mutex_lock(&p.current_pod->mutex);
  //access memory in Pod
  printf("workdone, returning %d ram and %d cpu\n",
  	p.t->ram,
  	p.t->cpu
  );
  p.current_pod->ram += p.t->ram;
  p.current_pod->cpu += p.t->ram;
  pthread_mutex_unlock(&p.current_pod->mutex);
  p.t->status = DONE;
  pthread_mutex_lock(&p.t->current_job->mutex);
  //access memory in Job
  p.t->current_job->done++;
  printf(
    "Task[%d] done, tasks done until now = %d, total_tasks = %d\n",
    p.t->i,
    p.t->current_job->done,
    p.t->current_job->size
  );
  pthread_mutex_unlock(&p.t->current_job->mutex);
  return NULL;
}

void sched_jobs(std::vector<Job> j_vec, std::vector<Pod> p_vec){
  int highest_sum = 0;
  for(int i = 0; i<p_vec.size(); i++){
    for(int j = 0; j<j_vec.size(); j++){
      //if()
    }
  }
}

int main(){
  //srand(time(NULL));
  std::vector<Job> j_vec = {
    Job(3),
    Job(2)
  };

  std::vector<Pod> p_vec = {
    Pod(4000,16),
    Pod(3000,8)
  };
  
#if DEBUG == 1
  std::cout << "smallest ram = " << j_vec[0].get_smallest_task_ram()->ram << std::endl;
  std::cout << "biggest ram = " << j_vec[0].get_biggest_task_ram()->ram << std::endl;
  std::cout << "smallest cpu = " << j_vec[0].get_smallest_task_cpu()->cpu << std::endl;
  std::cout << "biggest cpu = " <<j_vec[0].get_biggest_task_cpu()->cpu << std::endl;
#endif
  while(1){
    for(int i = 0; i<p_vec.size();i++){
      printf(
        "p_vec[%d] = .ram=%d, .cpu=%d\n",
        i,
        p_vec[i].ram,p_vec[i].cpu
      );
    }
    sleep(1);
  };

  return 0;
}
