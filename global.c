#include "global.h"

// Initialization of global variables
char home[1024];
char previous_directory[1024];
size_t MAXPATH = 1024;
char normal[1024] = "NORMAL";

process_info process_list[MAX_PROCESSES]; // No need to initialize, but can set default values if needed
int process_list_count = 0;  // Proper initialization here

// pid_t fg_pid = -1;