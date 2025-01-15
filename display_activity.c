// display_activity.c

#include "display_activity.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Comparator function to sort process list by command name
int compare_process(const void *a, const void *b) {
    process_info *processA = (process_info *)a;
    process_info *processB = (process_info *)b;
    return (processB->pid - processA->pid);
}

// Function to display all activities in lexicographic order
void display_activities(process_info process_list[], int process_list_count) {
    // Sort processes by PID
    qsort(process_list, process_list_count, sizeof(process_info), compare_process);
    
    // Print each process with its PID and state
    printf("%d\n", process_list_count);
    for (int i = 0; i < process_list_count; i++) {
        printf("[%d] : %s - %s\n", process_list[i].pid, process_list[i].command_name, process_list[i].state);
    }
}
