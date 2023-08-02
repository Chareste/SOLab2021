#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 #include <fcntl.h>

//#include ".libs/mylib.h"

#define BUFFERSIZE 1024

/*
Scrivere un programma, utilizzando chiamate di sistema, che implementi l'equivalente del comando Unix cp. 
Il programma deve accettare 3 argomenti come segue: mycp_sc filein fileout [buffersize]
L'argomento 'buffersize' e' la dimensione del buffer da utilizzare per le letture e scritture con le SCs 
read e write (se non specificato assegnare un valore di default, es. 256bytes):
*/

int isNumber(char* s) {
   char* b = NULL;
   long val = strtol(s, &b, 0);
   if (b != NULL && *b == (char)0) return 0; 
   return 1;
}


int main(int argc, char* argv[]){
   if(argc < 3 || argc > 4){
      printf("Usage: mycp_sc filein fileout [buffersize]\n");
      exit(1);
   }
   char* buf;
   int size;
   if(argc==4){
      if (!isNumber(argv[3])){
         puts("Error in allocating buffer, did you put an integer as buffersize?");
         exit(EXIT_FAILURE);
      }
      size= atoi(argv[3]);
   }
   else size = BUFFERSIZE;
   buf = malloc(size*sizeof(char));

   int fin = open(argv[1], O_RDONLY);
   int fout = open(argv[2], O_WRONLY |O_CREAT | O_TRUNC, 0666);

   int t = read(fin, buf, size);
   while(t !=-1 && t!=0){
      write(fout, buf, t);
      t = read(fin, buf, size);
   }

   
   free(buf);

}