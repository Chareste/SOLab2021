#ifndef _FUNCTS_H
#define _FUNCTS_H

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <errno.h>


#ifndef BUFSIZE
#define BUFSIZE 256
#endif

/* GENERAL UTILITIES */

/*
 * Receives a string nptr and a base, then calls strtol (man 3 strtol)
 * and checks if the value is actually an integer.
 * Returns the integer or kills the program.
 */
static int parseInt(const char* nptr, int base){
    errno = 0;
    char* p; //see man strtol for its use

    long n = strtol(nptr, &p, base);

    if(p==nptr || '\0'!= *p){
        fprintf(stderr, "Error: %s is NaN\n", nptr);
        exit(EXIT_FAILURE);
    }
    if(errno || n>INT32_MAX || n< INT32_MIN){
        fprintf(stderr, "Error: overflow (errno = %d)\n", errno);
        exit(EXIT_FAILURE);
    }
    return (int)n;
}

void sleep_ms(float ms){
    int ns = (int)(ms*1000000);
    struct timespec t = {0,ns};
    nanosleep(&t, NULL);

}

/* THREADS AND MUTEXES */

void thread_create(pthread_t *tid,const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg){
    int err;
    if((err = pthread_create(tid, attr, start_routine, arg)) != 0){
        perror("pthread_create");
        errno = err;
        pthread_exit(&errno);
    }
    //else 
        //printf("thread(%d) created", *tid);
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
        puts("mutex_lock");
        errno = err;
        pthread_exit(&errno);
    }
    //else 
        //puts("mutex locked");
        
}

void mtx_unlock(pthread_mutex_t *mtx_ptr){
    int err;
    if((err = pthread_mutex_unlock(mtx_ptr)) != 0){
        perror("mutex_unlock");
        errno = err;
        pthread_exit(&errno);
    }
    //else 
        //puts("mutex unlocked");
        
}

void mtx_wait(pthread_mutex_t* mtx_ptr){
    mtx_unlock(mtx_ptr);
    struct timespec t = {0,1000000};
      nanosleep(&t, NULL);
    mtx_lock(mtx_ptr);
}

/* PIPES */



#endif /* _FUNCTS_H */