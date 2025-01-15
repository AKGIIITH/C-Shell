// hop.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "utils.h"
#include "global.h"

#define MAX_PATH_LENGTH 1024

// Function to change directory and print the current working directory
void change_directory(const char *path) {
    if (chdir(path) != 0) {
        perror("Error changing directory");
    }
    else {
        char cwd[MAX_PATH_LENGTH];
        getcwd(cwd, sizeof(cwd));
        // relative_directory(cwd,home);
        printf("%s\n", cwd);
    }
}

// Function to handle the hop command
void hop(int argc, char *argv[]) {

    for (int i = 1; i < argc; i++) {
        char path[MAX_PATH_LENGTH];
        // if (strcmp(argv[i], "~") == 0) {
        //     strcpy(path, home);
        // }

        // else if (strcmp(argv[i], "-") == 0) {
        //     if (strlen(previous_directory) > 0) {
        //         strcpy(path, previous_directory);
        //     }
        //     else {
        //         fprintf(stderr, "No previous directory to hop to\n");
        //         continue;
        //     }
        //     // My Previous_directory will alway contain 
        // }

        // else if (argv[i][0] == '/') {
        //     // Absolute path
        //     strcpy(path, argv[i]);
        // }
        
        // else if (strncmp(argv[i], "~/", 2) == 0) {
        //     // Path from home directory
        //     snprintf(path, sizeof(path), "%s/%s", home, argv[i] + 2);
        // }

        // else if (strcmp(argv[i], "..") == 0) {
        //     // Parent directory
        //     char *cwd = getcwd(NULL, 0);
        //     if (cwd == NULL) {
        //         perror("getcwd failed");
        //         continue;
        //     }
        //     char *last_slash = strrchr(cwd, '/');
        //     if (last_slash != NULL) {
        //         *last_slash = '\0';
        //         strcpy(path, cwd);
        //     } else {
        //         strcpy(path, "/"); // Root directory
        //     }
        // }

        // else {
        //     snprintf(path, sizeof(path), "%s/%s", getcwd(NULL, 0), argv[i]);
        // }
        resolve_path(argv[i],path);
        // printf("Resolved path: %s\n", path);

        strcpy(previous_directory,getcwd(NULL,0));
        
        trim_whitespace(path);
        change_directory(path);
    }
}
