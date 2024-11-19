#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>

#include <errno.h>

int main(){
	
	// gethostname()
	char hostname[256];
	if(gethostname(hostname, sizeof(hostname)) == -1){
		perror("hostinfo: gethostname");
		exit(EXIT_FAILURE);
	}

	// uname()
	struct utsname buf;
	if(uname(&buf)==-1){
		perror("hostinfo: uname");
		exit(EXIT_FAILURE);
	}

	// gethostid()
	long hostid = gethostid();
	if(hostid == -1){
		perror("hostinfo: gethostid");
		exit(EXIT_FAILURE);
	}

	printf("hostname: %s\n%s\nhostid: %ld\n", hostname, buf.release, hostid);

	return 0;
}