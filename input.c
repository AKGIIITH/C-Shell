#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "input.h"
#include "utils.h"
#include "execution.h"
#include "log.h"
#include "alias.h"

#define MAX_COMMANDS 100

void handle_input(char *input) {
    input=trim_whitespace(input);
    // add_to_log(input);

    char *foreground_commands[MAX_COMMANDS];
    int num_foreground=0;
    char *background_commands[MAX_COMMANDS];
    int num_background=0;

    // Tokenize the input by ';'
    char *command;
    command = strtok(input, ";");
    
    while (command != NULL) {
        // if(handle_function(command)==1){ // It is a Function
        //     continue;
        // }
        command=trim_whitespace(command);
        if (strlen(command) > 0) {
            // char *sub_command;
            char *ampersand;
            while ((ampersand = strchr(command, '&')) != NULL) {
                *ampersand = '\0';
                command=trim_whitespace(command); // Trim trailing spaces
                if (strlen(command) > 0) {
                    background_commands[num_background++] = strdup(command);
                }
                command = ampersand+1; // Move past '&'
            }
            command=trim_whitespace(command);
            if (command!=NULL && strlen(command) > 0) {
                foreground_commands[num_foreground++] = strdup(command);
            }
        }
        command = strtok(NULL, ";");
    }

    // printf("Foreground Commands:\n");
    // for (int i = 0; i < num_foreground; i++) {
    //     // foreground_commands[i]=trim_whitespace(foreground_commands[i]);
    //     printf("%s\n", foreground_commands[i]);
    // }

    // printf("\nBackground Commands:\n");
    // for (int i = 0; i < num_background; i++) {
    //     // background_commands[i]=trim_whitespace(background_commands[i]);
    //     printf("%s\n", background_commands[i]);
    // }
    execute_commands(foreground_commands,num_foreground,background_commands,num_background);    
}
