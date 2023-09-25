#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <pthread.h>
#include <vector>

enum Status{
	UNDONE=0,
  IN_PROGRESS=1,
	DONE=2,
};

class Job;
struct Task{
	int ram,
			cpu,
			status;
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
      (tasks+i)->ram=random()%1500;
      (tasks+i)->cpu=random()%3;
      (tasks+i)->status=UNDONE;
    }
    mutex=PTHREAD_MUTEX_INITIALIZER;
	}
	~Job(){
		free(tasks);
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
  pthread_mutex_unlock(&p.t->current_job->mutex);
  return NULL;
}

int main(){

  std::vector<Job> p_job = {
    Job(3),
    Job(2)
  };

  std::vector<Pod> p_vec = {
    Pod(4000,6),
    Pod(3000,4)
  };
  
  p_vec[1].add_worker(
    p_job[0].tasks+1
  );

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
