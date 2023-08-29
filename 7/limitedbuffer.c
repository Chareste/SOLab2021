#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>

//#define BUF_SIZE 1
#define OBJ_NUM 100

/*
   Scrivere un programma C con due threads, un produttore (P) ed un consumatore (C).
   Il thread P genera, uno alla volta, una sequenza di numeri inserendoli in un buffer di una sola posizione 
   condiviso con il thread C.
   Il thread consumatore estrae i numeri dal buffer e li stampa sullo standard output. 
   Se il buffer e' pieno P attende che C consumi il dato, 
   analogamente se il buffer e' vuoto C attende che P produca un valore da consumare.
*/

void* producer();
void* consumer();


static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int buffer;
static char bufempty=1;  // flag che indica se il buffer e' vuoto


int main(int argc, char *argv[]) {

   pthread_t pid;
   pthread_t cid;
   int err;

   if((err = pthread_create(&pid, NULL, producer, NULL))!=0) {
      fprintf(stderr, "producer - pthread_create: %s\n", strerror(err));
      exit(EXIT_FAILURE);
   }
   if((err = pthread_create(&cid, NULL, consumer, NULL))!=0) {
      fprintf(stderr, "consumer - pthread_create: %s\n", strerror(err));
      exit(EXIT_FAILURE);
   }
   if(pthread_join(pid, NULL)!=0) {
      fprintf(stderr, "producer - pthread_join: %s\n", strerror(err));
      exit(EXIT_FAILURE);
   }
   if(pthread_join(cid, NULL)!=0) {
      fprintf(stderr, "consumer - pthread_join: %s\n", strerror(err));
      exit(EXIT_FAILURE);
   }
     return 0;  
}

void* consumer(){
   int n = 0;
   while(n>=0){
      pthread_mutex_lock(&mtx);
      while(bufempty){
         pthread_cond_wait(&cond, &mtx);
      }
      n = buffer;
      bufempty = 1;
      pthread_cond_signal(&cond);
      pthread_mutex_unlock(&mtx);
      printf("Consumer: %d\n", n);
   }
   puts("Consumer: exit");
   pthread_exit(NULL);
}


void* producer(){
  for(int i=0; i<OBJ_NUM; i++){
      int n = rand()%100;
      printf("Producer: %d\n", n);
      pthread_mutex_lock(&mtx);
      while(!bufempty){
         pthread_cond_wait(&cond, &mtx);
      }
      buffer = n;
      bufempty = 0;
      pthread_cond_signal(&cond);
      pthread_mutex_unlock(&mtx);
   }
   puts("Producer: exit");

   pthread_mutex_lock(&mtx);
   while (!bufempty){
      pthread_cond_wait(&cond, &mtx);
   }
   buffer = -1;
   bufempty = 0;
   pthread_cond_signal(&cond);
   pthread_mutex_unlock(&mtx);
   
   pthread_exit(NULL);
   
}