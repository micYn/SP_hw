#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#include <errno.h>
void errorHandling(const char *str){
	perror(str);
	exit(EXIT_FAILURE);
}
int is_numeric(const char *str){
	int i;
	for(i=0;str[i]!='\0';i++){
		if(!isdigit(str[i])){
			return 0;
		}
	}
	return 1;
}
int main(int argc, char *argv[]){
	if(argc!=2){
		fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// open directory /proc
	DIR *proc_dir = opendir("/proc");
	if(proc_dir == NULL){
		errorHandling("opendir");
	}

	struct dirent *proc_entry;
	errno = 0;
	while((proc_entry = readdir(proc_dir))!=NULL){

		if(is_numeric(proc_entry->d_name)){

			//for each directories /proc/[PID], open its fd directory and loop through all of its entries:
			char path[512];
			snprintf(path, sizeof(path), "%s/%s/%s", "/proc", proc_entry->d_name, "fd");
			DIR *proc_pid_fd_dir = opendir(path);
			if(proc_pid_fd_dir==NULL){
				errorHandling("opendir");
			}

			errno=0;
			struct dirent *proc_pid_fd_entry;
			while((proc_pid_fd_entry = readdir(proc_pid_fd_dir))!=NULL){

				if (strcmp(proc_pid_fd_entry->d_name, ".") == 0 || strcmp(proc_pid_fd_entry->d_name, "..") == 0) {
			        continue; // Skip these entries
			    }

				//for each symbolic link, we use readlink to get the file pathname that the symbolic link points at:
				// printf("%s\n",proc_pid_fd_entry->d_name);
				char symboliclink[1024];
				snprintf(symboliclink, sizeof(symboliclink), "%s/%s/%s/%s", "/proc", proc_entry->d_name, "fd", proc_pid_fd_entry->d_name);
				// printf("%s\n", symboliclink);
				
				char filepathname[1024];
				int len;
				len = readlink(symboliclink, filepathname, sizeof(filepathname));
				if(len==-1){
					if (errno == ENOENT) {
			            // This error means the link no longer exists
			            printf("Warning: Symbolic link %s no longer exists.\n", symboliclink);
			            errno=0;
			            continue; // Skip this entry and continue
			        }
					errorHandling("readlink");
				} else if(len == 0){
					continue;
				}

				// readlink() does not append \0 at the end.
				filepathname[len] = '\0';

				if(strcmp(filepathname, argv[1])==0){
					printf("Process with pid:%s has %s opened.\n", proc_entry->d_name, filepathname);
				}
			}
			if(errno!=0){
				errorHandling("readdir");
			}
			if(closedir(proc_pid_fd_dir)==-1){
				errorHandling("closedir");
			}
		}

	}
	if(errno!=0){
		errorHandling("readdir");
	}
	if(closedir(proc_dir)==-1){
		errorHandling("closedir");
	}
	exit(EXIT_SUCCESS);
}