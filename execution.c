#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <errno.h>
#include <time.h>
#include "global.h"
#include "utils.h"
#include "hop.h"
#include "reveal.h"
#include "log.h"
#include "display.h"
#include "proclore.h"
#include "seek.h"
#include "alias.h"
#include "display_activity.h"
#include "signal_handler.h"
#include "iMan.h"

#define MAX_ARGS 10
#define MAX_PIPES 10

extern process_info process_list[MAX_PROCESSES];
extern int process_list_count;
extern pid_t fg_pid;
extern char normal[1024];

int handle_redirection(char *argv[], int *input_fd, int *output_fd) {
    int i = 0;
    while (argv[i] != NULL) {
        if (strcmp(argv[i], ">") == 0) {
            *output_fd = open(argv[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (*output_fd < 0) {
                perror("Error opening output file for redirection with >");
                return -1;
            }
            argv[i] = NULL;  // Nullify redirection symbol
        }
        // Output redirection with append ">>"
        else if (strcmp(argv[i], ">>") == 0) {
            *output_fd = open(argv[i + 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
            if (*output_fd < 0) {
                perror("Error opening output file for redirection with >>");
                return -1;
            }
            argv[i] = NULL;  
        }
        // Input redirection "<"
        else if (strcmp(argv[i], "<") == 0) {
            *input_fd = open(argv[i + 1], O_RDONLY);
            if (*input_fd < 0) {
                printf("No such input file found!\n");
                return -1;
            }
            argv[i] = NULL; 
        }
        i++;
    }
    return 0;
}

void execute_command(char *command,int input_fd,int output_fd,int is_background) {
    char *argv[MAX_ARGS];
    int argc = 0;
    
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);    
    // int input_fd = -1, output_fd = -1; //Flags to check which redirection is used

    char* command_copy = strdup(command); //Copy of the command  
    char *token = strtok(command," ");
    
    char *alias_copy = strdup(token); 
    replace_alias(alias_copy);
    
    while (token != NULL  && argc < MAX_ARGS - 1) {
        // printf("%s\n",token);
        argv[argc++] = strdup(token);
        token = strtok(NULL, " ");
    }
    // argv[0]=alias_copy;
    argv[argc] = NULL;
    
    if (handle_redirection(argv, &input_fd, &output_fd) == -1) {
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
        return;
    }
    
    int i=0;
    while (argv[i]!=NULL){
        i++;
    }
    argc=i;
    if (input_fd != -1) {
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }
    if (output_fd != -1) {
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }
    // printf("%d %d\n",STDIN_FILENO,STDOUT_FILENO);
    
    if (strncmp(argv[0], "reveal", 6) == 0) {
        reveal(argc, argv);
    } 
    else if (strncmp(argv[0], "hop", 3) == 0) {
        hop(argc, argv);
    }
    else if (strncmp(argv[0], "log", 3) == 0) {
        log_command(argc, argv);
    }
    else if (strncmp(argv[0], "proclore", 8) == 0) {
        proclore(argc, argv);
    }
    else if (strncmp(argv[0], "seek", 4) == 0) {
        seek(argc, argv);
    }
    else if (strncmp(argv[0], "activities",10)==0){
        printf("%d\n", process_list_count);
        display_activities(process_list,process_list_count);
    }
    else if (strcmp(argv[0], "ping") == 0) {
        handle_ping(argv);
    } 
    else if (strcmp(argv[0], "exit") == 0) {
        exit(EXIT_SUCCESS);
    }
    else if (strncmp(argv[0],"iMAN",4)==0){
        iMan(argv[1]);
    }
    else {                     
        if (is_background) {
            pid_t bpid = fork();
            if (bpid == -1) {
                perror("fork");
                return;
            }
            if (bpid>0){
                strcpy(process_list[process_list_count].command, command_copy);
                strcpy(process_list[process_list_count].command_name, argv[0]);
                strcpy(process_list[process_list_count].state, "Running");
                process_list[process_list_count].pid = bpid;
                process_list_count++;

                dup2(saved_stdin, STDIN_FILENO);
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdin);
                close(saved_stdout);
                return;
            }
            else{
                printf("PID: %d\n", getpid());
                if (execvp(argv[0], argv) == -1) {
                    perror("execvp");
                    exit(EXIT_FAILURE);
                }
            } 
        }

        else{
            pid_t pid = fork();
            if (pid == -1) {
                perror("fork");
                return;
            }
            if (pid == 0) {
                if (execvp(argv[0], argv) == -1) {
                    perror("execvp");
                    exit(EXIT_FAILURE);
                }
            }
            else { // Parent process
                fg_pid = pid;
                time_t start_time = time(NULL);
                // Wait for the foreground process to finish
                int status;
                waitpid(pid, &status, 0);
                
                process_list[process_list_count].pid = pid;
                strcpy(process_list[process_list_count].state, "Stopped"); //Handles Activities
                process_list_count++;
                
                time_t end_time = time(NULL);
                int elapsed_time = (int) difftime(end_time, start_time);
                if (elapsed_time > 2) {
                    char elapsed_info[50];
                    snprintf(elapsed_info, sizeof(elapsed_info), " %s : %ds",argv[0], elapsed_time);
                    strcpy(normal,elapsed_info);
                }
            }
        }
    }
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
    return;
}

void execute_commands_piped(char *commands[], int num_commands, int is_background) {
    int pipe_fds[2 * (num_commands - 1)]; // Array to store pipe file descriptors
    
    // Create pipes for each command except the last one
    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipe_fds + i * 2) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Loop through each command
    for (int i = 0; i < num_commands; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        
        if (pid == 0) { // Child process
            // If not the first command, set input redirection from previous pipe
            if (i > 0) {
                dup2(pipe_fds[(i - 1) * 2], STDIN_FILENO); // Redirect stdin from the previous pipe's read end
            }
            // If not the last command, set output redirection to the current pipe
            if (i < num_commands - 1) {
                dup2(pipe_fds[i * 2 + 1], STDOUT_FILENO); // Redirect stdout to the current pipe's write end
            }

            // Close all pipe file descriptors
            for (int j = 0; j < 2 * (num_commands - 1); j++) {
                close(pipe_fds[j]);
            }

            // Execute the command
            execute_command(commands[i], -1, -1, is_background); 
            exit(EXIT_FAILURE); // Exit child process after executing the command
        }
    }

    // Parent process closes all pipe file descriptors
    for (int i = 0; i < 2 * (num_commands - 1); i++) {
        close(pipe_fds[i]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_commands; i++) {
        wait(NULL);
    }
}


void execute_commands(char *foreground_commands[], int num_foreground,char *background_commands[], int num_background) {
    for (int i = 0; i < num_foreground; i++) {
        char *command = foreground_commands[i];
        // printf("Executing foreground command: %s\n", command);
        char* piped_command = strtok(command,"|");
        char* piped_commands[MAX_PIPES];
        int number_piped_commands=0;
        while (piped_command != NULL) {
            piped_commands[number_piped_commands] = strdup(trim_whitespace(piped_command));
            number_piped_commands++;
            piped_command = strtok(NULL, "|");
        }
        execute_commands_piped(piped_commands,number_piped_commands,0);
        // execute_command(command,0);
    }
    for (int i = 0; i < num_background; i++) {
        char *command = background_commands[i];
        // printf("Executing background command: %s\n", command);
        char* piped_command = strtok(command,"|");
        char* piped_commands[MAX_PIPES];
        int number_piped_commands=0;
        while (piped_command != NULL) {
            piped_commands[number_piped_commands] = strdup(trim_whitespace(piped_command));
            number_piped_commands++;
            piped_command = strtok(NULL, "|");
        }
        execute_commands_piped(piped_commands,number_piped_commands,1);
        // execute_command(command,1);
    }
}

