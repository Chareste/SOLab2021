
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASSWD_FILE "/etc/passwd"
#define OUTPUT_FILE "./output.txt"

#ifndef MAX_LEN //posso definire max len a compilazione
	#define MAX_LEN 1024
#endif

void close(FILE *f1, FILE *f2, char *b);

int main(int argc, char *argv[]){
	
	FILE *fpwd=NULL;
	FILE *fout=NULL;
	char *buf=NULL;
	char *tok;

	//controllo apertura file 
	if ((fpwd=fopen(PASSWD_FILE, "r")) == NULL) {
		perror("opening password file");
		close(fpwd,fout, buf);
		exit(EXIT_FAILURE);
	}
	if ((fout=fopen(OUTPUT_FILE, "w")) == NULL) { 
		perror("opening output file");
		close(fpwd,fout,buf);
		exit(EXIT_FAILURE);
	}

	//controllo malloc per buf
	if((buf=malloc(MAX_LEN*sizeof(char))) == NULL) {
		perror("malloc buffer");
		close(fpwd,fout,buf);
		exit(EXIT_FAILURE);
	}
	
	while((fgets(buf, MAX_LEN, fpwd))!=NULL){
		tok = strtok(buf,":");
		fprintf(fout,"%s\n",tok);
	}
	close(fpwd,fout,buf);
	exit(EXIT_SUCCESS);

}

void close(FILE *f1, FILE *f2, char *b){
	if(f1) fclose(f1);
   if(f2) fclose(f2);
   if(b) free(b);
}
