#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <pthread.h>
#include <string.h>

#include "libs/deque.h"


pthread_t *thpool;
Deque_t* deque_12;
Deque_t* deque_23;

void* readfile(void* arg);
void* tokenizer();
void* printwords();
char rd;
char tk;


void init(){
   deque_12 = new_deque();
   deque_23 = new_deque();
   rd = 0;
   tk = 0;
}

int main(int argc, char *argv[]){
   
   char* filename;
   if(argc!=2){
      fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
      exit(EXIT_FAILURE);
   }
   else filename = argv[1];
   
   init();
   thpool = malloc(sizeof(pthread_t)*3);
   
   thread_create(&thpool[0],/*attrs*/ NULL, readfile, (void*)filename);
   thread_create(&thpool[1],/*attrs*/ NULL, tokenizer, /*args*/NULL);
   thread_create(&thpool[2],/*attrs*/ NULL, printwords, /*args*/NULL);

   for(int i=0; i<3; i++)
      thread_join(thpool[i], NULL);

   free_deque(deque_12,1);
   free_deque(deque_23,0);
   free(thpool);
}



void* readfile(void* arg){
   FILE* fp;
   fp = fopen((char*)arg, "r");
   if(fp==NULL){
      fprintf(stderr, "Error opening file %s\n", (char*)arg);
      exit(EXIT_FAILURE);
   }
   char* line= calloc(1,sizeof(char)*BUFSIZE);
   while(fgets(line, BUFSIZE, fp)!=NULL){
      //printf("line: %s\n", line);
      push_tail(deque_12, (void*)line);
      line= calloc(1,sizeof(char)*BUFSIZE);
   }
   rd=1;
   fflush(stdout);
   printf("1: File read\n");
   fclose(fp);
   free(line);
   return NULL; 
}

void* tokenizer(){
   struct timespec t = {0,1000000};
   while(isEmpty(deque_12))
      nanosleep(&t, NULL);
   char* bfr;
   //char* bfr = malloc(sizeof(char)*BUFSIZE);
   while(1){
      pop_head(deque_12, (void*)&bfr,1);
   
      if(bfr==NULL && rd) break;
      else if(bfr==NULL){
         nanosleep(&t, NULL);
         continue;
      }
      char* state;
      char* token = strtok_r(bfr, " ",&state);
      while(token!=NULL){
         push_tail(deque_23, (void*)token);
         token = strtok_r(NULL, " ",&state);
      }
      fflush(stdout);
      
   }
   
   tk=1;
   puts("2: Tokenized");
   return NULL;
}
void* printwords(){
   struct timespec t = {0,1000000};
   while(isEmpty(deque_23))
      nanosleep(&t, NULL);
   char* word;
   while(1){
      pop_head(deque_23, (void*)&word,0);
      if(word==NULL && tk) break;
      else if(word==NULL){
         nanosleep(&t, NULL);
         continue;
      }
      printf(">%s\n",word);
      fflush(stdout);
   }
   puts("3: all words printed");
   return NULL;
}

//todo: solve leaks at tokenizer and printwords
