#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

   int size;
   if(argc ==1){
      size = 15;
   
   }
   else{
      size = atoi(argv[1]);
   }
   if(size>512){
      puts("Size too big");
      exit(EXIT_FAILURE);
   }
   
   float* matrix = malloc(size*size*sizeof(float));
   int i,j;
   for(i=0; i<size; i++){
      for(j=0; j<size; j++){
         matrix[i*size+j] = i+j/2.0;
      }
   }


   FILE* fptr = fopen("mat_dump.txt", "w");
   FILE* fbptr = fopen("mat_dump.dat", "wb");

   for(i=0; i<size; i++){
      for(j=0; j<size; j++){
         fprintf(fptr, "%.2f ", matrix[i*size+j]);
         fwrite(&matrix[i*size+j], sizeof(float), 1, fbptr);
      }
      fprintf(fptr, "\n");
   }
   fclose(fptr);
   fclose(fbptr);

   free(matrix);

}
