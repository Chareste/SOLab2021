#ifndef _SYSC_H
#define _SYSC_H

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>



void pipe_create(int* pfd){
    if((pipe(pfd))==-1){
        perror("pipe_create");
        pthread_exit(&errno);
    }
    //else puts("pipe done");
}
void close_check(int pfd){
    if((close(pfd))==-1){
        perror("close_check");
        pthread_exit(&errno);
    }
    //else puts("close done");
}

//usage fork_create(&pid);
void fork_create(int* pid){
    if((*pid=fork())==-1){
        perror("fork_create");
        pthread_exit(&errno);
    }
    
}








#endif