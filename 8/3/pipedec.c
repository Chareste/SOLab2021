#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libs/functs.h"
#include "libs/sysc.h"

/* 8.3

Scrivere un programma (chiamato pipedec) che prende in ingresso un intero positivo. 
pipedec lancia 2 processi figli ognuno dei quali esegue con una chiamata exec* il programma dec. 
Solo ad uno dei due processi figli viene passato come argomento l'intero positivo passato al programma
pipedec. I due processi figli devono essere connessi tra di loro in modo tale che lo standard input
di un processo sia connesso con lo standard output dell'altro processo 
(la connessione e' quindi bidirezionale).

*/
int debug = 1;

int main(int argc, char* argv[]){
   int n;
   if(!debug){
      check_args(argc,2,2,argv[0],"n");
      n = parse_int(argv[1]);
      if(n<=0) {
	      fprintf(stderr, "n deve essere maggiore di 0!\n");
	      exit(EXIT_FAILURE);
      }
   }
   else n=7; 
   

   pid_t pid1 = -1, pid2 = -1;
   int fromP1[2], toP1[2];
   
   pipe_create(fromP1); pipe_create(toP1);

   fork_create(&pid2);
   if(pid2 != 0)
      fork_create(&pid1);
   
   if(pid2 !=0 && pid1 != 0){ //parent
      close_check(fromP1[0]); close_check(fromP1[1]);
      close_check(toP1[1]); close_check(toP1[0]);
   }
   else if(pid2 ==0){
      dup2(fromP1[0], 0); dup2(toP1[1], 1);
        
      close_check(fromP1[1]); close_check(toP1[0]);

      execl("./dec", "dec", (char *)NULL);  
      perror("execl pid2");
      exit(EXIT_FAILURE);

   }
   else if(pid1 ==0){
      dup2(toP1[0], 0); dup2(fromP1[1], 1);

      close_check(fromP1[0]); close_check(toP1[1]);

      execl("./dec", "dec", argv[1], (char *)NULL);
        
      perror("execl pid1");
      exit(EXIT_FAILURE);

   }

   n%2==0 ? waitpid(pid2,NULL,0) : waitpid(pid1,NULL,0);
   puts("done!");
}