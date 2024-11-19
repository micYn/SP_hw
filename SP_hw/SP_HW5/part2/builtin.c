/*
 * builtin.c : check for shell built-in commands
 * structure of file is
 * 1. definition of builtin functions
 * 2. lookup-table
 * 3. definition of is_builtin and do_builtin
*/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/utsname.h>
#include "shell.h"

/****************************************************************************/
/* builtin function definitions                                             */
/****************************************************************************/
static void bi_builtin(char ** argv);	/* "builtin" command tells whether a command is builtin or not. */
static void bi_cd(char **argv) ;		/* "cd" command. */
static void bi_echo(char **argv);		/* "echo" command.  Does not print final <CR> if "-n" encountered. */
static void bi_hostname(char ** argv);	/* "hostname" command. */
static void bi_id(char ** argv);		/* "id" command shows user and group of this process. */
static void bi_pwd(char ** argv);		/* "pwd" command. */
static void bi_quit(char **argv);		/* quit/exit/logout/bye command. */




/****************************************************************************/
/* lookup table                                                             */
/****************************************************************************/

static struct cmd {
  	char * keyword;					/* When this field is argv[0] ... */
  	void (* do_it)(char **);		/* ... this function is executed. */
} inbuilts[] = {
  	{ "builtin",    bi_builtin },   /* List of (argv[0], function) pairs. */

    /* Fill in code. */
    { "echo",       bi_echo },
    { "quit",       bi_quit },
    { "exit",       bi_quit },
    { "bye",        bi_quit },
    { "logout",     bi_quit },
    { "cd",         bi_cd },
    { "pwd",        bi_pwd },
    { "id",         bi_id },
    { "hostname",   bi_hostname },
    {  NULL,        NULL }          /* NULL terminated. */
};


static void bi_builtin(char ** argv) {
	/* Fill in code. */
    int i = 0;
    for(i = 0; i < 10; i++){
        if(strcmp(argv[1], inbuilts[i].keyword) == 0){
            printf("%s is a builtin feature.\n", argv[1]);
            return;
        }
    }
    printf("%s is NOT a builtin feature.\n", argv[1]);
}

static void bi_cd(char **argv) {
	/* Fill in code. */
    if(chdir(argv[1]) == -1){
        perror("chdir");
        exit(EXIT_FAILURE);
    }
}

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
        if(specified_number>strings_count){
            fprintf(stderr,"specified string doesn't exist\n");
            exit(EXIT_FAILURE);
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

static void bi_hostname(char ** argv) {
	/* Fill in code. */
    char hostname[512];
    if(gethostname(hostname, sizeof(hostname))==-1){
        perror("gethostname");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", hostname);
}

static void bi_id(char ** argv) {
 	/* Fill in code. */
    uid_t uid = getuid();   // never fail
    struct passwd *pw = getpwuid(uid);
    if(pw==NULL){
        perror("getpwuid");
        exit(EXIT_FAILURE);
    }

    gid_t gid = pw->pw_gid;
    struct group *gr = getgrgid(gid);
    if(gr==NULL){
        perror("getgrgid");
        exit(EXIT_FAILURE);
    }

    printf("UserID = %d(%s), GroupID = %d(%s)\n",uid,pw->pw_name,gid,gr->gr_name);
}

static void bi_pwd(char ** argv) {
	/* Fill in code. */
    char cwd[PATH_MAX]; //PATH_MAX is defined in limits.h
    if(getcwd(cwd,sizeof(cwd)) == NULL){
        perror("getcwd");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", cwd);
}

static void bi_quit(char **argv) {
	exit(0);
}


/****************************************************************************/
/* is_builtin and do_builtin                                                */
/****************************************************************************/

static struct cmd * this; /* close coupling between is_builtin & do_builtin */

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
