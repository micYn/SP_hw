/*
 * parse.c : use whitespace to tokenise a line
 * Initialise a vector big enough
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "shell.h"

/* Parse a commandline string into an argv array. */
char ** parse(char *line) {

  	static char delim[] = " \t\n"; /* SPACE or TAB or NL */
  	int count = 1;
  	char * token;
  	char **newArgv;

  	/* Nothing entered. */
  	if (line == NULL || strcmp(line,"\n")==0) {
    	return NULL;
  	}

  	/* Init strtok with commandline, then get first token.
     * Return NULL if no tokens in line.
	 *
	 * Fill in code.
     */
	token = strtok(line, delim);
	if (token == NULL) {
    		return NULL;
  	}


  	/* Create array with room for first token.
  	 *
	 * Fill in code.
	 */
	newArgv = (char**)realloc(NULL, count*sizeof(char*));
     if(newArgv==NULL){
     	perror("realloc");
     	exit(EXIT_FAILURE);
     }
     newArgv[count-1] = (char*)malloc((strlen(token) + 1) * sizeof(char));
	if (newArgv[count-1] == NULL) {
	    perror("malloc");
	    exit(EXIT_FAILURE);
	}
     strcpy(newArgv[count-1], token);


  	/* While there are more tokens...
	 *
	 *  - Get next token.
	 *	- Resize array.
	 *  - Give token its own memory, then install it.
	 * 
  	 * Fill in code.
	 */
	token = strtok(NULL, delim);
     while(token!=NULL){
     	count++;
     	newArgv = (char**)realloc(newArgv,count*sizeof(char*));
     	if(newArgv==NULL){
     		perror("realloc");
     		exit(EXIT_FAILURE);
     	}
     	newArgv[count-1] = (char*)malloc((strlen(token) + 1) * sizeof(char));
		if (newArgv[count-1] == NULL) {
		    perror("malloc");
		    exit(EXIT_FAILURE);
		}
     	strcpy(newArgv[count-1], token);
     	token = strtok(NULL, delim);
     }

     count++;
     newArgv = (char**)realloc(newArgv,count*sizeof(char*));
     if(newArgv==NULL){
     	perror("realloc");
     	exit(EXIT_FAILURE);
     }
     newArgv[count-1] = NULL;

     // int i;
     // for(i=0;i<count-1;i++){
     // 	printf("[%d] : %s\n",i,newArgv[i]);
     // }

  	/* Null terminate the array and return it.
	 *
  	 * Fill in code.
	 */

  	return newArgv;
}


/*
 * Free memory associated with argv array passed in.
 * Argv array is assumed created with parse() above.
 */
void free_argv(char **oldArgv) {

	int i = 0;

	/* Free each string hanging off the array.
	 * Free the oldArgv array itself.
	 *
	 * Fill in code.
	 */
	for(i=0;oldArgv[i]!=NULL;i++){
		free(oldArgv[i]);
	}
	free(oldArgv);

}
