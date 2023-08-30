/* 8.1

Scrivere un programma C in cui si attivano M thread produttori ed N thread consumatori 
che condividono una coda (di capacità “infinita”). Il programma accetta come argomento 
anche un intero K che corrisponde al numero totale di messaggi che i produttori producono 
concorrentemente nella coda (ogni produttore produce K/M messaggi – se M divide K). 

I consumatori leggono i messaggi dalla coda in un ciclo infinito fino a quando non ricevono 
un messaggio speciale che li fa terminare. Implementare la coda concorrente, ed il protocollo 
di terminazione senza usare la cancellazione esplicita dei threads. 

Testare il programma al variare di M ed N. Controllare con valgrind che non ci siano problemi 
nell'accesso alle variabili del programma e che tutta la memoria allocata sia completamente
rilasciata all'uscita del programma.

*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include<string.h>

#include "libs/deque.h"

int debug = 0;

Deque_t *buf;
pthread_t* prodpool;
pthread_t* conspool;
int finished;

typedef struct thargs{
   int id;
   long msg_nr;
}Thargs_t;

void* producer(void* arg);
void* consumer(void* arg);

void init(int p, int c){
   buf = new_deque();

   prodpool = malloc(sizeof(pthread_t)*p);
   conspool = malloc(sizeof(pthread_t)*c);
}
int main(int argc, char* argv[]){
   if(!debug){
      if(argc!=4){
            printf("correct usage: %s Prod Cons Msgs", argv[0]);
            exit(EXIT_FAILURE);
         }
      int p = parseInt(argv[1], 10);
      int c = parseInt(argv[2], 10);
      int k = parseInt(argv[3], 10);
      if(p<=0||c<=0||k<=0){
         printf("make sure all numbers are positive integers - [%d %d %d]",p,c,k);
         exit(EXIT_FAILURE);
      }
   }
   else{
      int p = 3;
      int c = 2;
      int k = 10;
   }
   init(p, c);

   
   for(int i=0; i<p; i++){
      Thargs_t* msg = malloc(sizeof(Thargs_t));
      //p doesn't necessarily divide k, so we're making sure all messages are sent
      msg->id = i;
      msg->msg_nr = ((k%p) >i) ? k/p+1 : k/p; 
      thread_create(&prodpool[i],NULL,producer,(void*)msg);
   }

   for(int i=0; i<c; i++)
      thread_create(&conspool[i],NULL,consumer,(void*)(long)i);
   
   for(int i=0; i<p; i++)
      thread_join(prodpool[i],NULL);
   finished = 1;

   for(int i=0; i<c; i++)
      thread_join(conspool[i],NULL);

   free_deque(buf,0); 
   free(prodpool);
   free(conspool);
}

void* producer(void* arg){
   Thargs_t* msg = (Thargs_t*)arg;
   char* str = calloc(1,sizeof(char)*BUFSIZE);
   for(int i=0; i< msg->msg_nr; i++){
      snprintf(str, BUFSIZE, "Producer #%d - message %d",msg->id, i);
      //printf(" %s\n", str);
      push_tail(buf, (void*)str);
      str = calloc(1,sizeof(char)*BUFSIZE);
   }
   
   free(str);
   free(msg);
   return NULL;
}

void* consumer(void* arg){
   char* str;
   while(1){
      pop_head(buf,(void*)&str, 0); 
      if(str==NULL && finished) break;
      if(str==NULL){
         sleep_ms(1);
         continue;
      }
      fflush(stdout);
      printf("consumer %ld: extracted message [%s]\n", (long)arg+1, str);
      free(str);
   }  
   
   return NULL;
}
