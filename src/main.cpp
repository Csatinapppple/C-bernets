#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <pthread.h>

enum class status{
	UNDONE,
	DONE,
};

struct task{
	int ram,
			cpu,
			status;
};

class Job{
	struct task *tasks;
	int size,
			done;
	Job(int size){
		this->size=size;
		tasks=(struct task*)calloc(size,sizeof(struct task));
	}
	~Job(){
		free(tasks);
	}
};

class Pod;
struct Pkg {
  int worker_ram,
      worker_cpu;
  Pod *current_pod;
};

void *work(void *ptr);

class Pod{
public:
  int ram,
      cpu;
  
  pthread_mutex_t mutex;
  pthread_cond_t cond;

  Pod(int ram,int cpu){
    this->ram = ram,this->cpu = cpu;
    mutex = PTHREAD_MUTEX_INITIALIZER;
    cond = PTHREAD_COND_INITIALIZER;
  }
  

  void add_worker(int ram, int cpu){
    pthread_t t;
    if((this->ram - ram) < 0 || (this->cpu - cpu) < 0){
      printf(
          "ERROR ADDING WORKER: NOT ENOUGH RAM(%d) OR MEMORY(%d)\n",
          this->ram-ram,
          this->cpu-cpu
      );
      return;
    }
    this->ram-=ram;
    this->cpu-=cpu;
    Pkg p = {
    	.worker_ram = ram,
    	.worker_cpu = cpu,
    	.current_pod = this
    };
    pthread_create(&t,NULL,&work,&p);
    sleep(1);
    //precisa esperar para que a memoria de stack dessa funcao seja copiada
    //para a funcao criada pela thread se nao vai acessar uma area de memoria 
    //indisponivel
    //pthread_cond_wait(&cond,&mutex);
    pthread_detach(t);
  }
};

void *work(void *ptr){
  Pkg p = *(Pkg *)ptr;
  //pthread_cond_signal(&p.current_pod->cond);
  sleep(random()%10);
  pthread_mutex_lock(&p.current_pod->mutex);puts("assss");//locks here
  printf("workdone, returning %d ram and %d cpu\n",
  	p.worker_ram,
  	p.worker_cpu
  );
  p.current_pod->ram+=p.worker_ram;
  p.current_pod->cpu+=p.worker_cpu;
  pthread_mutex_unlock(&p.current_pod->mutex);
  return NULL;
}

int main(){
  
  Pod w1(4000,6),w2(3000,4);
  
  w1.add_worker(100,1);
  w2.add_worker(3001,5);
  
  while(1){
    printf("w1.ram=%d,w1.cpu=%d\n",w1.ram,w1.cpu);
    sleep(1);
  }
  ;

  return 0;
}
