#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include "proclore.h"

void get_process_info(pid_t pid) {
    char path[256];
    sprintf(path, "/proc/%d/status", pid);

    FILE *status_file;
    status_file = fopen(path, "r");
    if (status_file == NULL) {
        perror("fopen");
        return;
    }

    // char *process_status = NULL;
    int process_group = 0;
    int virtual_memory = 0;
    
    char exec_path[256];
    sprintf(exec_path, "/proc/%d/exe", pid);

    char line[256];
    char status[3];
    while (fgets(line, sizeof(line), status_file) != NULL) {
        if (strncmp(line, "State:", 6) == 0) {
            sscanf(line, "State:\t%c", &status[0]);
        }
        else if (strncmp(line, "Tgid:", 5) == 0) {
            process_group = atoi(strchr(line, ':') + 2);
        }
        else if (strncmp(line, "VmSize:", 7) == 0) {
            virtual_memory = atoi(strchr(line, ':') + 2);
        }
    }
    fclose(status_file);

    char resolved_path[256];
    ssize_t len = readlink(exec_path, resolved_path, sizeof(resolved_path) - 1);
    if (len != -1) {
        resolved_path[len] = '\0';
    } else {
        strcpy(resolved_path, "Unknown");
    }

                
    if (process_group == pid) {
        status[1]='+';
        status[2]='\0';
        //For Foreground Process GroupID is same as PID
    }

    printf("Pid : %d\n", pid);
    printf("Process status : %s\n",status);
    printf("Process Group : %d\n", process_group);
    printf("Virtual memory : %d\n", virtual_memory);
    printf("executable path : %s\n", resolved_path);
}

void proclore(int argc, char *argv[]) {
    pid_t pid;
    if (argc == 1) {
        pid = getpid();
    }
    else {
        pid = atoi(argv[1]);
    }
    get_process_info(pid);
}
