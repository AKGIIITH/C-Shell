#ifndef GLOBALS_H
#define GLOBALS_H

#include <sys/types.h>

#define MAX_PROCESSES 100
#define MAX_COMMAND_LENGTH 256
#define MAX_STATE_LENGTH 10

// Structure to store process details
typedef struct {
    pid_t pid;
    char command[MAX_COMMAND_LENGTH];
    char state[MAX_STATE_LENGTH];  // "Running" or "Stopped"
    char command_name[MAX_COMMAND_LENGTH];
} process_info;

// Extern declarations of global variables
extern char home[1024]; 
extern char previous_directory[1024];
extern size_t MAXPATH;
extern char normal[1024];
extern process_info process_list[MAX_PROCESSES];
extern int process_list_count;
extern pid_t fg_pid;

#endif  // GLOBALS_H
