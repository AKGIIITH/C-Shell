#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "input.h" 
#include "utils.h"

#define MAX_ALIASES 100
#define MAX_FUNCTIONS 100

typedef struct {
    char alias[50];
    char command[200];
} Alias;

typedef struct {
    char name[256];  // The command name, e.g., "mkdir"
    int input_indices[10];  // Indices of inputs, e.g., $1, $2, etc.
    int input_count;  // Number of inputs to the command
} Command;

typedef struct {
    char function[50];
    Command commands[10]; // Support for multiple commands in a function
    int command_count;
} Function;

Alias aliases[MAX_ALIASES];
int alias_count = 0;

Function functions[MAX_FUNCTIONS];
int function_count = 0;

void load_myshrc() {
    FILE *file = fopen(".myshrc", "r");
    if (!file) {
        perror("Error opening .myshrc");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Check for alias definitions
        if (strncmp(line, "alias ", 6) == 0) {
            char *alias_name = strtok(line + 6, " =");
            char *alias_command = strtok(NULL, "\n");
            alias_command =alias_command+1;

            strcpy(aliases[alias_count].alias, alias_name);
            strcpy(aliases[alias_count].command, alias_command);
            alias_count++;
        }
        // Check for function definitions
        else if (strncmp(line, "func ", 5) == 0) {
            char *func_name = strtok(line+5, "(");
            strcpy(functions[function_count].function, func_name);

            // Reading the next lines for function commands
            char ch; 
            while ((ch = fgetc(file)) != EOF && ch != '{');

            functions[function_count].command_count = 0;
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}") != NULL) {
                    break;
                }
                if (strlen(line) == 0) {
                    continue;
                }
                Command command;
                char *command_name = strtok(line, " ");

                if(command_name != NULL) {
                    strcpy(command.name, command_name);

                    // Parsing Arguments
                    int arg_index = 0;
                    char *arg = strtok(NULL, " ");
                    command.input_count = 0;
                    while (arg != NULL) {
                        if (arg[0] == '$') {  // If it's an input placeholder like $1
                            command.input_indices[command.input_count] = atoi(arg + 1);  // Store the index (1 for $1, 2 for $2, etc.)
                            command.input_count++;
                        }
                        arg = strtok(NULL, " ");
                    }

                    // Add the command to the function
                    functions[function_count].commands[functions[function_count].command_count++] = command;
                }
            }
            function_count++;
        }
    }
    for (int i = 0; i < alias_count; i++) {
        printf("Alias: %s -> %s\n", aliases[i].alias, aliases[i].command);
    }
    for (int i = 0; i < function_count; i++) {
        printf("Function: %s\n", functions[i].function);
        for (int j = 0; j < functions[i].command_count; j++) {
            Command *cmd = &functions[i].commands[j];
            printf("Command: %s\n", cmd->name);
            for (int k = 0; k < cmd->input_count; k++) {
                printf("Input %d: $%d\n", k, cmd->input_indices[k]);
            }
        }
    }

    fclose(file);
}

int replace_alias(char *input) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(input, aliases[i].alias) == 0) {
            strcpy(input, aliases[i].command);
            // printf("Alias replaced: %s\n", input);
            return 1;
        }
    }
    return 0;
}

int handle_function(char *input_string) {
    char *args[10];  // For storing arguments
    int argc = 0;

    // Tokenize the input string into command and arguments
    char *token = strtok(input_string, " ");
    char command_name[64];
    strcpy(command_name, token);  // The first token is the command (e.g., "mk_hop")
    
    // Store the rest of the arguments
    while ((token = strtok(NULL, " ")) != NULL) {
        args[argc++] = token;
    }

    // Check if command_name matches any function defined in .myshrc
    for (int i = 0; i < function_count; i++) {
        if (strcmp(functions[i].function, command_name) == 0) { // Function
            for (int j = 0; j < functions[i].command_count; j++) {
                Command *cmd = &functions[i].commands[j];
                char command_to_execute[512] = {0};
                strcpy(command_to_execute, cmd->name);  // Start with the command name
                
                // Append the inputs, replacing $1, $2, etc., with actual arguments
                for (int k = 0; k < cmd->input_count; k++) {
                    int input_index = cmd->input_indices[k];
                    if (input_index <= argc) {
                        strcat(command_to_execute, " ");
                        strcat(command_to_execute, args[input_index - 1]);  // $1 is args[0]
                    }
                }

                // Pass the constructed command to handle_input
                printf("Executing: %s\n", command_to_execute);
                handle_input(command_to_execute);
            }
            return 1;  // Exit after executing the function
        }
    }
    return 0; // Not a function
}




