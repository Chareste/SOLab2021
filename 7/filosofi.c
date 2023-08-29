#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

#include "libs/functs.h"

#define EAT_NUM 100
#define PHILOSOPHER_NUM 5
#define NANOSLEEP_MAX 1000000


/*
   N filosofi siedono ad una tavola con un piatto di spaghetti davanti ed una forchetta alla loro destra 
   ed una alla loro sinistra. Per mangiare gli spaghetti un filosofo ha bisogno di entrambe le forchette vicine (!). 
   Ogni filosofo è impegnato ininterrottamente in una sequenza di 3 attivita': 
   meditare, cercare di acquisire le forchette e mangiare. 
   Scrivere un programma C che attivi N threads filosofi (con N>=5), ognuno dei quali esegue il ciclo 
   descritto in precedenza per 100 volte. 
   
   La meditazione e la fase in cui il filosofo mangia deve essere implementata con un ritardo variabile 
   usando ad esempio la chiamata di sistema nanosleep e la funzione rand_r().

SUGGERIMENTO: Per evitare il deadlock nell'aquisizione delle forchette, 
una possibile soluzione è definire un ordinamento opportuno per l'acquisizione delle forchette da parte di ogni filosofo.
*/



void* philosopher(void* arg);

static pthread_mutex_t mtx_forks[PHILOSOPHER_NUM];
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_t* th;

void init(){
   for(int i=0; i<PHILOSOPHER_NUM; i++){
      pthread_mutex_init(&mtx_forks[i], NULL);
   }
   th = malloc(sizeof(pthread_t) * PHILOSOPHER_NUM);
}

int main(){
   init();
   
   for(long i=0; i<PHILOSOPHER_NUM; i++){
      thread_create(&th[i], NULL, philosopher, (void*)i);
   }

   for(int i=0;i<PHILOSOPHER_NUM; i++){
      if (pthread_join(th[i], NULL) == -1) fprintf(stderr, "pthread_join failed\n");
   }
   puts("Pasto avvenuto con successo!");
   free(th);
   return 0;
}



void* philosopher(void* arg){
   long id = (long)arg;
   unsigned int seed = id*time(NULL);
   

   for(int j=0; j<EAT_NUM; j++){
      // meditazione
      if(1){
      long r = rand_r(&seed)%NANOSLEEP_MAX;
      struct timespec t = {0,r};
      nanosleep(&t, NULL);
      printf("filosofo %ld medita: %ld us\n", id+1, r/1000);
      }  
      if(id%2){//sx poi dx
         mtx_lock(&mtx_forks[id]);
         printf("%ld: ottengo forchetta sx(%ld)\n", id+1, id);
         mtx_lock(&mtx_forks[(id+1)%5]);
         printf("%ld: ottengo forchetta dx(%ld)\n", id+1, (id+1)%5);
      }
      else{ // dx poi sx
         mtx_lock(&mtx_forks[(id+1)%5]);
         printf("%ld: ottengo forchetta dx(%ld)\n", id+1, (id+1)%5);
         mtx_lock(&mtx_forks[id]);
         printf("%ld: ottengo forchetta sx(%ld)\n", id+1, id);
      }
      
      // mangio
      if(1){
      long r = rand_r(&seed)%NANOSLEEP_MAX;
      struct timespec t = {0,r};
      nanosleep(&t, NULL);
      printf("filosofo %ld mangia: %ld us\n", id+1, r/1000);
      } 

      if(!id%2){ // sx poi dx
         mtx_unlock(&mtx_forks[id]);
         //printf("%ld: rilascio forchetta sx(%ld)\n", id+1, id);
         mtx_unlock(&mtx_forks[(id+1)%5]);
         //printf("%ld: rilascio forchetta dx(%ld)\n", id+1, (id+1)%5);
      }
      else{ // dx poi sx
         mtx_unlock(&mtx_forks[(id+1)%5]);
         //printf("%ld: rilascio forchetta dx(%ld)\n", id+1, (id+1)%5);
         mtx_unlock(&mtx_forks[id]);
         //printf("%ld: rilascio forchetta sx(%ld)\n", id+1, id);   
      }


   }
   printf("filosofo %ld ha finito di mangiare\n", id+1);
   return NULL;
}