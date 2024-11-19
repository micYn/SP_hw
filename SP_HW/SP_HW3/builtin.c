/*
 * builtin.c : check for shell built-in commands
 * structure of file is
 * 1. definition of builtin functions
 * 2. lookup-table
 * 3. definition of is_builtin and do_builtin
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "shell.h"



/****************************************************************************/
/* builtin function definitions                                             */
/****************************************************************************/

/* "echo" command.  Does not print final <CR> if "-n" encountered. */
static void bi_echo(char **argv) {
  	/* Fill in code. */
  	int argc = 0;
  	while(argv[argc]!=NULL){
  		argc++;
  	}
  	if(argc==1){
  		fprintf(stderr,"Usage: %s [-n N] <strings>\n",argv[0]);
  		exit(EXIT_FAILURE);
  	}

  	if(strcmp(argv[1],"-n")==0){
  		int specified_number = atoi(argv[2]);
  		int strings_count = 0;
  		while(argv[strings_count+3]!=NULL){
  			strings_count++;
  		}
  		if(specified_number>=strings_count || specified_number<=0){
  			fprintf(stderr,"specified string doesn't exist\n");
  			// exit(EXIT_FAILURE);
  		}
  		else{
  			printf("%s\n", argv[specified_number+3-1]);
  			fflush(stdout);
  		}
  	}
  	else{
  		int idx = 1;
  		printf("%s",argv[idx++]);
  		while(argv[idx]!=NULL){
  			printf(" %s",argv[idx++]);
  		}
  		printf("\n");
  	}
}
/* Fill in code. */
static void my_exit(){
	exit(EXIT_SUCCESS);
}



/****************************************************************************/
/* lookup table                                                             */
/****************************************************************************/

static struct cmd {
	char * keyword;				/* When this field is argv[0] ... */
	void (* do_it)(char **);	/* ... this function is executed. */
} inbuilts[] = {

	{ "exit", my_exit},
	{ "quit", my_exit},
	{ "logout", my_exit},
	{ "bye", my_exit},
	{ "echo", bi_echo },		/* When "echo" is typed, bi_echo() executes.  */
	{ NULL, NULL }				/* NULL terminated. */
};




/****************************************************************************/
/* is_builtin and do_builtin                                                */
/****************************************************************************/

static struct cmd * this; 		/* close coupling between is_builtin & do_builtin */

/* Check to see if command is in the inbuilts table above.
Hold handle to it if it is. */
int is_builtin(char *cmd) {
  	struct cmd *tableCommand;

  	for (tableCommand = inbuilts ; tableCommand->keyword != NULL; tableCommand++)
    	if (strcmp(tableCommand->keyword,cmd) == 0) {
			this = tableCommand;
			return 1;
		}
  	return 0;
}


/* Execute the function corresponding to the builtin cmd found by is_builtin. */
int do_builtin(char **argv) {
  	this->do_it(argv);
}
