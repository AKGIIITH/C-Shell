#include "utils.h"
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include "global.h"

void get_username(char *username) {
    struct passwd *pw;
    pw = getpwuid(getuid());
    strcpy(username, pw->pw_name);
}

void get_systemname(char *systemname) {
    struct utsname sysinfo;
    if(uname(&sysinfo)==0){
        strcpy(systemname, sysinfo.nodename);
    }
    else{
        perror("Uname is not working");
    }
    //uname function returns ustname which contains many system details
}

void get_current_directory(char *cwd,char* home) {
    getcwd(cwd, 1024);
    // returns absolute path
    relative_directory(cwd,home);
    //~/[-home/user](length)/address
}

void get_home_directory(char *home) {
    getcwd(home, 1024); // Assuming the shell's home directory is where it was invoked
}

char* trim_whitespace(char *str) {
    char *end;

    while(isspace((unsigned char)*str)){
        str++;//Shifts Pointer to next character
    }
    //Part of ctype.h to check whether a character is a string or not

    if(*str == 0) 
        return NULL;
    //No Charcters apart form Spaces

    end = str + strlen(str) - 1;//Pointer to last Character
    while(end > str && isspace((unsigned char)*end)){
        end--;
    }
    *(end+1) = '\0';
    // Write new null terminator character
    return str;
}

void relative_directory(char *path,char* home) {
    if (strncmp(path, home, strlen(home)) == 0) {
        path[0] = '~';
        strcpy(path + 1, path + strlen(home));
    }
}

void resolve_path(char *arg, char *path) {
    if (strcmp(arg, "~") == 0) {
        strcpy(path, home);
    }
    else if (strcmp(arg, "-") == 0) {
        if (strlen(previous_directory) > 0) {
            strcpy(path,previous_directory);
        }
        else {
            fprintf(stderr, "No previous directory to hop to\n");
            path[0] = '\0'; // Return an empty string to indicate an error
        }
    }
    else if (arg[0] == '/') {
        // Absolute path
        strcpy(path, arg);
    }
    else if (strncmp(arg, "~/", 2) == 0) {
        // Path from home directory
        snprintf(path, MAXPATH, "%s/%s", home, arg + 2);
    }
    else if (strcmp(arg, "..") == 0) {
        // Parent directory
        char *cwd = getcwd(NULL, 0);
        if (cwd == NULL) {
            perror("getcwd failed");
        }
        char *last_slash = strrchr(cwd, '/');
        if (last_slash != NULL) {
            *last_slash = '\0';
            strcpy(path, cwd);
        } else {
            strcpy(path, "/"); // Root directory
        }
    }
    else {
        // Relative path
        snprintf(path, MAXPATH, "%s/%s", getcwd(NULL, 0), arg);
    }
}

void update_process_list() {
    for (int i = 0; i < process_list_count; i++) {
        if (strcmp(process_list[i].state, "Running") == 0) {
            int status;
            pid_t result = waitpid(process_list[i].pid, &status, WNOHANG);
            
            if (result > 0) {
                if (WIFEXITED(status) || WIFSIGNALED(status)) {
                    // The process has exited or been killed, update the state to "Stopped"
                    printf("%s with pid %d has exited normally\n", process_list[i].command_name, process_list[i].pid);
                    strcpy(process_list[i].state, "Stopped");
                }
            }
            // If result == -1, there was an error
            else if (result == -1) {
                // perror("Either Process does not exist");
                strcpy(process_list[i].state, "Stopped");
            }
        }
    }
}

