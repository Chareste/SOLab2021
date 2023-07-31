#include <stdio.h>
#include <stdlib.h>


int compare(float* m1, float* m2, int size);

int main(int argc, char *argv[]){

   FILE *fptr, *fbptr;
   int size;

   if(argc==1){
      size = 15;
      fptr = fopen("mat_dump.txt", "r");
      fbptr = fopen("mat_dump.dat", "rb");
   }
   else{
      size = atoi(argv[1]);
      fptr = fopen(argv[2], "r");
      fbptr = fopen(argv[3], "rb");
   }
   int i,j;
   float* m1 = malloc(size*size*sizeof(float));
   float* m2 = malloc(size*size*sizeof(float));
   for(i=0; i<size; i++){
      for(j=0; j<size; j++){
         fscanf(fptr, "%f", &m1[i*size+j]);
         fread(&m2[i*size+j], sizeof(float), 1, fbptr);
      }
   }
   if(compare(m1, m2, size)){
      puts("Matrices are same");
   }
   else{
      puts("Matrices are different");
   }

   free(m1);
   free(m2);
}

int compare(float* m1, float* m2, int size){
   int i,j;
   for(i=0; i<size; i++){
      for(j=0; j<size; j++){
         if(m1[i*size+j] != m2[i*size+j]){
            return 0;
         }
      }
   }
   return 1;
}