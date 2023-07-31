#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 1024
/*

Scrivere un programma che implementa una versione semplificata del comando Unix wc (word count) 
limitatamente alle opzioni -l e -w (man 1 wc). Usare getopt per il parsing degli argomenti.

Se non vengono passate opzioni, il programma stampa sia il numero di linee che il numero di parole 
del/dei file i cui nomi sono passati come argomento (mywc [-l -w] file1 [file2 file3 ….]).

Suggerimento: scrivere una funzione che conta le parole di una stringa utilizzando le funzioni di 
utilità della libreria standard come isspace, iscntrl (man 3 isspace).


-l      The number of lines in each input file is written to the standard output.
-w      The number of words in each input file is written to the standard output.
*/

//punteggiatura compresa tra spazi viene considerata come parola
int countWords(FILE* fptr, char* buf){
   rewind(fptr);
   //puts("countwords");
   int ctr=0; //conta parole
   int bufFill =0;
   while(fgets(buf, MAXLINE, fptr) != NULL){
      //printf("%c\n",buf[strlen(buf)-1]);
      int i=0;
      int prevSpace=1;
      if(bufFill){ //ho riempito il buffer precedente, controllo se sono alla fine della parola
         if(isspace(buf[0])) {ctr++;}
         bufFill =0;
         //puts("buffer check");
      }
      while(!iscntrl(buf[i])){
         prevSpace=0;
         if(isspace(buf[i])){
            if(!prevSpace) ctr++;   
            prevSpace=1;
         } 
         else prevSpace=0;
         i++;
      }
      if(i+1==MAXLINE && !isspace(buf[i-1])){
         //printf("buffer\n");
         bufFill=1;
         prevSpace=1;
      } 
      if(buf[i]== '\n' && i >1) ctr++; //se una riga è vuota viene segnata senza parole
   
   }
   return ctr;
}

int countLines(FILE* fptr, char* buf){
   int ctr=0;
   while(fgets(buf, MAXLINE, fptr) != NULL){
      //controllo se ultimo carattere del buffer è un \n, altrimenti buffer pieno, continuo
      if(buf[strlen(buf)-1]=='\n'){ 
         if(!iscntrl(buf[0])) ctr++;
      }
      
   } //raggiunto EOF, controllo l'ultimo buffer se ha qualcosa dentro
   if(!iscntrl(buf[0])) ctr++;
   return ctr;
}



int main(int argc, char *argv[]){
   if(argc==1){ 
      puts("Error: missing file(s). Correct usage: mywc [-l -w] file1 [file2 file3 ...]");
      exit(EXIT_FAILURE);
   }
   char opt;
   int l=0, w=0;
   while ((opt = getopt(argc, argv,":lw"))!=-1){
      switch(opt){
         case 'l': //line
            l=1;
            break;
         case 'w': //words
            w=1;
            break;
         case '?':
            puts("Error: unknown option. Correct usage: mywc [-l -w] file1 [file2 file3 ...]");
            exit(EXIT_FAILURE); 
      }
   }
   if(!w&&!l){ //caso default: stampa sia il numero di linee che il numero di parole
      w=1;
      l=1;
   }
   char* buf = malloc(MAXLINE*sizeof(char));

   for(int i=1;i<argc;i++){
      FILE* fptr = fopen(argv[i], "r");
      if(fptr==NULL){
         printf("Error in opening file %d\n", i);
         exit(EXIT_FAILURE); 
      }
      if(l){
         printf("file %d: %d lines\n", i, countLines(fptr, buf));
      }
      if(w){
         printf("file %d: %d words\n", i, countWords(fptr, buf));
      }
      puts("");
      fclose(fptr);
   }
   free(buf);
}


