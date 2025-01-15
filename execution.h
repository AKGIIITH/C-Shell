#ifndef EXECUTION_H
#define EXECUTION_H

int handle_redirection(char *argv[], int *input_fd, int *output_fd);
void execute_command(char *command, int input_fd, int output_fd, int is_background);
void execute_commands_piped(char *commands[], int num_commands,int is_background);
void execute_commands(char *foreground_commands[], int num_foreground,char *background_commands[], int num_background);

#endif

