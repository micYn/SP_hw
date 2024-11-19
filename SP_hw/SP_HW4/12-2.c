#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

#include <errno.h>
void errorHandling(const char *str){
	perror(str);
	exit(EXIT_FAILURE);
}

typedef struct Process{
	int pid;
	int ppid;
	char name[256];
} Process;

int is_numeric(const char *str){
	int i=0;
	for(i=0;str[i]!='\0';i++){
		if(!isdigit(str[i])){
			return 0;
		}
	}
	return 1;
}
int get_process_info(const char *pid, Process *proc){
	char path[256];
	snprintf(path, sizeof(path), "%s/%s/%s", "/proc", pid, "status");
	FILE *file = fopen(path, "r");
	if(file==NULL){
		return -1;
	}

	char buffer[256];
	char key[32];
	proc->pid = atoi(pid);
	proc->ppid = -1;
	strcpy(proc->name, "[Unknown]");
	while(fgets(buffer, sizeof(buffer), file)){
		sscanf(buffer, "%31s", key);
		if(strcmp(key, "PPid:")==0){
			sscanf(buffer, "%*s %d", &(proc->ppid));
		}
		if(strcmp(key, "Name:")==0){
			sscanf(buffer, "%*s %s", proc->name);
		}
	}
	fclose(file);
	return 0;
}
void print_tree(int pid, int level, Process *processes, int process_cnt){
	int i;
	for(i=0;i<level;i++){
		printf("\t\t");
	}
	for(i=0;i<process_cnt;i++){
		if(processes[i].pid==pid){
			// print itself first
			printf("%d [%s]\n", processes[i].pid, processes[i].name);

			// print all of its children:
			int j;
			for (j=0;j<process_cnt;j++) {
                if (processes[j].ppid == pid) {  // Correct: child processes have PPid equal to current PID
                    print_tree(processes[j].pid, level + 1, processes, process_cnt);
                }
            }
		}

	}
}
int main(){
	DIR *proc_dir = opendir("/proc");
	if(proc_dir == NULL){
		errorHandling("opendir");
	}

	struct dirent *entry;
	Process processes[1024];
	int process_cnt = 0;

	//1. iterate through all /proc/[PID] directories
	errno = 0;
	while((entry=readdir(proc_dir)) != NULL){
		if(is_numeric(entry->d_name)){

			//2. go into /proc/[PID]/status file for each process.
			//   and store the information of the process into "processes"
			Process proc;
			if(get_process_info(entry->d_name, &proc) == 0){
				// Add a process entry in the array
				processes[process_cnt++] = proc;
			}

		}
	}
	if(errno!=0){
		errorHandling("readdir");
	}
	if(closedir(proc_dir)==-1){
		errorHandling("closedir");
	}
	
	//3. print processes in a recursive way. From PID 1 (init)
	printf("Process tree:\n");
	print_tree(1,0,processes, process_cnt);

}