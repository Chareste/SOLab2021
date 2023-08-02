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

Realizzare quindi lo stesso programma, utilizzando le chiamate di libreria fread ed fwrite. 
Chiamiamo questa seconda versione 'mycp_std'.
*/

int isNumber(char* s) {
   char* b = NULL;
   long val = strtol(s, &b, 0);
   if (b != NULL && *b == (char)0) return 0; 
   return 1;
}


int main(int argc, char* argv[]){
   if(argc < 3 || argc > 4){
      printf("Usage: mycp_std filein fileout [buffersize]\n");
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

   FILE* fin = fopen(argv[1], "r");
   FILE* fout = fopen(argv[2], "w");

   int e = fread(buf, sizeof(char), size,fin);

   while(e!=0){
      fwrite(buf, sizeof(char), e, fout);
      e = fread(buf, sizeof(char), size, fin);
      //printf("%d\n", e);
   }

   
   free(buf);

}