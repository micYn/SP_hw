/*
 * lookup1 : straight linear search through a local file
 * 	         of fixed length records. The file name is passed
 *	         as resource.
 */
#include <string.h>
#include <stdlib.h>
#include "dict.h"
#define BIGLINE 512


int lookup(Dictrec * sought, const char * resource) {
	Dictrec dr;
	static FILE * in;
	static int first_time = 1;
	char line[BIGLINE];

	if (first_time) { 
		first_time = 0;
		/* open up the file
		 *
		 * Fill in code. */
		in = fopen(resource,"r");
		if(in==NULL){
			perror("fopen");
			exit(EXIT_FAILURE);
		}
	}

	/* read from top of file, looking for match
	 *
	 * Fill in code. */

	rewind(in);
	while(fread(&dr, sizeof(Dictrec), 1, in) == 1) {
		/* Fill in code. */

	
		
		dr.word[strlen(dr.word)-1] = '\0';
		dr.text[strlen(dr.text)-1] = '\0';
	// 		printf("Word: %s\n", dr.word);
    // printf("Definition: %s\n", dr.text);


		if(strcmp(dr.word, sought->word)==0){

			strcpy(sought->text, dr.text);

			return FOUND;
		}
	}

	return NOTFOUND;
}
