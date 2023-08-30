#ifndef _DEQUE_H
#define _DEQUE_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "functs.h"


typedef struct node{
   void* data;
	struct node *next;
	struct node *prev;
}Node_t;

typedef struct deque{
   Node_t *head;
   Node_t *tail;
   pthread_mutex_t mtx;
}Deque_t;

/* > Deque_t* new_deque()
 * 
 * Returns a new initialised deque, containing a mutex.
 * 
*/
Deque_t* new_deque(){
   Deque_t* d = malloc(sizeof(Deque_t));
   d->head = NULL;
   d->tail = NULL;
   pthread_mutex_init(&(d->mtx), NULL);
   return d;
}

int isEmpty(Deque_t *d){
   return (d->head == NULL);
}


void push_head(Deque_t* d, void* data){
   Node_t *newNode = malloc(sizeof(Node_t));
   newNode->data = data;
   newNode->prev = NULL;

   mtx_lock(&(d->mtx));

   if(isEmpty(d)){
      newNode->next = NULL;
      d->tail = newNode;
   }
   else{
      d->head->prev = newNode;
      newNode->next = d->head;
   }
   d->head = newNode;
   mtx_unlock(&(d->mtx));
} void push_tail(Deque_t* d, void* data){
   Node_t *newNode = malloc(sizeof(Node_t));
   newNode->data = data;
   newNode->next = NULL;

   mtx_lock(&(d->mtx));

   if(isEmpty(d)){
      newNode->prev = NULL;
      d->head = newNode;
   }
   else{
      newNode->prev = d->tail;
      d->tail->next = newNode;
   }
   d->tail = newNode;
   mtx_unlock(&(d->mtx));
}


/* 
 * > void pop_----(Deque_t* d, void ** val, int ismallocd)
 * 
 * These functions remove respectfully the head or the tail, but, instead of returning
 * the data, it is put inside void** val, pointer to the same type of the data.
 * 
 * ismallocd: 1 if data is dynamically allocated, 0 otherwise
 * 
 * -- Usage examples --
 * - retrieving a long: long* value; pop_head(d,(void*)value, 0);
 * - retrieving a string: char* string; pop_tail(d,(void*)&string, 1);
 *
*/
void pop_head(Deque_t* d, void** val, int ismallocd){
   mtx_lock(&(d->mtx));
   if(isEmpty(d)){
      mtx_unlock(&(d->mtx));
      //puts("No data to remove");
      *val = NULL;
      return;
   }

   Node_t *tmp = d->head;
   *val = d->head->data;
   if(d->head == d->tail){
      d->head = NULL;
      d->tail = NULL;
   }
   else{
      d->head = d->head->next;
      d->head->prev = NULL;
   }
   //puts("data retrieved");
   mtx_unlock(&(d->mtx));
   if(ismallocd) free(tmp->data);
   free(tmp);
} void pop_tail(Deque_t* d, void** val, int ismallocd){
   mtx_lock(&(d->mtx));
   if(isEmpty(d)){
      mtx_unlock(&(d->mtx));
      //puts("No data to remove");
      *val = NULL;
      return;
   }

   Node_t *tmp = d->tail;
   *val = d->tail->data;
   if(d->head == d->tail){
      d->head = NULL;
      d->tail = NULL;
   }
   else{
      d->tail = d->tail->prev;
      d->tail->next = NULL;

   }
   //puts("data retrieved");
   mtx_unlock(&(d->mtx));
   if(ismallocd) free(tmp->data);
   free(tmp);
}

void print_deque(Deque_t* d){
   mtx_lock(&(d->mtx));
   if(isEmpty(d)){
      mtx_unlock(&(d->mtx));
      puts("No data to display");
      return;
   }
   Node_t * NodePtr = d->head;
   puts("Printing...");
   int i=0;
   while(NodePtr != NULL){
      printf("%d: %p\n",i+1,NodePtr->data);
      NodePtr = NodePtr->next;
      i++;
   }
   mtx_unlock(&(d->mtx));
   
}

void free_deque(Deque_t* d, int ismallocd){
   Node_t* tmp;

   while(d->head != NULL){
      tmp = d->head;
      d->head = d->head->next;
      if(ismallocd) free(tmp->data);
      free(tmp);
    }
    free(d);
}



#endif