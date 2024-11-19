#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <dirent.h>
#include <pwd.h>

#include <unistd.h>
#include <sys/types.h>

#include <errno.h>

uid_t get_user_id(const char *username) {
    struct passwd *pw = getpwnam(username);
    if (pw == NULL) {
        fprintf(stderr, "Error: user %s not found.\n", username);
        exit(EXIT_FAILURE);
    }
    return pw->pw_uid;
}

int is_numeric(const char *str) {
    int i;
    for (i=0;str[i]!='\0';i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

uid_t get_process_uid(const char *pid) {
    char path[256];
    snprintf(path, sizeof(path), "%s/%s/%s", "/proc", pid, "status");

    char buffer[256];
    char key[32];
    uid_t uid = -1;
    FILE *file = fopen(path, "r");
    if (file==NULL) {
        return -1;
    }
    
    // read status line by line, and check if the line begins with "Uid:"
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%31s", key);
        if (strcmp(key, "Uid:") == 0) {
            sscanf(buffer, "%*s %u", &uid);
            break;
        }
    }

    fclose(file);
    return uid;
}


void get_process_cmdname(const char *pid, char *cmdname, size_t size) {
    char path[256];
    snprintf(path, sizeof(path), "%s/%s/%s", "/proc", pid, "status");

    char buffer[256];
    char key[32];
    FILE *file = fopen(path, "r");
    if (file==NULL) {
        strcpy(cmdname, "[Unknown]");
        return;
    }
    // read status line by line, and check if the line begins with "Name:"
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%31s", key);
        if (strcmp(key, "Name:") == 0) {
            sscanf(buffer, "%*s %s", cmdname);
            break;
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <username>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // For each user:
    int user_cnt;
    for(user_cnt=1;user_cnt<argc;user_cnt++){
        printf("User: %s\n", argv[user_cnt]);

        // Get the UID of the user
        const char *username = argv[user_cnt];
        uid_t user_id = get_user_id(username);

        // Open /proc directory
        DIR *proc_dir = opendir("/proc");
        if (proc_dir==NULL) {
            perror("opendir");
            exit(EXIT_FAILURE);
        }

        struct dirent *entry;
        errno=0;
        while ((entry = readdir(proc_dir)) != NULL) {
            // Only consider numeric directories: /proc/[PID]
            if (is_numeric(entry->d_name)) {

                // Get the UID of the process (by looking into "status")
                uid_t proc_uid = get_process_uid(entry->d_name);

                if (proc_uid == user_id) {

                    // Get the command name of the process (by looking into "status")
                    char cmdname[256];
                    get_process_cmdname(entry->d_name, cmdname, sizeof(cmdname));

                    // Print the process ID and command name
                    printf("PID: %s, CMD NAME: %s\n", entry->d_name, cmdname);
                }
            }
        }
        if(errno!=0){
            perror("readdir");
            exit(EXIT_FAILURE);
        }

        closedir(proc_dir);
        printf("-------------------------\n");
    }
    
    return 0;
}
