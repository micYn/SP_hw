#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
int main(){
	char cwd[PATH_MAX];	//PATH_MAX is defined in limits.h
	if(getcwd(cwd,sizeof(cwd)) == NULL){
		perror("printdir: getcwd");
		exit(EXIT_FAILURE);
	}
	printf("%s\n", cwd);

	return 0;
}