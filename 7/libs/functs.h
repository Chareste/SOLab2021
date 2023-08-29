#ifndef _FUNCTS_H
#define _FUNCTS_H

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <errno.h>


#ifndef BUFSIZE
#define BUFSIZE 1024
#endif

/* THREADS AND MUTEXES */



void thread_create(pthread_t *tid,const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg){
    int err;
    if((err = pthread_create(tid, attr, start_routine, arg)) != 0){
        fprintf(stderr,"FATAL ERROR: thread_create");
        errno = err;
        pthread_exit(&errno);
    }
    else 
        printf("thread(%ld) created\n", (long)*tid);
}

void thread_join(pthread_t tid, void **retval){
    int err;
    if((err = pthread_join(tid, retval)) != 0){
        fprintf(stderr,"FATAL_ERROR: thread_join");
        errno = err;
        pthread_exit(&errno);
    }
    //else 
        //printf("thread(%ld) joined\n", (long)tid);
}


void mtx_lock(pthread_mutex_t *mtx_ptr){
    int err;
    if((err = pthread_mutex_lock(mtx_ptr)) != 0){
        fprintf(stderr,"FATAL ERROR: mtx_lock");
        errno = err;
        pthread_exit(&errno);
    }
    //else 
        //puts("mutex locked");
        
}

void mtx_unlock(pthread_mutex_t *mtx_ptr){
    int err;
    if((err = pthread_mutex_unlock(mtx_ptr)) != 0){
        fprintf(stderr,"Fatal error: mtx_unlock");
        errno = err;
        pthread_exit(&errno);
    }
    //else 
       // puts("mutex unlocked");
        
}

void mtx_wait(pthread_mutex_t* mtx_ptr){
    mtx_unlock(mtx_ptr);
    struct timespec t = {0,1000000};
      nanosleep(&t, NULL);
    mtx_lock(mtx_ptr);
}


/* LISTS, QUEUES AND STRUCTS */



#endif /* _FUNCTS_H */